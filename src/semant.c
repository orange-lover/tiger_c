#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "util.h"
#include "types.h"
#include "semant.h"
#include "symbol.h"
#include "absyn.h"
#include "env.h"
#include "errormsg.h"
#include "translate.h"
#include "frame.h"
#include "prabsyn.h"
#include <string.h>
#include "printtree.h"

typedef struct expty_
{
    Tr_exp exp;
    Ty_ty ty;
} expty;
expty expTy(Tr_exp exp, Ty_ty ty)
{
    expty p;
    p.exp = exp;
    p.ty = ty;
    return p;
};
int inside = 0;
static Tr_exp brk[16];

expty transVar(Tr_level level, S_table venv, S_table tenv, A_var v);
expty transExp(Tr_level level, S_table venv, S_table tenv, A_exp a);
Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, A_dec d);
Ty_ty transTy(S_table tenv, A_ty a);

static Ty_ty actual_ty(Ty_ty ty)
{
    Ty_ty t = ty;
    while (t->kind == Ty_name)
        t = t->u.name.ty;
    return t;
}
static bool actual_eq(Ty_ty source, Ty_ty target)
{
    Ty_ty t1 = actual_ty(source);
    Ty_ty t2 = actual_ty(target);
    return ((t1->kind == Ty_record || t1->kind == Ty_array) && t1 == t2) || (t1->kind == Ty_record && t2->kind == Ty_nil) || (t1->kind == Ty_nil && t2->kind == Ty_record) || (t1->kind != Ty_record && t1->kind != Ty_array && t1->kind == t2->kind);
}

F_fragList SEM_transProg(A_exp exp)
{
    S_table tenv = E_base_tenv();
    S_table venv = E_base_venv();
    Tr_level level = Tr_outermost();
    expty trans_exp = transExp(level, venv, tenv, exp);
    Tr_procEntryExit(level, trans_exp.exp, NULL);
    return Tr_getResult();
}

