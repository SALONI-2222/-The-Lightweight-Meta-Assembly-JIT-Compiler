#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"
#include "parser.h"

void ir_list_init(IRList *list) {
    list->head = list->tail = NULL;
    list->temp_count = 0;
    list->label_count = 0;
}

static IRInst *ir_inst_new() {
    IRInst *inst = malloc(sizeof(IRInst));
    inst->next = NULL;
    inst->var_name = NULL;
    inst->label = NULL;
    return inst;
}

static void ir_list_append(IRList *list, IRInst *inst) {
    if (list->tail) {
        list->tail->next = inst;
        list->tail = inst;
    } else {
        list->head = list->tail = inst;
    }
}

int ir_emit_const(IRList *list, int value) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_LOAD_CONST;
    inst->dest = list->temp_count++;
    inst->value = value;
    ir_list_append(list, inst);
    return inst->dest;
}

int ir_emit_fconst(IRList *list, float value) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_FLOAD_CONST;
    inst->dest = list->temp_count++;
    *(float *)&inst->value = value;
    ir_list_append(list, inst);
    return inst->dest;
}

int ir_emit_string(IRList *list, const char *str) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_STRING_CONST;
    inst->dest = list->temp_count++;
    inst->var_name = strdup(str);
    ir_list_append(list, inst);
    return inst->dest;
}

int ir_emit_binop(IRList *list, IROp op, int left, int right) {
    IRInst *inst = ir_inst_new();
    inst->op = op;
    inst->dest = list->temp_count++;
    inst->src1 = left;
    inst->src2 = right;
    ir_list_append(list, inst);
    return inst->dest;
}

int ir_emit_assign(IRList *list, const char *var_name, int src) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_STORE_VAR;
    inst->src1 = src;
    inst->var_name = strdup(var_name);
    ir_list_append(list, inst);
    return -1;
}

int ir_emit_load_var(IRList *list, const char *var_name) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_LOAD_VAR;
    inst->dest = list->temp_count++;
    inst->var_name = strdup(var_name);
    ir_list_append(list, inst);
    return inst->dest;
}

void ir_emit_label(IRList *list, const char *label) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_LABEL;
    inst->label = strdup(label);
    ir_list_append(list, inst);
}

void ir_emit_jump(IRList *list, const char *label) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_JUMP;
    inst->label = strdup(label);
    ir_list_append(list, inst);
}

void ir_emit_jump_if_false(IRList *list, int cond, const char *label) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_JUMP_IF_FALSE;
    inst->src1 = cond;
    inst->label = strdup(label);
    ir_list_append(list, inst);
}

void ir_emit_return(IRList *list, int value) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_RETURN;
    inst->src1 = value;
    ir_list_append(list, inst);
}

static char *ir_new_label(IRList *list, const char *prefix) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%s_%d", prefix, list->label_count++);
    return strdup(buf);
}

int ir_emit_cast(IRList *list, IROp op, int src) {
    IRInst *inst = ir_inst_new();
    inst->op = op;
    inst->src1 = src;
    inst->dest = list->temp_count++;
    ir_list_append(list, inst);
    return inst->dest;
}

int ir_emit_assign_temp(IRList *list, int dest, int src) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_MOVE;  // Use IR_MOVE for SSA-to-SSA assignment
    inst->dest = dest;
    inst->src1 = src;
    ir_list_append(list, inst);
    return dest;
}

