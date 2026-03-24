/**
 * @file vm_v3.c
 * @brief KLang Register-Based Virtual Machine Implementation (Version 3.0)
 * 
 * High-performance register-based VM with fixed 8-byte instruction format.
 * Designed for 10x performance improvement over stack-based VM.
 */

#define _POSIX_C_SOURCE 200809L

#include "vm_register.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

/* ============================================================================
 * FORWARD DECLARATIONS
 * ============================================================================ */

static void execute_instruction(VM *vm, CallFrame *frame, Instruction *instr);
static void init_special_registers(CallFrame *frame);

/* ============================================================================
 * VALUE OPERATIONS
 * ============================================================================ */

/**
 * @brief Create an integer value
 */
Value value_make_int(int64_t val) {
    Value v;
    v.type = VALUE_TYPE_INT;
    v.data.as_int = val;
    return v;
}

/**
 * @brief Create a float value
 */
Value value_make_float(double val) {
    Value v;
    v.type = VALUE_TYPE_FLOAT;
    v.data.as_float = val;
    return v;
}

/**
 * @brief Create a string value
 */
Value value_make_string(const char *str) {
    Value v;
    v.type = VALUE_TYPE_STRING;
    v.data.as_string = strdup(str);
    return v;
}

/**
 * @brief Create a boolean value
 */
Value value_make_bool(bool val) {
    Value v;
    v.type = VALUE_TYPE_BOOL;
    v.data.as_bool = val;
    return v;
}

/**
 * @brief Create a null value
 */
Value value_make_null(void) {
    Value v;
    v.type = VALUE_TYPE_NULL;
    v.data.as_object = NULL;
    return v;
}

/**
 * @brief Free value memory (for heap-allocated data)
 */
static void vm_value_free(Value *val) {
    if (val->type == VALUE_TYPE_STRING && val->data.as_string) {
        free(val->data.as_string);
        val->data.as_string = NULL;
    }
    // TODO: Free objects, arrays, closures when implemented
}

/**
 * @brief Check if two values are equal
 */
bool value_equals(Value *a, Value *b) {
    if (a->type != b->type) {
        return false;
    }
    
    switch (a->type) {
        case VALUE_TYPE_INT:
            return a->data.as_int == b->data.as_int;
        case VALUE_TYPE_FLOAT:
            return a->data.as_float == b->data.as_float;
        case VALUE_TYPE_BOOL:
            return a->data.as_bool == b->data.as_bool;
        case VALUE_TYPE_NULL:
            return true;
        case VALUE_TYPE_STRING:
            if (a->data.as_string == NULL || b->data.as_string == NULL) {
                return a->data.as_string == b->data.as_string;
            }
            return strcmp(a->data.as_string, b->data.as_string) == 0;
        default:
            // For objects/arrays/functions, compare pointers
            return a->data.as_object == b->data.as_object;
    }
}

/**
 * @brief Check if value is truthy
 */
bool value_is_truthy(Value *val) {
    switch (val->type) {
        case VALUE_TYPE_BOOL:
            return val->data.as_bool;
        case VALUE_TYPE_INT:
            return val->data.as_int != 0;
        case VALUE_TYPE_FLOAT:
            return val->data.as_float != 0.0;
        case VALUE_TYPE_NULL:
            return false;
        case VALUE_TYPE_STRING:
            return val->data.as_string != NULL && val->data.as_string[0] != '\0';
        default:
            return true; // Objects, arrays, functions are truthy
    }
}

/**
 * @brief Print value for debugging
 */
static void vm_value_print(Value *val) {
    switch (val->type) {
        case VALUE_TYPE_INT:
            printf("%lld", (long long)val->data.as_int);
            break;
        case VALUE_TYPE_FLOAT:
            printf("%g", val->data.as_float);
            break;
        case VALUE_TYPE_BOOL:
            printf("%s", val->data.as_bool ? "true" : "false");
            break;
        case VALUE_TYPE_NULL:
            printf("null");
            break;
        case VALUE_TYPE_STRING:
            printf("%s", val->data.as_string ? val->data.as_string : "(null)");
            break;
        case VALUE_TYPE_OBJECT:
            printf("<object %p>", val->data.as_object);
            break;
        case VALUE_TYPE_ARRAY:
            printf("<array %p>", val->data.as_array);
            break;
        case VALUE_TYPE_FUNCTION:
            printf("<function %p>", val->data.as_function);
            break;
        default:
            printf("<unknown type %d>", val->type);
    }
}

