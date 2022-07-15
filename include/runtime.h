
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct String *string;
struct String
{
    int len;
    unsigned char content[1];
};
string constchar[256];
string emptychar;

unsigned char *initArray(int size, int initVal);
unsigned char *allocRecord(int size);
void printStr(string str);
void printInt(int i);
void flush();
string tigerGetchar();
int ord(string str);
string chr(int ch);
int size(string str);
string substring(string str, int first, int len);
int stringcmp(string s1, string s2);
string concat(string str1, string str2);
int not(int i);
void tigerExit(int i);
