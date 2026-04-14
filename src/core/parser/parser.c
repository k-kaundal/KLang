#include "core/parser/parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_statement(Parser *parser);
static ASTNode *parse_block(Parser *parser);
static ASTNode *parse_break(Parser *parser);
static ASTNode *parse_continue(Parser *parser);
static ASTNode *parse_class_def(Parser *parser);
static ASTNode *parse_struct_def(Parser *parser, int is_union);
static ASTNode *parse_import(Parser *parser);
static ASTNode *parse_export(Parser *parser);
static ASTNode *parse_func_def(Parser *parser);
static ASTNode *parse_unsafe_block(Parser *parser);  /* Security: unsafe block parser */
static int check_ahead(Parser *parser, TokenType type);

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
    
    /* CRITICAL FIX: Return a dummy token to prevent double-free
     * The caller will free this token, so we must NOT return parser->current
     * which is still owned by the parser and will be freed later.
     */
    Token dummy;
    dummy.type = type;  /* Expected type */
    dummy.value = strdup("");  /* Empty string - safe to free */
    dummy.line = parser->current.line;
    return dummy;
}

/* Parse a template literal and convert ${...} into expression nodes */
static ASTNode *parse_template_literal(Parser *parser) {
    int line = parser->current.line;
    Token t = advance(parser);
    const char *template_str = t.value;
    ASTNode *node = ast_new_template_literal(line);
    
    /* Count parts and expressions */
    int expr_count = 0;
    int i, j;
    for (i = 0; template_str[i]; i++) {
        if (template_str[i] == '$' && template_str[i+1] == '{') {
            expr_count++;
        }
    }
    
    /* Allocate arrays for parts and expressions */
    int part_count = expr_count + 1;
    node->data.template_literal.count = part_count;
    node->data.template_literal.parts = malloc(part_count * sizeof(char*));
    node->data.template_literal.exprs = malloc(expr_count * sizeof(ASTNode*));
    
    /* Initialize parts */
    for (i = 0; i < part_count; i++) {
        node->data.template_literal.parts[i] = NULL;
    }
    
    /* Parse the template string */
    int part_idx = 0;
    int expr_idx = 0;
    int part_cap = 64;
    int part_len = 0;
    char *part_buf = malloc(part_cap);
    
    for (i = 0; template_str[i]; ) {
        if (template_str[i] == '$' && template_str[i+1] == '{') {
            /* Found interpolation start */
            /* Save current part */
            part_buf[part_len] = '\0';
            node->data.template_literal.parts[part_idx++] = strdup(part_buf);
            part_len = 0;
            
            /* Find matching } */
            i += 2; /* Skip ${ */
            int expr_start = i;
            int brace_depth = 1;
            while (template_str[i] && brace_depth > 0) {
                if (template_str[i] == '{') brace_depth++;
                else if (template_str[i] == '}') brace_depth--;
                if (brace_depth > 0) i++;
            }
            
            /* Extract expression */
            int expr_len = i - expr_start;
            char *expr_str = malloc(expr_len + 1);
            for (j = 0; j < expr_len; j++) {
                expr_str[j] = template_str[expr_start + j];
            }
            expr_str[expr_len] = '\0';
            
            /* Parse expression */
            Lexer expr_lexer;
            Parser expr_parser;
            lexer_init(&expr_lexer, expr_str);
            parser_init(&expr_parser, &expr_lexer);
            node->data.template_literal.exprs[expr_idx++] = parse_expression(&expr_parser);
            parser_free(&expr_parser);
            lexer_free(&expr_lexer);
            free(expr_str);
            
            if (template_str[i] == '}') i++; /* Skip closing } */
        } else {
            /* Regular character */
            if (part_len + 1 >= part_cap) {
                part_cap *= 2;
                part_buf = realloc(part_buf, part_cap);
            }
            part_buf[part_len++] = template_str[i++];
        }
    }
    
    /* Save final part */
    part_buf[part_len] = '\0';
    node->data.template_literal.parts[part_idx] = strdup(part_buf);
    free(part_buf);
    
    token_free(&t);
    return node;
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
    if (check(parser, TOKEN_TEMPLATE_LITERAL)) {
        return parse_template_literal(parser);
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
    if (check(parser, TOKEN_NULL)) {
        Token t = advance(parser);
        token_free(&t);
        return ast_new_null(line);
    }
    if (check(parser, TOKEN_IDENT)) {
        Token t = advance(parser);
        char *ident_name = strdup(t.value);
        token_free(&t);
        
        /* Check if this is a struct literal (Identifier { ... }) */
        if (check(parser, TOKEN_LBRACE)) {
            Token lbrace = advance(parser);
            ASTNode *struct_lit = ast_new_struct_literal(ident_name, line);
            token_free(&lbrace);
            free(ident_name);
            
            /* Parse field initializers */
            while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
                char *field_name;
                ASTNode *field_value;
                Token field_tok;
                
                while (match(parser, TOKEN_COMMA)) {}
                if (check(parser, TOKEN_RBRACE)) break;
                
                /* Parse field name */
                field_tok = consume(parser, TOKEN_IDENT);
                field_name = strdup(field_tok.value);
                token_free(&field_tok);
                
                /* Expect colon */
                {
                    Token colon = consume(parser, TOKEN_COLON);
                    token_free(&colon);
                }
                
                /* Parse field value */
                field_value = parse_expression(parser);
                
                /* Add field to struct literal */
                ast_struct_literal_add_field(struct_lit, field_name, field_value);
                free(field_name);
                
                /* Allow comma separator */
                match(parser, TOKEN_COMMA);
            }
            
            {
                Token rbrace = consume(parser, TOKEN_RBRACE);
                token_free(&rbrace);
            }
            
            return struct_lit;
        } else {
            /* Regular identifier */
            ASTNode *n = ast_new_ident(ident_name, line);
            free(ident_name);
            return n;
        }
    }
    if (check(parser, TOKEN_LPAREN)) {
        Token t = advance(parser);
        int line_lparen = t.line;
        token_free(&t);
        
        /* Check for empty tuple () */
        if (check(parser, TOKEN_RPAREN)) {
            Token t2 = advance(parser);
            token_free(&t2);
            ASTNode *tuple = ast_new_tuple(line_lparen);
            return tuple;
        }
        
        /* Parse first expression */
        ASTNode *expr = parse_expression(parser);
        
        /* Check if this is a tuple (has comma) or just grouping */
        if (check(parser, TOKEN_COMMA)) {
            /* It's a tuple */
            ASTNode *tuple = ast_new_tuple(line_lparen);
            nodelist_push(&tuple->data.tuple.elements, expr);
            
            /* Parse remaining elements */
            while (match(parser, TOKEN_COMMA)) {
                if (check(parser, TOKEN_RPAREN)) break; /* Trailing comma */
                nodelist_push(&tuple->data.tuple.elements, parse_expression(parser));
            }
            
            Token t2 = consume(parser, TOKEN_RPAREN);
            token_free(&t2);
            return tuple;
        } else {
            /* It's just a grouped expression */
            Token t2 = consume(parser, TOKEN_RPAREN);
            token_free(&t2);
            return expr;
        }
    }
    if (check(parser, TOKEN_LBRACKET)) {
        Token t = advance(parser);
        ASTNode *list;
        Token t2;
        token_free(&t);
        list = ast_new_list(line);
        while (!check(parser, TOKEN_RBRACKET) && !check(parser, TOKEN_EOF)) {
            /* Check for spread element */
            if (check(parser, TOKEN_SPREAD)) {
                Token spread_tok = advance(parser);
                token_free(&spread_tok);
                ASTNode *spread_expr = parse_expression(parser);
                ASTNode *spread_node = ast_new_spread(spread_expr, line);
                nodelist_push(&list->data.list.elements, spread_node);
            } else {
                nodelist_push(&list->data.list.elements, parse_expression(parser));
            }
            if (!match(parser, TOKEN_COMMA)) break;
        }
        t2 = consume(parser, TOKEN_RBRACKET);
        token_free(&t2);
        return list;
    }
    
    /* Object literal */
    if (check(parser, TOKEN_LBRACE)) {
        Token t = advance(parser);
        ASTNode *obj;
        token_free(&t);
        obj = ast_new_object(line);
        
        while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
            int is_shorthand = 0;
            int is_method = 0;
            char *key = NULL;
            ASTNode *key_expr = NULL;
            ASTNode *value = NULL;
            
            /* Check for spread property ...obj */
            if (check(parser, TOKEN_SPREAD)) {
                Token spread_tok = advance(parser);
                token_free(&spread_tok);
                ASTNode *spread_expr = parse_expression(parser);
                ASTNode *spread_node = ast_new_spread(spread_expr, line);
                /* Store spread as a special property with NULL key */
                ast_object_add_property(obj, NULL, NULL, spread_node, 0, 0);
                if (!match(parser, TOKEN_COMMA)) break;
                continue;
            }
            
            /* Check for computed property name [expr] */
            if (check(parser, TOKEN_LBRACKET)) {
                Token t2 = advance(parser);
                token_free(&t2);
                key_expr = parse_expression(parser);
                {
                    Token t3 = consume(parser, TOKEN_RBRACKET);
                    token_free(&t3);
                }
                {
                    Token t4 = consume(parser, TOKEN_COLON);
                    token_free(&t4);
                }
                value = parse_expression(parser);
            }
            /* Check for identifier (could be shorthand, method, or regular) */
            else if (check(parser, TOKEN_IDENT)) {
                Token id_tok = advance(parser);
                key = strdup(id_tok.value);
                token_free(&id_tok);
                
                /* Method shorthand: key() { ... } */
                if (check(parser, TOKEN_LPAREN)) {
                    Token lparen_tok = advance(parser);
                    ASTNode *func;
                    token_free(&lparen_tok);
                    is_method = 1;
                    
                    /* Create function definition */
                    func = ast_new_func_def(key, NULL, line);
                    
                    /* Parse parameters */
                    while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
                        Token param_tok = consume(parser, TOKEN_IDENT);
                        ASTNode *param = ast_new_ident(param_tok.value, line);
                        nodelist_push(&func->data.func_def.params, param);
                        token_free(&param_tok);
                        if (!match(parser, TOKEN_COMMA)) break;
                    }
                    {
                        Token rparen_tok = consume(parser, TOKEN_RPAREN);
                        token_free(&rparen_tok);
                    }
                    
                    /* Parse function body */
                    func->data.func_def.body = parse_statement(parser);
                    value = func;
                }
                /* Property shorthand or regular property */
                else if (check(parser, TOKEN_COMMA) || check(parser, TOKEN_RBRACE)) {
                    /* Shorthand: {x} means {x: x} */
                    is_shorthand = 1;
                    value = ast_new_ident(key, line);
                }
                else {
                    /* Regular property: key: value */
                    Token colon_tok = consume(parser, TOKEN_COLON);
                    token_free(&colon_tok);
                    value = parse_expression(parser);
                }
            }
            /* Allow keywords as property names (e.g., {static: value}) */
            else if (parser->current.type != TOKEN_RBRACE && parser->current.type != TOKEN_EOF) {
                /* Use the keyword token's value as the key */
                Token key_tok = advance(parser);
                key = strdup(key_tok.value);
                token_free(&key_tok);
                
                /* Must be followed by colon */
                Token colon_tok = consume(parser, TOKEN_COLON);
                token_free(&colon_tok);
                value = parse_expression(parser);
            }
            else {
                fprintf(stderr, "Syntax error: expected property key\n");
                exit(1);
            }
            
            ast_object_add_property(obj, key, key_expr, value, is_shorthand, is_method);
            if (key) free(key);
            
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        {
            Token t2 = consume(parser, TOKEN_RBRACE);
            token_free(&t2);
        }
        return obj;
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
    
    /* Function expression: fn(...) { ... } or fn name(...) { ... } */
    if (check(parser, TOKEN_FN)) {
        return parse_func_def(parser);
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
        } else if (check(parser, TOKEN_OPTIONAL_CHAIN)) {
            Token t = advance(parser);
            Token member_tok;
            char *member_name;
            token_free(&t);
            member_tok = consume(parser, TOKEN_IDENT);
            member_name = strdup(member_tok.value);
            token_free(&member_tok);
            /* Create optional chaining node */
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = NODE_OPTIONAL_CHAIN;
            node->line = line;
            node->data.optional_chain.obj = expr;
            node->data.optional_chain.member = member_name;
            expr = node;
        } else if (check(parser, TOKEN_DOT)) {
            Token t = advance(parser);
            Token member_tok;
            char *member_name;
            token_free(&t);
            /* Allow keywords as member names */
            if (parser->current.type != TOKEN_EOF && parser->current.type != TOKEN_SEMICOLON) {
                member_tok = advance(parser);
                member_name = strdup(member_tok.value);
                token_free(&member_tok);
            } else {
                fprintf(stderr, "Parse error: expected member name after '.'\n");
                parser->had_error = 1;
                member_name = strdup("");
            }
            /* Handle super.member specially */
            if (expr->type == NODE_SUPER) {
                ast_free(expr);
                expr = ast_new_super(member_name, line);
            } else {
                expr = ast_new_member_access(expr, member_name, line);
            }
            free(member_name);
        } else if (check(parser, TOKEN_ARROW)) {
            /* Handle pointer member access ptr->member (equivalent to (*ptr).member) */
            Token t = advance(parser);
            Token member_tok;
            char *member_name;
            token_free(&t);
            /* Allow keywords as member names */
            if (parser->current.type != TOKEN_EOF && parser->current.type != TOKEN_SEMICOLON) {
                member_tok = advance(parser);
                member_name = strdup(member_tok.value);
                token_free(&member_tok);
            } else {
                fprintf(stderr, "Parse error: expected member name after '->'\n");
                parser->had_error = 1;
                member_name = strdup("");
            }
            token_free(&member_tok);
            expr = ast_new_pointer_member(expr, member_name, line);
            free(member_name);
        } else if (check(parser, TOKEN_PLUS_PLUS) || check(parser, TOKEN_MINUS_MINUS)) {
            /* Postfix increment/decrement */
            Token t = advance(parser);
            char op[4];
            strncpy(op, t.value, 3);
            op[3] = '\0';
            token_free(&t);
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = NODE_POSTFIX;
            node->line = line;
            node->data.postfix.operand = expr;
            strncpy(node->data.postfix.op, op, 3);
            node->data.postfix.op[3] = '\0';
            node->data.postfix.is_postfix = 1;
            expr = node;
        } else {
            break;
        }
    }
    return expr;
}

