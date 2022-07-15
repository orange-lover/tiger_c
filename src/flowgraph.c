#include <stdio.h>
#include "flowgraph.h"
#include "table.h"
#include "assem.h"

Temp_tempList FG_def(G_node n)
{
    AS_instr instr = (AS_instr)G_nodeInfo(n);
    switch (instr->kind)
    {
    case I_MOVE:
        return instr->u.MOVE.dst;
    case I_OPER:
        return instr->u.OPER.dst;
    }
    return NULL;
}
Temp_tempList FG_use(G_node n)
{
    AS_instr instr = (AS_instr)G_nodeInfo(n);
    switch (instr->kind)
    {
    case I_MOVE:
        return instr->u.MOVE.src;
    case I_OPER:
        return instr->u.OPER.src;
    }
    return NULL;
}
bool FG_isMove(G_node n)
{
    AS_instr instr = (AS_instr)G_nodeInfo(n);
    return instr->kind == I_MOVE;
}
G_graph FG_AssemFlowGraph(AS_instrList il)
{
    //printf("FG_AssemFlowGraph 1 \n");
    G_graph g = G_Graph();
    TAB_table lt = TAB_empty();
    G_node last = NULL;
    for (AS_instrList iter = il; iter; iter = iter->tail)
    {
        G_node n = G_Node(g, iter->head);
        if (iter->head->kind == I_OPER && iter->head->u.OPER.jumps != NULL)
        {
            if (last != NULL)
                G_addEdge(last, n);
            last = NULL;
        }
        else
        {
            if (last != NULL)
                G_addEdge(last, n);
            last = n;
        }
        if (iter->head->kind == I_LABEL)
        {
            TAB_enter(lt, iter->head->u.LABEL.label, n);
        }
    }
    //printf("FG_AssemFlowGraph 2 \n");
    for (G_nodeList nl = G_nodes(g); nl; nl = nl->tail)
    {
        //printf("FG_AssemFlowGraph 2.1 \n");
        G_node n = nl->head;
        AS_instr instr = (AS_instr)G_nodeInfo(n);
        //printf("FG_AssemFlowGraph 2.2 \n");
        if (instr->kind == I_OPER)
        {
            AS_targets jumps = instr->u.OPER.jumps;
            if (jumps == NULL)
                continue;

            //printf("FG_AssemFlowGraph 2.21 \n");
            for (Temp_labelList l = jumps->labels; l; l = l->tail)
            {
                //printf("FG_AssemFlowGraph 2.22 \n");

                G_node label_node = TAB_look(lt, l->head);
                if (label_node)
                    G_addEdge(n, label_node);
                //printf("FG_AssemFlowGraph 2.23 \n");
            }
        }
       // printf("FG_AssemFlowGraph 2.3 \n");
    }
   // printf("FG_AssemFlowGraph 3 \n");
    return g;
}