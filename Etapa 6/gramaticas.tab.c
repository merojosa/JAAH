/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "gramaticas.cpp" /* yacc.c:339  */

#include <stdio.h>
#include <string>
#include "ArbolC++.h"
using namespace std;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);
Arbol arbol;
int cuenta = 0;
void imprimir_tabs(int cantidad_tabs)
{
	for(int contador = 0; contador < cantidad_tabs; ++contador)
	{
		cout << '\t';
	}
}


/**
* EFE: Lista las etiquetas del arbol haciendo un recorrido en pre-orden.
* REQ: Arbol inicializado.
* MOD: N/A.
*/
void listPreORecur(Arbol& arbol, Nodo nodo, int cantidad_tabs)
{
	imprimir_tabs(cantidad_tabs);\
	if(cantidad_tabs == 0){
		cout << *nodo->tokenName << endl;
	} else {
		if(arbol.esHoja(nodo)){
			cout<<"Instruccion ";
		} else {
			if(cantidad_tabs == 1){
				cout << "Metodo ";
			} else {
				cout<<"Bloque ";
			}
		}
		cout << *nodo->tokenName << " ";
	}				// Imprimo instrucción.
	if(nodo->params != 0x0){
		if(nodo->params->size() != 0){
			cout<<"Parametros: ";
			for(list<string*>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
				cout<<**it;
			}
		}
	}
	cout<<"\n";
	Nodo hijo = arbol.hijoMasIzq(nodo);

	if( hijo != nodoNulo )							// Si tiene hijo.
	{
		++cantidad_tabs;							// Bajo de nivel, aumento tab.
		listPreORecur(arbol, hijo, cantidad_tabs);	// Repito con el hijo.
		--cantidad_tabs;
		// Al salir del if, terminó de recorrer el nivel.  Por lo que resto un tab.
	}

	Nodo hermano_derecho = arbol.hermanoDer(nodo);
	if( hermano_derecho != nodoNulo )				// Verifico si tiene hermano.
	{
		listPreORecur(arbol, hermano_derecho, cantidad_tabs);
	}
	// Si no tiene hermano, termino (sea nivel o método).
}

// El llamado que se hace en el main.
void listPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		listPreORecur(arbol, arbol.raiz(), 0);
	}
}


#line 148 "gramaticas.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "gramaticas.tab.h".  */
#ifndef YY_YY_GRAMATICAS_TAB_H_INCLUDED
# define YY_YY_GRAMATICAS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 84 "gramaticas.cpp" /* yacc.c:355  */

	#include <list>
	#include <string>
	//#include "ArbolC++.h"
	using namespace std;

#line 185 "gramaticas.tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HILERA = 258,
    FOR = 259,
    WHILE = 260,
    IF = 261,
    ELSE = 262,
    COMPARACION = 263,
    AND_OR = 264,
    BOOL = 265,
    NUM = 266,
    INI = 267,
    FIN = 268,
    RET = 269,
    MAS = 270,
    MENOS = 271,
    POR = 272,
    ENTRE = 273,
    MOD = 274,
    ENTRADA = 275,
    SALIDA = 276,
    ID = 277,
    ERROR = 278,
    EJECUCION = 279,
    PARD = 280,
    CORD = 281,
    PARI = 282,
    CORI = 283,
    IGUAL = 284,
    DOSP = 285,
    COM = 286,
    PUNTO = 287,
    POINTER = 288,
    PYC = 289,
    LINK = 290,
    JAAH = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 90 "gramaticas.cpp" /* yacc.c:355  */

	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<string*> *params;

