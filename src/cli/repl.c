#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "core/lexer/lexer.h"
#include "core/parser/parser.h"
#include "interpreter.h"
#include "runtime/runtime.h"
#include "cli_colors.h"

static Value builtin_exit(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;
    (void)argc;
    /* Exit code will be 0 unless specified */
    int code = 0;
    if (argc > 0 && args[0].type == VAL_INT) {
        code = (int)args[0].as.int_val;
    }
    exit(code);
    return make_null(); /* Never reached */
}

void run_repl(void) {
    Interpreter *interp = interpreter_new();
    runtime_init(interp);
    
    /* Add exit and quit as built-in functions for REPL */
    Value exit_fn;
    exit_fn.type = VAL_BUILTIN;
    exit_fn.as.builtin = builtin_exit;
    env_set_local(interp->global_env, "exit", exit_fn);
    env_set_local(interp->global_env, "quit", exit_fn);

    printf("\n");
    printf("%s╔═══════════════════════════════════════════════════════════╗%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s║              %sKLang REPL v0.3.0%s                       ║%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_BOLD_WHITE),
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s║        Interactive Programming Environment              ║%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s║        Now with readline support!                       ║%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("%s╚═══════════════════════════════════════════════════════════╝%s\n",
           get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    printf("\n");
    printf("%sFeatures:%s\n", get_color(COLOR_BOLD_YELLOW), get_color(COLOR_RESET));
    printf("  • Command history (↑/↓ arrows)\n");
    printf("  • Line editing (←/→/Home/End)\n");
    printf("  • Type %sexit%s or %squit%s to leave, or press Ctrl+D\n",
           get_color(COLOR_CYAN), get_color(COLOR_RESET),
           get_color(COLOR_CYAN), get_color(COLOR_RESET));
    printf("\n");

    char *line;
    while (1) {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;

        // Use readline for better input handling
        char prompt[256];
        snprintf(prompt, sizeof(prompt), "%s>%s ", 
                get_color(COLOR_BOLD_GREEN), get_color(COLOR_RESET));
        
        line = readline(prompt);
        
        // Check for EOF (Ctrl+D)
        if (!line) {
            printf("\n");
            break;
        }
        
        // Skip empty lines
        if (line[0] == '\0') {
            free(line);
            continue;
        }
        
        // Add to history if not empty
        add_history(line);
        
        // Check for exit commands
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            free(line);
            break;
        }

        lexer_init(&lexer, line);
        parser_init(&parser, &lexer);

        nodes = parse_program(&parser, &count);

        for (i = 0; i < count; i++) {
            Value result = eval_node(interp, nodes[i]);
            if (result.type != VAL_NULL) {
                printf("%s=> %s", get_color(COLOR_CYAN), get_color(COLOR_RESET));
                value_print(&result);
                printf("\n");
            }
            value_free(&result);
            ast_free(nodes[i]);
        }
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
        
        // Free readline allocated memory
        free(line);
    }

    printf("\n%sGoodbye!%s\n", get_color(COLOR_BOLD_CYAN), get_color(COLOR_RESET));
    interpreter_free(interp);
}
