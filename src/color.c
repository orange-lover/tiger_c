#include "temp.h"
#include "graph.h"
#include "liveness.h"
#include "color.h"

static Temp_tempList precolored;
static G_nodeList simplifyWorklist;
static G_nodeList freezeWorklist;
static G_nodeList spillWorklist;
static G_nodeList spilledNodes;
static G_nodeList coalescedNodes;
static G_nodeList coloredNodes;
static G_nodeList selectStack;

static Live_moveList coalescedMoves;
static Live_moveList constrainedMoves;
static Live_moveList frozenMoves;
static Live_moveList worklistMoves;
static Live_moveList activeMoves;

static G_table degree;
static G_table moveList;
static G_table alias;
static G_table color;

static G_table nodeCost;
static G_graph interferenceGraph;

static int K = 32;

static void build(Temp_tempList precolor, Live_moveList moves, G_graph ig, G_table nodeMove, G_table cost);
static void makeWorklist();
static void simplify();
static void coalesce();
static void freeze();
static void selectSpill();
static void assignColors();

static void addEdge(G_node u, G_node v);
static G_nodeList adjacent(G_node node);
static Live_moveList nodeMoves(G_node node);
static bool moveRelated(G_node node);
static void decrementDegree(G_node node);
static void enableMoves(G_nodeList nodes);
static void addWorkList(G_node node);
static bool OK(G_node l, G_node r);
static bool allAdjacentOK(G_node v, G_node u);
static bool conservative(G_nodeList nodes);
static G_node getAlias(G_node node);
static void combine(G_node u, G_node v);
static void freezeMoves(G_node node);

static int getDegree(G_node node);
static void setDegree(G_node node, int deg);
static G_nodeList set(G_node node);
static Live_moveList moveSet(G_node src, G_node dst);
static Temp_temp getColor(G_node node);
static void setColor(G_node node, Temp_temp color);
static bool inPrecolored(G_node node);

static void nodeListPush(G_node n);
static G_node nodeListPop();

struct COL_result COL_color(struct Live_graph liveGraph, Temp_map initial, Temp_tempList regs)
{
    struct COL_result ret;
    G_graph ig = liveGraph.graph;
    Live_moveList moves = liveGraph.moves;
    G_table nodeCost = liveGraph.nodeCost;
    G_table nodeMove = liveGraph.nodeToMove;
    build(regs, moves, ig, nodeMove, nodeCost);
    makeWorklist();
    //printf("COL_color 1\n");
    do
    {
        if (simplifyWorklist != NULL)
            simplify();
        if (worklistMoves != NULL)
            coalesce();
        if (freezeWorklist != NULL)
            freeze();
        if (spillWorklist != NULL)
            selectSpill();
    } while (simplifyWorklist != NULL || freezeWorklist != NULL || spillWorklist != NULL || worklistMoves != NULL);
    //printf("COL_color 2\n");
    assignColors();
    // printf("COL_color 3\n");
    Temp_map map = Temp_empty();
    for (G_nodeList nl = G_nodes(interferenceGraph); nl; nl = nl->tail)
    {
        G_node n = nl->head;
        Temp_temp reg = getColor(n);
        if (reg)
            Temp_enter(map, Live_gtemp(n), Temp_look(initial, reg));
    }
    ret.coloring = map;

    Temp_tempList spills = NULL;
    for (G_nodeList nl = spilledNodes; nl; nl = nl->tail)
        spills = Temp_TempList(Live_gtemp(nl->head), spills);
    ret.spills = spills;
    return ret;
}

