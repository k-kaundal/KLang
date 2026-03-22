#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_statement(Parser *parser);
static ASTNode *parse_block(Parser *parser);
static ASTNode *parse_break(Parser *parser);
static ASTNode *parse_continue(Parser *parser);
static ASTNode *parse_class_def(Parser *parser);

void parser_init(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser->had_error = 0;
    parser->current = lexer_next_token(lexer);
    parser->peek = lexer_next_token(lexer);
}

void parser_free(Parser *parser) {
    token_free(&parser->current);
    token_free(&parser->peek);
}

static Token advance(Parser *parser) {
    Token old = parser->current;
    parser->current = parser->peek;
    parser->peek = lexer_next_token(parser->lexer);
    return old;
}

static int check(Parser *parser, TokenType type) {
    return parser->current.type == type;
}

static int match(Parser *parser, TokenType type) {
    if (check(parser, type)) {
        Token t = advance(parser);
        token_free(&t);
        return 1;
    }
    return 0;
}

static Token consume(Parser *parser, TokenType type) {
    if (check(parser, type)) return advance(parser);
    fprintf(stderr, "Parse error at line %d: expected %s, got %s ('%s')\n",
            parser->current.line, token_type_name(type),
            token_type_name(parser->current.type), parser->current.value);
    parser->had_error = 1;
    return parser->current;
}

static ASTNode *parse_primary(Parser *parser) {
    int line = parser->current.line;

    if (check(parser, TOKEN_NUMBER)) {
        Token t = advance(parser);
        double val = atof(t.value);
        ASTNode *n = ast_new_number(val, line);
        token_free(&t);
        return n;
    }
    if (check(parser, TOKEN_STRING)) {
        Token t = advance(parser);
        ASTNode *n = ast_new_string(t.value, line);
        token_free(&t);
        return n;
    }
    if (check(parser, TOKEN_TRUE)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_bool(1, line);
    }
    if (check(parser, TOKEN_FALSE)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_bool(0, line);
    }
    if (check(parser, TOKEN_IDENT)) {
        Token t = advance(parser);
        ASTNode *n = ast_new_ident(t.value, line);
        token_free(&t);
        return n;
    }
    if (check(parser, TOKEN_LPAREN)) {
        Token t = advance(parser);
        ASTNode *expr;
        Token t2;
        token_free(&t);
        expr = parse_expression(parser);
        t2 = consume(parser, TOKEN_RPAREN);
        token_free(&t2);
        return expr;
    }
    if (check(parser, TOKEN_LBRACKET)) {
        Token t = advance(parser);
        ASTNode *list;
        Token t2;
        token_free(&t);
        list = ast_new_list(line);
        while (!check(parser, TOKEN_RBRACKET) && !check(parser, TOKEN_EOF)) {
            nodelist_push(&list->data.list.elements, parse_expression(parser));
            if (!match(parser, TOKEN_COMMA)) break;
        }
        t2 = consume(parser, TOKEN_RBRACKET);
        token_free(&t2);
        return list;
    }
    if (check(parser, TOKEN_NEW)) {
        Token t = advance(parser);
        Token class_tok;
        char *class_name;
        ASTNode *new_expr;
        token_free(&t);
        class_tok = consume(parser, TOKEN_IDENT);
        class_name = strdup(class_tok.value);
        token_free(&class_tok);
        new_expr = ast_new_new(class_name, line);
        free(class_name);
        return new_expr;
    }
    if (check(parser, TOKEN_THIS)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_this(line);
    }
    if (check(parser, TOKEN_SUPER)) {
        Token t = advance(parser);
        token_free(&t);
        /* super.member will be handled in parse_postfix */
        return ast_new_super("", line);
    }

    fprintf(stderr, "Parse error at line %d: unexpected token %s ('%s')\n",
            line, token_type_name(parser->current.type), parser->current.value);
    parser->had_error = 1;
    {
        Token t = advance(parser);
        token_free(&t);
    }
    return ast_new_null(line);
}

