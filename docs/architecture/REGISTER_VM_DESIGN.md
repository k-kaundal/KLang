# KLang Register-Based Virtual Machine Design
## Detailed Architecture Specification

**Version:** 3.0
**Status:** Design Phase
**Owner:** VM Team

---

## Overview

The KLang Register VM is a high-performance, register-based bytecode virtual machine designed to replace the current stack-based VM. It provides significantly better performance through reduced memory traffic and serves as the foundation for the multi-tier execution system (Interpreter → Baseline JIT → Optimizing JIT).

### Design Goals

1. **Performance**: 10x faster than current stack-based VM
2. **Simplicity**: Clear, maintainable implementation
3. **JIT-Friendly**: Easy to compile to native code
4. **Profile-Guided**: Collect data for JIT optimization decisions
5. **Debuggable**: Support breakpoints, stepping, inspection

---

## Architecture

### Register File

The VM maintains a register file with 256 registers per function frame.

```c
typedef enum {
    VALUE_TYPE_INT,
    VALUE_TYPE_FLOAT,
    VALUE_TYPE_STRING,
    VALUE_TYPE_BOOL,
    VALUE_TYPE_NULL,
    VALUE_TYPE_OBJECT,
    VALUE_TYPE_ARRAY,
    VALUE_TYPE_FUNCTION,
} ValueType;

typedef struct {
    ValueType type;
    union {
        int64_t as_int;
        double as_float;
        char *as_string;
        bool as_bool;
        void *as_object;
    } data;
} Value;

// 256 registers per frame
#define MAX_REGISTERS 256
```

**Register Allocation Convention:**
- R0-R15: Reserved for special purposes
  - R0: Constant zero (optimization)
  - R1: Constant one (optimization)
  - R2: This pointer (for methods)
  - R3: Return value
  - R4-R15: Scratch registers
- R16-R255: General purpose

### Instruction Format

**Fixed 8-byte instruction format:**

```c
typedef struct {
    uint8_t opcode;       // Operation code (1 byte)
    uint8_t dest;         // Destination register (1 byte)
    uint8_t src1;         // Source register 1 (1 byte)
    uint8_t src2;         // Source register 2 (1 byte)
    int32_t immediate;    // Immediate value / constant pool index (4 bytes)
} Instruction;
```

**Advantages:**
- Fixed size simplifies instruction pointer arithmetic
- 4 bytes for immediate values allows encoding small constants inline
- Predictable memory layout improves CPU cache utilization

### Complete Instruction Set (256 Opcodes)

#### 1. Arithmetic Operations (20 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x00 | NOP | - | No operation |
| 0x01 | ADD_I | R[d] = R[s1] + R[s2] | Integer addition |
| 0x02 | ADD_F | R[d] = R[s1] + R[s2] | Float addition |
| 0x03 | SUB_I | R[d] = R[s1] - R[s2] | Integer subtraction |
| 0x04 | SUB_F | R[d] = R[s1] - R[s2] | Float subtraction |
| 0x05 | MUL_I | R[d] = R[s1] * R[s2] | Integer multiplication |
| 0x06 | MUL_F | R[d] = R[s1] * R[s2] | Float multiplication |
| 0x07 | DIV_I | R[d] = R[s1] / R[s2] | Integer division |
| 0x08 | DIV_F | R[d] = R[s1] / R[s2] | Float division |
| 0x09 | MOD_I | R[d] = R[s1] % R[s2] | Integer modulo |
| 0x0A | POW_I | R[d] = R[s1] ** R[s2] | Integer power |
| 0x0B | POW_F | R[d] = R[s1] ** R[s2] | Float power |
| 0x0C | NEG_I | R[d] = -R[s1] | Integer negation |
| 0x0D | NEG_F | R[d] = -R[s1] | Float negation |
| 0x0E | ABS_I | R[d] = abs(R[s1]) | Absolute value (int) |
| 0x0F | ABS_F | R[d] = abs(R[s1]) | Absolute value (float) |
| 0x10 | INC | R[d] = R[s1] + 1 | Increment |
| 0x11 | DEC | R[d] = R[s1] - 1 | Decrement |
| 0x12 | ADD_IMM | R[d] = R[s1] + imm | Add immediate |
| 0x13 | MUL_IMM | R[d] = R[s1] * imm | Multiply immediate |

