#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "project_init.h"
#include "cli_colors.h"
#include "generators.h"

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

static const char *README_TEMPLATE =
"# %s\n\n"
"A KLang project.\n\n"
"## Getting Started\n\n"
"```bash\n"
"# Run the main file\n"
"klang run src/main.kl\n\n"
"# Run tests\n"
"klang test tests/\n\n"
"# Format code\n"
"klang fmt src/\n"
"```\n\n"
"## Project Structure\n\n"
"- `src/` - Source code\n"
"- `tests/` - Test files\n"
"- `lib/` - Libraries and dependencies\n"
"- `examples/` - Example code\n";

static const char *MAIN_TEMPLATE =
"# Main entry point for %s\n\n"
"fn main() {\n"
"    println(\"Hello from %s!\")\n"
"}\n\n"
"main()\n";

static const char *TEST_TEMPLATE =
"# Test file for %s\n\n"
"fn test_example() {\n"
"    let result = 2 + 2\n"
"    if result != 4 {\n"
"        println(\"Test failed: 2 + 2 should equal 4\")\n"
"        exit(1)\n"
"    }\n"
"    println(\"Test passed\")\n"
"}\n\n"
"test_example()\n";

static const char *GITIGNORE_TEMPLATE =
"# KLang\n"
"*.o\n"
"*.tmp\n"
".klang_history\n\n"
"# OS\n"
".DS_Store\n"
"Thumbs.db\n\n"
"# IDE\n"
".vscode/\n"
".idea/\n"
"*.swp\n"
"*.swo\n";

static const char *CONFIG_TEMPLATE =
"# KLang project configuration\n\n"
"[formatting]\n"
"indent_size = 4\n"
"use_tabs = false\n"
"max_line_length = 100\n\n"
"[repl]\n"
"history_size = 1000\n\n"
"[colors]\n"
"enabled = true\n";

void init_project(const char *project_name, int create_git) {
    char path[512];
    char content[2048];
    
    printf("\n%sInitializing KLang project:%s %s\n\n",
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
    const char *dirs[] = {"src", "tests", "lib", "examples"};
    int i;
    for (i = 0; i < 4; i++) {
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
    snprintf(content, sizeof(content), README_TEMPLATE, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: README.md\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create src/main.kl */
    snprintf(path, sizeof(path), "%s/src/main.kl", project_name);
    snprintf(content, sizeof(content), MAIN_TEMPLATE, project_name, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: src/main.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create tests/main_test.kl */
    snprintf(path, sizeof(path), "%s/tests/main_test.kl", project_name);
    snprintf(content, sizeof(content), TEST_TEMPLATE, project_name);
    if (write_file(path, content) == 0) {
        printf("  %s✓%s Created file: tests/main_test.kl\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create .gitignore */
    snprintf(path, sizeof(path), "%s/.gitignore", project_name);
    if (write_file(path, GITIGNORE_TEMPLATE) == 0) {
        printf("  %s✓%s Created file: .gitignore\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
    }
    
    /* Create .klang.toml */
    snprintf(path, sizeof(path), "%s/.klang.toml", project_name);
    if (write_file(path, CONFIG_TEMPLATE) == 0) {
        printf("  %s✓%s Created file: .klang.toml\n",
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
    printf("\n%s✓ Project initialized successfully!%s\n\n",
           get_color(COLOR_BOLD_GREEN),
           get_color(COLOR_RESET));
    
    printf("Next steps:\n");
    printf("  cd %s\n", project_name);
    printf("  klang run src/main.kl\n");
    printf("  klang test tests/\n\n");
}

/* Initialize a new server project (wrapper for generators) */
void init_project_server(const char *project_name, int create_git) {
    init_server_project(project_name, TEMPLATE_SERVER, create_git);
}
