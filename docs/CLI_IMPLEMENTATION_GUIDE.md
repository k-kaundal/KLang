# KLang CLI Implementation Guide

## 🏗️ Technical Implementation Details

This document provides detailed implementation guidance for enhancing the KLang CLI.

---

## 📁 Phase 1: Core Infrastructure

### 1.1 Argument Parsing Module

**File:** `src/cli/args.c` and `src/cli/args.h`

#### Data Structures

```c
/* args.h */
#ifndef KLANG_CLI_ARGS_H
#define KLANG_CLI_ARGS_H

#include <stdbool.h>

typedef enum {
    ARG_TYPE_NONE,      // Flag without value (--verbose)
    ARG_TYPE_STRING,    // String value (--output file.txt)
    ARG_TYPE_INT,       // Integer value (--jobs 4)
    ARG_TYPE_BOOL,      // Boolean value (--watch true)
} ArgType;

typedef struct {
    const char *long_name;      // --version
    char short_name;            // 'v' for -v
    const char *description;    // Help text
    ArgType type;               // Type of argument
    const char *value_name;     // Name shown in help: <FILE>
    bool required;              // Is this argument required?
    const char *default_value;  // Default value if not provided
} ArgSpec;

typedef struct {
    const char *name;
    char *value;
    bool is_set;
} ParsedArg;

typedef struct {
    ParsedArg *args;
    int arg_count;
    char **positional;
    int positional_count;
} ParsedArgs;

// Parse command line arguments
ParsedArgs *args_parse(int argc, char **argv, ArgSpec *specs, int spec_count);

// Get argument value
const char *args_get_string(ParsedArgs *args, const char *name);
int args_get_int(ParsedArgs *args, const char *name, int default_val);
bool args_get_bool(ParsedArgs *args, const char *name, bool default_val);
bool args_is_set(ParsedArgs *args, const char *name);

// Get positional argument
const char *args_get_positional(ParsedArgs *args, int index);
int args_get_positional_count(ParsedArgs *args);

// Free parsed arguments
void args_free(ParsedArgs *args);

// Print usage/help
void args_print_usage(const char *program_name, const char *command, 
                      ArgSpec *specs, int spec_count);

#endif // KLANG_CLI_ARGS_H
```

#### Implementation