#### 2. Comparison Operations (12 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x20 | EQ_I | R[d] = R[s1] == R[s2] | Integer equality |
| 0x21 | EQ_F | R[d] = R[s1] == R[s2] | Float equality |
| 0x22 | NE_I | R[d] = R[s1] != R[s2] | Integer not equal |
| 0x23 | NE_F | R[d] = R[s1] != R[s2] | Float not equal |
| 0x24 | LT_I | R[d] = R[s1] < R[s2] | Less than (int) |
| 0x25 | LT_F | R[d] = R[s1] < R[s2] | Less than (float) |
| 0x26 | LE_I | R[d] = R[s1] <= R[s2] | Less or equal (int) |
| 0x27 | LE_F | R[d] = R[s1] <= R[s2] | Less or equal (float) |
| 0x28 | GT_I | R[d] = R[s1] > R[s2] | Greater than (int) |
| 0x29 | GT_F | R[d] = R[s1] > R[s2] | Greater than (float) |
| 0x2A | GE_I | R[d] = R[s1] >= R[s2] | Greater or equal (int) |
| 0x2B | GE_F | R[d] = R[s1] >= R[s2] | Greater or equal (float) |

#### 3. Logical/Bitwise Operations (12 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x30 | AND | R[d] = R[s1] & R[s2] | Bitwise AND |
| 0x31 | OR | R[d] = R[s1] \| R[s2] | Bitwise OR |
| 0x32 | XOR | R[d] = R[s1] ^ R[s2] | Bitwise XOR |
| 0x33 | NOT | R[d] = ~R[s1] | Bitwise NOT |
| 0x34 | SHL | R[d] = R[s1] << R[s2] | Shift left |
| 0x35 | SHR | R[d] = R[s1] >> R[s2] | Shift right (logical) |
| 0x36 | SAR | R[d] = R[s1] >> R[s2] | Shift right (arithmetic) |
| 0x37 | AND_B | R[d] = R[s1] && R[s2] | Logical AND |
| 0x38 | OR_B | R[d] = R[s1] \|\| R[s2] | Logical OR |
| 0x39 | NOT_B | R[d] = !R[s1] | Logical NOT |
| 0x3A | AND_IMM | R[d] = R[s1] & imm | AND immediate |
| 0x3B | OR_IMM | R[d] = R[s1] \| imm | OR immediate |

#### 4. Memory Operations (20 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x40 | MOVE | R[d] = R[s1] | Move register |
| 0x41 | LOAD_I | R[d] = imm | Load immediate int |
| 0x42 | LOAD_F | R[d] = float(imm) | Load immediate float |
| 0x43 | LOAD_TRUE | R[d] = true | Load boolean true |
| 0x44 | LOAD_FALSE | R[d] = false | Load boolean false |
| 0x45 | LOAD_NULL | R[d] = null | Load null |
| 0x46 | LOAD_CONST | R[d] = const_pool[imm] | Load from constant pool |
| 0x47 | LOAD_LOCAL | R[d] = locals[imm] | Load local variable |
| 0x48 | STORE_LOCAL | locals[imm] = R[s1] | Store local variable |
| 0x49 | LOAD_GLOBAL | R[d] = globals[imm] | Load global variable |
| 0x4A | STORE_GLOBAL | globals[imm] = R[s1] | Store global variable |
| 0x4B | LOAD_UPVALUE | R[d] = upvalues[imm] | Load closure upvalue |
| 0x4C | STORE_UPVALUE | upvalues[imm] = R[s1] | Store closure upvalue |
| 0x4D | LOAD_MEM | R[d] = mem[R[s1] + imm] | Load from memory |
| 0x4E | STORE_MEM | mem[R[d] + imm] = R[s1] | Store to memory |
| 0x4F | SWAP | swap(R[d], R[s1]) | Swap registers |

