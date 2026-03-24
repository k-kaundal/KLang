#include "ssa_ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* === Module Management === */

SSAModule *ssa_module_create(void) {
    SSAModule *module = malloc(sizeof(SSAModule));
    module->functions = NULL;
    module->func_count = 0;
    module->func_capacity = 0;
    module->global_vars = NULL;
    module->global_count = 0;
    return module;
}

void ssa_module_free(SSAModule *module) {
    if (!module) return;
    for (int i = 0; i < module->func_count; i++) {
        ssa_function_free(module->functions[i]);
    }
    free(module->functions);
    for (int i = 0; i < module->global_count; i++) {
        free(module->global_vars[i]);
    }
    free(module->global_vars);
    free(module);
}

void ssa_module_add_function(SSAModule *module, SSAFunction *func) {
    if (module->func_count >= module->func_capacity) {
        module->func_capacity = module->func_capacity == 0 ? 8 : module->func_capacity * 2;
        module->functions = realloc(module->functions, module->func_capacity * sizeof(SSAFunction *));
    }
    module->functions[module->func_count++] = func;
}

/* === Function Management === */

SSAFunction *ssa_function_create(const char *name) {
    SSAFunction *func = malloc(sizeof(SSAFunction));
    func->name = name ? strdup(name) : NULL;
    func->blocks = NULL;
    func->block_count = 0;
    func->block_capacity = 0;
    func->entry = NULL;
    func->exit = NULL;
    func->params = NULL;
    func->param_count = 0;
    func->registers = NULL;
    func->reg_count = 0;
    func->reg_capacity = 0;
    func->next_reg_id = 0;
    func->next_block_id = 0;
    return func;
}

void ssa_function_free(SSAFunction *func) {
    if (!func) return;
    free(func->name);
    for (int i = 0; i < func->block_count; i++) {
        basic_block_free(func->blocks[i]);
    }
    free(func->blocks);
    for (int i = 0; i < func->reg_count; i++) {
        ssa_register_free(func->registers[i]);
    }
    free(func->registers);
    free(func->params);
    free(func);
}

BasicBlock *ssa_function_add_block(SSAFunction *func, const char *label) {
    BasicBlock *bb = basic_block_create(func->next_block_id++, label);
    if (func->block_count >= func->block_capacity) {
        func->block_capacity = func->block_capacity == 0 ? 8 : func->block_capacity * 2;
        func->blocks = realloc(func->blocks, func->block_capacity * sizeof(BasicBlock *));
    }
    func->blocks[func->block_count++] = bb;
    return bb;
}

/* === Basic Block Management === */

BasicBlock *basic_block_create(int id, const char *label) {
    BasicBlock *bb = malloc(sizeof(BasicBlock));
    bb->id = id;
    bb->label = label ? strdup(label) : NULL;
    bb->first = NULL;
    bb->last = NULL;
    bb->predecessors = NULL;
    bb->pred_count = 0;
    bb->pred_capacity = 0;
    bb->successors = NULL;
    bb->succ_count = 0;
    bb->succ_capacity = 0;
    bb->idom = NULL;
    bb->dom_children = NULL;
    bb->dom_child_count = 0;
    bb->dom_frontier = NULL;
    bb->df_count = 0;
    bb->visited = 0;
    bb->rpo_num = -1;
    return bb;
}

void basic_block_free(BasicBlock *bb) {
    if (!bb) return;
    free(bb->label);
    
    IRInstruction *instr = bb->first;
    while (instr) {
        IRInstruction *next = instr->next;
        ir_instruction_free(instr);
        instr = next;
    }
    
    free(bb->predecessors);
    free(bb->successors);
    free(bb->dom_children);
    free(bb->dom_frontier);
    free(bb);
}

void basic_block_add_instruction(BasicBlock *bb, IRInstruction *instr) {
    if (!bb->first) {
        bb->first = bb->last = instr;
    } else {
        bb->last->next = instr;
        bb->last = instr;
    }
    instr->next = NULL;
}