```c
/* args.c */
#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_short_flag(const char *arg) {
    return arg[0] == '-' && arg[1] != '-' && arg[1] != '\0';
}

static bool is_long_flag(const char *arg) {
    return arg[0] == '-' && arg[1] == '-' && arg[2] != '\0';
}

static ArgSpec *find_spec_by_long(ArgSpec *specs, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (specs[i].long_name && strcmp(specs[i].long_name, name) == 0) {
            return &specs[i];
        }
    }
    return NULL;
}

static ArgSpec *find_spec_by_short(ArgSpec *specs, int count, char c) {
    for (int i = 0; i < count; i++) {
        if (specs[i].short_name == c) {
            return &specs[i];
        }
    }
    return NULL;
}

ParsedArgs *args_parse(int argc, char **argv, ArgSpec *specs, int spec_count) {
    ParsedArgs *parsed = calloc(1, sizeof(ParsedArgs));
    parsed->args = calloc(spec_count, sizeof(ParsedArg));
    parsed->arg_count = spec_count;
    parsed->positional = calloc(argc, sizeof(char *));
    parsed->positional_count = 0;
    
    // Initialize parsed args with specs
    for (int i = 0; i < spec_count; i++) {
        parsed->args[i].name = specs[i].long_name;
        parsed->args[i].value = specs[i].default_value ? 
                                strdup(specs[i].default_value) : NULL;
        parsed->args[i].is_set = false;
    }
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        
        if (is_long_flag(arg)) {
            // Long flag: --name or --name=value
            char *name = arg + 2;
            char *equals = strchr(name, '=');
            char *value = NULL;
            
            if (equals) {
                *equals = '\0';
                value = equals + 1;
            }
            
            ArgSpec *spec = find_spec_by_long(specs, spec_count, name);
            if (!spec) {
                fprintf(stderr, "Error: Unknown option --%s\n", name);
                args_free(parsed);
                return NULL;
            }
            
            // Find parsed arg
            for (int j = 0; j < spec_count; j++) {
                if (parsed->args[j].name == spec->long_name) {
                    parsed->args[j].is_set = true;
                    
                    if (spec->type != ARG_TYPE_NONE) {
                        if (value) {
                            free(parsed->args[j].value);
                            parsed->args[j].value = strdup(value);
                        } else if (i + 1 < argc && argv[i + 1][0] != '-') {
                            free(parsed->args[j].value);
                            parsed->args[j].value = strdup(argv[++i]);
                        } else {
                            fprintf(stderr, "Error: Option --%s requires a value\n", name);
                            args_free(parsed);
                            return NULL;
                        }
                    } else {
                        free(parsed->args[j].value);
                        parsed->args[j].value = strdup("true");
                    }
                    break;
                }
            }
        } else if (is_short_flag(arg)) {
            // Short flag: -f or -abc (multiple flags)
            for (int j = 1; arg[j] != '\0'; j++) {
                char c = arg[j];
                ArgSpec *spec = find_spec_by_short(specs, spec_count, c);
                
                if (!spec) {
                    fprintf(stderr, "Error: Unknown option -%c\n", c);
                    args_free(parsed);
                    return NULL;
                }
                
                // Find parsed arg
                for (int k = 0; k < spec_count; k++) {
                    if (parsed->args[k].name == spec->long_name) {
                        parsed->args[k].is_set = true;
                        
                        if (spec->type != ARG_TYPE_NONE) {
                            if (arg[j + 1] != '\0') {
                                // Value attached: -ofile.txt
                                free(parsed->args[k].value);
                                parsed->args[k].value = strdup(&arg[j + 1]);
                                break;
                            } else if (i + 1 < argc && argv[i + 1][0] != '-') {
                                // Value as next arg: -o file.txt
                                free(parsed->args[k].value);
                                parsed->args[k].value = strdup(argv[++i]);
                                break;
                            } else {
                                fprintf(stderr, "Error: Option -%c requires a value\n", c);
                                args_free(parsed);
                                return NULL;
                            }
                        } else {
                            free(parsed->args[k].value);
                            parsed->args[k].value = strdup("true");
                        }
                        break;
                    }
                }
            }
        } else {
            // Positional argument
            parsed->positional[parsed->positional_count++] = strdup(arg);
        }
    }
    
    // Check required arguments
    for (int i = 0; i < spec_count; i++) {
        if (specs[i].required && !parsed->args[i].is_set) {
            fprintf(stderr, "Error: Required option --%s is missing\n", 
                    specs[i].long_name);
            args_free(parsed);
            return NULL;
        }
    }
    
    return parsed;
}

const char *args_get_string(ParsedArgs *args, const char *name) {
    for (int i = 0; i < args->arg_count; i++) {
        if (strcmp(args->args[i].name, name) == 0) {
            return args->args[i].value;
        }
    }
    return NULL;
}

int args_get_int(ParsedArgs *args, const char *name, int default_val) {
    const char *val = args_get_string(args, name);
    return val ? atoi(val) : default_val;
}

bool args_get_bool(ParsedArgs *args, const char *name, bool default_val) {
    for (int i = 0; i < args->arg_count; i++) {
        if (strcmp(args->args[i].name, name) == 0) {
            if (!args->args[i].is_set) return default_val;
            if (!args->args[i].value) return true;
            const char *v = args->args[i].value;
            return strcmp(v, "true") == 0 || strcmp(v, "1") == 0 || 
                   strcmp(v, "yes") == 0;
        }
    }
    return default_val;
}

bool args_is_set(ParsedArgs *args, const char *name) {
    for (int i = 0; i < args->arg_count; i++) {
        if (strcmp(args->args[i].name, name) == 0) {
            return args->args[i].is_set;
        }
    }
    return false;
}

const char *args_get_positional(ParsedArgs *args, int index) {
    if (index >= 0 && index < args->positional_count) {
        return args->positional[index];
    }
    return NULL;
}

int args_get_positional_count(ParsedArgs *args) {
    return args->positional_count;
}

void args_free(ParsedArgs *args) {
    if (!args) return;
    
    for (int i = 0; i < args->arg_count; i++) {
        free(args->args[i].value);
    }
    free(args->args);
    
    for (int i = 0; i < args->positional_count; i++) {
        free(args->positional[i]);
    }
    free(args->positional);
    
    free(args);
}

void args_print_usage(const char *program_name, const char *command,
                      ArgSpec *specs, int spec_count) {
    if (command) {
        printf("Usage: %s %s [OPTIONS]", program_name, command);
    } else {
        printf("Usage: %s [OPTIONS] <COMMAND>", program_name);
    }
    
    // Print positional args from required specs
    for (int i = 0; i < spec_count; i++) {
        if (specs[i].required && specs[i].value_name) {
            printf(" %s", specs[i].value_name);
        }
    }
    printf("\n\n");
    
    if (spec_count > 0) {
        printf("Options:\n");
        for (int i = 0; i < spec_count; i++) {
            if (specs[i].short_name) {
                printf("  -%c, ", specs[i].short_name);
            } else {
                printf("      ");
            }
            
            printf("--%-20s", specs[i].long_name);
            printf("%s", specs[i].description);
            
            if (specs[i].default_value) {
                printf(" [default: %s]", specs[i].default_value);
            }
            if (specs[i].required) {
                printf(" [required]");
            }
            printf("\n");
        }
    }
}
```