/* ============================================================================
 * CONSTANT POOL
 * ============================================================================ */

/**
 * @brief Create a new constant pool
 */
ConstantPool *constant_pool_new(void) {
    ConstantPool *pool = calloc(1, sizeof(ConstantPool));
    if (!pool) {
        return NULL;
    }
    pool->values = NULL;
    pool->count = 0;
    pool->capacity = 0;
    return pool;
}

/**
 * @brief Free constant pool
 */
void constant_pool_free(ConstantPool *pool) {
    if (!pool) {
        return;
    }
    
    // Free all string values
    for (int i = 0; i < pool->count; i++) {
        vm_value_free(&pool->values[i]);
    }
    
    free(pool->values);
    free(pool);
}

/**
 * @brief Add a value to the constant pool
 * 
 * Deduplicates constants - returns existing index if value already exists.
 */
int constant_pool_add(ConstantPool *pool, Value value) {
    if (!pool) {
        return -1;
    }
    
    // Check for existing constant (deduplication)
    for (int i = 0; i < pool->count; i++) {
        if (value_equals(&pool->values[i], &value)) {
            return i;
        }
    }
    
    // Grow capacity if needed
    if (pool->count >= pool->capacity) {
        int new_capacity = pool->capacity == 0 ? 8 : pool->capacity * 2;
        Value *new_values = realloc(pool->values, new_capacity * sizeof(Value));
        if (!new_values) {
            return -1;
        }
        pool->values = new_values;
        pool->capacity = new_capacity;
    }
    
    // Add constant
    pool->values[pool->count] = value;
    return pool->count++;
}

/**
 * @brief Get a constant from the pool
 */
Value constant_pool_get(ConstantPool *pool, int index) {
    if (!pool || index < 0 || index >= pool->count) {
        return value_make_null();
    }
    return pool->values[index];
}

/* ============================================================================
 * GLOBAL VARIABLE TABLE
 * ============================================================================ */

/**
 * @brief Initialize global table
 */
void global_table_init(GlobalTable *table) {
    table->names = NULL;
    table->values = NULL;
    table->count = 0;
    table->capacity = 0;
}

/**
 * @brief Free global table
 */
void global_table_free(GlobalTable *table) {
    for (int i = 0; i < table->count; i++) {
        free(table->names[i]);
        vm_value_free(&table->values[i]);
    }
    free(table->names);
    free(table->values);
    global_table_init(table);
}

/**
 * @brief Set a global variable
 */
int global_table_set(GlobalTable *table, const char *name, Value value) {
    // Check if variable already exists
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->names[i], name) == 0) {
            vm_value_free(&table->values[i]);
            table->values[i] = value;
            return i;
        }
    }
    
    // Add new variable
    if (table->count >= table->capacity) {
        int new_capacity = table->capacity == 0 ? 16 : table->capacity * 2;
        char **new_names = realloc(table->names, new_capacity * sizeof(char *));
        Value *new_values = realloc(table->values, new_capacity * sizeof(Value));
        if (!new_names || !new_values) {
            return -1;
        }
        table->names = new_names;
        table->values = new_values;
        table->capacity = new_capacity;
    }
    
    table->names[table->count] = strdup(name);
    table->values[table->count] = value;
    return table->count++;
}

/**
 * @brief Get a global variable
 */
Value global_table_get(GlobalTable *table, const char *name, bool *found) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->names[i], name) == 0) {
            if (found) *found = true;
            return table->values[i];
        }
    }
    
    if (found) *found = false;
    return value_make_null();
}

/* ============================================================================
 * VM LIFECYCLE
 * ============================================================================ */

