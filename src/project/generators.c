#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "generators.h"
#include "cli_colors.h"

/* Helper functions */
static int create_directory(const char *path) {
    #ifdef _WIN32
    return mkdir(path);
    #else
    return mkdir(path, 0755);
    #endif
}

static int write_file(const char *path, const char *content) {
    FILE *f = fopen(path, "w");
    if (!f) {
        return -1;
    }
    fprintf(f, "%s", content);
    fclose(f);
    return 0;
}

static int directory_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

static int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

/* Convert name to PascalCase */
static void to_pascal_case(const char *name, char *output, size_t output_size) {
    size_t i = 0, j = 0;
    int capitalize_next = 1;
    
    while (name[i] && j < output_size - 1) {
        if (name[i] == '-' || name[i] == '_' || name[i] == ' ') {
            capitalize_next = 1;
            i++;
            continue;
        }
        
        if (capitalize_next) {
            output[j++] = (name[i] >= 'a' && name[i] <= 'z') ? name[i] - 32 : name[i];
            capitalize_next = 0;
        } else {
            output[j++] = name[i];
        }
        i++;
    }
    output[j] = '\0';
}

/* Convert name to kebab-case */
static void to_kebab_case(const char *name, char *output, size_t output_size) {
    size_t i = 0, j = 0;
    
    while (name[i] && j < output_size - 1) {
        if (name[i] >= 'A' && name[i] <= 'Z') {
            if (j > 0) {
                output[j++] = '-';
            }
            output[j++] = name[i] + 32; // to lowercase
        } else if (name[i] == '_' || name[i] == ' ') {
            output[j++] = '-';
        } else {
            output[j++] = name[i];
        }
        i++;
    }
    output[j] = '\0';
}

/* Check if we're in a KLang project directory */
int is_in_project_directory(void) {
    return file_exists("package.kl.json") || file_exists(".klang.toml") || directory_exists("src");
}

/* Template: Server README */
static const char *SERVER_README_TEMPLATE =
"# %s\n\n"
"A KLang server application.\n\n"
"## Getting Started\n\n"
"```bash\n"
"# Install dependencies\n"
"klang pkg install\n\n"
"# Run in development mode (with hot reload)\n"
"klang run src/main.kl --mode=dev\n\n"
"# Run in production mode\n"
"klang run src/main.kl --mode=production\n\n"
"# Run tests\n"
"klang test tests/\n"
"```\n\n"
"## Project Structure\n\n"
"```\n"
"%s/\n"
"├── src/\n"
"│   ├── main.kl              # Application entry point\n"
"│   ├── app.kl               # Application setup\n"
"│   ├── controllers/         # Request handlers\n"
"│   ├── services/            # Business logic\n"
"│   ├── middleware/          # Custom middleware\n"
"│   ├── models/              # Data models\n"
"│   ├── routes/              # Route definitions\n"
"│   └── config/              # Configuration\n"
"├── tests/\n"
"│   ├── unit/               # Unit tests\n"
"│   └── integration/        # Integration tests\n"
"├── public/                 # Static files\n"
"└── .env                    # Environment variables\n"
"```\n\n"
"## Available Commands\n\n"
"- `klang generate module <name>` - Generate a new module\n"
"- `klang generate controller <name>` - Generate a new controller\n"
"- `klang generate service <name>` - Generate a new service\n"
"- `klang generate middleware <name>` - Generate new middleware\n\n"
"## API Endpoints\n\n"
"See the route files in `src/routes/` for available endpoints.\n";

/* Template: Main file for server */
static const char *SERVER_MAIN_TEMPLATE =
"# Main entry point for %s\n"
"import server from \"stdlib/server\"\n"
"import app from \"./app\"\n\n"
"# Get port from environment or use default\n"
"let port = env(\"PORT\") || 3000\n\n"
"# Start the server\n"
"app.listen(port)\n\n"
"println(\"🚀 Server running on http://localhost:${port}\")\n"
"println(\"📚 Environment: ${env(\"NODE_ENV\") || \"development\"}\")\n"
"println(\"\")\n"
"println(\"Press Ctrl+C to stop\")\n";