static ASTNode *parse_postfix(Parser *parser) {
    ASTNode *expr = parse_primary(parser);

    while (1) {
        int line = parser->current.line;
        if (check(parser, TOKEN_LPAREN)) {
            Token t = advance(parser);
            ASTNode *call;
            token_free(&t);
            call = ast_new_call(expr, line);
            while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
                nodelist_push(&call->data.call.args, parse_expression(parser));
                if (!match(parser, TOKEN_COMMA)) break;
            }
            {
                Token t2 = consume(parser, TOKEN_RPAREN);
                token_free(&t2);
            }
            expr = call;
        } else if (check(parser, TOKEN_LBRACKET)) {
            Token t = advance(parser);
            ASTNode *idx;
            token_free(&t);
            idx = parse_expression(parser);
            {
                Token t2 = consume(parser, TOKEN_RBRACKET);
                token_free(&t2);
            }
            expr = ast_new_index(expr, idx, line);
        } else if (check(parser, TOKEN_DOT)) {
            Token t = advance(parser);
            Token member_tok;
            char *member_name;
            token_free(&t);
            member_tok = consume(parser, TOKEN_IDENT);
            member_name = strdup(member_tok.value);
            token_free(&member_tok);
            /* Handle super.member specially */
            if (expr->type == NODE_SUPER) {
                ast_free(expr);
                expr = ast_new_super(member_name, line);
            } else {
                expr = ast_new_member_access(expr, member_name, line);
            }
            free(member_name);
        } else {
            break;
        }
    }
    return expr;
}

static ASTNode *parse_unary(Parser *parser) {
    int line = parser->current.line;
    if (check(parser, TOKEN_MINUS)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_unop("-", parse_unary(parser), line);
    }
    if (check(parser, TOKEN_BANG)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_unop("!", parse_unary(parser), line);
    }
    return parse_postfix(parser);
}

