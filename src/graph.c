#include "graph.h"
#include "util.h"
#include <stdio.h>
#include "table.h"
#include <assert.h>
#include "assem.h"

struct G_graph_
{
    int nodecount;
    G_nodeList mynodes, mylast;
};
struct G_node_
{
    G_graph mygraph;
    int mykey;
    G_nodeList succs;
    G_nodeList preds;
    void *info;
};

G_graph G_Graph(void)
{
    G_graph g = checked_malloc(sizeof(*g));
    g->nodecount = 0;
    g->mynodes = NULL;
    g->mylast = NULL;
    return g;
}
G_nodeList G_NodeList(G_node head, G_nodeList tail)
{
    G_nodeList nl = checked_malloc(sizeof(*nl));
    nl->head = head;
    nl->tail = tail;
    return nl;
}
G_node G_Node(G_graph g, void *info)
{
    G_node n = checked_malloc(sizeof(*n));
    n->mygraph = g;
    n->mykey = g->nodecount++;
    G_nodeList p = G_NodeList(n, NULL);
    if (g->mylast == NULL)
        g->mynodes = g->mylast = p;
    else
        g->mylast = g->mylast->tail = p;
    n->preds = NULL;
    n->succs = NULL;
    n->info = info;
    return n;
}
G_nodeList G_nodes(G_graph g)
{
    return g->mynodes;
}
bool G_inNodeList(G_node a, G_nodeList l)
{
    for (G_nodeList p = l; p; p = p->tail)
    {
        if (p->head == a)
        {
            return true;
        }
    }
    return false;
}
void G_addEdge(G_node from, G_node to)
{
    //printf("G_addEdge 1\n");
    if (G_goesTo(from, to))
    {
        ///printf("G_addEdge 2\n");
        return;
    }
//printf("G_addEdge 3\n");
    from->succs = G_NodeList(to, from->succs);
    //printf("G_addEdge 4\n");
    //if(to==NULL)printf("G_addEdge to is null\n");
    to->preds = G_NodeList(from, to->preds);
    //printf("G_addEdge 5\n");
}
static G_nodeList delete (G_nodeList l, G_node node)
{
    if (l->head == node)
        return l->tail;
    else
        return G_NodeList(l->head, delete (l->tail, node));
}
void G_rmEdge(G_node from, G_node to)
{
    from->succs = delete (from->succs, to);
    to->preds = delete (to->preds, from);
}
void G_show(FILE *out, G_nodeList p, void showInfo(void *))
{
    int count =0;
    for (; p != NULL; p = p->tail)
    {
        G_node n = p->head;
        G_nodeList q;
        assert(n);
        if (showInfo)
            showInfo(n->info);
        AS_printinstrlist(out,AS_InstrList(((AS_instr)n->info),NULL));
        fprintf(out, " (%d): ", n->mykey);
        for (q = G_succ(n); q != NULL; q = q->tail)
            fprintf(out, "%d ", q->head->mykey);
        fprintf(out, "\n");

        count++;
    }

    fprintf(out, "node count: %d\n", count);
}
G_nodeList G_succ(G_node n)
{
    return n->succs;
}
G_nodeList G_pred(G_node n)
{
    return n->preds;
}
static G_nodeList cat(G_nodeList a, G_nodeList b)
{
    if (a == NULL)
        return b;
    else
        return G_NodeList(a->head, cat(a->tail, b));
}
G_nodeList G_adj(G_node n)
{
    return cat(n->preds, n->succs);
}
bool G_goesTo(G_node a, G_node b)
{
    return G_inNodeList(b, G_succ(a));
}
static int inDegree(G_node node)
{
    int deg = 0;
    for (G_nodeList p = G_pred(node); p; p = p->tail)
        deg++;

    return deg;
}
static int outDegree(G_node node)
{
    int deg = 0;
    for (G_nodeList p = G_succ(node); p; p = p->tail)
        deg++;

    return deg;
}
int G_degree(G_node n)
{
    return inDegree(n) + outDegree(n);
}
void *G_nodeInfo(G_node n)
{
    return n->info;
}

typedef struct TAB_table_ *G_table;
G_table G_empty(void)
{
    return TAB_empty();
}
void G_enter(G_table t, G_node n, void *v)
{
    return TAB_enter(t, n, v);
}
void *G_look(G_table t, G_node n)
{
    return TAB_look(t, n);
}

G_nodeList nodeListUnion(G_nodeList lhs, G_nodeList rhs)
{
    G_nodeList ret = rhs;
    for (G_nodeList i = lhs; i; i = i->tail)
    {
        if (i->head == NULL)
            continue;
        G_nodeList j;
        for (j = rhs; j; j = j->tail)
        {
            if (i->head == j->head)
                break;
        }
        if (j == NULL)
            ret = G_NodeList(i->head, ret);
    }
    return ret;
}
G_nodeList nodeListJoin(G_nodeList lhs, G_nodeList rhs)
{
    G_nodeList ret = NULL;
    for (G_nodeList i = lhs; i; i = i->tail)
    {
        for (G_nodeList j = rhs; j; j = j->tail)
        {
            if (i->head == j->head)
                ret = G_NodeList(i->head, ret);
        }
    }
    return ret;
}
G_nodeList nodeListDiff(G_nodeList lhs, G_nodeList rhs)
{
    G_nodeList ret = NULL;
    for (G_nodeList i = lhs; i; i = i->tail)
    {
        G_nodeList j;
        for (j = rhs; j; j = j->tail)
        {
            if (i->head == j->head)
                break;
        }
        if (j == NULL)
            ret = G_NodeList(i->head, ret);
    }
    return ret;
}
bool nodeListIn(G_nodeList list, G_node node)
{
    for (G_nodeList p = list; p; p = p->tail)
    {
        if (p->head == node)
            return true;
    }

    return false;
}
G_nodeList push(G_nodeList list, G_node node)
{
    if (list == NULL)
        return G_NodeList(node, NULL);

    G_nodeList p;
    for (p = list; p->tail; p = p->tail)
        ;
    p->tail = G_NodeList(node, NULL);
    return p;
}
G_node nodePop(G_nodeList list)
{
    G_node n = NULL;
    if (list == NULL)
        return n;

    if (list->tail == NULL)
    {
        n = list->head;
        list->head = NULL;
        return n;
    }

    G_nodeList prev;
    for (G_nodeList it = list; list->tail; list = list->tail)
        prev = it;
    n = prev->tail->head;
    prev->tail = NULL;
    return n;
}