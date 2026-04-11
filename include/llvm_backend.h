#ifndef LLVM_BACKEND_H
#define LLVM_BACKEND_H

#include "core/ast/ast.h"
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>

/**
 * LLVM Backend for KLang Compiler
 * Generates native machine code via LLVM IR
 */

// Compilation context
typedef struct {
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMContextRef context;
    
    // Symbol table for variables (name -> LLVMValueRef)
    struct SymbolTable *symbols;
    
    // Function context
    LLVMValueRef current_function;
    LLVMBasicBlockRef current_break_target;
    LLVMBasicBlockRef current_continue_target;
    
    // Type cache
    LLVMTypeRef double_type;
    LLVMTypeRef i1_type;     // bool
    LLVMTypeRef i8_type;     // char
    LLVMTypeRef i32_type;    // int
    LLVMTypeRef i64_type;    // long
    LLVMTypeRef void_type;
    LLVMTypeRef string_type; // i8*
    
    // Runtime function declarations
    LLVMValueRef printf_func;
    LLVMValueRef malloc_func;
    LLVMValueRef free_func;
    LLVMValueRef strlen_func;
    LLVMValueRef strcmp_func;
    LLVMValueRef strcat_func;
    LLVMValueRef strcpy_func;
    
    // Error handling
    char *error_msg;
    int has_error;
} LLVMCompilerContext;

// Symbol table entry
typedef struct SymbolEntry {
    char *name;
    LLVMValueRef value;
    struct SymbolEntry *next;
} SymbolEntry;

typedef struct SymbolTable {
    SymbolEntry **buckets;
    int size;
    struct SymbolTable *parent; // For nested scopes
} SymbolTable;

/**
 * Initialize LLVM backend
 * Must be called before any compilation
 */
void llvm_backend_init(void);

/**
 * Create a new LLVM compiler context
 * @param module_name Name for the LLVM module
 * @return Compiler context or NULL on error
 */
LLVMCompilerContext *llvm_context_new(const char *module_name);

/**
 * Free LLVM compiler context
 */
void llvm_context_free(LLVMCompilerContext *ctx);

/**
 * Compile AST to native executable
 * @param nodes Array of AST nodes (top-level statements)
 * @param count Number of nodes
 * @param output_path Path for output executable
 * @return 0 on success, non-zero on error
 */
int llvm_compile_ast(ASTNode **nodes, int count, const char *output_path);

/**
 * Compile AST to object file
 * @param nodes Array of AST nodes
 * @param count Number of nodes
 * @param output_path Path for output object file (.o)
 * @return 0 on success, non-zero on error
 */
int llvm_compile_to_object(ASTNode **nodes, int count, const char *output_path);

/**
 * Compile single AST node to LLVM IR
 * @param ctx Compiler context
 * @param node AST node to compile
 * @return LLVM value or NULL on error
 */
LLVMValueRef llvm_compile_node(LLVMCompilerContext *ctx, ASTNode *node);

/**
 * Generate LLVM IR for expression node
 */
LLVMValueRef llvm_compile_expr(LLVMCompilerContext *ctx, ASTNode *node);

/**
 * Generate LLVM IR for statement node
 */
void llvm_compile_stmt(LLVMCompilerContext *ctx, ASTNode *node);

/**
 * Apply optimization passes
 * @param ctx Compiler context
 */
void llvm_apply_optimizations(LLVMCompilerContext *ctx);

/**
 * Write LLVM IR to file (for debugging)
 */
int llvm_write_ir(LLVMCompilerContext *ctx, const char *path);

/**
 * Write object file
 */
int llvm_write_object_file(LLVMCompilerContext *ctx, const char *path);

/**
 * Link object file to executable
 */
int llvm_link_executable(const char *object_path, const char *executable_path);

// Symbol table operations
SymbolTable *symbol_table_new(SymbolTable *parent);
void symbol_table_free(SymbolTable *table);
void symbol_table_insert(SymbolTable *table, const char *name, LLVMValueRef value);
LLVMValueRef symbol_table_lookup(SymbolTable *table, const char *name);

// Helper functions for specific node types
LLVMValueRef llvm_compile_binop(LLVMCompilerContext *ctx, ASTNode *node);
LLVMValueRef llvm_compile_unop(LLVMCompilerContext *ctx, ASTNode *node);
LLVMValueRef llvm_compile_call(LLVMCompilerContext *ctx, ASTNode *node);
LLVMValueRef llvm_compile_string(LLVMCompilerContext *ctx, const char *str);
void llvm_compile_function(LLVMCompilerContext *ctx, ASTNode *node);
void llvm_compile_if(LLVMCompilerContext *ctx, ASTNode *node);
void llvm_compile_while(LLVMCompilerContext *ctx, ASTNode *node);
void llvm_compile_for(LLVMCompilerContext *ctx, ASTNode *node);
void llvm_compile_return(LLVMCompilerContext *ctx, ASTNode *node);

#endif // LLVM_BACKEND_H
