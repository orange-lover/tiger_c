#include <assert.h>
#include <string.h>
#include "frame.h"
#include "temp.h"
#include "util.h"
#include "assem.h"
#include "tree.h"

#define F_MAX_REGS 32

const int F_k = 6;
const int F_WORD_SIZE = 4;
// const int F_MAX_REGS = 32;

struct F_frame_
{
    Temp_label name;
    F_accessList formals;
    unsigned int locals;
    unsigned int max_argnum;
};

struct F_access_
{
    enum
    {
        inFrame,
        inReg
    } kind;
    union
    {
        int offset;
        Temp_temp reg;
    } u;
};

static Temp_temp regs[F_MAX_REGS];

static F_access InFrame(int offset);
static F_access InReg(Temp_temp reg);
static F_accessList F_AccessList(F_access head, F_accessList tail);

static void F_regs_init();
static Temp_tempList F_spec_regs(int r[], int n);

F_accessList F_make_formals(F_frame f, U_boolList formals)
{
    F_accessList ret = NULL, p = NULL;
    U_boolList tf = formals;
    int count = 0;
    for (; tf; tf = tf->tail)
    {
        F_access a;
        if (tf->head)
        {
            a = InFrame(count * F_WORD_SIZE);
        }
        else
        {
            a = InReg(Temp_newtemp());
        }
        if (ret == NULL)
        {
            ret = F_AccessList(a, NULL);
            p = ret;
        }
        else
        {
            p->tail = F_AccessList(a, NULL);
            p = p->tail;
        }
        count++;
    }
    return ret;
}
F_frame F_newFrame(Temp_label name, U_boolList formals)
{
    F_frame f = checked_malloc(sizeof(*f));
    f->name = name;
    f->locals = 0;
    f->max_argnum = 0;
    f->formals = F_make_formals(f, formals);
    return f;
}

Temp_label F_name(F_frame f)
{
    return f->name;
}
F_accessList F_formals(F_frame f)
{
    return f->formals;
}
F_access F_allocLocal(F_frame f, bool escape)
{
    F_access fa;
    if (escape)
    {
        f->locals++;
        fa = InFrame(-f->locals * F_WORD_SIZE);
    }
    else
    {
        fa = InReg(Temp_newtemp());
    }

    // if (!strcmp(Temp_labelstring(f->name), "tigerMain"))
    // {
    //     printf("%s:%d\n", Temp_labelstring(f->name), f->locals);
    // }

    return fa;
}

static F_access InFrame(int offset)
{
    F_access a = checked_malloc(sizeof(*a));
    a->kind = inFrame;
    a->u.offset = offset;
    return a;
}
static F_access InReg(Temp_temp reg)
{
    F_access a = checked_malloc(sizeof(*a));
    a->kind = inReg;
    a->u.reg = reg;
    return a;
}
static F_accessList F_AccessList(F_access head, F_accessList tail)
{
    F_accessList al = checked_malloc(sizeof(*al));
    al->head = head;
    al->tail = tail;
    return al;
}
int F_getOffset(F_access acc) { return acc->u.offset; }
void F_set_maxarg(F_frame f, int n)
{
    if (n > f->max_argnum)
        f->max_argnum = n;
};

F_frag F_StringFrag(Temp_label label, string str)
{
    F_frag p = checked_malloc(sizeof(*p));
    p->kind = F_stringFrag;
    p->u.stringg.label = label;
    p->u.stringg.str = str;
    return p;
}
F_frag F_ProcFrag(T_stm body, F_frame frame)
{
    F_frag p = checked_malloc(sizeof(*p));
    p->kind = F_procFrag;
    p->u.proc.body = body;
    p->u.proc.frame = frame;
    return p;
}
F_fragList F_FragList(F_frag head, F_fragList tail)
{
    F_fragList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}
T_exp F_Exp(F_access acc, T_exp framePtr)
{
    // printf("F_exp %d\n",framePtr);
    switch (acc->kind)
    {
    case inFrame:
        return T_Mem(T_Binop(T_plus, framePtr, T_Const(acc->u.offset)));
    case inReg:
        return T_Temp(acc->u.reg);
    }
    assert(0);
}
T_exp F_externalCall(string s, T_expList args)
{
    return T_Call(T_Name(Temp_namedlabel(s)), args);
}