#### 5. Control Flow (15 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x50 | JUMP | ip = imm | Unconditional jump |
| 0x51 | JUMP_IF_TRUE | if (R[s1]) ip = imm | Conditional jump |
| 0x52 | JUMP_IF_FALSE | if (!R[s1]) ip = imm | Conditional jump |
| 0x53 | JUMP_IF_NULL | if (R[s1] == null) ip = imm | Jump if null |
| 0x54 | JUMP_IF_NOT_NULL | if (R[s1] != null) ip = imm | Jump if not null |
| 0x55 | CALL | Call function | Call function |
| 0x56 | CALL_BUILTIN | Call builtin | Call builtin function |
| 0x57 | CALL_METHOD | Call method | Call object method |
| 0x58 | RETURN | Return from function | Return |
| 0x59 | RETURN_VALUE | Return R[d] | Return with value |
| 0x5A | YIELD | Yield R[d] | Yield from generator |
| 0x5B | AWAIT | Await R[s1] | Await promise |
| 0x5C | THROW | Throw exception | Throw error |
| 0x5D | TRY_BEGIN | Begin try block | Try block start |
| 0x5E | TRY_END | End try block | Try block end |

#### 6. Object Operations (25 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x60 | NEW_OBJECT | R[d] = {} | Create empty object |
| 0x61 | NEW_CLASS | R[d] = class | Create class |
| 0x62 | NEW_INSTANCE | R[d] = new R[s1] | Instantiate class |
| 0x63 | GET_FIELD | R[d] = R[s1].field[imm] | Get object property |
| 0x64 | SET_FIELD | R[d].field[imm] = R[s1] | Set object property |
| 0x65 | GET_FIELD_IC | Get field (inline cache) | Fast property access |
| 0x66 | SET_FIELD_IC | Set field (inline cache) | Fast property write |
| 0x67 | HAS_FIELD | R[d] = hasField(R[s1], imm) | Check property exists |
| 0x68 | DELETE_FIELD | delete R[s1].field[imm] | Delete property |
| 0x69 | GET_PROTO | R[d] = R[s1].__proto__ | Get prototype |
| 0x6A | SET_PROTO | R[d].__proto__ = R[s1] | Set prototype |
| 0x6B | INSTANCEOF | R[d] = R[s1] instanceof R[s2] | Type check |
| 0x6C | TYPEOF | R[d] = typeof(R[s1]) | Get type |
| 0x6D | CLONE | R[d] = clone(R[s1]) | Clone object |

#### 7. Array Operations (15 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x70 | NEW_ARRAY | R[d] = [] | Create empty array |
| 0x71 | NEW_ARRAY_SIZE | R[d] = new Array(R[s1]) | Create sized array |
| 0x72 | ARRAY_GET | R[d] = R[s1][R[s2]] | Get array element |
| 0x73 | ARRAY_SET | R[d][R[s1]] = R[s2] | Set array element |
| 0x74 | ARRAY_LEN | R[d] = R[s1].length | Get array length |
| 0x75 | ARRAY_PUSH | R[s1].push(R[s2]) | Append to array |
| 0x76 | ARRAY_POP | R[d] = R[s1].pop() | Remove last element |
| 0x77 | ARRAY_SLICE | R[d] = R[s1].slice(...) | Slice array |
| 0x78 | ARRAY_CONCAT | R[d] = R[s1].concat(R[s2]) | Concatenate arrays |

#### 8. String Operations (12 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x80 | STR_CONCAT | R[d] = R[s1] + R[s2] | Concatenate strings |
| 0x81 | STR_LEN | R[d] = len(R[s1]) | String length |
| 0x82 | STR_GET | R[d] = R[s1][R[s2]] | Get character |
| 0x83 | STR_SLICE | R[d] = R[s1].slice(...) | Substring |
| 0x84 | STR_SPLIT | R[d] = R[s1].split(R[s2]) | Split string |
| 0x85 | STR_JOIN | R[d] = R[s1].join(R[s2]) | Join array to string |
| 0x86 | STR_UPPER | R[d] = R[s1].upper() | To uppercase |
| 0x87 | STR_LOWER | R[d] = R[s1].lower() | To lowercase |
| 0x88 | STR_EQ | R[d] = R[s1] == R[s2] | String equality |
| 0x89 | STR_CONTAINS | R[d] = R[s1].contains(R[s2]) | Contains substring |

