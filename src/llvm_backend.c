#include "llvm_backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <llvm-c/TargetMachine.h>

// Try to detect LLVM version by checking for legacy pass headers
// Legacy pass manager was removed in LLVM 17
// Note: LLVM_VERSION_MAJOR is not available in LLVM C API headers,
// so we use __has_include to detect which API is available
#if __has_include(<llvm-c/Transforms/Scalar.h>)
    #include <llvm-c/Transforms/Scalar.h>
    #include <llvm-c/Transforms/Utils.h>
    #define LLVM_LEGACY_PASSES 1
#else
    #include <llvm-c/Transforms/PassBuilder.h>
    #include <llvm-c/Target.h>
    #define LLVM_LEGACY_PASSES 0
#endif

#define SYMBOL_TABLE_SIZE 256

// Forward declarations
static void declare_runtime_functions(LLVMCompilerContext *ctx);
static LLVMValueRef create_main_wrapper(LLVMCompilerContext *ctx, ASTNode **nodes, int count);
static unsigned hash_string(const char *str);

// ============================================================================
// LLVM Backend Initialization
// ============================================================================

void llvm_backend_init(void) {
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
}

// ============================================================================
// Symbol Table Implementation
// ============================================================================

static unsigned hash_string(const char *str) {
    unsigned hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % SYMBOL_TABLE_SIZE;
}

SymbolTable *symbol_table_new(SymbolTable *parent) {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    table->size = SYMBOL_TABLE_SIZE;
    table->buckets = calloc(SYMBOL_TABLE_SIZE, sizeof(SymbolEntry*));
    table->parent = parent;
    
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    
    return table;
}

void symbol_table_free(SymbolTable *table) {
    if (!table) return;
    
    for (int i = 0; i < table->size; i++) {
        SymbolEntry *entry = table->buckets[i];
        while (entry) {
            SymbolEntry *next = entry->next;
            free(entry->name);
            free(entry);
            entry = next;
        }
    }
    
    free(table->buckets);
    free(table);
}

void symbol_table_insert(SymbolTable *table, const char *name, LLVMValueRef value) {
    if (!table || !name) return;
    
    unsigned idx = hash_string(name);
    
    // Check if already exists and update
    SymbolEntry *entry = table->buckets[idx];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    entry = malloc(sizeof(SymbolEntry));
    entry->name = strdup(name);
    entry->value = value;
    entry->next = table->buckets[idx];
    table->buckets[idx] = entry;
}

LLVMValueRef symbol_table_lookup(SymbolTable *table, const char *name) {
    if (!table || !name) return NULL;
    
    // Search current scope
    unsigned idx = hash_string(name);
    SymbolEntry *entry = table->buckets[idx];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    // Search parent scopes
    if (table->parent) {
        return symbol_table_lookup(table->parent, name);
    }
    
    return NULL;
}

// ============================================================================
// Compiler Context
// ============================================================================

LLVMCompilerContext *llvm_context_new(const char *module_name) {
    LLVMCompilerContext *ctx = malloc(sizeof(LLVMCompilerContext));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(LLVMCompilerContext));
    
    // Create LLVM context and module
    ctx->context = LLVMContextCreate();
    ctx->module = LLVMModuleCreateWithNameInContext(module_name, ctx->context);
    ctx->builder = LLVMCreateBuilderInContext(ctx->context);
    
    // Create symbol table
    ctx->symbols = symbol_table_new(NULL);
    
    // Cache common types
    ctx->double_type = LLVMDoubleTypeInContext(ctx->context);
    ctx->i1_type = LLVMInt1TypeInContext(ctx->context);
    ctx->i8_type = LLVMInt8TypeInContext(ctx->context);
    ctx->i32_type = LLVMInt32TypeInContext(ctx->context);
    ctx->i64_type = LLVMInt64TypeInContext(ctx->context);
    ctx->void_type = LLVMVoidTypeInContext(ctx->context);
    ctx->string_type = LLVMPointerType(ctx->i8_type, 0);
    
    // Declare runtime functions
    declare_runtime_functions(ctx);
    
    return ctx;
}

void llvm_context_free(LLVMCompilerContext *ctx) {
    if (!ctx) return;
    
    symbol_table_free(ctx->symbols);
    
    if (ctx->builder) LLVMDisposeBuilder(ctx->builder);
    if (ctx->module) LLVMDisposeModule(ctx->module);
    if (ctx->context) LLVMContextDispose(ctx->context);
    
    free(ctx->error_msg);
    free(ctx);
}

// ============================================================================
// Runtime Function Declarations
// ============================================================================

static void declare_runtime_functions(LLVMCompilerContext *ctx) {
    // printf(char*, ...) -> i32
    LLVMTypeRef printf_args[] = { ctx->string_type };
    LLVMTypeRef printf_type = LLVMFunctionType(ctx->i32_type, printf_args, 1, 1); // variadic
    ctx->printf_func = LLVMAddFunction(ctx->module, "printf", printf_type);
    
    // malloc(i64) -> i8*
    LLVMTypeRef malloc_args[] = { ctx->i64_type };
    LLVMTypeRef malloc_type = LLVMFunctionType(ctx->string_type, malloc_args, 1, 0);
    ctx->malloc_func = LLVMAddFunction(ctx->module, "malloc", malloc_type);
    
    // free(i8*) -> void
    LLVMTypeRef free_args[] = { ctx->string_type };
    LLVMTypeRef free_type = LLVMFunctionType(ctx->void_type, free_args, 1, 0);
    ctx->free_func = LLVMAddFunction(ctx->module, "free", free_type);
    
    // strlen(char*) -> i64
    LLVMTypeRef strlen_args[] = { ctx->string_type };
    LLVMTypeRef strlen_type = LLVMFunctionType(ctx->i64_type, strlen_args, 1, 0);
    ctx->strlen_func = LLVMAddFunction(ctx->module, "strlen", strlen_type);
    
    // strcmp(char*, char*) -> i32
    LLVMTypeRef strcmp_args[] = { ctx->string_type, ctx->string_type };
    LLVMTypeRef strcmp_type = LLVMFunctionType(ctx->i32_type, strcmp_args, 2, 0);
    ctx->strcmp_func = LLVMAddFunction(ctx->module, "strcmp", strcmp_type);
    
    // strcat(char*, char*) -> char*
    LLVMTypeRef strcat_args[] = { ctx->string_type, ctx->string_type };
    LLVMTypeRef strcat_type = LLVMFunctionType(ctx->string_type, strcat_args, 2, 0);
    ctx->strcat_func = LLVMAddFunction(ctx->module, "strcat", strcat_type);
    
    // strcpy(char*, char*) -> char*
    LLVMTypeRef strcpy_args[] = { ctx->string_type, ctx->string_type };
    LLVMTypeRef strcpy_type = LLVMFunctionType(ctx->string_type, strcpy_args, 2, 0);
    ctx->strcpy_func = LLVMAddFunction(ctx->module, "strcpy", strcpy_type);
}

