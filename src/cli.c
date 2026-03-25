#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "compiler.h"
#include "vm_stack.h"
#include "runtime.h"
#include "cli_colors.h"
#include "cli_help.h"
#include "cli_commands.h"
#include "config.h"
#include "test_runner.h"
#include "project_init.h"
#include "llvm_backend.h"
#include "../include/package_manager.h"
#include "../include/lsp_server.h"
#include "../include/http_server.h"

void run_repl(void);

static int validate_file_extension(const char *path) {
    const char *ext = strrchr(path, '.');
    if (ext == NULL) {
        return 0;
    }
    return (strcmp(ext, ".kl") == 0 || strcmp(ext, ".k") == 0 || strcmp(ext, ".klang") == 0);
}

static char *read_file(const char *path) {
    FILE *f = fopen(path, "r");
    long size;
    char *buf;
    size_t read_bytes;
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(size + 1);
    read_bytes = fread(buf, 1, size, f);
    buf[read_bytes] = '\0';
    fclose(f);
    return buf;
}

static void run_file(const char *path) {
    char *source;
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    int i;
    
    if (!validate_file_extension(path)) {
        print_error("Invalid file extension. KLang files must have .kl, .k, or .klang extension");
        fprintf(stderr, "Given file: %s\n", path);
        return;
    }
    
    source = read_file(path);
    if (!source) {
        print_error("Cannot open file");
        return;
    }

    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);

    nodes = parse_program(&parser, &count);

    if (!parser.had_error) {
        Interpreter *interp = interpreter_new();
        runtime_init(interp);
        
        /* Set the current module directory for relative imports */
        char *path_copy = strdup(path);
        char *dir = dirname(path_copy);
        interp->current_module_dir = strdup(dir);
        free(path_copy);

        for (i = 0; i < count; i++) {
            Value result = eval_node(interp, nodes[i]);
            value_free(&result);
            if (interp->had_error) break;
        }

        interpreter_free(interp);
    }

    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
}

static void build_file(const char *path) {
    char *source;
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    int i;
    
    if (!validate_file_extension(path)) {
        print_error("Invalid file extension. KLang files must have .kl, .k, or .klang extension");
        fprintf(stderr, "Given file: %s\n", path);
        return;
    }
    
    source = read_file(path);
    if (!source) {
        print_error("Cannot open file");
        return;
    }

    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);

    nodes = parse_program(&parser, &count);

    if (!parser.had_error) {
        Compiler *comp = compiler_new();
        Chunk *chunk = compile_program(comp, nodes, count);
        VM *vm = vm_new();
        vm_execute(vm, chunk);
        vm_free(vm);
        chunk_free(chunk);
        compiler_free(comp);
    }

    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
}

static void compile_file(const char *path) {
    char *source;
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    int i;
    Config *cfg = config_get();
    
    if (!validate_file_extension(path)) {
        print_error("Invalid file extension. KLang files must have .kl, .k, or .klang extension");
        fprintf(stderr, "Given file: %s\n", path);
        return;
    }
    
    source = read_file(path);
    if (!source) {
        print_error("Cannot open file");
        return;
    }

    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);

    nodes = parse_program(&parser, &count);

    if (!parser.had_error) {
        // Generate output filename (remove extension and use as executable name)
        char output_path[1024];
        const char *base = strrchr(path, '/');
        base = base ? base + 1 : path;
        
        // Copy base name without extension
        char *dot = strrchr(base, '.');
        int name_len = dot ? (int)(dot - base) : (int)strlen(base);
        strncpy(output_path, base, name_len);
        output_path[name_len] = '\0';
        
        print_success("Compiling with LLVM backend...");
        printf("Input: %s\n", path);
        printf("Output: %s\n", output_path);
        printf("Build mode: %s\n", config_get_build_mode_name(cfg->build_mode));
        printf("Optimization: %s\n", config_get_opt_level_name(cfg->opt_level));
        printf("LTO: %s\n", cfg->enable_lto ? "enabled" : "disabled");
        
        // Initialize LLVM backend
        llvm_backend_init();
        
        // Compile to native executable
        if (llvm_compile_ast(nodes, count, output_path) == 0) {
            print_success("Compilation successful!");
            printf("Generated executable: %s\n", output_path);
        } else {
            print_error("Compilation failed");
        }
    }

    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
}

