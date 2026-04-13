#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER, NODE_STRING, NODE_BOOL, NODE_IDENT, NODE_NULL,
    NODE_BINOP, NODE_UNOP, NODE_CALL, NODE_INDEX,
    NODE_LET, NODE_ASSIGN, NODE_IF, NODE_WHILE, NODE_FOR, NODE_FOR_OF, NODE_FOR_C_STYLE,
    NODE_RETURN, NODE_BREAK, NODE_CONTINUE, NODE_BLOCK, NODE_FUNC_DEF, NODE_STRUCT_DEF, NODE_UNION_DEF, NODE_IMPORT,
    NODE_LIST, NODE_OBJECT, NODE_SPREAD, NODE_TUPLE,
    NODE_CLASS_DEF, NODE_NEW, NODE_MEMBER_ACCESS, NODE_THIS, NODE_SUPER,
    NODE_TEMPLATE_LITERAL, NODE_TERNARY, NODE_SWITCH, NODE_CASE,
    NODE_AWAIT, NODE_YIELD,
    NODE_EXPORT, NODE_IMPORT_NAMED, NODE_IMPORT_DEFAULT, NODE_IMPORT_NAMESPACE,
    NODE_DESTRUCTURE_ARRAY, NODE_DESTRUCTURE_OBJECT, NODE_DESTRUCTURE_ELEMENT,
    NODE_TRY_CATCH, NODE_THROW,
    NODE_POSTFIX, NODE_OPTIONAL_CHAIN, NODE_NULLISH_COALESCE,
    NODE_ADDRESS_OF, NODE_DEREFERENCE, NODE_POINTER_MEMBER,
    NODE_STRUCT_LITERAL,
    NODE_UNSAFE_BLOCK  /* Security: unsafe block */
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

typedef struct {
    char *name;          // Field name
    char *type;          // Field type annotation (e.g., "int", "float", "*Point")
    ASTNode *default_value; // Optional default value
} StructField;

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
        struct { ASTNode *init; ASTNode *cond; ASTNode *update; ASTNode *body; } for_c_style_stmt;
        struct { ASTNode *value; } return_stmt;
        struct { } break_stmt;
        struct { } continue_stmt;
        struct { NodeList stmts; } block;
        struct { char *name; NodeList params; char **param_types; ASTNode **default_values; char *return_type; ASTNode *body; int is_static; AccessModifier access; int is_abstract; int is_arrow; int is_async; int is_generator; int has_rest_param; } func_def;
        struct { NodeList elements; } list;
        struct { NodeList elements; } tuple;
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
        /* Destructuring nodes */
        struct {
            NodeList elements;      // Array of destructure elements
            ASTNode *source;        // Expression to destructure from
            DeclType decl_type;     // let/const/var
        } destructure_array;
        struct {
            NodeList properties;    // Array of destructure elements  
            ASTNode *source;        // Expression to destructure from
            DeclType decl_type;     // let/const/var
        } destructure_object;
        struct {
            char *name;             // Variable name to bind to
            char *key;              // Object key (for object destructuring)
            ASTNode *default_value; // Default value if undefined
            int is_rest;            // Is this a rest element (...rest)
            int is_hole;            // Is this a hole/skip in array destructuring
            ASTNode *nested;        // Nested destructuring pattern
        } destructure_element;
        struct {
            ASTNode *try_block;
            char *catch_param;
            ASTNode *catch_block;
            ASTNode *finally_block;
        } try_catch;
        struct {
            ASTNode *expression;
        } throw_stmt;
        struct { char op[4]; ASTNode *operand; int is_postfix; } postfix;
        struct { ASTNode *obj; char *member; } optional_chain;
        struct { ASTNode *left; ASTNode *right; } nullish_coalesce;
        /* Pointer operations (C/C++ compatibility) */
        struct { ASTNode *operand; } address_of;      // &variable
        struct { ASTNode *operand; } dereference;     // *pointer
        struct { ASTNode *ptr; char *member; } pointer_member;  // ptr->member
        /* Struct/Union support (C/C++ compatibility) */
        struct {
            char *name;                  // struct/union name
            StructField *fields;         // Array of fields
            int field_count;             // Number of fields
            int is_union;                // true for union, false for struct
        } struct_def;
        struct {
            char *struct_name;           // Name of struct/union type
            ObjectProperty *fields;      // Field initializers (reuse ObjectProperty)
            int field_count;             // Number of field initializers
        } struct_literal;
        /* Security: Unsafe block support */
        struct {
            NodeList stmts;              // Statements inside unsafe block
        } unsafe_block;
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
ASTNode *ast_new_for_c_style(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line);
ASTNode *ast_new_return(ASTNode *value, int line);
ASTNode *ast_new_break(int line);
ASTNode *ast_new_continue(int line);
ASTNode *ast_new_block(int line);
ASTNode *ast_new_func_def(const char *name, const char *return_type, int line);
ASTNode *ast_new_list(int line);
ASTNode *ast_new_tuple(int line);
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
ASTNode *ast_new_try_catch(ASTNode *try_block, const char *catch_param, ASTNode *catch_block, ASTNode *finally_block, int line);
ASTNode *ast_new_throw(ASTNode *expression, int line);
/* Module system constructors */
ASTNode *ast_new_import_named(char **names, char **aliases, int count, const char *module_path, int line);
ASTNode *ast_new_import_default(const char *name, const char *module_path, int line);
ASTNode *ast_new_import_namespace(const char *namespace, const char *module_path, int line);
ASTNode *ast_new_export(int is_default, ASTNode *declaration, char **names, int count, int line);
ASTNode *ast_new_destructure_array(ASTNode *source, DeclType decl_type, int line);
ASTNode *ast_new_destructure_object(ASTNode *source, DeclType decl_type, int line);
ASTNode *ast_new_destructure_element(const char *name, const char *key, ASTNode *default_value, int is_rest, int is_hole, int line);
/* Pointer operation constructors (C/C++ compatibility) */
ASTNode *ast_new_address_of(ASTNode *operand, int line);
ASTNode *ast_new_dereference(ASTNode *operand, int line);
ASTNode *ast_new_pointer_member(ASTNode *ptr, const char *member, int line);
/* Struct/Union constructors (C/C++ compatibility) */
ASTNode *ast_new_struct_def(const char *name, int is_union, int line);
void ast_struct_add_field(ASTNode *struct_node, const char *field_name, const char *field_type, ASTNode *default_value);
ASTNode *ast_new_struct_literal(const char *struct_name, int line);
void ast_struct_literal_add_field(ASTNode *literal, const char *field_name, ASTNode *value);
/* Security: Unsafe block constructor */
ASTNode *ast_new_unsafe_block(int line);
void ast_free(ASTNode *node);

#endif