void basic_block_add_predecessor(BasicBlock *bb, BasicBlock *pred) {
    if (bb->pred_count >= bb->pred_capacity) {
        bb->pred_capacity = bb->pred_capacity == 0 ? 4 : bb->pred_capacity * 2;
        bb->predecessors = realloc(bb->predecessors, bb->pred_capacity * sizeof(BasicBlock *));
    }
    bb->predecessors[bb->pred_count++] = pred;
}

void basic_block_add_successor(BasicBlock *bb, BasicBlock *succ) {
    if (bb->succ_count >= bb->succ_capacity) {
        bb->succ_capacity = bb->succ_capacity == 0 ? 4 : bb->succ_capacity * 2;
        bb->successors = realloc(bb->successors, bb->succ_capacity * sizeof(BasicBlock *));
    }
    bb->successors[bb->succ_count++] = succ;
}

/* === Instruction Management === */

IRInstruction *ir_instruction_create(IROpcode opcode) {
    IRInstruction *instr = malloc(sizeof(IRInstruction));
    instr->opcode = opcode;
    instr->dest = NULL;
    instr->operands = NULL;
    instr->operand_count = 0;
    instr->phi_entries = NULL;
    instr->phi_count = 0;
    instr->line = 0;
    instr->next = NULL;
    return instr;
}

void ir_instruction_free(IRInstruction *instr) {
    if (!instr) return;
    for (int i = 0; i < instr->operand_count; i++) {
        if (instr->operands[i].type == OPERAND_CONSTANT &&
            instr->operands[i].constant.const_type == CONST_STR) {
            free(instr->operands[i].constant.str_val);
        }
    }
    free(instr->operands);
    free(instr->phi_entries);
    free(instr);
}

void ir_instruction_set_dest(IRInstruction *instr, SSARegister *dest) {
    instr->dest = dest;
}

void ir_instruction_add_operand(IRInstruction *instr, IROperand operand) {
    instr->operands = realloc(instr->operands, (instr->operand_count + 1) * sizeof(IROperand));
    instr->operands[instr->operand_count++] = operand;
}

IRInstruction *ir_create_binop(IROpcode opcode, SSARegister *dest, SSARegister *left, SSARegister *right) {
    IRInstruction *instr = ir_instruction_create(opcode);
    instr->dest = dest;
    ir_instruction_add_operand(instr, ir_operand_register(left));
    ir_instruction_add_operand(instr, ir_operand_register(right));
    return instr;
}

IRInstruction *ir_create_const_int(SSARegister *dest, long long value) {
    IRInstruction *instr = ir_instruction_create(IR_CONST_INT);
    instr->dest = dest;
    ir_instruction_add_operand(instr, ir_operand_const_int(value));
    return instr;
}

IRInstruction *ir_create_const_float(SSARegister *dest, double value) {
    IRInstruction *instr = ir_instruction_create(IR_CONST_FLOAT);
    instr->dest = dest;
    ir_instruction_add_operand(instr, ir_operand_const_float(value));
    return instr;
}

IRInstruction *ir_create_const_str(SSARegister *dest, const char *value) {
    IRInstruction *instr = ir_instruction_create(IR_CONST_STR);
    instr->dest = dest;
    ir_instruction_add_operand(instr, ir_operand_const_str(value));
    return instr;
}

IRInstruction *ir_create_phi(SSARegister *dest) {
    IRInstruction *instr = ir_instruction_create(IR_PHI);
    instr->dest = dest;
    return instr;
}

void ir_phi_add_entry(IRInstruction *phi, SSARegister *value, BasicBlock *block) {
    phi->phi_entries = realloc(phi->phi_entries, (phi->phi_count + 1) * sizeof(PhiEntry));
    phi->phi_entries[phi->phi_count].value = value;
    phi->phi_entries[phi->phi_count].block = block;
    phi->phi_count++;
}

