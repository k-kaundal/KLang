#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "compiler.h"
#include "vm.h"
#include "runtime.h"
#include "cli_colors.h"
#include "cli_help.h"
#include "cli_commands.h"
#include "config.h"
#include "test_runner.h"
#include "project_init.h"
#include "llvm_backend.h"

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
            fprintf(stderr, "Usage: klang compile <file.kl>\n");
            fprintf(stderr, "Try 'klang help compile' for more information.\n");
            return 1;
        }
        compile_file(argv[2]);
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
