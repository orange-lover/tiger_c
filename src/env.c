#include <stdio.h>
#include "env.h"
#include "util.h"
#include "table.h"
#include "translate.h"
#include "temp.h"

E_enventry E_VarEntry(Tr_access access, Ty_ty ty)
{
    E_enventry p = checked_malloc(sizeof(*p));
    p->kind = E_varEntry;
    p->u.var.ty = ty;
    p->u.var.access = access;
    return p;
}
E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty result)
{
    E_enventry p = checked_malloc(sizeof(*p));
    p->kind = E_funEntry;
    p->u.fun.level = level;
    p->u.fun.label = label;
    p->u.fun.formals = formals;
    p->u.fun.result = result;
    return p;
}
S_table E_base_tenv(void)
{
    S_table tenv = S_empty();
    S_enter(tenv, S_Symbol("int"), Ty_Int());
    S_enter(tenv, S_Symbol("string"), Ty_String());
    S_enter(tenv, S_Symbol("nil"), Ty_Nil());
    return tenv;
}
S_table E_base_venv(void)
{
    // S_table venv = S_empty();
    // S_enter(venv, S_Symbol("print"), E_FunEntry(NULL, Temp_namedlabel("print"), Ty_TyList(Ty_String(), NULL), Ty_Void()));
    // S_enter(venv, S_Symbol("flush"), E_FunEntry(NULL, Temp_namedlabel("flush"), NULL, Ty_Void()));
    // S_enter(venv, S_Symbol("getChar"), E_FunEntry(NULL, Temp_namedlabel("getChar"), NULL, Ty_String()));
    // S_enter(venv, S_Symbol("ord"), E_FunEntry(NULL, Temp_namedlabel("ord"), Ty_TyList(Ty_String(), NULL), Ty_Int()));
    // S_enter(venv, S_Symbol("chr"), E_FunEntry(NULL, Temp_namedlabel("chr"), Ty_TyList(Ty_Int(), NULL), Ty_String()));
    // S_enter(venv, S_Symbol("size"), E_FunEntry(NULL, Temp_namedlabel("size"), Ty_TyList(Ty_String(), NULL), Ty_Int()));
    // S_enter(venv, S_Symbol("substring"), E_FunEntry(NULL, Temp_namedlabel("substring"), Ty_TyList(Ty_String(), Ty_TyList(Ty_Int(), Ty_TyList(Ty_Int(), NULL))), Ty_String()));
    // S_enter(venv, S_Symbol("concat"), E_FunEntry(NULL, Temp_namedlabel("concat"), Ty_TyList(Ty_String(), Ty_TyList(Ty_String(), NULL)), Ty_String()));
    // S_enter(venv, S_Symbol("not"), E_FunEntry(NULL, Temp_namedlabel("not"), Ty_TyList(Ty_Int(), NULL), Ty_Int()));
    // S_enter(venv, S_Symbol("exit"), E_FunEntry(NULL, Temp_namedlabel("exit"), Ty_TyList(Ty_Int(), NULL), Ty_Void()));
    // S_enter(venv, S_Symbol("stringcmp"), E_FunEntry(NULL, Temp_namedlabel("stringcmp"), Ty_TyList(Ty_String(), Ty_TyList(Ty_String(), NULL)), Ty_Int()));
    // S_enter(venv, S_Symbol("printStr"), E_FunEntry(NULL, Temp_namedlabel("printStr"), Ty_TyList(Ty_String(), NULL), Ty_Void()));
    // S_enter(venv, S_Symbol("printInt"), E_FunEntry(NULL, Temp_namedlabel("printInt"), Ty_TyList(Ty_Int(), NULL), Ty_Void()));
    // return venv;

    S_table tmp = S_empty();
	S_beginScope(tmp);
	S_enter(tmp,S_Symbol(String("printStr")),E_FunEntry(NULL, Temp_namedlabel("printStr"),Ty_TyList(Ty_String(),NULL), Ty_Void()));
	S_enter(tmp,S_Symbol(String("printInt")),E_FunEntry(NULL, Temp_namedlabel("printInt"),Ty_TyList(Ty_Int(),NULL), Ty_Void()));
	S_enter(tmp,S_Symbol(String("flush")),E_FunEntry(NULL, Temp_namedlabel("flush"),NULL, Ty_Void()));
	S_enter(tmp,S_Symbol(String("tigerGetchar")),E_FunEntry(NULL, Temp_namedlabel("tigerGetchar"),NULL, Ty_String()));
	S_enter(tmp,S_Symbol(String("ord")),E_FunEntry(NULL, Temp_namedlabel("ord"),Ty_TyList(Ty_String(),NULL), Ty_Int()));
	S_enter(tmp,S_Symbol(String("chr")),E_FunEntry(NULL, Temp_namedlabel("chr"),Ty_TyList(Ty_Int(),NULL), Ty_String()));
	S_enter(tmp,S_Symbol(String("size")),E_FunEntry(NULL, Temp_namedlabel("size"),Ty_TyList(Ty_String(),NULL), Ty_Int()));
	S_enter(tmp,S_Symbol(String("substring")),E_FunEntry(NULL, Temp_namedlabel("substring"),Ty_TyList(Ty_String(),Ty_TyList(Ty_Int(),Ty_TyList(Ty_Int(),NULL))), Ty_String()));
	S_enter(tmp,S_Symbol(String("concat")),E_FunEntry(NULL, Temp_namedlabel("concat"),Ty_TyList(Ty_String(),Ty_TyList(Ty_String(),NULL)), Ty_String()));
	S_enter(tmp,S_Symbol(String("not")),E_FunEntry(NULL, Temp_namedlabel("not"),Ty_TyList(Ty_Int(),NULL), Ty_Int()));
	S_enter(tmp,S_Symbol(String("tigerExit")),E_FunEntry(NULL, Temp_namedlabel("tigerExit"),Ty_TyList(Ty_Int(),NULL), Ty_Void()));
	S_enter(tmp,S_Symbol(String("stringcmp")),E_FunEntry(NULL, Temp_namedlabel("stringcmp"),Ty_TyList(Ty_String(),Ty_TyList(Ty_String(),NULL)), Ty_Int()));
	return tmp;
}