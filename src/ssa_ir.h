#ifndef SSA_IR_H
#define SSA_IR_H

#include "ast.h"

/* SSA IR Instruction Opcodes */
typedef enum {
    /* Arithmetic Operations */
    IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_MOD,
    /* Logical Operations */
    IR_AND, IR_OR, IR_NOT, IR_XOR,
    /* Comparison Operations */
    IR_EQ, IR_NE, IR_LT, IR_LE, IR_GT, IR_GE,
    /* Memory Operations */
    IR_LOAD, IR_STORE, IR_ALLOCA,
    /* Control Flow */
    IR_BR, IR_CONDBR, IR_PHI, IR_CALL, IR_RET,
    /* Type Operations */
    IR_CAST, IR_TYPEOF, IR_INSTANCEOF,
    /* Constants */
    IR_CONST_INT, IR_CONST_FLOAT, IR_CONST_STR, IR_CONST_BOOL, IR_CONST_NULL,
    /* Other */
    IR_MOVE, IR_PARAM, IR_NOP
} IROpcode;

/* Forward declarations */
typedef struct IRInstruction IRInstruction;
typedef struct BasicBlock BasicBlock;
typedef struct SSAFunction SSAFunction;
typedef struct SSAModule SSAModule;

/* SSA Virtual Register */
typedef struct {
    int id;                  /* Unique register ID */
    char *name;             /* Optional name for debugging */
    int version;            /* SSA version number */
} SSARegister;

/* IR Instruction Operand */
typedef enum {
    OPERAND_REGISTER,
    OPERAND_CONSTANT,
    OPERAND_BLOCK,
    OPERAND_FUNCTION
} OperandType;

typedef struct {
    OperandType type;
    union {
        SSARegister *reg;
        struct {
            enum { CONST_INT, CONST_FLOAT, CONST_STR, CONST_BOOL, CONST_NULL } const_type;
            union {
                long long int_val;
                double float_val;
                char *str_val;
                int bool_val;
            };
        } constant;
        BasicBlock *block;
        SSAFunction *func;
    };
} IROperand;

/* Phi Node Entry */
typedef struct {
    SSARegister *value;     /* Incoming value */
    BasicBlock *block;      /* Incoming block */
} PhiEntry;

/* SSA IR Instruction */
struct IRInstruction {
    IROpcode opcode;
    SSARegister *dest;      /* Destination register (NULL for void) */
    IROperand *operands;    /* Array of operands */
    int operand_count;
    
    /* Phi node specific */
    PhiEntry *phi_entries;
    int phi_count;
    
    /* Debugging */
    int line;
    IRInstruction *next;
};

/* Basic Block */
struct BasicBlock {
    int id;                     /* Unique block ID */
    char *label;               /* Optional label */
    
    IRInstruction *first;       /* First instruction */
    IRInstruction *last;        /* Last instruction */
    
    /* Control Flow Graph */
    BasicBlock **predecessors;
    int pred_count;
    int pred_capacity;
    
    BasicBlock **successors;
    int succ_count;
    int succ_capacity;
    
    /* Dominator Tree */
    BasicBlock *idom;          /* Immediate dominator */
    BasicBlock **dom_children;
    int dom_child_count;
    
    /* Dominance Frontier */
    BasicBlock **dom_frontier;
    int df_count;
    
    /* Analysis data */
    int visited;
    int rpo_num;               /* Reverse post-order number */
};

/* SSA Function */
struct SSAFunction {
    char *name;
    BasicBlock **blocks;
    int block_count;
    int block_capacity;
    
    BasicBlock *entry;         /* Entry block */
    BasicBlock *exit;          /* Exit block */
    
    SSARegister **params;      /* Function parameters */
    int param_count;
    
    SSARegister **registers;   /* All virtual registers */
    int reg_count;
    int reg_capacity;
    
    int next_reg_id;
    int next_block_id;
};

/* SSA Module (top-level) */
struct SSAModule {
    SSAFunction **functions;
    int func_count;
    int func_capacity;
    
    char **global_vars;
    int global_count;
};

