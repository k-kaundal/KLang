#ifndef PLUGIN_SYSTEM_H
#define PLUGIN_SYSTEM_H

#include <stdio.h>

/* Platform-specific includes for dynamic loading */
#ifdef _WIN32
    #include <windows.h>
    typedef HMODULE PluginHandle;
#else
    #include <dlfcn.h>
    typedef void* PluginHandle;
#endif

#include "interpreter.h"

/* Plugin API version */
#define PLUGIN_API_VERSION 1

/* Plugin types */
typedef enum {
    PLUGIN_TYPE_EXTENSION,    /* Language extension */
    PLUGIN_TYPE_LIBRARY,      /* Standard library addition */
    PLUGIN_TYPE_BUILTIN,      /* Built-in function provider */
    PLUGIN_TYPE_TRANSFORM,    /* AST transformer */
    PLUGIN_TYPE_BACKEND       /* Code generation backend */
} PluginType;

/* Plugin metadata */
typedef struct {
    char *name;
    char *version;
    char *author;
    char *description;
    PluginType type;
    int api_version;
} PluginMetadata;

/* Plugin interface */
typedef struct Plugin {
    PluginHandle handle;  /* Dynamic library handle (HMODULE on Windows, void* on POSIX) */
    PluginMetadata metadata;
    
    /* Lifecycle callbacks */
    int (*init)(struct Plugin *plugin, Interpreter *interp);
    void (*cleanup)(struct Plugin *plugin);
    
    /* Function registration */
    int (*register_functions)(struct Plugin *plugin, Interpreter *interp);
    
    /* AST transformation (optional) */
    ASTNode* (*transform_ast)(struct Plugin *plugin, ASTNode *node);
    
    /* User data */
    void *user_data;
    
    /* Next plugin in chain */
    struct Plugin *next;
} Plugin;

/* Plugin manager */
typedef struct {
    Plugin *plugins;
    int plugin_count;
    char *plugin_directory;
    Interpreter *interp;
} PluginManager;

/* Plugin manager operations */
PluginManager* plugin_manager_create(Interpreter *interp);
void plugin_manager_destroy(PluginManager *pm);
void plugin_manager_set_directory(PluginManager *pm, const char *dir);

/* Plugin loading */
Plugin* plugin_load(const char *path);
int plugin_manager_load(PluginManager *pm, const char *name);
int plugin_manager_load_all(PluginManager *pm);
void plugin_unload(Plugin *plugin);

/* Plugin registration */
int plugin_manager_register(PluginManager *pm, Plugin *plugin);
Plugin* plugin_manager_find(PluginManager *pm, const char *name);
void plugin_manager_list(PluginManager *pm);

/* Plugin execution */
int plugin_init(Plugin *plugin, Interpreter *interp);
void plugin_cleanup(Plugin *plugin);
int plugin_register_functions(Plugin *plugin, Interpreter *interp);

/* Plugin sandboxing */
typedef struct {
    int allow_file_access;
    int allow_network_access;
    int allow_process_spawn;
    int memory_limit_mb;
    int cpu_time_limit_ms;
} PluginSandbox;

PluginSandbox* plugin_sandbox_create(void);
void plugin_sandbox_destroy(PluginSandbox *sandbox);
int plugin_sandbox_apply(Plugin *plugin, PluginSandbox *sandbox);

/* Plugin helper macros */
#define PLUGIN_EXPORT __attribute__((visibility("default")))

/* Plugin registration macro */
#define KLANG_PLUGIN(name, version, author, desc, type) \
    PLUGIN_EXPORT PluginMetadata klang_plugin_metadata = { \
        .name = name, \
        .version = version, \
        .author = author, \
        .description = desc, \
        .type = type, \
        .api_version = PLUGIN_API_VERSION \
    };

#define KLANG_PLUGIN_INIT(func) \
    PLUGIN_EXPORT int klang_plugin_init(Plugin *plugin, Interpreter *interp) { \
        return func(plugin, interp); \
    }

#define KLANG_PLUGIN_CLEANUP(func) \
    PLUGIN_EXPORT void klang_plugin_cleanup(Plugin *plugin) { \
        func(plugin); \
    }

#define KLANG_PLUGIN_REGISTER(func) \
    PLUGIN_EXPORT int klang_plugin_register_functions(Plugin *plugin, Interpreter *interp) { \
        return func(plugin, interp); \
    }

/* Plugin discovery */
typedef struct {
    char **plugin_paths;
    int count;
} PluginDiscovery;

PluginDiscovery* plugin_discover(const char *directory);
void plugin_discovery_free(PluginDiscovery *discovery);

/* Plugin registry (for online/remote plugins) */
typedef struct {
    char *name;
    char *version;
    char *url;
    char *checksum;
} PluginRegistryEntry;

typedef struct {
    PluginRegistryEntry *entries;
    int count;
    char *registry_url;
} PluginRegistry;

PluginRegistry* plugin_registry_create(const char *url);
void plugin_registry_destroy(PluginRegistry *registry);
int plugin_registry_fetch(PluginRegistry *registry);
PluginRegistryEntry* plugin_registry_find(PluginRegistry *registry, const char *name);
int plugin_registry_install(PluginRegistry *registry, const char *name, const char *dest_dir);

#endif /* PLUGIN_SYSTEM_H */
