#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENT,
    TOKEN_FN, TOKEN_LET, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR,
    TOKEN_RETURN, TOKEN_IN, TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,
    TOKEN_EQ, TOKEN_NEQ, TOKEN_LT, TOKEN_GT, TOKEN_LEQ, TOKEN_GEQ,
    TOKEN_ASSIGN, TOKEN_BANG,
    TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_ARROW, TOKEN_DOTDOT,
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
