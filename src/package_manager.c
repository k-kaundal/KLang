#include "package_manager.h"
#include "cli_colors.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

/* Platform-specific directory creation */
static int create_directory(const char *path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}

/* Check if directory exists */
static int dir_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* Create package directory structure */
static int create_pkg_structure(const char *base_dir) {
    char path[512];
    
    /* Create base directory */
    if (!dir_exists(base_dir)) {
        if (create_directory(base_dir) != 0) {
            return -1;
        }
    }
    
    /* Create packages directory */
    snprintf(path, sizeof(path), "%s/packages", base_dir);
    if (!dir_exists(path)) {
        if (create_directory(path) != 0) {
            return -1;
        }
    }
    
    /* Create cache directory */
    snprintf(path, sizeof(path), "%s/cache", base_dir);
    if (!dir_exists(path)) {
        if (create_directory(path) != 0) {
            return -1;
        }
    }
    
    return 0;
}

/* Initialize package manager */
PackageManager* pkg_manager_init(const char *project_dir) {
    PackageManager *pm = malloc(sizeof(PackageManager));
    char path[512];
    
    if (!pm) {
        return NULL;
    }
    
    /* Set up directories */
    snprintf(path, sizeof(path), "%s/.kpkg", project_dir);
    pm->cache_dir = strdup(path);
    
    snprintf(path, sizeof(path), "%s/.kpkg/packages", project_dir);
    pm->install_dir = strdup(path);
    
    snprintf(path, sizeof(path), "%s/package.kl.json", project_dir);
    pm->manifest_path = strdup(path);
    
    /* Create directory structure */
    snprintf(path, sizeof(path), "%s/.kpkg", project_dir);
    if (create_pkg_structure(path) != 0) {
        fprintf(stderr, "Warning: Could not create package directories\n");
    }
    
    /* Try to load manifest */
    pm->manifest = pkg_manifest_load(pm->manifest_path);
    
    return pm;
}

/* Free package manager resources */
void pkg_manager_free(PackageManager *pm) {
    if (!pm) return;
    
    free(pm->cache_dir);
    free(pm->install_dir);
    free(pm->manifest_path);
    
    if (pm->manifest) {
        pkg_manifest_free(pm->manifest);
    }
    
    free(pm);
}

/* Initialize a new package manifest */
PackageManifest* pkg_manifest_init(const char *name, const char *version) {
    PackageManifest *manifest = malloc(sizeof(PackageManifest));
    
    if (!manifest) {
        return NULL;
    }
    
    manifest->name = strdup(name);
    manifest->version = strdup(version);
    manifest->description = strdup("");
    manifest->author = strdup("");
    manifest->dependencies = NULL;
    manifest->dep_versions = NULL;
    manifest->dep_count = 0;
    manifest->entry_point = strdup("main.kl");
    manifest->license = strdup("MIT");
    
    return manifest;
}

