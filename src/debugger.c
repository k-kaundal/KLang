#include "debugger.h"
#include "cli_colors.h"
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/* Initialize debugger */
Debugger* debugger_init(Interpreter *interp) {
    Debugger *dbg = malloc(sizeof(Debugger));
    
    if (!dbg) {
        return NULL;
    }
    
    dbg->interp = interp;
    dbg->state = DBG_PAUSED;
    dbg->breakpoints = NULL;
    dbg->breakpoint_count = 0;
    dbg->current_file = NULL;
    dbg->current_line = 0;
    dbg->call_depth = 0;
    dbg->step_target_depth = 0;
    dbg->on_break = NULL;
    dbg->on_step = NULL;
    dbg->callback_context = NULL;
    
    return dbg;
}

/* Free debugger */
void debugger_free(Debugger *dbg) {
    if (!dbg) return;
    
    /* Free breakpoints */
    Breakpoint *bp = dbg->breakpoints;
    while (bp) {
        Breakpoint *next = bp->next;
        free(bp->file);
        free(bp);
        bp = next;
    }
    
    free(dbg->current_file);
    free(dbg);
}

/* Add breakpoint */
int debugger_add_breakpoint(Debugger *dbg, const char *file, int line) {
    /* Check if breakpoint already exists */
    if (debugger_has_breakpoint(dbg, file, line)) {
        return 0;
    }
    
    Breakpoint *bp = malloc(sizeof(Breakpoint));
    if (!bp) {
        return -1;
    }
    
    bp->file = strdup(file);
    bp->line = line;
    bp->enabled = 1;
    bp->hit_count = 0;
    bp->next = dbg->breakpoints;
    
    dbg->breakpoints = bp;
    dbg->breakpoint_count++;
    
    print_success("Breakpoint added:");
    printf(" %s:%d\n", file, line);
    
    return 0;
}

/* Remove breakpoint */
int debugger_remove_breakpoint(Debugger *dbg, const char *file, int line) {
    Breakpoint **bp_ptr = &dbg->breakpoints;
    
    while (*bp_ptr) {
        Breakpoint *bp = *bp_ptr;
        
        if (strcmp(bp->file, file) == 0 && bp->line == line) {
            *bp_ptr = bp->next;
            free(bp->file);
            free(bp);
            dbg->breakpoint_count--;
            
            print_success("Breakpoint removed:");
            printf(" %s:%d\n", file, line);
            return 0;
        }
        
        bp_ptr = &bp->next;
    }
    
    return -1;
}

/* Toggle breakpoint */
int debugger_toggle_breakpoint(Debugger *dbg, const char *file, int line) {
    if (debugger_has_breakpoint(dbg, file, line)) {
        return debugger_remove_breakpoint(dbg, file, line);
    } else {
        return debugger_add_breakpoint(dbg, file, line);
    }
}

/* List breakpoints */
void debugger_list_breakpoints(Debugger *dbg) {
    if (dbg->breakpoint_count == 0) {
        print_info("No breakpoints set");
        return;
    }
    
    printf("\n");
    print_info("Breakpoints:\n");
    
    int i = 1;
    Breakpoint *bp = dbg->breakpoints;
    while (bp) {
        printf("  %d. %s%s:%d%s", i,
               get_color(COLOR_CYAN),
               bp->file, bp->line,
               get_color(COLOR_RESET));
        
        if (!bp->enabled) {
            printf(" (disabled)");
        }
        
        if (bp->hit_count > 0) {
            printf(" [hit %d times]", bp->hit_count);
        }
        
        printf("\n");
        
        bp = bp->next;
        i++;
    }
    
    printf("\n");
}

/* Check if breakpoint exists */
int debugger_has_breakpoint(Debugger *dbg, const char *file, int line) {
    Breakpoint *bp = dbg->breakpoints;
    
    while (bp) {
        if (strcmp(bp->file, file) == 0 && bp->line == line && bp->enabled) {
            return 1;
        }
        bp = bp->next;
    }
    
    return 0;
}

