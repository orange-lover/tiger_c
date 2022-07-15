#include "codegen.h"
#include "frame.h"
#include <assert.h>
#include "printtree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void emit(AS_instr instr);
static Temp_temp emit_binop_imm(string op, T_exp e, int v);
static Temp_temp emit_binop_tri(string op, T_exp e1, T_exp e2);
static void munchStm(T_stm s);
static Temp_temp munchExp(T_exp e);
static Temp_tempList munchArgs(T_expList args);

static AS_instrList instr_head = NULL, instr_last = NULL;
static F_frame stack_frame = NULL;

AS_instrList F_codegen(F_frame f, T_stmList stmList)
{
    //printStmList(stdout, stmList);
    //printf("F_codegen 1\n");
    // 栈帧大小常数
    string fname = Temp_labelstring(F_name(f));
    T_stmList sl = stmList;
    instr_head = NULL, instr_last = NULL;
    stack_frame = f;
    for (; sl; sl = sl->tail)
        munchStm(sl->head);
    //printf("F_codegen 2\n");
    return instr_head;
}

static void emit(AS_instr instr)
{
    if (instr_head == NULL)
    {
        instr_head = AS_InstrList(instr, NULL);
        instr_last = instr_head;
    }
    else
    {
        instr_last->tail = AS_InstrList(instr, NULL);
        instr_last = instr_last->tail;
    }
}
static Temp_temp emit_binop_imm(string op, T_exp e, int v)
{
    Temp_temp t = Temp_newtemp();
    char buf[100];
    sprintf(buf, "%s `d0, `s0, %d\n", op, v);
    emit(AS_Oper(String(buf), Temp_TempList(t, NULL), Temp_TempList(munchExp(e), NULL), NULL));
    return t;
}
static Temp_temp emit_binop_tri(string op, T_exp e1, T_exp e2)
{
    Temp_temp t = Temp_newtemp();
    char buf[100];
    sprintf(buf, "%s `d0, `s0, `s1\n", op);
    emit(AS_Oper(String(buf), Temp_TempList(t, NULL), Temp_TempList(munchExp(e1), Temp_TempList(munchExp(e2), NULL)), NULL));
    return t;
}
static void munchStm(T_stm s)
{
    // printf("munchStm 1\n");
    switch (s->kind)
    {
    case T_MOVE:
    {
        //printf("T_MOVE 1\n");
        // pr_stm(stdout, s,0);
        T_exp dst = s->u.MOve.dst, src = s->u.MOve.src;
        if (dst->kind == T_MEM)
        {
            //printf("T_MOVE 2\n");
             //pr_stm(stdout, s,0);
            if (dst->u.MEM->kind == T_BINOP && dst->u.MEM->u.BINOP.right->kind == T_CONST) /* MOVE(MEM(BINOP(PLUS,e1,CONST(i))),e2) */
            {
                //printf("T_MOVE 6-1\n");
                T_exp e1 = dst->u.MEM->u.BINOP.left, e2 = src;
                char buf[100];
                sprintf(buf, "sw `s0, %d(`s1)\n", dst->u.MEM->u.BINOP.right->u.CONST);
                emit(AS_Oper(String(buf), NULL, Temp_TempList(munchExp(e2), Temp_TempList(munchExp(e1), NULL)), NULL));
                //printf("T_MOVE 6-2\n");
            }
            else if (dst->u.MEM->kind == T_BINOP && dst->u.MEM->u.BINOP.left->kind == T_CONST) /* MOVE(MEM(BINOP(PLUS,CONST(i),e1)),e2) */
            {
                //printf("T_MOVE 5-1\n");
                T_exp e1 = dst->u.MEM->u.BINOP.right, e2 = src;
                char buf[100];
                sprintf(buf, "sw `s0, %d(`s1)\n", dst->u.MEM->u.BINOP.left->u.CONST);
                emit(AS_Oper(String(buf), NULL, Temp_TempList(munchExp(e2), Temp_TempList(munchExp(e1), NULL)), NULL));
                //printf("T_MOVE 5-2\n");
            }
            else if (dst->u.MEM->kind == T_CONST) /* MOVE(MEM(CONST(i)),e2) */
            {
                //printf("T_MOVE 4-1\n");
                T_exp e2 = src;
                char buf[100];
                sprintf(buf, "sw `s0, %d(`s1)\n", dst->u.MEM->u.CONST);
                emit(AS_Oper(String(buf), NULL, Temp_TempList(munchExp(e2), Temp_TempList(F_ZERO(), NULL)), NULL));
                //printf("T_MOVE 4-2\n");
            }
            else /* MOVE(MEM(e1),e2) */
            {
                //printf("T_MOVE 2-1\n");
                T_exp e1 = dst->u.MEM, e2 = src;
                char buf[100];
                sprintf(buf, "sw `s0, 0(`s1)\n");
                emit(AS_Oper(String(buf), NULL, Temp_TempList(munchExp(e2), Temp_TempList(munchExp(e1), NULL)), NULL));
                //printf("T_MOVE 2-2\n");
            }
            //printf("T_MOVE 5\n");
        }
        else if (dst->kind == T_TEMP) /* MOVE(TEMP(i),e2) */
        {
            //printf("T_MOVE 3\n");
            if (src->kind == T_MEM)
            {
                if (src->u.MEM->kind == T_BINOP && src->u.MEM->u.BINOP.right->kind == T_CONST) /* MOVE(TEMP(i),MEM(BINOP(PLUS,e1,CONST(i)))) */
                {
                    T_exp e1 = src->u.MEM->u.BINOP.left, e2 = dst;
                    char buf[100];
                    sprintf(buf, "lw `d0, %d(`s0)\n", src->u.MEM->u.BINOP.right->u.CONST);
                    emit(AS_Oper(String(buf), Temp_TempList(munchExp(e2), NULL), Temp_TempList(munchExp(e1), NULL), NULL)); /* MOVE(TEMP(i),MEM(BINOP(PLUS,CONST(i),e1))) */
                }
                else if (src->u.MEM->kind == T_BINOP && src->u.MEM->u.BINOP.left->kind == T_CONST)
                {
                    T_exp e1 = src->u.MEM->u.BINOP.right, e2 = dst;
                    char buf[100];
                    sprintf(buf, "lw `d0, %d(`s0)\n", src->u.MEM->u.BINOP.left->u.CONST);
                    emit(AS_Oper(String(buf), Temp_TempList(munchExp(e2), NULL), Temp_TempList(munchExp(e1), NULL), NULL));
                }
                else if (src->u.MEM->kind == T_CONST) /* MOVE(TEMP(i),MEM(CONST(i))) */
                {
                    T_exp e2 = dst;
                    char buf[100];
                    sprintf(buf, "lw `d0, %d(`s0)\n", src->u.MEM->u.CONST);
                    emit(AS_Oper(String(buf), Temp_TempList(munchExp(e2), NULL), Temp_TempList(F_ZERO(), NULL), NULL));
                }
                else
                {
                    T_exp e1 = src->u.MEM, e2 = dst;
                    char buf[100];
                    sprintf(buf, "lw `d0, 0(`s0)\n");
                    emit(AS_Oper(String(buf), Temp_TempList(munchExp(e2), NULL), Temp_TempList(munchExp(e1), NULL), NULL));
                }
            }
            // else if (src->kind == T_BINOP)
            // {
            //     if (src->u.BINOP.right->kind == T_CONST)
            //     {
            //         T_exp e1 = src->u.BINOP.left;
            //         char buf[100];
            //         sprintf(buf, "addi `d0, `s0,%d\n", src->u.BINOP.right->u.CONST);
            //         emit(AS_Oper(String(buf), Temp_TempList(munchExp(dst), NULL), Temp_TempList(munchExp(e1), NULL), NULL));
            //     }
            //     else if (src->u.BINOP.left->kind == T_CONST)
            //     {
            //         T_exp e1 = src->u.BINOP.right;
            //         char buf[100];
            //         sprintf(buf, "addi `d0, `s0,%d\n", src->u.BINOP.left->u.CONST);
            //         emit(AS_Oper(String(buf), Temp_TempList(munchExp(dst), NULL), Temp_TempList(munchExp(e1), NULL), NULL));
            //     }
            //     else
            //     {
            //         T_exp e1 = src->u.BINOP.left;
            //         T_exp e2 = src->u.BINOP.right;
            //         char buf[100];
            //         sprintf(buf, "add `d0, `s0,`s1\n");
            //         emit(AS_Oper(String(buf), Temp_TempList(munchExp(dst), NULL), Temp_TempList(munchExp(e1), Temp_TempList(munchExp(e2), NULL)), NULL));
            //     }
            // }
            else if (src->kind == T_CONST)
            {
                //printf("T_MOVE 4\n");
                char buf[100];
                sprintf(buf, "li `d0, %d\n", src->u.CONST);
                emit(AS_Oper(String(buf), Temp_TempList(munchExp(dst), NULL), NULL, NULL));
            }
            else
            {
                emit(AS_Move(String("move `d0, `s0\n"), Temp_TempList(munchExp(dst), NULL), Temp_TempList(munchExp(src), NULL)));
            }
        }
        else
        {
            assert(0);
        }
        //printf("T_MOVE 4\n");
        break;
    }
    case T_JUMP:
    {
        // printf("T_JUMP 1\n");
        emit(AS_Oper(String("j `j0\n"), NULL, NULL, AS_Targets(s->u.JUMP.jumps)));
        emit(AS_Oper("nop\n", NULL, NULL, NULL));
        // printf("T_JUMP 2\n");
        break;
    }
    case T_CJUMP:
    {
        // printf("T_CJUMP 1\n");
        Temp_temp left = munchExp(s->u.CJUMP.left);
        Temp_temp right = munchExp(s->u.CJUMP.right);
        char buf[100];
        switch (s->u.CJUMP.op)
        {
        case T_eq:
            sprintf(buf, "%s `s0, `s1, `j0\n", "beq");
            break;
        case T_ne:
            sprintf(buf, "%s `s0, `s1, `j0\n", "bne");
            break;
        case T_lt:
            sprintf(buf, "%s `s0, `s1, `j0\n", "blt");
            break;
        case T_gt:
            sprintf(buf, "%s `s0, `s1, `j0\n", "bgt");
            break;
        case T_le:
            sprintf(buf, "%s `s0, `s1, `j0\n", "ble");
            break;
        case T_ge:
            sprintf(buf, "%s `s0, `s1, `j0\n", "bge");
            break;
        default:
            assert(0);
        }
        emit(AS_Oper(String(buf), NULL, Temp_TempList(left, Temp_TempList(right, NULL)), AS_Targets(Temp_LabelList(s->u.CJUMP._true, NULL))));
        emit(AS_Oper("nop\n", NULL, NULL, NULL));
        // printf("T_CJUMP 2\n");
        break;
    }
    case T_LABEL:
    {
        // printf("T_LABEL 1\n");
        char buf[100];
        sprintf(buf, "\n%s: \n", Temp_labelstring(s->u.label));
        emit(AS_Label(String(buf), s->u.label));
        // printf("T_LABEL 2\n");
        break;
    }
    case T_EXP:
    {
        // printf("T_EXP 1\n");
        munchExp(s->u.exp);
        break;
    }
    default:
        assert(0);
    }
}
static Temp_temp munchExp(T_exp e)
{
    switch (e->kind)
    {
    case T_BINOP:
    {
        // printf("T_BINOP 1\n");
        string op = NULL;
        T_exp e1 = e->u.BINOP.left, e2 = e->u.BINOP.right;
        int cst = e->u.BINOP.left->kind == T_CONST || e->u.BINOP.right->kind == T_CONST;
        switch (e->u.BINOP.op)
        {
        case T_plus:
            if (cst)
                op = String("addi");
            else
                op = String("add");
            break;
        case T_minus:
            op = String("sub");
            break;
        case T_mul:
            op = String("mul");
            break;
        case T_div:
            op = String("div");
            break;
        case T_and:
            if (cst)
                op = String("andi");
            else
                op = String("and");
            break;
        case T_or:
            if (cst)
                op = String("ori");
            else
                op = String("or");
            break;
        case T_xor:
            if (cst)
                op = String("xori");
            else
                op = String("xor");
            break;
        case T_lshift:
            op = String("sll");
            break;
        case T_rshift:
            op = String("srl");
            break;
        case T_arshift:
            op = String("sra");
            break;
        default:
            assert(!"Invalid operator");
        }
        if (e->u.BINOP.right->kind == T_CONST) /* BINOP(op, CONST(i), right) */
            return emit_binop_imm(op, e1, e->u.BINOP.right->u.CONST);
        else if (e->u.BINOP.left->kind == T_CONST) /* BINOP(op, left, CONST(i) */
            return emit_binop_imm(op, e2, e->u.BINOP.left->u.CONST);
        else /* BINOP(op, left, right) */
            return emit_binop_tri(op, e1, e2);
    }
    case T_MEM:
    {
        // printf("T_MEM 1\n");
        T_exp phase = e->u.MEM;
        Temp_temp t = Temp_newtemp();
        char buf[100];
        if (phase->kind == T_BINOP)
        {
            // printf("T_MEM 2\n");
            if (phase->u.BINOP.left->kind == T_CONST) /* MEM(BINOP(plus, CONST(i), e2)) */
            {
                T_exp e2 = phase->u.BINOP.right;
                sprintf(buf, "lw `d0,%d(`s0)\n", phase->u.BINOP.left->u.CONST);
                emit(AS_Oper(String(buf), Temp_TempList(t, NULL), Temp_TempList(munchExp(e2), NULL), NULL));

                return t;
            }
            else if (phase->u.BINOP.right->kind == T_CONST) /* MEM(BINOP(plus, e1, CONST(i))) */
            {
                T_exp e1 = phase->u.BINOP.left;
                sprintf(buf, "lw `d0,%d(`s0)\n", phase->u.BINOP.right->u.CONST);
                emit(AS_Oper(String(buf), Temp_TempList(t, NULL), Temp_TempList(munchExp(e1), NULL), NULL));

                return t;
            }
            else
            {
                // printf("T_MEM 2-3\n");
                emit(AS_Oper("lw `d0, 0(`s0)\n", Temp_TempList(t, NULL), Temp_TempList(munchExp(phase), NULL), NULL));
                // printf("T_MEM 2-4\n");
                return t;
            }
        }
        else if (phase->kind == T_CONST) /* MEM(CONST(i)) */
        {
            // printf("T_MEM 3\n");
            sprintf(buf, "lw `d0, %d(`s0)\n", phase->u.CONST);
            emit(AS_Oper(String(buf), Temp_TempList(t, NULL), Temp_TempList(F_ZERO(), NULL), NULL));
            return t;
        }
        else /* MEM(e1) */
        {
            // printf("T_MEM 4\n");
            sprintf(buf, "lw `d0,0(`s0)\n");
            emit(AS_Move(String(buf), Temp_TempList(t, NULL), Temp_TempList(munchExp(phase), NULL)));
            return t;
        }
    }
    case T_CONST:
    {
        // printf("T_CONST 1\n");
        Temp_temp t;
        if (e->u.CONST)
        {
            t = Temp_newtemp();
            char buf[100];
            sprintf(buf, "li `d0,%d\n", e->u.CONST);
            emit(AS_Oper(String(buf), Temp_TempList(t, NULL), NULL, NULL));
        }
        else
        {
            t = F_ZERO();
        }
        return t;
    }
    case T_NAME:
    {
        // printf("T_NAME 1\n");
        Temp_temp t = Temp_newtemp();
        char buf[100];
        sprintf(buf, "la `d0,%s\n", Temp_labelstring(e->u.NAME));
        emit(AS_Oper(String(buf), Temp_TempList(t, NULL), NULL, NULL));
        return t;
    }
    case T_CALL:
    {
        // printf("T_CALL 1\n");
        Temp_tempList l = munchArgs(e->u.CALL.args);
        // Temp_tempList l = NULL;
        char buf[100];
        sprintf(buf, "jal %s\n", Temp_labelstring(e->u.CALL.fun->u.NAME));
        emit(AS_Oper(String(buf), F_CallSaves(), l, NULL));
        emit(AS_Oper("nop\n", NULL, NULL, NULL));
        // printf("T_CALL 2\n");
        return F_RV();
    }
    case T_TEMP:
    {
        // printf("T_TEMP 1\n");
        return e->u.TEMP;
    }
    default:
        assert(0);
    }
}
static Temp_tempList munchArgs(T_expList args)
{
    int cnt = 0;
    Temp_temp sp = F_SP();
    Temp_tempList fars = F_ArgsRegs();

    T_expList el = args;
    Temp_tempList rlist = NULL, rlast = NULL;

    for (; el; el = el->tail)
    {
        Temp_temp t = munchExp(el->head);
        if (rlist == NULL)
        {
            rlist = Temp_TempList(t, NULL);
            rlast = rlist;
        }
        else
        {
            rlast->tail = Temp_TempList(t, NULL);
            rlast = rlast->tail;
        }

        if (cnt < 4)
        {
            if (t == F_FP())
                emit(AS_Oper(FormatString("addi `d0, `s0, 0\n"), Temp_TempList(fars->head, NULL), Temp_TempList(F_FP(), NULL), NULL));
            else
                emit(AS_Move("move `d0, `s0\n", Temp_TempList(fars->head, NULL), Temp_TempList(t, NULL)));
            fars = fars->tail;
        }
        else
        {
            char buf[100];
            sprintf(buf, "sw `s0, %d(`s1)\n", cnt * F_WORD_SIZE);
            emit(AS_Oper(String(buf), NULL, Temp_TempList(t, Temp_TempList(sp, NULL)), NULL));
        }
        cnt++;
    }
    return rlist;
}
AS_instrList As_rewriteSpill(F_frame f, AS_instrList il, Temp_tempList spills)
{
    for (; spills; spills = spills->tail)
    {
        Temp_temp t = spills->head;
        F_access acc = F_allocLocal(f, true);
        Temp_temp newTemp = Temp_newtemp();
        for (AS_instrList i = il; i; i = i->tail)
        {
            AS_instr instr = i->head;
            if (instr->kind == I_LABEL)
                continue;

            Temp_tempList src = NULL, dst = NULL;
            if (instr->kind == I_OPER)
            {
                src = instr->u.OPER.src;
                dst = instr->u.OPER.dst;
            }
            else
            {
                src = instr->u.MOVE.src;
                dst = instr->u.MOVE.dst;
            }

            if (Temp_tempListLook(src, t))
            {
                char buf[256];
                sprintf(buf, "lw `d0,%d(`s0)\n", F_getOffset(acc));
                AS_instr newInstr = AS_Oper(String(buf), Temp_TempList(newTemp, NULL), Temp_TempList(F_FP(), NULL), NULL);
                Temp_tempReplace(src, t, newTemp);
                i->tail = AS_InstrList(i->head, i->tail);
                i->head = newInstr;
                i = i->tail;
            }
            if (Temp_tempListLook(dst, t))
            {
                char buf[256];
                sprintf(buf, "sw `s0, %d(`s1)\n", F_getOffset(acc));
                AS_instr newInstr = AS_Oper(String(buf), NULL, Temp_TempList(F_FP(), Temp_TempList(newTemp, NULL)), NULL);
                Temp_tempReplace(dst, t, newTemp);
                i->tail = AS_InstrList(newInstr, i->tail);
                i = i->tail;
            }
        }
    }

    return il;
}
AS_instrList AS_rewrite(AS_instrList il, Temp_map coloring)
{
    AS_instrList prev = NULL;
    AS_instrList list = il;
    for (AS_instrList i = il; i; i = i->tail)
    {
        AS_instr instr = i->head;
        if (instr->kind == I_MOVE)
        {
            if (Temp_look(coloring, instr->u.MOVE.src->head) == Temp_look(coloring, instr->u.MOVE.dst->head))
            {
                if (prev)
                {
                    prev->tail = i->tail;
                    continue;
                }
                else
                {
                    list = i->tail;
                }
            }
        }
        else if (instr->kind == I_OPER && instr->u.OPER.jumps != NULL)
        {
            if (i->tail)
            {
                AS_instr next = i->tail->head;
                if (next->kind == I_LABEL && next->u.LABEL.label == instr->u.OPER.jumps->labels->head)
                {
                    i = i->tail;
                    if (prev)
                    {
                        prev->tail = i->tail;
                        continue;
                    }
                    else
                    {
                        list = i->tail;
                    }
                }
            }
        }

        prev = i;
    }
    return list;
}