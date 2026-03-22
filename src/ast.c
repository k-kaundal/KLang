#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void nodelist_init(NodeList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void nodelist_push(NodeList *list, ASTNode *node) {
    if (list->count >= list->capacity) {
        list->capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        list->items = realloc(list->items, list->capacity * sizeof(ASTNode *));
    }
    list->items[list->count++] = node;
}

void nodelist_free(NodeList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static ASTNode *ast_alloc(NodeType type, int line) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    n->type = type;
    n->line = line;
    return n;
}

ASTNode *ast_new_number(double val, int line) {
    ASTNode *n = ast_alloc(NODE_NUMBER, line);
    n->data.num_val = val;
    return n;
}

ASTNode *ast_new_string(const char *val, int line) {
    ASTNode *n = ast_alloc(NODE_STRING, line);
    n->data.str_val = strdup(val);
    return n;
}

ASTNode *ast_new_bool(int val, int line) {
    ASTNode *n = ast_alloc(NODE_BOOL, line);
    n->data.bool_val = val;
    return n;
}

ASTNode *ast_new_ident(const char *name, int line) {
    ASTNode *n = ast_alloc(NODE_IDENT, line);
    n->data.ident.name = strdup(name);
    return n;
}

ASTNode *ast_new_null(int line) {
    return ast_alloc(NODE_NULL, line);
}

ASTNode *ast_new_binop(const char *op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *n = ast_alloc(NODE_BINOP, line);
    strncpy(n->data.binop.op, op, 3);
    n->data.binop.op[3] = '\0';
    n->data.binop.left = left;
    n->data.binop.right = right;
    return n;
}

ASTNode *ast_new_unop(const char *op, ASTNode *operand, int line) {
    ASTNode *n = ast_alloc(NODE_UNOP, line);
    strncpy(n->data.unop.op, op, 3);
    n->data.unop.op[3] = '\0';
    n->data.unop.operand = operand;
    return n;
}

ASTNode *ast_new_call(ASTNode *callee, int line) {
    ASTNode *n = ast_alloc(NODE_CALL, line);
    n->data.call.callee = callee;
    nodelist_init(&n->data.call.args);
    return n;
}

ASTNode *ast_new_index(ASTNode *obj, ASTNode *index, int line) {
    ASTNode *n = ast_alloc(NODE_INDEX, line);
    n->data.index_expr.obj = obj;
    n->data.index_expr.index = index;
    return n;
}

ASTNode *ast_new_let(const char *name, const char *type_annot, ASTNode *value, int line) {
    ASTNode *n = ast_alloc(NODE_LET, line);
    n->data.let_stmt.name = strdup(name);
    n->data.let_stmt.type_annot = type_annot ? strdup(type_annot) : NULL;
    n->data.let_stmt.value = value;
    n->data.let_stmt.is_static = 0;
    n->data.let_stmt.access = ACCESS_PUBLIC;
    n->data.let_stmt.decl_type = DECL_LET;
    return n;
}

ASTNode *ast_new_var_decl(const char *name, const char *type_annot, ASTNode *value, DeclType decl_type, int line) {
    ASTNode *n = ast_alloc(NODE_LET, line);
    n->data.let_stmt.name = strdup(name);
    n->data.let_stmt.type_annot = type_annot ? strdup(type_annot) : NULL;
    n->data.let_stmt.value = value;
    n->data.let_stmt.is_static = 0;
    n->data.let_stmt.access = ACCESS_PUBLIC;
    n->data.let_stmt.decl_type = decl_type;
    return n;
}

ASTNode *ast_new_assign(const char *name, ASTNode *value, int line) {
    ASTNode *n = ast_alloc(NODE_ASSIGN, line);
    n->data.assign_stmt.name = strdup(name);
    n->data.assign_stmt.value = value;
    return n;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_block, ASTNode *else_block, int line) {
    ASTNode *n = ast_alloc(NODE_IF, line);
    n->data.if_stmt.cond = cond;
    n->data.if_stmt.then_block = then_block;
    n->data.if_stmt.else_block = else_block;
    return n;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = ast_alloc(NODE_WHILE, line);
    n->data.while_stmt.cond = cond;
    n->data.while_stmt.body = body;
    return n;
}

ASTNode *ast_new_for(const char *var, ASTNode *start, ASTNode *end, ASTNode *body, int line) {
    ASTNode *n = ast_alloc(NODE_FOR, line);
    n->data.for_stmt.var = strdup(var);
    n->data.for_stmt.start = start;
    n->data.for_stmt.end = end;
    n->data.for_stmt.body = body;
    return n;
}

