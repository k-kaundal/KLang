#include "plugin_system.h"
#include <stdlib.h>
#include <string.h>

/* Platform-specific includes for directory operations */
#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#else
    #include <dirent.h>
    #include <sys/stat.h>
#endif

/* Platform-specific dynamic loading wrappers */
#ifdef _WIN32
    #define PLUGIN_EXTENSION ".dll"
    
    static PluginHandle plugin_dlopen(const char *path) {
        return LoadLibraryA(path);
    }
    
    static void* plugin_dlsym(PluginHandle handle, const char *symbol) {
        return (void*)GetProcAddress(handle, symbol);
    }
    
    static int plugin_dlclose(PluginHandle handle) {
        return FreeLibrary(handle) ? 0 : -1;
    }
    
    static const char* plugin_dlerror(void) {
        static char error_buf[256];
        DWORD error = GetLastError();
        if (error == 0) return NULL;
        
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, error, 0, error_buf, sizeof(error_buf), NULL);
        return error_buf;
    }
#else
    #define PLUGIN_EXTENSION ".so"
    
    static PluginHandle plugin_dlopen(const char *path) {
        return dlopen(path, RTLD_LAZY);
    }
    
    static void* plugin_dlsym(PluginHandle handle, const char *symbol) {
        return dlsym(handle, symbol);
    }
    
    static int plugin_dlclose(PluginHandle handle) {
        return dlclose(handle);
    }
    
    static const char* plugin_dlerror(void) {
        return dlerror();
    }
#endif

/* Create plugin manager */
PluginManager* plugin_manager_create(Interpreter *interp) {
    PluginManager *pm = malloc(sizeof(PluginManager));
    pm->plugins = NULL;
    pm->plugin_count = 0;
    pm->plugin_directory = strdup("./plugins");
    pm->interp = interp;
    return pm;
}

/* Destroy plugin manager */
void plugin_manager_destroy(PluginManager *pm) {
    if (!pm) return;
    
    /* Unload all plugins */
    Plugin *plugin = pm->plugins;
    while (plugin) {
        Plugin *next = plugin->next;
        plugin_unload(plugin);
        plugin = next;
    }
    
    free(pm->plugin_directory);
    free(pm);
}

/* Set plugin directory */
void plugin_manager_set_directory(PluginManager *pm, const char *dir) {
    if (!pm) return;
    free(pm->plugin_directory);
    pm->plugin_directory = strdup(dir);
}

/* Load plugin from path */
Plugin* plugin_load(const char *path) {
    /* Open dynamic library */
    PluginHandle handle = plugin_dlopen(path);
    if (!handle) {
        fprintf(stderr, "Failed to load plugin %s: %s\n", path, plugin_dlerror());
        return NULL;
    }
    
    /* Get metadata */
    PluginMetadata *metadata = (PluginMetadata*)plugin_dlsym(handle, "klang_plugin_metadata");
    if (!metadata) {
        fprintf(stderr, "Plugin %s missing metadata\n", path);
        plugin_dlclose(handle);
        return NULL;
    }
    
    /* Check API version */
    if (metadata->api_version != PLUGIN_API_VERSION) {
        fprintf(stderr, "Plugin %s has incompatible API version %d (expected %d)\n",
                path, metadata->api_version, PLUGIN_API_VERSION);
        plugin_dlclose(handle);
        return NULL;
    }
    
    /* Create plugin structure */
    Plugin *plugin = malloc(sizeof(Plugin));
    plugin->handle = handle;
    plugin->metadata = *metadata;
    plugin->metadata.name = strdup(metadata->name);
    plugin->metadata.version = strdup(metadata->version);
    plugin->metadata.author = strdup(metadata->author);
    plugin->metadata.description = strdup(metadata->description);
    
    /* Load callback functions */
    plugin->init = (int (*)(Plugin*, Interpreter*))plugin_dlsym(handle, "klang_plugin_init");
    plugin->cleanup = (void (*)(Plugin*))plugin_dlsym(handle, "klang_plugin_cleanup");
    plugin->register_functions = (int (*)(Plugin*, Interpreter*))plugin_dlsym(handle, "klang_plugin_register_functions");
    plugin->transform_ast = (ASTNode* (*)(Plugin*, ASTNode*))plugin_dlsym(handle, "klang_plugin_transform_ast");
    
    plugin->user_data = NULL;
    plugin->next = NULL;
    
    return plugin;
}

