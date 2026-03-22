#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/type_checker.h"

// Symbol table for type tracking
typedef struct Symbol {
    char *name;
    Type *type;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *symbols;
    struct Scope *parent;
} Scope;

static Scope *scope_create(Scope *parent) {
    Scope *scope = malloc(sizeof(Scope));
    scope->symbols = NULL;
    scope->parent = parent;
    return scope;
}

static void scope_destroy(Scope *scope) {
    Symbol *sym = scope->symbols;
    while (sym) {
        Symbol *next = sym->next;
        free(sym->name);
        type_destroy(sym->type);
        free(sym);
        sym = next;
    }
    free(scope);
}

static void scope_add_symbol(Scope *scope, const char *name, Type *type) {
    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->next = scope->symbols;
    scope->symbols = sym;
}

static Type *scope_lookup(Scope *scope, const char *name) {
    for (Scope *s = scope; s != NULL; s = s->parent) {
        for (Symbol *sym = s->symbols; sym != NULL; sym = sym->next) {
            if (strcmp(sym->name, name) == 0) {
                return sym->type;
            }
        }
    }
    return NULL;
}

// Type creation and management
Type *type_create(TypeKind kind) {
    Type *type = malloc(sizeof(Type));
    type->kind = kind;
    type->name = NULL;
    type->element_type = NULL;
    type->param_types = NULL;
    type->return_type = NULL;
    type->param_count = 0;
    return type;
}

Type *type_from_string(const char *type_str) {
    if (!type_str) return type_create(TYPE_ANY);
    
    if (strcmp(type_str, "int") == 0) return type_create(TYPE_INT);
    if (strcmp(type_str, "float") == 0) return type_create(TYPE_FLOAT);
    if (strcmp(type_str, "string") == 0) return type_create(TYPE_STRING);
    if (strcmp(type_str, "bool") == 0) return type_create(TYPE_BOOL);
    if (strcmp(type_str, "null") == 0) return type_create(TYPE_NULL);
    if (strcmp(type_str, "void") == 0) return type_create(TYPE_VOID);
    if (strcmp(type_str, "any") == 0) return type_create(TYPE_ANY);
    
    // Array types
    if (strstr(type_str, "[]")) {
        Type *type = type_create(TYPE_ARRAY);
        char *base = strndup(type_str, strlen(type_str) - 2);
        type->element_type = type_from_string(base);
        free(base);
        return type;
    }
    
    return type_create(TYPE_UNKNOWN);
}

void type_destroy(Type *type) {
    if (!type) return;
    free(type->name);
    type_destroy(type->element_type);
    type_destroy(type->return_type);
    if (type->param_types) {
        for (int i = 0; i < type->param_count; i++) {
            type_destroy(type->param_types[i]);
        }
        free(type->param_types);
    }
    free(type);
}

int types_compatible(Type *expected, Type *actual) {
    if (!expected || !actual) return 0;
    if (expected->kind == TYPE_ANY || actual->kind == TYPE_ANY) return 1;
    if (expected->kind == TYPE_UNKNOWN || actual->kind == TYPE_UNKNOWN) return 1;
    
    if (expected->kind != actual->kind) {
        // Allow int to float conversion
        if (expected->kind == TYPE_FLOAT && actual->kind == TYPE_INT) return 1;
        return 0;
    }
    
    if (expected->kind == TYPE_ARRAY) {
        return types_compatible(expected->element_type, actual->element_type);
    }
    
    return 1;
}

const char *type_to_string(Type *type) {
    if (!type) return "unknown";
    
    switch (type->kind) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_STRING: return "string";
        case TYPE_BOOL: return "bool";
        case TYPE_NULL: return "null";
        case TYPE_ARRAY: return "array";
        case TYPE_TUPLE: return "tuple";
        case TYPE_DICT: return "dict";
        case TYPE_OBJECT: return "object";
        case TYPE_FUNCTION: return "function";
        case TYPE_ANY: return "any";
        case TYPE_VOID: return "void";
        default: return "unknown";
    }
}

// Type checker implementation
TypeChecker *type_checker_create(void) {
    TypeChecker *checker = malloc(sizeof(TypeChecker));
    checker->error_count = 0;
    checker->strict_mode = 0;
    return checker;
}

void type_checker_destroy(TypeChecker *checker) {
    free(checker);
}

void type_checker_set_strict(TypeChecker *checker, int strict) {
    checker->strict_mode = strict;
}

static void type_error(TypeChecker *checker, int line, const char *msg) {
    fprintf(stderr, "Type error at line %d: %s\n", line, msg);
    checker->error_count++;
}

static Type *infer_type(TypeChecker *checker, ASTNode *node, Scope *scope);