#line 242 "gramaticas.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMATICAS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 259 "gramaticas.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   194

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      35,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   161,   161,   167,   168,   172,   178,   186,   193,   200,
     204,   211,   217,   223,   229,   239,   240,   244,   249,   258,
     259,   260,   264,   265,   266,   267,   268,   273,   277,   282,
     286,   290,   293,   297,   302,   306,   309,   313,   317,   328,
     336,   337,   341,   348,   355,   360,   368,   373,   377,   384,
     390,   396,   405,   410,   414,   421,   429,   444,   452,   462,
     472,   476,   480,   481,   482,   483,   484
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"Hilera\"", "\"para\"", "\"mientras\"",
  "\"si\"", "\"sino\"", "\"comparador\"", "\"&& o ||\"",
  "\"verdadero o falso\"", "\"numero\"", "\"ini\"", "\"fin\"", "\"ret\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"", "\"entrada\"", "\"salida\"",
  "\"identificador\"", "ERROR", "\"principal\"", "\")\"", "\"]\"", "\"(\"",
  "\"[\"", "\"=\"", "\":\"", "\",\"", "\".\"", "\"->\"", "\";\"",
  "\"enlazador de archivos\"", "\"ruta del archivo\"", "$accept", "super",
  "inicio", "principal", "declarations", "assign", "concat_links",
  "operaciones", "tipos", "operando", "instrucciones", "method",
  "method_call", "method_arguments", "concat_IDs_operations",
  "comparaciones", "comparacionesbooleanas", "ret", "rule_IF", "rule_for",
  "rule_While", "rule_ELSE", "arreglo", "tamano_arreglo", "OPERACIONES", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291
};
# endif

#define YYPACT_NINF -63

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-63)))

#define YYTABLE_NINF -10

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -19,    99,   -28,    14,   -63,   -63,    10,    -8,     7,    18,
      80,     2,     7,    16,   -63,    40,    38,     7,   -63,    54,
     -63,    56,    52,    40,   161,   161,   -63,    62,   -63,   -63,
      79,   161,   -63,   161,   -63,   -63,   -63,    55,    60,   -63,
     -63,   161,   -63,   -63,   -63,   -63,   -63,   -63,   -63,   -63,
      80,    80,   -63,   132,   -63,   118,   -63,   -63,   -63,   156,
      72,    76,    91,    96,   100,    54,   111,   114,    82,   118,
      63,   134,   115,   116,    32,   118,   118,   118,    54,    54,
     -63,   129,    54,    54,   173,   145,   -63,   146,   136,    54,
     -63,   118,   -63,   118,   118,   137,   -63,   -63,   -63,   -63,
     -63,   -63,    65,   125,   173,   145,   139,   140,    54,    54,
      54,   127,   148,   -63,   -63,   -63,   -63,   118,    54,   118,
     118,   161,   -63,   161,   -63,   -63,   118,   118,   143,   128,
     159,   160,   -63,   -63,   -63,    80,   -63,   -63,    15,   161,
     118,   168,   -63
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       9,     0,     0,     0,     2,     4,     0,     0,     9,     0,
       0,     0,     9,    16,     1,     0,     6,     9,     3,     0,
      23,     0,    22,     0,    61,    60,    25,     0,    21,    20,
      22,    11,    10,    19,    14,     8,    15,     0,     0,     5,
       7,    13,    12,    24,    26,    62,    63,    64,    65,    66,
       0,     0,    59,     0,    39,    37,    17,    18,    41,    45,
      44,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,    37,    37,    37,    37,     0,     0,
      40,     0,     0,     0,    53,    52,    54,    47,     0,     0,
      35,    37,    38,    37,    37,     0,    31,    29,    32,    30,
      43,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,    27,    36,    34,    37,     0,    37,
      37,    51,    50,    49,    48,    46,    37,    37,     0,     0,
       0,     0,    33,    28,    58,     0,    57,    55,     0,     0,
      37,     0,    56
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -63,   -63,   -63,    70,    46,    11,   181,    -4,     6,    -6,
      43,   -63,    66,   -63,    67,   -62,   -63,   -63,   -63,   -63,
     -63,   -63,   -10,   -63,   -23
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    70,     8,   104,   105,    33,
      71,    16,    72,    54,    61,    86,    87,    73,    74,    75,
      76,    77,     9,    27,    50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,    26,    51,     1,    25,    28,    24,    31,    13,    26,
      51,     7,    29,    20,    14,    41,     2,    32,    21,     7,
     106,   107,    15,     7,    30,    42,    17,     7,     7,     1,
      45,    46,    47,    48,    49,    23,    62,    63,    64,    95,
      26,    26,   140,    26,    56,    57,    65,    19,   125,    59,
      -9,     2,    66,    67,    68,    26,   129,    28,    35,    60,
       1,    84,    37,    40,    29,    20,    69,    43,    26,    26,
      21,    85,    26,    26,    59,    59,    22,    34,    18,    26,
      10,    38,    53,   113,    60,    60,    39,    23,    52,    44,
      55,    20,   103,    10,    11,   112,    21,    91,    26,    26,
      26,    80,    22,    79,   121,   123,    53,    10,    26,    53,
      10,    11,    90,    23,   122,   124,    51,    96,    97,    98,
      99,    81,    62,    63,    64,    26,    82,    10,    11,   139,
      83,   138,    65,    12,   114,    28,   115,   116,    66,    67,
      68,    88,    29,    20,    89,   100,   101,    92,    21,    93,
      94,   102,    69,   109,    22,   110,   118,    58,   111,   135,
     128,   126,   130,   131,   117,    23,   119,   120,   134,   132,
     133,    45,    46,    47,    48,    49,    45,    46,    47,    48,
      49,   108,   127,   141,   136,   137,     0,    78,    45,    46,
      47,    48,    49,   142,    36
};