---

### 1.2 Color Output Module

**File:** `src/cli/colors.c` and `src/cli/colors.h`

```c
/* colors.h */
#ifndef KLANG_CLI_COLORS_H
#define KLANG_CLI_COLORS_H

#include <stdbool.h>

// Color codes
typedef enum {
    COLOR_RESET,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_GRAY,
    COLOR_BOLD,
    COLOR_DIM,
} Color;

// Initialize colors (detect TTY)
void colors_init(bool force_disable);

// Check if colors are enabled
bool colors_enabled(void);

// Get ANSI code for color
const char *color_code(Color color);

// Colored printf
void color_printf(Color color, const char *format, ...);

// Convenience functions
void print_error(const char *format, ...);
void print_warning(const char *format, ...);
void print_success(const char *format, ...);
void print_info(const char *format, ...);

#endif // KLANG_CLI_COLORS_H
```

```c
/* colors.c */
#include "colors.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

static bool g_colors_enabled = true;

static const char *ANSI_CODES[] = {
    [COLOR_RESET]   = "\033[0m",
    [COLOR_RED]     = "\033[31m",
    [COLOR_GREEN]   = "\033[32m",
    [COLOR_YELLOW]  = "\033[33m",
    [COLOR_BLUE]    = "\033[34m",
    [COLOR_MAGENTA] = "\033[35m",
    [COLOR_CYAN]    = "\033[36m",
    [COLOR_WHITE]   = "\033[37m",
    [COLOR_GRAY]    = "\033[90m",
    [COLOR_BOLD]    = "\033[1m",
    [COLOR_DIM]     = "\033[2m",
};

void colors_init(bool force_disable) {
    if (force_disable) {
        g_colors_enabled = false;
        return;
    }
    
    // Check if stdout is a TTY
    g_colors_enabled = isatty(fileno(stdout));
    
    // Check NO_COLOR environment variable
    if (getenv("NO_COLOR") != NULL) {
        g_colors_enabled = false;
    }
    
    // Check KLANG_NO_COLOR
    if (getenv("KLANG_NO_COLOR") != NULL) {
        g_colors_enabled = false;
    }
}

bool colors_enabled(void) {
    return g_colors_enabled;
}

const char *color_code(Color color) {
    if (!g_colors_enabled) {
        return "";
    }
    return ANSI_CODES[color];
}

void color_printf(Color color, const char *format, ...) {
    va_list args;
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[color]);
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RESET]);
    }
}

void print_error(const char *format, ...) {
    va_list args;
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RED]);
    }
    printf("Error: ");
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RESET]);
    }
    printf("\n");
}

void print_warning(const char *format, ...) {
    va_list args;
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_YELLOW]);
    }
    printf("Warning: ");
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RESET]);
    }
    printf("\n");
}

void print_success(const char *format, ...) {
    va_list args;
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_GREEN]);
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RESET]);
    }
    printf("\n");
}

void print_info(const char *format, ...) {
    va_list args;
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_CYAN]);
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    if (g_colors_enabled) {
        printf("%s", ANSI_CODES[COLOR_RESET]);
    }
    printf("\n");
}
```

