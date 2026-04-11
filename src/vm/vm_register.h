/**
 * @file vm_v3.h
 * @brief KLang Register-Based Virtual Machine (Version 3.0)
 * 
 * High-performance register-based VM designed to replace the stack-based VM.
 * Target: 10x faster than current stack-based implementation.
 * 
 * Key Features:
 * - 256 registers per call frame (R0-R255)
 * - Fixed 8-byte instruction format
 * - 256 opcodes for comprehensive operations
 * - Call frame management with stack of frames
 * - Constant pool for efficient value storage
 * - Profile-guided execution for JIT optimization
 */

#ifndef VM_REGISTER_H
#define VM_REGISTER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * VALUE SYSTEM
 * ============================================================================ */

/**
 * @brief Value types supported by the VM
 */
typedef enum {
    VALUE_TYPE_INT,         // 64-bit integer
    VALUE_TYPE_FLOAT,       // 64-bit floating point
    VALUE_TYPE_STRING,      // UTF-8 string
    VALUE_TYPE_BOOL,        // Boolean
    VALUE_TYPE_NULL,        // Null/None
    VALUE_TYPE_OBJECT,      // Object/Map
    VALUE_TYPE_ARRAY,       // Array/List
    VALUE_TYPE_FUNCTION,    // Function reference
    VALUE_TYPE_CLOSURE,     // Closure with upvalues
    VALUE_TYPE_EXCEPTION,   // Exception object
} ValueType;

/**
 * @brief Tagged union representing a VM value
 */
typedef struct {
    ValueType type;
    union {
        int64_t as_int;
        double as_float;
        char *as_string;
        bool as_bool;
        void *as_object;    // Generic object pointer
        void *as_array;     // Array pointer
        void *as_function;  // Function pointer
    } data;
} Value;

/* ============================================================================
 * INSTRUCTION FORMAT
 * ============================================================================ */

/**
 * @brief Fixed 8-byte instruction format
 * 
 * Layout:
 * [1 byte opcode][1 byte dest][1 byte src1][1 byte src2][4 bytes immediate]
 */
typedef struct {
    uint8_t opcode;        // Operation code
    uint8_t dest;          // Destination register (R0-R255)
    uint8_t src1;          // Source register 1 (R0-R255)
    uint8_t src2;          // Source register 2 (R0-R255)
    int32_t immediate;     // Immediate value / constant pool index / offset
} Instruction;

/* ============================================================================
 * OPCODES (256 total)
 * ============================================================================ */

