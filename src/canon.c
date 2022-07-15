#include "canon.h"
#include <assert.h>

typedef struct expRefList_ *expRefList;
struct expRefList_
{
    T_exp *head;
    expRefList tail;
};
expRefList ExpRefList(T_exp *head, expRefList tail)
{
    expRefList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}
struct stmExp
{
    T_stm s;
    T_exp e;
};
struct stmExp StmExp(T_stm s, T_exp e)
{
    struct stmExp p;
    p.s = s;
    p.e = e;
    return p;
}
static C_stmListList C_StmListList(T_stmList head, C_stmListList tail)
{
    C_stmListList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}
static bool isNop(T_stm x)
{
    return x->kind == T_EXP && x->u.exp->kind == T_CONST;
}
static bool commute(T_stm x, T_exp y)
{
    return isNop(x) || y->kind == T_NAME || y->kind == T_CONST;
}
static T_stm seq(T_stm x, T_stm y)
{
    if (isNop(x))
        return y;
    if (isNop(y))
        return x;
    return T_Seq(x, y);
}
static expRefList get_call_rlist(T_exp exp)
{
    expRefList rlist, curr;
    curr = rlist = ExpRefList(&exp->u.CALL.fun, NULL);
    for (T_expList el = exp->u.CALL.args; el; el = el->tail)
        curr = curr->tail = ExpRefList(&el->head, NULL);
    return rlist;
}
static T_stm do_stm(T_stm stm);
static struct stmExp do_exp(T_exp exp);
static T_stm reorder(expRefList rlist);
static C_stmListList next(T_stmList prevstms, T_stmList stms, Temp_label done);
static C_stmListList mkBlocks(T_stmList stms, Temp_label done);
static T_stmList getLast(T_stmList list);
static void trace(T_stmList list);
static T_stmList getNext(void);