IRInstruction *ir_create_branch(BasicBlock *target) {
    IRInstruction *instr = ir_instruction_create(IR_BR);
    ir_instruction_add_operand(instr, ir_operand_block(target));
    return instr;
}

IRInstruction *ir_create_cond_branch(SSARegister *cond, BasicBlock *true_bb, BasicBlock *false_bb) {
    IRInstruction *instr = ir_instruction_create(IR_CONDBR);
    ir_instruction_add_operand(instr, ir_operand_register(cond));
    ir_instruction_add_operand(instr, ir_operand_block(true_bb));
    ir_instruction_add_operand(instr, ir_operand_block(false_bb));
    return instr;
}

IRInstruction *ir_create_return(SSARegister *value) {
    IRInstruction *instr = ir_instruction_create(IR_RET);
    if (value) {
        ir_instruction_add_operand(instr, ir_operand_register(value));
    }
    return instr;
}

IRInstruction *ir_create_call(SSARegister *dest, const char *func_name, SSARegister **args, int arg_count) {
    IRInstruction *instr = ir_instruction_create(IR_CALL);
    instr->dest = dest;
    ir_instruction_add_operand(instr, ir_operand_const_str(func_name));
    for (int i = 0; i < arg_count; i++) {
        ir_instruction_add_operand(instr, ir_operand_register(args[i]));
    }
    return instr;
}

/* === Register Management === */

SSARegister *ssa_register_create(SSAFunction *func, const char *name) {
    SSARegister *reg = malloc(sizeof(SSARegister));
    reg->id = func->next_reg_id++;
    reg->name = name ? strdup(name) : NULL;
    reg->version = 0;
    
    if (func->reg_count >= func->reg_capacity) {
        func->reg_capacity = func->reg_capacity == 0 ? 16 : func->reg_capacity * 2;
        func->registers = realloc(func->registers, func->reg_capacity * sizeof(SSARegister *));
    }
    func->registers[func->reg_count++] = reg;
    
    return reg;
}

void ssa_register_free(SSARegister *reg) {
    if (!reg) return;
    free(reg->name);
    free(reg);
}

/* === Operand Helpers === */

IROperand ir_operand_register(SSARegister *reg) {
    IROperand op;
    op.type = OPERAND_REGISTER;
    op.reg = reg;
    return op;
}

IROperand ir_operand_const_int(long long value) {
    IROperand op;
    op.type = OPERAND_CONSTANT;
    op.constant.const_type = CONST_INT;
    op.constant.int_val = value;
    return op;
}

IROperand ir_operand_const_float(double value) {
    IROperand op;
    op.type = OPERAND_CONSTANT;
    op.constant.const_type = CONST_FLOAT;
    op.constant.float_val = value;
    return op;
}

IROperand ir_operand_const_str(const char *value) {
    IROperand op;
    op.type = OPERAND_CONSTANT;
    op.constant.const_type = CONST_STR;
    op.constant.str_val = value ? strdup(value) : NULL;
    return op;
}

IROperand ir_operand_const_bool(int value) {
    IROperand op;
    op.type = OPERAND_CONSTANT;
    op.constant.const_type = CONST_BOOL;
    op.constant.bool_val = value;
    return op;
}

IROperand ir_operand_const_null(void) {
    IROperand op;
    op.type = OPERAND_CONSTANT;
    op.constant.const_type = CONST_NULL;
    return op;
}

IROperand ir_operand_block(BasicBlock *block) {
    IROperand op;
    op.type = OPERAND_BLOCK;
    op.block = block;
    return op;
}

/* === SSA Context Management === */

SSAContext *ssa_context_create(SSAFunction *func) {
    SSAContext *ctx = malloc(sizeof(SSAContext));
    ctx->current_func = func;
    ctx->current_block = NULL;
    ctx->var_versions = NULL;
    ctx->var_version_count = 0;
    ctx->break_target = NULL;
    ctx->continue_target = NULL;
    return ctx;
}