/* Unload plugin */
void plugin_unload(Plugin *plugin) {
    if (!plugin) return;
    
    /* Cleanup */
    if (plugin->cleanup) {
        plugin->cleanup(plugin);
    }
    
    /* Close library */
    if (plugin->handle) {
        plugin_dlclose(plugin->handle);
    }
    
    /* Free metadata */
    free(plugin->metadata.name);
    free(plugin->metadata.version);
    free(plugin->metadata.author);
    free(plugin->metadata.description);
    
    free(plugin);
}

/* Register plugin with manager */
int plugin_manager_register(PluginManager *pm, Plugin *plugin) {
    if (!pm || !plugin) return -1;
    
    /* Add to linked list */
    plugin->next = pm->plugins;
    pm->plugins = plugin;
    pm->plugin_count++;
    
    /* Initialize plugin */
    if (plugin->init) {
        if (plugin->init(plugin, pm->interp) != 0) {
            fprintf(stderr, "Failed to initialize plugin %s\n", plugin->metadata.name);
            return -1;
        }
    }
    
    /* Register functions */
    if (plugin->register_functions) {
        if (plugin->register_functions(plugin, pm->interp) != 0) {
            fprintf(stderr, "Failed to register functions for plugin %s\n", plugin->metadata.name);
            return -1;
        }
    }
    
    return 0;
}

/* Find plugin by name */
Plugin* plugin_manager_find(PluginManager *pm, const char *name) {
    if (!pm || !name) return NULL;
    
    Plugin *plugin = pm->plugins;
    while (plugin) {
        if (strcmp(plugin->metadata.name, name) == 0) {
            return plugin;
        }
        plugin = plugin->next;
    }
    
    return NULL;
}

/* Load plugin by name */
int plugin_manager_load(PluginManager *pm, const char *name) {
    if (!pm || !name) return -1;
    
    /* Build plugin path with platform-specific extension */
    char path[512];
    snprintf(path, sizeof(path), "%s/%s%s", pm->plugin_directory, name, PLUGIN_EXTENSION);
    
    /* Load plugin */
    Plugin *plugin = plugin_load(path);
    if (!plugin) {
        return -1;
    }
    
    /* Register plugin */
    return plugin_manager_register(pm, plugin);
}

/* Load all plugins from directory */
int plugin_manager_load_all(PluginManager *pm) {
    if (!pm) return -1;
    
    int loaded = 0;
    
#ifdef _WIN32
    /* Windows directory traversal */
    char search_path[512];
    snprintf(search_path, sizeof(search_path), "%s/*%s", pm->plugin_directory, PLUGIN_EXTENSION);
    
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(search_path, &find_data);
    
    if (find_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Cannot open plugin directory: %s\n", pm->plugin_directory);
        return -1;
    }
    
    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pm->plugin_directory, find_data.cFileName);
            
            Plugin *plugin = plugin_load(path);
            if (plugin) {
                if (plugin_manager_register(pm, plugin) == 0) {
                    loaded++;
                }
            }
        }
    } while (FindNextFileA(find_handle, &find_data) != 0);
    
    FindClose(find_handle);
#else
    /* POSIX directory traversal */
    DIR *dir = opendir(pm->plugin_directory);
    if (!dir) {
        fprintf(stderr, "Cannot open plugin directory: %s\n", pm->plugin_directory);
        return -1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        /* Check if it's a .so file */
        const char *ext = strrchr(entry->d_name, '.');
        if (ext && strcmp(ext, PLUGIN_EXTENSION) == 0) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pm->plugin_directory, entry->d_name);
            
            Plugin *plugin = plugin_load(path);
            if (plugin) {
                if (plugin_manager_register(pm, plugin) == 0) {
                    loaded++;
                }
            }
        }
    }
    
    closedir(dir);
#endif
    
    return loaded;
}

/* List all plugins */
void plugin_manager_list(PluginManager *pm) {
    if (!pm) return;
    
    printf("\nLoaded plugins (%d):\n", pm->plugin_count);
    
    Plugin *plugin = pm->plugins;
    while (plugin) {
        printf("  • %s v%s\n", plugin->metadata.name, plugin->metadata.version);
        printf("    Author: %s\n", plugin->metadata.author);
        printf("    Type: %d\n", plugin->metadata.type);
        printf("    Description: %s\n", plugin->metadata.description);
        printf("\n");
        plugin = plugin->next;
    }
}

