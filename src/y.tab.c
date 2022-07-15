/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/tiger.y"

  /* C语法写的一些定义和声明 */
  #include <stdio.h>
  #include "util.h"
  #include "errormsg.h"
  #include "absyn.h"
  #include "symbol.h"

  extern int yylex(void);
  void yyerror(char *);

  A_exp absyn_root;

#line 85 "src/y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "y.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_COMMA = 6,                      /* COMMA  */
  YYSYMBOL_COLON = 7,                      /* COLON  */
  YYSYMBOL_SEMICOLON = 8,                  /* SEMICOLON  */
  YYSYMBOL_LPAREN = 9,                     /* LPAREN  */
  YYSYMBOL_RPAREN = 10,                    /* RPAREN  */
  YYSYMBOL_LBRACK = 11,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 12,                    /* RBRACK  */
  YYSYMBOL_LBRACE = 13,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 14,                    /* RBRACE  */
  YYSYMBOL_DOT = 15,                       /* DOT  */
  YYSYMBOL_PLUS = 16,                      /* PLUS  */
  YYSYMBOL_MINUS = 17,                     /* MINUS  */
  YYSYMBOL_TIMES = 18,                     /* TIMES  */
  YYSYMBOL_DIVIDE = 19,                    /* DIVIDE  */
  YYSYMBOL_EQ = 20,                        /* EQ  */
  YYSYMBOL_NEQ = 21,                       /* NEQ  */
  YYSYMBOL_LT = 22,                        /* LT  */
  YYSYMBOL_LE = 23,                        /* LE  */
  YYSYMBOL_GT = 24,                        /* GT  */
  YYSYMBOL_GE = 25,                        /* GE  */
  YYSYMBOL_AND = 26,                       /* AND  */
  YYSYMBOL_OR = 27,                        /* OR  */
  YYSYMBOL_ASSIGN = 28,                    /* ASSIGN  */
  YYSYMBOL_ARRAY = 29,                     /* ARRAY  */
  YYSYMBOL_IF = 30,                        /* IF  */
  YYSYMBOL_THEN = 31,                      /* THEN  */
  YYSYMBOL_ELSE = 32,                      /* ELSE  */
  YYSYMBOL_WHILE = 33,                     /* WHILE  */
  YYSYMBOL_FOR = 34,                       /* FOR  */
  YYSYMBOL_TO = 35,                        /* TO  */
  YYSYMBOL_DO = 36,                        /* DO  */
  YYSYMBOL_LET = 37,                       /* LET  */
  YYSYMBOL_IN = 38,                        /* IN  */
  YYSYMBOL_END = 39,                       /* END  */
  YYSYMBOL_OF = 40,                        /* OF  */
  YYSYMBOL_BREAK = 41,                     /* BREAK  */
  YYSYMBOL_NIL = 42,                       /* NIL  */
  YYSYMBOL_FUNCTION = 43,                  /* FUNCTION  */
  YYSYMBOL_VAR = 44,                       /* VAR  */
  YYSYMBOL_TYPE = 45,                      /* TYPE  */
  YYSYMBOL_LOW = 46,                       /* LOW  */
  YYSYMBOL_UMINUS = 47,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program = 49,                   /* program  */
  YYSYMBOL_exp = 50,                       /* exp  */
  YYSYMBOL_func_call = 51,                 /* func_call  */
  YYSYMBOL_explist = 52,                   /* explist  */
  YYSYMBOL_explist_nonempty = 53,          /* explist_nonempty  */
  YYSYMBOL_airth_exp = 54,                 /* airth_exp  */
  YYSYMBOL_cmp_exp = 55,                   /* cmp_exp  */
  YYSYMBOL_bool_exp = 56,                  /* bool_exp  */
  YYSYMBOL_record_create = 57,             /* record_create  */
  YYSYMBOL_record_create_list = 58,        /* record_create_list  */
  YYSYMBOL_record_create_list_nonempty = 59, /* record_create_list_nonempty  */
  YYSYMBOL_record_create_field = 60,       /* record_create_field  */
  YYSYMBOL_array_create = 61,              /* array_create  */
  YYSYMBOL_decs = 62,                      /* decs  */
  YYSYMBOL_dec = 63,                       /* dec  */
  YYSYMBOL_tydeclist = 64,                 /* tydeclist  */
  YYSYMBOL_tydec = 65,                     /* tydec  */
  YYSYMBOL_ty = 66,                        /* ty  */
  YYSYMBOL_tyfields = 67,                  /* tyfields  */
  YYSYMBOL_tyfields_nonempty = 68,         /* tyfields_nonempty  */
  YYSYMBOL_tyfield = 69,                   /* tyfield  */
  YYSYMBOL_vardec = 70,                    /* vardec  */
  YYSYMBOL_fundeclist = 71,                /* fundeclist  */
  YYSYMBOL_funcdec = 72,                   /* funcdec  */
  YYSYMBOL_lvalue = 73,                    /* lvalue  */
  YYSYMBOL_expsqe = 74                     /* expsqe  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  41
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   302

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  147

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    74,    74,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    98,    99,   100,   101,   102,   105,   106,
     107,   108,   111,   112,   113,   114,   115,   116,   119,   120,
     123,   124,   125,   126,   127,   128,   131,   134,   135,   136,
     137,   138,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   154,   155,   158,   159,   160,   161,   164,
     165,   166,   167,   170,   171
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "STRING", "INT",
  "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
  "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ",
  "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF",
  "THEN", "ELSE", "WHILE", "FOR", "TO", "DO", "LET", "IN", "END", "OF",
  "BREAK", "NIL", "FUNCTION", "VAR", "TYPE", "LOW", "UMINUS", "$accept",
  "program", "exp", "func_call", "explist", "explist_nonempty",
  "airth_exp", "cmp_exp", "bool_exp", "record_create",
  "record_create_list", "record_create_list_nonempty",
  "record_create_field", "array_create", "decs", "dec", "tydeclist",
  "tydec", "ty", "tyfields", "tyfields_nonempty", "tyfield", "vardec",
  "fundeclist", "funcdec", "lvalue", "expsqe", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-51)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      56,    15,   -51,   -51,    -1,    56,    56,    56,    11,   -25,
     -51,   -51,    37,   244,   -51,   -51,   -51,   -51,   -51,   -51,
       2,    56,    56,    35,   -51,   151,    29,   -51,   200,    99,
      27,    54,    55,    59,    25,   -25,   -51,    21,   -51,   -51,
      28,   -51,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    67,    56,    58,    62,   -51,
     216,    68,    73,   -51,    85,    56,   -51,    56,    56,    56,
      87,     3,    74,    56,   -51,   -51,   -51,    16,    16,    16,
      16,   267,   267,   267,   267,   267,   267,   277,   256,   232,
     -51,   244,    56,   -51,    60,    56,   -51,    35,   -51,   183,
     244,   163,    96,    98,    56,    -2,    63,   -51,   -51,    56,
     244,   -51,    56,    56,   100,    93,   -51,   102,    77,   244,
     -51,    96,    66,   -51,   -51,   244,   244,   130,   107,     5,
      96,    56,    97,   111,    56,   -51,   124,    56,   -51,   244,
     -51,   -51,   244,   108,   244,    56,   244
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    69,    12,    11,     0,     0,     0,     0,     0,    47,
       7,     9,     0,     2,    14,    15,    16,    17,    18,    19,
       3,    24,     0,    41,    10,    73,     0,    13,     0,     0,
       0,     0,     0,     0,     0,    47,    49,    52,    50,    51,
      65,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,     0,    25,
       0,     0,     0,    42,    43,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    48,    53,    66,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      71,     4,     0,    23,    70,     0,    40,     0,    74,     5,
       6,     0,    58,     0,     0,     0,     0,    72,    27,     0,
      45,    44,     0,     0,     0,     0,    59,    60,     0,    63,
      55,    58,     0,    54,    21,    46,     8,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,    61,    64,
      56,    57,    20,     0,    67,     0,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -51,   -51,     0,   -51,   -51,    38,   -51,   -51,   -51,   -51,
     -51,    32,   -51,   -51,   101,   -51,    95,   -51,   -51,    12,
       8,   -51,   -51,   103,   -51,   -51,   -50
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    12,    25,    14,    58,    59,    15,    16,    17,    18,
      62,    63,    64,    19,    34,    35,    36,    37,   123,   115,
     116,   117,    38,    39,    40,    20,    26
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   120,     1,     2,     3,    27,    28,    29,     4,    24,
     103,   121,   136,    54,    30,    98,     5,    55,    31,    32,
      33,    57,    60,   106,    21,   137,    22,   122,    23,     6,
      56,   104,     7,     8,    44,    45,     9,    41,    61,    66,
      10,    11,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    69,    91,    70,    71,     1,
       2,     3,    72,    73,    92,     4,    33,    99,   100,   101,
      90,    31,    93,     5,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,     6,    96,    95,     7,
       8,    97,    57,     9,   105,   110,   102,    10,    11,   114,
     109,   118,   124,   129,   119,   131,   133,   128,   130,   125,
     135,   140,   126,   127,   141,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,   143,   145,   111,
     108,   139,    75,   132,   142,    68,    74,   144,   138,     0,
       0,     0,     0,    76,     0,   146,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,     0,    65,
       0,     0,     0,     0,     0,     0,   134,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     0,     0,     0,     0,     0,     0,     0,   113,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     0,     0,     0,     0,   112,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    94,     0,
       0,    67,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,   107,     0,     0,     0,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    42,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51
};

static const yytype_int16 yycheck[] =
{
       0,     3,     3,     4,     5,     5,     6,     7,     9,    10,
       7,    13,     7,    11,     3,    65,    17,    15,    43,    44,
      45,    21,    22,    73,     9,    20,    11,    29,    13,    30,
      28,    28,    33,    34,    18,    19,    37,     0,     3,    10,
      41,    42,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    28,    56,     3,     3,     3,
       4,     5,     3,    38,     6,     9,    45,    67,    68,    69,
       3,    43,    10,    17,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    30,    14,    20,    33,
      34,     6,    92,    37,    20,    95,     9,    41,    42,     3,
      40,     3,    39,    10,   104,    28,    40,     7,     6,   109,
       3,    14,   112,   113,     3,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     3,    20,    97,
      92,   131,    37,   121,   134,    36,    35,   137,   130,    -1,
      -1,    -1,    -1,    40,    -1,   145,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    32,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    12,    -1,
      -1,    31,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    12,    -1,    -1,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     9,    17,    30,    33,    34,    37,
      41,    42,    49,    50,    51,    54,    55,    56,    57,    61,
      73,     9,    11,    13,    10,    50,    74,    50,    50,    50,
       3,    43,    44,    45,    62,    63,    64,    65,    70,    71,
      72,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    11,    15,    28,    50,    52,    53,
      50,     3,    58,    59,    60,     8,    10,    31,    36,    28,
       3,     3,     3,    38,    62,    64,    71,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
       3,    50,     6,    10,    12,    20,    14,     6,    74,    50,
      50,    50,     9,     7,    28,    20,    74,    12,    53,    40,
      50,    59,    32,    35,     3,    67,    68,    69,     3,    50,
       3,    13,    29,    66,    39,    50,    50,    50,     7,    10,
       6,    28,    67,    40,    36,     3,     7,    20,    68,    50,
      14,     3,    50,     3,    50,    20,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    51,    52,    52,    53,    53,    54,    54,
      54,    54,    55,    55,    55,    55,    55,    55,    56,    56,
      57,    58,    58,    59,    59,    60,    61,    62,    62,    63,
      63,    63,    64,    64,    65,    66,    66,    66,    67,    67,
      68,    68,    69,    70,    70,    71,    71,    72,    72,    73,
      73,    73,    73,    74,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     3,     4,     4,     1,     6,     1,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       8,     5,     3,     4,     0,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     0,     1,     1,     3,     3,     6,     0,     2,     1,
       1,     1,     1,     2,     4,     1,     3,     3,     0,     1,
       1,     3,     3,     4,     6,     1,     2,     7,     9,     1,
       4,     3,     4,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: exp  */
