#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cli_commands.h"
#include "cli_colors.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

/* Helper to validate file extension */
static int validate_file_extension(const char *path) {
    const char *ext = strrchr(path, '.');
    if (ext == NULL) {
        return 0;
    }
    return (strcmp(ext, ".kl") == 0 || strcmp(ext, ".k") == 0 || strcmp(ext, ".klang") == 0);
}

/* Helper to read file content */
static char *read_file(const char *path) {
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

void cmd_check_file(const char *path) {
    char *source;
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    int i;
    
    printf("\n%sChecking:%s %s\n", 
           get_color(COLOR_BOLD_CYAN), 
           get_color(COLOR_RESET), 
           path);
    
    /* Validate extension */
    if (!validate_file_extension(path)) {
        print_error("Invalid file extension. KLang files must have .kl, .k, or .klang extension");
        fprintf(stderr, "Given file: %s\n", path);
        return;
    }
    
    /* Read file */
    source = read_file(path);
    if (!source) {
        print_error("Cannot open file");
        fprintf(stderr, "File: %s\n", path);
        return;
    }

    /* Lexical analysis */
    printf("%s→%s Running lexical analysis...\n", 
           get_color(COLOR_CYAN), get_color(COLOR_RESET));
    lexer_init(&lexer, source);
    
    /* Parsing */
    printf("%s→%s Running syntax analysis...\n", 
           get_color(COLOR_CYAN), get_color(COLOR_RESET));
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);

    if (parser.had_error) {
        print_error("Syntax check failed");
        printf("\n%sSummary:%s Found syntax errors in %s\n", 
               get_color(COLOR_BOLD_RED), 
               get_color(COLOR_RESET), 
               path);
    } else {
        print_success("Syntax check passed");
        printf("\n%sSummary:%s No syntax errors found in %s\n", 
               get_color(COLOR_BOLD_GREEN), 
               get_color(COLOR_RESET), 
               path);
        printf("  • %d statement(s) parsed successfully\n", count);
    }

    /* Cleanup */
    for (i = 0; i < count; i++) {
        if (nodes[i]) ast_free(nodes[i]);
    }
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
}

void cmd_info_file(const char *path) {
    char *source;
    struct stat st;
    int lines = 0;
    int non_empty_lines = 0;
    int comment_lines = 0;
    char *p;
    
    printf("\n%sFile Information:%s %s\n\n", 
           get_color(COLOR_BOLD_CYAN), 
           get_color(COLOR_RESET), 
           path);
    
    /* Validate extension */
    if (!validate_file_extension(path)) {
        print_error("Invalid file extension. KLang files must have .kl, .k, or .klang extension");
        return;
    }
    
    /* Get file stats */
    if (stat(path, &st) != 0) {
        print_error("Cannot access file");
        return;
    }
    
    /* Read file */
    source = read_file(path);
    if (!source) {
        print_error("Cannot read file");
        return;
    }
    
    /* Count lines and analyze */
    p = source;
    lines = 1; /* Start with 1 for the first line */
    while (*p) {
        if (*p == '\n') {
            lines++;
        }
        p++;
    }
    
    /* Count non-empty and comment lines */
    p = source;
    int in_line = 1;
    int line_has_content = 0;
    int line_is_comment = 0;
    
    while (*p) {
        if (*p == '\n') {
            if (line_has_content && !line_is_comment) {
                non_empty_lines++;
            }
            if (line_is_comment) {
                comment_lines++;
            }
            in_line = 1;
            line_has_content = 0;
            line_is_comment = 0;
            p++;
            continue;
        }
        
        if (in_line) {
            if (*p == ' ' || *p == '\t') {
                p++;
                continue;
            }
            if (*p == '#' || (*p == '/' && (*(p+1) == '/' || *(p+1) == '*'))) {
                line_is_comment = 1;
            }
            line_has_content = 1;
            in_line = 0;
        }
        p++;
    }
    
    /* Handle last line if no trailing newline */
    if (line_has_content && !line_is_comment) {
        non_empty_lines++;
    }
    if (line_is_comment) {
        comment_lines++;
    }
    
    /* Display information */
    printf("%sGeneral:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("  Path:         %s\n", path);
    printf("  Size:         %ld bytes\n", (long)st.st_size);
    printf("  Extension:    %s\n", strrchr(path, '.'));
    
    printf("\n%sStatistics:%s\n", get_color(COLOR_BOLD_WHITE), get_color(COLOR_RESET));
    printf("  Total lines:       %d\n", lines);
    printf("  Code lines:        %d\n", non_empty_lines);
    printf("  Comment lines:     %d\n", comment_lines);
    printf("  Empty lines:       %d\n", lines - non_empty_lines - comment_lines);
    
    if (lines > 0) {
        int comment_percentage = (comment_lines * 100) / lines;
        printf("  Comment ratio:     %d%%\n", comment_percentage);
    }
    
    printf("\n");
    
    free(source);
}