// ============================================================================
// String Literal Compilation
// ============================================================================

LLVMValueRef llvm_compile_string(LLVMCompilerContext *ctx, const char *str) {
    return LLVMBuildGlobalStringPtr(ctx->builder, str, ".str");
}

// ============================================================================
// Expression Compilation
// ============================================================================

LLVMValueRef llvm_compile_expr(LLVMCompilerContext *ctx, ASTNode *node) {
    if (!node) return NULL;
    
    switch (node->type) {
        case NODE_NUMBER:
            return LLVMConstReal(ctx->double_type, node->data.num_val);
        
        case NODE_BOOL:
            return LLVMConstInt(ctx->i1_type, node->data.bool_val, 0);
        
        case NODE_STRING:
            return llvm_compile_string(ctx, node->data.str_val);
        
        case NODE_NULL:
            return LLVMConstNull(ctx->string_type);
        
        case NODE_IDENT: {
            LLVMValueRef var = symbol_table_lookup(ctx->symbols, node->data.ident.name);
            if (!var) {
                fprintf(stderr, "Undefined variable: %s\n", node->data.ident.name);
                ctx->has_error = 1;
                return NULL;
            }
            return LLVMBuildLoad2(ctx->builder, LLVMGetAllocatedType(var), var, node->data.ident.name);
        }
        
        case NODE_BINOP:
            return llvm_compile_binop(ctx, node);
        
        case NODE_UNOP:
            return llvm_compile_unop(ctx, node);
        
        case NODE_CALL:
            return llvm_compile_call(ctx, node);
        
        case NODE_TERNARY: {
            LLVMValueRef cond = llvm_compile_expr(ctx, node->data.ternary.cond);
            if (!cond) return NULL;
            
            // Convert to i1 if needed
            if (LLVMGetTypeKind(LLVMTypeOf(cond)) != LLVMIntegerTypeKind) {
                cond = LLVMBuildFCmp(ctx->builder, LLVMRealONE, cond, 
                                     LLVMConstReal(ctx->double_type, 0.0), "cond");
            }
            
            LLVMBasicBlockRef then_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "ternary.then");
            LLVMBasicBlockRef else_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "ternary.else");
            LLVMBasicBlockRef merge_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "ternary.merge");
            
            LLVMBuildCondBr(ctx->builder, cond, then_bb, else_bb);
            
            // Then branch
            LLVMPositionBuilderAtEnd(ctx->builder, then_bb);
            LLVMValueRef then_val = llvm_compile_expr(ctx, node->data.ternary.true_expr);
            LLVMBasicBlockRef then_end = LLVMGetInsertBlock(ctx->builder);
            LLVMBuildBr(ctx->builder, merge_bb);
            
            // Else branch
            LLVMPositionBuilderAtEnd(ctx->builder, else_bb);
            LLVMValueRef else_val = llvm_compile_expr(ctx, node->data.ternary.false_expr);
            LLVMBasicBlockRef else_end = LLVMGetInsertBlock(ctx->builder);
            LLVMBuildBr(ctx->builder, merge_bb);
            
            // Merge
            LLVMPositionBuilderAtEnd(ctx->builder, merge_bb);
            LLVMValueRef phi = LLVMBuildPhi(ctx->builder, LLVMTypeOf(then_val), "ternary.result");
            LLVMValueRef incoming_values[] = { then_val, else_val };
            LLVMBasicBlockRef incoming_blocks[] = { then_end, else_end };
            LLVMAddIncoming(phi, incoming_values, incoming_blocks, 2);
            
            return phi;
        }
        
        case NODE_LIST: {
            // Basic list support - allocate array
            int count = node->data.list.elements.count;
            LLVMTypeRef array_type = LLVMArrayType(ctx->double_type, count);
            LLVMValueRef array = LLVMBuildAlloca(ctx->builder, array_type, "list");
            
            for (int i = 0; i < count; i++) {
                LLVMValueRef elem = llvm_compile_expr(ctx, node->data.list.elements.items[i]);
                if (elem) {
                    LLVMValueRef indices[] = { 
                        LLVMConstInt(ctx->i32_type, 0, 0),
                        LLVMConstInt(ctx->i32_type, i, 0)
                    };
                    LLVMValueRef ptr = LLVMBuildGEP2(ctx->builder, array_type, array, indices, 2, "elem_ptr");
                    LLVMBuildStore(ctx->builder, elem, ptr);
                }
            }
            
            return array;
        }
        
        case NODE_INDEX: {
            LLVMValueRef obj = llvm_compile_expr(ctx, node->data.index_expr.obj);
            LLVMValueRef index = llvm_compile_expr(ctx, node->data.index_expr.index);
            
            if (!obj || !index) return NULL;
            
            // Convert index to i32
            if (LLVMGetTypeKind(LLVMTypeOf(index)) != LLVMIntegerTypeKind) {
                index = LLVMBuildFPToSI(ctx->builder, index, ctx->i32_type, "idx");
            }
            
            LLVMValueRef indices[] = { LLVMConstInt(ctx->i32_type, 0, 0), index };
            LLVMTypeRef elem_type = LLVMGetElementType(LLVMGetAllocatedType(obj));
            LLVMValueRef ptr = LLVMBuildGEP2(ctx->builder, LLVMGetAllocatedType(obj), obj, indices, 2, "idx_ptr");
            return LLVMBuildLoad2(ctx->builder, elem_type, ptr, "idx_val");
        }
        
        default:
            fprintf(stderr, "Unsupported expression node type: %d\n", node->type);
            ctx->has_error = 1;
            return NULL;
    }
}

