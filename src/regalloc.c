#include <stdio.h>
#include <stdlib.h>
#include "regalloc.h"
#include "graph.h"
#include "flowgraph.h"
#include "liveness.h"
#include "color.h"
#include "codegen.h"
#include "frame.h"
#include "table.h"

void show(void *key, void *value)
{
    G_node n = (G_node)key;
    Temp_temp t = (Temp_temp)G_nodeInfo(n);
    printf("var(t%d):%f\n", Temp_getnum(t), *((double *)value));
}

struct RA_result RA_regAlloc(F_frame f, AS_instrList il, int cnt)
{
    if (cnt > 10)
        abort();

    // printf("RA_regAlloc 1\n");
    struct RA_result ret;

    G_graph flow = FG_AssemFlowGraph(il);
    // G_show(stdout, G_nodes(flow), NULL);
    // printf("RA_regAlloc 2\n");

    struct Live_graph live_graph = Live_liveness(flow);
    // printf("RA_regAlloc 3\n");
    //  G_show(stdout, G_nodes(live_graph.graph), NULL);
    //  moveListDump(stdout, live_graph.moves);
    //  TAB_dump(live_graph.nodeCost, show);
    //Temp_tempList registers = Temp_tempListUnion(F_CalleeSaves(), F_CallerSaves());
    struct COL_result col = COL_color(live_graph, F_tempMap(), F_registers());
    // printf("RA_regAlloc 4\n");
    if (col.spills)
    {
        il = As_rewriteSpill(f, il, col.spills);
        // printf("RA_regAlloc 5\n");
        return RA_regAlloc(f, il, cnt + 1);
    }

    //ret.il = AS_rewrite(il, col.coloring);
    ret.il = il;
    ret.coloring = col.coloring;
    // printf("RA_regAlloc 6\n");
    return ret;
}