void ssa_context_free(SSAContext *ctx) {
    if (!ctx) return;
    for (int i = 0; i < ctx->var_version_count; i++) {
        free(ctx->var_versions[i].var_name);
        free(ctx->var_versions[i].versions);
    }
    free(ctx->var_versions);
    free(ctx);
}

int ssa_context_get_var_version(SSAContext *ctx, const char *var_name) {
    for (int i = 0; i < ctx->var_version_count; i++) {
        if (strcmp(ctx->var_versions[i].var_name, var_name) == 0) {
            if (ctx->var_versions[i].version_count > 0) {
                return ctx->var_versions[i].versions[ctx->var_versions[i].version_count - 1];
            }
            return 0;
        }
    }
    return 0;
}

void ssa_context_set_var_version(SSAContext *ctx, const char *var_name, int version) {
    for (int i = 0; i < ctx->var_version_count; i++) {
        if (strcmp(ctx->var_versions[i].var_name, var_name) == 0) {
            ctx->var_versions[i].versions = realloc(ctx->var_versions[i].versions,
                (ctx->var_versions[i].version_count + 1) * sizeof(int));
            ctx->var_versions[i].versions[ctx->var_versions[i].version_count++] = version;
            return;
        }
    }
    
    ctx->var_versions = realloc(ctx->var_versions, (ctx->var_version_count + 1) * sizeof(*ctx->var_versions));
    ctx->var_versions[ctx->var_version_count].var_name = strdup(var_name);
    ctx->var_versions[ctx->var_version_count].versions = malloc(sizeof(int));
    ctx->var_versions[ctx->var_version_count].versions[0] = version;
    ctx->var_versions[ctx->var_version_count].version_count = 1;
    ctx->var_version_count++;
}

/* === AST to SSA Conversion === */

static IROpcode ast_binop_to_ir(const char *op) {
    if (strcmp(op, "+") == 0) return IR_ADD;
    if (strcmp(op, "-") == 0) return IR_SUB;
    if (strcmp(op, "*") == 0) return IR_MUL;
    if (strcmp(op, "/") == 0) return IR_DIV;
    if (strcmp(op, "%") == 0) return IR_MOD;
    if (strcmp(op, "&&") == 0 || strcmp(op, "and") == 0) return IR_AND;
    if (strcmp(op, "||") == 0 || strcmp(op, "or") == 0) return IR_OR;
    if (strcmp(op, "^") == 0) return IR_XOR;
    if (strcmp(op, "==") == 0) return IR_EQ;
    if (strcmp(op, "!=") == 0) return IR_NE;
    if (strcmp(op, "<") == 0) return IR_LT;
    if (strcmp(op, "<=") == 0) return IR_LE;
    if (strcmp(op, ">") == 0) return IR_GT;
    if (strcmp(op, ">=") == 0) return IR_GE;
    return IR_NOP;
}

