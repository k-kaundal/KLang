#include "type_system.h"
#include <string.h>
#include <stdio.h>

/* Global built-in types */
Type *BUILTIN_TYPE_INT = NULL;
Type *BUILTIN_TYPE_FLOAT = NULL;
Type *BUILTIN_TYPE_STRING = NULL;
Type *BUILTIN_TYPE_BOOL = NULL;
Type *BUILTIN_TYPE_NULL = NULL;
Type *BUILTIN_TYPE_VOID_T = NULL;
Type *BUILTIN_TYPE_ANY_T = NULL;

/* Initialize type system */
void type_system_init(void) {
    BUILTIN_TYPE_INT = type_create_primitive("int");
    BUILTIN_TYPE_FLOAT = type_create_primitive("float");
    BUILTIN_TYPE_STRING = type_create_primitive("string");
    BUILTIN_TYPE_BOOL = type_create_primitive("bool");
    BUILTIN_TYPE_NULL = type_create_primitive("null");
    BUILTIN_TYPE_VOID_T = type_create_void();
    BUILTIN_TYPE_ANY_T = type_create_any();
}

/* Cleanup type system */
void type_system_cleanup(void) {
    type_free(BUILTIN_TYPE_INT);
    type_free(BUILTIN_TYPE_FLOAT);
    type_free(BUILTIN_TYPE_STRING);
    type_free(BUILTIN_TYPE_BOOL);
    type_free(BUILTIN_TYPE_NULL);
    type_free(TYPE_KIND_VOID_T);
    type_free(TYPE_KIND_ANY_T);
}

/* Create primitive type */
Type* type_create_primitive(const char *name) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_PRIMITIVE;
    type->name = strdup(name);
    type->data.primitive.is_const = 0;
    return type;
}

/* Create function type */
Type* type_create_function(Type **params, int param_count, Type *return_type) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_FUNCTION;
    type->name = strdup("function");
    type->data.function.param_types = malloc(param_count * sizeof(Type*));
    for (int i = 0; i < param_count; i++) {
        type->data.function.param_types[i] = params[i];
    }
    type->data.function.param_count = param_count;
    type->data.function.return_type = return_type;
    type->data.function.is_async = 0;
    type->data.function.is_generator = 0;
    return type;
}

/* Create generic type */
Type* type_create_generic(const char *param_name, Type *constraint) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_GENERIC;
    type->name = strdup(param_name);
    type->data.generic.param_name = strdup(param_name);
    type->data.generic.constraint = constraint;
    return type;
}

/* Create union type */
Type* type_create_union(Type **types, int type_count) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_UNION;
    type->name = strdup("union");
    type->data.union_type.types = malloc(type_count * sizeof(Type*));
    for (int i = 0; i < type_count; i++) {
        type->data.union_type.types[i] = types[i];
    }
    type->data.union_type.type_count = type_count;
    return type;
}

/* Create intersection type */
Type* type_create_intersection(Type **types, int type_count) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_INTERSECTION;
    type->name = strdup("intersection");
    type->data.intersection.types = malloc(type_count * sizeof(Type*));
    for (int i = 0; i < type_count; i++) {
        type->data.intersection.types[i] = types[i];
    }
    type->data.intersection.type_count = type_count;
    return type;
}

/* Create array type */
Type* type_create_array(Type *element_type) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_ARRAY;
    type->name = strdup("array");
    type->data.array.element_type = element_type;
    return type;
}

/* Create tuple type */
Type* type_create_tuple(Type **element_types, int element_count) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_TUPLE;
    type->name = strdup("tuple");
    type->data.tuple.element_types = malloc(element_count * sizeof(Type*));
    for (int i = 0; i < element_count; i++) {
        type->data.tuple.element_types[i] = element_types[i];
    }
    type->data.tuple.element_count = element_count;
    return type;
}

/* Create object type */
Type* type_create_object(char **field_names, Type **field_types, int field_count) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_OBJECT;
    type->name = strdup("object");
    type->data.object.field_names = malloc(field_count * sizeof(char*));
    type->data.object.field_types = malloc(field_count * sizeof(Type*));
    for (int i = 0; i < field_count; i++) {
        type->data.object.field_names[i] = strdup(field_names[i]);
        type->data.object.field_types[i] = field_types[i];
    }
    type->data.object.field_count = field_count;
    return type;
}

/* Create nullable type */
Type* type_create_nullable(Type *base_type) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_NULLABLE;
    type->name = strdup("nullable");
    type->data.nullable.base_type = base_type;
    return type;
}

/* Create void type */
Type* type_create_void(void) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_VOID_T;
    type->name = strdup("void");
    return type;
}

/* Create any type */
Type* type_create_any(void) {
    Type *type = malloc(sizeof(Type));
    type->kind = TYPE_KIND_ANY_T;
    type->name = strdup("any");
    return type;
}

