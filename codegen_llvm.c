#include "codegen_llvm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int temp_id = 0;
static int str_count = 0;

static const char *get_temp() {
    static char buf[32];
    snprintf(buf, sizeof(buf), "%%v%d", temp_id++);
    return buf;
}

static void emit_header(FILE *out) {
    fprintf(out,
        "declare i32 @printf(i8*, ...)\n"
        "declare void @print_int(i32)\n"
        "declare void @print_char(i8)\n"
        "declare void @print_string(i8*)\n"
        "declare void @print_float(float)\n\n"
        "@.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n"
        "@.str_char = private unnamed_addr constant [4 x i8] c\"%%c\\0A\\00\", align 1\n"
        "@.str_float = private unnamed_addr constant [4 x i8] c\"%%f\\0A\\00\", align 1\n\n");
}

static void emit_global_string(FILE *out, const char *str, int id) {
    int len = (int)strlen(str);
    fprintf(out, "@.str%d = private unnamed_addr constant [%d x i8] c\"", id, len + 1);
    for (int i = 0; i < len; ++i) {
        if (str[i] == '\\') fprintf(out, "\\5C");
        else if (str[i] == '\n') fprintf(out, "\\0A");
        else if (str[i] == '"') fprintf(out, "\\22");
        else fprintf(out, "%c", str[i]);
    }
    fprintf(out, "\\00\", align 1\n");
}

void codegen_llvm_emit(IRList *ir, FILE *out) {
    emit_header(out);

    for (IRInst *inst = ir->head; inst; inst = inst->next) {
        if (inst->op == IR_STRING_CONST) {
            emit_global_string(out, inst->var_name, str_count++);
        } else if (inst->op == IR_ARRAY_LOAD || inst->op == IR_ARRAY_STORE) {
            fprintf(out, "@%s = global [100 x i32] zeroinitializer\n", inst->var_name);
        }
    }

    fprintf(out, "\ndefine i32 @main() {\nentry:\n");

    for (IRInst *inst = ir->head; inst; inst = inst->next) {
        const char *dst = get_temp();

        switch (inst->op) {
            case IR_LOAD_CONST:
                fprintf(out, "  %s = add i32 0, %d\n", dst, inst->value);
                break;
            case IR_ADD:
                fprintf(out, "  %s = add i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_SUB:
                fprintf(out, "  %s = sub i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_MUL:
                fprintf(out, "  %s = mul i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_DIV:
                fprintf(out, "  %s = sdiv i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_MOD:
                fprintf(out, "  %s = srem i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_AND:
                fprintf(out, "  %s = and i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_OR:
                fprintf(out, "  %s = or i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_XOR:
                fprintf(out, "  %s = xor i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_SHL:
                fprintf(out, "  %s = shl i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_SHR:
                fprintf(out, "  %s = ashr i32 %%v%d, %%v%d\n", dst, inst->src1, inst->src2);
                break;
            case IR_NEG:
                fprintf(out, "  %s = sub i32 0, %%v%d\n", dst, inst->src1);
                break;
            case IR_NOT:
                fprintf(out, "  %s = xor i32 %%v%d, -1\n", dst, inst->src1);
                break;
            case IR_LOGICAL_NOT:
                fprintf(out, "  %%tmp%d = icmp eq i32 %%v%d, 0\n", temp_id, inst->src1);
                fprintf(out, "  %s = zext i1 %%tmp%d to i32\n", dst, temp_id);
                temp_id++;
                break;
            case IR_PRINT:
                fprintf(out, "  call void @print_int(i32 %%v%d)\n", inst->src1);
                break;
            case IR_PRINT_CHAR:
                fprintf(out, "  call void @print_char(i8 %%v%d)\n", inst->src1);
                break;
            case IR_PRINT_STRING:
                fprintf(out, "  %%str%d = getelementptr inbounds [%d x i8], [%d x i8]* @.str%d, i32 0, i32 0\n",
                        inst->src1, (int)strlen(inst->var_name)+1, (int)strlen(inst->var_name)+1, inst->src1);
                fprintf(out, "  call void @print_string(i8* %%str%d)\n", inst->src1);
                break;
            case IR_CALL2:
                fprintf(out, "  %s = call i32 @%s(i32 %%v%d, i32 %%v%d)\n", dst, inst->label, inst->src1, inst->src2);
                break;
            case IR_RETURN:
                fprintf(out, "  ret i32 %%v%d\n", inst->src1);
                break;
            case IR_LABEL:
                fprintf(out, "%s:\n", inst->label);
                break;
            case IR_JUMP:
                fprintf(out, "  br label %%%s\n", inst->label);
                break;
            case IR_JUMP_IF_FALSE:
                fprintf(out, "  %%cmp%d = icmp eq i32 %%v%d, 0\n", temp_id, inst->src1);
                fprintf(out, "  br i1 %%cmp%d, label %%%s, label %%cont%d\n", temp_id, inst->label, temp_id);
                fprintf(out, "cont%d:\n", temp_id);
                temp_id++;
                break;
            case IR_ARRAY_LOAD:
                fprintf(out, "  %%arr_ptr%d = getelementptr inbounds [100 x i32], [100 x i32]* @%s, i32 0, i32 %%v%d\n",
                        temp_id, inst->var_name, inst->src1);
                fprintf(out, "  %s = load i32, i32* %%arr_ptr%d\n", dst, temp_id);
                temp_id++;
                break;
            case IR_ARRAY_STORE:
                fprintf(out, "  %%arr_ptr%d = getelementptr inbounds [100 x i32], [100 x i32]* @%s, i32 0, i32 %%v%d\n",
                        temp_id, inst->var_name, inst->src1);
                fprintf(out, "  store i32 %%v%d, i32* %%arr_ptr%d\n", inst->src2, temp_id);
                temp_id++;
                break;
            default:
                fprintf(out, "  ; Unhandled IR op: %%d\n", inst->op);
                break;
        }
    }

    fprintf(out, "  ret i32 0\n}\n");
}