typedef enum {
    /* Control (0x00-0x0F) */
    OP_NOP              = 0x00,  // No operation
    OP_HALT             = 0x01,  // Stop execution
    
    /* Arithmetic - Integer (0x10-0x1F) */
    OP_ADD_I            = 0x10,  // R[d] = R[s1] + R[s2]
    OP_SUB_I            = 0x11,  // R[d] = R[s1] - R[s2]
    OP_MUL_I            = 0x12,  // R[d] = R[s1] * R[s2]
    OP_DIV_I            = 0x13,  // R[d] = R[s1] / R[s2]
    OP_MOD_I            = 0x14,  // R[d] = R[s1] % R[s2]
    OP_NEG_I            = 0x15,  // R[d] = -R[s1]
    OP_ABS_I            = 0x16,  // R[d] = abs(R[s1])
    OP_INC              = 0x17,  // R[d] = R[s1] + 1
    OP_DEC              = 0x18,  // R[d] = R[s1] - 1
    OP_ADD_IMM          = 0x19,  // R[d] = R[s1] + imm
    OP_MUL_IMM          = 0x1A,  // R[d] = R[s1] * imm
    
    /* Arithmetic - Float (0x20-0x2F) */
    OP_ADD_F            = 0x20,  // R[d] = R[s1] + R[s2] (float)
    OP_SUB_F            = 0x21,  // R[d] = R[s1] - R[s2] (float)
    OP_MUL_F            = 0x22,  // R[d] = R[s1] * R[s2] (float)
    OP_DIV_F            = 0x23,  // R[d] = R[s1] / R[s2] (float)
    OP_NEG_F            = 0x24,  // R[d] = -R[s1] (float)
    OP_ABS_F            = 0x25,  // R[d] = abs(R[s1]) (float)
    OP_POW_F            = 0x26,  // R[d] = R[s1] ** R[s2] (float)
    
    /* Comparison - Integer (0x30-0x3F) */
    OP_EQ_I             = 0x30,  // R[d] = R[s1] == R[s2]
    OP_NE_I             = 0x31,  // R[d] = R[s1] != R[s2]
    OP_LT_I             = 0x32,  // R[d] = R[s1] < R[s2]
    OP_LE_I             = 0x33,  // R[d] = R[s1] <= R[s2]
    OP_GT_I             = 0x34,  // R[d] = R[s1] > R[s2]
    OP_GE_I             = 0x35,  // R[d] = R[s1] >= R[s2]
    
    /* Comparison - Float (0x36-0x3B) */
    OP_EQ_F             = 0x36,  // R[d] = R[s1] == R[s2] (float)
    OP_NE_F             = 0x37,  // R[d] = R[s1] != R[s2] (float)
    OP_LT_F             = 0x38,  // R[d] = R[s1] < R[s2] (float)
    OP_LE_F             = 0x39,  // R[d] = R[s1] <= R[s2] (float)
    OP_GT_F             = 0x3A,  // R[d] = R[s1] > R[s2] (float)
    OP_GE_F             = 0x3B,  // R[d] = R[s1] >= R[s2] (float)
    
    /* Logical/Bitwise (0x40-0x4F) */
    OP_AND              = 0x40,  // R[d] = R[s1] & R[s2] (bitwise)
    OP_OR               = 0x41,  // R[d] = R[s1] | R[s2] (bitwise)
    OP_XOR              = 0x42,  // R[d] = R[s1] ^ R[s2] (bitwise)
    OP_NOT              = 0x43,  // R[d] = ~R[s1] (bitwise)
    OP_SHL              = 0x44,  // R[d] = R[s1] << R[s2]
    OP_SHR              = 0x45,  // R[d] = R[s1] >> R[s2] (logical)
    OP_SAR              = 0x46,  // R[d] = R[s1] >> R[s2] (arithmetic)
    OP_AND_B            = 0x47,  // R[d] = R[s1] && R[s2] (logical)
    OP_OR_B             = 0x48,  // R[d] = R[s1] || R[s2] (logical)
    OP_NOT_B            = 0x49,  // R[d] = !R[s1] (logical)
    
    /* Memory Operations (0x50-0x6F) */
    OP_MOVE             = 0x50,  // R[d] = R[s1]
    OP_LOAD_I           = 0x51,  // R[d] = imm (int)
    OP_LOAD_F           = 0x52,  // R[d] = float(imm)
    OP_LOAD_TRUE        = 0x53,  // R[d] = true
    OP_LOAD_FALSE       = 0x54,  // R[d] = false
    OP_LOAD_NULL        = 0x55,  // R[d] = null
    OP_LOAD_CONST       = 0x56,  // R[d] = const_pool[imm]
    OP_LOAD_LOCAL       = 0x57,  // R[d] = locals[imm]
    OP_STORE_LOCAL      = 0x58,  // locals[imm] = R[s1]
    OP_LOAD_GLOBAL      = 0x59,  // R[d] = globals[imm]
    OP_STORE_GLOBAL     = 0x5A,  // globals[imm] = R[s1]
    OP_LOAD_UPVALUE     = 0x5B,  // R[d] = upvalues[imm]
    OP_STORE_UPVALUE    = 0x5C,  // upvalues[imm] = R[s1]
    OP_SWAP             = 0x5D,  // swap(R[d], R[s1])
    
    /* Control Flow (0x70-0x7F) */
    OP_JUMP             = 0x70,  // ip = imm
    OP_JUMP_IF_TRUE     = 0x71,  // if (R[s1]) ip = imm
    OP_JUMP_IF_FALSE    = 0x72,  // if (!R[s1]) ip = imm
    OP_JUMP_IF_NULL     = 0x73,  // if (R[s1] == null) ip = imm
    OP_JUMP_IF_NOT_NULL = 0x74,  // if (R[s1] != null) ip = imm
    OP_CALL             = 0x75,  // Call function
    OP_CALL_BUILTIN     = 0x76,  // Call builtin function
    OP_CALL_METHOD      = 0x77,  // Call object method
    OP_RETURN           = 0x78,  // Return from function
    OP_RETURN_VALUE     = 0x79,  // Return R[d]
    
    /* Object Operations (0x80-0x8F) */
    OP_NEW_OBJECT       = 0x80,  // R[d] = {} (empty object)
    OP_NEW_ARRAY        = 0x81,  // R[d] = [] (empty array)
    OP_NEW_ARRAY_SIZE   = 0x82,  // R[d] = new Array(R[s1])
    OP_GET_FIELD        = 0x83,  // R[d] = R[s1].field[imm]
    OP_SET_FIELD        = 0x84,  // R[d].field[imm] = R[s1]
    OP_HAS_FIELD        = 0x85,  // R[d] = hasField(R[s1], imm)
    OP_DELETE_FIELD     = 0x86,  // delete R[s1].field[imm]
    OP_TYPEOF           = 0x87,  // R[d] = typeof(R[s1])
    
    /* Array Operations (0x90-0x9F) */
    OP_ARRAY_GET        = 0x90,  // R[d] = R[s1][R[s2]]
    OP_ARRAY_SET        = 0x91,  // R[d][R[s1]] = R[s2]
    OP_ARRAY_LEN        = 0x92,  // R[d] = R[s1].length
    OP_ARRAY_PUSH       = 0x93,  // R[s1].push(R[s2])
    OP_ARRAY_POP        = 0x94,  // R[d] = R[s1].pop()
    
    /* String Operations (0xA0-0xAF) */
    OP_STR_CONCAT       = 0xA0,  // R[d] = R[s1] + R[s2]
    OP_STR_LEN          = 0xA1,  // R[d] = len(R[s1])
    OP_STR_GET          = 0xA2,  // R[d] = R[s1][R[s2]]
    OP_STR_EQ           = 0xA3,  // R[d] = R[s1] == R[s2] (string)
    
    /* Type Conversion (0xB0-0xBF) */
    OP_INT_TO_FLOAT     = 0xB0,  // R[d] = float(R[s1])
    OP_FLOAT_TO_INT     = 0xB1,  // R[d] = int(R[s1])
    OP_INT_TO_STR       = 0xB2,  // R[d] = str(R[s1])
    OP_FLOAT_TO_STR     = 0xB3,  // R[d] = str(R[s1])
    OP_STR_TO_INT       = 0xB4,  // R[d] = int(R[s1])
    OP_STR_TO_FLOAT     = 0xB5,  // R[d] = float(R[s1])
    OP_BOOL_TO_INT      = 0xB6,  // R[d] = int(R[s1])
    
    /* Advanced String Operations (0xC0-0xCF) */
    OP_STR_SLICE        = 0xC0,  // R[d] = R[s1][start:end]
    OP_STR_SPLIT        = 0xC1,  // R[d] = R[s1].split(R[s2])
    OP_STR_JOIN         = 0xC2,  // R[d] = R[s1].join(R[s2])
    OP_STR_FIND         = 0xC3,  // R[d] = R[s1].find(R[s2])
    OP_STR_REPLACE      = 0xC4,  // R[d] = R[s1].replace(R[s2], ...)
    OP_STR_LOWER        = 0xC5,  // R[d] = R[s1].lower()
    OP_STR_UPPER        = 0xC6,  // R[d] = R[s1].upper()
    OP_STR_TRIM         = 0xC7,  // R[d] = R[s1].trim()
    
    /* Exception Handling (0xD0-0xDF) */
    OP_TRY              = 0xD0,  // Begin try block
    OP_CATCH            = 0xD1,  // Begin catch block
    OP_THROW            = 0xD2,  // Throw exception
    OP_FINALLY          = 0xD3,  // Begin finally block
    OP_END_TRY          = 0xD4,  // End try-catch-finally
    
    /* Advanced Control Flow (0xE0-0xEF) */
    OP_SWITCH           = 0xE0,  // Switch statement
    OP_CASE             = 0xE1,  // Case label
    OP_DEFAULT          = 0xE2,  // Default case
    OP_BREAK            = 0xE3,  // Break from loop/switch
    OP_CONTINUE         = 0xE4,  // Continue loop
    OP_FOR_INIT         = 0xE5,  // For loop initialization
    OP_FOR_COND         = 0xE6,  // For loop condition
    OP_FOR_UPDATE       = 0xE7,  // For loop update
    
    /* Debugging & Profiling (0xF0-0xFF) */
    OP_PRINT_REG        = 0xF0,  // Print register (debug)
    OP_BREAKPOINT       = 0xF1,  // Debugger breakpoint
    OP_PROFILE_ENTER    = 0xF2,  // Enter profiled region
    OP_PROFILE_EXIT     = 0xF3,  // Exit profiled region
    OP_ASSERT           = 0xF4,  // Runtime assertion
    
} Opcode;