/* Continue execution */
void debugger_continue(Debugger *dbg) {
    dbg->state = DBG_RUNNING;
    print_info("Continuing execution...");
}

/* Step over */
void debugger_step_over(Debugger *dbg) {
    dbg->state = DBG_STEP_OVER;
    dbg->step_target_depth = dbg->call_depth;
}

/* Step into */
void debugger_step_into(Debugger *dbg) {
    dbg->state = DBG_STEP_INTO;
}

/* Step out */
void debugger_step_out(Debugger *dbg) {
    dbg->state = DBG_STEP_OUT;
    dbg->step_target_depth = dbg->call_depth - 1;
}

/* Pause execution */
void debugger_pause(Debugger *dbg) {
    dbg->state = DBG_PAUSED;
}

/* Stop execution */
void debugger_stop(Debugger *dbg) {
    dbg->state = DBG_STOPPED;
    print_info("Debugging stopped");
}

/* Check if debugger should break */
int debugger_should_break(Debugger *dbg, const char *file, int line) {
    if (!dbg || dbg->state == DBG_STOPPED) {
        return 0;
    }
    
    /* Update current location */
    if (dbg->current_file) {
        free(dbg->current_file);
    }
    dbg->current_file = strdup(file);
    dbg->current_line = line;
    
    /* Check breakpoints */
    if (debugger_has_breakpoint(dbg, file, line)) {
        /* Increment hit count */
        Breakpoint *bp = dbg->breakpoints;
        while (bp) {
            if (strcmp(bp->file, file) == 0 && bp->line == line) {
                bp->hit_count++;
                break;
            }
            bp = bp->next;
        }
        
        print_warning("Breakpoint hit:");
        printf(" %s:%d\n", file, line);
        dbg->state = DBG_PAUSED;
        
        if (dbg->on_break) {
            dbg->on_break(dbg->callback_context, line);
        }
        
        return 1;
    }
    
    /* Check step modes */
    if (dbg->state == DBG_STEP_INTO) {
        dbg->state = DBG_PAUSED;
        return 1;
    }
    
    if (dbg->state == DBG_STEP_OVER && dbg->call_depth <= dbg->step_target_depth) {
        dbg->state = DBG_PAUSED;
        return 1;
    }
    
    if (dbg->state == DBG_STEP_OUT && dbg->call_depth <= dbg->step_target_depth) {
        dbg->state = DBG_PAUSED;
        return 1;
    }
    
    return 0;
}

/* Print variables in current scope */
void debugger_print_variables(Debugger *dbg) {
    if (!dbg || !dbg->interp) {
        return;
    }
    
    printf("\n");
    print_info("Local variables:\n");
    
    /* In a full implementation, would walk the interpreter's scope chain */
    printf("  (Variable inspection not yet fully implemented)\n");
    printf("\n");
}

/* Evaluate expression */
Value debugger_evaluate_expression(Debugger *dbg, const char *expr) {
    Value result;
    result.type = VAL_NULL;
    
    if (!dbg || !dbg->interp) {
        return result;
    }
    
    /* In a full implementation, would parse and evaluate the expression */
    (void)expr;  /* Parameter not used in stub implementation */
    print_warning("Expression evaluation not yet fully implemented");
    
    return result;
}

/* Print call stack */
void debugger_print_call_stack(Debugger *dbg) {
    if (!dbg) {
        return;
    }
    
    printf("\n");
    print_info("Call stack:\n");
    printf("  Depth: %d\n", dbg->call_depth);
    
    if (dbg->current_file) {
        printf("  Current: %s:%d\n", dbg->current_file, dbg->current_line);
    }
    
    printf("\n");
}