ASTNode *ast_new_for_of(const char *var, ASTNode *iterable, ASTNode *body, DeclType decl_type, int line) {
    ASTNode *n = ast_alloc(NODE_FOR_OF, line);
    n->data.for_of_stmt.var = strdup(var);
    n->data.for_of_stmt.iterable = iterable;
    n->data.for_of_stmt.body = body;
    n->data.for_of_stmt.decl_type = decl_type;
    return n;
}

ASTNode *ast_new_return(ASTNode *value, int line) {
    ASTNode *n = ast_alloc(NODE_RETURN, line);
    n->data.return_stmt.value = value;
    return n;
}

ASTNode *ast_new_break(int line) {
    return ast_alloc(NODE_BREAK, line);
}

ASTNode *ast_new_continue(int line) {
    return ast_alloc(NODE_CONTINUE, line);
}

ASTNode *ast_new_block(int line) {
    ASTNode *n = ast_alloc(NODE_BLOCK, line);
    nodelist_init(&n->data.block.stmts);
    return n;
}

ASTNode *ast_new_func_def(const char *name, const char *return_type, int line) {
    ASTNode *n = ast_alloc(NODE_FUNC_DEF, line);
    n->data.func_def.name = strdup(name);
    n->data.func_def.return_type = return_type ? strdup(return_type) : NULL;
    nodelist_init(&n->data.func_def.params);
    n->data.func_def.param_types = NULL;
    n->data.func_def.body = NULL;
    n->data.func_def.is_static = 0;
    n->data.func_def.access = ACCESS_PUBLIC;
    n->data.func_def.is_abstract = 0;
    n->data.func_def.is_arrow = 0;
    return n;
}

ASTNode *ast_new_list(int line) {
    ASTNode *n = ast_alloc(NODE_LIST, line);
    nodelist_init(&n->data.list.elements);
    return n;
}

ASTNode *ast_new_object(int line) {
    ASTNode *n = ast_alloc(NODE_OBJECT, line);
    n->data.object.props = NULL;
    n->data.object.count = 0;
    n->data.object.capacity = 0;
    return n;
}

void ast_object_add_property(ASTNode *obj, const char *key, ASTNode *key_expr, ASTNode *value, int is_shorthand, int is_method) {
    if (obj->data.object.count >= obj->data.object.capacity) {
        int new_cap = obj->data.object.capacity == 0 ? 8 : obj->data.object.capacity * 2;
        obj->data.object.props = realloc(obj->data.object.props, new_cap * sizeof(ObjectProperty));
        obj->data.object.capacity = new_cap;
    }
    ObjectProperty *prop = &obj->data.object.props[obj->data.object.count++];
    prop->key = key ? strdup(key) : NULL;
    prop->key_expr = key_expr;
    prop->value = value;
    prop->is_shorthand = is_shorthand;
    prop->is_method = is_method;
}

ASTNode *ast_new_class_def(const char *name, const char *parent_name, int line) {
    ASTNode *n = ast_alloc(NODE_CLASS_DEF, line);
    n->data.class_def.name = strdup(name);
    n->data.class_def.parent_name = parent_name ? strdup(parent_name) : NULL;
    nodelist_init(&n->data.class_def.members);
    n->data.class_def.is_abstract = 0;
    return n;
}

ASTNode *ast_new_new(const char *class_name, int line) {
    ASTNode *n = ast_alloc(NODE_NEW, line);
    n->data.new_expr.class_name = strdup(class_name);
    nodelist_init(&n->data.new_expr.args);
    return n;
}

ASTNode *ast_new_member_access(ASTNode *obj, const char *member, int line) {
    ASTNode *n = ast_alloc(NODE_MEMBER_ACCESS, line);
    n->data.member_access.obj = obj;
    n->data.member_access.member = strdup(member);
    return n;
}

ASTNode *ast_new_this(int line) {
    return ast_alloc(NODE_THIS, line);
}

ASTNode *ast_new_super(const char *member, int line) {
    ASTNode *n = ast_alloc(NODE_SUPER, line);
    n->data.super_expr.member = member ? strdup(member) : NULL;
    return n;
}

ASTNode *ast_new_template_literal(int line) {
    ASTNode *n = ast_alloc(NODE_TEMPLATE_LITERAL, line);
    n->data.template_literal.parts = NULL;
    n->data.template_literal.exprs = NULL;
    n->data.template_literal.count = 0;
    return n;
}

ASTNode *ast_new_ternary(ASTNode *cond, ASTNode *true_expr, ASTNode *false_expr, int line) {
    ASTNode *n = ast_alloc(NODE_TERNARY, line);
    n->data.ternary.cond = cond;
    n->data.ternary.true_expr = true_expr;
    n->data.ternary.false_expr = false_expr;
    return n;
}

ASTNode *ast_new_switch(ASTNode *expr, int line) {
    ASTNode *n = ast_alloc(NODE_SWITCH, line);
    n->data.switch_stmt.expr = expr;
    nodelist_init(&n->data.switch_stmt.cases);
    n->data.switch_stmt.default_case = NULL;
    return n;
}