/**
 * @brief Create a new VM instance
 */
VM *vm_v3_new(void) {
    VM *vm = calloc(1, sizeof(VM));
    if (!vm) {
        return NULL;
    }
    
    vm->frame_count = 0;
    global_table_init(&vm->globals);
    vm->running = false;
    vm->return_value = value_make_null();
    vm->instruction_count = 0;
    vm->profiling_enabled = false;
    vm->has_error = false;
    vm->error_message = NULL;
    
    return vm;
}

/**
 * @brief Free VM instance
 */
void vm_v3_free(VM *vm) {
    if (!vm) {
        return;
    }
    
    global_table_free(&vm->globals);
    vm_value_free(&vm->return_value);
    free(vm);
}

/**
 * @brief Reset VM state
 */
void vm_v3_reset(VM *vm) {
    if (!vm) {
        return;
    }
    
    vm->frame_count = 0;
    vm->running = false;
    vm_value_free(&vm->return_value);
    vm->return_value = value_make_null();
    vm->instruction_count = 0;
    vm_v3_clear_error(vm);
}

/* ============================================================================
 * ERROR HANDLING
 * ============================================================================ */

/**
 * @brief Report an error
 */
void vm_v3_error(VM *vm, const char *format, ...) {
    static char error_buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(error_buffer, sizeof(error_buffer), format, args);
    va_end(args);
    
    vm->has_error = true;
    vm->error_message = error_buffer;
    vm->running = false;
}

/**
 * @brief Get last error message
 */
const char *vm_v3_get_error(VM *vm) {
    return vm->has_error ? vm->error_message : NULL;
}

/**
 * @brief Clear error state
 */
void vm_v3_clear_error(VM *vm) {
    vm->has_error = false;
    vm->error_message = NULL;
}

/* ============================================================================
 * SPECIAL REGISTER INITIALIZATION
 * ============================================================================ */

/**
 * @brief Initialize special registers (R0, R1, etc.)
 */
static void init_special_registers(CallFrame *frame) {
    frame->registers[REG_ZERO] = value_make_int(0);
    frame->registers[REG_ONE] = value_make_int(1);
    frame->registers[REG_THIS] = value_make_null();
    frame->registers[REG_RETURN] = value_make_null();
}

/* ============================================================================
 * INSTRUCTION EXECUTION
 * ============================================================================ */

/**
 * @brief Execute a single instruction
 */
