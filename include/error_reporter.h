#ifndef ERROR_REPORTER_H
#define ERROR_REPORTER_H

/* General error reporting with source context */
void report_error_with_context(
    const char *source,
    int line,
    int column,
    const char *error_type,
    const char *message,
    const char *suggestion
);

/* Specific error types */
void report_syntax_error(
    const char *source,
    int line,
    int column,
    const char *found,
    const char *expected
);

void report_undefined_variable(
    const char *source,
    int line,
    int column,
    const char *var_name
);

void report_type_error(
    const char *source,
    int line,
    int column,
    const char *operation,
    const char *actual_type,
    const char *expected_type
);

void report_redeclaration_error(
    const char *source,
    int line,
    int column,
    const char *var_name
);

void report_const_assignment_error(
    const char *source,
    int line,
    int column,
    const char *var_name
);

#endif /* ERROR_REPORTER_H */