static void build(Temp_tempList precolor, Live_moveList moves, G_graph ig, G_table nodeMove, G_table cost)
{
    precolored = precolor;

    simplifyWorklist = NULL;
    freezeWorklist = NULL;
    spillWorklist = NULL;
    spilledNodes = NULL;
    coalescedNodes = NULL;
    coloredNodes = NULL;
    selectStack = NULL;

    coalescedMoves = NULL;
    constrainedMoves = NULL;
    frozenMoves = NULL;
    worklistMoves = moves;
    activeMoves = NULL;

    degree = G_empty();
    moveList = nodeMove;
    alias = G_empty();
    color = G_empty();

    nodeCost = cost;
    interferenceGraph = ig;

    for (G_nodeList nl = G_nodes(interferenceGraph); nl; nl = nl->tail)
    {
        G_node n = nl->head;
        if (!inPrecolored(n))
            setDegree(n, G_degree(n) / 2);
        else
            setDegree(n, 233333);
    }
}
static void makeWorklist()
{
    for (G_nodeList nl = G_nodes(interferenceGraph); nl; nl = nl->tail)
    {
        G_node n = nl->head;
        if (inPrecolored(n))
            continue;

        if (getDegree(n) >= K)
            spillWorklist = nodeListUnion(spillWorklist, set(n));
        else if (moveRelated(n))
            freezeWorklist = nodeListUnion(freezeWorklist, set(n));
        else
            simplifyWorklist = nodeListUnion(simplifyWorklist, set(n));
    }
}
static void simplify()
{
    //printf("simplify 1\n");
    G_node node = simplifyWorklist->head;
    simplifyWorklist = simplifyWorklist->tail;
    nodeListPush(node);
    for (G_nodeList nl = adjacent(node); nl; nl = nl->tail)
        decrementDegree(nl->head);
    //printf("simplify 2\n");
}
static void coalesce()
{
    //printf("coalesce 0\n");
    Live_moveList m = worklistMoves;
    G_node x = getAlias(m->src);
    G_node y = getAlias(m->dst);
    G_node u = x, v = y;
    if (inPrecolored(y))
    {
        u = y;
        v = x;
    }
    worklistMoves = worklistMoves->tail;
    //printf("coalesce 1\n");
    if (u == v)
    {
        coalescedMoves = moveListUnion(coalescedMoves, m);
        addWorkList(u);
        //printf("coalesce 2\n");
    }
    else if (inPrecolored(v) || G_goesTo(u, v))
    {
        constrainedMoves = moveListUnion(constrainedMoves, m);
        addWorkList(u);
        addWorkList(v);
        //printf("coalesce 3\n");
    }
    else if ((inPrecolored(u) && allAdjacentOK(v, u)) || (!inPrecolored(u) && conservative(nodeListUnion(adjacent(u), adjacent(v)))))
    {
        coalescedMoves = moveListUnion(coalescedMoves, m);
        combine(u, v);
        addWorkList(u);
        //printf("coalesce 4\n");
    }
    else
    {
        activeMoves = moveListUnion(activeMoves, m);
    }
}
static void freeze()
{
    //printf("freeze 1\n");
    G_node n = freezeWorklist->head;
    freezeWorklist = freezeWorklist->tail;
    simplifyWorklist = nodeListUnion(simplifyWorklist, set(n));
    freezeMoves(n);
    //printf("freeze 2\n");
}
static void selectSpill()
{
    //printf("selectSpill 1\n");
    G_node minCost = NULL;
    for (G_nodeList nl = spillWorklist; nl; nl = nl->tail)
    {
        G_node n = nl->head;
        if (!minCost)
            minCost = nl->head;
        if (getCost(nodeCost, n) < getCost(nodeCost, minCost))
            minCost = n;
    }
    G_nodeList u = set(minCost);
    spillWorklist = nodeListDiff(spillWorklist, u);
    simplifyWorklist = nodeListUnion(simplifyWorklist, u);
    //printf("selectSpill 2\n");
    freezeMoves(minCost);
    //printf("selectSpill 3\n");
}
static void assignColors()
{
    //printf("assignColors 1\n");
    for (G_nodeList nl = G_nodes(interferenceGraph); nl; nl = nl->tail)
    {
        G_node n = nl->head;
        if (inPrecolored(n))
        {
            setColor(n, Live_gtemp(n));
            coloredNodes = nodeListUnion(coloredNodes, set(n));
        }
    }

    while (selectStack)
    {
        G_node n = nodeListPop();
        if (G_inNodeList(n, coloredNodes))
            continue;

        Temp_tempList okColors = precolored;
        G_nodeList adj = G_adj(n);
        for (G_nodeList nl = adj; nl; nl = nl->tail)
        {
            G_node w = nl->head;
            if (inPrecolored(getAlias(w)) || G_inNodeList(getAlias(w), coloredNodes))
                okColors = Temp_tempListDiff(okColors, Temp_TempList(getColor(getAlias(w)), NULL));
        }
        if (okColors)
        {
            coloredNodes = nodeListUnion(coloredNodes, set(n));
            setColor(n, okColors->head);
        }
        else
        {
            spilledNodes = nodeListUnion(spilledNodes, set(n));
        }
    }

    for (G_nodeList nl = coalescedNodes; nl; nl = nl->tail)
        setColor(nl->head, getColor(getAlias(nl->head)));

    //printf("assignColors 2\n");
}

