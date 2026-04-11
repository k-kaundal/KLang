#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdio.h>
#include <stdlib.h>
#include "runtime/interpreter/interpreter.h"
#include "core/ast/ast.h"

/* Debugger state */
typedef enum {
    DBG_RUNNING,
    DBG_PAUSED,
    DBG_STEP_OVER,
    DBG_STEP_INTO,
    DBG_STEP_OUT,
    DBG_STOPPED
} DebuggerState;

/* Breakpoint */
typedef struct Breakpoint {
    char *file;
    int line;
    int enabled;
    int hit_count;
    struct Breakpoint *next;
} Breakpoint;

/* Call frame info */
typedef struct {
    char *function_name;
    char *file;
    int line;
    int depth;
} CallFrame;

/* Debugger context */
typedef struct {
    Interpreter *interp;
    DebuggerState state;
    Breakpoint *breakpoints;
    int breakpoint_count;
    
    /* Current execution state */
    char *current_file;
    int current_line;
    int call_depth;
    
    /* Step control */
    int step_target_depth;
    
    /* Callbacks */
    void (*on_break)(void *ctx, int line);
    void (*on_step)(void *ctx, int line);
    void *callback_context;
} Debugger;

/* Initialize debugger */
Debugger* debugger_init(Interpreter *interp);

/* Free debugger */
void debugger_free(Debugger *dbg);

/* Breakpoint management */
int debugger_add_breakpoint(Debugger *dbg, const char *file, int line);
int debugger_remove_breakpoint(Debugger *dbg, const char *file, int line);
int debugger_toggle_breakpoint(Debugger *dbg, const char *file, int line);
void debugger_list_breakpoints(Debugger *dbg);
int debugger_has_breakpoint(Debugger *dbg, const char *file, int line);

/* Execution control */
void debugger_continue(Debugger *dbg);
void debugger_step_over(Debugger *dbg);
void debugger_step_into(Debugger *dbg);
void debugger_step_out(Debugger *dbg);
void debugger_pause(Debugger *dbg);
void debugger_stop(Debugger *dbg);

/* Inspection */
void debugger_print_variables(Debugger *dbg);
Value debugger_evaluate_expression(Debugger *dbg, const char *expr);
void debugger_print_call_stack(Debugger *dbg);
void debugger_print_source_context(Debugger *dbg, int context_lines);

/* Hook for interpreter to check debugger state */
int debugger_should_break(Debugger *dbg, const char *file, int line);

/* Interactive debugger REPL */
void debugger_repl(Debugger *dbg);

#endif /* DEBUGGER_H */
