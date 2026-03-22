#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include <stdio.h>

int main() {
    const char *code = "let x = (1, 2, 3)";
    Lexer lexer;
    lexer_init(&lexer, code);
    
    Parser parser;
    parser_init(&parser, &lexer);
    
    int count = 0;
    ASTNode **ast_nodes = parse_program(&parser, &count);
    
    if (parser.had_error) {
        printf("Parser had error\n");
        return 1;
    }
    
    printf("Parsed successfully! Count: %d\n", count);
    
    if (count > 0 && ast_nodes[0]) {
        ASTNode *stmt = ast_nodes[0];
        printf("First statement type: %d\n", stmt->type);
        
        if (stmt->type == NODE_LET && stmt->data.let_stmt.value) {
            printf("Value type: %d (NODE_TUPLE=%d)\n", stmt->data.let_stmt.value->type, NODE_TUPLE);
        }
        
        // Free nodes
        for (int i = 0; i < count; i++) {
            ast_free(ast_nodes[i]);
        }
        free(ast_nodes);
    }
    
    printf("Freed successfully!\n");
    
    return 0;
}
