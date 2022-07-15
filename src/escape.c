#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "escape.h"
#include "absyn.h"
#include "table.h"

typedef struct EscapeEntry_
{
    int d;
    bool *escape;
} * escape_entry;

escape_entry Escape_Entry(int d, bool *escape)
{
    escape_entry p = checked_malloc(sizeof(*p));
    p->d = d;
    p->escape = escape;
    return p;
}

static void traverseExp(S_table vevn, int depth, A_exp exp);
static void traverseDec(S_table vevn, int depth, A_dec exp);
static void traverseVar(S_table vevn, int depth, A_var exp);

void Esc_findEscape(A_exp exp)
{
    S_table escape_table = S_empty();
    S_beginScope(escape_table);
    traverseExp(escape_table, 0, exp);
}
static void traverseExp(S_table vevn, int depth, A_exp exp)
{
    switch (exp->kind)
    {
    case A_varExp:
    {
        traverseVar(vevn, depth, exp->u.var);
        return;
    }
    case A_nilExp:
    case A_intExp:
    case A_stringExp:
        return;
    case A_callExp:
    {
        A_expList args = exp->u.call.args;
        for (; args; args = args->tail)
        {
            traverseExp(vevn, depth, args->head);
        }
        return;
    }
    case A_opExp:
    {
        traverseExp(vevn, depth, exp->u.op.left);
        traverseExp(vevn, depth, exp->u.op.right);
        return;
    }
    case A_recordExp:
    {
        A_efieldList fields = exp->u.record.fields;
        for (; fields; fields = fields->tail)
        {
            traverseExp(vevn, depth, fields->head->exp);
        }
        return;
    }
    case A_seqExp:
    {
        A_expList seqs = exp->u.seq;
        for (; seqs; seqs = seqs->tail)
        {
            traverseExp(vevn, depth, seqs->head);
        }
        return;
    }
    case A_assignExp:
    {
        traverseExp(vevn, depth, exp->u.assign.exp);
        traverseVar(vevn, depth, exp->u.assign.var);
        return;
    }
    case A_ifExp:
    {
        traverseExp(vevn, depth, exp->u.iff.test);
        traverseExp(vevn, depth, exp->u.iff.then);
        if (exp->u.iff.elsee)
        {
            traverseExp(vevn, depth, exp->u.iff.elsee);
        }
        return;
    }
    case A_whileExp:
    {
        traverseExp(vevn, depth, exp->u.whilee.test);
        traverseExp(vevn, depth, exp->u.whilee.body);
        return;
    }
    case A_forExp:
    {
        S_beginScope(vevn);
        exp->u.forr.escape = false;
        S_enter(vevn, exp->u.forr.var, Escape_Entry(depth, &(exp->u.forr.escape)));
        traverseExp(vevn, depth, exp->u.forr.lo);
        traverseExp(vevn, depth, exp->u.forr.hi);
        traverseExp(vevn, depth, exp->u.forr.body);
        S_endScope(vevn);
        return;
    }
    case A_breakExp:
        return;
    case A_letExp:
    {
        S_beginScope(vevn);
        A_decList d = exp->u.let.decs;
        for (; d; d = d->tail)
        {
            traverseDec(vevn, depth, d->head);
        }

        traverseExp(vevn, depth, exp->u.let.body);
        S_endScope(vevn);
        return;
    }
    case A_arrayExp:
    {
        traverseExp(vevn, depth, exp->u.array.init);
        traverseExp(vevn, depth, exp->u.array.size);
        return;
    }
    }
    assert(0);
}
static void traverseDec(S_table vevn, int depth, A_dec exp)
{
    switch (exp->kind)
    {
    case A_typeDec:
        return;
    case A_varDec:
    {
        traverseExp(vevn, depth, exp->u.var.init);
        exp->u.var.escape = false;
        S_enter(vevn, exp->u.var.var, Escape_Entry(depth, &(exp->u.var.escape)));
        return;
    }
    case A_functionDec:
    {
        A_fundecList fl = exp->u.function;
        for (; fl; fl = fl->tail)
        {
            S_beginScope(vevn);
            A_fieldList l = fl->head->params;
            for (; l; l = l->tail)
            {
                l->head->escape = false;
                S_enter(vevn, l->head->name, Escape_Entry(depth + 1, &(l->head->escape)));
            }
            traverseExp(vevn, depth + 1, fl->head->body);
            S_endScope(vevn);
        }
        return;
    }
    }
    /*set error output*/
    assert(0);
}
static void traverseVar(S_table vevn, int depth, A_var exp)
{
    switch (exp->kind)
    {
    case A_simpleVar:
    {
        escape_entry x = S_look(vevn, exp->u.simple);
        if (x)
        {
            if (depth > x->d)
                *(x->escape) = true;
        }
        return;
    }
    case A_fieldVar:
    {
        traverseVar(vevn, depth, exp->u.field.var);
        return;
    }
    case A_subscriptVar:
    {
        traverseVar(vevn, depth, exp->u.subscript.var);
        traverseExp(vevn, depth, exp->u.subscript.exp);
        return;
    }
    }
    assert(0);
}
