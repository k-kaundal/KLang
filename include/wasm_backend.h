#ifndef WASM_BACKEND_H
#define WASM_BACKEND_H

#include <stdio.h>
#include <stdint.h>
#include "core/ast/ast.h"

/* WASM value types */
typedef enum {
    WASM_I32,
    WASM_I64,
    WASM_F32,
    WASM_F64,
    WASM_VOID
} WasmType;

/* WASM opcodes (subset) */
typedef enum {
    /* Control */
    WASM_OP_UNREACHABLE = 0x00,
    WASM_OP_NOP = 0x01,
    WASM_OP_BLOCK = 0x02,
    WASM_OP_LOOP = 0x03,
    WASM_OP_IF = 0x04,
    WASM_OP_ELSE = 0x05,
    WASM_OP_END = 0x0B,
    WASM_OP_BR = 0x0C,
    WASM_OP_BR_IF = 0x0D,
    WASM_OP_RETURN = 0x0F,
    WASM_OP_CALL = 0x10,
    
    /* Parametric */
    WASM_OP_DROP = 0x1A,
    WASM_OP_SELECT = 0x1B,
    
    /* Variable */
    WASM_OP_LOCAL_GET = 0x20,
    WASM_OP_LOCAL_SET = 0x21,
    WASM_OP_LOCAL_TEE = 0x22,
    WASM_OP_GLOBAL_GET = 0x23,
    WASM_OP_GLOBAL_SET = 0x24,
    
    /* Memory */
    WASM_OP_I32_LOAD = 0x28,
    WASM_OP_I64_LOAD = 0x29,
    WASM_OP_F32_LOAD = 0x2A,
    WASM_OP_F64_LOAD = 0x2B,
    WASM_OP_I32_STORE = 0x36,
    WASM_OP_I64_STORE = 0x37,
    WASM_OP_F32_STORE = 0x38,
    WASM_OP_F64_STORE = 0x39,
    
    /* Numeric i32 */
    WASM_OP_I32_CONST = 0x41,
    WASM_OP_I32_EQZ = 0x45,
    WASM_OP_I32_EQ = 0x46,
    WASM_OP_I32_NE = 0x47,
    WASM_OP_I32_LT_S = 0x48,
    WASM_OP_I32_GT_S = 0x4A,
    WASM_OP_I32_LE_S = 0x4C,
    WASM_OP_I32_GE_S = 0x4E,
    
    /* Numeric i64 */
    WASM_OP_I64_CONST = 0x42,
    
    /* Numeric f32 */
    WASM_OP_F32_CONST = 0x43,
    
    /* Numeric f64 */
    WASM_OP_F64_CONST = 0x44,
    
    /* i32 operations */
    WASM_OP_I32_ADD = 0x6A,
    WASM_OP_I32_SUB = 0x6B,
    WASM_OP_I32_MUL = 0x6C,
    WASM_OP_I32_DIV_S = 0x6D,
    WASM_OP_I32_REM_S = 0x6F,
    WASM_OP_I32_AND = 0x71,
    WASM_OP_I32_OR = 0x72,
    WASM_OP_I32_XOR = 0x73,
    
    /* f64 operations */
    WASM_OP_F64_ADD = 0xA0,
    WASM_OP_F64_SUB = 0xA1,
    WASM_OP_F64_MUL = 0xA2,
    WASM_OP_F64_DIV = 0xA3
} WasmOpcode;

/* WASM section IDs */
typedef enum {
    WASM_SECTION_CUSTOM = 0,
    WASM_SECTION_TYPE = 1,
    WASM_SECTION_IMPORT = 2,
    WASM_SECTION_FUNCTION = 3,
    WASM_SECTION_TABLE = 4,
    WASM_SECTION_MEMORY = 5,
    WASM_SECTION_GLOBAL = 6,
    WASM_SECTION_EXPORT = 7,
    WASM_SECTION_START = 8,
    WASM_SECTION_ELEMENT = 9,
    WASM_SECTION_CODE = 10,
    WASM_SECTION_DATA = 11
} WasmSectionId;

/* WASM function signature */
typedef struct {
    WasmType *param_types;
    int param_count;
    WasmType *result_types;
    int result_count;
} WasmFunctionType;

/* WASM function */
typedef struct {
    int type_index;
    WasmType *local_types;
    int local_count;
    uint8_t *code;
    int code_size;
} WasmFunction;

/* WASM module */
typedef struct {
    WasmFunctionType *types;
    int type_count;
    
    WasmFunction *functions;
    int function_count;
    
    char **exports;
    int *export_indices;
    int export_count;
    
    int memory_initial;
    int memory_maximum;
    
    uint8_t *data_section;
    int data_size;
} WasmModule;

/* WASM code generator */
typedef struct {
    uint8_t *buffer;
    int size;
    int capacity;
    
    int *local_indices;  /* Map of variable names to local indices */
    int local_count;
    
    int label_count;     /* For generating unique labels */
} WasmCodeGen;

/* WASM module builder */
WasmModule* wasm_module_create(void);
void wasm_module_free(WasmModule *module);

/* Type management */
int wasm_module_add_type(WasmModule *module, WasmType *params, int param_count,
                         WasmType *results, int result_count);

/* Function management */
int wasm_module_add_function(WasmModule *module, int type_index);
void wasm_module_set_function_code(WasmModule *module, int func_index,
                                   WasmType *locals, int local_count,
                                   uint8_t *code, int code_size);

/* Export management */
void wasm_module_add_export(WasmModule *module, const char *name, int func_index);

/* Memory management */
void wasm_module_set_memory(WasmModule *module, int initial, int maximum);

/* Code generation */
WasmCodeGen* wasm_codegen_create(void);
void wasm_codegen_free(WasmCodeGen *gen);
void wasm_codegen_emit_byte(WasmCodeGen *gen, uint8_t byte);
void wasm_codegen_emit_i32(WasmCodeGen *gen, int32_t value);
void wasm_codegen_emit_i64(WasmCodeGen *gen, int64_t value);
void wasm_codegen_emit_f32(WasmCodeGen *gen, float value);
void wasm_codegen_emit_f64(WasmCodeGen *gen, double value);
void wasm_codegen_emit_opcode(WasmCodeGen *gen, WasmOpcode op);

/* Compile AST to WASM */
int wasm_compile_ast(ASTNode *node, WasmModule *module, WasmCodeGen *gen);
int wasm_compile_function(ASTNode *func, WasmModule *module);
int wasm_compile_expression(ASTNode *expr, WasmCodeGen *gen);
int wasm_compile_statement(ASTNode *stmt, WasmCodeGen *gen);

/* Binary format generation */
uint8_t* wasm_module_to_binary(WasmModule *module, int *size);
int wasm_module_write_to_file(WasmModule *module, const char *filename);

/* WAT (WebAssembly Text) format generation */
char* wasm_module_to_wat(WasmModule *module);
int wasm_module_write_wat_to_file(WasmModule *module, const char *filename);

/* JavaScript interop */
char* wasm_generate_js_wrapper(WasmModule *module);

#endif /* WASM_BACKEND_H */
