#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Package metadata structure */
typedef struct {
    char *name;
    char *version;
    char *description;
    char *author;
    char **dependencies;     /* Array of dependency names */
    char **dep_versions;     /* Array of dependency versions */
    int dep_count;
    char *entry_point;       /* Main file */
    char *license;
} PackageManifest;

/* Package manager context */
typedef struct {
    char *cache_dir;         /* Local package cache directory */
    char *install_dir;       /* Package installation directory */
    char *manifest_path;     /* Path to package.kl.json */
    PackageManifest *manifest;
} PackageManager;

/* Initialize package manager */
PackageManager* pkg_manager_init(const char *project_dir);

/* Free package manager resources */
void pkg_manager_free(PackageManager *pm);

/* Load package manifest from file */
PackageManifest* pkg_manifest_load(const char *path);

/* Save package manifest to file */
int pkg_manifest_save(const PackageManifest *manifest, const char *path);

/* Free manifest */
void pkg_manifest_free(PackageManifest *manifest);

/* Install a package */
int pkg_install(PackageManager *pm, const char *package_name, const char *version);

/* Uninstall a package */
int pkg_uninstall(PackageManager *pm, const char *package_name);

/* Update a package */
int pkg_update(PackageManager *pm, const char *package_name);

/* Update all packages */
int pkg_update_all(PackageManager *pm);

/* List installed packages */
void pkg_list_installed(PackageManager *pm);

/* Check if a package is installed */
int pkg_is_installed(PackageManager *pm, const char *package_name);

/* Initialize a new package manifest */
PackageManifest* pkg_manifest_init(const char *name, const char *version);

/* Add dependency to manifest */
int pkg_manifest_add_dependency(PackageManifest *manifest, const char *name, const char *version);

/* Remove dependency from manifest */
int pkg_manifest_remove_dependency(PackageManifest *manifest, const char *name);

/* Resolve dependencies */
int pkg_resolve_dependencies(PackageManager *pm);

#endif /* PACKAGE_MANAGER_H */