// ============================================================================
// Binary Operations
// ============================================================================

LLVMValueRef llvm_compile_binop(LLVMCompilerContext *ctx, ASTNode *node) {
    LLVMValueRef left = llvm_compile_expr(ctx, node->data.binop.left);
    LLVMValueRef right = llvm_compile_expr(ctx, node->data.binop.right);
    
    if (!left || !right) return NULL;
    
    const char *op = node->data.binop.op;
    
    // Check if string comparison
    LLVMTypeKind left_kind = LLVMGetTypeKind(LLVMTypeOf(left));
    LLVMTypeKind right_kind = LLVMGetTypeKind(LLVMTypeOf(right));
    int is_string_op = (left_kind == LLVMPointerTypeKind && right_kind == LLVMPointerTypeKind);
    
    // Arithmetic operators
    if (strcmp(op, "+") == 0) {
        if (is_string_op) {
            // String concatenation - simplified version
            // In real implementation, allocate new buffer
            return left; // Placeholder
        }
        return LLVMBuildFAdd(ctx->builder, left, right, "add");
    }
    if (strcmp(op, "-") == 0) return LLVMBuildFSub(ctx->builder, left, right, "sub");
    if (strcmp(op, "*") == 0) return LLVMBuildFMul(ctx->builder, left, right, "mul");
    if (strcmp(op, "/") == 0) return LLVMBuildFDiv(ctx->builder, left, right, "div");
    if (strcmp(op, "%") == 0) return LLVMBuildFRem(ctx->builder, left, right, "mod");
    
    // Comparison operators
    if (strcmp(op, "==") == 0) {
        if (is_string_op) {
            LLVMValueRef cmp_result = LLVMBuildCall2(ctx->builder, 
                LLVMGetElementType(LLVMTypeOf(ctx->strcmp_func)),
                ctx->strcmp_func, (LLVMValueRef[]){ left, right }, 2, "strcmp");
            return LLVMBuildICmp(ctx->builder, LLVMIntEQ, cmp_result, 
                                LLVMConstInt(ctx->i32_type, 0, 0), "streq");
        }
        return LLVMBuildFCmp(ctx->builder, LLVMRealOEQ, left, right, "eq");
    }
    if (strcmp(op, "!=") == 0) {
        if (is_string_op) {
            LLVMValueRef cmp_result = LLVMBuildCall2(ctx->builder,
                LLVMGetElementType(LLVMTypeOf(ctx->strcmp_func)),
                ctx->strcmp_func, (LLVMValueRef[]){ left, right }, 2, "strcmp");
            return LLVMBuildICmp(ctx->builder, LLVMIntNE, cmp_result, 
                                LLVMConstInt(ctx->i32_type, 0, 0), "strne");
        }
        return LLVMBuildFCmp(ctx->builder, LLVMRealONE, left, right, "ne");
    }
    if (strcmp(op, "<") == 0) return LLVMBuildFCmp(ctx->builder, LLVMRealOLT, left, right, "lt");
    if (strcmp(op, ">") == 0) return LLVMBuildFCmp(ctx->builder, LLVMRealOGT, left, right, "gt");
    if (strcmp(op, "<=") == 0) return LLVMBuildFCmp(ctx->builder, LLVMRealOLE, left, right, "le");
    if (strcmp(op, ">=") == 0) return LLVMBuildFCmp(ctx->builder, LLVMRealOGE, left, right, "ge");
    
    // Logical operators
    if (strcmp(op, "&&") == 0) {
        // Convert to bool if needed
        if (LLVMGetTypeKind(LLVMTypeOf(left)) != LLVMIntegerTypeKind) {
            left = LLVMBuildFCmp(ctx->builder, LLVMRealONE, left, 
                                LLVMConstReal(ctx->double_type, 0.0), "lbool");
        }
        if (LLVMGetTypeKind(LLVMTypeOf(right)) != LLVMIntegerTypeKind) {
            right = LLVMBuildFCmp(ctx->builder, LLVMRealONE, right, 
                                 LLVMConstReal(ctx->double_type, 0.0), "rbool");
        }
        return LLVMBuildAnd(ctx->builder, left, right, "and");
    }
    if (strcmp(op, "||") == 0) {
        if (LLVMGetTypeKind(LLVMTypeOf(left)) != LLVMIntegerTypeKind) {
            left = LLVMBuildFCmp(ctx->builder, LLVMRealONE, left, 
                                LLVMConstReal(ctx->double_type, 0.0), "lbool");
        }
        if (LLVMGetTypeKind(LLVMTypeOf(right)) != LLVMIntegerTypeKind) {
            right = LLVMBuildFCmp(ctx->builder, LLVMRealONE, right, 
                                 LLVMConstReal(ctx->double_type, 0.0), "rbool");
        }
        return LLVMBuildOr(ctx->builder, left, right, "or");
    }
    
    fprintf(stderr, "Unknown binary operator: %s\n", op);
    ctx->has_error = 1;
    return NULL;
}