expty transVar(Tr_level level, S_table venv, S_table tenv, A_var v)
{
    switch (v->kind)
    {
    case A_simpleVar:
    {
        E_enventry x = S_look(venv, v->u.simple);
        if (x->kind == E_varEntry)
            return expTy(Tr_simpleVar(x->u.var.access, level), actual_ty(x->u.var.ty));
        else
        {
            EM_error(v->pos, "simple var expression:undefined variable %s", S_name(v->u.simple));
            exit(1);
        }
    }
    case A_fieldVar:
    {
        expty var = transVar(level, venv, tenv, v->u.field.var);
        if (var.ty->kind != Ty_record)
        {
            EM_error(v->pos, "field var expression: not a record type");
            exit(1);
        }
        int offset = 0;
        for (Ty_fieldList fl = var.ty->u.record; fl; fl = fl->tail, offset++)
        {
            if (fl->head->name == v->u.field.sym)
                return expTy(Tr_fieldVar(var.exp, offset), actual_ty(fl->head->ty));
        }

        EM_error(v->pos, "field var expression: not a such field <%s> in record", S_name(v->u.field.sym));
        exit(1);
    }
    case A_subscriptVar:
    {
        expty var = transVar(level, venv, tenv, v->u.subscript.var);
        if (var.ty->kind != Ty_array)
        {
            EM_error(v->pos, "subscript var expression: not a array type");
            exit(1);
        }
        expty index = transExp(level, venv, tenv, v->u.subscript.exp);
        if (index.ty->kind != Ty_int)
        {
            EM_error(v->pos, "subscript var expression: integer required in array index");
            exit(1);
        }

        return expTy(Tr_subscriptVar(var.exp, index.exp), actual_ty(var.ty->u.array));
    }
    }
    assert(0);
}
expty transExp(Tr_level level, S_table venv, S_table tenv, A_exp a)
{
    switch (a->kind)
    {
    case A_varExp:
        return transVar(level, venv, tenv, a->u.var);
    case A_nilExp:
        return expTy(Tr_nilExp(), Ty_Nil());
    case A_intExp:
        return expTy(Tr_intExp(a->u.intt), Ty_Int());
    case A_stringExp:
        return expTy(Tr_stringExp(a->u.stringg), Ty_String());
    case A_callExp:
    {
        E_enventry fun = S_look(venv, a->u.call.func);
        if (fun == NULL || fun->kind != E_funEntry)
        {
            EM_error(a->pos, "call expression: undefined type %s", S_name(a->u.call.func));
            exit(1);
        }
        A_expList el;
        Ty_tyList tl = fun->u.fun.formals;
        Tr_expList tr_el = NULL, tr_eld = NULL;
        int count = 0;
        for (el = a->u.call.args; el; el = el->tail, tl = tl->tail)
        {
            expty exp = transExp(level, venv, tenv, el->head);
            Ty_ty ty = actual_ty(tl->head);
            if (!actual_eq(exp.ty, ty))
            {
                EM_error(a->pos, "call expression: argument type dosen't match the paramater");
                exit(1);
            }
            if (tr_el == NULL)
            {
                tr_el = Tr_ExpList(exp.exp, NULL);
                tr_eld = tr_el;
            }
            else
            {
                tr_eld->tail = Tr_ExpList(exp.exp, NULL);
                tr_eld = tr_eld->tail;
            }
            count++;
        }
        if (el != NULL)
        {
            EM_error(a->pos, "call expression: too many arguments");
            exit(1);
        }
        if (tl != NULL)
        {
            EM_error(a->pos, "call expression: not enough arguments");
            exit(1);
        }

        Tr_setargnum(level, count);
        expty ret = expTy(Tr_callExp(level, fun->u.fun.level, fun->u.fun.label, tr_el), actual_ty(fun->u.fun.result));
        return ret;
    }
    case A_opExp:
    {
        expty left = transExp(level, venv, tenv, a->u.op.left);
        expty right = transExp(level, venv, tenv, a->u.op.right);
        switch (a->u.op.oper)
        {
        case A_plusOp:
        case A_minusOp:
        case A_timesOp:
        case A_divideOp:
            if (left.ty->kind != Ty_int)
            {
                EM_error(a->u.op.left->pos, "binary operation: integer required");
                exit(1);
            }
            if (right.ty->kind != Ty_int)
            {
                EM_error(a->u.op.left->pos, "binary operation: integer required");
                exit(1);
            }
            return expTy(Tr_arithExp(a->u.op.oper, left.exp, right.exp), Ty_Int());
        case A_eqOp:
        case A_neqOp:
            if (!actual_eq(left.ty, right.ty))
            {
                EM_error(a->pos, "operators compare: different type for compare");
                exit(1);
            }
            return expTy(Tr_relExp(a->u.op.oper, left.exp, right.exp), Ty_Int());
        case A_ltOp:
        case A_leOp:
        case A_gtOp:
        case A_geOp:
            if (left.ty->kind != Ty_int || right.ty->kind != Ty_int)
            {
                EM_error(a->pos, "binary compare: integer required");
                exit(1);
            }
            return expTy(Tr_relExp(a->u.op.oper, left.exp, right.exp), Ty_Int());
        }
        assert(0);
    }
    case A_recordExp:
    {
        Ty_ty recordty = S_look(tenv, a->u.record.typ);
        if (recordty == NULL)
        {
            EM_error(a->pos, "record expression: no such record type");
            exit(1);
        }
        Ty_ty actual = actual_ty(recordty);
        if (actual->kind != Ty_record)
        {
            EM_error(a->pos, "record expression: not record type");
            exit(1);
        }

        A_efieldList efl;
        Ty_fieldList fl = actual->u.record;
        Tr_expList tr_el = NULL, tr_eld = NULL;
        int n_fields = 0;
        for (efl = a->u.record.fields; efl; efl = efl->tail, fl = fl->tail)
        {
            if (efl->head->name != fl->head->name)
            {
                EM_error(a->pos, "record expression: not valid record field name");
                exit(1);
            }
            expty exp = transExp(level, venv, tenv, efl->head->exp);
            if (!actual_eq(exp.ty, fl->head->ty))
            {
                EM_error(a->pos, "record expression: record field not matched");
                exit(1);
            }
            if (tr_el == NULL)
            {
                tr_el = Tr_ExpList(exp.exp, NULL);
                tr_eld = tr_el;
            }
            else
            {
                tr_eld->tail = Tr_ExpList(exp.exp, NULL);
                tr_eld = tr_eld->tail;
            }
            n_fields++;
        }
        return expTy(Tr_recordExp(tr_el, n_fields), actual);
    }
    case A_seqExp:
    {
        Ty_ty ret = Ty_Void();
        A_expList el;
        Tr_expList tr_el = NULL, tr_eld = NULL;
        for (el = a->u.seq; el; el = el->tail)
        {
            expty e = transExp(level, venv, tenv, el->head);
            if (tr_el == NULL)
            {
                tr_el = Tr_ExpList(e.exp, NULL);
                tr_eld = tr_el;
            }
            else
            {
                tr_eld->tail = Tr_ExpList(e.exp, NULL);
                tr_eld = tr_eld->tail;
            }
            ret = e.ty;
        }
        return expTy(Tr_seqExp(tr_el), actual_ty(ret));
    }
    case A_assignExp:
    {
        expty var = transVar(level, venv, tenv, a->u.assign.var);
        expty exp = transExp(level, venv, tenv, a->u.assign.exp);
        if (!actual_eq(var.ty, exp.ty))
        {
            EM_error(a->pos, "assign expression: dismatch type between variable and expression");
            exit(1);
        }
        return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());
    }
    case A_ifExp:
    {
        // printf("A_ifExp 1\n") ;
        expty test = transExp(level, venv, tenv, a->u.iff.test);
        if (test.ty->kind != Ty_int)
        {
            EM_error(a->pos, "condition expression: test section must be integer");
            exit(1);
        }
        expty then = transExp(level, venv, tenv, a->u.iff.then);
        if (a->u.iff.elsee == NULL)
        {
            if (then.ty->kind != Ty_void)
            {
                EM_error(a->pos, "condition expression: then section must be void");
                exit(1);
            }
            return expTy(Tr_ifExp(test.exp, then.exp, NULL), Ty_Void());
        }
        else
        {
            expty elsee = transExp(level, venv, tenv, a->u.iff.elsee);
            if (!actual_eq(then.ty, elsee.ty))
            {
                EM_error(a->pos, "condition expression: then-else section must be the same type");
                exit(1);
            }

            Tr_exp tp = Tr_ifExp(test.exp, then.exp, elsee.exp);
            // pr_stm(stdout, Tr_unNx(tp), 0);
            return expTy(tp, then.ty);
        }
    }
    case A_whileExp:
    {
        expty test = transExp(level, venv, tenv, a->u.whilee.test);
        if (test.ty->kind != Ty_int)
        {
            EM_error(a->pos, "while loop: test section must produce integer");
            exit(1);
        }
        inside++;
        Tr_exp done = Tr_doneExp();
        brk[inside] = done;
        expty body = transExp(level, venv, tenv, a->u.whilee.body);
        inside--;
        if (body.ty->kind != Ty_void)
        {
            EM_error(a->pos, "while loop: body section must produce no value");
            exit(1);
        }
        Tr_exp tp = Tr_whileExp(test.exp, done, body.exp);
        return expTy(tp, Ty_Void());
    }
    case A_forExp:
    {
        // expty lo = transExp(level, venv, tenv, a->u.forr.lo);
        // expty hi = transExp(level, venv, tenv, a->u.forr.hi);
        // if (lo.ty->kind != Ty_int)
        // {
        //     EM_error(a->pos, "for loop: lower part must be integer");
        //     return expTy(NULL, Ty_Void());
        // }
        // if (hi.ty->kind != Ty_int)
        // {
        //     EM_error(a->pos, "for loop: higer part must be integer");
        //     return expTy(NULL, Ty_Void());
        // }
        // S_beginScope(venv);
        // Tr_access access = Tr_allocLocal(level, true);
        // S_enter(venv, a->u.forr.var, E_VarEntry(access, Ty_Int()));
        // inside++;
        // expty body = transExp(level, venv, tenv, a->u.forr.body);
        // inside--;
        // if (body.ty->kind != Ty_void)
        // {
        //     EM_error(a->pos, "for loop: body section must produce no value");
        //     return expTy(NULL, Ty_Void());
        // }
        // S_endScope(tenv);
        // return expTy(NULL, Ty_Void());
        /**
         * convert for to while
         *
         * for i := lo to hi do body
         *
         * let var i := lo
         *     var limit := hi
         * in
         *     while i <= limit do (body; i := i + 1)
         * end
         *
         * tip: 你可以使用前面的 prabsyn 将上述打印出来
         * note: 使用这样的转化可以省很多事 我之前直接使用for语句很麻烦
         */

        A_dec i = A_VarDec(a->pos, a->u.forr.var, NULL, a->u.forr.lo);
        A_dec limit = A_VarDec(a->pos, S_Symbol("limit"), NULL, a->u.forr.hi);
        A_decList let_declare = A_DecList(i, A_DecList(limit, NULL));

        A_exp increment_exp = A_AssignExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var), A_OpExp(a->pos, A_plusOp, A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var)), A_IntExp(a->pos, 1)));
        A_exp while_test = A_OpExp(a->pos, A_leOp, A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var)), A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("limit"))));
        A_exp while_body = A_SeqExp(a->pos, A_ExpList(a->u.forr.body, A_ExpList(increment_exp, NULL)));

        A_exp let_body = A_SeqExp(a->pos, A_ExpList(A_WhileExp(a->pos, while_test, while_body), NULL));
        A_exp let_exp = A_LetExp(a->pos, let_declare, let_body);
        return transExp(level, venv, tenv, let_exp);
    }
    case A_breakExp:
    {
        if (inside == 0)
        {
            EM_error(a->pos, "break expression: break expression outside loop");
            exit(1);
        }
        return expTy(Tr_breakExp(brk[inside]), Ty_Void());
    }
    case A_letExp:
    {
        S_beginScope(tenv);
        S_beginScope(venv);
        A_decList dl;
        Tr_expList head = NULL, last = NULL;
        for (dl = a->u.let.decs; dl; dl = dl->tail)
        {
            if (head == NULL)
            {
                head = Tr_ExpList(transDec(level, venv, tenv, dl->head), NULL);
                last = head;
            }
            else
            {
                last->tail = Tr_ExpList(transDec(level, venv, tenv, dl->head), NULL);
                last = last->tail;
            }
        }
        expty exp = transExp(level, venv, tenv, a->u.let.body);
        last->tail = Tr_ExpList(exp.exp, NULL);
        S_endScope(tenv);
        S_endScope(venv);
        return expTy(Tr_letExp(head), exp.ty);
    }
    case A_arrayExp:
    {
        Ty_ty ty = S_look(tenv, a->u.array.typ);
        if (ty == NULL)
        {
            EM_error(a->pos, "array expression: no such array type");
            exit(1);
        }
        Ty_ty actual = actual_ty(ty);
        if (actual->kind != Ty_array)
        {
            EM_error(a->pos, "array expression: array type required but given another %s", S_name(a->u.array.typ));
            exit(1);
        }

        expty size = transExp(level, venv, tenv, a->u.array.size);
        if (size.ty->kind != Ty_int)
        {
            EM_error(a->pos, "array expression: integer required with array size");
            exit(1);
        }
        expty init = transExp(level, venv, tenv, a->u.array.init);
        if (actual_eq(actual, init.ty))
        {
            EM_error(a->pos, "array expression: initialize type does not match with given type");
            exit(1);
        }
        return expTy(Tr_arrayExp(size.exp, init.exp), actual);
    }
    }

    assert(0);
}
Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, A_dec d)
{
    switch (d->kind)
    {
    case A_typeDec:
    {
        A_nametyList nl;
        bool cycle_decl = true;
        int index = 0;
        void *typenames[10];
        for (nl = d->u.type; nl; nl = nl->tail)
        {
            S_enter(tenv, nl->head->name, Ty_Name(nl->head->name, NULL));
            for (int i = 0; i < index; i++)
            {
                if (typenames[i] == (void *)nl->head->name)
                {
                    EM_error(d->pos, "type declare: redeclaration type <%s>, there are two types with the same name in the same (consecutive) batch of mutually recursive types.", S_name(nl->head->name));
                    exit(1);
                }
            }
            typenames[index++] = (void *)nl->head->name;
        }
        for (nl = d->u.type; nl; nl = nl->tail)
        {
            Ty_ty ty = transTy(tenv, nl->head->ty);
            Ty_ty namety = S_look(tenv, nl->head->name);
            namety->u.name.ty = ty;

            if (ty->kind != Ty_name)
                cycle_decl = false;
        }

        if (cycle_decl)
        {
            EM_error(d->pos, "type declare: illegal cycle type declaration: must contain at least one built-in type");
            exit(1);
        }
        return Tr_noExp();
    }
    case A_varDec:
    {
        Ty_ty dec_ty = NULL;
        if (d->u.var.typ != NULL)
        {
            dec_ty = S_look(tenv, d->u.var.typ);
            if (!dec_ty)
            {
                EM_error(d->pos, "Variable declare: undifined type %s", S_name(d->u.var.typ));
                exit(1);
            }
        }
        expty init_exp = transExp(level, venv, tenv, d->u.var.init);
        if (dec_ty != NULL)
        {
            if (!actual_eq(dec_ty, init_exp.ty))
            {
                EM_error(d->pos, "Variable declare: dismatch type between declare and initialze");
                exit(1);
            }
        }
        else
        {
            if (init_exp.ty->kind == Ty_nil)
            {
                EM_error(d->pos, "Variable declare: illegal nil type: nil must be assign to a explictly record type");
                exit(1);
            }
        }
        Tr_access access = Tr_allocLocal(level, d->u.var.escape);
        // if (!strcmp(Tr_getLevelName(level), "tigerMain"))
        //     printf("%s\n", S_name(d->u.var.var));
        S_enter(venv, d->u.var.var, E_VarEntry(access, init_exp.ty));
        return Tr_assignExp(Tr_simpleVar(access, level), init_exp.exp);
    }
    case A_functionDec:
    {
        A_fundecList funl;
        int index = 0;
        void *typenames[10];
        for (funl = d->u.function; funl; funl = funl->tail)
        {
            Ty_ty r;
            if (funl->head->result != NULL)
            {
                r = S_look(tenv, funl->head->result);
                if (r == NULL)
                {
                    EM_error(d->pos, "Function declare: undifined result type %s", S_name(funl->head->result));
                    exit(1);
                }
            }
            else
            {
                r = Ty_Void();
            }
            // printf("function name:%s\n", S_name(funl->head->name));
            // if (funl->head->result != NULL)
            // {
            //     if (!strcmp(S_name(funl->head->name), "readint"))
            //     {
            //         printf("function return result:%s\n", S_name(funl->head->result));
            //         pr_exp(stdout, funl->head->body, 0);
            //     }
            // }
            A_fieldList fl;
            Ty_ty ty;
            U_boolList bl_head = NULL, bl_tail = NULL;
            Ty_tyList tl_head = NULL, tl_tail = NULL;
            for (fl = funl->head->params; fl; fl = fl->tail)
            {
                ty = S_look(tenv, fl->head->typ);
                if (ty == NULL)
                {
                    EM_error(d->pos, "Function declare: undifined parameter type %s", S_name(fl->head->name));
                    exit(1);
                }
                if (tl_head == NULL)
                {
                    tl_head = Ty_TyList(ty, NULL);
                    tl_tail = tl_head;

                    bl_head = U_BoolList(true, NULL);
                    bl_tail = bl_head;
                }
                else
                {
                    tl_tail->tail = Ty_TyList(ty, NULL);
                    tl_tail = tl_tail->tail;

                    bl_tail->tail = U_BoolList(true, NULL);
                    bl_tail = bl_tail->tail;
                }
            }
            Temp_label fname = Temp_namedlabel(S_name(funl->head->name));
            Tr_level l = Tr_newLevel(level, fname, bl_head);
            S_enter(venv, funl->head->name, E_FunEntry(l, fname, tl_head, r));
            for (int i = 0; i < index; i++)
            {
                if (typenames[i] == (void *)funl->head->name)
                {
                    EM_error(d->pos, "function declare: redeclaration type <%s>, there are two types with the same name in the same (consecutive) batch of mutually recursive types.", S_name(funl->head->name));
                    exit(1);
                }
            }
            typenames[index++] = (void *)funl->head->name;
        }
        for (funl = d->u.function; funl; funl = funl->tail)
        {
            // printf(S_name(funl->head->name));
            // printf("\n");
            E_enventry entry = S_look(venv, funl->head->name);
            S_beginScope(venv);

            A_fieldList fl;
            Ty_tyList tl = entry->u.fun.formals;
            Tr_accessList al = Tr_formals(entry->u.fun.level);
            Tr_accessList m_accessList = Tr_formals(entry->u.fun.level);
            for (fl = funl->head->params; fl; fl = fl->tail, tl = tl->tail, al = al->tail)
            {
                S_enter(venv, fl->head->name, E_VarEntry(al->head, tl->head));
            }
            expty exp = transExp(entry->u.fun.level, venv, tenv, funl->head->body);
            // if (!strcmp(S_name(funl->head->name), "readlist"))
            // {
            //     printf("%s \n", S_name(funl->head->name));
            //     //printf("%s \n", Temp_labelstring(entry->u.fun.label));
            //     // pr_exp(stdout, funl->head->body, 0);
            //     pr_stm(stdout, T_Exp(Tr_unEx(exp.exp)), 0);
            // }
            if (!actual_eq(entry->u.fun.result, exp.ty))
            {
                EM_error(d->pos, "function declare: body type and return type with <%s>", S_name(funl->head->name));
                S_endScope(venv);
                exit(1);
            }
            Tr_procEntryExit(entry->u.fun.level, exp.exp, m_accessList);
            S_endScope(venv);
        }
        return Tr_noExp();
    }
    }
    /*set error output*/
    assert(0);
}
Ty_ty transTy(S_table tenv, A_ty a)
{
    switch (a->kind)
    {
    case A_nameTy:
    {
        Ty_ty ty = S_look(tenv, a->u.name);
        if (!ty)
        {
            EM_error(a->pos, "translate name type: undifined type %s", S_name(a->u.name));
            return Ty_Void();
        }
        return ty;
    }
    case A_recordTy:
    {
        Ty_fieldList ty_fl_head = NULL, ty_fl_tail = NULL;
        for (A_fieldList fl = a->u.record; fl; fl = fl->tail)
        {
            Ty_ty ty = S_look(tenv, fl->head->typ);
            if (!ty)
            {
                EM_error(a->pos, "translate record type: undifined type %s", S_name(fl->head->typ));
                return Ty_Void();
            }
            Ty_field field = Ty_Field(fl->head->name, ty);
            if (ty_fl_head)
            {
                ty_fl_tail->tail = Ty_FieldList(field, NULL);
                ty_fl_tail = ty_fl_tail->tail;
            }
            else
            {
                ty_fl_head = Ty_FieldList(field, NULL);
                ty_fl_tail = ty_fl_head;
            }
        }
        return Ty_Record(ty_fl_head);
    }
    case A_arrayTy:
    {
        Ty_ty ty = S_look(tenv, a->u.array);
        if (!ty)
        {
            EM_error(a->pos, "translate array type: undifined type %s", S_name(a->u.array));
            return Ty_Void();
        }
        return Ty_Array(ty);
    }
    }
    assert(0);
}