SSARegister *ssa_convert_expression(SSAContext *ctx, ASTNode *expr) {
    if (!expr) return NULL;
    
    switch (expr->type) {
        case NODE_NUMBER: {
            SSARegister *reg = ssa_register_create(ctx->current_func, NULL);
            IRInstruction *instr = ir_create_const_float(reg, expr->num_val);
            basic_block_add_instruction(ctx->current_block, instr);
            return reg;
        }
        
        case NODE_STRING: {
            SSARegister *reg = ssa_register_create(ctx->current_func, NULL);
            IRInstruction *instr = ir_create_const_str(reg, expr->str_val);
            basic_block_add_instruction(ctx->current_block, instr);
            return reg;
        }
        
        case NODE_BOOL: {
            SSARegister *reg = ssa_register_create(ctx->current_func, NULL);
            IRInstruction *instr = ir_instruction_create(IR_CONST_BOOL);
            instr->dest = reg;
            ir_instruction_add_operand(instr, ir_operand_const_bool(expr->bool_val));
            basic_block_add_instruction(ctx->current_block, instr);
            return reg;
        }
        
        case NODE_NULL: {
            SSARegister *reg = ssa_register_create(ctx->current_func, NULL);
            IRInstruction *instr = ir_instruction_create(IR_CONST_NULL);
            instr->dest = reg;
            basic_block_add_instruction(ctx->current_block, instr);
            return reg;
        }
        
        case NODE_IDENT: {
            int version = ssa_context_get_var_version(ctx, expr->ident.name);
            SSARegister *reg = ssa_register_create(ctx->current_func, expr->ident.name);
            reg->version = version;
            return reg;
        }
        
        case NODE_BINOP: {
            SSARegister *left = ssa_convert_expression(ctx, expr->binop.left);
            SSARegister *right = ssa_convert_expression(ctx, expr->binop.right);
            SSARegister *dest = ssa_register_create(ctx->current_func, NULL);
            
            IROpcode opcode = ast_binop_to_ir(expr->binop.op);
            IRInstruction *instr = ir_create_binop(opcode, dest, left, right);
            basic_block_add_instruction(ctx->current_block, instr);
            return dest;
        }
        
        case NODE_UNOP: {
            SSARegister *operand = ssa_convert_expression(ctx, expr->unop.operand);
            SSARegister *dest = ssa_register_create(ctx->current_func, NULL);
            
            IROpcode opcode = IR_NOP;
            if (strcmp(expr->unop.op, "!") == 0 || strcmp(expr->unop.op, "not") == 0) {
                opcode = IR_NOT;
            } else if (strcmp(expr->unop.op, "-") == 0) {
                opcode = IR_SUB;
                SSARegister *zero = ssa_register_create(ctx->current_func, NULL);
                IRInstruction *zero_instr = ir_create_const_int(zero, 0);
                basic_block_add_instruction(ctx->current_block, zero_instr);
                operand = zero;
            }
            
            IRInstruction *instr = ir_instruction_create(opcode);
            instr->dest = dest;
            ir_instruction_add_operand(instr, ir_operand_register(operand));
            basic_block_add_instruction(ctx->current_block, instr);
            return dest;
        }
        
        case NODE_CALL: {
            // Convert arguments
            SSARegister **args = malloc(expr->call.args.count * sizeof(SSARegister *));
            for (int i = 0; i < expr->call.args.count; i++) {
                args[i] = ssa_convert_expression(ctx, expr->call.args.items[i]);
            }
            
            // Get function name
            const char *func_name = NULL;
            if (expr->call.callee->type == NODE_IDENT) {
                func_name = expr->call.callee->ident.name;
            }
            
            SSARegister *dest = ssa_register_create(ctx->current_func, NULL);
            IRInstruction *instr = ir_create_call(dest, func_name, args, expr->call.args.count);
            basic_block_add_instruction(ctx->current_block, instr);
            free(args);
            return dest;
        }
        
        default:
            return NULL;
    }
}