/* Template: App setup file */
static const char *SERVER_APP_TEMPLATE =
"# Application setup for %s\n"
"import server from \"stdlib/server\"\n"
"import apiRoutes from \"./routes/api.routes\"\n"
"import loggerMiddleware from \"./middleware/logger.middleware\"\n\n"
"# Create server application\n"
"let app = server.createApp()\n\n"
"# Global middleware\n"
"app.use(loggerMiddleware)\n\n"
"# Enable CORS\n"
"app.use(server.cors())\n\n"
"# Parse JSON bodies\n"
"app.use(server.json())\n\n"
"# API routes\n"
"app.use(\"/api\", apiRoutes)\n\n"
"# Serve static files\n"
"app.static(\"/\", \"./public\")\n\n"
"# Health check endpoint\n"
"app.get(\"/health\", (req, res) => {\n"
"    res.json({\n"
"        status: \"healthy\",\n"
"        timestamp: Date.now(),\n"
"        uptime: process.uptime()\n"
"    })\n"
"})\n\n"
"# Error handler\n"
"app.useErrorHandler((err, req, res) => {\n"
"    println(\"Error:\", err.message)\n"
"    res.status(500).json({\n"
"        error: err.message,\n"
"        timestamp: Date.now()\n"
"    })\n"
"})\n\n"
"export app\n";

/* Template: API routes */
static const char *API_ROUTES_TEMPLATE =
"# API Routes\n"
"import server from \"stdlib/server\"\n\n"
"let router = server.Router()\n\n"
"# Welcome endpoint\n"
"router.get(\"/\", (req, res) => {\n"
"    res.json({\n"
"        message: \"Welcome to the API\",\n"
"        version: \"1.0.0\",\n"
"        endpoints: [\n"
"            \"GET /api/\",\n"
"            \"GET /health\"\n"
"        ]\n"
"    })\n"
"})\n\n"
"export router\n";

/* Template: Logger middleware */
static const char *LOGGER_MIDDLEWARE_TEMPLATE =
"# Logger Middleware\n\n"
"fn loggerMiddleware(req, res, next) {\n"
"    let start = Date.now()\n"
"    \n"
"    println(\"[${req.method}] ${req.path}\")\n"
"    \n"
"    # Continue to next middleware/handler\n"
"    next()\n"
"    \n"
"    let duration = Date.now() - start\n"
"    println(\"Completed in ${duration}ms\")\n"
"}\n\n"
"export loggerMiddleware\n";

/* Template: Environment file */
static const char *ENV_TEMPLATE =
"# Server Configuration\n"
"PORT=3000\n"
"NODE_ENV=development\n\n"
"# Database Configuration (if needed)\n"
"# DB_HOST=localhost\n"
"# DB_PORT=5432\n"
"# DB_NAME=myapp\n"
"# DB_USER=user\n"
"# DB_PASSWORD=password\n\n"
"# API Keys (if needed)\n"
"# API_KEY=your_api_key_here\n";

/* Template: Package manifest for server */
static const char *SERVER_PACKAGE_TEMPLATE =
"{\n"
"  \"name\": \"%s\",\n"
"  \"version\": \"1.0.0\",\n"
"  \"description\": \"A KLang server application\",\n"
"  \"main\": \"src/main.kl\",\n"
"  \"scripts\": {\n"
"    \"start\": \"klang run src/main.kl --mode=production\",\n"
"    \"dev\": \"klang run src/main.kl --mode=dev --hot-reload\",\n"
"    \"test\": \"klang test tests/\"\n"
"  },\n"
"  \"dependencies\": {},\n"
"  \"devDependencies\": {}\n"
"}\n";

