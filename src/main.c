#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "frame.h"
#include "tree.h"
#include "assem.h"
#include "canon.h"
#include "codegen.h"
#include "parse.h"
#include "absyn.h"
#include "printtree.h"
#include "prabsyn.h"
#include "escape.h"
#include "semant.h"
#include "regalloc.h"

static void do_proc(FILE *out, F_frame frame, T_stm body)
{
    printf("do_proc 1 proc name:%s!\n", Temp_labelstring(F_name(frame)));
    //  return;
    AS_instrList instr_l = NULL;
    T_stmList stm_l = NULL;
    struct C_block block;

    //  if (!strcmp(Temp_labelstring(F_name(frame)), "readint"))
    //    pr_stm(stdout,body,0);

    /* canon */
    stm_l = C_linearize(body);
    //   if (!strcmp(Temp_labelstring(F_name(frame)), "L37"))
    //     printStmList(stdout, stm_l);

    // printf("do_proc C_linearize \n");
    block = C_basicBlocks(stm_l);
    //   if (!strcmp(Temp_labelstring(F_name(frame)), "L37"))
    //     printStmList(stdout, stm_l);

    // printf("do_proc C_basicBlocks \n");
    stm_l = C_traceSchedule(block);
    // printf("do_proc C_traceSchedule \n");

    //    if (!strcmp(Temp_labelstring(F_name(frame)), "readlist"))
    //     printStmList(stdout, stm_l);

    /* assembly */
    instr_l = F_codegen(frame, stm_l);
    // AS_printinstrlist(out,instr_l);
    // printf("do_proc F_codegen \n");
    instr_l = F_procEntryExit2(instr_l);
    // printf("do_proc F_procEntryExit2 \n");
    //     if ( !strcmp(Temp_labelstring(F_name(frame)),"L37"))
    // {
    //    AS_printinstrlist(stdout, instr_l);

    // }

    /* color */
    struct RA_result rt = RA_regAlloc(frame, instr_l, 0);
    // AS_printinstrlist(out, rt.il);
    //           if ( !strcmp(Temp_labelstring(F_name(frame)),"L37"))
    //   {
    //      AS_printinstrlist(out, rt.il);
    //      return;
    //   }
    // printf("do_proc RA_regAlloc \n");
    AS_proc proc = F_procEntryExit3(frame, instr_l);
    // printf("do_proc F_procEntryExit3 \n");

    fprintf(out, ".text\n.align 2\n.globl %s\n", Temp_labelstring(F_name(frame)));
    fprintf(out, "%s", proc->prolog);
    AS_printInstrList(out, rt.il, rt.coloring);
    // AS_printInstrList(out, instr_l, NULL);
    fprintf(out, "%s", proc->epilog);
    fprintf(out, "\n\n\n\n");
}
static void do_string(FILE *out, F_frag frag)
{
    fprintf(out, ".align 2\n");
    fprintf(out, "%s", F_string(frag));
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out filename\n");
        return 1;
    }

    /*absyn tree*/
    A_exp absyn_root = parse(String(argv[1]));
    if (absyn_root == NULL)
    {
        fprintf(stderr, "something wrong with parser\n");
        return 1;
    }

    // pr_exp(stdout,absyn_root,2);
    //  printf("absyn tree completed!\n");

    // FILE *out = stdout;
    FILE *out = fopen("a.s" , "w");

    /* Set escape varibles */
    Esc_findEscape(absyn_root);

    // printf("Set escape varibles completed!\n");

    /* Semantic and Translate */
    F_fragList frags = SEM_transProg(absyn_root);

    // printf("Semantic and Translate completed!\n");

    /* Assembly code */
    /* string */
    fprintf(out, ".data\n");
    for (F_fragList f = frags; f; f = f->tail)
    {
        if (f->head->kind == F_stringFrag)
        {
            do_string(out, f->head);
        }
    }
    // printf("Assembly code string completed!\n");
    /* proc */
    for (F_fragList f = frags; f; f = f->tail)
    {
        if (f->head->kind == F_procFrag)
        {
            do_proc(out, f->head->u.proc.frame, f->head->u.proc.body);
        }
    }
    // printf("Assembly code proc completed!\n");

    printf("Tiger code compile completed!\n");
    return 0;
}