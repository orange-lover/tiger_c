/*
 * color.h - Data structures and function prototypes for coloring algorithm
 *             to determine register allocation.
 */

#ifndef COLOR_H
#define COLOR_H

#include "temp.h"
#include "graph.h"
#include "liveness.h"

struct COL_result
{
    Temp_map coloring;
    Temp_tempList spills;
};
struct COL_result COL_color(struct Live_graph ig, Temp_map initial, Temp_tempList regs);

#endif