/* Initialize a new server project */
void init_server_project(const char *project_name, TemplateType template, int create_git) {
    (void)template;  /* Unused parameter - reserved for future template variations */
    char path[512];
    char content[4096];
    
    printf("\n%s🚀 Creating KLang server project:%s %s\n\n",
           get_color(COLOR_BOLD_CYAN),
           get_color(COLOR_RESET),
           project_name);
    
    /* Create project directory */
    if (create_directory(project_name) != 0 && errno != EEXIST) {
        print_error("Cannot create project directory");
        fprintf(stderr, "Directory: %s\n", project_name);
        return;
    }
    
    printf("  %s✓%s Created directory: %s\n",
           get_color(COLOR_GREEN),
           get_color(COLOR_RESET),
           project_name);
    
    /* Create subdirectories */
    const char *dirs[] = {
        "src", "src/controllers", "src/services", "src/middleware",
        "src/models", "src/routes", "src/config",
        "tests", "tests/unit", "tests/integration",
        "public"
    };
    
    for (int i = 0; i < 11; i++) {
        snprintf(path, sizeof(path), "%s/%s", project_name, dirs[i]);
        if (create_directory(path) != 0 && errno != EEXIST) {
            fprintf(stderr, "Warning: Could not create directory %s\n", path);
        } else {
            printf("  %s✓%s Created directory: %s\n",
                   get_color(COLOR_GREEN),
                   get_color(COLOR_RESET),
                   path);
        }
    }
    
    /* Create README.md */
    snprintf(path, sizeof(path), "%s/README.md", project_name);
    snprintf(content, sizeof(content), SERVER_README_TEMPLATE, project_name, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: README.md\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create src/main.kl */
    snprintf(path, sizeof(path), "%s/src/main.kl", project_name);
    snprintf(content, sizeof(content), SERVER_MAIN_TEMPLATE, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: src/main.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create src/app.kl */
    snprintf(path, sizeof(path), "%s/src/app.kl", project_name);
    snprintf(content, sizeof(content), SERVER_APP_TEMPLATE, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: src/app.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create src/routes/api.routes.kl */
    snprintf(path, sizeof(path), "%s/src/routes/api.routes.kl", project_name);
    if (write_file(path, API_ROUTES_TEMPLATE) == 0) {
        printf("  %s✓%s Created file: src/routes/api.routes.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create src/middleware/logger.middleware.kl */
    snprintf(path, sizeof(path), "%s/src/middleware/logger.middleware.kl", project_name);
    if (write_file(path, LOGGER_MIDDLEWARE_TEMPLATE) == 0) {
        printf("  %s✓%s Created file: src/middleware/logger.middleware.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create .env */
    snprintf(path, sizeof(path), "%s/.env", project_name);
    if (write_file(path, ENV_TEMPLATE) == 0) {
        printf("  %s✓%s Created file: .env\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create package.kl.json */
    snprintf(path, sizeof(path), "%s/package.kl.json", project_name);
    snprintf(content, sizeof(content), SERVER_PACKAGE_TEMPLATE, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: package.kl.json\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create .gitignore */
    snprintf(path, sizeof(path), "%s/.gitignore", project_name);
    const char *gitignore = "# KLang\n*.o\n*.tmp\n.klang_history\nnode_modules/\n\n# Environment\n.env.local\n.env.production\n\n# OS\n.DS_Store\nThumbs.db\n\n# IDE\n.vscode/\n.idea/\n*.swp\n*.swo\n";
    if (write_file(path, gitignore) == 0) {
        printf("  %s✓%s Created file: .gitignore\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create public/index.html */
    snprintf(path, sizeof(path), "%s/public/index.html", project_name);
    const char *index_html = "<!DOCTYPE html>\n<html>\n<head>\n    <title>%s</title>\n</head>\n<body>\n    <h1>Welcome to %s</h1>\n    <p>Your KLang server is running!</p>\n</body>\n</html>\n";
    snprintf(content, sizeof(content), index_html, project_name, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: public/index.html\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Initialize git if requested */
    if (create_git) {
        char git_cmd[1024];
        snprintf(git_cmd, sizeof(git_cmd), "cd %s && git init -q", project_name);
        if (system(git_cmd) == 0) {
            printf("  %s✓%s Initialized git repository\n",
                   get_color(COLOR_GREEN),
                   get_color(COLOR_RESET));
        }
    }
    
    /* Print success message */
    printf("\n%s✓ Server project initialized successfully!%s\n\n",
           get_color(COLOR_BOLD_GREEN),
           get_color(COLOR_RESET));
    
    printf("Next steps:\n");
    printf("  cd %s\n", project_name);
    printf("  klang run src/main.kl --mode=dev\n\n");
    printf("Generate components:\n");
    printf("  klang generate controller users\n");
    printf("  klang generate service users\n");
    printf("  klang generate middleware auth\n\n");
}

/* Generate a controller */
int generate_controller(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        fprintf(stderr, "Run 'klang new server <name>' first\n");
        return -1;
    }
    
    if (!directory_exists("src/controllers")) {
        print_error("controllers directory not found");
        fprintf(stderr, "Expected: src/controllers/\n");
        return -1;
    }
    
    char pascal_name[256];
    char kebab_name[256];
    char filename[512];
    char content[2048];
    
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    
    snprintf(filename, sizeof(filename), "src/controllers/%s.controller.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Controller already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Controller\n"
        "import server from \"stdlib/server\"\n\n"
        "class %sController {\n"
        "    constructor() {\n"
        "        # Initialize controller\n"
        "    }\n\n"
        "    # GET /%s\n"
        "    getAll(req, res) {\n"
        "        res.json({\n"
        "            message: \"Get all %s\",\n"
        "            data: []\n"
        "        })\n"
        "    }\n\n"
        "    # GET /%s/:id\n"
        "    getById(req, res) {\n"
        "        let id = req.params.id\n"
        "        res.json({\n"
        "            message: \"Get %s by id\",\n"
        "            id: id\n"
        "        })\n"
        "    }\n\n"
        "    # POST /%s\n"
        "    create(req, res) {\n"
        "        let data = req.body\n"
        "        res.status(201).json({\n"
        "            message: \"%s created\",\n"
        "            data: data\n"
        "        })\n"
        "    }\n\n"
        "    # PUT /%s/:id\n"
        "    update(req, res) {\n"
        "        let id = req.params.id\n"
        "        let data = req.body\n"
        "        res.json({\n"
        "            message: \"%s updated\",\n"
        "            id: id,\n"
        "            data: data\n"
        "        })\n"
        "    }\n\n"
        "    # DELETE /%s/:id\n"
        "    delete(req, res) {\n"
        "        let id = req.params.id\n"
        "        res.status(204).send()\n"
        "    }\n"
        "}\n\n"
        "export %sController\n";
    
    snprintf(content, sizeof(content), template, 
             pascal_name, pascal_name, kebab_name, kebab_name,
             kebab_name, kebab_name, kebab_name, pascal_name,
             kebab_name, pascal_name, kebab_name, pascal_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated controller: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create controller file");
    return -1;
}

/* Generate a service */
int generate_service(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        fprintf(stderr, "Run 'klang new server <name>' first\n");
        return -1;
    }
    
    if (!directory_exists("src/services")) {
        print_error("services directory not found");
        fprintf(stderr, "Expected: src/services/\n");
        return -1;
    }
    
    char pascal_name[256];
    char kebab_name[256];
    char filename[512];
    char content[2048];
    
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    
    snprintf(filename, sizeof(filename), "src/services/%s.service.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Service already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Service\n\n"
        "class %sService {\n"
        "    constructor() {\n"
        "        # Initialize service\n"
        "        this.data = []\n"
        "    }\n\n"
        "    findAll() {\n"
        "        return this.data\n"
        "    }\n\n"
        "    findById(id) {\n"
        "        return this.data.find(item => item.id == id)\n"
        "    }\n\n"
        "    create(data) {\n"
        "        let newItem = {\n"
        "            id: this.data.length + 1,\n"
        "            ...data,\n"
        "            createdAt: Date.now()\n"
        "        }\n"
        "        this.data.push(newItem)\n"
        "        return newItem\n"
        "    }\n\n"
        "    update(id, data) {\n"
        "        let index = this.data.findIndex(item => item.id == id)\n"
        "        if (index >= 0) {\n"
        "            this.data[index] = {\n"
        "                ...this.data[index],\n"
        "                ...data,\n"
        "                updatedAt: Date.now()\n"
        "            }\n"
        "            return this.data[index]\n"
        "        }\n"
        "        return null\n"
        "    }\n\n"
        "    delete(id) {\n"
        "        let initialLength = this.data.length\n"
        "        this.data = this.data.filter(item => item.id != id)\n"
        "        return this.data.length < initialLength\n"
        "    }\n"
        "}\n\n"
        "export %sService\n";
    
    snprintf(content, sizeof(content), template, pascal_name, pascal_name, pascal_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated service: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create service file");
    return -1;
}

/* Generate middleware */
int generate_middleware(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        fprintf(stderr, "Run 'klang new server <name>' first\n");
        return -1;
    }
    
    if (!directory_exists("src/middleware")) {
        print_error("middleware directory not found");
        fprintf(stderr, "Expected: src/middleware/\n");
        return -1;
    }
    
    char kebab_name[256];
    char filename[512];
    char content[1024];
    
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    
    snprintf(filename, sizeof(filename), "src/middleware/%s.middleware.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Middleware already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Middleware\n\n"
        "fn %sMiddleware(req, res, next) {\n"
        "    # Middleware logic here\n"
        "    println(\"[%s] Processing request\")\n\n"
        "    # Call next middleware/handler\n"
        "    next()\n"
        "}\n\n"
        "export %sMiddleware\n";
    
    char pascal_name[256];
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    
    snprintf(content, sizeof(content), template, pascal_name, kebab_name, pascal_name, kebab_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated middleware: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create middleware file");
    return -1;
}

/* Generate a route file */
int generate_route(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        fprintf(stderr, "Run 'klang new server <name>' first\n");
        return -1;
    }
    
    if (!directory_exists("src/routes")) {
        print_error("routes directory not found");
        fprintf(stderr, "Expected: src/routes/\n");
        return -1;
    }
    
    char kebab_name[256];
    char pascal_name[256];
    char filename[512];
    char content[1024];
    
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    
    snprintf(filename, sizeof(filename), "src/routes/%s.routes.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Route file already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Routes\n"
        "import server from \"stdlib/server\"\n\n"
        "let router = server.Router()\n\n"
        "# Define routes here\n"
        "router.get(\"/\", (req, res) => {\n"
        "    res.json({ message: \"%s routes\" })\n"
        "})\n\n"
        "export router\n";
    
    snprintf(content, sizeof(content), template, pascal_name, pascal_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated route file: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create route file");
    return -1;
}

/* Generate a guard */
int generate_guard(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        return -1;
    }
    
    if (!directory_exists("src/middleware")) {
        create_directory("src/middleware");
    }
    
    char kebab_name[256];
    char pascal_name[256];
    char filename[512];
    char content[1024];
    
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    
    snprintf(filename, sizeof(filename), "src/middleware/%s.guard.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Guard already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Guard\n\n"
        "fn %sGuard(req, res, next) {\n"
        "    # Guard logic here\n"
        "    let isAuthorized = true // Replace with actual check\n\n"
        "    if (isAuthorized) {\n"
        "        next()\n"
        "    } else {\n"
        "        res.status(403).json({ error: \"Forbidden\" })\n"
        "    }\n"
        "}\n\n"
        "export %sGuard\n";
    
    snprintf(content, sizeof(content), template, pascal_name, kebab_name, kebab_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated guard: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create guard file");
    return -1;
}

/* Generate an interceptor */
int generate_interceptor(const char *name) {
    if (!is_in_project_directory()) {
        print_error("Not in a KLang project directory");
        return -1;
    }
    
    if (!directory_exists("src/middleware")) {
        create_directory("src/middleware");
    }
    
    char kebab_name[256];
    char pascal_name[256];
    char filename[512];
    char content[1024];
    
    to_kebab_case(name, kebab_name, sizeof(kebab_name));
    to_pascal_case(name, pascal_name, sizeof(pascal_name));
    
    snprintf(filename, sizeof(filename), "src/middleware/%s.interceptor.kl", kebab_name);
    
    if (file_exists(filename)) {
        print_error("Interceptor already exists");
        fprintf(stderr, "File: %s\n", filename);
        return -1;
    }
    
    const char *template =
        "# %s Interceptor\n\n"
        "fn %sInterceptor(req, res, next) {\n"
        "    # Pre-processing\n"
        "    println(\"[%s Interceptor] Before request\")\n\n"
        "    # Store original send method\n"
        "    let originalSend = res.send\n\n"
        "    # Override send method\n"
        "    res.send = (data) => {\n"
        "        # Post-processing\n"
        "        println(\"[%s Interceptor] After request\")\n"
        "        originalSend.call(res, data)\n"
        "    }\n\n"
        "    next()\n"
        "}\n\n"
        "export %sInterceptor\n";
    
    snprintf(content, sizeof(content), template, 
             pascal_name, kebab_name, pascal_name, pascal_name, kebab_name);
    
    if (write_file(filename, content) == 0) {
        printf("%s✓%s Generated interceptor: %s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET),
               filename);
        return 0;
    }
    
    print_error("Failed to create interceptor file");
    return -1;
}

/* Generate a complete module */
int generate_module(const char *name) {
    printf("\n%s📦 Generating module:%s %s\n\n",
           get_color(COLOR_BOLD_CYAN),
           get_color(COLOR_RESET),
           name);
    
    int success = 1;
    
    if (generate_controller(name) != 0) success = 0;
    if (generate_service(name) != 0) success = 0;
    if (generate_route(name) != 0) success = 0;
    
    if (success) {
        printf("\n%s✓ Module generated successfully!%s\n\n",
               get_color(COLOR_BOLD_GREEN),
               get_color(COLOR_RESET));
        printf("Next steps:\n");
        printf("  1. Import the route in src/app.kl\n");
        printf("  2. Register the route with app.use()\n");
        printf("  3. Implement business logic in the service\n\n");
        return 0;
    } else {
        printf("\n%s✗ Some files could not be generated%s\n",
               get_color(COLOR_BOLD_RED),
               get_color(COLOR_RESET));
        return -1;
    }
}

/* Generic component generator */
int generate_component(GeneratorType type, const char *name) {
    switch (type) {
        case GEN_MODULE:
            return generate_module(name);
        case GEN_CONTROLLER:
            return generate_controller(name);
        case GEN_SERVICE:
            return generate_service(name);
        case GEN_MIDDLEWARE:
            return generate_middleware(name);
        case GEN_ROUTE:
            return generate_route(name);
        case GEN_GUARD:
            return generate_guard(name);
        case GEN_INTERCEPTOR:
            return generate_interceptor(name);
        default:
            print_error("Unknown generator type");
            return -1;
    }
}
