#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_TEMPLATE_LITERAL, TOKEN_IDENT,
    TOKEN_FN, TOKEN_LET, TOKEN_VAR, TOKEN_CONST, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR,
    TOKEN_RETURN, TOKEN_BREAK, TOKEN_CONTINUE, TOKEN_IN, TOKEN_OF, TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_SWITCH, TOKEN_CASE, TOKEN_DEFAULT,
    TOKEN_CLASS, TOKEN_NEW, TOKEN_THIS, TOKEN_EXTENDS, TOKEN_SUPER, TOKEN_STATIC,
    TOKEN_PUBLIC, TOKEN_PRIVATE, TOKEN_PROTECTED, TOKEN_ABSTRACT,
    TOKEN_ASYNC, TOKEN_AWAIT, TOKEN_YIELD,
    TOKEN_TRY, TOKEN_CATCH, TOKEN_FINALLY, TOKEN_THROW,
    TOKEN_IMPORT, TOKEN_EXPORT, TOKEN_FROM, TOKEN_AS,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,
    TOKEN_EQ, TOKEN_NEQ, TOKEN_LT, TOKEN_GT, TOKEN_LEQ, TOKEN_GEQ,
    TOKEN_ASSIGN, TOKEN_BANG,
    TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_ARROW, TOKEN_FAT_ARROW, TOKEN_SPREAD, TOKEN_DOTDOT, TOKEN_DOT,
    TOKEN_QUESTION,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *value;
    int line;
    int col;
} Token;

typedef struct {
    const char *source;
    int pos;
    int line;
    int col;
    Token peeked;
    int has_peeked;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token lexer_next_token(Lexer *lexer);
Token lexer_peek_token(Lexer *lexer);
void lexer_free(Lexer *lexer);
void token_free(Token *tok);
const char *token_type_name(TokenType type);

#endif
