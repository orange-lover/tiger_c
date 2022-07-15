#include <assert.h>
#include "tree.h"
#include "util.h"

T_stmList T_StmList(T_stm head, T_stmList tail)
{
    T_stmList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}
T_expList T_ExpList(T_exp head, T_expList tail)
{
    T_expList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}

T_stm T_Seq(T_stm left, T_stm right)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_SEQ;
    p->u.SEQ.left = left;
    p->u.SEQ.right = right;
    return p;
}
T_stm T_Label(Temp_label label)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_LABEL;
    p->u.label = label;
    return p;
}
T_stm T_Jump(T_exp exp, Temp_labelList jumps)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_JUMP;
    p->u.JUMP.exp = exp;
    p->u.JUMP.jumps = jumps;
    return p;
}
T_stm T_Cjump(T_relOp op, T_exp left, T_exp right, Temp_label _true, Temp_label _false)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_CJUMP;
    p->u.CJUMP.op = op;
    p->u.CJUMP.left = left;
    p->u.CJUMP.right = right;
    p->u.CJUMP._true = _true;
    p->u.CJUMP._false = _false;
    return p;
}
T_stm T_Move(T_exp dst, T_exp src)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_MOVE;
    p->u.MOve.dst = dst;
    p->u.MOve.src = src;
    return p;
}
T_stm T_Exp(T_exp exp)
{
    T_stm p = checked_malloc(sizeof(*p));
    p->kind = T_EXP;
    p->u.exp = exp;
    return p;
}

T_exp T_Binop(T_binOp op, T_exp left, T_exp right)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_BINOP;
    p->u.BINOP.op = op;
    p->u.BINOP.left = left;
    p->u.BINOP.right = right;
    return p;
}
T_exp T_Mem(T_exp mem)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_MEM;
    p->u.MEM = mem;
    return p;
}
T_exp T_Temp(Temp_temp temp)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_TEMP;
    p->u.TEMP = temp;
    return p;
}
T_exp T_Eseq(T_stm stm, T_exp exp)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_ESEQ;
    p->u.ESEQ.stm = stm;
    p->u.ESEQ.exp = exp;
    return p;
}
T_exp T_Name(Temp_label name)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_NAME;
    p->u.NAME = name;
    return p;
}
T_exp T_Const(int num)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_CONST;
    p->u.CONST = num;
    return p;
}
T_exp T_Call(T_exp fun, T_expList args)
{
    T_exp p = checked_malloc(sizeof(*p));
    p->kind = T_CALL;
    p->u.CALL.fun = fun;
    p->u.CALL.args = args;
    return p;
}

T_relOp T_notRel(T_relOp op)
{
    switch (op)
    {
    case T_eq:
        return T_ne;
    case T_ne:
        return T_eq;
    case T_lt:
        return T_ge;
    case T_gt:
        return T_le;
    case T_le:
        return T_gt;
    case T_ge:
        return T_lt;
    case T_ult:
        return T_uge;
    case T_ugt:
        return T_ule;
    case T_ule:
        return T_ugt;
    case T_uge:
        return T_ult;
    }
    assert(0);
    return 0;
}
T_relOp T_commute(T_relOp op)
{
    switch (op)
    {
    case T_eq:
        return T_eq;
    case T_ne:
        return T_ne;
    case T_lt:
        return T_gt;
    case T_gt:
        return T_lt;
    case T_le:
        return T_ge;
    case T_ge:
        return T_le;
    case T_ult:
        return T_ugt;
    case T_ugt:
        return T_ult;
    case T_ule:
        return T_uge;
    case T_uge:
        return T_ule;
    }
    assert(0);
    return 0;
}