static void execute_instruction(VM *vm, CallFrame *frame, Instruction *instr) {
    Value *regs = frame->registers;
    
    switch (instr->opcode) {
        /* ===== CONTROL ===== */
        case OP_NOP:
            // No operation
            break;
            
        case OP_HALT:
            vm->running = false;
            break;
        
        /* ===== ARITHMETIC - INTEGER ===== */
        case OP_ADD_I:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int + regs[instr->src2].data.as_int
            );
            break;
            
        case OP_SUB_I:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int - regs[instr->src2].data.as_int
            );
            break;
            
        case OP_MUL_I:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int * regs[instr->src2].data.as_int
            );
            break;
            
        case OP_DIV_I:
            if (regs[instr->src2].data.as_int == 0) {
                vm_v3_error(vm, "Division by zero");
                return;
            }
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int / regs[instr->src2].data.as_int
            );
            break;
            
        case OP_MOD_I:
            if (regs[instr->src2].data.as_int == 0) {
                vm_v3_error(vm, "Modulo by zero");
                return;
            }
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int % regs[instr->src2].data.as_int
            );
            break;
            
        case OP_NEG_I:
            regs[instr->dest] = value_make_int(-regs[instr->src1].data.as_int);
            break;
            
        case OP_ABS_I: {
            int64_t val = regs[instr->src1].data.as_int;
            regs[instr->dest] = value_make_int(val < 0 ? -val : val);
            break;
        }
            
        case OP_INC:
            regs[instr->dest] = value_make_int(regs[instr->src1].data.as_int + 1);
            break;
            
        case OP_DEC:
            regs[instr->dest] = value_make_int(regs[instr->src1].data.as_int - 1);
            break;
            
        case OP_ADD_IMM:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int + instr->immediate
            );
            break;
            
        case OP_MUL_IMM:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int * instr->immediate
            );
            break;
        
        /* ===== ARITHMETIC - FLOAT ===== */
        case OP_ADD_F:
            regs[instr->dest] = value_make_float(
                regs[instr->src1].data.as_float + regs[instr->src2].data.as_float
            );
            break;
            
        case OP_SUB_F:
            regs[instr->dest] = value_make_float(
                regs[instr->src1].data.as_float - regs[instr->src2].data.as_float
            );
            break;
            
        case OP_MUL_F:
            regs[instr->dest] = value_make_float(
                regs[instr->src1].data.as_float * regs[instr->src2].data.as_float
            );
            break;
            
        case OP_DIV_F:
            if (regs[instr->src2].data.as_float == 0.0) {
                vm_v3_error(vm, "Division by zero");
                return;
            }
            regs[instr->dest] = value_make_float(
                regs[instr->src1].data.as_float / regs[instr->src2].data.as_float
            );
            break;
            
        case OP_NEG_F:
            regs[instr->dest] = value_make_float(-regs[instr->src1].data.as_float);
            break;
            
        case OP_ABS_F:
            regs[instr->dest] = value_make_float(fabs(regs[instr->src1].data.as_float));
            break;
            
        case OP_POW_F:
            regs[instr->dest] = value_make_float(
                pow(regs[instr->src1].data.as_float, regs[instr->src2].data.as_float)
            );
            break;
        
        /* ===== COMPARISON - INTEGER ===== */
        case OP_EQ_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int == regs[instr->src2].data.as_int
            );
            break;
            
        case OP_NE_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int != regs[instr->src2].data.as_int
            );
            break;
            
        case OP_LT_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int < regs[instr->src2].data.as_int
            );
            break;
            
        case OP_LE_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int <= regs[instr->src2].data.as_int
            );
            break;
            
        case OP_GT_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int > regs[instr->src2].data.as_int
            );
            break;
            
        case OP_GE_I:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_int >= regs[instr->src2].data.as_int
            );
            break;
        
        /* ===== COMPARISON - FLOAT ===== */
        case OP_EQ_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float == regs[instr->src2].data.as_float
            );
            break;
            
        case OP_NE_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float != regs[instr->src2].data.as_float
            );
            break;
            
        case OP_LT_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float < regs[instr->src2].data.as_float
            );
            break;
            
        case OP_LE_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float <= regs[instr->src2].data.as_float
            );
            break;
            
        case OP_GT_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float > regs[instr->src2].data.as_float
            );
            break;
            
        case OP_GE_F:
            regs[instr->dest] = value_make_bool(
                regs[instr->src1].data.as_float >= regs[instr->src2].data.as_float
            );
            break;
        
        /* ===== LOGICAL/BITWISE ===== */
        case OP_AND:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int & regs[instr->src2].data.as_int
            );
            break;
            
        case OP_OR:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int | regs[instr->src2].data.as_int
            );
            break;
            
        case OP_XOR:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int ^ regs[instr->src2].data.as_int
            );
            break;
            
        case OP_NOT:
            regs[instr->dest] = value_make_int(~regs[instr->src1].data.as_int);
            break;
            
        case OP_SHL:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int << regs[instr->src2].data.as_int
            );
            break;
            
        case OP_SHR:
            regs[instr->dest] = value_make_int(
                (uint64_t)regs[instr->src1].data.as_int >> regs[instr->src2].data.as_int
            );
            break;
            
        case OP_SAR:
            regs[instr->dest] = value_make_int(
                regs[instr->src1].data.as_int >> regs[instr->src2].data.as_int
            );
            break;
            
        case OP_AND_B:
            regs[instr->dest] = value_make_bool(
                value_is_truthy(&regs[instr->src1]) && value_is_truthy(&regs[instr->src2])
            );
            break;
            
        case OP_OR_B:
            regs[instr->dest] = value_make_bool(
                value_is_truthy(&regs[instr->src1]) || value_is_truthy(&regs[instr->src2])
            );
            break;
            
        case OP_NOT_B:
            regs[instr->dest] = value_make_bool(!value_is_truthy(&regs[instr->src1]));
            break;
        
        /* ===== MEMORY OPERATIONS ===== */
        case OP_MOVE:
            regs[instr->dest] = regs[instr->src1];
            break;
            
        case OP_LOAD_I:
            regs[instr->dest] = value_make_int(instr->immediate);
            break;
            
        case OP_LOAD_F:
            // Store float bits in immediate and reinterpret
            regs[instr->dest] = value_make_float((double)instr->immediate);
            break;
            
        case OP_LOAD_TRUE:
            regs[instr->dest] = value_make_bool(true);
            break;
            
        case OP_LOAD_FALSE:
            regs[instr->dest] = value_make_bool(false);
            break;
            
        case OP_LOAD_NULL:
            regs[instr->dest] = value_make_null();
            break;
            
        case OP_LOAD_CONST:
            if (frame->constants) {
                regs[instr->dest] = constant_pool_get(frame->constants, instr->immediate);
            } else {
                vm_v3_error(vm, "No constant pool available");
            }
            break;
            
        case OP_LOAD_LOCAL:
            if (frame->locals && instr->immediate < frame->local_count) {
                regs[instr->dest] = frame->locals[instr->immediate];
            } else {
                vm_v3_error(vm, "Invalid local variable index: %d", instr->immediate);
            }
            break;
            
        case OP_STORE_LOCAL:
            if (frame->locals && instr->immediate < frame->local_count) {
                frame->locals[instr->immediate] = regs[instr->src1];
            } else {
                vm_v3_error(vm, "Invalid local variable index: %d", instr->immediate);
            }
            break;
            
        case OP_LOAD_GLOBAL: {
            // For now, use immediate as index directly (simplified)
            // TODO: Implement proper global name lookup
            if (instr->immediate < vm->globals.count) {
                regs[instr->dest] = vm->globals.values[instr->immediate];
            } else {
                vm_v3_error(vm, "Invalid global variable index: %d", instr->immediate);
            }
            break;
        }
            
        case OP_STORE_GLOBAL:
            if (instr->immediate < vm->globals.count) {
                vm->globals.values[instr->immediate] = regs[instr->src1];
            } else {
                vm_v3_error(vm, "Invalid global variable index: %d", instr->immediate);
            }
            break;
            
        case OP_SWAP: {
            Value temp = regs[instr->dest];
            regs[instr->dest] = regs[instr->src1];
            regs[instr->src1] = temp;
            break;
        }
        
        /* ===== CONTROL FLOW ===== */
        case OP_JUMP:
            frame->ip = instr->immediate;
            break;
            
        case OP_JUMP_IF_TRUE:
            if (value_is_truthy(&regs[instr->src1])) {
                frame->ip = instr->immediate;
            }
            break;
            
        case OP_JUMP_IF_FALSE:
            if (!value_is_truthy(&regs[instr->src1])) {
                frame->ip = instr->immediate;
            }
            break;
            
        case OP_JUMP_IF_NULL:
            if (regs[instr->src1].type == VALUE_TYPE_NULL) {
                frame->ip = instr->immediate;
            }
            break;
            
        case OP_JUMP_IF_NOT_NULL:
            if (regs[instr->src1].type != VALUE_TYPE_NULL) {
                frame->ip = instr->immediate;
            }
            break;
            
        case OP_RETURN:
            // Pop frame (handled in main execution loop)
            vm->return_value = value_make_null();
            break;
            
        case OP_RETURN_VALUE:
            vm->return_value = regs[instr->dest];
            break;
        
        /* ===== TYPE CONVERSION ===== */
        case OP_INT_TO_FLOAT:
            regs[instr->dest] = value_make_float((double)regs[instr->src1].data.as_int);
            break;
            
        case OP_FLOAT_TO_INT:
            regs[instr->dest] = value_make_int((int64_t)regs[instr->src1].data.as_float);
            break;
            
        case OP_INT_TO_STR: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%lld", (long long)regs[instr->src1].data.as_int);
            regs[instr->dest] = value_make_string(buffer);
            break;
        }
            
        case OP_FLOAT_TO_STR: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%g", regs[instr->src1].data.as_float);
            regs[instr->dest] = value_make_string(buffer);
            break;
        }
            
        case OP_BOOL_TO_INT:
            regs[instr->dest] = value_make_int(regs[instr->src1].data.as_bool ? 1 : 0);
            break;
        
        /* ===== STRING OPERATIONS ===== */
        case OP_STR_CONCAT: {
            if (regs[instr->src1].type == VALUE_TYPE_STRING &&
                regs[instr->src2].type == VALUE_TYPE_STRING) {
                const char *s1 = regs[instr->src1].data.as_string;
                const char *s2 = regs[instr->src2].data.as_string;
                size_t len = strlen(s1) + strlen(s2) + 1;
                char *result = malloc(len);
                if (result) {
                    strcpy(result, s1);
                    strcat(result, s2);
                    regs[instr->dest].type = VALUE_TYPE_STRING;
                    regs[instr->dest].data.as_string = result;
                }
            } else {
                vm_v3_error(vm, "String concatenation requires string operands");
            }
            break;
        }
            
        case OP_STR_LEN:
            if (regs[instr->src1].type == VALUE_TYPE_STRING) {
                regs[instr->dest] = value_make_int(
                    strlen(regs[instr->src1].data.as_string)
                );
            } else {
                regs[instr->dest] = value_make_int(0);
            }
            break;
            
        case OP_STR_EQ:
            if (regs[instr->src1].type == VALUE_TYPE_STRING &&
                regs[instr->src2].type == VALUE_TYPE_STRING) {
                regs[instr->dest] = value_make_bool(
                    strcmp(regs[instr->src1].data.as_string,
                           regs[instr->src2].data.as_string) == 0
                );
            } else {
                regs[instr->dest] = value_make_bool(false);
            }
            break;
        
        /* ===== DEBUGGING ===== */
        case OP_PRINT_REG:
            printf("R%d = ", instr->dest);
            vm_value_print(&regs[instr->dest]);
            printf("\n");
            break;
            
        case OP_BREAKPOINT:
            printf("BREAKPOINT at ip=%d\n", frame->ip - 1);
            // TODO: Hook into debugger
            break;
        
        /* ===== UNIMPLEMENTED/PLACEHOLDER ===== */
        case OP_NEW_OBJECT:
        case OP_NEW_ARRAY:
        case OP_NEW_ARRAY_SIZE:
        case OP_GET_FIELD:
        case OP_SET_FIELD:
        case OP_HAS_FIELD:
        case OP_DELETE_FIELD:
        case OP_TYPEOF:
        case OP_ARRAY_GET:
        case OP_ARRAY_SET:
        case OP_ARRAY_LEN:
        case OP_ARRAY_PUSH:
        case OP_ARRAY_POP:
        case OP_STR_GET:
        case OP_STR_TO_INT:
        case OP_STR_TO_FLOAT:
        case OP_CALL:
        case OP_CALL_BUILTIN:
        case OP_CALL_METHOD:
        case OP_LOAD_UPVALUE:
        case OP_STORE_UPVALUE:
        case OP_PROFILE_ENTER:
        case OP_PROFILE_EXIT:
        case OP_ASSERT:
            vm_v3_error(vm, "Opcode not yet implemented: 0x%02X", instr->opcode);
            break;
        
        default:
            vm_v3_error(vm, "Unknown opcode: 0x%02X", instr->opcode);
            break;
    }
}