static void F_regs_init()
{
    if (regs[0] == NULL)
    {
        for (int i = 0; i < F_MAX_REGS; i++)
        {
            regs[i] = Temp_newtemp();
        }
    }
}
static Temp_tempList F_spec_regs(int r[], int n)
{
    assert(n < F_MAX_REGS);
    if (regs[0] == NULL)
        F_regs_init();

    Temp_tempList list = NULL, last = NULL;
    for (int i = 0; i < n; i++)
    {
        if (list == NULL)
        {
            list = Temp_TempList(regs[r[i]], NULL);
            last = list;
        }
        else
        {
            last->tail = Temp_TempList(regs[r[i]], NULL);
            last = last->tail;
        }
    }
    return list;
}
Temp_map F_tempMap()
{
    static Temp_map tempMap = NULL;
    if (regs[0] == NULL)
        F_regs_init();

    if (tempMap == NULL)
    {
        tempMap = Temp_empty();
        Temp_enter(tempMap, regs[0], "$zero");
        Temp_enter(tempMap, regs[1], "$at");
        Temp_enter(tempMap, regs[2], "$v0");
        Temp_enter(tempMap, regs[3], "$v1");
        Temp_enter(tempMap, regs[4], "$a0");
        Temp_enter(tempMap, regs[5], "$a1");
        Temp_enter(tempMap, regs[6], "$a2");
        Temp_enter(tempMap, regs[7], "$a3");
        Temp_enter(tempMap, regs[8], "$t0");
        Temp_enter(tempMap, regs[9], "$t1");
        Temp_enter(tempMap, regs[10], "$t2");
        Temp_enter(tempMap, regs[11], "$t3");
        Temp_enter(tempMap, regs[12], "$t4");
        Temp_enter(tempMap, regs[13], "$t5");
        Temp_enter(tempMap, regs[14], "$t6");
        Temp_enter(tempMap, regs[15], "$t7");
        Temp_enter(tempMap, regs[16], "$s0");
        Temp_enter(tempMap, regs[17], "$s1");
        Temp_enter(tempMap, regs[18], "$s2");
        Temp_enter(tempMap, regs[19], "$s3");
        Temp_enter(tempMap, regs[20], "$s4");
        Temp_enter(tempMap, regs[21], "$s5");
        Temp_enter(tempMap, regs[22], "$s6");
        Temp_enter(tempMap, regs[23], "$s7");
        Temp_enter(tempMap, regs[24], "$t8");
        Temp_enter(tempMap, regs[25], "$t9");
        Temp_enter(tempMap, regs[26], "$k0");
        Temp_enter(tempMap, regs[27], "$k1");
        Temp_enter(tempMap, regs[28], "$gp");
        Temp_enter(tempMap, regs[29], "$sp");
        Temp_enter(tempMap, regs[30], "$fp");
        Temp_enter(tempMap, regs[31], "$ra");
        tempMap = Temp_layerMap(tempMap, Temp_name());
    }
    return tempMap;
}
/**
 * MIPS calling convention
 *
 * the $v registers are for function returns,
 * the $a registers are for function arguments,
 * the $t variables are temporary caller saved registers,
 * the $s registers are callee saved.
 */