---

### 1.3 Command System

**File:** `src/cli/commands.c` and `src/cli/commands.h`

```c
/* commands.h */
#ifndef KLANG_CLI_COMMANDS_H
#define KLANG_CLI_COMMANDS_H

#include "args.h"

typedef struct {
    const char *name;
    const char *description;
    const char *usage;
    const char *examples;
    ArgSpec *args;
    int arg_count;
    int (*handler)(int argc, char **argv);
} Command;

// Register a command
void command_register(Command *cmd);

// Find a command by name
Command *command_find(const char *name);

// Get all commands
Command **command_get_all(int *count);

// Print all commands
void command_print_all(void);

// Find similar command (for suggestions)
const char *command_suggest(const char *name);

// Execute a command
int command_execute(const char *name, int argc, char **argv);

#endif // KLANG_CLI_COMMANDS_H
```

```c
/* commands.c */
#include "commands.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 32

static Command *commands[MAX_COMMANDS];
static int command_count = 0;

// Levenshtein distance for fuzzy matching
static int levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int matrix[len1 + 1][len2 + 1];
    
    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;
    
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            int del = matrix[i-1][j] + 1;
            int ins = matrix[i][j-1] + 1;
            int sub = matrix[i-1][j-1] + cost;
            
            matrix[i][j] = del < ins ? (del < sub ? del : sub) : 
                                       (ins < sub ? ins : sub);
        }
    }
    
    return matrix[len1][len2];
}

void command_register(Command *cmd) {
    if (command_count < MAX_COMMANDS) {
        commands[command_count++] = cmd;
    }
}

Command *command_find(const char *name) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(commands[i]->name, name) == 0) {
            return commands[i];
        }
    }
    return NULL;
}

Command **command_get_all(int *count) {
    *count = command_count;
    return commands;
}

void command_print_all(void) {
    printf("Commands:\n");
    for (int i = 0; i < command_count; i++) {
        printf("  %-12s %s\n", commands[i]->name, commands[i]->description);
    }
}

const char *command_suggest(const char *name) {
    int min_dist = 999;
    const char *best_match = NULL;
    
    for (int i = 0; i < command_count; i++) {
        int dist = levenshtein(name, commands[i]->name);
        if (dist < min_dist && dist <= 2) {  // Max 2 edits
            min_dist = dist;
            best_match = commands[i]->name;
        }
    }
    
    return best_match;
}

int command_execute(const char *name, int argc, char **argv) {
    Command *cmd = command_find(name);
    
    if (!cmd) {
        print_error("Unknown command '%s'", name);
        
        const char *suggestion = command_suggest(name);
        if (suggestion) {
            printf("\n");
            color_printf(COLOR_CYAN, "Did you mean '%s'?\n", suggestion);
        }
        
        printf("\n");
        command_print_all();
        printf("\nRun 'klang --help' for more information.\n");
        return 1;
    }
    
    return cmd->handler(argc, argv);
}
```

---

### 1.4 Help System

**File:** `src/cli/help.c` and `src/cli/help.h`

```c
/* help.h */
#ifndef KLANG_CLI_HELP_H
#define KLANG_CLI_HELP_H

#include "commands.h"

// Print general help
void help_print_general(const char *program_name);

// Print command-specific help
void help_print_command(Command *cmd);

// Print version information
void help_print_version(void);

#endif // KLANG_CLI_HELP_H
```