/* ============================================================================
 * VM EXECUTION
 * ============================================================================ */

/**
 * @brief Execute bytecode
 */
int vm_v3_execute(VM *vm, Instruction *bytecode, int bytecode_len) {
    if (!vm || !bytecode || bytecode_len <= 0) {
        return -1;
    }
    
    vm_v3_clear_error(vm);
    vm->running = true;
    
    // Initialize first frame if not already set
    if (vm->frame_count == 0) {
        vm->frame_count = 1;
    }
    
    CallFrame *frame = &vm->frames[vm->frame_count - 1];
    
    // Initialize frame if bytecode is new or frame is empty
    if (frame->bytecode != bytecode) {
        ConstantPool *saved_pool = frame->constants;  // Preserve constant pool
        memset(frame, 0, sizeof(CallFrame));
        frame->constants = saved_pool;
    }
    
    frame->bytecode = bytecode;
    frame->ip = 0;
    frame->base_ip = 0;
    frame->hotness_counters = NULL;
    frame->upvalues = NULL;
    frame->upvalue_count = 0;
    frame->locals = NULL;
    frame->local_count = 0;
    frame->function_name = "<main>";
    frame->line_number = 0;
    
    init_special_registers(frame);
    
    // Main execution loop
    while (vm->running && frame->ip < (uint32_t)bytecode_len) {
        Instruction *instr = &bytecode[frame->ip];
        
        // Update profiling counters
        if (vm->profiling_enabled && frame->hotness_counters) {
            frame->hotness_counters[frame->ip]++;
        }
        
        // Advance instruction pointer
        frame->ip++;
        vm->instruction_count++;
        
        // Execute instruction
        execute_instruction(vm, frame, instr);
        
        // Check for errors
        if (vm->has_error) {
            return -1;
        }
        
        // Handle RETURN opcodes
        if (instr->opcode == OP_RETURN || instr->opcode == OP_RETURN_VALUE) {
            vm->frame_count--;
            if (vm->frame_count == 0) {
                // Top-level return - exit
                vm->running = false;
                break;
            }
            // Pop to previous frame
            frame = &vm->frames[vm->frame_count - 1];
        }
    }
    
    return 0;
}

