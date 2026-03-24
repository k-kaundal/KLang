#ifndef VM_STACK_H
#define VM_STACK_H

typedef enum {
    OP_PUSH_INT, OP_PUSH_FLOAT, OP_PUSH_STR, OP_PUSH_BOOL, OP_PUSH_NULL,
    OP_POP, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_NEG, OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_AND, OP_OR, OP_NOT,
    OP_LOAD, OP_STORE, OP_CALL, OP_RETURN,
    OP_JUMP, OP_JUMP_IF_FALSE, OP_JUMP_IF_TRUE,
    OP_PRINT, OP_HALT, OP_MAKE_LIST, OP_INDEX_GET, OP_INDEX_SET, OP_DUP
} Opcode;

typedef struct {
    Opcode op;
    union {
        long long ival;
        double dval;
        char *sval;
    } operand;
} Instruction;

typedef struct {
    Instruction *code;
    int count;
    int capacity;
} Chunk;

#define VM_STACK_MAX 1024

typedef struct {
    long long stack[VM_STACK_MAX];
    int stack_top;
    Chunk *chunk;
    int ip;
} VM;

Chunk *chunk_new(void);
void chunk_free(Chunk *chunk);
void chunk_emit_op(Chunk *chunk, Opcode op);
void chunk_emit_int(Chunk *chunk, Opcode op, long long val);
void chunk_emit_float(Chunk *chunk, Opcode op, double val);
void chunk_emit_str(Chunk *chunk, Opcode op, const char *val);

VM *vm_new(void);
void vm_free(VM *vm);
int vm_execute(VM *vm, Chunk *chunk);

#endif