// ============================================================================
// Unary Operations
// ============================================================================

LLVMValueRef llvm_compile_unop(LLVMCompilerContext *ctx, ASTNode *node) {
    LLVMValueRef operand = llvm_compile_expr(ctx, node->data.unop.operand);
    if (!operand) return NULL;
    
    const char *op = node->data.unop.op;
    
    if (strcmp(op, "-") == 0) {
        return LLVMBuildFNeg(ctx->builder, operand, "neg");
    }
    if (strcmp(op, "!") == 0) {
        // Convert to bool if needed
        if (LLVMGetTypeKind(LLVMTypeOf(operand)) != LLVMIntegerTypeKind) {
            operand = LLVMBuildFCmp(ctx->builder, LLVMRealONE, operand, 
                                   LLVMConstReal(ctx->double_type, 0.0), "bool");
        }
        return LLVMBuildNot(ctx->builder, operand, "not");
    }
    
    fprintf(stderr, "Unknown unary operator: %s\n", op);
    ctx->has_error = 1;
    return NULL;
}

// ============================================================================
// Function Calls
// ============================================================================

LLVMValueRef llvm_compile_call(LLVMCompilerContext *ctx, ASTNode *node) {
    // Get function name
    if (node->data.call.callee->type != NODE_IDENT) {
        fprintf(stderr, "Complex function calls not yet supported\n");
        ctx->has_error = 1;
        return NULL;
    }
    
    const char *func_name = node->data.call.callee->data.ident.name;
    
    // Handle built-in print function with a simpler approach
    if (strcmp(func_name, "print") == 0 || strcmp(func_name, "println") == 0) {
        int arg_count = node->data.call.args.count;
        
        // Build format string and collect all arguments
        char format_str[1024] = "";
        LLVMValueRef *print_args = malloc(sizeof(LLVMValueRef) * (arg_count + 1));
        int print_arg_count = 1; // Start with 1 for format string
        
        for (int i = 0; i < arg_count; i++) {
            ASTNode *arg_node = node->data.call.args.items[i];
            if (!arg_node) continue;
            
            LLVMValueRef arg = llvm_compile_expr(ctx, arg_node);
            if (!arg) continue;
            
            LLVMTypeRef arg_type = LLVMTypeOf(arg);
            LLVMTypeKind kind = LLVMGetTypeKind(arg_type);
            
            // Add space separator if not first argument
            if (i > 0) {
                strcat(format_str, " ");
            }
            
            if (kind == LLVMPointerTypeKind) {
                // String argument
                strcat(format_str, "%s");
                print_args[print_arg_count++] = arg;
            } else if (kind == LLVMIntegerTypeKind) {
                // Boolean - convert to "true" or "false" string inline
                LLVMValueRef current_func = ctx->current_function;
                
                LLVMBasicBlockRef true_bb = LLVMAppendBasicBlockInContext(ctx->context, current_func, "bool.true");
                LLVMBasicBlockRef false_bb = LLVMAppendBasicBlockInContext(ctx->context, current_func, "bool.false");
                LLVMBasicBlockRef merge_bb = LLVMAppendBasicBlockInContext(ctx->context, current_func, "bool.merge");
                
                LLVMBuildCondBr(ctx->builder, arg, true_bb, false_bb);
                
                LLVMPositionBuilderAtEnd(ctx->builder, true_bb);
                LLVMValueRef true_str = llvm_compile_string(ctx, "true");
                LLVMBuildBr(ctx->builder, merge_bb);
                LLVMBasicBlockRef true_end = LLVMGetInsertBlock(ctx->builder);
                
                LLVMPositionBuilderAtEnd(ctx->builder, false_bb);
                LLVMValueRef false_str = llvm_compile_string(ctx, "false");
                LLVMBuildBr(ctx->builder, merge_bb);
                LLVMBasicBlockRef false_end = LLVMGetInsertBlock(ctx->builder);
                
                LLVMPositionBuilderAtEnd(ctx->builder, merge_bb);
                LLVMValueRef phi = LLVMBuildPhi(ctx->builder, ctx->string_type, "bool.str");
                LLVMValueRef incoming_values[] = { true_str, false_str };
                LLVMBasicBlockRef incoming_blocks[] = { true_end, false_end };
                LLVMAddIncoming(phi, incoming_values, incoming_blocks, 2);
                
                strcat(format_str, "%s");
                print_args[print_arg_count++] = phi;
            } else {
                // Numeric (double)
                strcat(format_str, "%g");
                print_args[print_arg_count++] = arg;
            }
        }
        
        // Add newline for println
        if (strcmp(func_name, "println") == 0) {
            strcat(format_str, "\n");
        }
        
        // Create format string constant
        print_args[0] = llvm_compile_string(ctx, format_str);
        
        // Call printf directly with the function type we declared
        LLVMTypeRef printf_args_types[] = { ctx->string_type };
        LLVMTypeRef printf_type = LLVMFunctionType(ctx->i32_type, printf_args_types, 1, 1);
        LLVMBuildCall2(ctx->builder, printf_type, ctx->printf_func, print_args, print_arg_count, "");
        
        free(print_args);
        return LLVMConstReal(ctx->double_type, 0.0);
    }
    
    // Look up user-defined function
    LLVMValueRef func = LLVMGetNamedFunction(ctx->module, func_name);
    if (!func) {
        fprintf(stderr, "Undefined function: %s\n", func_name);
        ctx->has_error = 1;
        return NULL;
    }
    
    // Compile arguments
    int arg_count = node->data.call.args.count;
    LLVMValueRef *args = malloc(sizeof(LLVMValueRef) * arg_count);
    
    for (int i = 0; i < arg_count; i++) {
        args[i] = llvm_compile_expr(ctx, node->data.call.args.items[i]);
        if (!args[i]) {
            free(args);
            return NULL;
        }
    }
    
    LLVMTypeRef func_type = LLVMGetElementType(LLVMTypeOf(func));
    LLVMValueRef result = LLVMBuildCall2(ctx->builder, func_type, func, args, arg_count, "call");
    
    free(args);
    return result;
}

