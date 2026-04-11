#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "ast.h"

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_NULL,
    TYPE_ARRAY,
    TYPE_TUPLE,
    TYPE_DICT,
    TYPE_OBJECT,
    TYPE_FUNCTION,
    TYPE_ANY,
    TYPE_UNKNOWN,
    TYPE_VOID
} TypeKind;

typedef struct Type {
    TypeKind kind;
    char *name;  // For named types
    struct Type *element_type;  // For arrays
    struct Type **param_types;  // For functions
    struct Type *return_type;   // For functions
    int param_count;
} Type;

typedef struct TypeChecker {
    int error_count;
    int strict_mode;
} TypeChecker;

// Type checker API
TypeChecker *type_checker_create(void);
void type_checker_destroy(TypeChecker *checker);
int type_check_ast(TypeChecker *checker, ASTNode *node);
void type_checker_set_strict(TypeChecker *checker, int strict);

// Type utilities
Type *type_create(TypeKind kind);
Type *type_from_string(const char *type_str);
void type_destroy(Type *type);
int types_compatible(Type *expected, Type *actual);
const char *type_to_string(Type *type);

#endif
