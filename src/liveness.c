#include "liveness.h"
#include "util.h"
#include "temp.h"
#include "flowgraph.h"
#include "frame.h"
#include "table.h"

Live_moveList Live_MoveList(G_node src, G_node dst, Live_moveList tail)
{
    Live_moveList move = checked_malloc(sizeof(*move));
    move->src = src;
    move->dst = dst;
    move->tail = tail;
    return move;
}
Temp_temp Live_gtemp(G_node n)
{
    return G_nodeInfo(n);
}

double getCost(G_table t, G_node n)
{
    double *p = G_look(t, n);
    if (p)
    {
        return *p;
    }
    return 0.0;
}
static void setCost(G_table t, G_node n, double d)
{
    double *p = G_look(t, n);
    if (p == NULL)
    {
        p = checked_malloc(sizeof(double));
        G_enter(t, n, p);
    }
    *p = d;
}

struct Live_graph Live_liveness(G_graph flow)
{
    struct Live_graph lg;
    // 1. liveIn liveOut
    G_table liveIn = G_empty();
    G_table liveOut = G_empty();
    G_nodeList nodeList = G_nodes(flow);
    bool noChange = false;
    while (!noChange)
    {
        noChange = true;
        for (G_nodeList nl = nodeList; nl; nl = nl->tail)
        {
            G_node node = nl->head;
            Temp_tempList nodeIn = G_look(liveIn, node);
            Temp_tempList nodeOut = G_look(liveOut, node);

            // out <- union of all successors' in
            for (G_nodeList oi = G_succ(node); oi; oi = oi->tail)
            {
                G_node s = oi->head;
                Temp_tempList succIn = G_look(liveIn, s);
                for (Temp_tempList iter = succIn; iter; iter = iter->tail)
                {
                    if (!Temp_tempListLook(nodeOut, iter->head))
                    {
                        nodeOut = Temp_TempList(iter->head, nodeOut);
                        noChange = false;
                    }
                }
            }

            // in <- use + (out - def)
            for (Temp_tempList tl = FG_use(node); tl; tl = tl->tail)
            {
                if (!Temp_tempListLook(nodeIn, tl->head))
                {
                    nodeIn = Temp_TempList(tl->head, nodeIn);
                    noChange = false;
                }
            }
            Temp_tempList def = FG_def(node);
            for (Temp_tempList ii = nodeOut; ii; ii = ii->tail)
            {
                if (!Temp_tempListLook(nodeIn, ii->head) && !Temp_tempListLook(def, ii->head))
                {
                    nodeIn = Temp_TempList(ii->head, nodeIn);
                    noChange = false;
                }
            }

            G_enter(liveIn, node, nodeIn);
            G_enter(liveOut, node, nodeOut);
        }
    }
    // 2. interference
    G_graph interference = G_Graph();
    TAB_table interferenceMap = TAB_empty();
    Temp_tempList merged = F_registers();
    for (G_nodeList nl = nodeList; nl; nl = nl->tail)
    {
        G_node node = nl->head;
        Temp_tempList use = FG_use(node);
        Temp_tempList def = FG_def(node);
        merged = Temp_tempListUnion(Temp_tempListUnion(use, def), merged);
    }
    for (Temp_tempList it = merged; it; it = it->tail)
    {
        Temp_temp t = it->head;
        G_node node = G_Node(interference, t);
        TAB_enter(interferenceMap, t, node);
    }
    for (G_nodeList it = nodeList; it; it = it->tail)
    {
        G_node node = it->head;
        Temp_tempList l = FG_def(node);
        if (l == NULL)
            continue;
        Temp_tempList r = G_look(liveOut, node);
        if (FG_isMove(node))
            r = Temp_tempListDiff(r, FG_use(node));

        for (Temp_tempList it1 = l; it1; it1 = it1->tail)
        {
            for (Temp_tempList it2 = r; it2; it2 = it2->tail)
            {
                G_node lNode = TAB_look(interferenceMap, it1->head);
                G_node rNode = TAB_look(interferenceMap, it2->head);
                if (lNode == rNode)
                    continue;

                G_addEdge(lNode, rNode);
                G_addEdge(rNode, lNode);
            }
        }
    }
    // 3. move list
    Live_moveList moveList = NULL;
    G_table nodeToMove = G_empty();
    for (G_nodeList it = nodeList; it; it = it->tail)
    {
        G_node node = it->head;
        if (FG_isMove(node))
        {
            AS_instr instr = G_nodeInfo(node);
            Temp_temp from = instr->u.MOVE.src->head;
            Temp_temp to = instr->u.MOVE.dst->head;
            if (!from)
                continue;

            G_node srcNode = TAB_look(interferenceMap, from);
            G_node dstNode = TAB_look(interferenceMap, to);
            moveList = Live_MoveList(srcNode, dstNode, moveList);
            G_enter(nodeToMove, srcNode, Live_MoveList(srcNode, dstNode, G_look(nodeToMove, srcNode)));
        }
    }
    // 4. cost
    G_table nodeCost = G_empty();
    for (G_nodeList it = nodeList; it; it = it->tail)
    {
        G_node node = it->head;
        Temp_tempList use = FG_use(node);
        Temp_tempList def = FG_def(node);
        for (Temp_tempList it1 = use; it1; it1 = it1->tail)
        {
            G_node tempNode = TAB_look(interferenceMap, it1->head);
            setCost(nodeCost, tempNode, getCost(nodeCost, tempNode) + 1.0);
        }
        for (Temp_tempList it2 = def; it2; it2 = it2->tail)
        {
            G_node tempNode = TAB_look(interferenceMap, it2->head);
            setCost(nodeCost, tempNode, getCost(nodeCost, tempNode) + 1.0);
        }
    }
    for (G_nodeList it = G_nodes(interference); it; it = it->tail)
    {
        G_node tempNode = it->head;
        setCost(nodeCost, tempNode, getCost(nodeCost, tempNode) / G_degree(tempNode) / 2);
    }
    lg.graph = interference;
    lg.moves = moveList;
    lg.nodeToMove = nodeToMove;
    lg.nodeCost = nodeCost;
    return lg;
}