#### 9. Type Conversion (10 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0x90 | INT_TO_FLOAT | R[d] = float(R[s1]) | Convert int to float |
| 0x91 | FLOAT_TO_INT | R[d] = int(R[s1]) | Convert float to int |
| 0x92 | INT_TO_STR | R[d] = str(R[s1]) | Convert int to string |
| 0x93 | FLOAT_TO_STR | R[d] = str(R[s1]) | Convert float to string |
| 0x94 | STR_TO_INT | R[d] = int(R[s1]) | Parse string to int |
| 0x95 | STR_TO_FLOAT | R[d] = float(R[s1]) | Parse string to float |
| 0x96 | BOOL_TO_INT | R[d] = int(R[s1]) | Convert bool to int |
| 0x97 | BOOL_TO_STR | R[d] = str(R[s1]) | Convert bool to string |

#### 10. Function Operations (10 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0xA0 | CLOSURE | R[d] = closure(...) | Create closure |
| 0xA1 | BIND_METHOD | R[d] = R[s1].bind(R[s2]) | Bind method to object |
| 0xA2 | CALL_FAST | Fast call | Optimized function call |
| 0xA3 | TAIL_CALL | Tail call | Tail call optimization |
| 0xA4 | APPLY | R[d] = apply(R[s1], R[s2]) | Apply function |

#### 11. Async/Generator Operations (10 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0xB0 | PROMISE_NEW | R[d] = new Promise | Create promise |
| 0xB1 | PROMISE_RESOLVE | R[s1].resolve(R[s2]) | Resolve promise |
| 0xB2 | PROMISE_REJECT | R[s1].reject(R[s2]) | Reject promise |
| 0xB3 | AWAIT | R[d] = await R[s1] | Await promise |
| 0xB4 | GENERATOR_NEW | R[d] = generator | Create generator |
| 0xB5 | YIELD | yield R[d] | Yield value |
| 0xB6 | YIELD_FROM | yield* R[s1] | Delegate yield |

#### 12. Debugging & Profiling (8 opcodes)

| Opcode | Mnemonic | Format | Description |
|--------|----------|--------|-------------|
| 0xF0 | BREAKPOINT | Breakpoint | Debugger breakpoint |
| 0xF1 | PROFILE_ENTER | Profile enter | Enter profiled region |
| 0xF2 | PROFILE_EXIT | Profile exit | Exit profiled region |
| 0xF3 | PRINT_REG | Print register | Debug print |
| 0xF4 | ASSERT | Assert condition | Runtime assertion |
| 0xF5 | CHECKPOINT | GC checkpoint | Suggest GC |

---

## VM Execution Loop

### Core Interpreter Loop