static const yytype_int16 yycheck[] =
{
      10,    11,    25,    22,    10,     3,    10,    11,    36,    19,
      33,     0,    10,    11,     0,    19,    35,    11,    16,     8,
      82,    83,    12,    12,    22,    19,    34,    16,    17,    22,
      15,    16,    17,    18,    19,    33,     4,     5,     6,     7,
      50,    51,    27,    53,    50,    51,    14,    29,   110,    53,
      12,    35,    20,    21,    22,    65,   118,     3,    12,    53,
      22,    65,    22,    17,    10,    11,    34,    11,    78,    79,
      16,    65,    82,    83,    78,    79,    22,    11,     8,    89,
      28,    15,    27,    89,    78,    79,    16,    33,    26,    23,
      30,    11,    81,    28,    29,    89,    16,    34,   108,   109,
     110,    25,    22,    31,   108,   109,    27,    28,   118,    27,
      28,    29,    69,    33,   108,   109,   139,    74,    75,    76,
      77,    30,     4,     5,     6,   135,    30,    28,    29,   135,
      30,   135,    14,    34,    91,     3,    93,    94,    20,    21,
      22,    30,    10,    11,    30,    78,    79,    13,    16,    34,
      34,    22,    34,     8,    22,     9,    31,    25,    22,    31,
     117,    34,   119,   120,    27,    33,    27,    27,    25,   126,
     127,    15,    16,    17,    18,    19,    15,    16,    17,    18,
      19,     8,    34,   140,    25,    25,    -1,    31,    15,    16,
      17,    18,    19,    25,    13
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    22,    35,    38,    39,    40,    41,    42,    43,    59,
      28,    29,    34,    36,     0,    12,    48,    34,    40,    29,
      11,    16,    22,    33,    44,    46,    59,    60,     3,    10,
      22,    44,    45,    46,    49,    41,    43,    22,    49,    40,
      41,    44,    45,    11,    49,    15,    16,    17,    18,    19,
      61,    61,    26,    27,    50,    30,    46,    46,    25,    44,
      45,    51,     4,     5,     6,    14,    20,    21,    22,    34,
      42,    47,    49,    54,    55,    56,    57,    58,    31,    31,
      25,    30,    30,    30,    44,    45,    52,    53,    30,    30,
      47,    34,    13,    34,    34,     7,    47,    47,    47,    47,
      51,    51,    22,    42,    44,    45,    52,    52,     8,     8,
       9,    22,    45,    46,    47,    47,    47,    27,    31,    27,
      27,    44,    45,    44,    45,    52,    34,    34,    47,    52,
      47,    47,    47,    47,    25,    31,    25,    25,    44,    46,
      27,    47,    25
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    41,    41,    41,
      42,    42,    42,    42,    42,    43,    43,    44,    44,    45,
      45,    45,    46,    46,    46,    46,    46,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    48,    49,
      50,    50,    51,    51,    51,    51,    52,    52,    53,    53,
      53,    53,    54,    54,    54,    55,    56,    57,    58,    59,
      60,    60,    61,    61,    61,    61,    61
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     2,     3,     3,     0,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     5,     2,
       2,     2,     2,     5,     3,     2,     3,     0,     5,     2,
       3,     2,     3,     3,     1,     1,     3,     1,     3,     3,
       3,     3,     2,     2,     2,     6,    10,     6,     5,     4,
       1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 2:
#line 162 "gramaticas.cpp" /* yacc.c:1646  */
    {
		string* root = new std::string("Programa:"); arbol.ponerRaiz(cuenta,root); arbol.agregarHijo(arbol.raiz(),(yyvsp[0].nodo)); listPreO(arbol);cuenta++;}
#line 1444 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 167 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 1450 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 168 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 1456 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 173 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[-1].nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[-2].nodo) != nodoNulo){(yyvsp[-2].nodo)->HD = (yyvsp[-1].nodo);}
		if((yyvsp[-2].nodo) != nodoNulo){(yyval.nodo) = (yyvsp[-2].nodo);} else { (yyval.nodo) = (yyvsp[-1].nodo);}
		}
