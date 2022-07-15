#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include "util.h"

void *checked_malloc(int len)
{
    void *p = malloc(len);
    assert(p);
    return p;
}
string String(char *ch)
{
    string str = checked_malloc(strlen(ch) + 1);
    strcpy(str, ch);
    return str;
}
U_boolList U_BoolList(bool head, U_boolList tail)
{
    U_boolList bl = checked_malloc(sizeof(*bl));
    bl->head = head;
    bl->tail = tail;
    return bl;
}
string FormatString(string s, ...){
	va_list ap;
	char tmp[50];
	va_start(ap, s);
	int len = vsnprintf(tmp, 50, s, ap);
	va_end(ap);
	string res = checked_malloc(len+1);
	strcpy(res, tmp);
	return res;
}