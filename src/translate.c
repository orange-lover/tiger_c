#include <stdio.h>
#include <assert.h>
#include "translate.h"
// #include "frame.h"
#include "util.h"
#include "absyn.h"
#include "printtree.h"
#include "frame.h"

struct Tr_level_
{
    Tr_level parent;
    Temp_label name;
    F_frame frame;
    Tr_accessList formals;
};
struct Tr_access_
{
    Tr_level level;
    F_access accsee;
};

typedef struct patchList_ *patchList;
struct patchList_
{
    Temp_label *head;
    patchList tail;
};
static patchList PatchList(Temp_label *head, patchList tail);
static void doPatch(patchList tList, Temp_label label);
static patchList joinPatch(patchList first, patchList second);

struct Cx
{
    patchList trues;
    patchList falses;
    T_stm stm;
};
struct Tr_exp_
{
    enum
    {
        Tr_ex,
        Tr_nx,
        Tr_cx
    } kind;
    union
    {
        T_exp ex;
        T_stm nx;
        struct Cx cx;
    } u;
};

static Tr_exp Tr_Ex(T_exp ex);
static Tr_exp Tr_Nx(T_stm nx);
static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm);
// T_exp Tr_unEx(Tr_exp e);
// T_stm Tr_unNx(Tr_exp e);
// struct Cx Tr_unCx(Tr_exp e);

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail)
{
    Tr_accessList al = checked_malloc(sizeof(*al));
    al->head = head;
    al->tail = tail;
    return al;
}
Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail)
{
    Tr_expList el = checked_malloc(sizeof(*el));
    el->head = head;
    el->tail = tail;
    return el;
}

static Tr_level outer = NULL;
Tr_level Tr_outermost(void)
{
    if (!outer)
        outer = Tr_newLevel(NULL, Temp_namedlabel("tigerMain"), NULL);
    return outer;
}
Tr_access Tr_Access(Tr_level level, F_access access)
{
    Tr_access a = checked_malloc(sizeof(*a));
    a->level = level;
    a->accsee = access;
    return a;
}
Tr_accessList Tr_make_formals(Tr_level level)
{
    Tr_accessList head = NULL, tail = NULL;
    F_accessList al = F_formals(level->frame);
    if (al == NULL)
        return NULL;

    for (al = al->tail; al; al = al->tail)
    {
        if (head)
        {
            tail->tail = Tr_AccessList(Tr_Access(level, al->head), NULL);
            tail = tail->tail;
        }
        else
        {
            head = Tr_AccessList(Tr_Access(level, al->head), NULL);
            tail = head;
        }
    }
    return head;
}
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals)
{
    Tr_level l = checked_malloc(sizeof(*l));
    l->parent = parent;
    l->name = name;
    l->frame = F_newFrame(name, U_BoolList(true, formals));
    l->formals = Tr_make_formals(l);
    return l;
}
Tr_accessList Tr_formals(Tr_level level)
{
    return level->formals;
}
Tr_access Tr_allocLocal(Tr_level level, bool escape)
{
    return Tr_Access(level, F_allocLocal(level->frame, escape));
}
string Tr_getLevelName(Tr_level level)
{
    return Temp_labelstring(F_name(level->frame));
}

static F_fragList global_string = NULL;
static F_fragList frag_list = NULL;
void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals)
{
    T_stm stm = F_procEntryExit1(level->frame, Tr_unEx(body));
    F_frag procFrag = F_ProcFrag(stm, level->frame);
    frag_list = F_FragList(procFrag, frag_list);
}
F_fragList Tr_getResult(void)
{
    F_fragList last_string_list = NULL, p = NULL;
    for (p = global_string; p; p = p->tail)
        last_string_list = p;
    if (last_string_list)
        last_string_list->tail = frag_list;

    return global_string ? global_string : frag_list;
}

static patchList PatchList(Temp_label *head, patchList tail)
{
    patchList pl = checked_malloc(sizeof(*pl));
    pl->head = head;
    pl->tail = tail;
    return pl;
}
static void doPatch(patchList tList, Temp_label label)
{
    for (; tList; tList = tList->tail)
        *(tList->head) = label;
}
static patchList joinPatch(patchList first, patchList second)
{
    if (!first)
        return second;
    for (; first->tail; first = first->tail)
        ;
    first->tail = second;
    return first;
}

