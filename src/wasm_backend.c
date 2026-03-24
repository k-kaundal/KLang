#include "wasm_backend.h"
#include <stdlib.h>
#include <string.h>

/* WASM magic number and version */
static const uint8_t WASM_MAGIC[] = {0x00, 0x61, 0x73, 0x6D};  /* \0asm */
static const uint8_t WASM_VERSION[] = {0x01, 0x00, 0x00, 0x00};  /* version 1 */

/* Create WASM module */
WasmModule* wasm_module_create(void) {
    WasmModule *module = malloc(sizeof(WasmModule));
    module->types = NULL;
    module->type_count = 0;
    module->functions = NULL;
    module->function_count = 0;
    module->exports = NULL;
    module->export_indices = NULL;
    module->export_count = 0;
    module->memory_initial = 1;  /* 1 page = 64KB */
    module->memory_maximum = 10; /* 10 pages */
    module->data_section = NULL;
    module->data_size = 0;
    return module;
}

/* Free WASM module */
void wasm_module_free(WasmModule *module) {
    if (!module) return;
    
    /* Free types */
    for (int i = 0; i < module->type_count; i++) {
        free(module->types[i].param_types);
        free(module->types[i].result_types);
    }
    free(module->types);
    
    /* Free functions */
    for (int i = 0; i < module->function_count; i++) {
        free(module->functions[i].local_types);
        free(module->functions[i].code);
    }
    free(module->functions);
    
    /* Free exports */
    for (int i = 0; i < module->export_count; i++) {
        free(module->exports[i]);
    }
    free(module->exports);
    free(module->export_indices);
    
    free(module->data_section);
    free(module);
}

/* Add function type */
int wasm_module_add_type(WasmModule *module, WasmType *params, int param_count,
                         WasmType *results, int result_count) {
    module->type_count++;
    module->types = realloc(module->types, module->type_count * sizeof(WasmFunctionType));
    
    WasmFunctionType *type = &module->types[module->type_count - 1];
    type->param_count = param_count;
    type->param_types = malloc(param_count * sizeof(WasmType));
    memcpy(type->param_types, params, param_count * sizeof(WasmType));
    
    type->result_count = result_count;
    type->result_types = malloc(result_count * sizeof(WasmType));
    memcpy(type->result_types, results, result_count * sizeof(WasmType));
    
    return module->type_count - 1;
}

/* Add function */
int wasm_module_add_function(WasmModule *module, int type_index) {
    module->function_count++;
    module->functions = realloc(module->functions, module->function_count * sizeof(WasmFunction));
    
    WasmFunction *func = &module->functions[module->function_count - 1];
    func->type_index = type_index;
    func->local_types = NULL;
    func->local_count = 0;
    func->code = NULL;
    func->code_size = 0;
    
    return module->function_count - 1;
}

/* Set function code */
void wasm_module_set_function_code(WasmModule *module, int func_index,
                                   WasmType *locals, int local_count,
                                   uint8_t *code, int code_size) {
    if (func_index >= module->function_count) return;
    
    WasmFunction *func = &module->functions[func_index];
    
    func->local_count = local_count;
    if (local_count > 0) {
        func->local_types = malloc(local_count * sizeof(WasmType));
        memcpy(func->local_types, locals, local_count * sizeof(WasmType));
    }
    
    func->code_size = code_size;
    func->code = malloc(code_size);
    memcpy(func->code, code, code_size);
}

/* Add export */
void wasm_module_add_export(WasmModule *module, const char *name, int func_index) {
    module->export_count++;
    module->exports = realloc(module->exports, module->export_count * sizeof(char*));
    module->export_indices = realloc(module->export_indices, module->export_count * sizeof(int));
    
    module->exports[module->export_count - 1] = strdup(name);
    module->export_indices[module->export_count - 1] = func_index;
}

/* Set memory */
void wasm_module_set_memory(WasmModule *module, int initial, int maximum) {
    module->memory_initial = initial;
    module->memory_maximum = maximum;
}

/* Code generator */
WasmCodeGen* wasm_codegen_create(void) {
    WasmCodeGen *gen = malloc(sizeof(WasmCodeGen));
    gen->capacity = 1024;
    gen->buffer = malloc(gen->capacity);
    gen->size = 0;
    gen->local_indices = NULL;
    gen->local_count = 0;
    gen->label_count = 0;
    return gen;
}

