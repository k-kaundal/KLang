#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_reporter.h"
#include "cli_colors.h"

#define MAX_LINE_LENGTH 1024

/* Get the line from source at given line number */
static char* get_source_line(const char *source, int target_line) {
    static char line_buffer[MAX_LINE_LENGTH];
    const char *p = source;
    int current_line = 1;
    int pos = 0;
    
    /* Skip to target line */
    while (*p && current_line < target_line) {
        if (*p == '\n') current_line++;
        p++;
    }
    
    /* Copy the line */
    while (*p && *p != '\n' && pos < MAX_LINE_LENGTH - 1) {
        line_buffer[pos++] = *p++;
    }
    line_buffer[pos] = '\0';
    
    return line_buffer;
}

/* Count leading spaces/tabs */
static int count_leading_whitespace(const char *line) {
    int count = 0;
    while (line[count] == ' ' || line[count] == '\t') {
        count++;
    }
    return count;
}

void report_error_with_context(
    const char *source,
    int line,
    int column,
    const char *error_type,
    const char *message,
    const char *suggestion
) {
    char *source_line;
    int i;
    int leading_ws;  /* Keep this for use below */
    
    /* Print error header */
    printf("\n%s%s%s at line %d, column %d:\n",
           get_color(COLOR_BOLD_RED),
           error_type,
           get_color(COLOR_RESET),
           line,
           column);
    
    printf("%s%s%s\n",
           get_color(COLOR_RED),
           message,
           get_color(COLOR_RESET));
    
    /* Get and display source line */
    source_line = get_source_line(source, line);
    leading_ws = count_leading_whitespace(source_line);
    (void)leading_ws;  /* May be used in future for better formatting */
    
    printf("\n%s%4d |%s ", get_color(COLOR_BLUE), line, get_color(COLOR_RESET));
    printf("%s\n", source_line);
    
    /* Print pointer to error location */
    printf("     %s|%s ", get_color(COLOR_BLUE), get_color(COLOR_RESET));
    
    /* Add spacing to point to the column */
    for (i = 0; i < column - 1; i++) {
        printf(" ");
    }
    
    printf("%s^%s", get_color(COLOR_RED), get_color(COLOR_RESET));
    
    /* Add underline for emphasis */
    printf("%s~~~%s", get_color(COLOR_RED), get_color(COLOR_RESET));
    printf("\n");
    
    /* Print suggestion if available */
    if (suggestion && suggestion[0] != '\0') {
        printf("\n%sHint:%s %s\n",
               get_color(COLOR_CYAN),
               get_color(COLOR_RESET),
               suggestion);
    }
    
    printf("\n");
}

void report_syntax_error(
    const char *source,
    int line,
    int column,
    const char *found,
    const char *expected
) {
    char message[512];
    char suggestion[512];
    
    snprintf(message, sizeof(message),
             "Unexpected token '%s', expected %s",
             found, expected);
    
    snprintf(suggestion, sizeof(suggestion),
             "Check for missing semicolons, braces, or parentheses before this line");
    
    report_error_with_context(source, line, column, "Syntax Error", message, suggestion);
}

void report_undefined_variable(
    const char *source,
    int line,
    int column,
    const char *var_name
) {
    char message[512];
    char suggestion[512];
    
    snprintf(message, sizeof(message),
             "Undefined variable '%s'",
             var_name);
    
    snprintf(suggestion, sizeof(suggestion),
             "Did you forget to declare '%s' with let, var, or const?",
             var_name);
    
    report_error_with_context(source, line, column, "Reference Error", message, suggestion);
}

void report_type_error(
    const char *source,
    int line,
    int column,
    const char *operation,
    const char *actual_type,
    const char *expected_type
) {
    char message[512];
    char suggestion[512];
    
    snprintf(message, sizeof(message),
             "Type mismatch in %s: got %s, expected %s",
             operation, actual_type, expected_type);
    
    snprintf(suggestion, sizeof(suggestion),
             "Convert the value to %s or use a different operation",
             expected_type);
    
    report_error_with_context(source, line, column, "Type Error", message, suggestion);
}

void report_redeclaration_error(
    const char *source,
    int line,
    int column,
    const char *var_name
) {
    char message[512];
    char suggestion[512];
    
    snprintf(message, sizeof(message),
             "Variable '%s' has already been declared",
             var_name);
    
    snprintf(suggestion, sizeof(suggestion),
             "Use a different variable name or use assignment (=) instead of declaration");
    
    report_error_with_context(source, line, column, "Declaration Error", message, suggestion);
}

void report_const_assignment_error(
    const char *source,
    int line,
    int column,
    const char *var_name
) {
    char message[512];
    char suggestion[512];
    
    snprintf(message, sizeof(message),
             "Cannot assign to const variable '%s'",
             var_name);
    
    snprintf(suggestion, sizeof(suggestion),
             "Const variables cannot be modified after declaration. Use 'let' if you need to modify the value");
    
    report_error_with_context(source, line, column, "Assignment Error", message, suggestion);
}