static Tr_exp Tr_Ex(T_exp ex)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p->kind = Tr_ex;
    p->u.ex = ex;
    return p;
}
static Tr_exp Tr_Nx(T_stm nx)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p->kind = Tr_nx;
    p->u.nx = nx;
    return p;
}
static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p->kind = Tr_cx;
    p->u.cx.trues = trues;
    p->u.cx.falses = falses;
    p->u.cx.stm = stm;
    return p;
}
T_exp Tr_unEx(Tr_exp e)
{
    switch (e->kind)
    {
    case Tr_ex:
        return e->u.ex;
    case Tr_nx:
        return T_Eseq(e->u.nx, T_Const(0));
    case Tr_cx:
    {
        Temp_temp r = Temp_newtemp();
        Temp_label t = Temp_newlabel();
        Temp_label f = Temp_newlabel();
        doPatch(e->u.cx.trues, t);
        doPatch(e->u.cx.falses, f);
        return T_Eseq(T_Move(T_Temp(r), T_Const(1)),
                      T_Eseq(e->u.cx.stm,
                             T_Eseq(T_Label(f),
                                    T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                                           T_Eseq(T_Label(t),
                                                  T_Temp(r))))));
    }
    }
    assert(0);
}
T_stm Tr_unNx(Tr_exp e)
{
    switch (e->kind)
    {
    case Tr_ex:
        return T_Exp(e->u.ex);
    case Tr_nx:
        return e->u.nx;
    case Tr_cx:
    {
        Temp_temp r = Temp_newtemp();
        Temp_label t = Temp_newlabel();
        Temp_label f = Temp_newlabel();
        doPatch(e->u.cx.trues, t);
        doPatch(e->u.cx.falses, f);
        return T_Exp(T_Eseq(T_Move(T_Temp(r), T_Const(1)),
                            T_Eseq(e->u.cx.stm,
                                   T_Eseq(T_Label(f),
                                          T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                                                 T_Eseq(T_Label(t),
                                                        T_Temp(r)))))));
    }
    }
    assert(0);
}
struct Cx Tr_unCx(Tr_exp e)
{
    switch (e->kind)
    {
    case Tr_ex:
    {
        struct Cx cx;
        cx.stm = T_Cjump(T_ne, e->u.ex, T_Const(0), NULL, NULL);
        cx.trues = PatchList(&(cx.stm->u.CJUMP._true), NULL);
        cx.falses = PatchList(&(cx.stm->u.CJUMP._false), NULL);
        return cx;
    }
    case Tr_nx:
        assert(0);
    case Tr_cx:
        return e->u.cx;
    }
    assert(0);
}

Tr_exp Tr_noExp()
{
    return Tr_Ex(T_Const(0));
}
// static Temp_temp nil = NULL;
Tr_exp Tr_nilExp()
{
    // if (!nil)
    // {
    //     nil = Temp_newtemp();
    //     return Tr_Ex(T_Eseq(T_Move(T_Temp(nil), F_externalCall("allocRecord", T_ExpList(T_Const(0), NULL))), T_Temp(nil)));
    // }
    // return Tr_Ex(T_Temp(nil));
    return Tr_Ex(T_Const(0));
}
Tr_exp Tr_intExp(int n)
{
    return Tr_Ex(T_Const(n));
}
/**
 * literal string, 在其他语言中被存储在堆里作为全局字符串
 * 这里维护一个fraglist表示全局字符串, 为每个字符串生成label, 即memory address
 */