void ssa_convert_statement(SSAContext *ctx, ASTNode *stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case NODE_LET: {
            if (stmt->let_stmt.value) {
                SSARegister *value = ssa_convert_expression(ctx, stmt->let_stmt.value);
                int version = ctx->current_func->next_reg_id;
                ssa_context_set_var_version(ctx, stmt->let_stmt.name, version);
                
                SSARegister *var_reg = ssa_register_create(ctx->current_func, stmt->let_stmt.name);
                var_reg->version = version;
                
                IRInstruction *store = ir_instruction_create(IR_STORE);
                ir_instruction_add_operand(store, ir_operand_register(var_reg));
                ir_instruction_add_operand(store, ir_operand_register(value));
                basic_block_add_instruction(ctx->current_block, store);
            }
            break;
        }
        
        case NODE_ASSIGN: {
            SSARegister *value = ssa_convert_expression(ctx, stmt->assign_stmt.value);
            int version = ctx->current_func->next_reg_id;
            ssa_context_set_var_version(ctx, stmt->assign_stmt.name, version);
            
            SSARegister *var_reg = ssa_register_create(ctx->current_func, stmt->assign_stmt.name);
            var_reg->version = version;
            
            IRInstruction *store = ir_instruction_create(IR_STORE);
            ir_instruction_add_operand(store, ir_operand_register(var_reg));
            ir_instruction_add_operand(store, ir_operand_register(value));
            basic_block_add_instruction(ctx->current_block, store);
            break;
        }
        
        case NODE_RETURN: {
            SSARegister *value = stmt->return_stmt.value ? 
                ssa_convert_expression(ctx, stmt->return_stmt.value) : NULL;
            IRInstruction *ret = ir_create_return(value);
            basic_block_add_instruction(ctx->current_block, ret);
            break;
        }
        
        case NODE_IF: {
            SSARegister *cond = ssa_convert_expression(ctx, stmt->if_stmt.cond);
            
            BasicBlock *then_bb = ssa_function_add_block(ctx->current_func, "if.then");
            BasicBlock *else_bb = stmt->if_stmt.else_block ? 
                ssa_function_add_block(ctx->current_func, "if.else") : NULL;
            BasicBlock *merge_bb = ssa_function_add_block(ctx->current_func, "if.merge");
            
            IRInstruction *branch = ir_create_cond_branch(cond, then_bb, else_bb ? else_bb : merge_bb);
            basic_block_add_instruction(ctx->current_block, branch);
            
            basic_block_add_successor(ctx->current_block, then_bb);
            basic_block_add_predecessor(then_bb, ctx->current_block);
            
            if (else_bb) {
                basic_block_add_successor(ctx->current_block, else_bb);
                basic_block_add_predecessor(else_bb, ctx->current_block);
            } else {
                basic_block_add_successor(ctx->current_block, merge_bb);
                basic_block_add_predecessor(merge_bb, ctx->current_block);
            }
            
            // Then block
            ctx->current_block = then_bb;
            ssa_convert_statement(ctx, stmt->if_stmt.then_block);
            IRInstruction *then_br = ir_create_branch(merge_bb);
            basic_block_add_instruction(ctx->current_block, then_br);
            basic_block_add_successor(then_bb, merge_bb);
            basic_block_add_predecessor(merge_bb, then_bb);
            
            // Else block
            if (else_bb) {
                ctx->current_block = else_bb;
                ssa_convert_statement(ctx, stmt->if_stmt.else_block);
                IRInstruction *else_br = ir_create_branch(merge_bb);
                basic_block_add_instruction(ctx->current_block, else_br);
                basic_block_add_successor(else_bb, merge_bb);
                basic_block_add_predecessor(merge_bb, else_bb);
            }
            
            ctx->current_block = merge_bb;
            break;
        }
        
        case NODE_WHILE: {
            BasicBlock *cond_bb = ssa_function_add_block(ctx->current_func, "while.cond");
            BasicBlock *body_bb = ssa_function_add_block(ctx->current_func, "while.body");
            BasicBlock *exit_bb = ssa_function_add_block(ctx->current_func, "while.exit");
            
            IRInstruction *goto_cond = ir_create_branch(cond_bb);
            basic_block_add_instruction(ctx->current_block, goto_cond);
            basic_block_add_successor(ctx->current_block, cond_bb);
            basic_block_add_predecessor(cond_bb, ctx->current_block);
            
            // Condition block
            ctx->current_block = cond_bb;
            SSARegister *cond = ssa_convert_expression(ctx, stmt->while_stmt.cond);
            IRInstruction *branch = ir_create_cond_branch(cond, body_bb, exit_bb);
            basic_block_add_instruction(ctx->current_block, branch);
            basic_block_add_successor(cond_bb, body_bb);
            basic_block_add_successor(cond_bb, exit_bb);
            basic_block_add_predecessor(body_bb, cond_bb);
            basic_block_add_predecessor(exit_bb, cond_bb);
            
            // Body block
            BasicBlock *old_break = ctx->break_target;
            BasicBlock *old_continue = ctx->continue_target;
            ctx->break_target = exit_bb;
            ctx->continue_target = cond_bb;
            
            ctx->current_block = body_bb;
            ssa_convert_statement(ctx, stmt->while_stmt.body);
            IRInstruction *loop_br = ir_create_branch(cond_bb);
            basic_block_add_instruction(ctx->current_block, loop_br);
            basic_block_add_successor(body_bb, cond_bb);
            basic_block_add_predecessor(cond_bb, body_bb);
            
            ctx->break_target = old_break;
            ctx->continue_target = old_continue;
            ctx->current_block = exit_bb;
            break;
        }
        
        case NODE_BLOCK: {
            for (int i = 0; i < stmt->block.stmts.count; i++) {
                ssa_convert_statement(ctx, stmt->block.stmts.items[i]);
            }
            break;
        }
        
        case NODE_BREAK: {
            if (ctx->break_target) {
                IRInstruction *br = ir_create_branch(ctx->break_target);
                basic_block_add_instruction(ctx->current_block, br);
            }
            break;
        }
        
        case NODE_CONTINUE: {
            if (ctx->continue_target) {
                IRInstruction *br = ir_create_branch(ctx->continue_target);
                basic_block_add_instruction(ctx->current_block, br);
            }
            break;
        }
        
        default:
            // Try to convert as expression statement
            ssa_convert_expression(ctx, stmt);
            break;
    }
}

