#ifndef GENERATORS_H
#define GENERATORS_H

/* Generator types */
typedef enum {
    GEN_MODULE,
    GEN_CONTROLLER,
    GEN_SERVICE,
    GEN_MIDDLEWARE,
    GEN_ROUTE,
    GEN_GUARD,
    GEN_INTERCEPTOR
} GeneratorType;

/* Project template types */
typedef enum {
    TEMPLATE_BASIC,
    TEMPLATE_SERVER,
    TEMPLATE_MICROSERVICE,
    TEMPLATE_FULLSTACK
} TemplateType;

/* Initialize a new server project with enhanced structure
 * project_name: Name of the project/directory to create
 * template: Type of project template to use
 * create_git: Whether to initialize a git repository
 */
void init_server_project(const char *project_name, TemplateType template, int create_git);

/* Generate a component in the current project
 * type: Type of component to generate
 * name: Name of the component
 * Returns: 0 on success, -1 on failure
 */
int generate_component(GeneratorType type, const char *name);

/* Generate a module with all related components
 * name: Name of the module
 * Returns: 0 on success, -1 on failure
 */
int generate_module(const char *name);

/* Generate a controller
 * name: Name of the controller
 * Returns: 0 on success, -1 on failure
 */
int generate_controller(const char *name);

/* Generate a service
 * name: Name of the service
 * Returns: 0 on success, -1 on failure
 */
int generate_service(const char *name);

/* Generate middleware
 * name: Name of the middleware
 * Returns: 0 on success, -1 on failure
 */
int generate_middleware(const char *name);

/* Generate a route file
 * name: Name of the route
 * Returns: 0 on success, -1 on failure
 */
int generate_route(const char *name);

/* Generate a guard
 * name: Name of the guard
 * Returns: 0 on success, -1 on failure
 */
int generate_guard(const char *name);

/* Generate an interceptor
 * name: Name of the interceptor
 * Returns: 0 on success, -1 on failure
 */
int generate_interceptor(const char *name);

/* Helper: Check if we're in a KLang project directory
 * Returns: 1 if in project, 0 otherwise
 */
int is_in_project_directory(void);

#endif /* GENERATORS_H */