static T_stm do_stm(T_stm stm)
{
    switch (stm->kind)
    {
    case T_SEQ:
    {
        return seq(do_stm(stm->u.SEQ.left), do_stm(stm->u.SEQ.right));
    }
    case T_JUMP:
    {
        return seq(reorder(ExpRefList(&stm->u.JUMP.exp, NULL)), stm);
    }
    case T_CJUMP:
    {
        return seq(reorder(ExpRefList(&stm->u.CJUMP.left, ExpRefList(&stm->u.CJUMP.right, NULL))), stm);
    }
    case T_MOVE:
    {
        if (stm->u.MOve.dst->kind == T_TEMP && stm->u.MOve.src->kind == T_CALL)
            return seq(reorder(get_call_rlist(stm->u.MOve.src)), stm);
        if (stm->u.MOve.dst->kind == T_TEMP)
            return seq(reorder(ExpRefList(&stm->u.MOve.src, NULL)), stm);
        if (stm->u.MOve.dst->kind == T_MEM)
            return seq(reorder(ExpRefList(&stm->u.MOve.dst->u.MEM, ExpRefList(&stm->u.MOve.src, NULL))), stm);
        if (stm->u.MOve.dst->kind == T_ESEQ)
        {
            T_stm s = stm->u.MOve.dst->u.ESEQ.stm;
            stm->u.MOve.dst = stm->u.MOve.dst->u.ESEQ.exp;
            return do_stm(seq(s, stm));
        }
    }
    case T_EXP:
    {
        if (stm->u.exp->kind == T_CALL)
            return seq(reorder(get_call_rlist(stm->u.exp)), stm);
        else
            return seq(reorder(ExpRefList(&stm->u.exp, NULL)), stm);
    }
    default:
    {
        return stm;
    }
    }
}
static struct stmExp do_exp(T_exp exp)
{
    switch (exp->kind)
    {
    case T_BINOP:
        return StmExp(reorder(ExpRefList(&exp->u.BINOP.left, ExpRefList(&exp->u.BINOP.right, NULL))), exp);
    case T_MEM:
        return StmExp(reorder(ExpRefList(&exp->u.MEM, NULL)), exp);
    case T_ESEQ:
    {
        struct stmExp x = do_exp(exp->u.ESEQ.exp);
        T_stm s = do_stm(exp->u.ESEQ.stm);
        return StmExp(seq(s, x.s), x.e);
    }
    case T_CALL:
        return StmExp(reorder(get_call_rlist(exp)), exp);
    default:
        return StmExp(reorder(NULL), exp);
    }
}
static T_stm reorder(expRefList rlist)
{
    if (!rlist)
    {
        return T_Exp(T_Const(0));
    }

    if ((*rlist->head)->kind == T_CALL)
    {
        Temp_temp t = Temp_newtemp();
        *rlist->head = T_Eseq(T_Move(T_Temp(t), *rlist->head), T_Temp(t));
        return reorder(rlist);
    }

    struct stmExp hd = do_exp(*rlist->head);
    T_stm s = reorder(rlist->tail);
    if (commute(s, hd.e))
    {
        *rlist->head = hd.e;
        return seq(hd.s, s);
    }
    else
    {
        Temp_temp t = Temp_newtemp();
        *rlist->head = T_Temp(t);
        return seq(hd.s, T_Seq(T_Move(T_Temp(t), hd.e), s));
    }
}
static C_stmListList next(T_stmList prevstms, T_stmList stms, Temp_label done)
{
    if (!stms)
        return next(prevstms, T_StmList(T_Jump(T_Name(done), Temp_LabelList(done, NULL)), NULL), done);

    if (stms->head->kind == T_JUMP || stms->head->kind == T_CJUMP)
    {
        prevstms->tail = stms;
        C_stmListList stmLists = mkBlocks(stms->tail, done);
        stms->tail = NULL;
        return stmLists;
    }

    if (stms->head->kind == T_LABEL)
        return next(prevstms, T_StmList(T_Jump(T_Name(stms->head->u.label), Temp_LabelList(stms->head->u.label, NULL)), stms), done);

    prevstms->tail = stms;
    return next(stms, stms->tail, done);
}
static C_stmListList mkBlocks(T_stmList stms, Temp_label done)
{
    if (!stms)
        return NULL;

    if (stms->head->kind != T_LABEL)
        return mkBlocks(T_StmList(T_Label(Temp_newlabel()), stms), done);

    return C_StmListList(stms, next(stms, stms->tail, done));
}
static S_table block_env;
static struct C_block global_block;
static T_stmList getLast(T_stmList list)
{
    T_stmList last = list;
    while (last->tail->tail)
        last = last->tail;

    return last;
}
static void trace(T_stmList list)
{
    T_stmList last = getLast(list);
    T_stm lab = list->head;
    T_stm s = last->tail->head;
    S_enter(block_env, lab->u.label, NULL);
    if (s->kind == T_JUMP)
    {
        T_stmList target = (T_stmList)S_look(block_env, s->u.JUMP.jumps->head);
        if (!s->u.JUMP.jumps->tail && target)
        {
            last->tail = target;
            trace(target);
        }
        else
        {
            last->tail->tail = getNext();
        }
    }
    else if (s->kind == T_CJUMP)
    {
        T_stmList _true = (T_stmList)S_look(block_env, s->u.CJUMP._true);
        T_stmList _false = (T_stmList)S_look(block_env, s->u.CJUMP._false);
        if (_false)
        {
            last->tail->tail = _false;
            trace(_false);
        }
        else if (_true)
        {
            last->tail->head = T_Cjump(T_notRel(s->u.CJUMP.op), s->u.CJUMP.left, s->u.CJUMP.right, s->u.CJUMP._false, s->u.CJUMP._true);
            last->tail->tail = _true;
            trace(_true);
        }
        else
        {
            Temp_label ft = Temp_newlabel();
            last->tail->head = T_Cjump(s->u.CJUMP.op, s->u.CJUMP.left, s->u.CJUMP.right, s->u.CJUMP._true, ft);
            last->tail->tail = T_StmList(T_Label(ft), T_StmList(T_Jump(T_Name(s->u.CJUMP._false), Temp_LabelList(s->u.CJUMP._false, NULL)), getNext()));
        }
    }
    else
        assert(0);
}
static T_stmList getNext(void)
{
    if (!global_block.stmLists)
        return T_StmList(T_Label(global_block.label), NULL);

    T_stmList s = global_block.stmLists->head;
    if (S_look(block_env, s->head->u.label))
    {
        trace(s);
        return s;
    }
    else
    {
        global_block.stmLists = global_block.stmLists->tail;
        return getNext();
    }
}

T_stmList linear(T_stm stm, T_stmList right)
{
    if (stm->kind == T_SEQ)
        return linear(stm->u.SEQ.left, linear(stm->u.SEQ.right, right));
    else
        return T_StmList(stm, right);
}
T_stmList C_linearize(T_stm stm)
{
    return linear(do_stm(stm), NULL);
}
struct C_block C_basicBlocks(T_stmList stmList)
{
    struct C_block b;
    b.label = Temp_newlabel();
    b.stmLists = mkBlocks(stmList, b.label);
    return b;
}

T_stmList C_traceSchedule(struct C_block b)
{
    C_stmListList sList;
    block_env = S_empty();
    global_block = b;

    for (sList = b.stmLists; sList; sList = sList->tail)
        S_enter(block_env, sList->head->head->u.label, sList->head);

    return getNext();
}
