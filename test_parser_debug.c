#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main() {
    const char *src = "fn* test(x) {\n    yield\n}\n";
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    
    printf("Parsing: %s\n", src);
    
    lexer_init(&lexer, src);
    parser_init(&parser, &lexer);
    
    nodes = parse_program(&parser, &count);
    
    printf("Parse result: had_error=%d, count=%d\n", parser.had_error, count);
    
    if (nodes) {
        int i;
        for (i = 0; i < count; i++) {
            if (nodes[i]) {
                printf("Node %d: type=%d\n", i, nodes[i]->type);
                ast_free(nodes[i]);
            }
        }
        free(nodes);
    }
    
    parser_free(&parser);
    lexer_free(&lexer);
    return 0;
}