/**
 * @brief Call a function (placeholder - to be fully implemented)
 */
int vm_v3_call(VM *vm, Value *function, Value *args, int arg_count) {
    (void)vm;
    (void)function;
    (void)args;
    (void)arg_count;
    
    // TODO: Implement function calls with new call frames
    return -1;
}

/* ============================================================================
 * DEBUGGING
 * ============================================================================ */

/**
 * @brief Get opcode name for debugging
 */
const char *opcode_name(uint8_t opcode) {
    switch (opcode) {
        case OP_NOP: return "NOP";
        case OP_HALT: return "HALT";
        case OP_ADD_I: return "ADD_I";
        case OP_SUB_I: return "SUB_I";
        case OP_MUL_I: return "MUL_I";
        case OP_DIV_I: return "DIV_I";
        case OP_MOD_I: return "MOD_I";
        case OP_NEG_I: return "NEG_I";
        case OP_ABS_I: return "ABS_I";
        case OP_INC: return "INC";
        case OP_DEC: return "DEC";
        case OP_ADD_IMM: return "ADD_IMM";
        case OP_MUL_IMM: return "MUL_IMM";
        case OP_ADD_F: return "ADD_F";
        case OP_SUB_F: return "SUB_F";
        case OP_MUL_F: return "MUL_F";
        case OP_DIV_F: return "DIV_F";
        case OP_NEG_F: return "NEG_F";
        case OP_ABS_F: return "ABS_F";
        case OP_POW_F: return "POW_F";
        case OP_EQ_I: return "EQ_I";
        case OP_NE_I: return "NE_I";
        case OP_LT_I: return "LT_I";
        case OP_LE_I: return "LE_I";
        case OP_GT_I: return "GT_I";
        case OP_GE_I: return "GE_I";
        case OP_EQ_F: return "EQ_F";
        case OP_NE_F: return "NE_F";
        case OP_LT_F: return "LT_F";
        case OP_LE_F: return "LE_F";
        case OP_GT_F: return "GT_F";
        case OP_GE_F: return "GE_F";
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case OP_XOR: return "XOR";
        case OP_NOT: return "NOT";
        case OP_SHL: return "SHL";
        case OP_SHR: return "SHR";
        case OP_SAR: return "SAR";
        case OP_AND_B: return "AND_B";
        case OP_OR_B: return "OR_B";
        case OP_NOT_B: return "NOT_B";
        case OP_MOVE: return "MOVE";
        case OP_LOAD_I: return "LOAD_I";
        case OP_LOAD_F: return "LOAD_F";
        case OP_LOAD_TRUE: return "LOAD_TRUE";
        case OP_LOAD_FALSE: return "LOAD_FALSE";
        case OP_LOAD_NULL: return "LOAD_NULL";
        case OP_LOAD_CONST: return "LOAD_CONST";
        case OP_LOAD_LOCAL: return "LOAD_LOCAL";
        case OP_STORE_LOCAL: return "STORE_LOCAL";
        case OP_LOAD_GLOBAL: return "LOAD_GLOBAL";
        case OP_STORE_GLOBAL: return "STORE_GLOBAL";
        case OP_SWAP: return "SWAP";
        case OP_JUMP: return "JUMP";
        case OP_JUMP_IF_TRUE: return "JUMP_IF_TRUE";
        case OP_JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case OP_JUMP_IF_NULL: return "JUMP_IF_NULL";
        case OP_JUMP_IF_NOT_NULL: return "JUMP_IF_NOT_NULL";
        case OP_CALL: return "CALL";
        case OP_RETURN: return "RETURN";
        case OP_RETURN_VALUE: return "RETURN_VALUE";
        case OP_INT_TO_FLOAT: return "INT_TO_FLOAT";
        case OP_FLOAT_TO_INT: return "FLOAT_TO_INT";
        case OP_INT_TO_STR: return "INT_TO_STR";
        case OP_FLOAT_TO_STR: return "FLOAT_TO_STR";
        case OP_BOOL_TO_INT: return "BOOL_TO_INT";
        case OP_STR_CONCAT: return "STR_CONCAT";
        case OP_STR_LEN: return "STR_LEN";
        case OP_STR_EQ: return "STR_EQ";
        case OP_PRINT_REG: return "PRINT_REG";
        case OP_BREAKPOINT: return "BREAKPOINT";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Disassemble an instruction
 */
void instruction_disassemble(Instruction *instr, int offset) {
    printf("%04d: %-16s dest=R%-3d src1=R%-3d src2=R%-3d imm=%-8d\n",
           offset,
           opcode_name(instr->opcode),
           instr->dest,
           instr->src1,
           instr->src2,
           instr->immediate);
}

/**
 * @brief Dump all registers in current frame
 */
void vm_v3_dump_registers(VM *vm) {
    if (!vm || vm->frame_count == 0) {
        return;
    }
    
    CallFrame *frame = &vm->frames[vm->frame_count - 1];
    printf("=== Register Dump (Frame %d) ===\n", vm->frame_count - 1);
    
    for (int i = 0; i < MAX_REGISTERS; i++) {
        Value *val = &frame->registers[i];
        
        // Skip uninitialized registers (all null)
        if (i > 10 && val->type == VALUE_TYPE_NULL) {
            continue;
        }
        
        printf("R%-3d: ", i);
        vm_value_print(val);
        printf("\n");
        
        // Only print first few registers by default
        if (i == 15) {
            printf("... (remaining registers omitted)\n");
            break;
        }
    }
}

/**
 * @brief Dump a specific call frame
 */
void vm_v3_dump_frame(VM *vm, int frame_index) {
    if (!vm || frame_index < 0 || frame_index >= vm->frame_count) {
        return;
    }
    
    CallFrame *frame = &vm->frames[frame_index];
    printf("=== Frame %d ===\n", frame_index);
    printf("Function: %s\n", frame->function_name ? frame->function_name : "<unknown>");
    printf("IP: %d\n", frame->ip);
    printf("Line: %d\n", frame->line_number);
    printf("Locals: %d\n", frame->local_count);
    printf("Upvalues: %d\n", frame->upvalue_count);
}
