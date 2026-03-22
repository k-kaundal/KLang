#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER, NODE_STRING, NODE_BOOL, NODE_IDENT, NODE_NULL,
    NODE_BINOP, NODE_UNOP, NODE_CALL, NODE_INDEX,
    NODE_LET, NODE_ASSIGN, NODE_IF, NODE_WHILE, NODE_FOR, NODE_FOR_OF,
    NODE_RETURN, NODE_BREAK, NODE_CONTINUE, NODE_BLOCK, NODE_FUNC_DEF, NODE_STRUCT_DEF, NODE_IMPORT,
    NODE_LIST, NODE_OBJECT, NODE_SPREAD,
    NODE_CLASS_DEF, NODE_NEW, NODE_MEMBER_ACCESS, NODE_THIS, NODE_SUPER,
    NODE_TEMPLATE_LITERAL, NODE_TERNARY, NODE_SWITCH, NODE_CASE,
    NODE_AWAIT, NODE_YIELD,
    NODE_EXPORT, NODE_IMPORT_NAMED, NODE_IMPORT_DEFAULT, NODE_IMPORT_NAMESPACE
} NodeType;

typedef enum {
    ACCESS_PUBLIC,
    ACCESS_PRIVATE,
    ACCESS_PROTECTED
} AccessModifier;

typedef enum {
    DECL_LET,
    DECL_VAR,
    DECL_CONST
} DeclType;

typedef struct ASTNode ASTNode;

typedef struct {
    ASTNode **items;
    int count;
    int capacity;
} NodeList;

typedef struct {
    char *key;           // Property key (NULL for computed)
    ASTNode *key_expr;   // Computed key expression
    ASTNode *value;      // Property value
    int is_shorthand;    // Property shorthand
    int is_method;       // Method shorthand
} ObjectProperty;

struct ASTNode {
    NodeType type;
    int line;
    union {
        double num_val;
        char *str_val;
        int bool_val;
        struct { char *name; } ident;
        struct { char op[4]; ASTNode *left; ASTNode *right; } binop;
        struct { char op[4]; ASTNode *operand; } unop;
        struct { ASTNode *callee; NodeList args; } call;
        struct { ASTNode *obj; ASTNode *index; } index_expr;
        struct { char *name; char *type_annot; ASTNode *value; int is_static; AccessModifier access; DeclType decl_type; } let_stmt;
        struct { char *name; ASTNode *value; } assign_stmt;
        struct { ASTNode *cond; ASTNode *then_block; ASTNode *else_block; } if_stmt;
        struct { ASTNode *cond; ASTNode *body; } while_stmt;
        struct { char *var; ASTNode *start; ASTNode *end; ASTNode *body; } for_stmt;
        struct { char *var; ASTNode *iterable; ASTNode *body; DeclType decl_type; } for_of_stmt;
        struct { ASTNode *value; } return_stmt;
        struct { } break_stmt;
        struct { } continue_stmt;
        struct { NodeList stmts; } block;
        struct { char *name; NodeList params; char **param_types; char *return_type; ASTNode *body; int is_static; AccessModifier access; int is_abstract; int is_arrow; int is_async; int is_generator; int has_rest_param; } func_def;
        struct { NodeList elements; } list;
        struct { ObjectProperty *props; int count; int capacity; } object;
        struct { ASTNode *argument; } spread;
        struct { char *name; char *parent_name; NodeList members; int is_abstract; } class_def;
        struct { char *class_name; NodeList args; } new_expr;
        struct { ASTNode *obj; char *member; } member_access;
        struct { } this_expr;
        struct { char *member; } super_expr;
        struct { char **parts; ASTNode **exprs; int count; } template_literal;
        struct { ASTNode *cond; ASTNode *true_expr; ASTNode *false_expr; } ternary;
        struct { ASTNode *expr; NodeList cases; ASTNode *default_case; } switch_stmt;
        struct { ASTNode *value; NodeList body; } case_stmt;
        struct { ASTNode *expr; } await_expr;
        struct { ASTNode *value; } yield_expr;
        /* Module system nodes */
        struct {
            char **names;           // Imported names
            char **aliases;         // Aliases (for "as")
            int count;              // Number of imports
            char *module_path;      // Module to import from
        } import_named;
        struct {
            char *name;             // Default import name
            char *module_path;      // Module to import from
        } import_default;
        struct {
            char *namespace;        // Namespace name (import * as X)
            char *module_path;      // Module to import from
        } import_namespace;
        struct {
            int is_default;         // Is this a default export?
            ASTNode *declaration;   // What's being exported (func, class, var, etc)
            char **names;           // Named exports (for re-export)
            int count;              // Number of named exports
        } export_stmt;
    } data;
};

void nodelist_init(NodeList *list);
void nodelist_push(NodeList *list, ASTNode *node);
void nodelist_free(NodeList *list);

ASTNode *ast_new_number(double val, int line);
ASTNode *ast_new_string(const char *val, int line);
ASTNode *ast_new_bool(int val, int line);
ASTNode *ast_new_ident(const char *name, int line);
ASTNode *ast_new_null(int line);
ASTNode *ast_new_binop(const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_new_unop(const char *op, ASTNode *operand, int line);
ASTNode *ast_new_call(ASTNode *callee, int line);
ASTNode *ast_new_index(ASTNode *obj, ASTNode *index, int line);
ASTNode *ast_new_let(const char *name, const char *type_annot, ASTNode *value, int line);
ASTNode *ast_new_var_decl(const char *name, const char *type_annot, ASTNode *value, DeclType decl_type, int line);
ASTNode *ast_new_assign(const char *name, ASTNode *value, int line);
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_block, ASTNode *else_block, int line);
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_new_for(const char *var, ASTNode *start, ASTNode *end, ASTNode *body, int line);
ASTNode *ast_new_for_of(const char *var, ASTNode *iterable, ASTNode *body, DeclType decl_type, int line);
ASTNode *ast_new_return(ASTNode *value, int line);
ASTNode *ast_new_break(int line);
ASTNode *ast_new_continue(int line);
ASTNode *ast_new_block(int line);
ASTNode *ast_new_func_def(const char *name, const char *return_type, int line);
ASTNode *ast_new_list(int line);
ASTNode *ast_new_object(int line);
ASTNode *ast_new_spread(ASTNode *argument, int line);
void ast_object_add_property(ASTNode *obj, const char *key, ASTNode *key_expr, ASTNode *value, int is_shorthand, int is_method);
ASTNode *ast_new_class_def(const char *name, const char *parent_name, int line);
ASTNode *ast_new_new(const char *class_name, int line);
ASTNode *ast_new_member_access(ASTNode *obj, const char *member, int line);
ASTNode *ast_new_this(int line);
ASTNode *ast_new_super(const char *member, int line);
ASTNode *ast_new_template_literal(int line);
ASTNode *ast_new_ternary(ASTNode *cond, ASTNode *true_expr, ASTNode *false_expr, int line);
ASTNode *ast_new_switch(ASTNode *expr, int line);
ASTNode *ast_new_case(ASTNode *value, int line);
ASTNode *ast_new_await(ASTNode *expr, int line);
ASTNode *ast_new_yield(ASTNode *value, int line);
/* Module system constructors */
ASTNode *ast_new_import_named(char **names, char **aliases, int count, const char *module_path, int line);
ASTNode *ast_new_import_default(const char *name, const char *module_path, int line);
ASTNode *ast_new_import_namespace(const char *namespace, const char *module_path, int line);
ASTNode *ast_new_export(int is_default, ASTNode *declaration, char **names, int count, int line);
void ast_free(ASTNode *node);

#endif