void wasm_codegen_free(WasmCodeGen *gen) {
    if (!gen) return;
    free(gen->buffer);
    free(gen->local_indices);
    free(gen);
}

void wasm_codegen_emit_byte(WasmCodeGen *gen, uint8_t byte) {
    if (gen->size >= gen->capacity) {
        gen->capacity *= 2;
        gen->buffer = realloc(gen->buffer, gen->capacity);
    }
    gen->buffer[gen->size++] = byte;
}

void wasm_codegen_emit_opcode(WasmCodeGen *gen, WasmOpcode op) {
    wasm_codegen_emit_byte(gen, (uint8_t)op);
}

void wasm_codegen_emit_i32(WasmCodeGen *gen, int32_t value) {
    /* LEB128 encoding for signed integers */
    int more = 1;
    while (more) {
        uint8_t byte = value & 0x7F;
        value >>= 7;
        
        if ((value == 0 && (byte & 0x40) == 0) ||
            (value == -1 && (byte & 0x40) != 0)) {
            more = 0;
        } else {
            byte |= 0x80;
        }
        
        wasm_codegen_emit_byte(gen, byte);
    }
}

void wasm_codegen_emit_i64(WasmCodeGen *gen, int64_t value) {
    /* LEB128 encoding for signed 64-bit integers */
    int more = 1;
    while (more) {
        uint8_t byte = value & 0x7F;
        value >>= 7;
        
        if ((value == 0 && (byte & 0x40) == 0) ||
            (value == -1 && (byte & 0x40) != 0)) {
            more = 0;
        } else {
            byte |= 0x80;
        }
        
        wasm_codegen_emit_byte(gen, byte);
    }
}

void wasm_codegen_emit_f32(WasmCodeGen *gen, float value) {
    uint8_t *bytes = (uint8_t*)&value;
    for (int i = 0; i < 4; i++) {
        wasm_codegen_emit_byte(gen, bytes[i]);
    }
}

void wasm_codegen_emit_f64(WasmCodeGen *gen, double value) {
    uint8_t *bytes = (uint8_t*)&value;
    for (int i = 0; i < 8; i++) {
        wasm_codegen_emit_byte(gen, bytes[i]);
    }
}

/* Compile expression to WASM */
int wasm_compile_expression(ASTNode *expr, WasmCodeGen *gen) {
    if (!expr) return -1;
    
    /* Placeholder - would need full AST node type mapping */
    (void)gen;
    
    /* This would compile different expression types:
     * - Numbers (NODE_NUMBER)
     * - Binary operations (NODE_BINOP)
     * - Function calls (NODE_CALL)
     * - etc.
     */
    
    return 0;
}

/* Generate JavaScript wrapper */
char* wasm_generate_js_wrapper(WasmModule *module) {
    static char buffer[4096];
    int offset = 0;
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "// Auto-generated JavaScript wrapper for KLang WASM module\n\n");
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "async function loadKLangModule() {\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "  const response = await fetch('module.wasm');\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "  const bytes = await response.arrayBuffer();\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "  const module = await WebAssembly.instantiate(bytes);\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "  return module.instance.exports;\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "}\n\n");
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "// Usage:\n");
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "// const klang = await loadKLangModule();\n");
    
    for (int i = 0; i < module->export_count; i++) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset,
            "// klang.%s();\n", module->exports[i]);
    }
    
    return strdup(buffer);
}

/* Convert module to binary */
uint8_t* wasm_module_to_binary(WasmModule *module, int *size) {
    /* Simplified binary generation - would need full implementation */
    *size = 8;  /* Just magic + version for now */
    uint8_t *binary = malloc(*size);
    
    memcpy(binary, WASM_MAGIC, 4);
    memcpy(binary + 4, WASM_VERSION, 4);
    
    return binary;
}

/* Write module to file */
int wasm_module_write_to_file(WasmModule *module, const char *filename) {
    int size;
    uint8_t *binary = wasm_module_to_binary(module, &size);
    
    FILE *f = fopen(filename, "wb");
    if (!f) {
        free(binary);
        return -1;
    }
    
    fwrite(binary, 1, size, f);
    fclose(f);
    free(binary);
    
    return 0;
}