static Type *check_binop(TypeChecker *checker, ASTNode *node, Scope *scope) {
    Type *left = infer_type(checker, node->data.binop.left, scope);
    Type *right = infer_type(checker, node->data.binop.right, scope);
    
    const char *op = node->data.binop.op;
    
    // Arithmetic operators
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || 
        strcmp(op, "%") == 0) {
        
        if (strcmp(op, "+") == 0 && 
            (left->kind == TYPE_STRING || right->kind == TYPE_STRING)) {
            return type_create(TYPE_STRING);
        }
        
        if ((left->kind == TYPE_INT || left->kind == TYPE_FLOAT) &&
            (right->kind == TYPE_INT || right->kind == TYPE_FLOAT)) {
            if (left->kind == TYPE_FLOAT || right->kind == TYPE_FLOAT) {
                return type_create(TYPE_FLOAT);
            }
            return type_create(TYPE_INT);
        }
        
        char msg[256];
        snprintf(msg, sizeof(msg), "Invalid operands for '%s': %s and %s",
                 op, type_to_string(left), type_to_string(right));
        type_error(checker, node->line, msg);
        return type_create(TYPE_UNKNOWN);
    }
    
    // Comparison operators
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
        strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) {
        return type_create(TYPE_BOOL);
    }
    
    // Logical operators
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        return type_create(TYPE_BOOL);
    }
    
    return type_create(TYPE_UNKNOWN);
}

static Type *infer_type(TypeChecker *checker, ASTNode *node, Scope *scope) {
    if (!node) return type_create(TYPE_VOID);
    
    switch (node->type) {
        case NODE_NUMBER: {
            // Check if it's a float or int
            double val = node->data.num_val;
            if (val == (int)val) {
                return type_create(TYPE_INT);
            }
            return type_create(TYPE_FLOAT);
        }
        
        case NODE_STRING:
            return type_create(TYPE_STRING);
        
        case NODE_BOOL:
            return type_create(TYPE_BOOL);
        
        case NODE_NULL:
            return type_create(TYPE_NULL);
        
        case NODE_IDENT: {
            Type *type = scope_lookup(scope, node->data.ident.name);
            if (!type) {
                char msg[256];
                snprintf(msg, sizeof(msg), "Undefined variable '%s'", node->data.ident.name);
                type_error(checker, node->line, msg);
                return type_create(TYPE_UNKNOWN);
            }
            return type;
        }
        
        case NODE_BINOP:
            return check_binop(checker, node, scope);
        
        case NODE_UNOP: {
            Type *operand_type = infer_type(checker, node->data.unop.operand, scope);
            const char *op = node->data.unop.op;
            
            if (strcmp(op, "-") == 0 || strcmp(op, "+") == 0) {
                if (operand_type->kind != TYPE_INT && operand_type->kind != TYPE_FLOAT) {
                    char msg[256];
                    snprintf(msg, sizeof(msg), "Unary '%s' requires numeric operand", op);
                    type_error(checker, node->line, msg);
                }
                return operand_type;
            }
            
            if (strcmp(op, "!") == 0) {
                return type_create(TYPE_BOOL);
            }
            
            return type_create(TYPE_UNKNOWN);
        }
        
        case NODE_LIST:
            return type_create(TYPE_ARRAY);
        
        case NODE_TUPLE:
            return type_create(TYPE_TUPLE);
        
        case NODE_OBJECT:
            return type_create(TYPE_OBJECT);
        
        case NODE_CALL: {
            Type *callee_type = infer_type(checker, node->data.call.callee, scope);
            if (callee_type->kind != TYPE_FUNCTION && callee_type->kind != TYPE_UNKNOWN) {
                type_error(checker, node->line, "Calling non-function");
            }
            return callee_type->return_type ? callee_type->return_type : type_create(TYPE_ANY);
        }
        
        case NODE_TERNARY: {
            Type *cond_type = infer_type(checker, node->data.ternary.cond, scope);
            Type *true_type = infer_type(checker, node->data.ternary.true_expr, scope);
            Type *false_type = infer_type(checker, node->data.ternary.false_expr, scope);
            
            if (types_compatible(true_type, false_type)) {
                return true_type;
            }
            return type_create(TYPE_ANY);
        }
        
        case NODE_MEMBER_ACCESS:
            return type_create(TYPE_ANY);  // Would need object type info
        
        case NODE_INDEX:
            return type_create(TYPE_ANY);  // Would need array element type
        
        default:
            return type_create(TYPE_UNKNOWN);
    }
}

static int check_statement(TypeChecker *checker, ASTNode *node, Scope *scope);

static int check_block(TypeChecker *checker, ASTNode *node, Scope *parent_scope) {
    Scope *scope = scope_create(parent_scope);
    
    for (int i = 0; i < node->data.block.stmts.count; i++) {
        check_statement(checker, node->data.block.stmts.items[i], scope);
    }
    
    scope_destroy(scope);
    return checker->error_count == 0;
}