#line 1466 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 179 "gramaticas.cpp" /* yacc.c:1646  */
    {
		if((yyvsp[-1].nodo) != nodoNulo){(yyvsp[-1].nodo)->HD = (yyvsp[0].nodo); (yyval.nodo) = (yyvsp[-1].nodo);}
		else { (yyval.nodo) = (yyvsp[0].nodo);}
	}
#line 1475 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 187 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		if((yyvsp[0].nodo) != nodoNulo){
			(yyvsp[-2].nodo)->HD = (yyvsp[0].nodo);
		}
	}
#line 1486 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 194 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-2].hilera),NULL,NULL);
		if((yyvsp[0].nodo) != nodoNulo){
			(yyval.nodo)->HD = (yyvsp[0].nodo);
		}
	}
#line 1497 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 200 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = nodoNulo;}
#line 1503 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 205 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-2].hilera),NULL,NULL);
		(yyval.nodo)->params = new list<string*>();
		(yyval.nodo)->params->push_back((yyvsp[-1].hilera));
		(yyval.nodo)->params->push_back((yyvsp[0].nodo)->tokenName);
		}
#line 1514 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 212 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-2].hilera),NULL,NULL);
		(yyvsp[0].params)->push_front((yyvsp[-1].hilera));
		(yyval.nodo)->params = (yyvsp[0].params);
	}
#line 1524 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 218 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->params->push_back((yyvsp[-1].hilera));
		(yyval.nodo)->params->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1534 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 224 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->params->push_back((yyvsp[-1].hilera));
		arbol.mergeLists((yyval.nodo)->params,(yyvsp[0].params));
	}
#line 1544 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 230 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-2].hilera),NULL,NULL);
		(yyval.nodo)->params = new list<string*>();
		(yyval.nodo)->params->push_back((yyvsp[-1].hilera));
		(yyval.nodo)->params->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1555 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 245 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params)->push_back((yyvsp[-1].hilera));
		(yyval.params)->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1564 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 249 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<string*>();
		(yyval.params)->push_back((yyvsp[-2].nodo)->tokenName);
		(yyval.params)->push_back((yyvsp[-1].hilera));
		(yyval.params)->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1575 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 258 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 1581 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 259 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);}
#line 1587 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 260 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);}
#line 1593 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 264 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);}
#line 1599 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 265 "gramaticas.cpp" /* yacc.c:1646  */
    {cuenta++;string* num = new string(to_string((yyvsp[0].intVal)));(yyval.nodo) = new Caja(cuenta,num,NULL,NULL);}
#line 1605 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 266 "gramaticas.cpp" /* yacc.c:1646  */
    {cuenta++;int value = 0-(yyvsp[0].intVal); string* sValue = new string(to_string(value)); (yyval.nodo) = new Caja(cuenta, sValue,NULL,NULL);}
#line 1611 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 267 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 1617 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 268 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 1623 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 273 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1632 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 277 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-4].hilera),(yyvsp[0].nodo),NULL);
		(yyval.nodo)->params = new list<string*>();
		(yyval.nodo)->params->push_back((yyvsp[-2].nodo)->tokenName);
	}
#line 1642 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 282 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1651 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 286 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD->HD = (yyvsp[0].nodo);
	}
#line 1660 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 290 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1668 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 293 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1677 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 297 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-4].hilera),(yyvsp[0].nodo),NULL);
		(yyval.nodo)->params = new list<string*>();
		(yyval.nodo)->params->push_back((yyvsp[-2].hilera));
	}
#line 1687 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 302 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1696 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 306 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[0].nodo);
	}
#line 1704 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 309 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
	}
#line 1713 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 313 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = nodoNulo;}
#line 1719 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 319 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-3].nodo);
		if((yyvsp[-1].nodo) != nodoNulo)
		{(yyval.nodo)->HMI = (yyvsp[-1].nodo);}

	}
#line 1730 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 329 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[0].params);
	}