SSAFunction *ast_function_to_ssa(ASTNode *func_node) {
    // TODO: Integrate with actual KLang AST structure
    // This is a placeholder that will be implemented when
    // integrating SSA IR with the KLang parser
    (void)func_node; // Suppress unused parameter warning
    return NULL;
}

SSAModule *ast_to_ssa(ASTNode *root) {
    if (!root) return NULL;
    
    SSAModule *module = ssa_module_create();
    
    // TODO: Integrate with actual KLang AST structure
    // This is a placeholder implementation
    // The actual implementation will traverse the KLang AST
    // and generate SSA IR based on the real node types
    
    return module;
}

/* === CFG Construction === */

void build_cfg(SSAFunction *func) {
    // CFG edges are built during SSA conversion
    // This function can be extended for validation or additional processing
    (void)func; // Suppress unused parameter warning
}

void compute_dominators(SSAFunction *func) {
    // Simplified dominator computation - placeholder for full algorithm
    if (func->entry) {
        func->entry->idom = NULL;
    }
}

void compute_dominance_frontier(SSAFunction *func) {
    // Placeholder for dominance frontier computation
    (void)func; // Suppress unused parameter warning
}

void insert_phi_nodes(SSAFunction *func) {
    // Placeholder for phi node insertion based on dominance frontier
    (void)func; // Suppress unused parameter warning
}

/* === IR Printing === */

const char *ir_opcode_name(IROpcode opcode) {
    switch (opcode) {
        case IR_ADD: return "add";
        case IR_SUB: return "sub";
        case IR_MUL: return "mul";
        case IR_DIV: return "div";
        case IR_MOD: return "mod";
        case IR_AND: return "and";
        case IR_OR: return "or";
        case IR_NOT: return "not";
        case IR_XOR: return "xor";
        case IR_EQ: return "eq";
        case IR_NE: return "ne";
        case IR_LT: return "lt";
        case IR_LE: return "le";
        case IR_GT: return "gt";
        case IR_GE: return "ge";
        case IR_LOAD: return "load";
        case IR_STORE: return "store";
        case IR_ALLOCA: return "alloca";
        case IR_BR: return "br";
        case IR_CONDBR: return "condbr";
        case IR_PHI: return "phi";
        case IR_CALL: return "call";
        case IR_RET: return "ret";
        case IR_CAST: return "cast";
        case IR_TYPEOF: return "typeof";
        case IR_INSTANCEOF: return "instanceof";
        case IR_CONST_INT: return "const_int";
        case IR_CONST_FLOAT: return "const_float";
        case IR_CONST_STR: return "const_str";
        case IR_CONST_BOOL: return "const_bool";
        case IR_CONST_NULL: return "const_null";
        case IR_MOVE: return "move";
        case IR_PARAM: return "param";
        case IR_NOP: return "nop";
        default: return "unknown";
    }
}