int ir_generate_expr(IRList *list, ASTNode *node) {
    if (!node) return -1;

    switch (node->type) {
        case AST_ARRAY_ACCESS:
            return ir_emit_array_load(list, node->array_access.array_name,
                                      ir_generate_expr(list, node->array_access.index));

        case AST_ARRAY_DECL: {
            int size = ir_generate_expr(list, node->array_decl.size_expr);
            printf("[IR] Array declaration: %s[%d]\n", node->array_decl.var_name, size);
            return -1;
        }

        case AST_NUMBER:
            return ir_emit_const(list, node->number);

        case AST_VAR:
            return ir_emit_load_var(list, node->var_name);

        case AST_FLOAT:
            return ir_emit_fconst(list, node->float_number);

        case AST_STRING:
            return ir_emit_string(list, node->string_value);

        case AST_CAST: {
            int src = ir_generate_expr(list, node->cast.operand);
            return ir_emit_cast(list,
                (node->cast.cast_type == TOKEN_INT) ? IR_CAST_F2I : IR_CAST_I2F,
                src);
        }

        case AST_ASSIGN: {
            if (node->assign.lhs->type == AST_ARRAY_ACCESS) {
                ASTNode *access = node->assign.lhs;
                int index = ir_generate_expr(list, access->array_access.index);
                int value = ir_generate_expr(list, node->assign.rhs);
                ir_emit_array_store(list, access->array_access.array_name, index, value);
                return value;
            }

            int rhs = ir_generate_expr(list, node->assign.rhs);
            ir_emit_assign(list, node->assign.lhs->var_name, rhs);
            return rhs;
        }

        case AST_BINARY_OP: {
            int left = ir_generate_expr(list, node->binop.left);
            int right = ir_generate_expr(list, node->binop.right);

            // Detect float op from previous IR (optional)
            IRInst *li = NULL, *ri = NULL;
            for (IRInst *scan = list->head; scan; scan = scan->next) {
                if (!li && scan->dest == left) li = scan;
                if (!ri && scan->dest == right) ri = scan;
                if (li && ri) break;
            }
            int float_op = li && ri && li->op == IR_FLOAD_CONST && ri->op == IR_FLOAD_CONST;

            IROp op;
            switch (node->binop.op) {
                case '+': op = float_op ? IR_FADD : IR_ADD; break;
                case '-': op = float_op ? IR_FSUB : IR_SUB; break;
                case '*': op = float_op ? IR_FMUL : IR_MUL; break;
                case '/': op = float_op ? IR_FDIV : IR_DIV; break;
                case '%': op = IR_MOD; break;
                case '=': op = IR_EQ; break;
                case '!': op = IR_NEQ; break;
                case '<': op = IR_LT; break;
                case '>': op = IR_GT; break;
                case 'l': op = IR_LE; break;
                case 'g': op = IR_GE; break;
                case '&': op = IR_BIT_AND; break;
                case '|': op = IR_BIT_OR; break;
                case '^': op = IR_BIT_XOR; break;
                case 'L': op = IR_SHL; break;
                case 'R': op = IR_SHR; break;
                default:
                    fprintf(stderr, "[IR] Unsupported binary op: %c\n", node->binop.op);
                    exit(1);
            }
            return ir_emit_binop(list, op, left, right);
        }

        case AST_UNARY_OP: {
            int operand = ir_generate_expr(list, node->unop.operand);
            IROp op;

            switch (node->unop.op) {
                case '-': op = IR_NEG; break;
                case '!': op = IR_LOG_NOT; break;
                case '~': op = IR_BIT_NOT; break;
                default:
                    fprintf(stderr, "[IR] Unsupported unary op: %c\n", node->unop.op);
                    exit(1);
            }

            IRInst *inst = ir_inst_new();
            inst->op = op;
            inst->src1 = operand;
            inst->dest = list->temp_count++;
            ir_list_append(list, inst);
            return inst->dest;
        }

        case AST_FUNCTION_CALL: {
            ASTList *arg = node->func_call.args;
            while (arg) {
                int temp = ir_generate_expr(list, arg->stmt);
                IRInst *arginst = ir_inst_new();
                arginst->op = IR_ARG;
                arginst->src1 = temp;
                ir_list_append(list, arginst);
                arg = arg->next;
            }

            IRInst *call = ir_inst_new();
            call->op = IR_CALL;
            call->label = strdup(node->func_call.name);
            call->dest = list->temp_count++;
            ir_list_append(list, call);
            return call->dest;
        }

        case AST_TERNARY: {
            int cond = ir_generate_expr(list, node->ternary.cond);
            char *label_else = ir_new_label(list, "tern_else");
            char *label_end = ir_new_label(list, "tern_end");

            int result = list->temp_count++;

            ir_emit_jump_if_false(list, cond, label_else);
            int then_val = ir_generate_expr(list, node->ternary.then_expr);
            ir_emit_assign_temp(list, result, then_val);
            ir_emit_jump(list, label_end);

            ir_emit_label(list, label_else);
            int else_val = ir_generate_expr(list, node->ternary.else_expr);
            ir_emit_assign_temp(list, result, else_val);

            ir_emit_label(list, label_end);
            return result;
        }

        default:
            fprintf(stderr, "[IR] Unsupported expression node type: %d\n", node->type);
            exit(1);
    }

    return -1;
}