```c
/* help.c */
#include "help.h"
#include "colors.h"
#include "commands.h"
#include <stdio.h>

#define KLANG_VERSION "0.1.0"
#define KLANG_BUILD_DATE __DATE__

void help_print_general(const char *program_name) {
    color_printf(COLOR_BOLD, "KLang v%s", KLANG_VERSION);
    printf(" - A modern programming language\n\n");
    
    printf("Usage: %s [OPTIONS] <COMMAND>\n\n", program_name);
    
    command_print_all();
    
    printf("\nOptions:\n");
    printf("  -h, --help       Show this help message\n");
    printf("  -v, --version    Show version information\n");
    printf("  -V, --verbose    Verbose output\n");
    printf("  -q, --quiet      Quiet mode\n");
    printf("  --no-color       Disable colored output\n");
    
    printf("\nRun '%s <command> --help' for more information on a command.\n", 
           program_name);
    
    printf("\nExamples:\n");
    printf("  %s run app.kl          Run a file\n", program_name);
    printf("  %s repl                Start REPL\n", program_name);
    printf("  %s check --all .       Check all files\n", program_name);
    
    printf("\nFor more information, visit: ");
    color_printf(COLOR_CYAN, "https://github.com/yourusername/KLang\n");
}

void help_print_command(Command *cmd) {
    color_printf(COLOR_BOLD, "klang-%s", cmd->name);
    printf(" - %s\n\n", cmd->description);
    
    if (cmd->usage) {
        printf("%s\n\n", cmd->usage);
    }
    
    if (cmd->args && cmd->arg_count > 0) {
        printf("Options:\n");
        for (int i = 0; i < cmd->arg_count; i++) {
            ArgSpec *spec = &cmd->args[i];
            
            if (spec->short_name) {
                printf("  -%c, ", spec->short_name);
            } else {
                printf("      ");
            }
            
            printf("--%-20s", spec->long_name);
            printf("%s", spec->description);
            
            if (spec->default_value) {
                printf(" [default: %s]", spec->default_value);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    if (cmd->examples) {
        printf("Examples:\n%s\n", cmd->examples);
    }
}

void help_print_version(void) {
    color_printf(COLOR_BOLD, "KLang %s\n", KLANG_VERSION);
    printf("Built: %s\n", KLANG_BUILD_DATE);
    printf("Platform: ");
    
    #ifdef __linux__
    printf("Linux");
    #elif defined(__APPLE__)
    printf("macOS");
    #elif defined(_WIN32)
    printf("Windows");
    #else
    printf("Unknown");
    #endif
    
    #ifdef __x86_64__
    printf(" x86_64");
    #elif defined(__i386__)
    printf(" i386");
    #elif defined(__arm__)
    printf(" ARM");
    #elif defined(__aarch64__)
    printf(" ARM64");
    #endif
    
    printf("\n");
    printf("Features: GC, REPL, VM\n");
}
```

---

## 📁 Phase 2: Command Implementations

### 2.1 Check Command

**File:** `src/cli_commands/cmd_check.c`

