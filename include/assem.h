#ifndef ASSEM_H
#define ASSEM_H

#include "temp.h"
#include "util.h"


typedef struct
{
    Temp_labelList labels;
} * AS_targets;
AS_targets AS_Targets(Temp_labelList labels);

typedef struct
{
    enum
    {
        I_OPER,
        I_LABEL,
        I_MOVE
    } kind;
    union
    {
        struct
        {
            string assem;
            Temp_tempList dst, src;
            AS_targets jumps;
        } OPER;
        struct
        {
            string assem;
            Temp_label label;
        } LABEL;
        struct
        {
            string assem;
            Temp_tempList dst, src;
        } MOVE;
    } u;

} * AS_instr;

AS_instr AS_Oper(string assem, Temp_tempList dst, Temp_tempList src, AS_targets jumps);
AS_instr AS_Label(string assem, Temp_label label);
AS_instr AS_Move(string assem, Temp_tempList dst, Temp_tempList src);

void AS_print(FILE *out, AS_instr i, Temp_map m);

typedef struct AS_instrList_ *AS_instrList;
struct AS_instrList_
{
    AS_instr head;
    AS_instrList tail;
};

AS_instrList AS_InstrList(AS_instr head, AS_instrList tail);

AS_instrList AS_splice(AS_instrList a, AS_instrList b);
void AS_printInstrList(FILE *out, AS_instrList iList, Temp_map m);

typedef struct AS_proc_ *AS_proc;
struct AS_proc_
{
    string prolog;
    AS_instrList body;
    string epilog;
};
AS_proc AS_Proc(string p, AS_instrList b, string e);

// void printCfgInfo(void *p);

void AS_printinstrlist(FILE *out, AS_instrList instr_l);


#endif