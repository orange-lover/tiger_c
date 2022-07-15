/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"

bool anyErrors = false;
static string fileName = "";
typedef struct intList
{
    int i;
    struct intList *rest;
} * IntList;
int EM_tokPos = 0;
static int lineNum = 1;
static IntList linePos = NULL;
extern FILE *yyin;

static IntList intList(int i, IntList rest)
{
    IntList l = checked_malloc(sizeof *l);
    l->i = i;
    l->rest = rest;
    return l;
}

void EM_newline(void)
{
    lineNum++;
    linePos = intList(EM_tokPos, linePos);
}

void EM_error(int pos, char *message, ...)
{
    va_list ap;
    IntList lines = linePos;
    int num = lineNum;

    anyErrors = true;
    while (lines && lines->i >= pos)
    {
        lines = lines->rest;
        num--;
    }

    if (fileName)
        fprintf(stderr, "%s:", fileName);
    if (lines)
        fprintf(stderr, "%d.%d: ", num, pos - lines->i);
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

void EM_reset(string fname)
{
    anyErrors = false;
    fileName = fname;
    lineNum = 1;
    linePos = intList(0, NULL);
    yyin = fopen(fname, "r");
    if (!yyin)
    {
        EM_error(0, "cannot open");
        exit(1);
    }
}