/* ============================================================================
 * OBJECT AND ARRAY TYPES
 * ============================================================================ */

/* Forward declarations */
typedef struct ConstantPool ConstantPool;

/**
 * @brief Object field (key-value pair)
 */
typedef struct {
    char *key;          // Field name
    Value value;        // Field value
} ObjectField;

/**
 * @brief Object/Map structure
 */
typedef struct {
    ObjectField *fields; // Array of fields
    int count;          // Number of fields
    int capacity;       // Allocated capacity
} Object;

/**
 * @brief Dynamic array structure
 */
typedef struct {
    Value *elements;    // Array elements
    int count;          // Number of elements
    int capacity;       // Allocated capacity
} Array;

/**
 * @brief Function structure
 */
typedef struct {
    const char *name;      // Function name
    Instruction *bytecode; // Function bytecode
    int bytecode_len;      // Bytecode length
    int param_count;       // Number of parameters
    int local_count;       // Number of local variables
    ConstantPool *constants; // Function constants
} Function;

/**
 * @brief Closure structure (function + captured variables)
 */
typedef struct {
    Function *function;    // Base function
    Value **upvalues;      // Captured variables
    int upvalue_count;     // Number of upvalues
} Closure;

/**
 * @brief Exception handler (try-catch-finally)
 */
