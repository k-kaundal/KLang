#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "test_runner.h"
#include "cli_colors.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "runtime.h"

typedef struct {
    int total;
    int passed;
    int failed;
    int skipped;
} TestStats;

static TestStats test_stats = {0, 0, 0, 0};

static int is_test_file(const char *filename) {
    size_t len = strlen(filename);
    
    /* Check for _test.kl, _test.k, .test.kl, .test.k patterns */
    if (len > 8 && strcmp(filename + len - 8, "_test.kl") == 0) return 1;
    if (len > 7 && strcmp(filename + len - 7, "_test.k") == 0) return 1;
    if (len > 8 && strcmp(filename + len - 8, ".test.kl") == 0) return 1;
    if (len > 7 && strcmp(filename + len - 7, ".test.k") == 0) return 1;
    if (len > 11 && strcmp(filename + len - 11, "_test.klang") == 0) return 1;
    if (len > 11 && strcmp(filename + len - 11, ".test.klang") == 0) return 1;
    
    return 0;
}

static char* read_file(const char *path) {
    FILE *f = fopen(path, "r");
    long size;
    char *buf;
    size_t read_bytes;
    
    if (!f) {
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

static int run_test_file(const char *path) {
    char *source;
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    int i;
    int had_error = 0;
    Interpreter *interp;
    
    printf("  %s%s%s... ",
           get_color(COLOR_CYAN),
           path,
           get_color(COLOR_RESET));
    fflush(stdout);
    
    source = read_file(path);
    if (!source) {
        printf("%s[SKIP]%s Cannot read file\n",
               get_color(COLOR_YELLOW),
               get_color(COLOR_RESET));
        test_stats.skipped++;
        return 0;
    }
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    
    if (parser.had_error) {
        printf("%s[FAIL]%s Parse error\n",
               get_color(COLOR_RED),
               get_color(COLOR_RESET));
        test_stats.failed++;
        had_error = 1;
        goto cleanup;
    }
    
    /* Run the test */
    interp = interpreter_new();
    runtime_init(interp);
    
    for (i = 0; i < count; i++) {
        Value result = eval_node(interp, nodes[i]);
        value_free(&result);
        if (interp->had_error) {
            had_error = 1;
            break;
        }
    }
    
    if (had_error) {
        printf("%s[FAIL]%s Runtime error\n",
               get_color(COLOR_RED),
               get_color(COLOR_RESET));
        test_stats.failed++;
    } else {
        printf("%s[PASS]%s\n",
               get_color(COLOR_GREEN),
               get_color(COLOR_RESET));
        test_stats.passed++;
    }
    
    interpreter_free(interp);
    
cleanup:
    for (i = 0; i < count; i++) {
        if (nodes[i]) ast_free(nodes[i]);
    }
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
    
    return !had_error;
}

static void discover_and_run_tests(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat path_stat;
    char full_path[1024];
    
    dir = opendir(dir_path);
    if (!dir) {
        fprintf(stderr, "Cannot open directory: %s\n", dir_path);
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        /* Skip . and .. */
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        if (stat(full_path, &path_stat) != 0) {
            continue;
        }
        
        if (S_ISDIR(path_stat.st_mode)) {
            /* Recursively search subdirectories */
            discover_and_run_tests(full_path);
        } else if (S_ISREG(path_stat.st_mode) && is_test_file(entry->d_name)) {
            test_stats.total++;
            run_test_file(full_path);
        }
    }
    
    closedir(dir);
}

void run_tests(const char *path) {
    struct stat path_stat;
    
    /* Reset stats */
    test_stats.total = 0;
    test_stats.passed = 0;
    test_stats.failed = 0;
    test_stats.skipped = 0;
    
    printf("\n%s=== KLang Test Runner ===%s\n\n",
           get_color(COLOR_BOLD_CYAN),
           get_color(COLOR_RESET));
    
    if (stat(path, &path_stat) != 0) {
        print_error("Path does not exist");
        fprintf(stderr, "Path: %s\n", path);
        return;
    }
    
    if (S_ISDIR(path_stat.st_mode)) {
        printf("Discovering tests in %s%s%s...\n\n",
               get_color(COLOR_CYAN),
               path,
               get_color(COLOR_RESET));
        discover_and_run_tests(path);
    } else if (S_ISREG(path_stat.st_mode) && is_test_file(path)) {
        test_stats.total++;
        run_test_file(path);
    } else {
        print_error("Not a test file or directory");
        fprintf(stderr, "Path: %s\n", path);
        return;
    }
    
    /* Print summary */
    printf("\n%s=== Test Summary ===%s\n",
           get_color(COLOR_BOLD_WHITE),
           get_color(COLOR_RESET));
    printf("  Total:   %d\n", test_stats.total);
    printf("  %sPassed:  %d%s\n",
           get_color(COLOR_GREEN),
           test_stats.passed,
           get_color(COLOR_RESET));
    
    if (test_stats.failed > 0) {
        printf("  %sFailed:  %d%s\n",
               get_color(COLOR_RED),
               test_stats.failed,
               get_color(COLOR_RESET));
    }
    
    if (test_stats.skipped > 0) {
        printf("  %sSkipped: %d%s\n",
               get_color(COLOR_YELLOW),
               test_stats.skipped,
               get_color(COLOR_RESET));
    }
    
    printf("\n");
    
    if (test_stats.failed > 0) {
        exit(1);
    }
}