// ============================================================================
// Statement Compilation
// ============================================================================

void llvm_compile_stmt(LLVMCompilerContext *ctx, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LET: {
            // Variable declaration
            const char *name = node->data.let_stmt.name;
            
            // Allocate stack space
            LLVMValueRef alloca = LLVMBuildAlloca(ctx->builder, ctx->double_type, name);
            
            // Initialize if value provided
            if (node->data.let_stmt.value) {
                LLVMValueRef init_val = llvm_compile_expr(ctx, node->data.let_stmt.value);
                if (init_val) {
                    LLVMBuildStore(ctx->builder, init_val, alloca);
                }
            } else {
                // Default initialize to 0
                LLVMBuildStore(ctx->builder, LLVMConstReal(ctx->double_type, 0.0), alloca);
            }
            
            // Add to symbol table
            symbol_table_insert(ctx->symbols, name, alloca);
            break;
        }
        
        case NODE_ASSIGN: {
            // Variable assignment
            const char *name = node->data.assign_stmt.name;
            LLVMValueRef var = symbol_table_lookup(ctx->symbols, name);
            
            if (!var) {
                fprintf(stderr, "Undefined variable: %s\n", name);
                ctx->has_error = 1;
                return;
            }
            
            LLVMValueRef value = llvm_compile_expr(ctx, node->data.assign_stmt.value);
            if (value) {
                LLVMBuildStore(ctx->builder, value, var);
            }
            break;
        }
        
        case NODE_IF:
            llvm_compile_if(ctx, node);
            break;
        
        case NODE_WHILE:
            llvm_compile_while(ctx, node);
            break;
        
        case NODE_FOR:
        case NODE_FOR_C_STYLE:
            llvm_compile_for(ctx, node);
            break;
        
        case NODE_RETURN:
            llvm_compile_return(ctx, node);
            break;
        
        case NODE_BREAK: {
            if (!ctx->current_break_target) {
                fprintf(stderr, "break statement outside loop\n");
                ctx->has_error = 1;
                return;
            }
            LLVMBuildBr(ctx->builder, ctx->current_break_target);
            break;
        }
        
        case NODE_CONTINUE: {
            if (!ctx->current_continue_target) {
                fprintf(stderr, "continue statement outside loop\n");
                ctx->has_error = 1;
                return;
            }
            LLVMBuildBr(ctx->builder, ctx->current_continue_target);
            break;
        }
        
        case NODE_BLOCK: {
            // Create new scope
            SymbolTable *old_symbols = ctx->symbols;
            ctx->symbols = symbol_table_new(old_symbols);
            
            for (int i = 0; i < node->data.block.stmts.count; i++) {
                llvm_compile_stmt(ctx, node->data.block.stmts.items[i]);
                if (ctx->has_error) break;
            }
            
            // Restore old scope
            symbol_table_free(ctx->symbols);
            ctx->symbols = old_symbols;
            break;
        }
        
        case NODE_FUNC_DEF:
            llvm_compile_function(ctx, node);
            break;
        
        case NODE_CALL:
            // Expression statement
            llvm_compile_call(ctx, node);
            break;
        
        default:
            // Try as expression
            llvm_compile_expr(ctx, node);
            break;
    }
}

// ============================================================================
// Control Flow - If Statement
// ============================================================================

void llvm_compile_if(LLVMCompilerContext *ctx, ASTNode *node) {
    LLVMValueRef cond = llvm_compile_expr(ctx, node->data.if_stmt.cond);
    if (!cond) return;
    
    // Convert to i1 if needed
    if (LLVMGetTypeKind(LLVMTypeOf(cond)) != LLVMIntegerTypeKind) {
        cond = LLVMBuildFCmp(ctx->builder, LLVMRealONE, cond, 
                            LLVMConstReal(ctx->double_type, 0.0), "ifcond");
    }
    
    LLVMBasicBlockRef then_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "if.then");
    LLVMBasicBlockRef else_bb = node->data.if_stmt.else_block ? 
        LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "if.else") : NULL;
    LLVMBasicBlockRef merge_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "if.merge");
    
    if (else_bb) {
        LLVMBuildCondBr(ctx->builder, cond, then_bb, else_bb);
    } else {
        LLVMBuildCondBr(ctx->builder, cond, then_bb, merge_bb);
    }
    
    // Then block
    LLVMPositionBuilderAtEnd(ctx->builder, then_bb);
    llvm_compile_stmt(ctx, node->data.if_stmt.then_block);
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
        LLVMBuildBr(ctx->builder, merge_bb);
    }
    
    // Else block
    if (else_bb) {
        LLVMPositionBuilderAtEnd(ctx->builder, else_bb);
        llvm_compile_stmt(ctx, node->data.if_stmt.else_block);
        if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
            LLVMBuildBr(ctx->builder, merge_bb);
        }
    }
    
    // Merge block
    LLVMPositionBuilderAtEnd(ctx->builder, merge_bb);
}

// ============================================================================
// Control Flow - While Loop
// ============================================================================