static void addEdge(G_node u, G_node v)
{
    if (G_goesTo(u, v))
        return;

    G_addEdge(u, v);
    G_addEdge(v, u);
    setDegree(u, getDegree(u) + 1);
    setDegree(v, getDegree(v) + 1);
}
static G_nodeList adjacent(G_node node)
{
    return nodeListDiff(G_adj(node), nodeListUnion(selectStack, coalescedNodes));
}
static Live_moveList nodeMoves(G_node node)
{
    //printf("nodeMoves 1\n");
    Live_moveList mov = G_look(moveList, node);
    //printf("nodeMoves 1.1\n");
    return moveListJoin(mov, moveListUnion(worklistMoves, activeMoves));
    //printf("nodeMoves 2\n");
}
static bool moveRelated(G_node node)
{
    return nodeMoves(node) != NULL;
}
static void decrementDegree(G_node node)
{
    int degree = getDegree(node);
    setDegree(node, degree - 1);
    if (degree == K)
    {
        enableMoves(nodeListUnion(adjacent(node), set(node)));
        spillWorklist = nodeListDiff(spillWorklist, set(node));
        if (moveRelated(node))
            freezeWorklist = nodeListUnion(freezeWorklist, set(node));
        else
            simplifyWorklist = nodeListUnion(simplifyWorklist, set(node));
    }
}
static void enableMoves(G_nodeList nodes)
{
    for (G_nodeList nl = nodes; nl; nl = nl->tail)
    {
        for (Live_moveList ml = nodeMoves(nl->head); ml; ml = ml->tail)
        {
            if (moveListIn(activeMoves, ml))
            {
                activeMoves = moveListDiff(activeMoves, moveSet(ml->src, ml->dst));
                worklistMoves = moveListUnion(worklistMoves, moveSet(ml->src, ml->dst));
            }
        }
    }
}
static void addWorkList(G_node node)
{
    if (!inPrecolored(node) && !moveRelated(node) && getDegree(node) < K)
    {
        freezeWorklist = nodeListDiff(freezeWorklist, set(node));
        simplifyWorklist = nodeListUnion(simplifyWorklist, set(node));
    }
}
static bool OK(G_node l, G_node r)
{
    return getDegree(l) < K || inPrecolored(l) || G_goesTo(l, r);
}
static bool allAdjacentOK(G_node v, G_node u)
{
    for (G_nodeList nl = adjacent(v); nl; nl = nl->tail)
    {
        if (!OK(nl->head, u))
            return false;
    }
    return true;
}
static bool conservative(G_nodeList nodes)
{
    int k = 0;
    for (G_nodeList nl = nodes; nl; nl = nl->tail)
    {
        if (getDegree(nl->head) >= K)
            k++;
    }
    return k < K;
}
static G_node getAlias(G_node node)
{
    return nodeListIn(coalescedNodes, node) ? getAlias(G_look(alias, node)) : node;
}
static void combine(G_node u, G_node v)
{
    if (nodeListIn(freezeWorklist, v))
        freezeWorklist = nodeListDiff(freezeWorklist, set(v));
    else
        spillWorklist = nodeListDiff(spillWorklist, set(v));
    coalescedNodes = nodeListUnion(coalescedNodes, set(v));
    G_enter(alias, v, u);
    G_enter(moveList, u, moveListUnion(G_look(moveList, u), G_look(moveList, v)));
    enableMoves(set(v));
    for (G_nodeList nl = adjacent(v); nl; nl = nl->tail)
    {
        addEdge(u, nl->head);
        decrementDegree(nl->head);
    }
    if (getDegree(u) >= K && nodeListIn(freezeWorklist, u))
    {
        freezeWorklist = nodeListDiff(freezeWorklist, set(u));
        spillWorklist = nodeListUnion(spillWorklist, set(u));
    }
}
static void freezeMoves(G_node node)
{
    //printf("freezeMoves 1\n");
    //if(node==NULL)       printf("node is null\n");
    for (Live_moveList ml = nodeMoves(node); ml; ml = ml->tail)
    {
        // printf("freezeMoves 1.1\n");
        G_node v = getAlias(ml->src) == getAlias(node) ? getAlias(ml->dst) : getAlias(ml->src);
         //printf("freezeMoves 1.2\n");
        activeMoves = moveListDiff(activeMoves, ml);
         //printf("freezeMoves 1.3\n");
        frozenMoves = moveListUnion(frozenMoves, ml);
         //printf("freezeMoves 1.4\n");
        if (nodeMoves(v) == NULL && getDegree(v) < K)
        {
            freezeWorklist = nodeListDiff(freezeWorklist, set(v));
            simplifyWorklist = nodeListUnion(simplifyWorklist, set(v));
        }
        // printf("freezeMoves 1.5\n");
    }
   // printf("freezeMoves 2\n");
}

static int getDegree(G_node node)
{
    return *(int *)G_look(degree, node);
}
static void setDegree(G_node node, int deg)
{
    int *i = checked_malloc(sizeof(int));
    *i = deg;
    G_enter(degree, node, i);
}
static G_nodeList set(G_node node)
{
    return G_NodeList(node, NULL);
}
static Live_moveList moveSet(G_node src, G_node dst)
{
    return Live_MoveList(src, dst, NULL);
}
static Temp_temp getColor(G_node node)
{
    return G_look(color, node);
}
static void setColor(G_node node, Temp_temp setcolor)
{
    G_enter(color, node, setcolor);
}
static bool inPrecolored(G_node node)
{
    return Temp_tempListLook(precolored, Live_gtemp(node));
}

static void nodeListPush(G_node n)
{
    selectStack = nodeListUnion(selectStack, set(n));
}
static G_node nodeListPop()
{
    G_node node = selectStack->head;
    selectStack = selectStack->tail;
    return node;
}