Tr_exp Tr_stringExp(string s)
{
    Temp_label str = Temp_newlabel();
    global_string = F_FragList(F_StringFrag(str, s), global_string);
    return Tr_Ex(T_Name(str));
}
Tr_exp Tr_simpleVar(Tr_access access, Tr_level level)
{
    T_exp fp = T_Temp(F_FP());
    Tr_level current_level = level;
    while (access->level != current_level)
    {
        fp = F_Exp(F_formals(current_level->frame)->head, fp);
        current_level = current_level->parent;
    }

    return Tr_Ex(F_Exp(access->accsee, fp));
}
Tr_exp Tr_fieldVar(Tr_exp record, int offset)
{
    // printf("Tr_fieldVar %d\n",offset * F_WORD_SIZE);
    return Tr_Ex(T_Mem(T_Binop(T_plus, Tr_unEx(record), T_Const(offset * F_WORD_SIZE))));
}
Tr_exp Tr_subscriptVar(Tr_exp array, Tr_exp index)
{
    // printf("Tr_subscriptVar %d\n",F_WORD_SIZE);
    return Tr_Ex(T_Mem(T_Binop(T_plus, Tr_unEx(array), T_Binop(T_mul, Tr_unEx(index), T_Const(F_WORD_SIZE)))));
}
Tr_exp Tr_arithExp(A_oper op, Tr_exp left, Tr_exp right)
{

    T_binOp binop;
    switch (op)
    {
    case A_plusOp:
        binop = T_plus;
        break;
    case A_minusOp:
        binop = T_minus;
        break;
    case A_timesOp:
        binop = T_mul;
        break;
    case A_divideOp:
        binop = T_div;
        break;
    default:
        printf("%s:%d arithExp cannot handle other operators", __FILE__, __LINE__);
    }
    return Tr_Ex(T_Binop(binop, Tr_unEx(left), Tr_unEx(right)));
}
Tr_exp Tr_relExp(A_oper op, Tr_exp left, Tr_exp right)
{
    T_relOp rop;
    switch (op)
    {
    case A_eqOp:
        rop = T_eq;
        break;
    case A_neqOp:
        rop = T_ne;
        break;
    case A_ltOp:
        rop = T_lt;
        break;
    case A_leOp:
        rop = T_le;
        break;
    case A_gtOp:
        rop = T_gt;
        break;
    case A_geOp:
        rop = T_ge;
        break;
    default:
        printf("%s:%d relExp cannot handle other operators", __FILE__, __LINE__);
    }
    T_stm stm = T_Cjump(rop, Tr_unEx(left), Tr_unEx(right), NULL, NULL);
    patchList trues = PatchList(&stm->u.CJUMP._true, NULL);
    patchList falses = PatchList(&stm->u.CJUMP._false, NULL);
    return Tr_Cx(trues, falses, stm);
}
Tr_exp Tr_callExp(Tr_level call_level, Tr_level func_level, Temp_label name, Tr_expList args)
{
    if (func_level != NULL)
    {
        T_exp fp = T_Temp(F_FP());
        while (call_level != func_level->parent)
        {
            F_accessList al = F_formals(call_level->frame);
            fp = F_Exp(al->head, fp);
            call_level = call_level->parent;
        }
        T_expList slargs = T_ExpList(fp, NULL);
        T_expList p = slargs;
        Tr_expList el = NULL;
        for (el = args; el; el = el->tail)
        {
            p->tail = T_ExpList(Tr_unEx(el->head), NULL);
            p = p->tail;
        }
        return Tr_Ex(T_Call(T_Name(name), slargs));
    }
    else
    {

        T_expList slargs = NULL;
        T_expList p = NULL;
        Tr_expList el = NULL;
        for (el = args; el; el = el->tail)
        {
            if (slargs == NULL)
            {
                slargs = T_ExpList(Tr_unEx(el->head), NULL);
                p = slargs;
            }
            else
            {
                p->tail = T_ExpList(Tr_unEx(el->head), NULL);
                p = p->tail;
            }
        }
        return Tr_Ex(T_Call(T_Name(name), slargs));
    }
}
Tr_exp Tr_recordExp(Tr_expList fields, int n_fields)
{
    Temp_temp t = Temp_newtemp();
    T_stm record_malloc = T_Move(T_Temp(t), F_externalCall("allocRecord", T_ExpList(T_Const(n_fields * F_WORD_SIZE), NULL)));

    Tr_expList el = NULL, head = NULL, tail = NULL;
    for (el = fields; el; el = el->tail)
    {
        if (head == NULL)
        {
            head = Tr_ExpList(el->head, NULL);
            tail = head;
        }
        else
        {
            tail->tail = Tr_ExpList(el->head, NULL);
            tail = tail->tail;
        }
    }
    T_stm seq = T_Move(T_Mem(T_Binop(T_plus, T_Temp(t), T_Const(0))), Tr_unEx(head->head));
    for (int i = 1; i < n_fields; i++)
    {
        head = head->tail;
        // printf("Tr_recordExp %d\n",i * F_WORD_SIZE);
        seq = T_Seq(T_Move(T_Mem(T_Binop(T_plus, T_Temp(t), T_Const(i * F_WORD_SIZE))), Tr_unEx(head->head)), seq);
    }

    return Tr_Ex(T_Eseq(T_Seq(record_malloc, seq), T_Temp(t)));
}
Tr_exp Tr_seqExp(Tr_expList seqs)
{
    if (seqs == NULL)
    {
        return Tr_Nx(T_Exp(T_Const(0)));
    }

    Tr_expList el = seqs;
    T_exp s = Tr_unEx(el->head);
    for (el = el->tail; el; el = el->tail)
        s = T_Eseq(T_Exp(s), Tr_unEx(el->head));
    return Tr_Ex(s);
}
Tr_exp Tr_assignExp(Tr_exp left, Tr_exp right)
{
    return Tr_Nx(T_Move(Tr_unEx(left), Tr_unEx(right)));
}
Tr_exp Tr_ifExp(Tr_exp test, Tr_exp then, Tr_exp elsee)
{
    Tr_exp con_exp;

    struct Cx c = Tr_unCx(test);
    Temp_label t = Temp_newlabel();
    Temp_label f = Temp_newlabel();
    doPatch(c.trues, t);
    doPatch(c.falses, f);

    if (elsee)
    {
        Temp_label z = Temp_newlabel();
        T_stm jump = T_Jump(T_Name(z), Temp_LabelList(z, NULL));

        // if (elsee->kind == Tr_ex)
        // {
        Temp_temp r = Temp_newtemp();
        return Tr_Ex(T_Eseq(c.stm,
                            T_Eseq(T_Label(t),
                                   T_Eseq(T_Move(T_Temp(r), Tr_unEx(then)),
                                          T_Eseq(jump,
                                                 T_Eseq(T_Label(f),
                                                        T_Eseq(T_Move(T_Temp(r), Tr_unEx(elsee)),
                                                               // T_Eseq(jump,
                                                               T_Eseq(T_Label(z), T_Temp(r)))))))));
        // }
        // else
        // {
        //     T_stm then_stm = Tr_unNx(then);
        //     T_stm else_stm = Tr_unNx(elsee);
        //     return Tr_Nx(T_Seq(c.stm,
        //                        T_Seq(T_Label(t),
        //                              T_Seq(then_stm,
        //                                    T_Seq(jump,
        //                                          T_Seq(T_Label(f),
        //                                                T_Seq(else_stm, T_Label(z))))))));
        // }
    }
    else
    {
        T_stm stm_then = Tr_unNx(then);
        // switch (then->kind)
        // {
        // case Tr_ex:
        //     stm_then = T_Exp(then->u.ex);
        // case Tr_nx:
        //     stm_then = then->u.nx;
        // case Tr_cx:
        //     stm_then = then->u.cx.stm;
        // }
        return Tr_Nx(T_Seq(c.stm, T_Seq(T_Label(t), T_Seq(stm_then, T_Label(f)))));
    }
}
Tr_exp Tr_whileExp(Tr_exp test, Tr_exp done, Tr_exp body)
{
    Temp_label test_label = Temp_newlabel(), body_label = Temp_newlabel();
    Tr_exp rt = Tr_Nx(T_Seq(T_Jump(T_Name(test_label), Temp_LabelList(test_label, NULL)),
                            T_Seq(T_Label(body_label),
                                  T_Seq(Tr_unNx(body),
                                        T_Seq(T_Label(test_label),
                                              T_Seq(T_Cjump(T_eq, Tr_unEx(test), T_Const(0), Tr_unEx(done)->u.NAME, body_label), T_Label(Tr_unEx(done)->u.NAME)))))));

    // pr_stm(stdout, Tr_unNx(rt), 0);
    return rt;
}
Tr_exp Tr_doneExp()
{
    return Tr_Ex(T_Name(Temp_newlabel()));
}
Tr_exp Tr_breakExp(Tr_exp brk)
{
    return Tr_Nx(T_Jump(Tr_unEx(brk), Temp_LabelList(Tr_unEx(brk)->u.NAME, NULL)));
}
Tr_exp Tr_letExp(Tr_expList exps)
{
    Tr_expList el = exps;
    T_stm s = Tr_unNx(el->head);
    for (el = el->tail; el; el = el->tail)
    {
        if (el->tail == NULL)
            break;
        s = T_Seq(s, Tr_unNx(el->head));
    }
    return Tr_Ex(T_Eseq(s, Tr_unEx(el->head)));
}
Tr_exp Tr_arrayExp(Tr_exp size, Tr_exp init)
{
    return Tr_Ex(F_externalCall(String("initArray"), T_ExpList(Tr_unEx(size), T_ExpList(Tr_unEx(init), NULL))));
}

void Tr_setargnum(Tr_level level, int n)
{
    F_set_maxarg(level->frame, n + 1);
}
void Tr_printTree(Tr_exp e)
{
    T_stmList sl = T_StmList(Tr_unNx(e), NULL);
    printStmList(stdout, sl);
}