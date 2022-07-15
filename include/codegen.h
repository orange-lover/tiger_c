#include "frame.h"
#include "tree.h"
#include "assem.h"

AS_instrList F_codegen(F_frame f, T_stmList stmList);

AS_instrList AS_rewrite(AS_instrList il, Temp_map coloring);
AS_instrList As_rewriteSpill(F_frame f, AS_instrList il, Temp_tempList spills);