/* Check if two types are equal */
int type_equals(Type *a, Type *b) {
    if (!a || !b) return 0;
    if (a->kind != b->kind) return 0;
    
    switch (a->kind) {
        case TYPE_KIND_PRIMITIVE:
        case TYPE_KIND_VOID_T:
        case TYPE_KIND_ANY_T:
            return strcmp(a->name, b->name) == 0;
            
        case TYPE_KIND_ARRAY:
            return type_equals(a->data.array.element_type, b->data.array.element_type);
            
        case TYPE_KIND_NULLABLE:
            return type_equals(a->data.nullable.base_type, b->data.nullable.base_type);
            
        case TYPE_KIND_FUNCTION:
            if (a->data.function.param_count != b->data.function.param_count) return 0;
            for (int i = 0; i < a->data.function.param_count; i++) {
                if (!type_equals(a->data.function.param_types[i], b->data.function.param_types[i])) {
                    return 0;
                }
            }
            return type_equals(a->data.function.return_type, b->data.function.return_type);
            
        default:
            return 0;
    }
}

/* Check if sub is a subtype of super */
int type_is_subtype(Type *sub, Type *super) {
    if (!sub || !super) return 0;
    
    /* Any is supertype of everything */
    if (super->kind == TYPE_KIND_ANY_T) return 1;
    
    /* Nothing is subtype of void except void */
    if (super->kind == TYPE_KIND_VOID_T) return sub->kind == TYPE_KIND_VOID_T;
    
    /* Check equality */
    if (type_equals(sub, super)) return 1;
    
    /* Nullable types */
    if (super->kind == TYPE_KIND_NULLABLE) {
        return type_is_subtype(sub, super->data.nullable.base_type);
    }
    
    /* Union types - sub must be subtype of at least one member */
    if (super->kind == TYPE_KIND_UNION) {
        for (int i = 0; i < super->data.union_type.type_count; i++) {
            if (type_is_subtype(sub, super->data.union_type.types[i])) {
                return 1;
            }
        }
        return 0;
    }
    
    return 0;
}

/* Check if value of type 'from' can be assigned to variable of type 'to' */
int type_is_assignable(Type *from, Type *to) {
    return type_is_subtype(from, to);
}

/* Free type */
void type_free(Type *type) {
    if (!type) return;
    
    free(type->name);
    
    switch (type->kind) {
        case TYPE_KIND_FUNCTION:
            free(type->data.function.param_types);
            type_free(type->data.function.return_type);
            break;
            
        case TYPE_KIND_GENERIC:
            free(type->data.generic.param_name);
            type_free(type->data.generic.constraint);
            break;
            
        case TYPE_KIND_UNION:
            for (int i = 0; i < type->data.union_type.type_count; i++) {
                type_free(type->data.union_type.types[i]);
            }
            free(type->data.union_type.types);
            break;
            
        case TYPE_KIND_INTERSECTION:
            for (int i = 0; i < type->data.intersection.type_count; i++) {
                type_free(type->data.intersection.types[i]);
            }
            free(type->data.intersection.types);
            break;
            
        case TYPE_KIND_ARRAY:
            type_free(type->data.array.element_type);
            break;
            
        case TYPE_KIND_TUPLE:
            for (int i = 0; i < type->data.tuple.element_count; i++) {
                type_free(type->data.tuple.element_types[i]);
            }
            free(type->data.tuple.element_types);
            break;
            
        case TYPE_KIND_OBJECT:
            for (int i = 0; i < type->data.object.field_count; i++) {
                free(type->data.object.field_names[i]);
                type_free(type->data.object.field_types[i]);
            }
            free(type->data.object.field_names);
            free(type->data.object.field_types);
            break;
            
        case TYPE_KIND_NULLABLE:
            type_free(type->data.nullable.base_type);
            break;
            
        default:
            break;
    }
    
    free(type);
}

/* Convert type to string */
char* type_system_to_string(Type *type) {
    if (!type) return strdup("unknown");
    
    static char buffer[1024];
    
    switch (type->kind) {
        case TYPE_KIND_PRIMITIVE:
        case TYPE_KIND_VOID_T:
        case TYPE_KIND_ANY_T:
            return strdup(type->name);
            
        case TYPE_KIND_ARRAY:
            snprintf(buffer, sizeof(buffer), "%s[]", type_system_to_string(type->data.array.element_type));
            return strdup(buffer);
            
        case TYPE_KIND_NULLABLE:
            snprintf(buffer, sizeof(buffer), "%s?", type_system_to_string(type->data.nullable.base_type));
            return strdup(buffer);
            
        case TYPE_KIND_FUNCTION:
            snprintf(buffer, sizeof(buffer), "(%d params) -> %s", 
                     type->data.function.param_count,
                     type_system_to_string(type->data.function.return_type));
            return strdup(buffer);
            
        default:
            return strdup(type->name);
    }
}

/* Type context operations */
TypeContext* type_context_create(void) {
    TypeContext *ctx = malloc(sizeof(TypeContext));
    ctx->type_vars = NULL;
    ctx->type_var_count = 0;
    ctx->strict_mode = 0;
    return ctx;
}

void type_context_free(TypeContext *ctx) {
    if (!ctx) return;
    free(ctx->type_vars);
    free(ctx);
}

void type_context_add_var(TypeContext *ctx, Type *var) {
    ctx->type_var_count++;
    ctx->type_vars = realloc(ctx->type_vars, ctx->type_var_count * sizeof(Type*));
    ctx->type_vars[ctx->type_var_count - 1] = var;
}