static int check_statement(TypeChecker *checker, ASTNode *node, Scope *scope) {
    if (!node) return 1;
    
    switch (node->type) {
        case NODE_LET: {
            Type *value_type = infer_type(checker, node->data.let_stmt.value, scope);
            Type *declared_type = type_from_string(node->data.let_stmt.type_annot);
            
            if (node->data.let_stmt.type_annot && !types_compatible(declared_type, value_type)) {
                char msg[256];
                snprintf(msg, sizeof(msg), 
                         "Type mismatch for '%s': expected %s, got %s",
                         node->data.let_stmt.name,
                         type_to_string(declared_type),
                         type_to_string(value_type));
                type_error(checker, node->line, msg);
            }
            
            scope_add_symbol(scope, node->data.let_stmt.name, 
                           declared_type->kind != TYPE_UNKNOWN ? declared_type : value_type);
            break;
        }
        
        case NODE_ASSIGN: {
            Type *var_type = scope_lookup(scope, node->data.assign_stmt.name);
            if (!var_type) {
                char msg[256];
                snprintf(msg, sizeof(msg), "Undefined variable '%s'", node->data.assign_stmt.name);
                type_error(checker, node->line, msg);
                break;
            }
            
            Type *value_type = infer_type(checker, node->data.assign_stmt.value, scope);
            if (!types_compatible(var_type, value_type)) {
                char msg[256];
                snprintf(msg, sizeof(msg),
                         "Cannot assign %s to variable of type %s",
                         type_to_string(value_type),
                         type_to_string(var_type));
                type_error(checker, node->line, msg);
            }
            break;
        }
        
        case NODE_IF:
            infer_type(checker, node->data.if_stmt.cond, scope);
            check_statement(checker, node->data.if_stmt.then_block, scope);
            if (node->data.if_stmt.else_block) {
                check_statement(checker, node->data.if_stmt.else_block, scope);
            }
            break;
        
        case NODE_WHILE:
            infer_type(checker, node->data.while_stmt.cond, scope);
            check_statement(checker, node->data.while_stmt.body, scope);
            break;
        
        case NODE_FOR:
            check_statement(checker, node->data.for_stmt.body, scope);
            break;
        
        case NODE_FOR_OF:
            check_statement(checker, node->data.for_of_stmt.body, scope);
            break;
        
        case NODE_FOR_C_STYLE: {
            Scope *for_scope = scope_create(scope);
            if (node->data.for_c_style_stmt.init) {
                check_statement(checker, node->data.for_c_style_stmt.init, for_scope);
            }
            if (node->data.for_c_style_stmt.cond) {
                infer_type(checker, node->data.for_c_style_stmt.cond, for_scope);
            }
            if (node->data.for_c_style_stmt.update) {
                infer_type(checker, node->data.for_c_style_stmt.update, for_scope);
            }
            check_statement(checker, node->data.for_c_style_stmt.body, for_scope);
            scope_destroy(for_scope);
            break;
        }
        
        case NODE_RETURN: {
            if (node->data.return_stmt.value) {
                Type *return_type = infer_type(checker, node->data.return_stmt.value, scope);
                // Would need to check against function return type
                (void)return_type;
            }
            break;
        }
        
        case NODE_FUNC_DEF: {
            Type *func_type = type_create(TYPE_FUNCTION);
            func_type->return_type = type_from_string(node->data.func_def.return_type);
            
            scope_add_symbol(scope, node->data.func_def.name, func_type);
            
            Scope *func_scope = scope_create(scope);
            
            // Add parameters to function scope
            for (int i = 0; i < node->data.func_def.params.count; i++) {
                ASTNode *param = node->data.func_def.params.items[i];
                Type *param_type = type_from_string(node->data.func_def.param_types ? 
                                                   node->data.func_def.param_types[i] : NULL);
                scope_add_symbol(func_scope, param->data.ident.name, param_type);
            }
            
            check_statement(checker, node->data.func_def.body, func_scope);
            scope_destroy(func_scope);
            break;
        }
        
        case NODE_BLOCK:
            check_block(checker, node, scope);
            break;
        
        case NODE_BREAK:
        case NODE_CONTINUE:
            break;
        
        default:
            // Expression statement
            infer_type(checker, node, scope);
            break;
    }
    
    return checker->error_count == 0;
}

int type_check_ast(TypeChecker *checker, ASTNode *node) {
    if (!node) return 1;
    
    checker->error_count = 0;
    Scope *global_scope = scope_create(NULL);
    
    // Add built-in functions
    Type *print_type = type_create(TYPE_FUNCTION);
    print_type->return_type = type_create(TYPE_VOID);
    scope_add_symbol(global_scope, "print", print_type);
    scope_add_symbol(global_scope, "println", print_type);
    
    int result = check_statement(checker, node, global_scope);
    
    scope_destroy(global_scope);
    
    if (checker->error_count > 0) {
        fprintf(stderr, "\nType checking failed with %d error(s).\n", checker->error_count);
        return 0;
    }
    
    printf("Type checking passed successfully.\n");
    return 1;
}