/* SSA Construction Context */
typedef struct {
    SSAFunction *current_func;
    BasicBlock *current_block;
    
    /* Variable version tracking for SSA construction */
    struct {
        char *var_name;
        int *versions;        /* Stack of versions for each scope */
        int version_count;
    } *var_versions;
    int var_version_count;
    
    /* Break/Continue targets */
    BasicBlock *break_target;
    BasicBlock *continue_target;
} SSAContext;

/* === API Functions === */

/* Module management */
SSAModule *ssa_module_create(void);
void ssa_module_free(SSAModule *module);
void ssa_module_add_function(SSAModule *module, SSAFunction *func);

/* Function management */
SSAFunction *ssa_function_create(const char *name);
void ssa_function_free(SSAFunction *func);
BasicBlock *ssa_function_add_block(SSAFunction *func, const char *label);

/* Basic Block management */
BasicBlock *basic_block_create(int id, const char *label);
void basic_block_free(BasicBlock *bb);
void basic_block_add_instruction(BasicBlock *bb, IRInstruction *instr);
void basic_block_add_predecessor(BasicBlock *bb, BasicBlock *pred);
void basic_block_add_successor(BasicBlock *bb, BasicBlock *succ);

/* Instruction creation */
IRInstruction *ir_instruction_create(IROpcode opcode);
void ir_instruction_free(IRInstruction *instr);
void ir_instruction_set_dest(IRInstruction *instr, SSARegister *dest);
void ir_instruction_add_operand(IRInstruction *instr, IROperand operand);
IRInstruction *ir_create_binop(IROpcode opcode, SSARegister *dest, SSARegister *left, SSARegister *right);
IRInstruction *ir_create_const_int(SSARegister *dest, long long value);
IRInstruction *ir_create_const_float(SSARegister *dest, double value);
IRInstruction *ir_create_const_str(SSARegister *dest, const char *value);
IRInstruction *ir_create_phi(SSARegister *dest);
void ir_phi_add_entry(IRInstruction *phi, SSARegister *value, BasicBlock *block);
IRInstruction *ir_create_branch(BasicBlock *target);
IRInstruction *ir_create_cond_branch(SSARegister *cond, BasicBlock *true_bb, BasicBlock *false_bb);
IRInstruction *ir_create_return(SSARegister *value);
IRInstruction *ir_create_call(SSARegister *dest, const char *func_name, SSARegister **args, int arg_count);

/* Register management */
SSARegister *ssa_register_create(SSAFunction *func, const char *name);
void ssa_register_free(SSARegister *reg);

/* Operand helpers */
IROperand ir_operand_register(SSARegister *reg);
IROperand ir_operand_const_int(long long value);
IROperand ir_operand_const_float(double value);
IROperand ir_operand_const_str(const char *value);
IROperand ir_operand_const_bool(int value);
IROperand ir_operand_const_null(void);
IROperand ir_operand_block(BasicBlock *block);

/* AST to SSA IR conversion */
SSAModule *ast_to_ssa(ASTNode *root);
SSAFunction *ast_function_to_ssa(ASTNode *func_node);
void ssa_convert_statement(SSAContext *ctx, ASTNode *stmt);
SSARegister *ssa_convert_expression(SSAContext *ctx, ASTNode *expr);

/* SSA Context management */
SSAContext *ssa_context_create(SSAFunction *func);
void ssa_context_free(SSAContext *ctx);
int ssa_context_get_var_version(SSAContext *ctx, const char *var_name);
void ssa_context_set_var_version(SSAContext *ctx, const char *var_name, int version);

/* CFG construction */
void build_cfg(SSAFunction *func);
void compute_dominators(SSAFunction *func);
void compute_dominance_frontier(SSAFunction *func);
void insert_phi_nodes(SSAFunction *func);

/* IR printing and debugging */
void ssa_module_print(SSAModule *module);
void ssa_function_print(SSAFunction *func);
void basic_block_print(BasicBlock *bb);
void ir_instruction_print(IRInstruction *instr);
const char *ir_opcode_name(IROpcode opcode);

#endif /* SSA_IR_H */
