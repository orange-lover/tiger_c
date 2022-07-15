#ifndef CANON_H
#define CANON_H

#include "tree.h"

typedef struct C_stmListList_ *C_stmListList;
struct C_stmListList_
{
    T_stmList head;
    C_stmListList tail;
};
struct C_block
{
    C_stmListList stmLists;
    Temp_label label;
};

T_stmList C_linearize(T_stm stm);
struct C_block C_basicBlocks(T_stmList stmList);
T_stmList C_traceSchedule(struct C_block b);

#endif