/* Simple JSON parser for package manifest */
PackageManifest* pkg_manifest_load(const char *path) {
    FILE *f = fopen(path, "r");
    PackageManifest *manifest;
    char line[1024];
    
    if (!f) {
        return NULL;
    }
    
    manifest = malloc(sizeof(PackageManifest));
    if (!manifest) {
        fclose(f);
        return NULL;
    }
    
    /* Initialize defaults */
    manifest->name = NULL;
    manifest->version = NULL;
    manifest->description = NULL;
    manifest->author = NULL;
    manifest->dependencies = NULL;
    manifest->dep_versions = NULL;
    manifest->dep_count = 0;
    manifest->entry_point = strdup("main.kl");
    manifest->license = strdup("MIT");
    
    /* Parse JSON (simple line-based parser) */
    while (fgets(line, sizeof(line), f)) {
        char *key, *value, *colon;
        char *line_trimmed = line;
        
        /* Trim whitespace */
        while (*line_trimmed == ' ' || *line_trimmed == '\t') {
            line_trimmed++;
        }
        
        /* Skip comments and empty lines */
        if (*line_trimmed == '\0' || *line_trimmed == '\n' || *line_trimmed == '{' || *line_trimmed == '}') {
            continue;
        }
        
        /* Find colon */
        colon = strchr(line_trimmed, ':');
        if (!colon) {
            continue;
        }
        
        /* Extract key and value */
        *colon = '\0';
        key = line_trimmed;
        value = colon + 1;
        
        /* Remove quotes and trim */
        while (*key == ' ' || *key == '\t' || *key == '"') key++;
        char *key_end = key + strlen(key) - 1;
        while (key_end > key && (*key_end == ' ' || *key_end == '\t' || *key_end == '"' || *key_end == ',')) {
            *key_end = '\0';
            key_end--;
        }
        
        while (*value == ' ' || *value == '\t' || *value == '"') value++;
        char *value_end = value + strlen(value) - 1;
        while (value_end > value && (*value_end == ' ' || *value_end == '\t' || *value_end == '"' || *value_end == ',' || *value_end == '\n')) {
            *value_end = '\0';
            value_end--;
        }
        
        /* Parse fields */
        if (strcmp(key, "name") == 0) {
            manifest->name = strdup(value);
        } else if (strcmp(key, "version") == 0) {
            manifest->version = strdup(value);
        } else if (strcmp(key, "description") == 0) {
            manifest->description = strdup(value);
        } else if (strcmp(key, "author") == 0) {
            manifest->author = strdup(value);
        } else if (strcmp(key, "entry_point") == 0 || strcmp(key, "main") == 0) {
            free(manifest->entry_point);
            manifest->entry_point = strdup(value);
        } else if (strcmp(key, "license") == 0) {
            free(manifest->license);
            manifest->license = strdup(value);
        }
    }
    
    fclose(f);
    
    /* Validate required fields */
    if (!manifest->name || !manifest->version) {
        pkg_manifest_free(manifest);
        return NULL;
    }
    
    return manifest;
}

/* Save package manifest to file */
int pkg_manifest_save(const PackageManifest *manifest, const char *path) {
    FILE *f = fopen(path, "w");
    int i;
    
    if (!f) {
        return -1;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"name\": \"%s\",\n", manifest->name);
    fprintf(f, "  \"version\": \"%s\",\n", manifest->version);
    fprintf(f, "  \"description\": \"%s\",\n", manifest->description ? manifest->description : "");
    fprintf(f, "  \"author\": \"%s\",\n", manifest->author ? manifest->author : "");
    fprintf(f, "  \"license\": \"%s\",\n", manifest->license ? manifest->license : "MIT");
    fprintf(f, "  \"main\": \"%s\"", manifest->entry_point ? manifest->entry_point : "main.kl");
    
    if (manifest->dep_count > 0) {
        fprintf(f, ",\n  \"dependencies\": {\n");
        for (i = 0; i < manifest->dep_count; i++) {
            fprintf(f, "    \"%s\": \"%s\"", 
                    manifest->dependencies[i], 
                    manifest->dep_versions[i]);
            if (i < manifest->dep_count - 1) {
                fprintf(f, ",");
            }
            fprintf(f, "\n");
        }
        fprintf(f, "  }\n");
    } else {
        fprintf(f, "\n");
    }
    
    fprintf(f, "}\n");
    
    fclose(f);
    return 0;
}

/* Free manifest */
void pkg_manifest_free(PackageManifest *manifest) {
    int i;
    
    if (!manifest) return;
    
    free(manifest->name);
    free(manifest->version);
    free(manifest->description);
    free(manifest->author);
    free(manifest->entry_point);
    free(manifest->license);
    
    if (manifest->dependencies) {
        for (i = 0; i < manifest->dep_count; i++) {
            free(manifest->dependencies[i]);
            free(manifest->dep_versions[i]);
        }
        free(manifest->dependencies);
        free(manifest->dep_versions);
    }
    
    free(manifest);
}

/* Add dependency to manifest */
int pkg_manifest_add_dependency(PackageManifest *manifest, const char *name, const char *version) {
    int i;
    
    /* Check if already exists */
    for (i = 0; i < manifest->dep_count; i++) {
        if (strcmp(manifest->dependencies[i], name) == 0) {
            /* Update version */
            free(manifest->dep_versions[i]);
            manifest->dep_versions[i] = strdup(version);
            return 0;
        }
    }
    
    /* Add new dependency */
    manifest->dep_count++;
    manifest->dependencies = realloc(manifest->dependencies, manifest->dep_count * sizeof(char*));
    manifest->dep_versions = realloc(manifest->dep_versions, manifest->dep_count * sizeof(char*));
    
    manifest->dependencies[manifest->dep_count - 1] = strdup(name);
    manifest->dep_versions[manifest->dep_count - 1] = strdup(version);
    
    return 0;
}

