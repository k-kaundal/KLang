#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->col = 1;
    lexer->has_peeked = 0;
    lexer->peeked.value = NULL;
}

void token_free(Token *tok) {
    if (tok && tok->value) {
        free(tok->value);
        tok->value = NULL;
    }
}

void lexer_free(Lexer *lexer) {
    if (lexer->has_peeked) {
        token_free(&lexer->peeked);
        lexer->has_peeked = 0;
    }
}

static Token make_token(TokenType type, const char *value, int line, int col) {
    Token t;
    t.type = type;
    t.value = value ? strdup(value) : strdup("");
    t.line = line;
    t.col = col;
    return t;
}

static void skip_whitespace_and_comments(Lexer *lexer) {
    while (lexer->source[lexer->pos]) {
        char c = lexer->source[lexer->pos];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if (c == '\n') { lexer->line++; lexer->col = 1; }
            else { lexer->col++; }
            lexer->pos++;
        } else if (c == '#') {
            /* Single-line comment with # */
            while (lexer->source[lexer->pos] && lexer->source[lexer->pos] != '\n')
                lexer->pos++;
        } else if (c == '/' && lexer->source[lexer->pos + 1] == '/') {
            /* Single-line comment with // or doc comment /// */
            lexer->pos += 2; lexer->col += 2;
            /* Check if it's a doc comment (///) */
            int is_doc_comment = (lexer->source[lexer->pos] == '/');
            if (is_doc_comment) {
                lexer->pos++; lexer->col++;
            }
            /* Skip to end of line */
            while (lexer->source[lexer->pos] && lexer->source[lexer->pos] != '\n') {
                lexer->pos++;
            }
        } else if (c == '/' && lexer->source[lexer->pos + 1] == '*') {
            /* Multi-line comment */
            lexer->pos += 2; lexer->col += 2;
            while (lexer->source[lexer->pos]) {
                if (lexer->source[lexer->pos] == '*' && lexer->source[lexer->pos + 1] == '/') {
                    lexer->pos += 2; lexer->col += 2;
                    break;
                }
                if (lexer->source[lexer->pos] == '\n') {
                    lexer->line++; lexer->col = 1;
                } else {
                    lexer->col++;
                }
                lexer->pos++;
            }
        } else {
            break;
        }
    }
}