Temp_temp F_ZERO()
{
    if (regs[0] == NULL)
        F_regs_init();

    return regs[0];
}
Temp_temp F_RV()
{
    if (regs[2] == NULL)
        F_regs_init();

    return regs[2];
}
Temp_temp F_SP()
{
    if (regs[29] == NULL)
        F_regs_init();

    return regs[29];
}
Temp_temp F_FP()
{
    if (regs[30] == NULL)
        F_regs_init();

    return regs[30];
}
Temp_temp F_RA()
{
    if (regs[31] == NULL)
        F_regs_init();

    return regs[31];
}
Temp_tempList F_CalleeSaves()
{
    static Temp_tempList list = NULL;
    if (list == NULL)
    {
        int index[] = {16, 17, 18, 19, 20, 21, 22, 23};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}
Temp_tempList F_CallerSaves()
{
    static Temp_tempList list = NULL;
    if (list == NULL)
    {
        int index[] = {8, 9, 10, 11, 12, 13, 14, 15, 24, 25};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}
Temp_tempList F_CallSaves()
{
    static Temp_tempList list = NULL;
    if (list == NULL)
    {
        int index[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}
static Temp_tempList list = NULL;
Temp_tempList F_ArgsRegs()
{
    if (list == NULL)
    {
        int index[] = {4, 5, 6, 7};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}
Temp_tempList F_registers()
{
    static Temp_tempList list = NULL;
    if (list == NULL)
    {
        for (int i = 24; i < F_MAX_REGS; i++)
            list = Temp_TempList(regs[i], list);
        for (int i = 0; i < 24; i++)
            list = Temp_TempList(regs[i], list);
    }
    return list;
}

T_stm F_procEntryExit1(F_frame frame, T_exp body)
{
    // int fc = 0;
    // F_accessList fcf = frame->formals;
    // for (; fcf; fcf = fcf->tail)
    // {
    //     fc++;
    // }
    // printf("%s,formals:%d\n", Temp_labelstring(frame->name), fc);
    // printf("%s,locals:%d\n", Temp_labelstring(frame->name), frame->locals);
    // //  ra
    F_access temp_ra = F_allocLocal(frame, true);
    T_stm pro = T_Move(F_Exp(temp_ra, T_Temp(F_FP())), T_Temp(F_RA()));
    // callee saves
    Temp_tempList calleesaves = F_CalleeSaves();
    F_accessList calleesaves_tmp = NULL, cp;
    for (; calleesaves; calleesaves = calleesaves->tail)
    {
        F_accessList t = F_AccessList(F_allocLocal(frame, true), NULL);
        if (calleesaves_tmp == NULL)
        {
            calleesaves_tmp = t;
            cp = t;
        }
        else
        {
            cp->tail = t;
            cp = cp->tail;
        }
        pro = T_Seq(pro, T_Move(F_Exp(t->head, T_Temp(F_FP())), T_Temp(calleesaves->head)));
    }
    // argument
    int i = 0;
    F_accessList args = frame->formals;
    Temp_tempList as = F_ArgsRegs();
    for (i = 0; args && i < 4; i++, args = args->tail, as = as->tail)
    {
        pro = T_Seq(pro, T_Move(F_Exp(args->head, T_Temp(F_FP())), T_Temp(as->head)));
    }
    while (args)
    {
        pro = T_Seq(pro, T_Move(F_Exp(args->head, T_Temp(F_FP())), T_Mem(T_Binop(T_plus, T_Temp(F_FP()), T_Const(i * F_WORD_SIZE)))));
        args = args->tail;
        i++;
    }

    // body
    T_stm epi = T_Move(T_Temp(F_RV()), body);

    Temp_tempList calleesaves_ = F_CalleeSaves();
    // callee saves
    for (; calleesaves_; calleesaves_ = calleesaves_->tail, calleesaves_tmp = calleesaves_tmp->tail)
    {

        // epi = T_Seq(epi, T_Exp(F_Exp(calleesaves_tmp->head, T_Temp(F_FP()))));
        T_stm p = T_Move(T_Temp(calleesaves_->head), F_Exp(calleesaves_tmp->head, T_Temp(F_FP())));
        epi = T_Seq(epi, p);
    }
    // ra
    epi = T_Seq(epi, T_Move(T_Temp(F_RA()), F_Exp(temp_ra, T_Temp(F_FP()))));
    return T_Seq(pro, epi);
}
AS_instrList F_procEntryExit2(AS_instrList body)
{
    static Temp_tempList returnSink = NULL;
    if (!returnSink)
    {
        returnSink = Temp_TempList(F_ZERO(),
                                   Temp_TempList(F_RA(),
                                                 Temp_TempList(F_SP(),
                                                               F_CalleeSaves())));
    }
    return AS_splice(body, AS_InstrList(AS_Oper("", NULL, returnSink, NULL), NULL));
}
AS_proc F_procEntryExit3(F_frame frame, AS_instrList body)
{
    int framesize = (frame->locals + frame->max_argnum + 1) * F_WORD_SIZE;
    // printf("%s,locals:%d,max_argnum:%d,framesize:%d\n", S_name(frame->name), frame->locals, frame->max_argnum, framesize);
    char buf[100];
    sprintf(buf,
            "%s:\n"
            "sw $fp,%d($sp)\n"
            "move $fp,$sp\n"
            "addi $sp,$sp,%d\n",
            Temp_labelstring(frame->name), -framesize + (frame->max_argnum) * F_WORD_SIZE, -framesize);
    string prolog = String(buf);
    sprintf(buf,
            "addi $sp,$sp,%d\n"
            "lw $fp,%d($sp)\n"
            "jr $ra\n",
            framesize, -framesize + (frame->max_argnum ) * F_WORD_SIZE);
    string epilog = String(buf);
    return AS_Proc(prolog, body, epilog);
}

string F_string(F_frag frag)
{
    char buffer[100];
    string s = frag->u.stringg.str;
    int count = 0, flag = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == '\\' && flag == 0)
        {
            flag = 1;
            continue;
        }

        flag = 0;
        count++;
    }
    sprintf(buffer, "%s:\n.word %d\n.ascii \"%s\"\n", Temp_labelstring(frag->u.stringg.label), count, frag->u.stringg.str);
    return String(buffer);
}