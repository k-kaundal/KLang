#include <stdio.h>
#include "lexer.h"

int main() {
    const char *src = "fn* test(x) {\n    yield\n}\n";
    Lexer lexer;
    lexer_init(&lexer, src);
    
    Token tok;
    int count = 0;
    while ((tok = lexer_next_token(&lexer)).type != TOKEN_EOF && count < 20) {
        printf("Token %d: type=%d (%s), value='%s'\n", count, tok.type, token_type_name(tok.type), tok.value);
        token_free(&tok);
        count++;
    }
    
    lexer_free(&lexer);
    return 0;
}