static bool moveEqual(Live_moveList lhs, Live_moveList rhs)
{
    return lhs->src == rhs->src && lhs->dst == rhs->dst;
}
bool moveListIn(Live_moveList list, Live_moveList move)
{
    for (Live_moveList it = list; it; it = it->tail)
    {
        if (moveEqual(it, move))
            return true;
    }
    return false;
}
Live_moveList moveListUnion(Live_moveList lhs, Live_moveList rhs)
{
    Live_moveList ret = NULL;
    for (Live_moveList it = lhs; it; it = it->tail)
    {
        if (!moveListIn(ret, it))
            ret = Live_MoveList(it->src, it->dst, ret);
    }
    for (Live_moveList it = lhs; it; it = it->tail)
    {
        if (!moveListIn(ret, it))
            ret = Live_MoveList(it->src, it->dst, ret);
    }
    return ret;
}
Live_moveList moveListJoin(Live_moveList lhs, Live_moveList rhs)
{
    Live_moveList ret = NULL;
    for (Live_moveList it = lhs; it; it = it->tail)
    {
        for (Live_moveList iter = lhs; iter; iter = iter->tail)
        {
            if (moveEqual(it, iter))
                ret = Live_MoveList(it->src, it->dst, ret);
        }
    }
    return ret;
}
Live_moveList moveListDiff(Live_moveList lhs, Live_moveList rhs)
{
    Live_moveList ret = NULL;
    for (Live_moveList it = lhs; it; it = it->tail)
    {
        Live_moveList iter = NULL;
        for (iter = lhs; iter; iter = iter->tail)
            if (moveEqual(it, iter))
                break;
        if (iter == NULL)
            ret = Live_MoveList(it->src, it->dst, ret);
    }
    return ret;
}
void moveListDump(FILE *out, Live_moveList list)
{
    for (Live_moveList iter = list; iter; iter = iter->tail)
    {
        fprintf(out, "t%d<-t%d \n", Temp_getnum(G_nodeInfo(iter->dst)),
                Temp_getnum(G_nodeInfo(iter->src)));
    }
    fprintf(out, "\n");
}