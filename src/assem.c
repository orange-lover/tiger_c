#include "assem.h"
#include "util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h> /* for atoi */
#include <stdio.h>

AS_targets AS_Targets(Temp_labelList labels)
{
    AS_targets p = checked_malloc(sizeof(*p));
    p->labels = labels;
    return p;
}
AS_instr AS_Oper(string assem, Temp_tempList dst, Temp_tempList src, AS_targets jumps)
{
    AS_instr p = checked_malloc(sizeof(*p));
    p->kind = I_OPER;
    p->u.OPER.assem = assem;
    p->u.OPER.dst = dst;
    p->u.OPER.src = src;
    p->u.OPER.jumps = jumps;
    return p;
}
AS_instr AS_Label(string assem, Temp_label label)
{
    AS_instr p = checked_malloc(sizeof(*p));
    p->kind = I_LABEL;
    p->u.LABEL.assem = assem;
    p->u.LABEL.label = label;
    return p;
}
AS_instr AS_Move(string assem, Temp_tempList dst, Temp_tempList src)
{
    AS_instr p = checked_malloc(sizeof(*p));
    p->kind = I_MOVE;
    p->u.MOVE.assem = assem;
    p->u.MOVE.dst = dst;
    p->u.MOVE.src = src;
    return p;
}

static Temp_temp nthTemp(Temp_tempList list, int i)
{
    assert(list);
    if (i == 0)
        return list->head;
    else
        return nthTemp(list->tail, i - 1);
}
static Temp_label nthLabel(Temp_labelList list, int i)
{
    assert(list);
    if (i == 0)
        return list->head;
    else
        return nthLabel(list->tail, i - 1);
}

static void format(char *result, string assem, Temp_tempList dst, Temp_tempList src, AS_targets jumps, Temp_map m)
{
    char *p;
    int i = 0;
    for (p = assem; p && *p != '\0'; p++)
    {
        if (*p == '`')
        {
            switch (*(++p))
            {
            case 's':
            {
                int n = atoi(++p);
                string s = Temp_look(m, nthTemp(src, n));
                // Temp_temp t = nthTemp(src, n);
                // char buff[100];
                // sprintf(buff, "T%d", Temp_getnum(t));
                // string s = String(buff);
                strcpy(result + i, s);
                i += strlen(s);
                break;
            }
            case 'd':
            {
                int n = atoi(++p);
                string s = Temp_look(m, nthTemp(dst, n));
                // Temp_temp t = nthTemp(dst, n);
                // char buff[100];
                // sprintf(buff, "T%d", Temp_getnum(t));
                // string s = String(buff);
                strcpy(result + i, s);
                i += strlen(s);
                break;
            }
            case 'j':
            {
                assert(jumps);
                int n = atoi(++p);
                string s = Temp_labelstring(nthLabel(jumps->labels, n));
                strcpy(result + i, s);
                i += strlen(s);
                break;
            }
            case '`':
            {
                result[i] = '`';
                i++;
                break;
            }
            default:
                assert(0);
            }
        }
        else
        {
            result[i] = *p;
            i++;
        }
    }
    result[i] = '\0';
}

void AS_print(FILE *out, AS_instr i, Temp_map m)
{
    char r[200];
    switch (i->kind)
    {
    case I_OPER:
    {
        format(r, i->u.OPER.assem, i->u.OPER.dst, i->u.OPER.src, i->u.OPER.jumps, m);
        fprintf(out, "%s", r);
        break;
    }

    case I_LABEL:
    {
        format(r, i->u.LABEL.assem, NULL, NULL, NULL, m);
        fprintf(out, "%s", r);
        break;
    }
    case I_MOVE:
    {
        format(r, i->u.MOVE.assem, i->u.MOVE.dst, i->u.MOVE.src, NULL, m);
        fprintf(out, "%s", r);
        break;
    }
    }
}

AS_instrList AS_InstrList(AS_instr head, AS_instrList tail)
{
    AS_instrList p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    return p;
}

AS_instrList AS_splice(AS_instrList a, AS_instrList b)
{
    if (a == NULL)
        return b;

    AS_instrList p = a;
    while (p->tail)
        p = p->tail;

    p->tail = b;
    return a;
}
void AS_printInstrList(FILE *out, AS_instrList iList, Temp_map m)
{
    for (; iList; iList = iList->tail)
    {
        AS_print(out, iList->head, m);
    }

    fprintf(out, "\n");
}

AS_proc AS_Proc(string p, AS_instrList b, string e)
{
    AS_proc proc = checked_malloc(sizeof(*proc));
    proc->prolog = p;
    proc->body = b;
    proc->epilog = e;
}

// void printCfgInfo(void *p)
// {
//     AS_instr ins = (AS_instr)p;
//     AS_print(stdout, ins, Temp_layerMap(F_tempMap(), Temp_name()));
// }

void AS_printinstrlist(FILE *out, AS_instrList instr_l)
{
    AS_instrList il = instr_l;
    int count = 0;
    for (; il; il = il->tail)
    {
        switch (il->head->kind)
        {
        case I_OPER:
        {
            fprintf(out, "%s", il->head->u.OPER.assem);
            break;
        }
        case I_LABEL:
        {
            fprintf(out, "%s", il->head->u.LABEL.assem);
            break;
        }
        case I_MOVE:
        {
            fprintf(out, "%s", il->head->u.MOVE.assem);
            break;
        }
        }

        count++;
    }
    if (count > 1)
        fprintf(out, "instruction count: %d\n", count);
}