/* Remove dependency from manifest */
int pkg_manifest_remove_dependency(PackageManifest *manifest, const char *name) {
    int i, j;
    
    for (i = 0; i < manifest->dep_count; i++) {
        if (strcmp(manifest->dependencies[i], name) == 0) {
            free(manifest->dependencies[i]);
            free(manifest->dep_versions[i]);
            
            /* Shift remaining dependencies */
            for (j = i; j < manifest->dep_count - 1; j++) {
                manifest->dependencies[j] = manifest->dependencies[j + 1];
                manifest->dep_versions[j] = manifest->dep_versions[j + 1];
            }
            
            manifest->dep_count--;
            return 0;
        }
    }
    
    return -1; /* Not found */
}

/* Install a package */
int pkg_install(PackageManager *pm, const char *package_name, const char *version) {
    char pkg_dir[512];
    
    print_info("Installing package:");
    printf(" %s@%s\n", package_name, version ? version : "latest");
    
    /* Create package directory */
    snprintf(pkg_dir, sizeof(pkg_dir), "%s/%s", pm->install_dir, package_name);
    
    if (!dir_exists(pkg_dir)) {
        if (create_directory(pkg_dir) != 0) {
            print_error("Failed to create package directory");
            return -1;
        }
    }
    
    /* In a real implementation, this would download from a registry */
    /* For now, we'll just create a placeholder */
    
    /* Add to manifest if we have one */
    if (pm->manifest) {
        pkg_manifest_add_dependency(pm->manifest, package_name, version ? version : "latest");
        pkg_manifest_save(pm->manifest, pm->manifest_path);
    }
    
    print_success("Package installed successfully");
    return 0;
}

/* Uninstall a package */
int pkg_uninstall(PackageManager *pm, const char *package_name) {
    print_info("Uninstalling package:");
    printf(" %s\n", package_name);
    
    /* Remove from manifest */
    if (pm->manifest) {
        if (pkg_manifest_remove_dependency(pm->manifest, package_name) == 0) {
            pkg_manifest_save(pm->manifest, pm->manifest_path);
        }
    }
    
    /* In a real implementation, this would remove the package directory */
    
    print_success("Package uninstalled successfully");
    return 0;
}

/* Update a package */
int pkg_update(PackageManager *pm, const char *package_name) {
    print_info("Updating package:");
    printf(" %s\n", package_name);
    
    /* In a real implementation, this would check for updates and download */
    
    print_success("Package updated successfully");
    return 0;
}

/* Update all packages */
int pkg_update_all(PackageManager *pm) {
    int i;
    
    if (!pm->manifest || pm->manifest->dep_count == 0) {
        print_info("No packages to update");
        return 0;
    }
    
    print_info("Updating all packages...\n");
    
    for (i = 0; i < pm->manifest->dep_count; i++) {
        pkg_update(pm, pm->manifest->dependencies[i]);
    }
    
    return 0;
}

/* List installed packages */
void pkg_list_installed(PackageManager *pm) {
    int i;
    
    if (!pm->manifest || pm->manifest->dep_count == 0) {
        print_info("No packages installed");
        return;
    }
    
    printf("\n");
    print_info("Installed packages:\n");
    
    for (i = 0; i < pm->manifest->dep_count; i++) {
        printf("  • %s@%s\n", 
               pm->manifest->dependencies[i],
               pm->manifest->dep_versions[i]);
    }
    
    printf("\n");
}

/* Check if a package is installed */
int pkg_is_installed(PackageManager *pm, const char *package_name) {
    int i;
    
    if (!pm->manifest) {
        return 0;
    }
    
    for (i = 0; i < pm->manifest->dep_count; i++) {
        if (strcmp(pm->manifest->dependencies[i], package_name) == 0) {
            return 1;
        }
    }
    
    return 0;
}

/* Resolve dependencies (recursive) */
int pkg_resolve_dependencies(PackageManager *pm) {
    if (!pm->manifest || pm->manifest->dep_count == 0) {
        return 0;
    }
    
    print_info("Resolving dependencies...\n");
    
    /* In a real implementation, this would recursively resolve all dependencies */
    /* For now, just report what we have */
    
    print_success("Dependencies resolved");
    return 0;
}