Token lexer_next_token(Lexer *lexer) {
    if (lexer->has_peeked) {
        lexer->has_peeked = 0;
        return lexer->peeked;
    }

    skip_whitespace_and_comments(lexer);

    if (!lexer->source[lexer->pos])
        return make_token(TOKEN_EOF, "", lexer->line, lexer->col);

    int line = lexer->line;
    int col = lexer->col;
    char c = lexer->source[lexer->pos];

    /* Numbers */
    if (isdigit((unsigned char)c)) {
        int start = lexer->pos;
        while (isdigit((unsigned char)lexer->source[lexer->pos])) { lexer->pos++; lexer->col++; }
        if (lexer->source[lexer->pos] == '.' && lexer->source[lexer->pos+1] != '.') {
            lexer->pos++; lexer->col++;
            while (isdigit((unsigned char)lexer->source[lexer->pos])) { lexer->pos++; lexer->col++; }
        }
        {
            int len = lexer->pos - start;
            char *buf = malloc(len + 1);
            Token t;
            memcpy(buf, lexer->source + start, len);
            buf[len] = '\0';
            t.type = TOKEN_NUMBER;
            t.value = buf;
            t.line = line;
            t.col = col;
            return t;
        }
    }

    /* Strings - both double and single quotes */
    if (c == '"' || c == '\'') {
        int cap = 64, len = 0;
        char *buf = malloc(cap);
        Token t;
        char quote_char = c;  /* Remember which quote type we're using */
        lexer->pos++; lexer->col++;
        while (lexer->source[lexer->pos] && lexer->source[lexer->pos] != quote_char) {
            char ch = lexer->source[lexer->pos];
            if (ch == '\\') {
                lexer->pos++; lexer->col++;
                ch = lexer->source[lexer->pos];
                if (ch == 'n') ch = '\n';
                else if (ch == 't') ch = '\t';
                else if (ch == 'r') ch = '\r';
                else if (ch == '\\') ch = '\\';
                else if (ch == '"') ch = '"';
                else if (ch == '\'') ch = '\'';
            }
            if (len + 1 >= cap) { cap *= 2; buf = realloc(buf, cap); }
            buf[len++] = ch;
            lexer->pos++; lexer->col++;
        }
        if (lexer->source[lexer->pos] == quote_char) { lexer->pos++; lexer->col++; }
        buf[len] = '\0';
        t.type = TOKEN_STRING;
        t.value = buf;
        t.line = line;
        t.col = col;
        return t;
    }

    /* Template literals with backticks */
    if (c == '`') {
        int cap = 256, len = 0;
        char *buf = malloc(cap);
        Token t;
        lexer->pos++; lexer->col++;
        
        while (lexer->source[lexer->pos] && lexer->source[lexer->pos] != '`') {
            char ch = lexer->source[lexer->pos];
            
            /* Handle newlines in template literals */
            if (ch == '\n') {
                lexer->line++;
                lexer->col = 1;
            } else {
                lexer->col++;
            }
            
            /* Handle escape sequences */
            if (ch == '\\' && lexer->source[lexer->pos + 1] == '`') {
                /* Escaped backtick */
                lexer->pos++;
                ch = '`';
            } else if (ch == '\\' && lexer->source[lexer->pos + 1] == '\\') {
                /* Escaped backslash */
                lexer->pos++;
                ch = '\\';
            }
            
            /* Store the character */
            if (len + 1 >= cap) { 
                cap *= 2; 
                buf = realloc(buf, cap); 
            }
            buf[len++] = ch;
            lexer->pos++;
        }
        
        if (lexer->source[lexer->pos] == '`') { 
            lexer->pos++; 
            lexer->col++; 
        }
        buf[len] = '\0';
        t.type = TOKEN_TEMPLATE_LITERAL;
        t.value = buf;
        t.line = line;
        t.col = col;
        return t;
    }

    /* Identifiers and keywords */
    if (isalpha((unsigned char)c) || c == '_') {
        int start = lexer->pos;
        while (isalnum((unsigned char)lexer->source[lexer->pos]) || lexer->source[lexer->pos] == '_') {
            lexer->pos++; lexer->col++;
        }
        {
            int len = lexer->pos - start;
            char *buf = malloc(len + 1);
            TokenType type = TOKEN_IDENT;
            Token t;
            memcpy(buf, lexer->source + start, len);
            buf[len] = '\0';
            if (strcmp(buf, "fn") == 0) type = TOKEN_FN;
            else if (strcmp(buf, "let") == 0) type = TOKEN_LET;
            else if (strcmp(buf, "var") == 0) type = TOKEN_VAR;
            else if (strcmp(buf, "const") == 0) type = TOKEN_CONST;
            else if (strcmp(buf, "if") == 0) type = TOKEN_IF;
            else if (strcmp(buf, "else") == 0) type = TOKEN_ELSE;
            else if (strcmp(buf, "while") == 0) type = TOKEN_WHILE;
            else if (strcmp(buf, "for") == 0) type = TOKEN_FOR;
            else if (strcmp(buf, "return") == 0) type = TOKEN_RETURN;
            else if (strcmp(buf, "break") == 0) type = TOKEN_BREAK;
            else if (strcmp(buf, "continue") == 0) type = TOKEN_CONTINUE;
            else if (strcmp(buf, "in") == 0) type = TOKEN_IN;
            else if (strcmp(buf, "true") == 0) type = TOKEN_TRUE;
            else if (strcmp(buf, "false") == 0) type = TOKEN_FALSE;
            else if (strcmp(buf, "class") == 0) type = TOKEN_CLASS;
            else if (strcmp(buf, "new") == 0) type = TOKEN_NEW;
            else if (strcmp(buf, "this") == 0) type = TOKEN_THIS;
            else if (strcmp(buf, "extends") == 0) type = TOKEN_EXTENDS;
            else if (strcmp(buf, "super") == 0) type = TOKEN_SUPER;
            else if (strcmp(buf, "static") == 0) type = TOKEN_STATIC;
            else if (strcmp(buf, "public") == 0) type = TOKEN_PUBLIC;
            else if (strcmp(buf, "private") == 0) type = TOKEN_PRIVATE;
            else if (strcmp(buf, "protected") == 0) type = TOKEN_PROTECTED;
            else if (strcmp(buf, "abstract") == 0) type = TOKEN_ABSTRACT;
            t.type = type;
            t.value = buf;
            t.line = line;
            t.col = col;
            return t;
        }
    }

    lexer->pos++; lexer->col++;

    switch (c) {
        case '+': return make_token(TOKEN_PLUS, "+", line, col);
        case '-':
            if (lexer->source[lexer->pos] == '>') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_ARROW, "->", line, col);
            }
            return make_token(TOKEN_MINUS, "-", line, col);
        case '*': return make_token(TOKEN_STAR, "*", line, col);
        case '/': return make_token(TOKEN_SLASH, "/", line, col);
        case '%': return make_token(TOKEN_PERCENT, "%", line, col);
        case '=':
            if (lexer->source[lexer->pos] == '=') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_EQ, "==", line, col);
            }
            if (lexer->source[lexer->pos] == '>') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_FAT_ARROW, "=>", line, col);
            }
            return make_token(TOKEN_ASSIGN, "=", line, col);
        case '!':
            if (lexer->source[lexer->pos] == '=') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_NEQ, "!=", line, col);
            }
            return make_token(TOKEN_BANG, "!", line, col);
        case '<':
            if (lexer->source[lexer->pos] == '=') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_LEQ, "<=", line, col);
            }
            return make_token(TOKEN_LT, "<", line, col);
        case '>':
            if (lexer->source[lexer->pos] == '=') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_GEQ, ">=", line, col);
            }
            return make_token(TOKEN_GT, ">", line, col);
        case '{': return make_token(TOKEN_LBRACE, "{", line, col);
        case '}': return make_token(TOKEN_RBRACE, "}", line, col);
        case '(': return make_token(TOKEN_LPAREN, "(", line, col);
        case ')': return make_token(TOKEN_RPAREN, ")", line, col);
        case '[': return make_token(TOKEN_LBRACKET, "[", line, col);
        case ']': return make_token(TOKEN_RBRACKET, "]", line, col);
        case ':': return make_token(TOKEN_COLON, ":", line, col);
        case ';': return make_token(TOKEN_SEMICOLON, ";", line, col);
        case ',': return make_token(TOKEN_COMMA, ",", line, col);
        case '.':
            if (lexer->source[lexer->pos] == '.') {
                lexer->pos++; lexer->col++;
                return make_token(TOKEN_DOTDOT, "..", line, col);
            }
            return make_token(TOKEN_DOT, ".", line, col);
        case '?': return make_token(TOKEN_QUESTION, "?", line, col);
        default: {
            char buf[2] = {c, 0};
            return make_token(TOKEN_EOF, buf, line, col);
        }
    }
}