ASTNode *ast_new_case(ASTNode *value, int line) {
    ASTNode *n = ast_alloc(NODE_CASE, line);
    n->data.case_stmt.value = value;
    nodelist_init(&n->data.case_stmt.body);
    return n;
}

ASTNode *ast_new_await(ASTNode *expr, int line) {
    ASTNode *n = ast_alloc(NODE_AWAIT, line);
    n->data.await_expr.expr = expr;
    return n;
}

ASTNode *ast_new_yield(ASTNode *value, int line) {
    ASTNode *n = ast_alloc(NODE_YIELD, line);
    n->data.yield_expr.value = value;
    return n;
}

/* Module system constructors */
ASTNode *ast_new_import_named(char **names, char **aliases, int count, const char *module_path, int line) {
    ASTNode *n = ast_alloc(NODE_IMPORT_NAMED, line);
    n->data.import_named.names = names;
    n->data.import_named.aliases = aliases;
    n->data.import_named.count = count;
    n->data.import_named.module_path = module_path ? strdup(module_path) : NULL;
    return n;
}

ASTNode *ast_new_import_default(const char *name, const char *module_path, int line) {
    ASTNode *n = ast_alloc(NODE_IMPORT_DEFAULT, line);
    n->data.import_default.name = name ? strdup(name) : NULL;
    n->data.import_default.module_path = module_path ? strdup(module_path) : NULL;
    return n;
}

ASTNode *ast_new_import_namespace(const char *namespace, const char *module_path, int line) {
    ASTNode *n = ast_alloc(NODE_IMPORT_NAMESPACE, line);
    n->data.import_namespace.namespace = namespace ? strdup(namespace) : NULL;
    n->data.import_namespace.module_path = module_path ? strdup(module_path) : NULL;
    return n;
}

ASTNode *ast_new_export(int is_default, ASTNode *declaration, char **names, int count, int line) {
    ASTNode *n = ast_alloc(NODE_EXPORT, line);
    n->data.export_stmt.is_default = is_default;
    n->data.export_stmt.declaration = declaration;
    n->data.export_stmt.names = names;
    n->data.export_stmt.count = count;
    return n;
}