/* Plugin sandboxing */
PluginSandbox* plugin_sandbox_create(void) {
    PluginSandbox *sandbox = malloc(sizeof(PluginSandbox));
    sandbox->allow_file_access = 0;
    sandbox->allow_network_access = 0;
    sandbox->allow_process_spawn = 0;
    sandbox->memory_limit_mb = 100;
    sandbox->cpu_time_limit_ms = 1000;
    return sandbox;
}

void plugin_sandbox_destroy(PluginSandbox *sandbox) {
    free(sandbox);
}

int plugin_sandbox_apply(Plugin *plugin, PluginSandbox *sandbox) {
    /* Apply sandbox restrictions to plugin */
    /* This would use OS-specific features like seccomp, pledge, etc. */
    (void)plugin;
    (void)sandbox;
    
    /* Placeholder implementation */
    return 0;
}

/* Plugin discovery */
PluginDiscovery* plugin_discover(const char *directory) {
    PluginDiscovery *discovery = malloc(sizeof(PluginDiscovery));
    discovery->plugin_paths = NULL;
    discovery->count = 0;
    
#ifdef _WIN32
    /* Windows directory traversal */
    char search_path[512];
    snprintf(search_path, sizeof(search_path), "%s/*%s", directory, PLUGIN_EXTENSION);
    
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(search_path, &find_data);
    
    if (find_handle == INVALID_HANDLE_VALUE) {
        return discovery;
    }
    
    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            discovery->count++;
            discovery->plugin_paths = realloc(discovery->plugin_paths,
                                            discovery->count * sizeof(char*));
            
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", directory, find_data.cFileName);
            discovery->plugin_paths[discovery->count - 1] = strdup(path);
        }
    } while (FindNextFileA(find_handle, &find_data) != 0);
    
    FindClose(find_handle);
#else
    /* POSIX directory traversal */
    DIR *dir = opendir(directory);
    if (!dir) {
        return discovery;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *ext = strrchr(entry->d_name, '.');
        if (ext && strcmp(ext, PLUGIN_EXTENSION) == 0) {
            discovery->count++;
            discovery->plugin_paths = realloc(discovery->plugin_paths,
                                            discovery->count * sizeof(char*));
            
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            discovery->plugin_paths[discovery->count - 1] = strdup(path);
        }
    }
    
    closedir(dir);
#endif
    
    return discovery;
}

void plugin_discovery_free(PluginDiscovery *discovery) {
    if (!discovery) return;
    
    for (int i = 0; i < discovery->count; i++) {
        free(discovery->plugin_paths[i]);
    }
    free(discovery->plugin_paths);
    free(discovery);
}

/* Plugin registry */
PluginRegistry* plugin_registry_create(const char *url) {
    PluginRegistry *registry = malloc(sizeof(PluginRegistry));
    registry->entries = NULL;
    registry->count = 0;
    registry->registry_url = strdup(url);
    return registry;
}

void plugin_registry_destroy(PluginRegistry *registry) {
    if (!registry) return;
    
    for (int i = 0; i < registry->count; i++) {
        free(registry->entries[i].name);
        free(registry->entries[i].version);
        free(registry->entries[i].url);
        free(registry->entries[i].checksum);
    }
    free(registry->entries);
    free(registry->registry_url);
    free(registry);
}

int plugin_registry_fetch(PluginRegistry *registry) {
    /* Fetch plugin list from remote registry */
    /* This would use HTTP client to download registry index */
    (void)registry;
    
    /* Placeholder implementation */
    return 0;
}

PluginRegistryEntry* plugin_registry_find(PluginRegistry *registry, const char *name) {
    if (!registry || !name) return NULL;
    
    for (int i = 0; i < registry->count; i++) {
        if (strcmp(registry->entries[i].name, name) == 0) {
            return &registry->entries[i];
        }
    }
    
    return NULL;
}

int plugin_registry_install(PluginRegistry *registry, const char *name, const char *dest_dir) {
    /* Find plugin in registry */
    PluginRegistryEntry *entry = plugin_registry_find(registry, name);
    if (!entry) {
        return -1;
    }
    
    /* Download and install plugin */
    /* This would use HTTP client to download the plugin */
    (void)dest_dir;
    
    /* Placeholder implementation */
    return 0;
}