```c
void vm_execute(VM *vm) {
    CallFrame *frame = &vm->frames[vm->frame_count - 1];
    Instruction *bytecode = frame->bytecode;
    Value *registers = frame->registers;
    uint32_t ip = frame->ip;
    
    while (1) {
        Instruction instr = bytecode[ip++];
        
        // Hotness counter for profiling
        frame->hotness_counters[ip - 1]++;
        
        // Check for JIT threshold
        if (frame->hotness_counters[ip - 1] > JIT_THRESHOLD) {
            baseline_jit_compile(frame, ip - 1);
        }
        
        switch (instr.opcode) {
            case OP_ADD_I:
                registers[instr.dest].data.as_int = 
                    registers[instr.src1].data.as_int + 
                    registers[instr.src2].data.as_int;
                break;
                
            case OP_ADD_F:
                registers[instr.dest].data.as_float = 
                    registers[instr.src1].data.as_float + 
                    registers[instr.src2].data.as_float;
                break;
                
            case OP_MUL_I:
                registers[instr.dest].data.as_int = 
                    registers[instr.src1].data.as_int * 
                    registers[instr.src2].data.as_int;
                break;
                
            case OP_LOAD_CONST:
                registers[instr.dest] = frame->constants[instr.immediate];
                break;
                
            case OP_JUMP:
                ip = instr.immediate;
                break;
                
            case OP_JUMP_IF_FALSE:
                if (!registers[instr.src1].data.as_bool) {
                    ip = instr.immediate;
                }
                break;
                
            case OP_CALL:
                // Function call - push new frame
                vm_call(vm, &registers[instr.dest], 
                       instr.src1, // arg start
                       instr.src2  // arg count
                );
                // Update frame after call
                frame = &vm->frames[vm->frame_count - 1];
                bytecode = frame->bytecode;
                registers = frame->registers;
                ip = frame->ip;
                break;
                
            case OP_RETURN:
                // Pop frame
                if (vm->frame_count == 1) {
                    return; // Top-level return
                }
                vm->frame_count--;
                frame = &vm->frames[vm->frame_count - 1];
                bytecode = frame->bytecode;
                registers = frame->registers;
                ip = frame->ip;
                break;
                
            case OP_RETURN_VALUE:
                // Return with value
                Value ret_val = registers[instr.dest];
                vm->frame_count--;
                frame = &vm->frames[vm->frame_count - 1];
                frame->registers[R_RETURN_VALUE] = ret_val;
                bytecode = frame->bytecode;
                registers = frame->registers;
                ip = frame->ip;
                break;
                
            // ... handle remaining 240+ opcodes
                
            default:
                vm_error("Unknown opcode: %d", instr.opcode);
                return;
        }
    }
}
```

---

## Constant Pool

The constant pool stores values that are referenced by bytecode.

```c
typedef struct {
    Value *values;
    int count;
    int capacity;
} ConstantPool;

void constant_pool_init(ConstantPool *pool) {
    pool->values = NULL;
    pool->count = 0;
    pool->capacity = 0;
}

int constant_pool_add(ConstantPool *pool, Value value) {
    // Check if constant already exists (deduplication)
    for (int i = 0; i < pool->count; i++) {
        if (value_equals(&pool->values[i], &value)) {
            return i;
        }
    }
    
    // Add new constant
    if (pool->count >= pool->capacity) {
        pool->capacity = pool->capacity == 0 ? 8 : pool->capacity * 2;
        pool->values = realloc(pool->values, 
                               pool->capacity * sizeof(Value));
    }
    
    pool->values[pool->count] = value;
    return pool->count++;
}
```

---

## Call Frame Management

```c
#define MAX_FRAMES 1024

typedef struct {
    Instruction *bytecode;      // Bytecode for this function
    Value registers[256];       // Register file
    Value *constants;           // Constant pool
    uint32_t ip;               // Instruction pointer
    uint32_t *hotness_counters; // For profiling
    
    // For closures
    Value **upvalues;
    int upvalue_count;
    
    // Debug info
    const char *function_name;
    int line_number;
} CallFrame;

typedef struct {
    CallFrame frames[MAX_FRAMES];
    int frame_count;
    
    // Global state
    HashMap *globals;
    
    // GC state
    GCHeap *heap;
    
    // Module system
    HashMap *modules;
} VM;
```

---

## Performance Optimizations

### 1. Threaded Interpreter

For even better interpreter performance, use computed goto (GCC extension):

```c
#ifdef USE_COMPUTED_GOTO

void vm_execute_threaded(VM *vm) {
    static void *dispatch_table[] = {
        &&OP_ADD_I_LABEL,
        &&OP_ADD_F_LABEL,
        &&OP_MUL_I_LABEL,
        // ... all opcodes
    };
    
    #define DISPATCH() goto *dispatch_table[bytecode[ip++].opcode]
    
    CallFrame *frame = &vm->frames[vm->frame_count - 1];
    Instruction *bytecode = frame->bytecode;
    Value *registers = frame->registers;
    uint32_t ip = frame->ip;
    
    DISPATCH();
    
    OP_ADD_I_LABEL:
        registers[bytecode[ip-1].dest].data.as_int = 
            registers[bytecode[ip-1].src1].data.as_int + 
            registers[bytecode[ip-1].src2].data.as_int;
        DISPATCH();
    
    OP_ADD_F_LABEL:
        registers[bytecode[ip-1].dest].data.as_float = 
            registers[bytecode[ip-1].src1].data.as_float + 
            registers[bytecode[ip-1].src2].data.as_float;
        DISPATCH();
    
    // ... all opcodes
}

#endif
```