void ir_instruction_print(IRInstruction *instr) {
    if (!instr) return;
    
    if (instr->dest) {
        printf("  %%%d", instr->dest->id);
        if (instr->dest->name) {
            printf(".%s", instr->dest->name);
        }
        printf(" = ");
    } else {
        printf("  ");
    }
    
    printf("%s", ir_opcode_name(instr->opcode));
    
    if (instr->opcode == IR_PHI) {
        printf(" [");
        for (int i = 0; i < instr->phi_count; i++) {
            if (i > 0) printf(", ");
            printf("%%%d from bb%d", instr->phi_entries[i].value->id, 
                   instr->phi_entries[i].block->id);
        }
        printf("]");
    } else {
        for (int i = 0; i < instr->operand_count; i++) {
            printf(" ");
            IROperand *op = &instr->operands[i];
            switch (op->type) {
                case OPERAND_REGISTER:
                    printf("%%%d", op->reg->id);
                    if (op->reg->name) {
                        printf(".%s", op->reg->name);
                    }
                    break;
                case OPERAND_CONSTANT:
                    switch (op->constant.const_type) {
                        case CONST_INT:
                            printf("%lld", op->constant.int_val);
                            break;
                        case CONST_FLOAT:
                            printf("%f", op->constant.float_val);
                            break;
                        case CONST_STR:
                            printf("\"%s\"", op->constant.str_val);
                            break;
                        case CONST_BOOL:
                            printf("%s", op->constant.bool_val ? "true" : "false");
                            break;
                        case CONST_NULL:
                            printf("null");
                            break;
                    }
                    break;
                case OPERAND_BLOCK:
                    printf("bb%d", op->block->id);
                    break;
                case OPERAND_FUNCTION:
                    printf("@func");
                    break;
            }
        }
    }
    printf("\n");
}

void basic_block_print(BasicBlock *bb) {
    if (!bb) return;
    
    printf("bb%d", bb->id);
    if (bb->label) {
        printf(" (%s)", bb->label);
    }
    printf(":\n");
    
    if (bb->pred_count > 0) {
        printf("  ; predecessors: ");
        for (int i = 0; i < bb->pred_count; i++) {
            if (i > 0) printf(", ");
            printf("bb%d", bb->predecessors[i]->id);
        }
        printf("\n");
    }
    
    IRInstruction *instr = bb->first;
    while (instr) {
        ir_instruction_print(instr);
        instr = instr->next;
    }
    printf("\n");
}

void ssa_function_print(SSAFunction *func) {
    if (!func) return;
    
    printf("function @%s(", func->name ? func->name : "anonymous");
    for (int i = 0; i < func->param_count; i++) {
        if (i > 0) printf(", ");
        printf("%%%d", func->params[i]->id);
        if (func->params[i]->name) {
            printf(".%s", func->params[i]->name);
        }
    }
    printf(") {\n");
    
    for (int i = 0; i < func->block_count; i++) {
        basic_block_print(func->blocks[i]);
    }
    
    printf("}\n\n");
}

void ssa_module_print(SSAModule *module) {
    if (!module) return;
    
    printf("; SSA IR Module\n");
    printf("; Functions: %d\n\n", module->func_count);
    
    for (int i = 0; i < module->func_count; i++) {
        ssa_function_print(module->functions[i]);
    }
}
