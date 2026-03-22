#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER, NODE_STRING, NODE_BOOL, NODE_IDENT, NODE_NULL,
    NODE_BINOP, NODE_UNOP, NODE_CALL, NODE_INDEX,
    NODE_LET, NODE_ASSIGN, NODE_IF, NODE_WHILE, NODE_FOR,
    NODE_RETURN, NODE_BREAK, NODE_CONTINUE, NODE_BLOCK, NODE_FUNC_DEF, NODE_STRUCT_DEF, NODE_IMPORT,
    NODE_LIST,
    NODE_CLASS_DEF, NODE_NEW, NODE_MEMBER_ACCESS, NODE_THIS, NODE_SUPER,
    NODE_TEMPLATE_LITERAL
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
        struct { ASTNode *value; } return_stmt;
        struct { } break_stmt;
        struct { } continue_stmt;
        struct { NodeList stmts; } block;
        struct { char *name; NodeList params; char **param_types; char *return_type; ASTNode *body; int is_static; AccessModifier access; int is_abstract; int is_arrow; } func_def;
        struct { NodeList elements; } list;
        struct { char *name; char *parent_name; NodeList members; int is_abstract; } class_def;
        struct { char *class_name; NodeList args; } new_expr;
        struct { ASTNode *obj; char *member; } member_access;
        struct { } this_expr;
        struct { char *member; } super_expr;
        struct { char **parts; ASTNode **exprs; int count; } template_literal;
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
ASTNode *ast_new_return(ASTNode *value, int line);
ASTNode *ast_new_break(int line);
ASTNode *ast_new_continue(int line);
ASTNode *ast_new_block(int line);
ASTNode *ast_new_func_def(const char *name, const char *return_type, int line);
ASTNode *ast_new_list(int line);
ASTNode *ast_new_class_def(const char *name, const char *parent_name, int line);
ASTNode *ast_new_new(const char *class_name, int line);
ASTNode *ast_new_member_access(ASTNode *obj, const char *member, int line);
ASTNode *ast_new_this(int line);
ASTNode *ast_new_super(const char *member, int line);
ASTNode *ast_new_template_literal(int line);
void ast_free(ASTNode *node);

#endif