```c
#include "../cli/commands.h"
#include "../cli/args.h"
#include "../cli/colors.h"
#include "../lexer.h"
#include "../parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static ArgSpec check_args[] = {
    {"all", 'a', "Check all .kl files in directory", ARG_TYPE_NONE, NULL, false, NULL},
    {"watch", 'w', "Watch mode - recheck on file changes", ARG_TYPE_NONE, NULL, false, NULL},
    {"verbose", 'V', "Verbose output", ARG_TYPE_NONE, NULL, false, NULL},
    {"help", 'h', "Show help", ARG_TYPE_NONE, NULL, false, NULL},
};

static bool is_klang_file(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return false;
    return strcmp(ext, ".kl") == 0 || strcmp(ext, ".k") == 0 || 
           strcmp(ext, ".klang") == 0;
}

static int check_file(const char *path, bool verbose) {
    FILE *f = fopen(path, "r");
    if (!f) {
        print_error("Cannot open file: %s", path);
        return 1;
    }
    
    // Read file
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source = malloc(size + 1);
    fread(source, 1, size, f);
    source[size] = '\0';
    fclose(f);
    
    // Parse
    Lexer lexer;
    Parser parser;
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    
    int count = 0;
    ASTNode **nodes = parse_program(&parser, &count);
    
    bool has_errors = parser.had_error;
    
    // Clean up
    for (int i = 0; i < count; i++) {
        ast_free(nodes[i]);
    }
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
    
    // Print result
    if (has_errors) {
        color_printf(COLOR_RED, "✗ ");
        printf("%s - ", path);
        color_printf(COLOR_RED, "Error\n");
        return 1;
    } else {
        color_printf(COLOR_GREEN, "✓ ");
        printf("%s - ", path);
        color_printf(COLOR_GREEN, "OK\n");
        return 0;
    }
}

static int check_directory(const char *path, bool verbose) {
    DIR *dir = opendir(path);
    if (!dir) {
        print_error("Cannot open directory: %s", path);
        return 1;
    }
    
    int total = 0;
    int errors = 0;
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(filepath, &st) == 0) {
            if (S_ISREG(st.st_mode) && is_klang_file(entry->d_name)) {
                total++;
                if (check_file(filepath, verbose) != 0) {
                    errors++;
                }
            } else if (S_ISDIR(st.st_mode)) {
                errors += check_directory(filepath, verbose);
            }
        }
    }
    
    closedir(dir);
    
    if (total > 0) {
        printf("\n");
        if (errors == 0) {
            print_success("All %d files passed checks", total);
        } else {
            print_error("%d of %d files have errors", errors, total);
        }
    }
    
    return errors;
}

static int cmd_check_handler(int argc, char **argv) {
    ParsedArgs *args = args_parse(argc, argv, check_args, 
                                  sizeof(check_args) / sizeof(ArgSpec));
    
    if (!args || args_is_set(args, "help")) {
        args_print_usage("klang", "check", check_args, 
                        sizeof(check_args) / sizeof(ArgSpec));
        args_free(args);
        return 0;
    }
    
    bool check_all = args_is_set(args, "all");
    bool verbose = args_is_set(args, "verbose");
    
    const char *path = args_get_positional(args, 0);
    if (!path) path = ".";
    
    int result;
    if (check_all) {
        result = check_directory(path, verbose);
    } else {
        result = check_file(path, verbose);
    }
    
    args_free(args);
    return result;
}

Command cmd_check = {
    .name = "check",
    .description = "Check syntax without running",
    .usage = "Usage: klang check [OPTIONS] [FILE]",
    .examples = "  klang check app.kl        Check a single file\n"
                "  klang check --all .       Check all files in directory\n"
                "  klang check --watch app.kl Watch and check on changes",
    .args = check_args,
    .arg_count = sizeof(check_args) / sizeof(ArgSpec),
    .handler = cmd_check_handler,
};
```

### 2.2 Format Command

**File:** `src/cli_commands/cmd_fmt.c`

```c
#include "../cli/commands.h"
#include "../cli/args.h"
#include "../cli/colors.h"
#include <stdio.h>

static ArgSpec fmt_args[] = {
    {"check", 'c', "Check if files are formatted", ARG_TYPE_NONE, NULL, false, NULL},
    {"write", 'w', "Write formatted output", ARG_TYPE_NONE, NULL, false, "true"},
    {"diff", 'd', "Show diff without writing", ARG_TYPE_NONE, NULL, false, NULL},
    {"indent", 'i', "Indentation size", ARG_TYPE_INT, "<n>", false, "4"},
    {"help", 'h', "Show help", ARG_TYPE_NONE, NULL, false, NULL},
};

static int cmd_fmt_handler(int argc, char **argv) {
    ParsedArgs *args = args_parse(argc, argv, fmt_args, 
                                  sizeof(fmt_args) / sizeof(ArgSpec));
    
    if (!args || args_is_set(args, "help")) {
        args_print_usage("klang", "fmt", fmt_args, 
                        sizeof(fmt_args) / sizeof(ArgSpec));
        args_free(args);
        return 0;
    }
    
    print_warning("Format command not yet implemented");
    
    // TODO: Implement formatting
    // 1. Parse file
    // 2. Pretty-print AST
    // 3. Write back or show diff
    
    args_free(args);
    return 0;
}

Command cmd_fmt = {
    .name = "fmt",
    .description = "Format KLang code",
    .usage = "Usage: klang fmt [OPTIONS] <FILE>",
    .examples = "  klang fmt app.kl          Format a file\n"
                "  klang fmt --check .       Check formatting\n"
                "  klang fmt --diff app.kl   Show diff",
    .args = fmt_args,
    .arg_count = sizeof(fmt_args) / sizeof(ArgSpec),
    .handler = cmd_fmt_handler,
};
```

