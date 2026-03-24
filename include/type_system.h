#ifndef TYPE_SYSTEM_H
#define TYPE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Type kinds */
typedef enum {
    TYPE_KIND_PRIMITIVE,    /* int, float, string, bool */
    TYPE_KIND_FUNCTION,     /* function types */
    TYPE_KIND_GENERIC,      /* generic type parameter */
    TYPE_KIND_UNION,        /* union types A | B */
    TYPE_KIND_INTERSECTION, /* intersection types A & B */
    TYPE_KIND_ARRAY,        /* array types */
    TYPE_KIND_TUPLE,        /* tuple types */
    TYPE_KIND_OBJECT,       /* object/class types */
    TYPE_KIND_NULLABLE,     /* nullable types T? */
    TYPE_KIND_VOID_T,         /* void type */
    TYPE_KIND_ANY_T           /* any type (top type) */
} TypeKind;

/* Type structure */
typedef struct Type {
    TypeKind kind;
    char *name;
    
    union {
        /* For primitive types */
        struct {
            int is_const;
        } primitive;
        
        /* For function types */
        struct {
            struct Type **param_types;
            int param_count;
            struct Type *return_type;
            int is_async;
            int is_generator;
        } function;
        
        /* For generic types */
        struct {
            char *param_name;
            struct Type *constraint;  /* optional constraint */
        } generic;
        
        /* For union types */
        struct {
            struct Type **types;
            int type_count;
        } union_type;
        
        /* For intersection types */
        struct {
            struct Type **types;
            int type_count;
        } intersection;
        
        /* For array types */
        struct {
            struct Type *element_type;
        } array;
        
        /* For tuple types */
        struct {
            struct Type **element_types;
            int element_count;
        } tuple;
        
        /* For object types */
        struct {
            char **field_names;
            struct Type **field_types;
            int field_count;
        } object;
        
        /* For nullable types */
        struct {
            struct Type *base_type;
        } nullable;
    } data;
} Type;

/* Type context for type checking */
typedef struct {
    Type **type_vars;  /* Type variables in scope */
    int type_var_count;
    int strict_mode;   /* Strict type checking */
} TypeContext;

/* Create types */
Type* type_create_primitive(const char *name);
Type* type_create_function(Type **params, int param_count, Type *return_type);
Type* type_create_generic(const char *param_name, Type *constraint);
Type* type_create_union(Type **types, int type_count);
Type* type_create_intersection(Type **types, int type_count);
Type* type_create_array(Type *element_type);
Type* type_create_tuple(Type **element_types, int element_count);
Type* type_create_object(char **field_names, Type **field_types, int field_count);
Type* type_create_nullable(Type *base_type);
Type* type_create_void(void);
Type* type_create_any(void);

/* Type operations */
int type_equals(Type *a, Type *b);
int type_is_subtype(Type *sub, Type *super);
int type_is_assignable(Type *from, Type *to);
Type* type_union(Type *a, Type *b);
Type* type_intersect(Type *a, Type *b);
Type* type_substitute(Type *type, char *var_name, Type *replacement);

/* Type inference */
Type* type_infer_expression(void *expr, TypeContext *ctx);
Type* type_infer_binary_op(Type *left, Type *right, const char *op);
Type* type_infer_call(Type *func_type, Type **arg_types, int arg_count);

/* Type checking */
int type_check_assignment(Type *target, Type *value);
int type_check_function_call(Type *func, Type **args, int arg_count);
int type_check_return(Type *expected, Type *actual);

/* Type utilities */
char* type_system_to_string(Type *type);
void type_free(Type *type);
Type* type_clone(Type *type);

/* Type context */
TypeContext* type_context_create(void);
void type_context_free(TypeContext *ctx);
void type_context_add_var(TypeContext *ctx, Type *var);

/* Built-in types */
extern Type *BUILTIN_TYPE_INT;
extern Type *BUILTIN_TYPE_FLOAT;
extern Type *BUILTIN_TYPE_STRING;
extern Type *BUILTIN_TYPE_BOOL;
extern Type *BUILTIN_TYPE_NULL;
extern Type *BUILTIN_TYPE_VOID_T;
extern Type *BUILTIN_TYPE_ANY_T;

/* Initialize type system */
void type_system_init(void);
void type_system_cleanup(void);

#endif /* TYPE_SYSTEM_H */