#line 74 "src/tiger.y"
                                                        {absyn_root=(yyvsp[0].exp);}
#line 1274 "src/y.tab.c"
    break;

  case 3: /* exp: lvalue  */
#line 76 "src/tiger.y"
                                                        {(yyval.exp)=A_VarExp(EM_tokPos,(yyvsp[0].var));}
#line 1280 "src/y.tab.c"
    break;

  case 4: /* exp: lvalue ASSIGN exp  */
#line 77 "src/tiger.y"
                                                        {(yyval.exp)=A_AssignExp(EM_tokPos,(yyvsp[-2].var),(yyvsp[0].exp));}
#line 1286 "src/y.tab.c"
    break;

  case 5: /* exp: IF exp THEN exp  */
#line 78 "src/tiger.y"
                                                        {(yyval.exp)=A_IfExp(EM_tokPos,(yyvsp[-2].exp),(yyvsp[0].exp),NULL);}
#line 1292 "src/y.tab.c"
    break;

  case 6: /* exp: WHILE exp DO exp  */
#line 79 "src/tiger.y"
                                                        {(yyval.exp)=A_WhileExp(EM_tokPos,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1298 "src/y.tab.c"
    break;

  case 7: /* exp: BREAK  */
#line 80 "src/tiger.y"
                                                        {(yyval.exp)=A_BreakExp(EM_tokPos);}
#line 1304 "src/y.tab.c"
    break;

  case 8: /* exp: IF exp THEN exp ELSE exp  */
#line 81 "src/tiger.y"
                                                        {(yyval.exp)=A_IfExp(EM_tokPos,(yyvsp[-4].exp),(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1310 "src/y.tab.c"
    break;

  case 9: /* exp: NIL  */
#line 82 "src/tiger.y"
                                                        {(yyval.exp)=A_NilExp(EM_tokPos);}
#line 1316 "src/y.tab.c"
    break;

  case 10: /* exp: LPAREN RPAREN  */
#line 83 "src/tiger.y"
                                                        {(yyval.exp)=A_SeqExp(EM_tokPos,NULL);}
#line 1322 "src/y.tab.c"
    break;

  case 11: /* exp: INT  */
#line 84 "src/tiger.y"
                                                        {(yyval.exp)=A_IntExp(EM_tokPos,(yyvsp[0].ival));}
#line 1328 "src/y.tab.c"
    break;

  case 12: /* exp: STRING  */
#line 85 "src/tiger.y"
                                                        {(yyval.exp)=A_StringExp(EM_tokPos,(yyvsp[0].sval));}
#line 1334 "src/y.tab.c"
    break;

  case 13: /* exp: MINUS exp  */
#line 86 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_minusOp,A_IntExp(EM_tokPos,0),(yyvsp[0].exp));}
#line 1340 "src/y.tab.c"
    break;

  case 14: /* exp: func_call  */
#line 87 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1346 "src/y.tab.c"
    break;

  case 15: /* exp: airth_exp  */
#line 88 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1352 "src/y.tab.c"
    break;

  case 16: /* exp: cmp_exp  */
#line 89 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1358 "src/y.tab.c"
    break;

  case 17: /* exp: bool_exp  */
#line 90 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1364 "src/y.tab.c"
    break;

  case 18: /* exp: record_create  */
#line 91 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1370 "src/y.tab.c"
    break;

  case 19: /* exp: array_create  */
#line 92 "src/tiger.y"
                                                        {(yyval.exp)=(yyvsp[0].exp);}
#line 1376 "src/y.tab.c"
    break;

  case 20: /* exp: FOR ID ASSIGN exp TO exp DO exp  */
#line 93 "src/tiger.y"
                                                        {(yyval.exp)=A_ForExp(EM_tokPos,S_Symbol((yyvsp[-6].sval)),(yyvsp[-4].exp),(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1382 "src/y.tab.c"
    break;

  case 21: /* exp: LET decs IN expsqe END  */
#line 94 "src/tiger.y"
                                                        {(yyval.exp)=A_LetExp(EM_tokPos,(yyvsp[-3].decList),A_SeqExp(EM_tokPos,(yyvsp[-1].expList)));}
#line 1388 "src/y.tab.c"
    break;

  case 22: /* exp: LPAREN expsqe RPAREN  */
#line 95 "src/tiger.y"
                                                        {(yyval.exp)=A_SeqExp(EM_tokPos,(yyvsp[-1].expList));}
#line 1394 "src/y.tab.c"
    break;

  case 23: /* func_call: ID LPAREN explist RPAREN  */
#line 98 "src/tiger.y"
                                                        {(yyval.exp)=A_CallExp(EM_tokPos,S_Symbol((yyvsp[-3].sval)),(yyvsp[-1].expList));}
#line 1400 "src/y.tab.c"
    break;

  case 24: /* explist: %empty  */
#line 99 "src/tiger.y"
                                                        {(yyval.expList)=NULL;}
#line 1406 "src/y.tab.c"
    break;

  case 25: /* explist: explist_nonempty  */
#line 100 "src/tiger.y"
                                                        {(yyval.expList)=(yyvsp[0].expList);}
#line 1412 "src/y.tab.c"
    break;

  case 26: /* explist_nonempty: exp  */
#line 101 "src/tiger.y"
                                                        {(yyval.expList)=A_ExpList((yyvsp[0].exp),NULL);}
#line 1418 "src/y.tab.c"
    break;

  case 27: /* explist_nonempty: exp COMMA explist_nonempty  */
#line 102 "src/tiger.y"
                                                        {(yyval.expList)=A_ExpList((yyvsp[-2].exp),(yyvsp[0].expList));}
#line 1424 "src/y.tab.c"
    break;

  case 28: /* airth_exp: exp PLUS exp  */
#line 105 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_plusOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1430 "src/y.tab.c"
    break;

  case 29: /* airth_exp: exp MINUS exp  */
#line 106 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_minusOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1436 "src/y.tab.c"
    break;

  case 30: /* airth_exp: exp TIMES exp  */
#line 107 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_timesOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1442 "src/y.tab.c"
    break;

  case 31: /* airth_exp: exp DIVIDE exp  */
#line 108 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_divideOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1448 "src/y.tab.c"
    break;

  case 32: /* cmp_exp: exp EQ exp  */
#line 111 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_eqOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1454 "src/y.tab.c"
    break;

  case 33: /* cmp_exp: exp NEQ exp  */
#line 112 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_neqOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1460 "src/y.tab.c"
    break;

  case 34: /* cmp_exp: exp LT exp  */
#line 113 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_ltOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1466 "src/y.tab.c"
    break;

  case 35: /* cmp_exp: exp LE exp  */
#line 114 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_leOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1472 "src/y.tab.c"
    break;

  case 36: /* cmp_exp: exp GT exp  */
#line 115 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_gtOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1478 "src/y.tab.c"
    break;

  case 37: /* cmp_exp: exp GE exp  */
#line 116 "src/tiger.y"
                                                        {(yyval.exp)=A_OpExp(EM_tokPos,A_geOp,(yyvsp[-2].exp),(yyvsp[0].exp));}
#line 1484 "src/y.tab.c"
    break;

  case 38: /* bool_exp: exp AND exp  */
#line 119 "src/tiger.y"
                                                        {(yyval.exp)=A_IfExp(EM_tokPos,(yyvsp[-2].exp),(yyvsp[0].exp),A_IntExp(EM_tokPos,0));}
#line 1490 "src/y.tab.c"
    break;

  case 39: /* bool_exp: exp OR exp  */
#line 120 "src/tiger.y"
                                                        {(yyval.exp)=A_IfExp(EM_tokPos,(yyvsp[-2].exp),A_IntExp(EM_tokPos,1),(yyvsp[0].exp));}
#line 1496 "src/y.tab.c"
    break;

  case 40: /* record_create: ID LBRACE record_create_list RBRACE  */
#line 123 "src/tiger.y"
                                                        {(yyval.exp)=A_RecordExp(EM_tokPos,S_Symbol((yyvsp[-3].sval)),(yyvsp[-1].efieldList));}
#line 1502 "src/y.tab.c"
    break;

  case 41: /* record_create_list: %empty  */
#line 124 "src/tiger.y"
                                                        {(yyval.efieldList)=NULL;}
#line 1508 "src/y.tab.c"
    break;

  case 42: /* record_create_list: record_create_list_nonempty  */
#line 125 "src/tiger.y"
                                                        {(yyval.efieldList)=(yyvsp[0].efieldList);}
#line 1514 "src/y.tab.c"
    break;

  case 43: /* record_create_list_nonempty: record_create_field  */
#line 126 "src/tiger.y"
                                                        {(yyval.efieldList)=A_EfieldList((yyvsp[0].efield),NULL);}
#line 1520 "src/y.tab.c"
    break;

  case 44: /* record_create_list_nonempty: record_create_field COMMA record_create_list_nonempty  */
#line 127 "src/tiger.y"
                                                                                       {(yyval.efieldList)=A_EfieldList((yyvsp[-2].efield),(yyvsp[0].efieldList));}
#line 1526 "src/y.tab.c"
    break;

  case 45: /* record_create_field: ID EQ exp  */
#line 128 "src/tiger.y"
                                                        {(yyval.efield)=A_Efield(S_Symbol((yyvsp[-2].sval)),(yyvsp[0].exp));}
#line 1532 "src/y.tab.c"
    break;

  case 46: /* array_create: ID LBRACK exp RBRACK OF exp  */
#line 131 "src/tiger.y"
                                                        {(yyval.exp)=A_ArrayExp(EM_tokPos,S_Symbol((yyvsp[-5].sval)),(yyvsp[-3].exp),(yyvsp[0].exp));}
#line 1538 "src/y.tab.c"
    break;

  case 47: /* decs: %empty  */
#line 134 "src/tiger.y"
                                                        {(yyval.decList)=NULL;}
#line 1544 "src/y.tab.c"
    break;

  case 48: /* decs: dec decs  */
#line 135 "src/tiger.y"
                                                        {(yyval.decList)=A_DecList((yyvsp[-1].dec),(yyvsp[0].decList));}
#line 1550 "src/y.tab.c"
    break;

  case 49: /* dec: tydeclist  */
#line 136 "src/tiger.y"
                                                        {(yyval.dec)=(yyvsp[0].dec);}
#line 1556 "src/y.tab.c"
    break;

  case 50: /* dec: vardec  */
#line 137 "src/tiger.y"
                                                        {(yyval.dec)=(yyvsp[0].dec);}
#line 1562 "src/y.tab.c"
    break;

  case 51: /* dec: fundeclist  */
#line 138 "src/tiger.y"
                                                        {(yyval.dec)=(yyvsp[0].dec);}
#line 1568 "src/y.tab.c"
    break;

  case 52: /* tydeclist: tydec  */
#line 141 "src/tiger.y"
                                                        {(yyval.dec)=A_TypeDec(EM_tokPos,A_NametyList((yyvsp[0].namety),NULL));}
#line 1574 "src/y.tab.c"
    break;

  case 53: /* tydeclist: tydec tydeclist  */
#line 142 "src/tiger.y"
                                                        {(yyval.dec)=A_TypeDec(EM_tokPos,A_NametyList((yyvsp[-1].namety),(yyvsp[0].dec)->u.type));}
#line 1580 "src/y.tab.c"
    break;

  case 54: /* tydec: TYPE ID EQ ty  */
#line 143 "src/tiger.y"
                                                        {(yyval.namety)=A_Namety(S_Symbol((yyvsp[-2].sval)),(yyvsp[0].ty));}
#line 1586 "src/y.tab.c"
    break;

  case 55: /* ty: ID  */
#line 144 "src/tiger.y"
                                                        {(yyval.ty)=A_NameTy(EM_tokPos,S_Symbol((yyvsp[0].sval)));}
#line 1592 "src/y.tab.c"
    break;

  case 56: /* ty: LBRACE tyfields RBRACE  */
#line 145 "src/tiger.y"
                                                        {(yyval.ty)=A_RecordTy(EM_tokPos,(yyvsp[-1].fieldList));}
#line 1598 "src/y.tab.c"
    break;

  case 57: /* ty: ARRAY OF ID  */
#line 146 "src/tiger.y"
                                                        {(yyval.ty)=A_ArrayTy(EM_tokPos,S_Symbol((yyvsp[0].sval)));}
#line 1604 "src/y.tab.c"
    break;

  case 58: /* tyfields: %empty  */
#line 147 "src/tiger.y"
                                                        {(yyval.fieldList)=NULL;}
#line 1610 "src/y.tab.c"
    break;

  case 59: /* tyfields: tyfields_nonempty  */
#line 148 "src/tiger.y"
                                                        {(yyval.fieldList)=(yyvsp[0].fieldList);}
#line 1616 "src/y.tab.c"
    break;

  case 60: /* tyfields_nonempty: tyfield  */
#line 149 "src/tiger.y"
                                                        {(yyval.fieldList)=A_FieldList((yyvsp[0].field),NULL);}
#line 1622 "src/y.tab.c"
    break;

  case 61: /* tyfields_nonempty: tyfield COMMA tyfields_nonempty  */
#line 150 "src/tiger.y"
                                                        {(yyval.fieldList)=A_FieldList((yyvsp[-2].field),(yyvsp[0].fieldList));}
#line 1628 "src/y.tab.c"
    break;

  case 62: /* tyfield: ID COLON ID  */
#line 151 "src/tiger.y"
                                                        {(yyval.field)=A_Field(EM_tokPos,S_Symbol((yyvsp[-2].sval)),S_Symbol((yyvsp[0].sval)));}
#line 1634 "src/y.tab.c"
    break;

  case 63: /* vardec: VAR ID ASSIGN exp  */
#line 154 "src/tiger.y"
                                                        {(yyval.dec)=A_VarDec(EM_tokPos,S_Symbol((yyvsp[-2].sval)),NULL,(yyvsp[0].exp));}
#line 1640 "src/y.tab.c"
    break;

  case 64: /* vardec: VAR ID COLON ID ASSIGN exp  */
#line 155 "src/tiger.y"
                                                        {(yyval.dec)=A_VarDec(EM_tokPos,S_Symbol((yyvsp[-4].sval)),S_Symbol((yyvsp[-2].sval)),(yyvsp[0].exp));}
#line 1646 "src/y.tab.c"
    break;

  case 65: /* fundeclist: funcdec  */
#line 158 "src/tiger.y"
                                                        {(yyval.dec)=A_FunctionDec(EM_tokPos,A_FundecList((yyvsp[0].fundec),NULL));}
#line 1652 "src/y.tab.c"
    break;

  case 66: /* fundeclist: funcdec fundeclist  */
#line 159 "src/tiger.y"
                                                        {(yyval.dec)=A_FunctionDec(EM_tokPos,A_FundecList((yyvsp[-1].fundec),(yyvsp[0].dec)->u.function));}
#line 1658 "src/y.tab.c"
    break;

  case 67: /* funcdec: FUNCTION ID LPAREN tyfields RPAREN EQ exp  */
#line 160 "src/tiger.y"
                                                        {(yyval.fundec)=A_Fundec(EM_tokPos,S_Symbol((yyvsp[-5].sval)),NULL,(yyvsp[-3].fieldList),(yyvsp[0].exp));}
#line 1664 "src/y.tab.c"
    break;

  case 68: /* funcdec: FUNCTION ID LPAREN tyfields RPAREN COLON ID EQ exp  */
#line 161 "src/tiger.y"
                                                               {(yyval.fundec)=A_Fundec(EM_tokPos,S_Symbol((yyvsp[-7].sval)),S_Symbol((yyvsp[-2].sval)),(yyvsp[-5].fieldList),(yyvsp[0].exp));}
#line 1670 "src/y.tab.c"
    break;

  case 69: /* lvalue: ID  */
#line 164 "src/tiger.y"
                                                        {(yyval.var)=A_SimpleVar(EM_tokPos,S_Symbol((yyvsp[0].sval)));}
#line 1676 "src/y.tab.c"
    break;

  case 70: /* lvalue: ID LBRACK exp RBRACK  */
#line 165 "src/tiger.y"
                                                        {(yyval.var)=A_SubscriptVar(EM_tokPos,A_SimpleVar(EM_tokPos,S_Symbol((yyvsp[-3].sval))),(yyvsp[-1].exp)); }
#line 1682 "src/y.tab.c"
    break;

  case 71: /* lvalue: lvalue DOT ID  */
#line 166 "src/tiger.y"
                                                        {(yyval.var)=A_FieldVar(EM_tokPos,(yyvsp[-2].var),S_Symbol((yyvsp[0].sval)));}
#line 1688 "src/y.tab.c"
    break;

  case 72: /* lvalue: lvalue LBRACK exp RBRACK  */
#line 167 "src/tiger.y"
                                                        {(yyval.var)=A_SubscriptVar(EM_tokPos,(yyvsp[-3].var),(yyvsp[-1].exp));}
#line 1694 "src/y.tab.c"
    break;

  case 73: /* expsqe: exp  */
#line 170 "src/tiger.y"
                                                        {(yyval.expList)=A_ExpList((yyvsp[0].exp),NULL);}
#line 1700 "src/y.tab.c"
    break;

  case 74: /* expsqe: exp SEMICOLON expsqe  */
#line 171 "src/tiger.y"
                                                        {(yyval.expList)=A_ExpList((yyvsp[-2].exp),(yyvsp[0].expList));}
#line 1706 "src/y.tab.c"
    break;


#line 1710 "src/y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 173 "src/tiger.y"

/*辅助函数段*/
void yyerror(char *s)
{
  EM_error(EM_tokPos,s);
}