typedef struct {
    uint32_t try_ip;       // IP at start of try block
    uint32_t catch_ip;     // IP of catch handler (0 if no catch)
    uint32_t finally_ip;   // IP of finally block (0 if no finally)
    uint32_t end_ip;       // IP after try-catch-finally
    int frame_index;       // Frame this handler belongs to
    bool active;           // Is this handler active?
} ExceptionHandler;

/* ============================================================================
 * CONSTANT POOL
 * ============================================================================ */

/**
 * @brief Constant pool for storing frequently used values
 */
struct ConstantPool {
    Value *values;       // Array of constant values
    int count;          // Number of constants
    int capacity;       // Allocated capacity
};

/* ============================================================================
 * CALL FRAME
 * ============================================================================ */

#define MAX_REGISTERS 256
#define MAX_FRAMES 1024

/**
 * @brief Call frame representing a function activation record
 */
typedef struct {
    Instruction *bytecode;      // Bytecode for this function
    Value registers[MAX_REGISTERS]; // Register file (R0-R255)
    ConstantPool *constants;    // Constant pool for this function
    uint32_t ip;               // Instruction pointer
    uint32_t base_ip;          // Base instruction pointer (for relative jumps)
    
    // Profiling data for JIT compilation
    uint32_t *hotness_counters; // Execution count per instruction
    uint32_t call_count;        // Number of times function called
    
    // Closure support
    Value **upvalues;           // Captured variables
    int upvalue_count;          // Number of upvalues
    
    // Local variable storage (beyond registers)
    Value *locals;              // Local variable array
    int local_count;            // Number of locals
    
    // Debug information
    const char *function_name;  // Function name for stack traces
    int line_number;           // Current line number
} CallFrame;

/* ============================================================================
 * GLOBAL STATE
 * ============================================================================ */

/**
 * @brief Global variable storage
 */
typedef struct {
    char **names;       // Variable names (hash table keys)
    Value *values;      // Variable values
    int count;          // Number of globals
    int capacity;       // Allocated capacity
} GlobalTable;