void ir_generate(IRList *list, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_ARRAY_DECL: {
            int size = ir_generate_expr(list, node->array_decl.size_expr);
            printf("[IR] Array declaration: %s[%d]\n", node->array_decl.var_name, size);
            break;
        }
        case AST_DECL:
            if (node->decl.init) {
                int val = ir_generate_expr(list, node->decl.init);
                ir_emit_assign(list, node->decl.var_name, val);
            }
            break;
        case AST_ASSIGN: {
            if (node->assign.lhs->type == AST_ARRAY_ACCESS) {
                ASTNode *access = node->assign.lhs;
                int index = ir_generate_expr(list, access->array_access.index);
                int value = ir_generate_expr(list, node->assign.rhs);
                ir_emit_array_store(list, access->array_access.array_name, index, value);
                break;
            }
            int src = ir_generate_expr(list, node->assign.rhs);
            ir_emit_assign(list, node->assign.lhs->var_name, src);
            break;
        }
        case AST_EXPR_STMT:
            ir_generate_expr(list, node->expr);
            break;
        case AST_BLOCK: {
            ASTList *cur = node->block.stmts;
            while (cur) {
                ir_generate(list, cur->stmt);
                cur = cur->next;
            }
            break;
        }
        case AST_IF: {
            int cond = ir_generate_expr(list, node->if_stmt.condition);
            char *label_else = ir_new_label(list, "else");
            char *label_end = ir_new_label(list, "endif");

            ir_emit_jump_if_false(list, cond, label_else);
            ir_generate(list, node->if_stmt.then_stmt);
            ir_emit_jump(list, label_end);
            ir_emit_label(list, label_else);

            if (node->if_stmt.else_branch) {
                ir_generate(list, node->if_stmt.else_branch);
            }

            ir_emit_label(list, label_end);
            free(label_else);
            free(label_end);
            break;
        }
        case AST_WHILE: {
            char *label_start = ir_new_label(list, "while_start");
            char *label_end = ir_new_label(list, "while_end");

            ir_emit_label(list, label_start);
            int cond = ir_generate_expr(list, node->while_stmt.condition);
            ir_emit_jump_if_false(list, cond, label_end);
            ir_generate(list, node->while_stmt.do_stmt);
            ir_emit_jump(list, label_start);
            ir_emit_label(list, label_end);

            free(label_start);
            free(label_end);
            break;
        }
        case AST_FOR: {
            if (node->for_stmt.init) {
                ir_generate(list, node->for_stmt.init);
            }

            char *label_start = ir_new_label(list, "for_start");
            char *label_end = ir_new_label(list, "for_end");

            ir_emit_label(list, label_start);

            if (node->for_stmt.condition) {
                int cond_val = ir_generate_expr(list, node->for_stmt.condition);
                ir_emit_jump_if_false(list, cond_val, label_end);
            }

            ir_generate(list, node->for_stmt.body);

            if (node->for_stmt.update) {
                ir_generate_expr(list, node->for_stmt.update);
            }

            ir_emit_jump(list, label_start);
            ir_emit_label(list, label_end);

            free(label_start);
            free(label_end);
            break;
        }
        case AST_PRINT: {
            if (node->print_stmt.expr->type == AST_STRING) {
                int str_id = ir_emit_string(list, node->print_stmt.expr->string_value);
                IRInst *inst = ir_inst_new();
                inst->op = IR_PRINT_STRING;
                inst->src1 = str_id;
                inst->var_name = strdup(node->print_stmt.expr->string_value);
                ir_list_append(list, inst);
            } else {
                int val = ir_generate_expr(list, node->print_stmt.expr);
                IRInst *inst = ir_inst_new();
                inst->op = IR_PRINT;
                inst->src1 = val;
                inst->dest = -1;
                ir_list_append(list, inst);
            }
            break;
        }
        case AST_RETURN: {
            int val = ir_generate_expr(list, node->expr);
            ir_emit_return(list, val);
            break;
        }
        case AST_FUNCTION: {
            ir_emit_label(list, node->function.name);
            ASTList *param = node->function.params;
            while (param) {
                IRInst *arg_load = ir_inst_new();
                arg_load->op = IR_ARG_LOAD;
                arg_load->dest = list->temp_count++;
                ir_list_append(list, arg_load);
                ir_emit_assign(list, param->stmt->decl.var_name, arg_load->dest);
                param = param->next;
            }
            ir_generate(list, node->function.body);  // assumes body is AST_BLOCK
            break;
        }
        
        default:
            fprintf(stderr, "Unsupported statement node type %d\n", node->type);
            exit(1);
    }
}

void ir_generate_program(IRList *list, ASTList *program) {
    while (program) {
        ir_generate(list, program->stmt);
        program = program->next;
    }
}

