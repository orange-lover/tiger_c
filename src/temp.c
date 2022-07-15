#include "temp.h"
#include "util.h"
#include "table.h"

struct Temp_temp_
{
   int num;
};

static int temps = 100;
Temp_temp Temp_newtemp(void)
{
   Temp_temp p = checked_malloc(sizeof(*p));
   p->num = temps++;

   char ch[16];
   sprintf(ch, "%d", p->num);
   Temp_enter(Temp_name(), p, String(ch));

   return p;
}

static int labels = 0;
Temp_label Temp_newlabel(void)
{
   char ch[100];
   sprintf(ch, "L%d", labels++);
   return Temp_namedlabel(String(ch));
}
Temp_label Temp_namedlabel(string name)
{
   return S_Symbol(name);
}
string Temp_labelstring(Temp_label s)
{
   return S_name(s);
}

Temp_tempList Temp_TempList(Temp_temp head, Temp_tempList tail)
{
   Temp_tempList tl = checked_malloc(sizeof(*tl));
   tl->head = head;
   tl->tail = tail;
   return tl;
}
Temp_labelList Temp_LabelList(Temp_label head, Temp_labelList tail)
{
   Temp_labelList tl = checked_malloc(sizeof(*tl));
   tl->head = head;
   tl->tail = tail;
   return tl;
}

struct Temp_map_
{
   TAB_table tab;
   Temp_map under;
};

Temp_map Temp_name(void)
{
   static Temp_map m = NULL;
   if (!m)
      m = Temp_empty();
   return m;
}
Temp_map newMap(TAB_table tab, Temp_map under)
{
   Temp_map m = checked_malloc(sizeof(*m));
   m->tab = tab;
   m->under = under;
   return m;
}
Temp_map Temp_empty(void)
{
   return newMap(TAB_empty(), NULL);
}
Temp_map Temp_layerMap(Temp_map over, Temp_map under)
{
   if (over == NULL)
      return under;

   return newMap(over->tab, Temp_layerMap(over->under, under));
}
void Temp_enter(Temp_map m, Temp_temp t, string s)
{
   if (m->tab == NULL)
   {
      printf("Temp_enter map is null \n");
   }
   if (t == NULL)
   {
      printf("Temp_enter t is null \n");
   }
   TAB_enter(m->tab, t, s);
}
string Temp_look(Temp_map m, Temp_temp t)
{
   // TAB_look(m->tab, t);
   string s = TAB_look(m->tab, t);
   if (s)
   {
      return s;
   }
   else if (m->under)
   {
      return TAB_look(m->under->tab, t);
   }
   else
      return NULL;
}
static FILE *outfile;
void showit(Temp_temp t, string s)
{
   fprintf(outfile, "t%d -> %s\n", t->num, s);
}
void Temp_dumpMap(FILE *out, Temp_map m)
{
   outfile = out;
   TAB_dump(m->tab, (void (*)(void *, void *))showit);
   if (m->under)
   {
      fprintf(out, "-----------\n");
      Temp_dumpMap(out, m->under);
   }
}

bool Temp_tempListLook(Temp_tempList list, Temp_temp t)
{
   for (Temp_tempList iter = list; iter; iter = iter->tail)
   {
      if (iter->head == t)
         return true;
   }
   return false;
}
Temp_tempList Temp_tempListUnion(Temp_tempList lhs, Temp_tempList rhs)
{
   Temp_tempList ret = rhs;
   for (Temp_tempList it = lhs; it; it = it->tail)
   {
      Temp_temp lt = it->head;
      // remove NULL
      if (lt == NULL)
         continue;
      Temp_tempList rIt;
      for (rIt = rhs; rIt; rIt = rIt->tail)
      {
         if (lt == rIt->head)
            break;
      }
      if (rIt == NULL)
         ret = Temp_TempList(lt, ret);
   }
   return ret;
}
Temp_tempList Temp_tempListDiff(Temp_tempList lhs, Temp_tempList rhs)
{
   Temp_tempList ret = NULL;
   for (Temp_tempList lIt = lhs; lIt; lIt = lIt->tail)
   {
      Temp_temp nl = lIt->head;
      Temp_tempList rIt = NULL;
      for (rIt = rhs; rIt; rIt = rIt->tail)
      {
         Temp_temp nr = rIt->head;
         if (nl == nr)
            break;
      }
      if (rIt == NULL)
         ret = Temp_TempList(nl, ret);
   }
   return ret;
}
void Temp_tempReplace(Temp_tempList l, Temp_temp origin, Temp_temp newTemp)
{
   for (; l; l = l->tail)
   {
      if (l->head == origin)
      {
         l->head = newTemp;
      }
   }
}
int Temp_getnum(Temp_temp t)
{
   return t->num;
}
void Temp_dumpList(FILE *out, Temp_tempList list)
{
   for (Temp_tempList iter = list; iter; iter = iter->tail)
   {
      fprintf(out, "temp%d, ", iter->head->num);
   }
   fprintf(out, "\n");
}