void llvm_compile_while(LLVMCompilerContext *ctx, ASTNode *node) {
    LLVMBasicBlockRef cond_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "while.cond");
    LLVMBasicBlockRef body_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "while.body");
    LLVMBasicBlockRef after_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "while.after");
    
    // Save old targets
    LLVMBasicBlockRef old_break = ctx->current_break_target;
    LLVMBasicBlockRef old_continue = ctx->current_continue_target;
    
    ctx->current_break_target = after_bb;
    ctx->current_continue_target = cond_bb;
    
    // Jump to condition
    LLVMBuildBr(ctx->builder, cond_bb);
    
    // Condition block
    LLVMPositionBuilderAtEnd(ctx->builder, cond_bb);
    LLVMValueRef cond = llvm_compile_expr(ctx, node->data.while_stmt.cond);
    if (cond) {
        if (LLVMGetTypeKind(LLVMTypeOf(cond)) != LLVMIntegerTypeKind) {
            cond = LLVMBuildFCmp(ctx->builder, LLVMRealONE, cond, 
                                LLVMConstReal(ctx->double_type, 0.0), "whilecond");
        }
        LLVMBuildCondBr(ctx->builder, cond, body_bb, after_bb);
    }
    
    // Body block
    LLVMPositionBuilderAtEnd(ctx->builder, body_bb);
    llvm_compile_stmt(ctx, node->data.while_stmt.body);
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
        LLVMBuildBr(ctx->builder, cond_bb);
    }
    
    // After block
    LLVMPositionBuilderAtEnd(ctx->builder, after_bb);
    
    // Restore old targets
    ctx->current_break_target = old_break;
    ctx->current_continue_target = old_continue;
}

// ============================================================================
// Control Flow - For Loop
// ============================================================================

void llvm_compile_for(LLVMCompilerContext *ctx, ASTNode *node) {
    if (node->type == NODE_FOR_C_STYLE) {
        // C-style for loop: for (init; cond; update) body
        
        // Initialize
        if (node->data.for_c_style_stmt.init) {
            llvm_compile_stmt(ctx, node->data.for_c_style_stmt.init);
        }
        
        LLVMBasicBlockRef cond_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.cond");
        LLVMBasicBlockRef body_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.body");
        LLVMBasicBlockRef update_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.update");
        LLVMBasicBlockRef after_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.after");
        
        LLVMBasicBlockRef old_break = ctx->current_break_target;
        LLVMBasicBlockRef old_continue = ctx->current_continue_target;
        
        ctx->current_break_target = after_bb;
        ctx->current_continue_target = update_bb;
        
        LLVMBuildBr(ctx->builder, cond_bb);
        
        // Condition
        LLVMPositionBuilderAtEnd(ctx->builder, cond_bb);
        if (node->data.for_c_style_stmt.cond) {
            LLVMValueRef cond = llvm_compile_expr(ctx, node->data.for_c_style_stmt.cond);
            if (cond) {
                if (LLVMGetTypeKind(LLVMTypeOf(cond)) != LLVMIntegerTypeKind) {
                    cond = LLVMBuildFCmp(ctx->builder, LLVMRealONE, cond, 
                                        LLVMConstReal(ctx->double_type, 0.0), "forcond");
                }
                LLVMBuildCondBr(ctx->builder, cond, body_bb, after_bb);
            } else {
                LLVMBuildBr(ctx->builder, body_bb);
            }
        } else {
            LLVMBuildBr(ctx->builder, body_bb);
        }
        
        // Body
        LLVMPositionBuilderAtEnd(ctx->builder, body_bb);
        llvm_compile_stmt(ctx, node->data.for_c_style_stmt.body);
        if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
            LLVMBuildBr(ctx->builder, update_bb);
        }
        
        // Update
        LLVMPositionBuilderAtEnd(ctx->builder, update_bb);
        if (node->data.for_c_style_stmt.update) {
            llvm_compile_stmt(ctx, node->data.for_c_style_stmt.update);
        }
        LLVMBuildBr(ctx->builder, cond_bb);
        
        // After
        LLVMPositionBuilderAtEnd(ctx->builder, after_bb);
        
        ctx->current_break_target = old_break;
        ctx->current_continue_target = old_continue;
    } else {
        // Range-based for: for i in start..end
        const char *var_name = node->data.for_stmt.var;
        
        // Allocate loop variable
        LLVMValueRef loop_var = LLVMBuildAlloca(ctx->builder, ctx->double_type, var_name);
        
        // Initialize
        LLVMValueRef start = llvm_compile_expr(ctx, node->data.for_stmt.start);
        LLVMValueRef end = llvm_compile_expr(ctx, node->data.for_stmt.end);
        if (!start || !end) return;
        
        LLVMBuildStore(ctx->builder, start, loop_var);
        symbol_table_insert(ctx->symbols, var_name, loop_var);
        
        LLVMBasicBlockRef cond_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.cond");
        LLVMBasicBlockRef body_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.body");
        LLVMBasicBlockRef update_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.update");
        LLVMBasicBlockRef after_bb = LLVMAppendBasicBlockInContext(ctx->context, ctx->current_function, "for.after");
        
        LLVMBasicBlockRef old_break = ctx->current_break_target;
        LLVMBasicBlockRef old_continue = ctx->current_continue_target;
        
        ctx->current_break_target = after_bb;
        ctx->current_continue_target = update_bb;
        
        LLVMBuildBr(ctx->builder, cond_bb);
        
        // Condition: i < end
        LLVMPositionBuilderAtEnd(ctx->builder, cond_bb);
        LLVMValueRef current = LLVMBuildLoad2(ctx->builder, ctx->double_type, loop_var, "i");
        LLVMValueRef cond = LLVMBuildFCmp(ctx->builder, LLVMRealOLT, current, end, "forcond");
        LLVMBuildCondBr(ctx->builder, cond, body_bb, after_bb);
        
        // Body
        LLVMPositionBuilderAtEnd(ctx->builder, body_bb);
        llvm_compile_stmt(ctx, node->data.for_stmt.body);
        if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
            LLVMBuildBr(ctx->builder, update_bb);
        }
        
        // Update: i++
        LLVMPositionBuilderAtEnd(ctx->builder, update_bb);
        current = LLVMBuildLoad2(ctx->builder, ctx->double_type, loop_var, "i");
        LLVMValueRef next = LLVMBuildFAdd(ctx->builder, current, 
                                         LLVMConstReal(ctx->double_type, 1.0), "next");
        LLVMBuildStore(ctx->builder, next, loop_var);
        LLVMBuildBr(ctx->builder, cond_bb);
        
        // After
        LLVMPositionBuilderAtEnd(ctx->builder, after_bb);
        
        ctx->current_break_target = old_break;
        ctx->current_continue_target = old_continue;
    }
}