void ir_print(IRList *list) {
    for (IRInst *inst = list->head; inst; inst = inst->next) {
        switch (inst->op) {
            case IR_LOAD_CONST:
                printf("t%d = %d\n", inst->dest, inst->value); break;
            case IR_FLOAD_CONST:
                printf("t%d = %f\n", inst->dest, *(float *)&inst->value); break;
            case IR_LOAD_VAR:
                printf("t%d = load %s\n", inst->dest, inst->var_name); break;
            case IR_STORE_VAR:
                printf("store %s, t%d\n", inst->var_name, inst->src1); break;

            case IR_ADD:      
                printf("t%d = t%d + t%d\n", inst->dest, inst->src1, inst->src2); 
                break;
            case IR_SUB:      
                printf("t%d = t%d - t%d\n", inst->dest, inst->src1, inst->src2); 
                break;
            case IR_MUL:      
                printf("t%d = t%d * t%d\n", inst->dest, inst->src1, inst->src2); 
                break;
            case IR_DIV:      
                printf("t%d = t%d / t%d\n", inst->dest, inst->src1, inst->src2); 
                break;
            case IR_MOD:      
                printf("t%d = t%d %% t%d\n", inst->dest, inst->src1, inst->src2); 
                break;

            case IR_FADD:     printf("t%d = t%d + t%d (float)\n", inst->dest, inst->src1, inst->src2); break;
            case IR_FSUB:     printf("t%d = t%d - t%d (float)\n", inst->dest, inst->src1, inst->src2); break;
            case IR_FMUL:     printf("t%d = t%d * t%d (float)\n", inst->dest, inst->src1, inst->src2); break;
            case IR_FDIV:     printf("t%d = t%d / t%d (float)\n", inst->dest, inst->src1, inst->src2); break;

            case IR_NEG:      printf("t%d = -t%d\n", inst->dest, inst->src1); break;
            case IR_LOG_NOT:  printf("t%d = !t%d\n", inst->dest, inst->src1); break;
            case IR_BIT_NOT:  printf("t%d = ~t%d\n", inst->dest, inst->src1); break;

            case IR_EQ:       printf("t%d = t%d == t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_NEQ:      printf("t%d = t%d != t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_LT:       printf("t%d = t%d < t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_GT:       printf("t%d = t%d > t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_LE:       printf("t%d = t%d <= t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_GE:       printf("t%d = t%d >= t%d\n", inst->dest, inst->src1, inst->src2); break;

            case IR_AND:      printf("t%d = t%d && t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_OR:       printf("t%d = t%d || t%d\n", inst->dest, inst->src1, inst->src2); break;

            case IR_BIT_AND:  printf("t%d = t%d & t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_BIT_OR:   printf("t%d = t%d | t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_BIT_XOR:  printf("t%d = t%d ^ t%d\n", inst->dest, inst->src1, inst->src2); break;

            case IR_SHL:      printf("t%d = t%d << t%d\n", inst->dest, inst->src1, inst->src2); break;
            case IR_SHR:      printf("t%d = t%d >> t%d\n", inst->dest, inst->src1, inst->src2); break;

            case IR_CAST_I2F: printf("t%d = (float) t%d\n", inst->dest, inst->src1); break;
            case IR_CAST_F2I: printf("t%d = (int) t%d\n", inst->dest, inst->src1); break;

            case IR_STRING_CONST:
                printf("t%d = \"%s\"\n", inst->dest, inst->var_name); break;

            case IR_PRINT:
                printf("print t%d\n", inst->src1); break;

            case IR_LABEL:
                printf("%s:\n", inst->label); break;

            case IR_JUMP:
                printf("jump %s\n", inst->label); break;

            case IR_JUMP_IF_FALSE:
                printf("jump_if_false t%d, %s\n", inst->src1, inst->label); break;

            case IR_ARRAY_LOAD:
                printf("t%d = %s[t%d]\n", inst->dest, inst->var_name, inst->src1); break;

            case IR_ARRAY_STORE:
                printf("%s[t%d] = t%d\n", inst->var_name, inst->src1, inst->src2); break;

            case IR_ARG:
                printf("arg t%d\n", inst->src1); break;

            case IR_ARG_LOAD:
                printf("t%d = arg\n", inst->dest); break;

            case IR_CALL:
                printf("t%d = call %s\n", inst->dest, inst->label); break;

            case IR_RETURN:
                printf("return t%d\n", inst->src1); break;

            case IR_MOVE:
                printf("t%d = t%d\n", inst->dest, inst->src1);
                break;
            
            case IR_PRINT_STRING:
                printf("print \"%s\"\n", inst->var_name); break;

            default:
                printf("Unknown IR instruction (op=%d)\n", inst->op); break;
        }
    }
}

void ir_free(IRList *list) {
    IRInst *inst = list->head;
    while (inst) {
        IRInst *next = inst->next;
        if (inst->var_name) free(inst->var_name);
        if (inst->label) free(inst->label);
        //if (inst->label) free(inst->label);
        free(inst);
        inst = next;
    }
    list->head = list->tail = NULL;
}


int ir_emit_array_store(IRList *list, const char *array_name, int index, int value) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_ARRAY_STORE;
    inst->src1 = index;
    inst->src2 = value;
    inst->var_name = strdup(array_name);
    ir_list_append(list, inst);
    return -1;
}

int ir_emit_array_load(IRList *list, const char *array_name, int index) {
    IRInst *inst = ir_inst_new();
    inst->op = IR_ARRAY_LOAD;
    inst->src1 = index;
    inst->dest = list->temp_count++;
    inst->var_name = strdup(array_name);
    ir_list_append(list, inst);
    return inst->dest;
}