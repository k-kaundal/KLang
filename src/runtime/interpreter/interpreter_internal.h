#ifndef INTERPRETER_INTERNAL_H
#define INTERPRETER_INTERNAL_H

#include "../../interpreter.h"
#include "../../lexer.h"
#include "../../parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <libgen.h>
#include <unistd.h>
#include <limits.h>

/* Debug flag for tracking object reference counting */
#define DEBUG_REFCOUNT 0

#if DEBUG_REFCOUNT
#define REFCOUNT_LOG(fmt, ...) fprintf(stderr, "[REFCOUNT] " fmt "\n", ##__VA_ARGS__)
#else
#define REFCOUNT_LOG(fmt, ...) ((void)0)
#endif

/* Forward declarations */
extern Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc);

/* Core evaluation functions (in evaluator.c) */
Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env);
Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
Value eval_node(Interpreter *interp, ASTNode *node);

/* Expression evaluation functions (in expressions.c) */
Value eval_number(Interpreter *interp, ASTNode *node, Env *env);
Value eval_string(Interpreter *interp, ASTNode *node, Env *env);
Value eval_bool(Interpreter *interp, ASTNode *node, Env *env);
Value eval_null(Interpreter *interp, ASTNode *node, Env *env);
Value eval_ident(Interpreter *interp, ASTNode *node, Env *env);
Value eval_binop(Interpreter *interp, ASTNode *node, Env *env);
Value eval_unop(Interpreter *interp, ASTNode *node, Env *env);
Value eval_list(Interpreter *interp, ASTNode *node, Env *env);
Value eval_tuple(Interpreter *interp, ASTNode *node, Env *env);
Value eval_object(Interpreter *interp, ASTNode *node, Env *env);
Value eval_index(Interpreter *interp, ASTNode *node, Env *env);
Value eval_call(Interpreter *interp, ASTNode *node, Env *env);
Value eval_member_access(Interpreter *interp, ASTNode *node, Env *env);
Value eval_this(Interpreter *interp, ASTNode *node, Env *env);
Value eval_super(Interpreter *interp, ASTNode *node, Env *env);
Value eval_template_literal(Interpreter *interp, ASTNode *node, Env *env);
Value eval_ternary(Interpreter *interp, ASTNode *node, Env *env);
Value eval_await(Interpreter *interp, ASTNode *node, Env *env);
Value eval_yield(Interpreter *interp, ASTNode *node, Env *env);
Value eval_postfix(Interpreter *interp, ASTNode *node, Env *env);
Value eval_optional_chain(Interpreter *interp, ASTNode *node, Env *env);
Value eval_nullish_coalesce(Interpreter *interp, ASTNode *node, Env *env);
Value eval_address_of(Interpreter *interp, ASTNode *node, Env *env);
Value eval_dereference(Interpreter *interp, ASTNode *node, Env *env);
Value eval_pointer_member(Interpreter *interp, ASTNode *node, Env *env);
Value eval_struct_literal(Interpreter *interp, ASTNode *node, Env *env);
Value eval_new(Interpreter *interp, ASTNode *node, Env *env);

/* Statement execution functions (in statements.c) */
Value eval_let(Interpreter *interp, ASTNode *node, Env *env);
Value eval_destructure_array(Interpreter *interp, ASTNode *node, Env *env);
Value eval_destructure_object(Interpreter *interp, ASTNode *node, Env *env);
Value eval_assign(Interpreter *interp, ASTNode *node, Env *env);
Value eval_block_stmt(Interpreter *interp, ASTNode *node, Env *env);
Value eval_func_def(Interpreter *interp, ASTNode *node, Env *env);
Value eval_return(Interpreter *interp, ASTNode *node, Env *env);
Value eval_break(Interpreter *interp, ASTNode *node, Env *env);
Value eval_continue(Interpreter *interp, ASTNode *node, Env *env);
Value eval_if(Interpreter *interp, ASTNode *node, Env *env);
Value eval_while(Interpreter *interp, ASTNode *node, Env *env);
Value eval_for(Interpreter *interp, ASTNode *node, Env *env);
Value eval_for_of(Interpreter *interp, ASTNode *node, Env *env);
Value eval_for_c_style(Interpreter *interp, ASTNode *node, Env *env);
Value eval_class_def(Interpreter *interp, ASTNode *node, Env *env);
Value eval_import_named(Interpreter *interp, ASTNode *node, Env *env);
Value eval_import_default(Interpreter *interp, ASTNode *node, Env *env);
Value eval_import_namespace(Interpreter *interp, ASTNode *node, Env *env);
Value eval_export(Interpreter *interp, ASTNode *node, Env *env);
Value eval_struct_def(Interpreter *interp, ASTNode *node, Env *env);

/* Environment management functions (in interpreter.c) */
Env *env_new(Env *parent);
void env_free(Env *env);
void env_retain(Env *env);
void env_release(Env *env);
Value *env_get(Env *env, const char *name);
EnvEntry *env_get_entry(Env *env, const char *name);
int can_access_member(AccessModifier access, int is_same_class, int is_subclass);
void env_set_local(Env *env, const char *name, Value val);
void env_set_local_with_access(Env *env, const char *name, Value val, AccessModifier access);
void env_set(Env *env, const char *name, Value val);
int env_has_local(Env *env, const char *name);
void env_declare(Env *env, const char *name, Value val, DeclType decl_type, int line, Interpreter *interp);

/* Value creation functions (in interpreter.c) */
Value make_int(long long v);
Value make_float(double v);
Value make_string(const char *s);
Value make_bool(int v);
Value make_null(void);
Value make_class(const char *name, const char *parent_name);
Value make_object(const char *class_name, Env *methods);
Value make_method(Value receiver, Value method);
Value make_promise(void);
Value make_module(const char *module_path, Env *exports, Env *module_env);
Value make_dict(void);
Value make_set(void);
Value make_pointer(void *ptr, const char *type_name);
Value make_struct(const char *struct_name, int is_union);
Value make_generator(FunctionVal *func, Env *env);

/* Value utility functions (in interpreter.c) */
void value_retain(Value *v);
void value_free(Value *v);
char *value_to_string(Value *v);
void value_print(Value *v);

/* Interpreter lifecycle functions (in interpreter.c) */
Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *interp);

/* Microtask queue functions (in interpreter.c) */
void microtask_queue_push(Interpreter *interp, Value callback, Value *args, int argc);
void microtask_queue_process(Interpreter *interp);

/* Module system functions (in interpreter.c) */
char *resolve_module_path(Interpreter *interp, const char *import_path);
LoadedModule *get_cached_module(Interpreter *interp, const char *module_path);
void cache_module(Interpreter *interp, const char *module_path, Env *exports, Env *module_env, ASTNode **ast_nodes, int ast_count);
int is_module_loading(Interpreter *interp, const char *module_path);
void set_module_loading(Interpreter *interp, const char *module_path, int loading);
Value load_module(Interpreter *interp, const char *module_path, Env *env);

#endif /* INTERPRETER_INTERNAL_H */