---

## 📁 Phase 3: REPL Enhancement

### 3.1 Readline Integration

**File:** `src/repl/readline.c`

```c
/* Using linenoise for simplicity (BSD licensed) */
#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linenoise callbacks
static char **completions = NULL;
static int completion_count = 0;

char *readline_read(const char *prompt) {
    // Simple implementation - replace with linenoise later
    printf("%s", prompt);
    fflush(stdout);
    
    char *line = malloc(4096);
    if (!fgets(line, 4096, stdin)) {
        free(line);
        return NULL;
    }
    
    // Remove newline
    int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }
    
    return line;
}

void readline_add_history(const char *line) {
    // TODO: Implement history
    (void)line;
}

void readline_set_completions(char **words, int count) {
    completions = words;
    completion_count = count;
}

void readline_clear_screen(void) {
    printf("\033[2J\033[H");
}

int readline_load_history(const char *filename) {
    // TODO: Implement persistent history
    (void)filename;
    return 0;
}

int readline_save_history(const char *filename) {
    // TODO: Implement persistent history
    (void)filename;
    return 0;
}
```

---

## 🔧 Makefile Updates

Add to Makefile:

```makefile
# CLI sources
CLI_SOURCES = src/cli/args.c src/cli/colors.c src/cli/commands.c src/cli/help.c
CLI_SOURCES += src/cli_commands/cmd_check.c src/cli_commands/cmd_fmt.c
CLI_OBJECTS = $(CLI_SOURCES:.c=.o)

# Update main target
klang: $(OBJECTS) $(CLI_OBJECTS) src/cli.o
	$(CC) $(CFLAGS) -o klang $(OBJECTS) $(CLI_OBJECTS) src/cli.o $(LDFLAGS)

# Clean target
clean:
	rm -f src/*.o src/cli/*.o src/cli_commands/*.o klang
```

---

## 📋 Testing Strategy

### Unit Tests

```c
/* test_args.c */
#include "src/cli/args.h"
#include <assert.h>

void test_parse_long_flags() {
    char *argv[] = {"prog", "--verbose", "--output", "file.txt"};
    ArgSpec specs[] = {
        {"verbose", 'v', "Verbose", ARG_TYPE_NONE, NULL, false, NULL},
        {"output", 'o', "Output", ARG_TYPE_STRING, "<file>", false, NULL},
    };
    
    ParsedArgs *args = args_parse(4, argv, specs, 2);
    assert(args != NULL);
    assert(args_is_set(args, "verbose"));
    assert(strcmp(args_get_string(args, "output"), "file.txt") == 0);
    args_free(args);
}

int main() {
    test_parse_long_flags();
    printf("All tests passed!\n");
    return 0;
}
```

---

## 📚 Documentation

Update `README.md`:

```markdown
## CLI Usage

### Basic Commands

```bash
klang run app.kl        # Run a file
klang repl              # Start REPL
klang check app.kl      # Check syntax
klang fmt app.kl        # Format code
```

### Help

```bash
klang --help            # General help
klang run --help        # Command help
```

### Options

```bash
klang run --watch app.kl     # Watch mode
klang check --all .          # Check all files
klang fmt --check .          # Check formatting
```
```

---

This implementation guide provides the foundation for a professional CLI. Next steps:
1. Implement Phase 1 (args, colors, commands)
2. Refactor existing cli.c to use new system
3. Add new commands one by one
4. Test thoroughly
5. Document everything
