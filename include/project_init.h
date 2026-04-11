#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H

/* Initialize a new KLang project with directory structure and template files
 * project_name: Name of the project/directory to create
 * create_git: Whether to initialize a git repository
 */
void init_project(const char *project_name, int create_git);

/* Initialize a new server project (wrapper for generators.h init_server_project)
 * project_name: Name of the project/directory to create
 * create_git: Whether to initialize a git repository
 */
void init_project_server(const char *project_name, int create_git);

#endif /* PROJECT_INIT_H */