// ============================================================================
// Return Statement
// ============================================================================

void llvm_compile_return(LLVMCompilerContext *ctx, ASTNode *node) {
    if (node->data.return_stmt.value) {
        LLVMValueRef ret_val = llvm_compile_expr(ctx, node->data.return_stmt.value);
        if (ret_val) {
            LLVMBuildRet(ctx->builder, ret_val);
        }
    } else {
        LLVMBuildRetVoid(ctx->builder);
    }
}

// ============================================================================
// Function Definition
// ============================================================================

void llvm_compile_function(LLVMCompilerContext *ctx, ASTNode *node) {
    if (!node || !ctx) return;
    
    const char *func_name = node->data.func_def.name;
    int param_count = node->data.func_def.params.count;
    
    // Build parameter types (all double for now)
    LLVMTypeRef *param_types = malloc(sizeof(LLVMTypeRef) * param_count);
    for (int i = 0; i < param_count; i++) {
        param_types[i] = ctx->double_type;
    }
    
    // Create function type
    LLVMTypeRef ret_type = node->data.func_def.return_type ? ctx->double_type : ctx->double_type;
    LLVMTypeRef func_type = LLVMFunctionType(ret_type, param_types, param_count, 0);
    
    // Create function
    LLVMValueRef func = LLVMAddFunction(ctx->module, func_name, func_type);
    
    // Create entry block
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx->context, func, "entry");
    LLVMPositionBuilderAtEnd(ctx->builder, entry);
    
    // Save state
    LLVMValueRef old_function = ctx->current_function;
    SymbolTable *old_symbols = ctx->symbols;
    
    ctx->current_function = func;
    ctx->symbols = symbol_table_new(old_symbols);
    
    // Allocate parameters and add to symbol table
    for (int i = 0; i < param_count; i++) {
        ASTNode *param = node->data.func_def.params.items[i];
        const char *param_name = param->data.ident.name;
        
        LLVMValueRef param_val = LLVMGetParam(func, i);
        LLVMValueRef alloca = LLVMBuildAlloca(ctx->builder, ctx->double_type, param_name);
        LLVMBuildStore(ctx->builder, param_val, alloca);
        
        symbol_table_insert(ctx->symbols, param_name, alloca);
    }
    
    // Compile function body
    llvm_compile_stmt(ctx, node->data.func_def.body);
    
    // Add default return if needed
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
        if (node->data.func_def.return_type) {
            LLVMBuildRet(ctx->builder, LLVMConstReal(ctx->double_type, 0.0));
        } else {
            LLVMBuildRetVoid(ctx->builder);
        }
    }
    
    // Restore state
    ctx->current_function = old_function;
    symbol_table_free(ctx->symbols);
    ctx->symbols = old_symbols;
    
    free(param_types);
}

// ============================================================================
// Main Wrapper Function
// ============================================================================

static LLVMValueRef create_main_wrapper(LLVMCompilerContext *ctx, ASTNode **nodes, int count) {
    
    // First pass: compile function definitions
    for (int i = 0; i < count; i++) {
        if (nodes[i] && nodes[i]->type == NODE_FUNC_DEF) {
            llvm_compile_function(ctx, nodes[i]);
            if (ctx->has_error) return NULL;
        }
    }
    
    
    // Create main function: int main(void)
    LLVMTypeRef main_type = LLVMFunctionType(ctx->i32_type, NULL, 0, 0);
    LLVMValueRef main_func = LLVMAddFunction(ctx->module, "main", main_type);
    
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx->context, main_func, "entry");
    LLVMPositionBuilderAtEnd(ctx->builder, entry);
    
    ctx->current_function = main_func;
    
    
    // Second pass: compile top-level statements (excluding function definitions)
    for (int i = 0; i < count; i++) {
        if (nodes[i] && nodes[i]->type != NODE_FUNC_DEF) {
            llvm_compile_stmt(ctx, nodes[i]);
            if (ctx->has_error) return NULL;
        }
    }
    
    
    // Add return 0 if not already terminated
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
        LLVMBuildRet(ctx->builder, LLVMConstInt(ctx->i32_type, 0, 0));
    }
    
    
    return main_func;
}

// ============================================================================
// Optimization Passes
// ============================================================================