/* Print source context */
void debugger_print_source_context(Debugger *dbg, int context_lines) {
    if (!dbg || !dbg->current_file) {
        return;
    }
    
    FILE *f = fopen(dbg->current_file, "r");
    if (!f) {
        print_error("Cannot open source file");
        return;
    }
    
    printf("\n");
    print_info("Source context:\n");
    
    char line[512];
    int line_num = 1;
    int start_line = dbg->current_line - context_lines;
    int end_line = dbg->current_line + context_lines;
    
    if (start_line < 1) start_line = 1;
    
    while (fgets(line, sizeof(line), f)) {
        if (line_num >= start_line && line_num <= end_line) {
            if (line_num == dbg->current_line) {
                printf(" %s->%s %3d | %s",
                       get_color(COLOR_BOLD_GREEN),
                       get_color(COLOR_RESET),
                       line_num, line);
            } else {
                printf("   %3d | %s", line_num, line);
            }
        }
        
        if (line_num > end_line) {
            break;
        }
        
        line_num++;
    }
    
    printf("\n");
    fclose(f);
}

/* Interactive debugger REPL */
void debugger_repl(Debugger *dbg) {
    char *input;
    
    printf("\n%sKLang Debugger%s\n", 
           get_color(COLOR_BOLD_CYAN),
           get_color(COLOR_RESET));
    printf("Type 'help' for available commands\n\n");
    
    while (dbg->state != DBG_STOPPED) {
        /* Show current location */
        if (dbg->current_file) {
            printf("%s[%s:%d]%s ",
                   get_color(COLOR_CYAN),
                   dbg->current_file, dbg->current_line,
                   get_color(COLOR_RESET));
        }
        
        input = readline("(dbg) ");
        
        if (!input) {
            break;
        }
        
        if (strlen(input) > 0) {
            add_history(input);
        }
        
        /* Parse command */
        if (strcmp(input, "help") == 0 || strcmp(input, "h") == 0) {
            printf("\nDebugger commands:\n");
            printf("  continue, c     - Continue execution\n");
            printf("  step, s         - Step into\n");
            printf("  next, n         - Step over\n");
            printf("  finish, f       - Step out\n");
            printf("  break <line>    - Set breakpoint\n");
            printf("  list, l         - List breakpoints\n");
            printf("  delete <line>   - Delete breakpoint\n");
            printf("  print, p        - Print variables\n");
            printf("  where, w        - Show call stack\n");
            printf("  source          - Show source context\n");
            printf("  quit, q         - Stop debugging\n");
            printf("  help, h         - Show this help\n\n");
        }
        else if (strcmp(input, "continue") == 0 || strcmp(input, "c") == 0) {
            debugger_continue(dbg);
            free(input);
            break;
        }
        else if (strcmp(input, "step") == 0 || strcmp(input, "s") == 0) {
            debugger_step_into(dbg);
            free(input);
            break;
        }
        else if (strcmp(input, "next") == 0 || strcmp(input, "n") == 0) {
            debugger_step_over(dbg);
            free(input);
            break;
        }
        else if (strcmp(input, "finish") == 0 || strcmp(input, "f") == 0) {
            debugger_step_out(dbg);
            free(input);
            break;
        }
        else if (strncmp(input, "break ", 6) == 0) {
            int line = atoi(input + 6);
            if (line > 0 && dbg->current_file) {
                debugger_add_breakpoint(dbg, dbg->current_file, line);
            } else {
                print_error("Invalid line number");
            }
        }
        else if (strcmp(input, "list") == 0 || strcmp(input, "l") == 0) {
            debugger_list_breakpoints(dbg);
        }
        else if (strncmp(input, "delete ", 7) == 0) {
            int line = atoi(input + 7);
            if (line > 0 && dbg->current_file) {
                debugger_remove_breakpoint(dbg, dbg->current_file, line);
            } else {
                print_error("Invalid line number");
            }
        }
        else if (strcmp(input, "print") == 0 || strcmp(input, "p") == 0) {
            debugger_print_variables(dbg);
        }
        else if (strcmp(input, "where") == 0 || strcmp(input, "w") == 0) {
            debugger_print_call_stack(dbg);
        }
        else if (strcmp(input, "source") == 0) {
            debugger_print_source_context(dbg, 5);
        }
        else if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
            debugger_stop(dbg);
            free(input);
            break;
        }
        else if (strlen(input) > 0) {
            print_error("Unknown command. Type 'help' for available commands.");
        }
        
        free(input);
    }
}