**Performance Impact:** 15-25% faster than switch-based dispatch.

### 2. Register Caching

Cache frequently accessed registers in local variables:

```c
void vm_execute_cached(VM *vm) {
    // Cache hot registers in local variables
    Value r0, r1, r2, r3;
    
    // ... execution loop
    
    // Sync back to register file before calls
}
```

### 3. Type-Specialized Opcodes

Separate opcodes for int vs. float avoid runtime type checks:

```c
// Instead of:
case OP_ADD:
    if (registers[src1].type == VALUE_TYPE_INT) {
        // int addition
    } else {
        // float addition
    }
    break;

// Use separate opcodes:
case OP_ADD_I:  // Always int
    registers[dest].data.as_int = 
        registers[src1].data.as_int + registers[src2].data.as_int;
    break;

case OP_ADD_F:  // Always float
    registers[dest].data.as_float = 
        registers[src1].data.as_float + registers[src2].data.as_float;
    break;
```

---

## Debugging Support

### Breakpoints

```c
typedef struct {
    uint32_t ip;               // Instruction pointer
    bool enabled;
    char *condition;           // Optional condition
} Breakpoint;

void vm_add_breakpoint(VM *vm, uint32_t ip) {
    // Insert OP_BREAKPOINT at specified location
    Instruction *instr = &vm->frames[0].bytecode[ip];
    instr->opcode = OP_BREAKPOINT;
}

void vm_handle_breakpoint(VM *vm, uint32_t ip) {
    // Pause execution
    vm->paused = true;
    
    // Notify debugger
    debugger_notify_break(vm, ip);
    
    // Wait for resume command
    while (vm->paused) {
        debugger_process_commands(vm);
    }
}
```

### Stack Traces

```c
void vm_print_stack_trace(VM *vm) {
    for (int i = vm->frame_count - 1; i >= 0; i--) {
        CallFrame *frame = &vm->frames[i];
        printf("  at %s (line %d)\n", 
               frame->function_name, 
               frame->line_number);
    }
}
```

---

## Migration Plan

### Phase 1: Core VM (2 weeks)
- Implement register file and instruction structure
- Implement basic arithmetic/logical opcodes
- Implement control flow opcodes
- Basic execution loop

### Phase 2: Memory & Objects (2 weeks)
- Implement memory load/store opcodes
- Implement object creation and property access
- Implement array operations
- String operations

### Phase 3: Functions & Calls (1 week)
- Implement call frame management
- Function call opcodes
- Closure support
- Return handling

### Phase 4: Advanced Features (1 week)
- Async/await opcodes
- Generator support
- Exception handling
- Debugging support

### Phase 5: Optimization (1 week)
- Threaded interpreter
- Register caching
- Profiling integration
- Performance tuning

### Phase 6: Testing (1 week)
- Comprehensive test suite
- Performance benchmarks
- Comparison with stack VM
- Bug fixes

**Total: 8 weeks**

---

## Success Metrics

- ✅ 10x faster than stack-based VM on microbenchmarks
- ✅ 5x faster than stack-based VM on real programs
- ✅ <2ms startup overhead
- ✅ Support all language features
- ✅ Pass all existing test suite
- ✅ Smooth integration with JIT compilers

---

## Future Enhancements

1. **SIMD Instructions**: Add SIMD opcodes for vectorized operations
2. **Superinstructions**: Combine common opcode sequences
3. **Tracing Interpreter**: Record hot traces for optimization
4. **Adaptive Optimization**: Dynamically choose interpreter vs. JIT

---

## References

- Lua 5.x VM Design
- CPython Bytecode
- JVM Specification
- V8 Ignition Interpreter
- PyPy RPython VM

---

**Next Steps:**
1. Review and approve this design
2. Begin Phase 1 implementation
3. Set up performance benchmarking infrastructure
4. Create detailed opcode specification document