void llvm_apply_optimizations(LLVMCompilerContext *ctx) {
#if LLVM_LEGACY_PASSES
    // Use legacy pass manager for LLVM < 17
    LLVMPassManagerRef pm = LLVMCreatePassManager();
    
    // Add optimization passes
    LLVMAddPromoteMemoryToRegisterPass(pm);      // mem2reg
    LLVMAddInstructionCombiningPass(pm);         // Instruction combining
    LLVMAddReassociatePass(pm);                  // Reassociate expressions
    LLVMAddGVNPass(pm);                          // Global Value Numbering
    LLVMAddCFGSimplificationPass(pm);            // Simplify CFG
    LLVMAddDeadStoreEliminationPass(pm);         // Dead store elimination
    LLVMAddAggressiveDCEPass(pm);                // Dead code elimination
    LLVMAddCorrelatedValuePropagationPass(pm);   // Correlated value propagation
    
    // Run passes
    LLVMRunPassManager(pm, ctx->module);
    
    // Cleanup
    LLVMDisposePassManager(pm);
#else
    // Use new pass manager for LLVM >= 17
    // Create a target machine for optimization (required by new PM)
    char *triple = LLVMGetDefaultTargetTriple();
    char *cpu = LLVMGetHostCPUName();
    char *features = LLVMGetHostCPUFeatures();
    
    LLVMTargetRef target;
    char *target_error = NULL;
    if (LLVMGetTargetFromTriple(triple, &target, &target_error) != 0) {
        fprintf(stderr, "Failed to get target: %s\n", target_error);
        LLVMDisposeMessage(target_error);
        LLVMDisposeMessage(triple);
        LLVMDisposeMessage(cpu);
        LLVMDisposeMessage(features);
        return;
    }
    
    LLVMTargetMachineRef tm = LLVMCreateTargetMachine(
        target, triple, cpu, features,
        LLVMCodeGenLevelDefault,
        LLVMRelocDefault,
        LLVMCodeModelDefault
    );
    
    // Create pass builder options
    LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();
    LLVMPassBuilderOptionsSetVerifyEach(options, 0);
    
    // Run optimization pipeline similar to -O2
    // This includes mem2reg, instcombine, reassociate, gvn, simplifycfg, dse, adce, and more
    const char *passes = "default<O2>";
    
    // LLVMRunPasses returns NULL on success, non-NULL LLVMErrorRef on failure
    // LLVMGetErrorMessage consumes the error, so no explicit cleanup needed
    LLVMErrorRef err = LLVMRunPasses(ctx->module, passes, tm, options);
    if (err) {
        char *err_msg = LLVMGetErrorMessage(err);
        fprintf(stderr, "Failed to run optimization passes: %s\n", err_msg);
        LLVMDisposeErrorMessage(err_msg);
    }
    
    // Cleanup
    LLVMDisposePassBuilderOptions(options);
    LLVMDisposeTargetMachine(tm);
    LLVMDisposeMessage(triple);
    LLVMDisposeMessage(cpu);
    LLVMDisposeMessage(features);
#endif
}

// ============================================================================
// File I/O Operations
// ============================================================================

int llvm_write_ir(LLVMCompilerContext *ctx, const char *path) {
    char *error = NULL;
    if (LLVMPrintModuleToFile(ctx->module, path, &error)) {
        fprintf(stderr, "Error writing IR: %s\n", error);
        LLVMDisposeMessage(error);
        return -1;
    }
    return 0;
}

int llvm_write_object_file(LLVMCompilerContext *ctx, const char *path) {
    char *error = NULL;
    
    // Get target triple
    char *target_triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(ctx->module, target_triple);
    
    // Get target
    LLVMTargetRef target;
    if (LLVMGetTargetFromTriple(target_triple, &target, &error)) {
        fprintf(stderr, "Error getting target: %s\n", error);
        LLVMDisposeMessage(error);
        LLVMDisposeMessage(target_triple);
        return -1;
    }
    
    // Create target machine
    LLVMTargetMachineRef machine = LLVMCreateTargetMachine(
        target, target_triple, "generic", "",
        LLVMCodeGenLevelDefault,
        LLVMRelocDefault,
        LLVMCodeModelDefault
    );
    
    // Emit object file
    if (LLVMTargetMachineEmitToFile(machine, ctx->module, (char*)path, 
                                    LLVMObjectFile, &error)) {
        fprintf(stderr, "Error emitting object file: %s\n", error);
        LLVMDisposeMessage(error);
        LLVMDisposeTargetMachine(machine);
        LLVMDisposeMessage(target_triple);
        return -1;
    }
    
    LLVMDisposeTargetMachine(machine);
    LLVMDisposeMessage(target_triple);
    
    return 0;
}

int llvm_link_executable(const char *object_path, const char *executable_path) {
    // Use system linker (gcc or clang) with -no-pie flag for compatibility
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "gcc -no-pie %s -o %s -lm", object_path, executable_path);
    
    int result = system(cmd);
    if (result != 0) {
        fprintf(stderr, "Error linking executable\n");
        return -1;
    }
    
    return 0;
}

// ============================================================================
// Main Compilation Functions
// ============================================================================

int llvm_compile_to_object(ASTNode **nodes, int count, const char *output_path) {
    
    LLVMCompilerContext *ctx = llvm_context_new("klang_module");
    if (!ctx) {
        fprintf(stderr, "Failed to create LLVM context\n");
        return -1;
    }
    
    
    // Create main function and compile AST
    create_main_wrapper(ctx, nodes, count);
    
    
    if (ctx->has_error) {
        fprintf(stderr, "Compilation errors occurred\n");
        llvm_context_free(ctx);
        return -1;
    }
    
    // Verify module
    char *error = NULL;
    if (LLVMVerifyModule(ctx->module, LLVMPrintMessageAction, &error)) {
        fprintf(stderr, "Module verification failed: %s\n", error);
        LLVMDisposeMessage(error);
        llvm_context_free(ctx);
        return -1;
    }
    
    
    // Apply optimizations
    llvm_apply_optimizations(ctx);
    
    
    // Write object file
    int result = llvm_write_object_file(ctx, output_path);
    
    
    llvm_context_free(ctx);
    return result;
}

int llvm_compile_ast(ASTNode **nodes, int count, const char *output_path) {
    // Generate object file path
    char object_path[1024];
    snprintf(object_path, sizeof(object_path), "%s.o", output_path);
    
    // Compile to object file
    if (llvm_compile_to_object(nodes, count, object_path) != 0) {
        return -1;
    }
    
    // Link to executable
    if (llvm_link_executable(object_path, output_path) != 0) {
        remove(object_path);
        return -1;
    }
    
    // Clean up object file
    remove(object_path);
    
    return 0;
}
