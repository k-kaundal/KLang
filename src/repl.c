#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "runtime.h"

void run_repl(void) {
    Interpreter *interp = interpreter_new();
    runtime_init(interp);

    printf("KLang REPL v0.1.0\n");
    printf("Type 'exit' to quit.\n");

    {
        char line[4096];
        while (1) {
            Lexer lexer;
            Parser parser;
            int count = 0;
            ASTNode **nodes;
            int i;

            printf("> ");
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            {
                int len = (int)strlen(line);
                if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
            }
            if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) break;
            if (line[0] == '\0') continue;

            lexer_init(&lexer, line);
            parser_init(&parser, &lexer);

            nodes = parse_program(&parser, &count);

            for (i = 0; i < count; i++) {
                Value result = eval_node(interp, nodes[i]);
                if (result.type != VAL_NULL) {
                    value_print(&result);
                    printf("\n");
                }
                value_free(&result);
                ast_free(nodes[i]);
            }
            free(nodes);
            parser_free(&parser);
            lexer_free(&lexer);
        }
    }

    interpreter_free(interp);
}