/* ============================================================================
 * VIRTUAL MACHINE
 * ============================================================================ */

/**
 * @brief Main VM structure
 */
typedef struct {
    CallFrame frames[MAX_FRAMES];  // Call frame stack
    int frame_count;               // Number of active frames
    
    GlobalTable globals;           // Global variable table
    
    // Execution state
    bool running;                  // Is VM running?
    Value return_value;            // Return value from last call
    
    // Exception handling
    ExceptionHandler exception_handlers[MAX_FRAMES]; // Exception handler stack
    int handler_count;             // Number of active handlers
    Value current_exception;       // Current exception being handled
    bool has_exception;           // Is an exception active?
    
    // Profiling and optimization
    uint32_t instruction_count;    // Total instructions executed
    bool profiling_enabled;        // Enable profiling?
    
    // Error handling
    const char *error_message;     // Last error message
    bool has_error;               // Error flag
} VM;

/* ============================================================================
 * API FUNCTIONS
 * ============================================================================ */

/* VM Lifecycle */
VM *vm_v3_new(void);
void vm_v3_free(VM *vm);
void vm_v3_reset(VM *vm);

/* Execution */
int vm_v3_execute(VM *vm, Instruction *bytecode, int bytecode_len);
int vm_v3_call(VM *vm, Value *function, Value *args, int arg_count);

/* Constant Pool Management */
ConstantPool *constant_pool_new(void);
void constant_pool_free(ConstantPool *pool);
int constant_pool_add(ConstantPool *pool, Value value);
Value constant_pool_get(ConstantPool *pool, int index);

/* Value Operations */
Value value_make_int(int64_t val);
Value value_make_float(double val);
Value value_make_string(const char *str);
Value value_make_bool(bool val);
Value value_make_null(void);
Value value_make_object(Object *obj);
Value value_make_array(Array *arr);
Value value_make_function(Function *func);
Value value_make_closure(Closure *closure);
void value_free(Value *val);
bool value_equals(Value *a, Value *b);
bool value_is_truthy(Value *val);
void value_print(Value *val);

/* Object Operations */
Object *object_new(void);
void object_free(Object *obj);
void object_set(Object *obj, const char *key, Value value);
Value object_get(Object *obj, const char *key, bool *found);
bool object_has(Object *obj, const char *key);
bool object_delete(Object *obj, const char *key);
int object_size(Object *obj);

/* Array Operations */
Array *array_new(void);
Array *array_new_with_capacity(int capacity);
void array_free(Array *arr);
void array_push(Array *arr, Value value);
Value array_pop(Array *arr, bool *ok);
Value array_get(Array *arr, int index, bool *ok);
void array_set(Array *arr, int index, Value value);
int array_length(Array *arr);

/* Function Operations */
Function *function_new(const char *name, Instruction *bytecode, int bytecode_len);
void function_free(Function *func);

/* Closure Operations */
Closure *closure_new(Function *func);
void closure_free(Closure *closure);
void closure_set_upvalue(Closure *closure, int index, Value *value);

/* Global Variables */
void global_table_init(GlobalTable *table);
void global_table_free(GlobalTable *table);
int global_table_set(GlobalTable *table, const char *name, Value value);
Value global_table_get(GlobalTable *table, const char *name, bool *found);

/* Debugging */
void vm_v3_dump_registers(VM *vm);
void vm_v3_dump_frame(VM *vm, int frame_index);
const char *opcode_name(uint8_t opcode);
void instruction_disassemble(Instruction *instr, int offset);

/* Error Handling */
void vm_v3_error(VM *vm, const char *format, ...);
const char *vm_v3_get_error(VM *vm);
void vm_v3_clear_error(VM *vm);

/* Register Constants */
#define REG_ZERO        0   // Always zero
#define REG_ONE         1   // Always one
#define REG_THIS        2   // This pointer (for methods)
#define REG_RETURN      3   // Return value
#define REG_SCRATCH_0   4   // Scratch register 0
#define REG_SCRATCH_1   5   // Scratch register 1
#define REG_SCRATCH_2   6   // Scratch register 2
#define REG_SCRATCH_3   7   // Scratch register 3

#endif /* VM_REGISTER_H */