static ASTNode *parse_unary(Parser *parser) {
    int line = parser->current.line;
    
    /* Handle await expression */
    if (check(parser, TOKEN_AWAIT)) {
        Token t = advance(parser);
        token_free(&t);
        ASTNode *expr = parse_unary(parser);
        return ast_new_await(expr, line);
    }
    
    /* Handle yield expression */
    if (check(parser, TOKEN_YIELD)) {
        Token t = advance(parser);
        token_free(&t);
        /* Check if there's a value to yield (could be empty for "yield" without value) */
        if (!check(parser, TOKEN_SEMICOLON) && !check(parser, TOKEN_RBRACE) && 
            !check(parser, TOKEN_EOF) && !check(parser, TOKEN_RPAREN) &&
            !check(parser, TOKEN_COMMA)) {
            ASTNode *value = parse_unary(parser);
            return ast_new_yield(value, line);
        } else {
            return ast_new_yield(NULL, line);
        }
    }
    
    /* Handle prefix increment/decrement */
    if (check(parser, TOKEN_PLUS_PLUS) || check(parser, TOKEN_MINUS_MINUS)) {
        Token t = advance(parser);
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        ASTNode *operand = parse_unary(parser);
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = NODE_POSTFIX;  // Reuse NODE_POSTFIX but with is_postfix=0
        node->line = line;
        node->data.postfix.operand = operand;
        strncpy(node->data.postfix.op, op, 3);
        node->data.postfix.op[3] = '\0';
        node->data.postfix.is_postfix = 0;  // Prefix
        return node;
    }
    
    /* Handle address-of operator & */
    if (check(parser, TOKEN_AMP)) {
        Token t = advance(parser);
        token_free(&t);
        ASTNode *operand = parse_unary(parser);
        return ast_new_address_of(operand, line);
    }
    
    /* Handle dereference operator * */
    /* Note: We need to be careful here - * can be either dereference or multiplication
     * In a unary context (here), it's dereference. In binary context, it's multiplication. */
    if (check(parser, TOKEN_STAR)) {
        Token t = advance(parser);
        token_free(&t);
        ASTNode *operand = parse_unary(parser);
        return ast_new_dereference(operand, line);
    }
    
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

static ASTNode *parse_exponentiation(Parser *parser) {
    ASTNode *left = parse_unary(parser);
    // Right-associative: parse recursively for right side
    if (check(parser, TOKEN_POWER)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        ASTNode *right = parse_exponentiation(parser);  // Recursive for right-associativity
        left = ast_new_binop(op, left, right, line);
    }
    return left;
}

static ASTNode *parse_multiplicative(Parser *parser) {
    ASTNode *left = parse_exponentiation(parser);
    while (check(parser, TOKEN_STAR) || check(parser, TOKEN_SLASH) || check(parser, TOKEN_PERCENT)) {
        Token t = advance(parser);
        int line = t.line;
        char op[4];
        strncpy(op, t.value, 3);
        op[3] = '\0';
        token_free(&t);
        {
            ASTNode *right = parse_exponentiation(parser);
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

static ASTNode *parse_logical_and(Parser *parser) {
    ASTNode *left = parse_equality(parser);
    while (check(parser, TOKEN_AND_AND)) {
        Token t = advance(parser);
        int line = t.line;
        token_free(&t);
        {
            ASTNode *right = parse_equality(parser);
            left = ast_new_binop("&&", left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_logical_or(Parser *parser) {
    ASTNode *left = parse_logical_and(parser);
    while (check(parser, TOKEN_OR_OR)) {
        Token t = advance(parser);
        int line = t.line;
        token_free(&t);
        {
            ASTNode *right = parse_logical_and(parser);
            left = ast_new_binop("||", left, right, line);
        }
    }
    return left;
}

static ASTNode *parse_nullish_coalesce(Parser *parser) {
    ASTNode *left = parse_logical_or(parser);
    while (check(parser, TOKEN_NULLISH_COALESCE)) {
        Token t = advance(parser);
        int line = t.line;
        token_free(&t);
        {
            ASTNode *right = parse_logical_or(parser);
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = NODE_NULLISH_COALESCE;
            node->line = line;
            node->data.nullish_coalesce.left = left;
            node->data.nullish_coalesce.right = right;
            left = node;
        }
    }
    return left;
}

static ASTNode *parse_ternary(Parser *parser) {
    ASTNode *expr = parse_nullish_coalesce(parser);
    
    if (check(parser, TOKEN_QUESTION)) {
        int line = parser->current.line;
        Token q = advance(parser);
        token_free(&q);
        
        ASTNode *true_expr = parse_ternary(parser);  // Right-associative, so call parse_ternary recursively
        
        Token colon = consume(parser, TOKEN_COLON);
        token_free(&colon);
        
        ASTNode *false_expr = parse_ternary(parser);  // Right-associative
        
        expr = ast_new_ternary(expr, true_expr, false_expr, line);
    }
    
    return expr;
}

static int is_arrow_function_start(Parser *parser) {
    // Check for single param without parens: ident => ...
    if (check(parser, TOKEN_IDENT) && parser->peek.type == TOKEN_FAT_ARROW) {
        return 1;
    }
    
    // Check for params with parens: () => ... or (params) => ...
    if (!check(parser, TOKEN_LPAREN)) {
        return 0;
    }
    
    // Scan the source directly to check for the pattern
    const char *src = parser->lexer->source;
    int pos = parser->lexer->pos - 1; // Current position is at the character after '('
    
    // Find where the current '(' is in the source by going backwards
    while (pos >= 0 && src[pos] != '(') pos--;
    if (pos < 0 || src[pos] != '(') return 0;
    
    // Now scan forward to find the matching ')'
    pos++; // Skip the '('
    int paren_depth = 1;
    while (src[pos] && paren_depth > 0) {
        if (src[pos] == '(') paren_depth++;
        else if (src[pos] == ')') paren_depth--;
        pos++;
    }
    
    if (paren_depth != 0) return 0; // Unmatched parens
    
    // Now pos is right after the ')', skip whitespace and check for =>
    while (src[pos] && (src[pos] == ' ' || src[pos] == '\t' || src[pos] == '\r' || src[pos] == '\n'))
        pos++;
    
    return (src[pos] == '=' && src[pos + 1] == '>');
}

static ASTNode *parse_arrow_function(Parser *parser);

static ASTNode *parse_expression(Parser *parser) {
    // Check for async arrow function: async (x) => ... or async x => ...
    if (check(parser, TOKEN_ASYNC)) {
        // Peek ahead to see if this is an arrow function
        if (parser->peek.type == TOKEN_IDENT || parser->peek.type == TOKEN_LPAREN) {
            // Look for => after identifier or (params)
            int saved_pos = parser->lexer->pos;
            int saved_line = parser->lexer->line;
            int saved_col = parser->lexer->col;
            Token saved_current = parser->current;
            Token saved_peek = parser->peek;
            
            // Advance past async
            advance(parser);
            
            // Check if followed by arrow function pattern
            int is_async_arrow = 0;
            if (check(parser, TOKEN_IDENT) && parser->peek.type == TOKEN_FAT_ARROW) {
                is_async_arrow = 1;
            } else if (check(parser, TOKEN_LPAREN)) {
                is_async_arrow = is_arrow_function_start(parser);
            }
            
            // Restore parser state
            parser->lexer->pos = saved_pos;
            parser->lexer->line = saved_line;
            parser->lexer->col = saved_col;
            parser->current = saved_current;
            parser->peek = saved_peek;
            
            if (is_async_arrow) {
                // Parse async arrow function
                Token async_tok = advance(parser);
                token_free(&async_tok);
                ASTNode *func = parse_arrow_function(parser);
                func->data.func_def.is_async = 1;
                return func;
            }
        }
    }
    
    // Check if this is a regular arrow function
    if (is_arrow_function_start(parser)) {
        return parse_arrow_function(parser);
    }
    
    return parse_ternary(parser);
}

static ASTNode *parse_arrow_function(Parser *parser) {
    int line = parser->current.line;
    ASTNode *func = ast_new_func_def("", NULL, line);
    func->data.func_def.is_arrow = 1;
    
    // Parse parameters
    if (check(parser, TOKEN_IDENT)) {
        // Single param without parens: x => ...
        Token param_tok = advance(parser);
        nodelist_push(&func->data.func_def.params, ast_new_ident(param_tok.value, line));
        token_free(&param_tok);
    } else if (check(parser, TOKEN_LPAREN)) {
        // Params with parens: () => ... or (x, y) => ...
        Token lp = advance(parser);
        token_free(&lp);
        
        while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
            Token param_tok = consume(parser, TOKEN_IDENT);
            nodelist_push(&func->data.func_def.params, ast_new_ident(param_tok.value, line));
            token_free(&param_tok);
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        Token rp = consume(parser, TOKEN_RPAREN);
        token_free(&rp);
    }
    
    // Consume =>
    Token arrow_tok = consume(parser, TOKEN_FAT_ARROW);
    token_free(&arrow_tok);
    
    // Parse body
    if (check(parser, TOKEN_LBRACE)) {
        // Block body: x => { return x * 2 }
        func->data.func_def.body = parse_block(parser);
    } else {
        // Expression body: x => x * 2 (or nested arrow: x => y => x + y)
        // Wrap the expression in a return statement within a block
        ASTNode *expr = parse_expression(parser);
        ASTNode *ret = ast_new_return(expr, line);
        ASTNode *block = ast_new_block(line);
        nodelist_push(&block->data.block.stmts, ret);
        func->data.func_def.body = block;
    }
    
    return func;
}

/* Helper function: Parse statement or block
 * If we see '{', parse as block. Otherwise, parse single statement and wrap in block.
 * This allows both `if (x) return` and `if (x) { return }` syntax.
 */
static ASTNode *parse_statement_or_block(Parser *parser) {
    int line = parser->current.line;
    if (check(parser, TOKEN_LBRACE)) {
        return parse_block(parser);
    } else {
        /* Single statement - wrap in a block */
        ASTNode *block = ast_new_block(line);
        ASTNode *stmt = parse_statement(parser);
        if (stmt) nodelist_push(&block->data.block.stmts, stmt);
        return block;
    }
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

/* Security: Parse unsafe block
 * Syntax: unsafe { statements }
 * Supports nested unsafe blocks
 */
static ASTNode *parse_unsafe_block(Parser *parser) {
    int line = parser->current.line;
    Token unsafe_tok = consume(parser, TOKEN_UNSAFE);
    ASTNode *unsafe_block;
    token_free(&unsafe_tok);
    
    /* Expect opening brace */
    Token lbrace = consume(parser, TOKEN_LBRACE);
    token_free(&lbrace);
    
    /* Create unsafe block node */
    unsafe_block = ast_new_unsafe_block(line);
    
    /* Parse statements inside the unsafe block */
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        ASTNode *stmt;
        while (match(parser, TOKEN_SEMICOLON)) {}
        if (check(parser, TOKEN_RBRACE) || check(parser, TOKEN_EOF)) break;
        stmt = parse_statement(parser);
        if (stmt) nodelist_push(&unsafe_block->data.unsafe_block.stmts, stmt);
        while (match(parser, TOKEN_SEMICOLON)) {}
    }
    
    /* Expect closing brace */
    {
        Token rbrace = consume(parser, TOKEN_RBRACE);
        token_free(&rbrace);
    }
    
    return unsafe_block;
}

/* Parse destructuring array pattern element */
static ASTNode *parse_destructure_array_element(Parser *parser) {
    int line = parser->current.line;
    
    /* Check for hole/skip: const [a, , c] = arr */
    if (check(parser, TOKEN_COMMA) || check(parser, TOKEN_RBRACKET)) {
        return ast_new_destructure_element(NULL, NULL, NULL, 0, 1, line);
    }
    
    /* Check for rest element: ...rest */
    if (check(parser, TOKEN_SPREAD)) {
        Token spread_tok = advance(parser);
        token_free(&spread_tok);
        
        if (!check(parser, TOKEN_IDENT)) {
            fprintf(stderr, "Parse error at line %d: expected identifier after ...\n", line);
            parser->had_error = 1;
            return NULL;
        }
        
        Token name_tok = advance(parser);
        ASTNode *elem = ast_new_destructure_element(name_tok.value, NULL, NULL, 1, 0, line);
        token_free(&name_tok);
        return elem;
    }
    
    /* Check for nested array destructuring: const [[a, b], c] = arr */
    if (check(parser, TOKEN_LBRACKET)) {
        /* Parse nested array pattern */
        Token lbracket = advance(parser);
        token_free(&lbracket);
        
        ASTNode *nested = ast_new_destructure_array(NULL, DECL_LET, line);
        while (!check(parser, TOKEN_RBRACKET) && !check(parser, TOKEN_EOF)) {
            ASTNode *elem = parse_destructure_array_element(parser);
            if (elem) nodelist_push(&nested->data.destructure_array.elements, elem);
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        Token rbracket = consume(parser, TOKEN_RBRACKET);
        token_free(&rbracket);
        
        ASTNode *wrapper = ast_new_destructure_element(NULL, NULL, NULL, 0, 0, line);
        wrapper->data.destructure_element.nested = nested;
        return wrapper;
    }
    
    /* Regular identifier with optional default */
    if (!check(parser, TOKEN_IDENT)) {
        fprintf(stderr, "Parse error at line %d: expected identifier in destructuring pattern\n", line);
        parser->had_error = 1;
        return NULL;
    }
    
    Token name_tok = advance(parser);
    char *name = strdup(name_tok.value);
    token_free(&name_tok);
    
    ASTNode *default_value = NULL;
    if (match(parser, TOKEN_ASSIGN)) {
        default_value = parse_expression(parser);
    }
    
    ASTNode *elem = ast_new_destructure_element(name, NULL, default_value, 0, 0, line);
    free(name);
    return elem;
}

/* Parse destructuring object pattern element */
static ASTNode *parse_destructure_object_element(Parser *parser) {
    int line = parser->current.line;
    
    /* Check for rest element: ...rest */
    if (check(parser, TOKEN_SPREAD)) {
        Token spread_tok = advance(parser);
        token_free(&spread_tok);
        
        if (!check(parser, TOKEN_IDENT)) {
            fprintf(stderr, "Parse error at line %d: expected identifier after ...\n", line);
            parser->had_error = 1;
            return NULL;
        }
        
        Token name_tok = advance(parser);
        ASTNode *elem = ast_new_destructure_element(name_tok.value, NULL, NULL, 1, 0, line);
        token_free(&name_tok);
        return elem;
    }
    
    if (!check(parser, TOKEN_IDENT)) {
        fprintf(stderr, "Parse error at line %d: expected identifier in object destructuring\n", line);
        parser->had_error = 1;
        return NULL;
    }
    
    Token key_tok = advance(parser);
    char *key = strdup(key_tok.value);
    char *name = strdup(key_tok.value); /* Default: same as key */
    token_free(&key_tok);
    
    /* Check for rename: {x: newX} */
    if (match(parser, TOKEN_COLON)) {
        /* Check for nested object destructuring: {outer: {inner}} */
        if (check(parser, TOKEN_LBRACE)) {
            Token lbrace = advance(parser);
            token_free(&lbrace);
            
            ASTNode *nested = ast_new_destructure_object(NULL, DECL_LET, line);
            while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
                ASTNode *elem = parse_destructure_object_element(parser);
                if (elem) nodelist_push(&nested->data.destructure_object.properties, elem);
                if (!match(parser, TOKEN_COMMA)) break;
            }
            
            Token rbrace = consume(parser, TOKEN_RBRACE);
            token_free(&rbrace);
            
            ASTNode *wrapper = ast_new_destructure_element(NULL, key, NULL, 0, 0, line);
            wrapper->data.destructure_element.nested = nested;
            free(key);
            free(name);
            return wrapper;
        }
        
        /* Regular rename */
        Token name_tok = consume(parser, TOKEN_IDENT);
        free(name);
        name = strdup(name_tok.value);
        token_free(&name_tok);
    }
    
    /* Check for default value */
    ASTNode *default_value = NULL;
    if (match(parser, TOKEN_ASSIGN)) {
        default_value = parse_expression(parser);
    }
    
    ASTNode *elem = ast_new_destructure_element(name, key, default_value, 0, 0, line);
    free(key);
    free(name);
    return elem;
}

static ASTNode *parse_var_decl(Parser *parser, TokenType decl_token, DeclType decl_type) {
    int line = parser->current.line;
    Token decl_tok = consume(parser, decl_token);
    token_free(&decl_tok);
    
    /* Check for array destructuring: const [a, b] = ... */
    if (check(parser, TOKEN_LBRACKET)) {
        Token lbracket = advance(parser);
        token_free(&lbracket);
        
        ASTNode *destructure = ast_new_destructure_array(NULL, decl_type, line);
        
        /* Parse array elements */
        while (!check(parser, TOKEN_RBRACKET) && !check(parser, TOKEN_EOF)) {
            ASTNode *elem = parse_destructure_array_element(parser);
            if (elem) {
                nodelist_push(&destructure->data.destructure_array.elements, elem);
            }
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        Token rbracket = consume(parser, TOKEN_RBRACKET);
        token_free(&rbracket);
        
        /* Must have initializer */
        if (!match(parser, TOKEN_ASSIGN)) {
            if (decl_type == DECL_CONST) {
                fprintf(stderr, "Error at line %d: const destructuring must be initialized\n", line);
                parser->had_error = 1;
            }
            /* For let/var, allow uninitialized (will be undefined) */
            destructure->data.destructure_array.source = ast_new_list(line);
        } else {
            destructure->data.destructure_array.source = parse_expression(parser);
        }
        
        return destructure;
    }
    
    /* Check for tuple destructuring: const (a, b) = ... */
    if (check(parser, TOKEN_LPAREN)) {
        Token lparen = advance(parser);
        token_free(&lparen);
        
        ASTNode *destructure = ast_new_destructure_array(NULL, decl_type, line);
        
        /* Parse tuple elements */
        while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
            ASTNode *elem = parse_destructure_array_element(parser);
            if (elem) {
                nodelist_push(&destructure->data.destructure_array.elements, elem);
            }
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        Token rparen = consume(parser, TOKEN_RPAREN);
        token_free(&rparen);
        
        /* Must have initializer */
        if (!match(parser, TOKEN_ASSIGN)) {
            if (decl_type == DECL_CONST) {
                fprintf(stderr, "Error at line %d: const destructuring must be initialized\n", line);
                parser->had_error = 1;
            }
            /* For let/var, allow uninitialized (will be undefined) */
            destructure->data.destructure_array.source = ast_new_tuple(line);
        } else {
            destructure->data.destructure_array.source = parse_expression(parser);
        }
        
        return destructure;
    }
    
    /* Check for object destructuring: const {x, y} = ... */
    if (check(parser, TOKEN_LBRACE)) {
        Token lbrace = advance(parser);
        token_free(&lbrace);
        
        ASTNode *destructure = ast_new_destructure_object(NULL, decl_type, line);
        
        /* Parse object properties */
        while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
            ASTNode *elem = parse_destructure_object_element(parser);
            if (elem) {
                nodelist_push(&destructure->data.destructure_object.properties, elem);
            }
            if (!match(parser, TOKEN_COMMA)) break;
        }
        
        Token rbrace = consume(parser, TOKEN_RBRACE);
        token_free(&rbrace);
        
        /* Must have initializer */
        if (!match(parser, TOKEN_ASSIGN)) {
            if (decl_type == DECL_CONST) {
                fprintf(stderr, "Error at line %d: const destructuring must be initialized\n", line);
                parser->had_error = 1;
            }
            /* For let/var, allow uninitialized (will be undefined) */
            destructure->data.destructure_object.source = ast_new_object(line);
        } else {
            destructure->data.destructure_object.source = parse_expression(parser);
        }
        
        return destructure;
    }
    
    /* Regular variable declaration */
    Token name_tok;
    char *varname;
    char *type_annot = NULL;
    ASTNode *value = NULL;
    ASTNode *n;
    
    name_tok = consume(parser, TOKEN_IDENT);
    varname = strdup(name_tok.value);
    token_free(&name_tok);

    if (match(parser, TOKEN_COLON)) {
        Token type_tok = consume(parser, TOKEN_IDENT);
        type_annot = strdup(type_tok.value);
        token_free(&type_tok);
    }

    /* Handle initialization */
    if (match(parser, TOKEN_ASSIGN)) {
        value = parse_expression(parser);
    } else if (decl_type == DECL_CONST) {
        /* const must be initialized */
        fprintf(stderr, "Error at line %d: const declaration must be initialized\n", line);
        free(varname);
        if (type_annot) free(type_annot);
        return NULL;
    }
    
    n = ast_new_var_decl(varname, type_annot, value, decl_type, line);
    free(varname);
    if (type_annot) free(type_annot);
    return n;
}

static ASTNode *parse_let(Parser *parser) {
    return parse_var_decl(parser, TOKEN_LET, DECL_LET);
}

static ASTNode *parse_var(Parser *parser) {
    return parse_var_decl(parser, TOKEN_VAR, DECL_VAR);
}

static ASTNode *parse_const(Parser *parser) {
    return parse_var_decl(parser, TOKEN_CONST, DECL_CONST);
}

static ASTNode *parse_func_def(Parser *parser) {
    int line = parser->current.line;
    int is_async = 0;
    int is_generator = 0;
    Token fn_tok;
    Token name_tok;
    char *fname;
    char **param_names = NULL;
    char **param_types = NULL;
    ASTNode **param_defaults = NULL;
    int param_count = 0;
    int param_cap = 0;
    int seen_default = 0; /* Track if we've seen a parameter with default */
    char *return_type = NULL;
    ASTNode *func = NULL;
    
    /* Check for async keyword before fn */
    if (check(parser, TOKEN_ASYNC)) {
        Token async_tok = advance(parser);
        token_free(&async_tok);
        is_async = 1;
    }
    
    fn_tok = consume(parser, TOKEN_FN);
    token_free(&fn_tok);
    
    /* Check for * after fn (generator function) */
    if (check(parser, TOKEN_STAR)) {
        Token star_tok = advance(parser);
        token_free(&star_tok);
        is_generator = 1;
    }
    
    /* Function name is optional for expressions (anonymous functions) */
    if (check(parser, TOKEN_IDENT)) {
        name_tok = advance(parser);
        fname = strdup(name_tok.value);
        token_free(&name_tok);
    } else {
        /* Anonymous function */
        fname = strdup("");
    }

    {
        Token lp_tok = consume(parser, TOKEN_LPAREN);
        token_free(&lp_tok);
    }

    while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
        int is_rest = 0;
        
        /* Check for rest parameter */
        if (check(parser, TOKEN_SPREAD)) {
            Token spread_tok = advance(parser);
            token_free(&spread_tok);
            is_rest = 1;
        }
        
        Token pname_tok = consume(parser, TOKEN_IDENT);
        if (param_count >= param_cap) {
            param_cap = param_cap == 0 ? 4 : param_cap * 2;
            param_names = realloc(param_names, param_cap * sizeof(char *));
            param_types = realloc(param_types, param_cap * sizeof(char *));
            param_defaults = realloc(param_defaults, param_cap * sizeof(ASTNode *));
        }
        param_names[param_count] = strdup(pname_tok.value);
        token_free(&pname_tok);
        param_types[param_count] = NULL;
        param_defaults[param_count] = NULL;
        
        if (match(parser, TOKEN_COLON)) {
            Token ptype_tok = consume(parser, TOKEN_IDENT);
            param_types[param_count] = strdup(ptype_tok.value);
            token_free(&ptype_tok);
        }
        
        /* Check for default value: name = expr */
        if (match(parser, TOKEN_ASSIGN)) {
            param_defaults[param_count] = parse_expression(parser);
            seen_default = 1;
        } else if (seen_default && !is_rest) {
            /* Required parameter after default parameter */
            fprintf(stderr, "Parse error at line %d: Required parameter '%s' cannot follow parameter with default value\n", 
                    parser->current.line, param_names[param_count]);
            parser->had_error = 1;
        }
        
        param_count++;
        
        /* Rest parameter must be last */
        if (is_rest) {
            func = ast_new_func_def(fname, return_type, line);
            func->data.func_def.has_rest_param = 1;
            if (!check(parser, TOKEN_RPAREN)) {
                fprintf(stderr, "Parse error at line %d: Rest parameter must be last\n", parser->current.line);
                parser->had_error = 1;
            }
            break;
        }
        
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

    /* Create func if not already created (rest param creates it early) */
    if (!func) {
        func = ast_new_func_def(fname, return_type, line);
    }
    free(fname);
    if (return_type) free(return_type);

    func->data.func_def.param_types = param_types;
    func->data.func_def.default_values = param_defaults;
    {
        int i;
        for (i = 0; i < param_count; i++) {
            nodelist_push(&func->data.func_def.params, ast_new_ident(param_names[i], line));
            free(param_names[i]);
        }
    }
    free(param_names);

    /* Set async and generator flags */
    func->data.func_def.is_async = is_async;
    func->data.func_def.is_generator = is_generator;

    // Check if this is an abstract method (no body, just semicolon)
    if (check(parser, TOKEN_SEMICOLON)) {
        func->data.func_def.body = NULL;
        // Don't consume semicolon here - let the class parsing handle it
    } else {
        func->data.func_def.body = parse_block(parser);
    }
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
    
    /* CRITICAL FIX: Allow single-statement bodies (no braces required) */
    then_block = parse_statement_or_block(parser);
    
    if (match(parser, TOKEN_ELSE)) {
        if (check(parser, TOKEN_IF)) {
            else_block = parse_if(parser);
        } else {
            /* Same logic for else block */
            else_block = parse_statement_or_block(parser);
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
    
    /* CRITICAL FIX: Allow single-statement bodies (no braces required) */
    body = parse_statement_or_block(parser);
    
    return ast_new_while(cond, body, line);
}

static ASTNode *parse_switch(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_SWITCH);
    ASTNode *expr;
    ASTNode *switch_node;
    token_free(&t);
    
    /* Consume '(' */
    {
        Token lparen = consume(parser, TOKEN_LPAREN);
        token_free(&lparen);
    }
    
    expr = parse_expression(parser);
    
    /* Consume ')' */
    {
        Token rparen = consume(parser, TOKEN_RPAREN);
        token_free(&rparen);
    }
    
    switch_node = ast_new_switch(expr, line);
    
    /* Consume '{' */
    {
        Token lbrace = consume(parser, TOKEN_LBRACE);
        token_free(&lbrace);
    }
    
    /* Parse cases and default */
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (check(parser, TOKEN_CASE)) {
            Token case_tok = consume(parser, TOKEN_CASE);
            ASTNode *case_value = parse_expression(parser);
            ASTNode *case_node;
            token_free(&case_tok);
            
            /* Consume ':' */
            {
                Token colon = consume(parser, TOKEN_COLON);
                token_free(&colon);
            }
            
            case_node = ast_new_case(case_value, line);
            
            /* Parse statements until next case/default/closing brace */
            while (!check(parser, TOKEN_CASE) && !check(parser, TOKEN_DEFAULT) && 
                   !check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
                ASTNode *stmt = parse_statement(parser);
                if (stmt) {
                    nodelist_push(&case_node->data.case_stmt.body, stmt);
                }
            }
            
            nodelist_push(&switch_node->data.switch_stmt.cases, case_node);
        } else if (check(parser, TOKEN_DEFAULT)) {
            Token default_tok = consume(parser, TOKEN_DEFAULT);
            ASTNode *default_block = ast_new_block(line);
            token_free(&default_tok);
            
            /* Consume ':' */
            {
                Token colon = consume(parser, TOKEN_COLON);
                token_free(&colon);
            }
            
            /* Parse statements until closing brace */
            while (!check(parser, TOKEN_CASE) && !check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
                ASTNode *stmt = parse_statement(parser);
                if (stmt) {
                    nodelist_push(&default_block->data.block.stmts, stmt);
                }
            }
            
            switch_node->data.switch_stmt.default_case = default_block;
        } else {
            break;
        }
    }
    
    /* Consume '}' */
    {
        Token rbrace = consume(parser, TOKEN_RBRACE);
        token_free(&rbrace);
    }
    
    return switch_node;
}

static ASTNode *parse_for(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_FOR);
    token_free(&t);
    
    // Check if this is a for loop with parentheses
    if (check(parser, TOKEN_LPAREN)) {
        Token lparen = consume(parser, TOKEN_LPAREN);
        token_free(&lparen);
        
        // Now we need to distinguish between:
        // 1. C-style: for (let i = 0; i < 5; i++)
        // 2. for-of with parens: for (item of arr) or for (const item of arr)
        
        // Try to determine which one by parsing the beginning
        DeclType decl_type = DECL_LET;
        
        // Check for optional declaration keyword
        if (check(parser, TOKEN_VAR) || check(parser, TOKEN_LET) || check(parser, TOKEN_CONST)) {
            Token decl_tok = advance(parser);
            if (decl_tok.type == TOKEN_VAR) decl_type = DECL_VAR;
            else if (decl_tok.type == TOKEN_LET) decl_type = DECL_LET;
            else if (decl_tok.type == TOKEN_CONST) decl_type = DECL_CONST;
            token_free(&decl_tok);
        }
        
        // After optional decl keyword, we should have an identifier
        if (check(parser, TOKEN_IDENT)) {
            Token ident_tok = advance(parser);
            char *varname = strdup(ident_tok.value);
            token_free(&ident_tok);
            
            // Now check what comes next
            if (check(parser, TOKEN_OF)) {
                // This is a for-of loop: for (item of arr) or for (let item of arr)
                Token of_tok = consume(parser, TOKEN_OF);
                token_free(&of_tok);
                
                ASTNode *iterable = parse_expression(parser);
                Token rparen = consume(parser, TOKEN_RPAREN);
                token_free(&rparen);
                
                ASTNode *body = parse_statement_or_block(parser);
                ASTNode *n = ast_new_for_of(varname, iterable, body, decl_type, line);
                free(varname);
                return n;
            } else if (check(parser, TOKEN_ASSIGN) || check(parser, TOKEN_COLON)) {
                // This is C-style: for (let i = 0; ...) or for (i = 0; ...)
                
                // We need to create the init node
                ASTNode *init = NULL;
                
                // Handle type annotation if present
                char *type_annot = NULL;
                if (check(parser, TOKEN_COLON)) {
                    Token colon_tok = advance(parser);
                    token_free(&colon_tok);
                    Token type_tok = consume(parser, TOKEN_IDENT);
                    type_annot = strdup(type_tok.value);
                    token_free(&type_tok);
                }
                
                // Now we should have ASSIGN
                Token assign_tok = consume(parser, TOKEN_ASSIGN);
                token_free(&assign_tok);
                
                ASTNode *value = parse_expression(parser);
                
                // Create the appropriate node based on whether we had a decl keyword
                if (decl_type != DECL_LET || type_annot != NULL) {
                    // We had a declaration keyword or type annotation
                    init = ast_new_var_decl(varname, type_annot, value, decl_type, line);
                } else {
                    // Plain assignment (e.g., for (i = 0; ...))
                    init = ast_new_assign(varname, value, line);
                }
                
                if (type_annot) free(type_annot);
                free(varname);
                
                // Now continue parsing the C-style for loop
                Token semi1 = consume(parser, TOKEN_SEMICOLON);
                token_free(&semi1);
                
                // Parse condition expression
                ASTNode *cond = NULL;
                if (!check(parser, TOKEN_SEMICOLON)) {
                    cond = parse_expression(parser);
                }
                
                Token semi2 = consume(parser, TOKEN_SEMICOLON);
                token_free(&semi2);
                
                // Parse update expression (can be assignment)
                ASTNode *update = NULL;
                if (!check(parser, TOKEN_RPAREN)) {
                    update = parse_expression(parser);
                    
                    // Handle assignment in update clause
                    if (update && check(parser, TOKEN_ASSIGN)) {
                        if (update->type == NODE_IDENT) {
                            char *name = strdup(update->data.ident.name);
                            ASTNode *val;
                            ast_free(update);
                            {
                                Token t = advance(parser);
                                token_free(&t);
                            }
                            val = parse_expression(parser);
                            update = ast_new_assign(name, val, line);
                            free(name);
                        } else if (update->type == NODE_MEMBER_ACCESS || update->type == NODE_INDEX) {
                            // Handle member/index assignment
                            Token asn_tok = advance(parser);
                            ASTNode *val = parse_expression(parser);
                            update = ast_new_binop("=", update, val, line);
                            token_free(&asn_tok);
                        }
                    }
                }
                
                Token rparen = consume(parser, TOKEN_RPAREN);
                token_free(&rparen);
                
                // Parse body
                ASTNode *body = parse_statement_or_block(parser);
                
                return ast_new_for_c_style(init, cond, update, body, line);
            } else if (check(parser, TOKEN_SEMICOLON)) {
                // C-style with just identifier as init (e.g., for (i; i < 10; i++))
                // This is unusual but valid - treat 'i' as an expression statement
                // We already consumed the identifier, so init is just that identifier
                ASTNode *init = ast_new_ident(varname, line);
                free(varname);
                
                Token semi1 = consume(parser, TOKEN_SEMICOLON);
                token_free(&semi1);
                
                // Parse condition expression
                ASTNode *cond = NULL;
                if (!check(parser, TOKEN_SEMICOLON)) {
                    cond = parse_expression(parser);
                }
                
                Token semi2 = consume(parser, TOKEN_SEMICOLON);
                token_free(&semi2);
                
                // Parse update expression (can be assignment)
                ASTNode *update = NULL;
                if (!check(parser, TOKEN_RPAREN)) {
                    update = parse_expression(parser);
                    
                    // Handle assignment in update clause
                    if (update && check(parser, TOKEN_ASSIGN)) {
                        if (update->type == NODE_IDENT) {
                            char *name = strdup(update->data.ident.name);
                            ASTNode *val;
                            ast_free(update);
                            {
                                Token t = advance(parser);
                                token_free(&t);
                            }
                            val = parse_expression(parser);
                            update = ast_new_assign(name, val, line);
                            free(name);
                        } else if (update->type == NODE_MEMBER_ACCESS || update->type == NODE_INDEX) {
                            // Handle member/index assignment
                            Token asn_tok = advance(parser);
                            ASTNode *val = parse_expression(parser);
                            update = ast_new_binop("=", update, val, line);
                            token_free(&asn_tok);
                        }
                    }
                }
                
                Token rparen = consume(parser, TOKEN_RPAREN);
                token_free(&rparen);
                
                // Parse body
                ASTNode *body = parse_statement_or_block(parser);
                
                return ast_new_for_c_style(init, cond, update, body, line);
            } else {
                fprintf(stderr, "Parse error at line %d: unexpected token after identifier in for loop\n", line);
                parser->had_error = 1;
                free(varname);
                return NULL;
            }
        } else if (check(parser, TOKEN_SEMICOLON)) {
            // C-style with empty init: for (; cond; update)
            Token semi1 = consume(parser, TOKEN_SEMICOLON);
            token_free(&semi1);
            
            // Parse condition expression
            ASTNode *cond = NULL;
            if (!check(parser, TOKEN_SEMICOLON)) {
                cond = parse_expression(parser);
            }
            
            Token semi2 = consume(parser, TOKEN_SEMICOLON);
            token_free(&semi2);
            
            // Parse update expression (can be assignment)
            ASTNode *update = NULL;
            if (!check(parser, TOKEN_RPAREN)) {
                update = parse_expression(parser);
                
                // Handle assignment in update clause
                if (update && check(parser, TOKEN_ASSIGN)) {
                    if (update->type == NODE_IDENT) {
                        char *name = strdup(update->data.ident.name);
                        ASTNode *val;
                        ast_free(update);
                        {
                            Token t = advance(parser);
                            token_free(&t);
                        }
                        val = parse_expression(parser);
                        update = ast_new_assign(name, val, line);
                        free(name);
                    } else if (update->type == NODE_MEMBER_ACCESS || update->type == NODE_INDEX) {
                        // Handle member/index assignment
                        Token asn_tok = advance(parser);
                        ASTNode *val = parse_expression(parser);
                        update = ast_new_binop("=", update, val, line);
                        token_free(&asn_tok);
                    }
                }
            }
            
            Token rparen = consume(parser, TOKEN_RPAREN);
            token_free(&rparen);
            
            // Parse body
            ASTNode *body = parse_statement_or_block(parser);
            
            return ast_new_for_c_style(NULL, cond, update, body, line);
        } else {
            fprintf(stderr, "Parse error at line %d: unexpected token in for loop\n", line);
            parser->had_error = 1;
            return NULL;
        }
    }
    
    // Otherwise, it's a range-based or collection-based for loop
    Token var_tok;
    char *varname;
    DeclType decl_type = DECL_LET;  // Default to let for simple for loops
    ASTNode *body;
    ASTNode *n;
    
    // Check if we have var/let/const declaration
    if (check(parser, TOKEN_VAR) || check(parser, TOKEN_LET) || check(parser, TOKEN_CONST)) {
        Token decl_tok = advance(parser);
        if (decl_tok.type == TOKEN_VAR) decl_type = DECL_VAR;
        else if (decl_tok.type == TOKEN_LET) decl_type = DECL_LET;
        else if (decl_tok.type == TOKEN_CONST) decl_type = DECL_CONST;
        token_free(&decl_tok);
    }
    
    var_tok = consume(parser, TOKEN_IDENT);
    varname = strdup(var_tok.value);
    token_free(&var_tok);
    
    // Check if it's for-of or for-in
    if (check(parser, TOKEN_OF)) {
        // for-of loop: for (item of iterable) { ... }
        Token of_tok = consume(parser, TOKEN_OF);
        ASTNode *iterable;
        token_free(&of_tok);
        
        iterable = parse_expression(parser);
        body = parse_statement_or_block(parser);
        n = ast_new_for_of(varname, iterable, body, decl_type, line);
        free(varname);
        return n;
    } else {
        // for-in loop: for (i in start..end) { ... }
        ASTNode *start;
        ASTNode *end;
        Token in_tok = consume(parser, TOKEN_IN);
        token_free(&in_tok);
        
        start = parse_expression(parser);
        {
            Token dd_tok = consume(parser, TOKEN_DOTDOT);
            token_free(&dd_tok);
        }
        end = parse_expression(parser);
        body = parse_statement_or_block(parser);
        n = ast_new_for(varname, start, end, body, line);
        free(varname);
        return n;
    }
}

static ASTNode *parse_return(Parser *parser) {
    int line = parser->current.line;
    Token t = consume(parser, TOKEN_RETURN);
    ASTNode *value = NULL;
    token_free(&t);
    
    /* CRITICAL FIX: When return is used without braces (single-statement body),
     * we need to be careful not to consume the next statement as an expression.
     * Check if the next token can start a new statement.
     */
    int is_statement_start = (
        check(parser, TOKEN_IF) || check(parser, TOKEN_FOR) || check(parser, TOKEN_WHILE) ||
        check(parser, TOKEN_LET) || check(parser, TOKEN_VAR) || check(parser, TOKEN_CONST) ||
        check(parser, TOKEN_RETURN) || check(parser, TOKEN_BREAK) || check(parser, TOKEN_CONTINUE) ||
        check(parser, TOKEN_FN) || check(parser, TOKEN_CLASS) || check(parser, TOKEN_STRUCT) ||
        check(parser, TOKEN_SWITCH) || check(parser, TOKEN_IMPORT) || check(parser, TOKEN_EXPORT) ||
        check(parser, TOKEN_UNSAFE)
    );
    
    if (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF) && 
        !check(parser, TOKEN_SEMICOLON) && !is_statement_start) {
        value = parse_expression(parser);
    }
    return ast_new_return(value, line);
}

static ASTNode *parse_statement(Parser *parser) {
    while (match(parser, TOKEN_SEMICOLON)) {}

    /* Module system - must be at top level */
    if (check(parser, TOKEN_IMPORT)) return parse_import(parser);
    if (check(parser, TOKEN_EXPORT)) return parse_export(parser);

    if (check(parser, TOKEN_ABSTRACT)) {
        Token abstract_tok = advance(parser);
        token_free(&abstract_tok);
        if (check(parser, TOKEN_CLASS)) {
            ASTNode *class_node = parse_class_def(parser);
            if (class_node) class_node->data.class_def.is_abstract = 1;
            return class_node;
        }
    }
    
    /* Handle async function declarations */
    if (check(parser, TOKEN_ASYNC)) {
        if (parser->peek.type == TOKEN_FN) {
            return parse_func_def(parser);
        }
    }
    
    if (check(parser, TOKEN_CLASS)) return parse_class_def(parser);
    if (check(parser, TOKEN_STRUCT)) return parse_struct_def(parser, 0);  // 0 = not union
    if (check(parser, TOKEN_UNION)) return parse_struct_def(parser, 1);   // 1 = union
    if (check(parser, TOKEN_FN)) return parse_func_def(parser);
    if (check(parser, TOKEN_UNSAFE)) return parse_unsafe_block(parser);  /* Security: unsafe block */
    if (check(parser, TOKEN_LET)) return parse_let(parser);
    if (check(parser, TOKEN_VAR)) return parse_var(parser);
    if (check(parser, TOKEN_CONST)) return parse_const(parser);
    if (check(parser, TOKEN_IF)) return parse_if(parser);
    if (check(parser, TOKEN_SWITCH)) return parse_switch(parser);
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

        if (expr && (check(parser, TOKEN_ASSIGN) || 
                     check(parser, TOKEN_PLUS_ASSIGN) || check(parser, TOKEN_MINUS_ASSIGN) ||
                     check(parser, TOKEN_STAR_ASSIGN) || check(parser, TOKEN_SLASH_ASSIGN) ||
                     check(parser, TOKEN_PERCENT_ASSIGN))) {
            
            /* Get the operator */
            Token op_tok = advance(parser);
            int is_compound = (op_tok.type != TOKEN_ASSIGN);
            char compound_op[4] = {0};
            
            if (is_compound) {
                /* Extract the operator (e.g., "+" from "+=") */
                if (op_tok.type == TOKEN_PLUS_ASSIGN) strcpy(compound_op, "+");
                else if (op_tok.type == TOKEN_MINUS_ASSIGN) strcpy(compound_op, "-");
                else if (op_tok.type == TOKEN_STAR_ASSIGN) strcpy(compound_op, "*");
                else if (op_tok.type == TOKEN_SLASH_ASSIGN) strcpy(compound_op, "/");
                else if (op_tok.type == TOKEN_PERCENT_ASSIGN) strcpy(compound_op, "%");
            }
            token_free(&op_tok);
            
            ASTNode *rhs = parse_expression(parser);
            
            if (expr->type == NODE_IDENT) {
                char *name = strdup(expr->data.ident.name);
                ASTNode *value;
                ASTNode *n;
                
                if (is_compound) {
                    /* Transform x += y into x = x + y */
                    ASTNode *lhs_copy = ast_new_ident(name, line);
                    value = ast_new_binop(compound_op, lhs_copy, rhs, line);
                } else {
                    value = rhs;
                }
                
                ast_free(expr);
                n = ast_new_assign(name, value, line);
                free(name);
                return n;
            } else if (expr->type == NODE_MEMBER_ACCESS || expr->type == NODE_INDEX) {
                /* For member access/index assignment (e.g., this.x = value or arr[0] = value)
                 * The AST structure doesn't directly support this, so we'll handle it
                 * as a special case. The interpreter/compiler will need to recognize
                 * assignments to member access/index expressions. 
                 * 
                 * For compound assignments, we need to transform:
                 *   obj.field += value  =>  obj.field = obj.field + value
                 */
                ASTNode *value;
                
                if (is_compound) {
                    /* Create a copy of the lhs for the binary operation */
                    ASTNode *lhs_copy;
                    if (expr->type == NODE_MEMBER_ACCESS) {
                        /* Deep copy member access */
                        lhs_copy = malloc(sizeof(ASTNode));
                        memcpy(lhs_copy, expr, sizeof(ASTNode));
                        lhs_copy->data.member_access.obj = expr->data.member_access.obj;
                        lhs_copy->data.member_access.member = strdup(expr->data.member_access.member);
                    } else {
                        /* Deep copy index */
                        lhs_copy = malloc(sizeof(ASTNode));
                        memcpy(lhs_copy, expr, sizeof(ASTNode));
                        lhs_copy->data.index_expr.obj = expr->data.index_expr.obj;
                        lhs_copy->data.index_expr.index = expr->data.index_expr.index;
                    }
                    value = ast_new_binop(compound_op, lhs_copy, rhs, line);
                } else {
                    value = rhs;
                }
                
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

static ASTNode *parse_struct_def(Parser *parser, int is_union) {
    int line = parser->current.line;
    Token struct_tok = advance(parser);  // Consume struct or union keyword
    Token name_tok;
    char *struct_name;
    ASTNode *struct_node;
    token_free(&struct_tok);
    
    name_tok = consume(parser, TOKEN_IDENT);
    struct_name = strdup(name_tok.value);
    token_free(&name_tok);
    
    struct_node = ast_new_struct_def(struct_name, is_union, line);
    free(struct_name);
    
    /* Parse struct body */
    {
        Token lbrace_tok = consume(parser, TOKEN_LBRACE);
        token_free(&lbrace_tok);
    }
    
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        char *field_name;
        char *field_type = NULL;
        ASTNode *default_value = NULL;
        Token field_tok;
        
        while (match(parser, TOKEN_SEMICOLON) || match(parser, TOKEN_COMMA)) {}
        if (check(parser, TOKEN_RBRACE) || check(parser, TOKEN_EOF)) break;
        
        /* Parse field name */
        field_tok = consume(parser, TOKEN_IDENT);
        field_name = strdup(field_tok.value);
        token_free(&field_tok);
        
        /* Parse optional type annotation */
        if (match(parser, TOKEN_COLON)) {
            Token type_tok = consume(parser, TOKEN_IDENT);
            field_type = strdup(type_tok.value);
            token_free(&type_tok);
            
            /* Handle pointer types (*type) */
            while (match(parser, TOKEN_STAR)) {
                char *ptr_type = malloc(strlen(field_type) + 2);
                sprintf(ptr_type, "*%s", field_type);
                free(field_type);
                field_type = ptr_type;
            }
        }
        
        /* Parse optional default value */
        if (match(parser, TOKEN_ASSIGN)) {
            default_value = parse_expression(parser);
        }
        
        /* Add field to struct */
        ast_struct_add_field(struct_node, field_name, field_type, default_value);
        free(field_name);
        if (field_type) free(field_type);
        
        /* Allow comma or semicolon as field separator */
        if (!check(parser, TOKEN_RBRACE)) {
            if (!match(parser, TOKEN_COMMA) && !match(parser, TOKEN_SEMICOLON)) {
                /* Optional separator - allow fields without separators */
            }
        }
    }
    
    {
        Token rbrace_tok = consume(parser, TOKEN_RBRACE);
        token_free(&rbrace_tok);
    }
    
    return struct_node;
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
        int is_abstract = 0;
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
        
        /* Check for abstract keyword (for methods) */
        if (match(parser, TOKEN_ABSTRACT)) {
            is_abstract = 1;
        }
        
        /* Check for static keyword */
        if (match(parser, TOKEN_STATIC)) {
            is_static = 1;
        }
        
        /* Parse class members (let statements, function definitions, and constructors) */
        if (check(parser, TOKEN_LET)) {
            if (is_abstract) {
                fprintf(stderr, "Parse error at line %d: fields cannot be abstract\n",
                        parser->current.line);
                parser->had_error = 1;
            }
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
                member->data.func_def.is_abstract = is_abstract;
                if (is_abstract && member->data.func_def.body != NULL) {
                    fprintf(stderr, "Warning at line %d: abstract method '%s' should not have a body\n",
                            member->line, member->data.func_def.name);
                }
                if (is_abstract && member->data.func_def.body == NULL) {
                    // Mark as abstract - it has no body
                    member->data.func_def.is_abstract = 1;
                }
                nodelist_push(&class_node->data.class_def.members, member);
            }
        } else if (check(parser, TOKEN_CONSTRUCTOR)) {
            /* Parse constructor - treat it as a special function named "constructor" */
            if (is_static) {
                fprintf(stderr, "Parse error at line %d: constructors cannot be static\n",
                        parser->current.line);
                parser->had_error = 1;
            }
            if (is_abstract) {
                fprintf(stderr, "Parse error at line %d: constructors cannot be abstract\n",
                        parser->current.line);
                parser->had_error = 1;
            }
            
            /* Consume constructor keyword */
            int ctor_line = parser->current.line;
            Token ctor_tok = advance(parser);
            token_free(&ctor_tok);
            
            /* Parse parameters */
            Token lparen_tok = consume(parser, TOKEN_LPAREN);
            token_free(&lparen_tok);
            
            char **param_names = NULL;
            char **param_types = NULL;
            ASTNode **param_defaults = NULL;
            int param_count = 0;
            int param_cap = 0;
            int seen_default = 0;
            
            while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
                int is_rest = 0;
                
                /* Check for rest parameter */
                if (check(parser, TOKEN_SPREAD)) {
                    Token spread_tok = advance(parser);
                    token_free(&spread_tok);
                    is_rest = 1;
                }
                
                Token pname_tok = consume(parser, TOKEN_IDENT);
                if (param_count >= param_cap) {
                    param_cap = param_cap == 0 ? 4 : param_cap * 2;
                    param_names = realloc(param_names, param_cap * sizeof(char *));
                    param_types = realloc(param_types, param_cap * sizeof(char *));
                    param_defaults = realloc(param_defaults, param_cap * sizeof(ASTNode *));
                }
                param_names[param_count] = strdup(pname_tok.value);
                token_free(&pname_tok);
                param_types[param_count] = NULL;
                param_defaults[param_count] = NULL;
                
                if (match(parser, TOKEN_COLON)) {
                    Token ptype_tok = consume(parser, TOKEN_IDENT);
                    param_types[param_count] = strdup(ptype_tok.value);
                    token_free(&ptype_tok);
                }
                
                /* Check for default value: name = expr */
                if (match(parser, TOKEN_ASSIGN)) {
                    param_defaults[param_count] = parse_expression(parser);
                    seen_default = 1;
                } else if (seen_default && !is_rest) {
                    fprintf(stderr, "Parse error at line %d: Required parameter '%s' cannot follow parameter with default value\n", 
                            parser->current.line, param_names[param_count]);
                    parser->had_error = 1;
                }
                
                param_count++;
                
                if (is_rest) {
                    if (!check(parser, TOKEN_RPAREN)) {
                        fprintf(stderr, "Parse error at line %d: Rest parameter must be last\n", parser->current.line);
                        parser->had_error = 1;
                    }
                    break;
                }
                
                if (!match(parser, TOKEN_COMMA)) break;
            }
            
            Token rparen_tok = consume(parser, TOKEN_RPAREN);
            token_free(&rparen_tok);
            
            /* Create a function node for the constructor */
            member = ast_new_func_def("constructor", NULL, ctor_line);
            member->data.func_def.param_types = param_types;
            member->data.func_def.default_values = param_defaults;
            
            int i;
            for (i = 0; i < param_count; i++) {
                nodelist_push(&member->data.func_def.params, ast_new_ident(param_names[i], ctor_line));
                free(param_names[i]);
            }
            free(param_names);
            
            /* Parse constructor body */
            member->data.func_def.body = parse_block(parser);
            member->data.func_def.is_static = 0;
            member->data.func_def.access = access;
            member->data.func_def.is_abstract = 0;
            
            nodelist_push(&class_node->data.class_def.members, member);
        } else if (check(parser, TOKEN_IDENT) && check_ahead(parser, TOKEN_LPAREN)) {
            /* Method shorthand syntax: methodName(...) { ... } */
            if (is_abstract) {
                fprintf(stderr, "Parse error at line %d: abstract methods must use 'fn' keyword\n",
                        parser->current.line);
                parser->had_error = 1;
            }
            
            int method_line = parser->current.line;
            Token method_name_tok = advance(parser);
            char *method_name = strdup(method_name_tok.value);
            token_free(&method_name_tok);
            
            /* Parse parameters */
            Token lparen_tok = consume(parser, TOKEN_LPAREN);
            token_free(&lparen_tok);
            
            char **param_names = NULL;
            char **param_types = NULL;
            ASTNode **param_defaults = NULL;
            int param_count = 0;
            int param_cap = 0;
            int seen_default = 0;
            int has_rest_param = 0;
            
            while (!check(parser, TOKEN_RPAREN) && !check(parser, TOKEN_EOF)) {
                int is_rest = 0;
                
                /* Check for rest parameter */
                if (check(parser, TOKEN_SPREAD)) {
                    Token spread_tok = advance(parser);
                    token_free(&spread_tok);
                    is_rest = 1;
                }
                
                Token pname_tok = consume(parser, TOKEN_IDENT);
                if (param_count >= param_cap) {
                    param_cap = param_cap == 0 ? 4 : param_cap * 2;
                    param_names = realloc(param_names, param_cap * sizeof(char *));
                    param_types = realloc(param_types, param_cap * sizeof(char *));
                    param_defaults = realloc(param_defaults, param_cap * sizeof(ASTNode *));
                }
                param_names[param_count] = strdup(pname_tok.value);
                token_free(&pname_tok);
                param_types[param_count] = NULL;
                param_defaults[param_count] = NULL;
                
                if (match(parser, TOKEN_COLON)) {
                    Token ptype_tok = consume(parser, TOKEN_IDENT);
                    param_types[param_count] = strdup(ptype_tok.value);
                    token_free(&ptype_tok);
                }
                
                /* Check for default value: name = expr */
                if (match(parser, TOKEN_ASSIGN)) {
                    param_defaults[param_count] = parse_expression(parser);
                    seen_default = 1;
                } else if (seen_default && !is_rest) {
                    fprintf(stderr, "Parse error at line %d: Required parameter '%s' cannot follow parameter with default value\n", 
                            parser->current.line, param_names[param_count]);
                    parser->had_error = 1;
                }
                
                param_count++;
                
                if (is_rest) {
                    has_rest_param = 1;
                    if (!check(parser, TOKEN_RPAREN)) {
                        fprintf(stderr, "Parse error at line %d: Rest parameter must be last\n", parser->current.line);
                        parser->had_error = 1;
                    }
                    break;
                }
                
                if (!match(parser, TOKEN_COMMA)) break;
            }
            
            Token rparen_tok = consume(parser, TOKEN_RPAREN);
            token_free(&rparen_tok);
            
            /* Create a function node for the method */
            member = ast_new_func_def(method_name, NULL, method_line);
            free(method_name);
            member->data.func_def.param_types = param_types;
            member->data.func_def.default_values = param_defaults;
            member->data.func_def.has_rest_param = has_rest_param;
            
            int i;
            for (i = 0; i < param_count; i++) {
                nodelist_push(&member->data.func_def.params, ast_new_ident(param_names[i], method_line));
                free(param_names[i]);
            }
            free(param_names);
            
            /* Parse method body */
            member->data.func_def.body = parse_block(parser);
            member->data.func_def.is_static = is_static;
            member->data.func_def.access = access;
            member->data.func_def.is_abstract = 0;
            
            nodelist_push(&class_node->data.class_def.members, member);
        } else {
            fprintf(stderr, "Parse error at line %d: expected 'let', 'fn', or 'constructor' in class body, got %s\n",
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

/* Check if the next token (peek) matches a type */
static int check_ahead(Parser *parser, TokenType type) {
    return parser->peek.type == type;
}

/* Module system parsing */

/* Parse import statement:
 * import {name1, name2 as alias} from "module"
 * import defaultName from "module"
 * import * as namespace from "module"
 * import defaultName, {name1, name2} from "module"
 * import "module" as alias
 */
static ASTNode *parse_import(Parser *parser) {
    Token import_tok = consume(parser, TOKEN_IMPORT);
    int line = import_tok.line;
    token_free(&import_tok);

    /* import "module" as alias */
    if (check(parser, TOKEN_STRING)) {
        Token path_tok = consume(parser, TOKEN_STRING);
        char *module_path = strdup(path_tok.value);
        Token as_tok, alias_tok;
        ASTNode *result;
        token_free(&path_tok);
        
        as_tok = consume(parser, TOKEN_AS);
        token_free(&as_tok);
        
        alias_tok = consume(parser, TOKEN_IDENT);
        char *alias = strdup(alias_tok.value);
        token_free(&alias_tok);
        
        result = ast_new_import_namespace(alias, module_path, line);
        free(alias);
        free(module_path);
        return result;
    }

    /* import * as namespace from "module" */
    if (check(parser, TOKEN_STAR)) {
        Token star_tok = advance(parser);
        Token as_tok, namespace_tok, from_tok, path_tok;
        char *namespace, *module_path;
        ASTNode *result;
        token_free(&star_tok);
        
        as_tok = consume(parser, TOKEN_AS);
        token_free(&as_tok);
        
        namespace_tok = consume(parser, TOKEN_IDENT);
        namespace = strdup(namespace_tok.value);
        token_free(&namespace_tok);
        
        from_tok = consume(parser, TOKEN_FROM);
        token_free(&from_tok);
        
        path_tok = consume(parser, TOKEN_STRING);
        module_path = strdup(path_tok.value);
        token_free(&path_tok);
        
        result = ast_new_import_namespace(namespace, module_path, line);
        free(namespace);
        free(module_path);
        return result;
    }
    
    /* Check for default import */
    if (check(parser, TOKEN_IDENT) && !check_ahead(parser, TOKEN_COMMA) && !check_ahead(parser, TOKEN_LBRACE)) {
        Token name_tok = consume(parser, TOKEN_IDENT);
        Token from_tok, path_tok;
        char *name = strdup(name_tok.value);
        char *module_path;
        ASTNode *result;
        token_free(&name_tok);
        
        from_tok = consume(parser, TOKEN_FROM);
        token_free(&from_tok);
        
        path_tok = consume(parser, TOKEN_STRING);
        module_path = strdup(path_tok.value);
        token_free(&path_tok);
        
        result = ast_new_import_default(name, module_path, line);
        free(name);
        free(module_path);
        return result;
    }
    
    /* Named imports: import {a, b as c} from "module" */
    if (check(parser, TOKEN_LBRACE)) {
        Token brace_tok = advance(parser);
        int cap = 4;
        int count = 0;
        char **names = malloc(cap * sizeof(char*));
        char **aliases = malloc(cap * sizeof(char*));
        Token from_tok, path_tok;
        char *module_path;
        ASTNode *result;
        token_free(&brace_tok);
        
        while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
            Token name_tok = consume(parser, TOKEN_IDENT);
            char *alias = NULL;
            
            if (count >= cap) {
                cap *= 2;
                names = realloc(names, cap * sizeof(char*));
                aliases = realloc(aliases, cap * sizeof(char*));
            }
            
            names[count] = strdup(name_tok.value);
            token_free(&name_tok);
            
            /* Check for 'as' alias */
            if (match(parser, TOKEN_AS)) {
                Token alias_tok = consume(parser, TOKEN_IDENT);
                alias = strdup(alias_tok.value);
                token_free(&alias_tok);
            }
            
            aliases[count] = alias;
            count++;
            
            if (!check(parser, TOKEN_RBRACE)) {
                Token comma = consume(parser, TOKEN_COMMA);
                token_free(&comma);
            }
        }
        
        brace_tok = consume(parser, TOKEN_RBRACE);
        token_free(&brace_tok);
        
        from_tok = consume(parser, TOKEN_FROM);
        token_free(&from_tok);
        
        path_tok = consume(parser, TOKEN_STRING);
        module_path = strdup(path_tok.value);
        token_free(&path_tok);
        
        result = ast_new_import_named(names, aliases, count, module_path, line);
        free(module_path);
        return result;
    }
    
    fprintf(stderr, "Syntax error at line %d: Invalid import syntax\n", line);
    parser->had_error = 1;
    return NULL;
}

/* Parse export statement:
 * export const x = 1
 * export fn foo() {}
 * export default expression
 * export {name1, name2}
 */
static ASTNode *parse_export(Parser *parser) {
    Token export_tok = consume(parser, TOKEN_EXPORT);
    int line = export_tok.line;
    token_free(&export_tok);
    
    /* export default ... */
    if (match(parser, TOKEN_DEFAULT)) {
        /* Check if it's a function or class declaration */
        if (check(parser, TOKEN_FN) || check(parser, TOKEN_CLASS) || 
            check(parser, TOKEN_ASYNC)) {
            ASTNode *decl = parse_statement(parser);
            return ast_new_export(1, decl, NULL, 0, line);
        } else {
            /* Otherwise it's an expression */
            ASTNode *expr = parse_expression(parser);
            return ast_new_export(1, expr, NULL, 0, line);
        }
    }
    
    /* export { name1, name2 } */
    if (check(parser, TOKEN_LBRACE)) {
        Token brace_tok = advance(parser);
        int cap = 4;
        int count = 0;
        char **names = malloc(cap * sizeof(char*));
        token_free(&brace_tok);
        
        while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
            Token name_tok = consume(parser, TOKEN_IDENT);
            
            if (count >= cap) {
                cap *= 2;
                names = realloc(names, cap * sizeof(char*));
            }
            
            names[count++] = strdup(name_tok.value);
            token_free(&name_tok);
            
            if (!check(parser, TOKEN_RBRACE)) {
                Token comma = consume(parser, TOKEN_COMMA);
                token_free(&comma);
            }
        }
        
        brace_tok = consume(parser, TOKEN_RBRACE);
        token_free(&brace_tok);
        
        return ast_new_export(0, NULL, names, count, line);
    }
    
    /* export const/let/var/fn/class ... */
    ASTNode *declaration = parse_statement(parser);
    return ast_new_export(0, declaration, NULL, 0, line);
}