#line 1739 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 336 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = (yyvsp[-1].params);}
#line 1745 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 337 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = NULL;}
#line 1751 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 342 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<string*>();
			(yyval.params)->push_front((yyvsp[-2].nodo)->tokenName);
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
			delete (yyvsp[0].params);
		}
#line 1762 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 349 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<string*>();
			(yyvsp[0].params)->push_front((yyvsp[-1].hilera));
			arbol.mergeLists((yyvsp[0].params),(yyvsp[-2].params));
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
		}
#line 1773 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 356 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<string*>();
			(yyval.params)->push_back((yyvsp[0].nodo)->tokenName);
		}
#line 1782 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 361 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<string*>();
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
		}
#line 1791 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 368 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[-2].params)->push_back((yyvsp[-1].hilera));
		arbol.mergeLists((yyvsp[-2].params),(yyvsp[0].params));
		(yyval.params) = (yyvsp[-2].params);
	}
#line 1801 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 373 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = (yyvsp[0].params);}
#line 1807 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 378 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<string*>();
		(yyval.params)->push_back((yyvsp[-2].nodo)->tokenName);
		(yyval.params)->push_back((yyvsp[-1].hilera));
		(yyval.params)->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1818 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 385 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[0].params)->push_front((yyvsp[-1].hilera));
		(yyvsp[0].params)->push_front((yyvsp[-2].nodo)->tokenName);
		(yyval.params) = (yyvsp[0].params);
	}
#line 1828 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 391 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[-2].params)->push_back((yyvsp[-1].hilera));
		(yyvsp[-2].params)->push_back((yyvsp[0].nodo)->tokenName);
		(yyval.params) = (yyvsp[-2].params);
	}
#line 1838 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 397 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[-2].params)->push_back((yyvsp[-1].hilera));
		arbol.mergeLists((yyvsp[-2].params), (yyvsp[0].params));
		(yyval.params) = (yyvsp[-2].params);
	}
#line 1848 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 405 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = new list<string*>();
		(yyval.nodo)->params->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1858 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 410 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[0].params);
	}
#line 1867 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 414 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[0].params);
	}
#line 1876 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 422 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-5].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[-3].params); (yyval.nodo)->HMI = (yyvsp[-1].nodo);
	}
#line 1885 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 430 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.nodo) = new Caja(cuenta++,(yyvsp[-9].hilera),NULL,NULL);
			/*for(Nodo tmp = $9; tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = $$;
			}*/
			(yyval.nodo)->params = new list<string*>();
		/*	arbol.mergeLists($$->params,$3->params);
			arbol.mergeLists($$->params, $5);
			arbol.mergeLists($$->params,$7);*/
			(yyval.nodo)->HD = (yyvsp[-1].nodo);
		}
#line 1901 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 444 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-5].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[-3].params);
		arbol.agregarHijo((yyval.nodo),(yyvsp[-1].nodo));
	}
#line 1911 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 453 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-3].hilera),NULL,NULL);
		(yyval.nodo)->HMI = (yyvsp[-1].nodo);
		(yyvsp[-4].nodo)->HD = (yyval.nodo);
		(yyval.nodo) = (yyvsp[-4].nodo);
	}
#line 1922 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 463 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-3].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[-1].params);
		(yyval.nodo)->params->push_front((yyvsp[-2].hilera));
		(yyval.nodo)->params->push_back((yyvsp[0].hilera));
	}
#line 1933 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 472 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<string*>();
		(yyval.params)->push_back((yyvsp[0].nodo)->tokenName);
	}
#line 1942 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 476 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = (yyvsp[0].params);}
#line 1948 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 480 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 1954 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 481 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 1960 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 482 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 1966 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 483 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 1972 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 484 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 1978 "gramaticas.tab.c" /* yacc.c:1646  */
    break;


#line 1982 "gramaticas.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 488 "gramaticas.cpp" /* yacc.c:1906  */

int main(int argc, char** argv) {
	if(argc > 1){
		yyin = fopen(argv[0],"r");
	} else {
		yyin = stdin;
	}
	yyparse();
	return 0;
}

void printError(string errormsg, char type){
	extern int yylineno;
	//printf("%s en la linea: %d\n", errormsg.c_str(), yylineno);
	cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
	if(type == 'a'){
		printf("El error es: %s\n",yytext);
		exit(-1);
	}
}

void yyerror(const char *s) {
	extern int yylineno;
	//printf("%s",s);
	printf("\n%s   , en la linea %d\n",s,yylineno);
	exit(-1);
}