Token lexer_peek_token(Lexer *lexer) {
    if (!lexer->has_peeked) {
        lexer->peeked = lexer_next_token(lexer);
        lexer->has_peeked = 1;
    }
    return lexer->peeked;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_TEMPLATE_LITERAL: return "TEMPLATE_LITERAL";
        case TOKEN_IDENT: return "IDENT";
        case TOKEN_FN: return "FN";
        case TOKEN_LET: return "LET";
        case TOKEN_VAR: return "VAR";
        case TOKEN_CONST: return "CONST";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_IN: return "IN";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_CLASS: return "CLASS";
        case TOKEN_NEW: return "NEW";
        case TOKEN_THIS: return "THIS";
        case TOKEN_EXTENDS: return "EXTENDS";
        case TOKEN_SUPER: return "SUPER";
        case TOKEN_STATIC: return "STATIC";
        case TOKEN_PUBLIC: return "PUBLIC";
        case TOKEN_PRIVATE: return "PRIVATE";
        case TOKEN_PROTECTED: return "PROTECTED";
        case TOKEN_ABSTRACT: return "ABSTRACT";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PERCENT: return "PERCENT";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NEQ: return "NEQ";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LEQ: return "LEQ";
        case TOKEN_GEQ: return "GEQ";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_BANG: return "BANG";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COLON: return "COLON";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_FAT_ARROW: return "FAT_ARROW";
        case TOKEN_DOTDOT: return "DOTDOT";
        case TOKEN_DOT: return "DOT";
        case TOKEN_QUESTION: return "QUESTION";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