void ast_free(ASTNode *node) {
    int i;
    if (!node) return;
    switch (node->type) {
        case NODE_STRING:
            free(node->data.str_val);
            break;
        case NODE_IDENT:
            free(node->data.ident.name);
            break;
        case NODE_BINOP:
            ast_free(node->data.binop.left);
            ast_free(node->data.binop.right);
            break;
        case NODE_UNOP:
            ast_free(node->data.unop.operand);
            break;
        case NODE_CALL:
            ast_free(node->data.call.callee);
            for (i = 0; i < node->data.call.args.count; i++)
                ast_free(node->data.call.args.items[i]);
            nodelist_free(&node->data.call.args);
            break;
        case NODE_INDEX:
            ast_free(node->data.index_expr.obj);
            ast_free(node->data.index_expr.index);
            break;
        case NODE_LET:
            free(node->data.let_stmt.name);
            if (node->data.let_stmt.type_annot) free(node->data.let_stmt.type_annot);
            ast_free(node->data.let_stmt.value);
            break;
        case NODE_ASSIGN:
            free(node->data.assign_stmt.name);
            ast_free(node->data.assign_stmt.value);
            break;
        case NODE_IF:
            ast_free(node->data.if_stmt.cond);
            ast_free(node->data.if_stmt.then_block);
            ast_free(node->data.if_stmt.else_block);
            break;
        case NODE_WHILE:
            ast_free(node->data.while_stmt.cond);
            ast_free(node->data.while_stmt.body);
            break;
        case NODE_FOR:
            free(node->data.for_stmt.var);
            ast_free(node->data.for_stmt.start);
            ast_free(node->data.for_stmt.end);
            ast_free(node->data.for_stmt.body);
            break;
        case NODE_FOR_OF:
            free(node->data.for_of_stmt.var);
            ast_free(node->data.for_of_stmt.iterable);
            ast_free(node->data.for_of_stmt.body);
            break;
        case NODE_RETURN:
            ast_free(node->data.return_stmt.value);
            break;
        case NODE_BREAK:
        case NODE_CONTINUE:
            break;
        case NODE_BLOCK:
            for (i = 0; i < node->data.block.stmts.count; i++)
                ast_free(node->data.block.stmts.items[i]);
            nodelist_free(&node->data.block.stmts);
            break;
        case NODE_FUNC_DEF:
            free(node->data.func_def.name);
            if (node->data.func_def.return_type) free(node->data.func_def.return_type);
            for (i = 0; i < node->data.func_def.params.count; i++) {
                ast_free(node->data.func_def.params.items[i]);
                if (node->data.func_def.param_types && node->data.func_def.param_types[i])
                    free(node->data.func_def.param_types[i]);
            }
            if (node->data.func_def.param_types) free(node->data.func_def.param_types);
            nodelist_free(&node->data.func_def.params);
            ast_free(node->data.func_def.body);
            break;
        case NODE_LIST:
            for (i = 0; i < node->data.list.elements.count; i++)
                ast_free(node->data.list.elements.items[i]);
            nodelist_free(&node->data.list.elements);
            break;
        case NODE_OBJECT:
            for (i = 0; i < node->data.object.count; i++) {
                ObjectProperty *prop = &node->data.object.props[i];
                if (prop->key) free(prop->key);
                if (prop->key_expr) ast_free(prop->key_expr);
                if (prop->value) ast_free(prop->value);
            }
            free(node->data.object.props);
            break;
        case NODE_CLASS_DEF:
            free(node->data.class_def.name);
            if (node->data.class_def.parent_name) free(node->data.class_def.parent_name);
            for (i = 0; i < node->data.class_def.members.count; i++)
                ast_free(node->data.class_def.members.items[i]);
            nodelist_free(&node->data.class_def.members);
            break;
        case NODE_NEW:
            free(node->data.new_expr.class_name);
            for (i = 0; i < node->data.new_expr.args.count; i++)
                ast_free(node->data.new_expr.args.items[i]);
            nodelist_free(&node->data.new_expr.args);
            break;
        case NODE_MEMBER_ACCESS:
            ast_free(node->data.member_access.obj);
            free(node->data.member_access.member);
            break;
        case NODE_THIS:
            break;
        case NODE_SUPER:
            if (node->data.super_expr.member) free(node->data.super_expr.member);
            break;
        case NODE_TEMPLATE_LITERAL:
            for (i = 0; i < node->data.template_literal.count; i++) {
                if (node->data.template_literal.parts[i])
                    free(node->data.template_literal.parts[i]);
            }
            free(node->data.template_literal.parts);
            for (i = 0; i < node->data.template_literal.count - 1; i++) {
                ast_free(node->data.template_literal.exprs[i]);
            }
            free(node->data.template_literal.exprs);
            break;
        case NODE_TERNARY:
            ast_free(node->data.ternary.cond);
            ast_free(node->data.ternary.true_expr);
            ast_free(node->data.ternary.false_expr);
            break;
        case NODE_SWITCH:
            ast_free(node->data.switch_stmt.expr);
            for (i = 0; i < node->data.switch_stmt.cases.count; i++)
                ast_free(node->data.switch_stmt.cases.items[i]);
            nodelist_free(&node->data.switch_stmt.cases);
            ast_free(node->data.switch_stmt.default_case);
            break;
        case NODE_CASE:
            ast_free(node->data.case_stmt.value);
            for (i = 0; i < node->data.case_stmt.body.count; i++)
                ast_free(node->data.case_stmt.body.items[i]);
            nodelist_free(&node->data.case_stmt.body);
            break;
        case NODE_AWAIT:
            ast_free(node->data.await_expr.expr);
            break;
        case NODE_YIELD:
            if (node->data.yield_expr.value) {
                ast_free(node->data.yield_expr.value);
            }
            break;
        case NODE_IMPORT_NAMED:
            if (node->data.import_named.names) {
                for (i = 0; i < node->data.import_named.count; i++)
                    free(node->data.import_named.names[i]);
                free(node->data.import_named.names);
            }
            if (node->data.import_named.aliases) {
                for (i = 0; i < node->data.import_named.count; i++)
                    if (node->data.import_named.aliases[i])
                        free(node->data.import_named.aliases[i]);
                free(node->data.import_named.aliases);
            }
            if (node->data.import_named.module_path)
                free(node->data.import_named.module_path);
            break;
        case NODE_IMPORT_DEFAULT:
            if (node->data.import_default.name)
                free(node->data.import_default.name);
            if (node->data.import_default.module_path)
                free(node->data.import_default.module_path);
            break;
        case NODE_IMPORT_NAMESPACE:
            if (node->data.import_namespace.namespace)
                free(node->data.import_namespace.namespace);
            if (node->data.import_namespace.module_path)
                free(node->data.import_namespace.module_path);
            break;
        case NODE_EXPORT:
            if (node->data.export_stmt.declaration)
                ast_free(node->data.export_stmt.declaration);
            if (node->data.export_stmt.names) {
                for (i = 0; i < node->data.export_stmt.count; i++)
                    if (node->data.export_stmt.names[i])
                        free(node->data.export_stmt.names[i]);
                free(node->data.export_stmt.names);
            }
            break;
        default:
            break;
    }
    free(node);
}