int main(int argc, char **argv) {
    int i;
    
    /* Initialize configuration */
    config_init();
    
    /* Check for --no-color flag first */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--no-color") == 0) {
            enable_colors(0);
            config_set_colors(0);
            break;
        }
    }
    
    /* No arguments - start REPL */
    if (argc < 2) {
        run_repl();
        config_free();
        return 0;
    }

    /* Check for help and version flags */
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        config_free();
        return 0;
    }
    
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        print_version();
        config_free();
        return 0;
    }
    
    /* Check for config command */
    if (strcmp(argv[1], "config") == 0) {
        config_print();
        config_free();
        return 0;
    }

    /* Handle commands */
    if (strcmp(argv[1], "repl") == 0) {
        run_repl();
    } 
    else if (strcmp(argv[1], "run") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang run <file.kl>\n");
            fprintf(stderr, "Try 'klang help run' for more information.\n");
            return 1;
        }
        run_file(argv[2]);
    } 
    else if (strcmp(argv[1], "build") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang build <file.kl>\n");
            fprintf(stderr, "Try 'klang help build' for more information.\n");
            return 1;
        }
        build_file(argv[2]);
    }
    else if (strcmp(argv[1], "compile") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang compile [--mode=<mode>] [--opt=<level>] [--lto] <file.kl>\n");
            fprintf(stderr, "Try 'klang help compile' for more information.\n");
            return 1;
        }
        
        /* Parse compile options */
        const char *file_path = NULL;
        for (i = 2; i < argc; i++) {
            if (strncmp(argv[i], "--mode=", 7) == 0) {
                config_set_build_mode(config_parse_build_mode(argv[i] + 7));
            } else if (strncmp(argv[i], "--opt=", 6) == 0 || strncmp(argv[i], "-O", 2) == 0) {
                const char *level_str = (argv[i][1] == 'O') ? argv[i] + 2 : argv[i] + 6;
                config_set_opt_level(config_parse_opt_level(level_str));
            } else if (strcmp(argv[i], "--lto") == 0) {
                Config *cfg = config_get();
                cfg->enable_lto = 1;
            } else if (strcmp(argv[i], "--no-lto") == 0) {
                Config *cfg = config_get();
                cfg->enable_lto = 0;
            } else if (strcmp(argv[i], "--strip") == 0) {
                Config *cfg = config_get();
                cfg->strip_debug = 1;
            } else if (argv[i][0] != '-') {
                file_path = argv[i];
            }
        }
        
        if (!file_path) {
            print_error("Missing file argument");
            return 1;
        }
        
        compile_file(file_path);
    }
    else if (strcmp(argv[1], "check") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang check <file.kl>\n");
            fprintf(stderr, "Try 'klang help check' for more information.\n");
            return 1;
        }
        cmd_check_file(argv[2]);
    }
    else if (strcmp(argv[1], "typecheck") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang typecheck [--strict] <file.kl>\n");
            fprintf(stderr, "Try 'klang help typecheck' for more information.\n");
            return 1;
        }
        
        int strict_mode = 0;
        const char *file_path = argv[2];
        
        if (strcmp(argv[2], "--strict") == 0) {
            if (argc < 4) {
                print_error("Missing file argument after --strict");
                return 1;
            }
            strict_mode = 1;
            file_path = argv[3];
        }
        
        cmd_typecheck_file(file_path, strict_mode);
    }
    else if (strcmp(argv[1], "info") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang info <file.kl>\n");
            fprintf(stderr, "Try 'klang help info' for more information.\n");
            return 1;
        }
        cmd_info_file(argv[2]);
    }
    else if (strcmp(argv[1], "fmt") == 0) {
        if (argc < 3) {
            print_error("Missing file argument");
            fprintf(stderr, "Usage: klang fmt <file.kl> [--check] [--indent=N] [--tabs]\n");
            fprintf(stderr, "Try 'klang help fmt' for more information.\n");
            return 1;
        }
        
        /* Parse fmt options */
        int check_only = 0;
        int indent_size = 4; /* Default */
        int use_tabs = 0;
        
        for (i = 3; i < argc; i++) {
            if (strcmp(argv[i], "--check") == 0) {
                check_only = 1;
            } else if (strncmp(argv[i], "--indent=", 9) == 0) {
                indent_size = atoi(argv[i] + 9);
                if (indent_size < 1 || indent_size > 8) {
                    print_error("Invalid indent size (must be 1-8)");
                    return 1;
                }
            } else if (strcmp(argv[i], "--tabs") == 0) {
                use_tabs = 1;
            }
        }
        
        cmd_fmt_file(argv[2], check_only, indent_size, use_tabs);
    }
    else if (strcmp(argv[1], "test") == 0) {
        if (argc < 3) {
            print_error("Missing path argument");
            fprintf(stderr, "Usage: klang test <file|directory>\n");
            fprintf(stderr, "Try 'klang help test' for more information.\n");
            config_free();
            return 1;
        }
        run_tests(argv[2]);
    }
    else if (strcmp(argv[1], "init") == 0) {
        if (argc < 3) {
            print_error("Missing project name");
            fprintf(stderr, "Usage: klang init <project-name> [--git]\n");
            fprintf(stderr, "Try 'klang help init' for more information.\n");
            config_free();
            return 1;
        }
        
        int create_git = 0;
        if (argc >= 4 && strcmp(argv[3], "--git") == 0) {
            create_git = 1;
        }
        
        init_project(argv[2], create_git);
    }
    else if (strcmp(argv[1], "config") == 0) {
        config_print();
    }
    else if (strcmp(argv[1], "version") == 0) {
        print_version();
    }
    else if (strcmp(argv[1], "help") == 0) {
        if (argc >= 3) {
            print_command_help(argv[2]);
        } else {
            print_help();
        }
    }
    else if (strcmp(argv[1], "pkg") == 0 || strcmp(argv[1], "package") == 0) {
        if (argc < 3) {
            print_error("Missing package command");
            fprintf(stderr, "Usage: klang pkg <install|uninstall|update|list|init>\n");
            fprintf(stderr, "Try 'klang help pkg' for more information.\n");
            config_free();
            return 1;
        }
        
        PackageManager *pm = pkg_manager_init(".");
        if (!pm) {
            print_error("Failed to initialize package manager");
            config_free();
            return 1;
        }
        
        if (strcmp(argv[2], "install") == 0 || strcmp(argv[2], "add") == 0) {
            if (argc < 4) {
                print_error("Missing package name");
                fprintf(stderr, "Usage: klang pkg install <package[@version]>\n");
                pkg_manager_free(pm);
                config_free();
                return 1;
            }
            
            /* Parse package@version */
            char *package = strdup(argv[3]);
            char *version = strchr(package, '@');
            if (version) {
                *version = '\0';
                version++;
            }
            
            pkg_install(pm, package, version);
            free(package);
        }
        else if (strcmp(argv[2], "uninstall") == 0 || strcmp(argv[2], "remove") == 0) {
            if (argc < 4) {
                print_error("Missing package name");
                fprintf(stderr, "Usage: klang pkg uninstall <package>\n");
                pkg_manager_free(pm);
                config_free();
                return 1;
            }
            
            pkg_uninstall(pm, argv[3]);
        }
        else if (strcmp(argv[2], "update") == 0) {
            if (argc >= 4) {
                pkg_update(pm, argv[3]);
            } else {
                pkg_update_all(pm);
            }
        }
        else if (strcmp(argv[2], "list") == 0 || strcmp(argv[2], "ls") == 0) {
            pkg_list_installed(pm);
        }
        else if (strcmp(argv[2], "init") == 0) {
            if (argc < 4) {
                print_error("Missing package name");
                fprintf(stderr, "Usage: klang pkg init <name>\n");
                pkg_manager_free(pm);
                config_free();
                return 1;
            }
            
            PackageManifest *manifest = pkg_manifest_init(argv[3], "1.0.0");
            if (manifest) {
                pkg_manifest_save(manifest, "package.kl.json");
                print_success("Package manifest created: package.kl.json");
                pkg_manifest_free(manifest);
            } else {
                print_error("Failed to create package manifest");
            }
        }
        else {
            print_error("Unknown package command");
            fprintf(stderr, "Command: %s\n", argv[2]);
            fprintf(stderr, "Try 'klang help pkg' for more information.\n");
        }
        
        pkg_manager_free(pm);
    }
    else if (strcmp(argv[1], "lsp") == 0) {
        /* Start LSP server */
        LSPServer *lsp = lsp_server_init();
        if (!lsp) {
            print_error("Failed to initialize LSP server");
            config_free();
            return 1;
        }
        
        int result = lsp_server_run(lsp);
        lsp_server_free(lsp);
        
        config_free();
        return result;
    }
    else if (strcmp(argv[1], "serve") == 0) {
        /* Start built-in HTTP server */
        int port = 8080; /* Default port */
        const char *static_dir = "."; /* Default directory */
        int hot_reload = 0;
        
        /* Parse serve options */
        for (i = 2; i < argc; i++) {
            if (strncmp(argv[i], "--port=", 7) == 0 || strncmp(argv[i], "-p", 2) == 0) {
                const char *port_str = (argv[i][1] == 'p' && argv[i][2] == '=') ? argv[i] + 3 :
                                       (argv[i][1] == 'p' && i + 1 < argc) ? argv[++i] : argv[i] + 7;
                port = atoi(port_str);
                if (port <= 0 || port > 65535) {
                    print_error("Invalid port number");
                    return 1;
                }
            } else if (strncmp(argv[i], "--dir=", 6) == 0) {
                static_dir = argv[i] + 6;
            } else if (strcmp(argv[i], "--hot-reload") == 0 || strcmp(argv[i], "--watch") == 0) {
                hot_reload = 1;
            } else if (strncmp(argv[i], "--mode=", 7) == 0) {
                config_set_build_mode(config_parse_build_mode(argv[i] + 7));
                Config *cfg = config_get();
                if (cfg->build_mode == BUILD_MODE_DEV) {
                    hot_reload = 1; /* Auto-enable hot reload in dev mode */
                }
            }
        }
        
        HttpServer *server = http_server_new(port);
        if (!server) {
            print_error("Failed to create HTTP server");
            config_free();
            return 1;
        }
        
        /* Setup static file serving */
        http_server_static(server, "/", static_dir);
        
        /* Enable hot reload if requested */
        if (hot_reload) {
            http_server_enable_hot_reload(server, 1);
            http_server_watch_directory(server, static_dir);
            print_success("Hot reload enabled");
        }
        
        /* Start server */
        if (http_server_start(server) < 0) {
            print_error("Failed to start HTTP server");
            http_server_free(server);
            config_free();
            return 1;
        }
        
        print_success("HTTP server started");
        printf("Listening on http://localhost:%d\n", port);
        printf("Serving files from: %s\n", static_dir);
        printf("Press Ctrl+C to stop the server\n");
        
        /* Keep server running */
        while (http_server_is_running(server)) {
            sleep(1);
        }
        
        http_server_free(server);
        config_free();
        return 0;
    }
    else {
        print_error("Unknown command");
        fprintf(stderr, "Command: %s\n", argv[1]);
        fprintf(stderr, "Try 'klang --help' for available commands.\n");
        config_free();
        return 1;
    }
    
    config_free();
    return 0;
}