static ASTNode *parse_multiplicative(Parser *parser) {
    ASTNode *left = parse_unary(parser);
    while (check(parser, TOKEN_STAR) || check(parser, TOKEN_SLASH) || check(parser, TOKEN_PERCENT)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        {
            ASTNode *right = parse_unary(parser);
            left = ast_new_binop(op, left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_additive(Parser *parser) {
    ASTNode *left = parse_multiplicative(parser);
    while (check(parser, TOKEN_PLUS) || check(parser, TOKEN_MINUS)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        {
            ASTNode *right = parse_multiplicative(parser);
            left = ast_new_binop(op, left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_comparison(Parser *parser) {
    ASTNode *left = parse_additive(parser);
    while (check(parser, TOKEN_LT) || check(parser, TOKEN_GT) ||
           check(parser, TOKEN_LEQ) || check(parser, TOKEN_GEQ)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        {
            ASTNode *right = parse_additive(parser);
            left = ast_new_binop(op, left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_equality(Parser *parser) {
    ASTNode *left = parse_comparison(parser);
    while (check(parser, TOKEN_EQ) || check(parser, TOKEN_NEQ)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        {
            ASTNode *right = parse_comparison(parser);
            left = ast_new_binop(op, left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_expression(Parser *parser) {
    return parse_equality(parser);
}

static ASTNode *parse_block(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_LBRACE);
    ASTNode *block;
    token_free(&t);
    block = ast_new_block(line);
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        ASTNode *stmt;
        while (match(parser, TOKEN_SEMICOLON)) {}
        if (check(parser, TOKEN_RBRACE) || check(parser, TOKEN_EOF)) break;
        stmt = parse_statement(parser);
        if (stmt) nodelist_push(&block->data.block.stmts, stmt);
        while (match(parser, TOKEN_SEMICOLON)) {}
    }
    {
        Token t2 = consume(parser, TOKEN_RBRACE);
        token_free(&t2);
    }
    return block;
}

static ASTNode *parse_let(Parser *parser) {
    int line = parser->current.line;
    Token let_tok = consume(parser, TOKEN_LET);
    Token name_tok;
    char *varname;
    char *type_annot = NULL;
    ASTNode *value;
    ASTNode *n;
    token_free(&let_tok);
    name_tok = consume(parser, TOKEN_IDENT);
    varname = strdup(name_tok.value);
    token_free(&name_tok);

    if (match(parser, TOKEN_COLON)) {
        Token type_tok = consume(parser, TOKEN_IDENT);
        type_annot = strdup(type_tok.value);
        token_free(&type_tok);
    }

    {
        Token eq_tok = consume(parser, TOKEN_ASSIGN);
        token_free(&eq_tok);
    }
    value = parse_expression(parser);
    n = ast_new_let(varname, type_annot, value, line);
    free(varname);
    if (type_annot) free(type_annot);
    return n;
}

static ASTNode *parse_func_def(Parser *parser) {
    int line = parser->current.line;
    Token fn_tok = consume(parser, TOKEN_FN);
    Token name_tok;
    char *fname;
    char **param_names = NULL;
    char **param_types = NULL;
    int param_count = 0;
    int param_cap = 0;
    char *return_type = NULL;
    ASTNode *func;
    token_free(&fn_tok);
    name_tok = consume(parser, TOKEN_IDENT);
    fname = strdup(name_tok.value);
    token_free(&name_tok);

    {
        Token lp_tok = consume(parser, TOKEN_LPAREN);
        token_free(&lp_tok);
    }

    while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
        Token pname_tok = consume(parser, TOKEN_IDENT);
        if (param_count >= param_cap) {
            param_cap = param_cap == 0 ? 4 : param_cap * 2;
            param_names = realloc(param_names, param_cap * sizeof(char *));
            param_types = realloc(param_types, param_cap * sizeof(char *));
        }
        param_names[param_count] = strdup(pname_tok.value);
        token_free(&pname_tok);
        param_types[param_count] = NULL;
        if (match(parser, TOKEN_COLON)) {
            Token ptype_tok = consume(parser, TOKEN_IDENT);
            param_types[param_count] = strdup(ptype_tok.value);
            token_free(&ptype_tok);
        }
        param_count++;
        if (!match(parser, TOKEN_COMMA)) break;
    }

    {
        Token rp_tok = consume(parser, TOKEN_RPAREN);
        token_free(&rp_tok);
    }

    if (match(parser, TOKEN_ARROW)) {
        Token rtype_tok = consume(parser, TOKEN_IDENT);
        return_type = strdup(rtype_tok.value);
        token_free(&rtype_tok);
    }

    func = ast_new_func_def(fname, return_type, line);
    free(fname);
    if (return_type) free(return_type);

    func->data.func_def.param_types = param_types;
    {
        int i;
        for (i = 0; i < param_count; i++) {
            nodelist_push(&func->data.func_def.params, ast_new_ident(param_names[i], line));
            free(param_names[i]);
        }
    }
    free(param_names);

    func->data.func_def.body = parse_block(parser);
    return func;
}

static ASTNode *parse_if(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_IF);
    ASTNode *cond;
    ASTNode *then_block;
    ASTNode *else_block = NULL;
    token_free(&t);
    cond = parse_expression(parser);
    then_block = parse_block(parser);
    if (match(parser, TOKEN_ELSE)) {
        if (check(parser, TOKEN_IF)) {
            else_block = parse_if(parser);
        } else {
            else_block = parse_block(parser);
        }
    }
    return ast_new_if(cond, then_block, else_block, line);
}

static ASTNode *parse_while(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_WHILE);
    ASTNode *cond;
    ASTNode *body;
    token_free(&t);
    cond = parse_expression(parser);
    body = parse_block(parser);
    return ast_new_while(cond, body, line);
}

static ASTNode *parse_for(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_FOR);
    Token var_tok;
    char *varname;
    ASTNode *start;
    ASTNode *end;
    ASTNode *body;
    ASTNode *n;
    token_free(&t);
    var_tok = consume(parser, TOKEN_IDENT);
    varname = strdup(var_tok.value);
    token_free(&var_tok);
    {
        Token in_tok = consume(parser, TOKEN_IN);
        token_free(&in_tok);
    }
    start = parse_expression(parser);
    {
        Token dd_tok = consume(parser, TOKEN_DOTDOT);
        token_free(&dd_tok);
    }
    end = parse_expression(parser);
    body = parse_block(parser);
    n = ast_new_for(varname, start, end, body, line);
    free(varname);
    return n;
}

static ASTNode *parse_return(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_RETURN);
    ASTNode *value = NULL;
    token_free(&t);
    if (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF) && !check(parser, TOKEN_SEMICOLON)) {
        value = parse_expression(parser);
    }
    return ast_new_return(value, line);
}

static ASTNode *parse_statement(Parser *parser) {
    while (match(parser, TOKEN_SEMICOLON)) {}

    if (check(parser, TOKEN_CLASS)) return parse_class_def(parser);
    if (check(parser, TOKEN_FN)) return parse_func_def(parser);
    if (check(parser, TOKEN_LET)) return parse_let(parser);
    if (check(parser, TOKEN_IF)) return parse_if(parser);
    if (check(parser, TOKEN_WHILE)) return parse_while(parser);
    if (check(parser, TOKEN_FOR)) return parse_for(parser);
    if (check(parser, TOKEN_RETURN)) return parse_return(parser);
    if (check(parser, TOKEN_BREAK)) return parse_break(parser);
    if (check(parser, TOKEN_CONTINUE)) return parse_continue(parser);
    if (check(parser, TOKEN_LBRACE)) return parse_block(parser);
    if (check(parser, TOKEN_EOF)) return NULL;

    {
        int line = parser->current.line;
        ASTNode *expr = parse_expression(parser);

        if (expr && check(parser, TOKEN_ASSIGN)) {
            if (expr->type == NODE_IDENT) {
                char *name = strdup(expr->data.ident.name);
                ASTNode *value;
                ASTNode *n;
                ast_free(expr);
                {
                    Token t = advance(parser);
                    token_free(&t);
                }
                value = parse_expression(parser);
                n = ast_new_assign(name, value, line);
                free(name);
                return n;
            } else if (expr->type == NODE_MEMBER_ACCESS || expr->type == NODE_INDEX) {
                /* For member access/index assignment (e.g., this.x = value or arr[0] = value)
                 * The AST structure doesn't directly support this, so we'll handle it
                 * as a special case. The interpreter/compiler will need to recognize
                 * assignments to member access/index expressions. 
                 * 
                 * For now, we create a binary operation with '=' operator which
                 * the runtime can intercept. */
                ASTNode *value;
                {
                    Token t = advance(parser);
                    token_free(&t);
                }
                value = parse_expression(parser);
                return ast_new_binop("=", expr, value, line);
            } else {
                /* Assignment to non-lvalue */
                fprintf(stderr, "Parse error at line %d: invalid assignment target\n", line);
                parser->had_error = 1;
                ast_free(expr);
                {
                    Token t = advance(parser);
                    token_free(&t);
                }
                {
                    ASTNode *value = parse_expression(parser);
                    ast_free(value);
                }
                return ast_new_null(line);
            }
        }

        return expr;
    }
}

static ASTNode *parse_break(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_BREAK);
    token_free(&t);
    return ast_new_break(line);
}

static ASTNode *parse_continue(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_CONTINUE);
    token_free(&t);
    return ast_new_continue(line);
}

static ASTNode *parse_class_def(Parser *parser) {
    int line = parser->current.line;
    Token class_tok = consume(parser, TOKEN_CLASS);
    Token name_tok;
    char *class_name;
    char *parent_name = NULL;
    ASTNode *class_node;
    token_free(&class_tok);
    
    name_tok = consume(parser, TOKEN_IDENT);
    class_name = strdup(name_tok.value);
    token_free(&name_tok);
    
    /* Check for extends clause */
    if (match(parser, TOKEN_EXTENDS)) {
        Token parent_tok = consume(parser, TOKEN_IDENT);
        parent_name = strdup(parent_tok.value);
        token_free(&parent_tok);
    }
    
    class_node = ast_new_class_def(class_name, parent_name, line);
    free(class_name);
    if (parent_name) free(parent_name);
    
    /* Parse class body */
    {
        Token lbrace_tok = consume(parser, TOKEN_LBRACE);
        token_free(&lbrace_tok);
    }
    
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        ASTNode *member;
        int is_static = 0;
        AccessModifier access = ACCESS_PUBLIC;
        while (match(parser, TOKEN_SEMICOLON)) {}
        if (check(parser, TOKEN_RBRACE) || check(parser, TOKEN_EOF)) break;
        
        /* Check for access modifier */
        if (match(parser, TOKEN_PUBLIC)) {
            access = ACCESS_PUBLIC;
        } else if (match(parser, TOKEN_PRIVATE)) {
            access = ACCESS_PRIVATE;
        } else if (match(parser, TOKEN_PROTECTED)) {
            access = ACCESS_PROTECTED;
        }
        
        /* Check for static keyword */
        if (match(parser, TOKEN_STATIC)) {
            is_static = 1;
        }
        
        /* Parse class members (let statements and function definitions) */
        if (check(parser, TOKEN_LET)) {
            member = parse_let(parser);
            if (member) {
                member->data.let_stmt.is_static = is_static;
                member->data.let_stmt.access = access;
                nodelist_push(&class_node->data.class_def.members, member);
            }
        } else if (check(parser, TOKEN_FN)) {
            member = parse_func_def(parser);
            if (member) {
                member->data.func_def.is_static = is_static;
                member->data.func_def.access = access;
                nodelist_push(&class_node->data.class_def.members, member);
            }
        } else {
            fprintf(stderr, "Parse error at line %d: expected 'let' or 'fn' in class body, got %s\n",
                    parser->current.line, token_type_name(parser->current.type));
            parser->had_error = 1;
            {
                Token t = advance(parser);
                token_free(&t);
            }
        }
        
        while (match(parser, TOKEN_SEMICOLON)) {}
    }
    
    {
        Token rbrace_tok = consume(parser, TOKEN_RBRACE);
        token_free(&rbrace_tok);
    }
    
    return class_node;
}

ASTNode **parse_program(Parser *parser, int *count) {
    int cap = 16;
    ASTNode **nodes = malloc(cap * sizeof(ASTNode *));
    *count = 0;

    while (!check(parser, TOKEN_EOF)) {
        ASTNode *stmt;
        while (match(parser, TOKEN_SEMICOLON)) {}
        if (check(parser, TOKEN_EOF)) break;
        stmt = parse_statement(parser);
        if (stmt) {
            if (*count >= cap) {
                cap *= 2;
                nodes = realloc(nodes, cap * sizeof(ASTNode *));
            }
            nodes[(*count)++] = stmt;
        }
    }
    return nodes;
}
