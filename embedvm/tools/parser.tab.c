/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 20 "parser.y"


#define _GNU_SOURCE

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "evmcomp.h"

void yyerror (char const *s) {
	fprintf(stderr, "Parser error in line %d: %s\n", yyget_lineno(), s);
	exit(1);
}

struct nametab_entry_s {
	char *name;
	int type, index, num_args;
	struct evm_insn_s *addr;
	struct nametab_entry_s *next;
	bool is_forward_decl;
};

struct nametab_entry_s *goto_ids;
struct nametab_entry_s *local_ids;
struct nametab_entry_s *global_ids;

static struct nametab_entry_s *add_nametab_goto(char *name);
static struct nametab_entry_s *add_nametab_local(char *name, int index);
static struct nametab_entry_s *add_nametab_global(char *name, int type, struct evm_insn_s *addr);
static struct nametab_entry_s *find_nametab_goto(char *name);
static struct nametab_entry_s *find_nametab_entry(char *name);
static struct nametab_entry_s *find_global_nametab_entry(char *name);

struct loopcontext_s {
	struct evm_insn_s *break_insn;
	struct evm_insn_s *continue_insn;
	struct evm_insn_s *body_insn;
	struct loopcontext_s *next;
};

struct loopcontext_s *loopctx_stack;

struct evm_section_s *sections;

static struct evm_insn_s *generate_pre_post_inc_dec(struct evm_insn_s *lv,
		bool is_pre, bool is_inc, bool is_expr);

static struct evm_insn_s *generate_combined_assign(struct evm_insn_s *lv,
		struct evm_insn_s *action, bool is_pre, bool is_expr);

struct func_call_args_desc_s {
	struct evm_insn_s *insn;
	int num;
};

// there is no asprintf() on win32
int my_asprintf(char **strp, const char *fmt, ...)
{
	int size = 64, rc;
	va_list ap;
	*strp = malloc(size + 1);
	while (1) {
		va_start(ap, fmt);
		rc = vsnprintf(*strp, size + 1, fmt, ap);
		va_end(ap);
		if (rc >= 0 && rc < size) break;
		size *= 2;
		*strp = realloc(*strp, size + 1);
	}
	return rc;
}


#line 145 "parser.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_NUMBER = 258,
    TOK_USERFUNC = 259,
    TOK_ID = 260,
    TOK_STRINGLIT = 261,
    TOK_IF = 262,
    TOK_ELSE = 263,
    TOK_DO = 264,
    TOK_FOR = 265,
    TOK_WHILE = 266,
    TOK_BREAK = 267,
    TOK_CONTINUE = 268,
    TOK_GOTO = 269,
    TOK_RETURN = 270,
    TOK_FUNCTION = 271,
    TOK_LOCAL = 272,
    TOK_GLOBAL = 273,
    TOK_GLOBAL_8U = 274,
    TOK_GLOBAL_8S = 275,
    TOK_ARRAY_8U = 276,
    TOK_ARRAY_8S = 277,
    TOK_ARRAY_16 = 278,
    TOK_EXTERN = 279,
    TOK_MEMADDR = 280,
    TOK_SECTION = 281,
    TOK_TRAMPOLINE = 282,
    TOK_LINE = 283,
    TOK_VMIP = 284,
    TOK_VMSP = 285,
    TOK_VMSFP = 286,
    TOK_PTR_8U = 287,
    TOK_PTR_8S = 288,
    TOK_PTR_16 = 289,
    TOK_PTR_F = 290,
    TOK_LOR = 291,
    TOK_LAND = 292,
    TOK_EQ = 293,
    TOK_NE = 294,
    TOK_LE = 295,
    TOK_GE = 296,
    TOK_SHL = 297,
    TOK_SHR = 298,
    NEG = 299,
    TOK_INC = 300,
    TOK_DEC = 301,
    TOK_ASSIGN_ADD = 302,
    TOK_ASSIGN_SUB = 303,
    TOK_ASSIGN_MUL = 304,
    TOK_ASSIGN_DIV = 305,
    TOK_ASSIGN_MOD = 306,
    TOK_ASSIGN_SHL = 307,
    TOK_ASSIGN_SHR = 308,
    TOK_ASSIGN_AND = 309,
    TOK_ASSIGN_OR = 310,
    TOK_ASSIGN_XOR = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 95 "parser.y"

	int number;
	char *string;
	struct evm_insn_s *insn;
	struct func_call_args_desc_s *fc;
	struct loopcontext_s *loopctx;
	struct array_init_s ainit;
	void *vp;

#line 264 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   730

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  132
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  253

#define YYUNDEFTOK  2
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    79,     2,     2,     2,    55,    42,     2,
      76,    77,    53,    51,    69,    52,     2,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    73,
      45,    70,    48,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    74,     2,    75,    41,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    71,    40,    72,    78,     2,     2,     2,
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
      35,    36,    37,    43,    44,    46,    47,    49,    50,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   158,   158,   173,   181,   184,   187,   192,   199,   211,
     226,   227,   228,   231,   232,   233,   236,   237,   238,   241,
     245,   250,   258,   259,   260,   269,   270,   278,   279,   282,
     296,   321,   347,   350,   361,   407,   410,   414,   420,   423,
     428,   432,   436,   442,   445,   450,   453,   458,   466,   474,
     477,   480,   484,   490,   495,   501,   508,   513,   520,   527,
     530,   534,   534,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   560,   563,   566,   569,   572,   575,   578,
     583,   584,   588,   589,   590,   593,   594,   597,   622,   644,
     672,   690,   694,   705,   708,   711,   714,   720,   723,   727,
     730,   733,   746,   773,   776,   779,   782,   785,   788,   791,
     794,   797,   800,   803,   806,   809,   812,   819,   826,   833,
     836,   839,   842,   845,   848,   851,   854,   857,   860,   863,
     871,   874,   879
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_NUMBER", "TOK_USERFUNC", "TOK_ID",
  "TOK_STRINGLIT", "TOK_IF", "TOK_ELSE", "TOK_DO", "TOK_FOR", "TOK_WHILE",
  "TOK_BREAK", "TOK_CONTINUE", "TOK_GOTO", "TOK_RETURN", "TOK_FUNCTION",
  "TOK_LOCAL", "TOK_GLOBAL", "TOK_GLOBAL_8U", "TOK_GLOBAL_8S",
  "TOK_ARRAY_8U", "TOK_ARRAY_8S", "TOK_ARRAY_16", "TOK_EXTERN",
  "TOK_MEMADDR", "TOK_SECTION", "TOK_TRAMPOLINE", "TOK_LINE", "TOK_VMIP",
  "TOK_VMSP", "TOK_VMSFP", "TOK_PTR_8U", "TOK_PTR_8S", "TOK_PTR_16",
  "TOK_PTR_F", "TOK_LOR", "TOK_LAND", "'?'", "':'", "'|'", "'^'", "'&'",
  "TOK_EQ", "TOK_NE", "'<'", "TOK_LE", "TOK_GE", "'>'", "TOK_SHL",
  "TOK_SHR", "'+'", "'-'", "'*'", "'/'", "'%'", "NEG", "TOK_INC",
  "TOK_DEC", "TOK_ASSIGN_ADD", "TOK_ASSIGN_SUB", "TOK_ASSIGN_MUL",
  "TOK_ASSIGN_DIV", "TOK_ASSIGN_MOD", "TOK_ASSIGN_SHL", "TOK_ASSIGN_SHR",
  "TOK_ASSIGN_AND", "TOK_ASSIGN_OR", "TOK_ASSIGN_XOR", "','", "'='", "'{'",
  "'}'", "';'", "'['", "']'", "'('", "')'", "'~'", "'!'", "$accept",
  "input", "program", "meta_statement", "array_type", "global_type",
  "number", "array_init_data", "array_init", "global_var_init",
  "maybe_extern", "global_data", "function_head", "function_def",
  "function_args", "function_vars", "function_var_list", "function_body",
  "statement_list", "statement", "loop_body", "$@1", "combined_assign",
  "core_statement", "maybe_core_statement", "ptr_type", "ptr_index",
  "lvalue", "func_expression", "expression", "func_call_args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    63,    58,
     124,    94,    38,   293,   294,    60,   295,   296,    62,   297,
     298,    43,    45,    42,    47,    37,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,    44,
      61,   123,   125,    59,    91,    93,    40,    41,   126,    33
};
# endif

#define YYPACT_NINF (-221)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -221,    10,    60,  -221,    21,    66,    67,    71,  -221,   124,
    -221,    78,  -221,  -221,  -221,    73,    83,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,    84,    85,    19,   113,  -221,    46,
      49,   116,  -221,     0,     4,    50,  -221,   -60,    51,    54,
    -221,   122,   132,  -221,  -221,   143,   -40,    54,     2,    64,
    -221,  -221,  -221,   133,  -221,    76,  -221,     4,  -221,   148,
    -221,   -43,  -221,  -221,   -10,  -221,   272,   153,   144,     4,
    -221,    86,   -25,    87,  -221,    88,    89,    94,    96,   155,
     151,  -221,  -221,  -221,    97,    45,    45,  -221,  -221,  -221,
      99,   100,   347,  -221,  -221,   165,  -221,   217,   345,   217,
     217,   217,   166,   345,    59,   217,  -221,  -221,   118,  -221,
      -9,  -221,  -221,  -221,  -221,  -221,   187,   217,   217,    45,
      45,  -221,   217,   217,   217,   -14,  -221,   534,   217,   127,
    -221,  -221,   304,  -221,   217,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,   217,   217,  -221,
     656,   -56,  -221,   462,   -42,   346,   128,  -221,  -221,   134,
     384,  -221,  -221,  -221,  -221,  -221,  -221,   404,  -221,  -221,
    -221,  -221,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,  -221,   498,  -221,  -221,   602,   656,   656,   217,  -221,
    -221,  -221,   345,   217,   184,  -221,  -221,   675,   422,   636,
     513,   478,   547,   318,   318,   145,   145,   145,   145,    79,
      79,    27,    27,  -221,  -221,  -221,   129,   217,   131,   656,
     202,   442,    59,   568,  -221,   217,   217,   656,  -221,   345,
     138,   154,    59,   422,   -41,  -221,  -221,  -221,   156,  -221,
    -221,  -221,  -221
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,    27,     1,     0,     0,     0,     0,     4,     0,
       5,     0,     6,    28,     7,     0,     0,    32,    13,    14,
      15,    10,    11,    12,     0,     0,     0,     0,     8,     0,
      25,    35,     9,     0,     0,     0,    36,     0,     0,    22,
      16,     0,     0,    26,    29,     0,     0,    22,     0,     0,
      17,    18,    37,    38,    33,     0,    24,    19,    31,     0,
      43,     0,    30,    20,     0,    40,     0,     0,    39,     0,
      23,     0,    87,     0,    61,     0,     0,     0,     0,     0,
       0,    82,    83,    84,     0,     0,     0,    45,    34,    44,
       0,     0,     0,    73,    41,     0,    21,   130,     0,     0,
     130,     0,     0,     0,    80,     0,    57,    58,     0,    95,
      87,    96,    97,    98,    99,   100,     0,     0,     0,     0,
       0,    60,     0,     0,     0,   101,    93,     0,     0,    87,
      74,    75,     0,    49,     0,    76,    77,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,     0,     0,    42,
     131,     0,    47,     0,     0,     0,     0,    62,    81,     0,
       0,    48,   102,   103,   117,   125,   126,     0,   116,   118,
     127,   128,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,    50,    46,    85,    79,    78,     0,    91,
      88,    90,     0,     0,     0,    61,    94,   115,   114,     0,
     112,   113,   111,   121,   122,   119,   120,   123,   124,   109,
     110,   104,   105,   106,   107,   108,     0,     0,     0,   132,
      51,     0,    80,     0,    54,     0,   130,    86,    89,     0,
       0,     0,    80,   129,     0,    52,    53,    61,     0,    92,
      56,    61,    55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -221,  -221,  -221,  -221,  -221,  -221,   -51,  -221,   178,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,   -87,
    -190,  -221,  -221,  -103,  -220,  -221,  -221,   -66,   -64,   -76,
     -95
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     8,    24,    25,    43,    64,    49,    35,
       9,    10,    11,    12,    37,    60,    61,    66,   132,    89,
     102,   103,   148,    90,   159,    91,   228,   125,   126,   150,
     151
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   158,    93,    38,   127,   154,    63,    40,    56,    45,
       3,   152,   241,   198,    98,   234,   157,    46,    96,   130,
     131,   199,   248,   153,    13,   155,    67,   198,   198,   160,
      68,    53,    92,    54,    93,   201,   249,    92,    92,    93,
      93,   163,   164,   170,   171,   194,   167,   168,   169,    99,
     129,   100,   192,   165,   166,    41,    42,   250,   195,    69,
      -2,   252,    70,    71,   110,    99,    92,   100,    93,    14,
      15,   196,   197,    57,    16,    39,    27,    81,    82,    83,
     188,   189,   190,    26,     4,     5,     6,     7,    28,    29,
      30,    81,    82,    83,    84,    31,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   230,    85,    86,    32,    34,
      33,    36,   229,    44,    48,    50,    47,   231,   233,   158,
     186,   187,   188,   189,   190,    51,    92,    58,    93,   158,
      17,   244,    18,    19,    20,    21,    22,    23,    52,    62,
      59,   237,   245,    65,   109,    71,   110,   111,    94,   243,
     108,    95,    97,   101,   104,   105,    92,   106,    93,   107,
     149,   128,   133,    92,   134,    93,    92,   156,    93,   112,
     113,   114,   115,    81,    82,    83,    84,   109,    71,   110,
     111,   161,   162,   116,   184,   185,   186,   187,   188,   189,
     190,    99,   117,   118,   203,   236,   238,   204,   119,   120,
     239,   246,   112,   113,   114,   115,    81,    82,    83,    84,
     109,    71,   110,   111,   121,    55,   116,   122,     0,   123,
     124,   247,     0,   251,     0,   117,   118,     0,     0,     0,
       0,   119,   120,     0,     0,   112,   113,   114,   115,    81,
      82,    83,    84,     0,     0,     0,     0,   232,     0,   116,
     122,     0,   123,   124,     0,     0,     0,     0,   117,   118,
       0,     0,     0,     0,   119,   120,    71,    72,     0,    73,
       0,    74,    75,    76,    77,    78,    79,    80,     0,     0,
       0,     0,     0,   122,     0,   123,   124,     0,     0,     0,
       0,     0,     0,     0,    81,    82,    83,    84,    71,    72,
       0,    73,     0,    74,    75,    76,    77,    78,    79,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
      86,     0,     0,     0,     0,     0,    81,    82,    83,    84,
       0,     0,     0,    87,    88,     0,     0,     0,     0,    71,
      72,     0,    73,     0,    74,    75,    76,    77,    78,    79,
      80,    85,    86,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,    87,   193,    81,    82,    83,
      84,     0,   172,   173,   174,     0,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    85,    86,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    87,   147,     0,     0,
     172,   173,   174,   202,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     172,   173,   174,     0,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     174,   205,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   172,   173,
     174,   206,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   172,   173,
     174,     0,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   240,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   172,   173,   174,   200,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     172,   173,   174,   226,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   172,   173,   174,   191,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,   173,
     174,   242,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   172,   173,   174,   235,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   172,   173,   174,     0,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   173,   174,     0,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190
};

static const yytype_int16 yycheck[] =
{
      66,   104,    66,     3,    80,   100,    57,     3,     6,    69,
       0,    98,   232,    69,    39,   205,   103,    77,    69,    85,
      86,    77,   242,    99,     3,   101,    69,    69,    69,   105,
      73,    71,    98,    73,    98,    77,    77,   103,   104,   103,
     104,   117,   118,    57,    58,   132,   122,   123,   124,    74,
       5,    76,   128,   119,   120,    51,    52,   247,   134,    69,
       0,   251,    72,     4,     5,    74,   132,    76,   132,     3,
       3,   147,   148,    71,     3,    75,     3,    32,    33,    34,
      53,    54,    55,     5,    24,    25,    26,    27,     5,     5,
       5,    32,    33,    34,    35,    76,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   202,    57,    58,     5,    70,
      74,     5,   198,    73,    70,     3,    75,   203,   204,   232,
      51,    52,    53,    54,    55,     3,   202,    73,   202,   242,
      16,   236,    18,    19,    20,    21,    22,    23,     5,    73,
      17,   227,   239,     5,     3,     4,     5,     6,     5,   235,
       5,    17,    76,    76,    76,    76,   232,    73,   232,    73,
       5,    74,    73,   239,    74,   239,   242,    11,   242,    28,
      29,    30,    31,    32,    33,    34,    35,     3,     4,     5,
       6,    73,     5,    42,    49,    50,    51,    52,    53,    54,
      55,    74,    51,    52,    76,    76,    75,    73,    57,    58,
       8,    73,    28,    29,    30,    31,    32,    33,    34,    35,
       3,     4,     5,     6,    73,    47,    42,    76,    -1,    78,
      79,    77,    -1,    77,    -1,    51,    52,    -1,    -1,    -1,
      -1,    57,    58,    -1,    -1,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    73,    -1,    42,
      76,    -1,    78,    79,    -1,    -1,    -1,    -1,    51,    52,
      -1,    -1,    -1,    -1,    57,    58,     4,     5,    -1,     7,
      -1,     9,    10,    11,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,     4,     5,
      -1,     7,    -1,     9,    10,    11,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,     4,
       5,    -1,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    57,    58,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    71,    72,    32,    33,    34,
      35,    -1,    36,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    57,    58,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    71,    70,    -1,    -1,
      36,    37,    38,    77,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      36,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      38,    77,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    36,    37,
      38,    77,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    36,    37,
      38,    -1,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    77,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    36,    37,    38,    75,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      36,    37,    38,    75,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    36,    37,    38,    73,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    73,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    36,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    81,    82,     0,    24,    25,    26,    27,    83,    90,
      91,    92,    93,     3,     3,     3,     3,    16,    18,    19,
      20,    21,    22,    23,    84,    85,     5,     3,     5,     5,
       5,    76,     5,    74,    70,    89,     5,    94,     3,    75,
       3,    51,    52,    86,    73,    69,    77,    75,    70,    88,
       3,     3,     5,    71,    73,    88,     6,    71,    73,    17,
      95,    96,    73,    86,    87,     5,    97,    69,    73,    69,
      72,     4,     5,     7,     9,    10,    11,    12,    13,    14,
      15,    32,    33,    34,    35,    57,    58,    71,    72,    99,
     103,   105,   107,   108,     5,    17,    86,    76,    39,    74,
      76,    76,   100,   101,    76,    76,    73,    73,     5,     3,
       5,     6,    28,    29,    30,    31,    42,    51,    52,    57,
      58,    73,    76,    78,    79,   107,   108,   109,    74,     5,
     107,   107,    98,    73,    74,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    70,   102,     5,
     109,   110,    99,   109,   110,   109,    11,    99,   103,   104,
     109,    73,     5,   109,   109,   107,   107,   109,   109,   109,
      57,    58,    36,    37,    38,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    73,   109,    72,    99,   109,   109,   109,    69,    77,
      75,    77,    77,    76,    73,    77,    77,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,    75,    69,   106,   109,
      99,   109,    73,   109,   100,    39,    76,   109,    75,     8,
      77,   104,    73,   109,   110,    99,    73,    77,   104,    77,
     100,    77,   100
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    80,    81,    82,    82,    82,    82,    83,    83,    83,
      84,    84,    84,    85,    85,    85,    86,    86,    86,    87,
      87,    87,    88,    88,    88,    89,    89,    90,    90,    91,
      91,    91,    92,    93,    93,    94,    94,    94,    95,    95,
      96,    96,    96,    97,    97,    98,    98,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,   101,   100,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   103,   103,   103,   103,   103,   103,   103,
     104,   104,   105,   105,   105,   106,   106,   107,   107,   107,
     108,   108,   108,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     110,   110,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     2,     2,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     0,
       1,     3,     0,     4,     2,     0,     2,     0,     2,     5,
       8,     7,     2,     6,     9,     0,     1,     3,     0,     2,
       2,     3,     4,     0,     2,     0,     2,     3,     3,     2,
       3,     5,     7,     7,     5,     9,     8,     2,     2,     3,
       2,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       0,     1,     1,     1,     1,     0,     2,     1,     4,     5,
       4,     4,     7,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     5,
       0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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
  case 2:
#line 158 "parser.y"
                {
		struct nametab_entry_s *e;
		for (e = global_ids; e != NULL; e = e->next) {
			if (e->is_forward_decl) {
				fprintf(stderr, "Got forward declaration but no implementation for function `%s'!\n", e->name);
				exit(1);
			}
		}

		struct evm_insn_s *end = new_insn(NULL, NULL);
		end->symbol = strdup("_end");
		codegen(new_insn((yyvsp[0].insn), end));
	}
#line 1736 "parser.tab.c"
    break;

  case 3:
#line 173 "parser.y"
                    {
		(yyval.insn) = NULL;
		sections = NULL;
		goto_ids = NULL;
		local_ids = NULL;
		global_ids = NULL;
		loopctx_stack = NULL;
	}
#line 1749 "parser.tab.c"
    break;

  case 4:
#line 181 "parser.y"
                               {
		(yyval.insn) = new_insn((yyvsp[-1].insn), (yyvsp[0].insn));
	}
#line 1757 "parser.tab.c"
    break;

  case 5:
#line 184 "parser.y"
                            {
		(yyval.insn) = new_insn((yyvsp[-1].insn), (yyvsp[0].insn));
	}
#line 1765 "parser.tab.c"
    break;

  case 6:
#line 187 "parser.y"
                             {
		(yyval.insn) = new_insn((yyvsp[-1].insn), (yyvsp[0].insn));
	}
#line 1773 "parser.tab.c"
    break;

  case 7:
#line 192 "parser.y"
                               {
		(yyval.insn) = new_insn(NULL, NULL);
		if (my_asprintf(&(yyval.insn)->symbol, "_memaddr_%04x", (yyvsp[0].number)) < 0)
			abort();
		(yyval.insn)->has_set_addr = true;
		(yyval.insn)->set_addr = (yyvsp[0].number);
	}
#line 1785 "parser.tab.c"
    break;

  case 8:
#line 199 "parser.y"
                                         {
		struct nametab_entry_s *e = find_global_nametab_entry((yyvsp[0].string));
		if (!e || e->type != VARTYPE_FUNC) {
			fprintf(stderr, "Unkown function `%s' in line %d!\n", (yyvsp[0].string), yyget_lineno());
			exit(1);
		}
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 1, e->addr, NULL, NULL);
		if (my_asprintf(&(yyval.insn)->symbol, "_trampoline_%s", (yyvsp[0].string)) < 0)
			abort();
		(yyval.insn)->has_set_addr = true;
		(yyval.insn)->set_addr = (yyvsp[-1].number);
	}
#line 1802 "parser.tab.c"
    break;

  case 9:
#line 211 "parser.y"
                                                 {
		struct evm_section_s *sect = calloc(1, sizeof(struct evm_section_s));
		sect->begin = (yyvsp[-2].number);
		sect->end = (yyvsp[-1].number);
		sect->name = strdup((yyvsp[0].string));
		sect->next = sections;
		sections = sect;
		(yyval.insn) = new_insn(NULL, NULL);
		if (my_asprintf(&(yyval.insn)->symbol, "_section_%s", (yyvsp[0].string)) < 0)
			abort();
		(yyval.insn)->has_set_addr = true;
		(yyval.insn)->set_addr = (yyvsp[-2].number);
	}
#line 1820 "parser.tab.c"
    break;

  case 10:
#line 226 "parser.y"
                     { (yyval.number) = VARTYPE_ARRAY_8U; }
#line 1826 "parser.tab.c"
    break;

  case 11:
#line 227 "parser.y"
                     { (yyval.number) = VARTYPE_ARRAY_8S; }
#line 1832 "parser.tab.c"
    break;

  case 12:
#line 228 "parser.y"
                     { (yyval.number) = VARTYPE_ARRAY_16; }
#line 1838 "parser.tab.c"
    break;

  case 13:
#line 231 "parser.y"
                   { (yyval.number) = VARTYPE_GLOBAL; }
#line 1844 "parser.tab.c"
    break;

  case 14:
#line 232 "parser.y"
                      { (yyval.number) = VARTYPE_GLOBAL_8U; }
#line 1850 "parser.tab.c"
    break;

  case 15:
#line 233 "parser.y"
                      { (yyval.number) = VARTYPE_GLOBAL_8S; }
#line 1856 "parser.tab.c"
    break;

  case 16:
#line 236 "parser.y"
                   { (yyval.number) = (yyvsp[0].number); }
#line 1862 "parser.tab.c"
    break;

  case 17:
#line 237 "parser.y"
                       { (yyval.number) = (yyvsp[0].number); }
#line 1868 "parser.tab.c"
    break;

  case 18:
#line 238 "parser.y"
                       { (yyval.number) = -(yyvsp[0].number); }
#line 1874 "parser.tab.c"
    break;

  case 19:
#line 241 "parser.y"
                    {
		(yyval.ainit).len = 0;
		(yyval.ainit).data = NULL;
	}
#line 1883 "parser.tab.c"
    break;

  case 20:
#line 245 "parser.y"
               {
		(yyval.ainit).len = 1;
		(yyval.ainit).data = malloc(64 * sizeof(int));
		(yyval.ainit).data[0] = (yyvsp[0].number);
	}
#line 1893 "parser.tab.c"
    break;

  case 21:
#line 250 "parser.y"
                                   {
		(yyval.ainit) = (yyvsp[-2].ainit);
		(yyval.ainit).len++;
		(yyval.ainit).data = realloc((yyval.ainit).data, 64 * ((yyval.ainit).len/64 + 1) * sizeof(int));
		(yyval.ainit).data[(yyval.ainit).len-1] = (yyvsp[0].number);
	}
#line 1904 "parser.tab.c"
    break;

  case 22:
#line 258 "parser.y"
                    { (yyval.ainit).len = -1; (yyval.ainit).data = NULL; }
#line 1910 "parser.tab.c"
    break;

  case 23:
#line 259 "parser.y"
                                    { (yyval.ainit) = (yyvsp[-1].ainit); }
#line 1916 "parser.tab.c"
    break;

  case 24:
#line 260 "parser.y"
                          {
		int i;
		(yyval.ainit).len = strlen((yyvsp[0].string)) + 1;
		(yyval.ainit).data = malloc((yyval.ainit).len * sizeof(int));
		for (i = 0; i < (yyval.ainit).len; i++)
			(yyval.ainit).data[i] = (yyvsp[0].string)[i];
	}
#line 1928 "parser.tab.c"
    break;

  case 25:
#line 269 "parser.y"
                    { (yyval.vp) = NULL; }
#line 1934 "parser.tab.c"
    break;

  case 26:
#line 270 "parser.y"
                   {
		uint8_t *p = malloc(2);
		p[0] = (yyvsp[0].number) >> 8;
		p[1] = (yyvsp[0].number);
		(yyval.vp) = p;
	}
#line 1945 "parser.tab.c"
    break;

  case 27:
#line 278 "parser.y"
                    { (yyval.number) = -1; }
#line 1951 "parser.tab.c"
    break;

  case 28:
#line 279 "parser.y"
                              { (yyval.number) = (yyvsp[0].number); }
#line 1957 "parser.tab.c"
    break;

  case 29:
#line 282 "parser.y"
                                                            {
		(yyval.insn) = new_insn_data((yyvsp[-3].number) == VARTYPE_GLOBAL ? 2 : 1, NULL, NULL);
		(yyval.insn)->symbol = strdup((yyvsp[-2].string));
		(yyval.insn)->initdata = (yyvsp[-1].vp);
		add_nametab_global((yyvsp[-2].string), (yyvsp[-3].number), (yyval.insn));
		if ((yyvsp[-4].number) >= 0) {
			if ((yyvsp[-1].vp) != NULL) {
				fprintf(stderr, "Error in line %d: Extern declaration of `%s' with initializer!\n", yyget_lineno(), (yyvsp[-2].string));
				exit(1);
			}
			(yyval.insn)->addr = (yyvsp[-4].number);
			(yyval.insn) = NULL;
		}
	}
#line 1976 "parser.tab.c"
    break;

  case 30:
#line 296 "parser.y"
                                                                         {
		int i, wordsize = (yyvsp[-6].number) == VARTYPE_ARRAY_16 ? 2 : 1;
		(yyval.insn) = new_insn_data(wordsize * (yyvsp[-3].number), NULL, NULL);
		(yyval.insn)->symbol = strdup((yyvsp[-5].string));
		if ((yyvsp[-1].ainit).len >= 0) {
			(yyval.insn)->initdata = calloc((yyvsp[-3].number), wordsize);
			for (i=0; i < (yyvsp[-3].number) && i < (yyvsp[-1].ainit).len; i++) {
				if ((yyvsp[-6].number) == VARTYPE_ARRAY_16) {
					(yyval.insn)->initdata[2*i] = (yyvsp[-1].ainit).data[i] >> 8;
					(yyval.insn)->initdata[2*i + 1] = (yyvsp[-1].ainit).data[i];
				} else {
					(yyval.insn)->initdata[i] = (yyvsp[-1].ainit).data[i];
				}
			}
		}
		add_nametab_global((yyvsp[-5].string), (yyvsp[-6].number), (yyval.insn));
		if ((yyvsp[-7].number) >= 0) {
			if ((yyvsp[-1].ainit).len >= 0) {
				fprintf(stderr, "Error in line %d: Extern declaration of `%s' with initializer!\n", yyget_lineno(), (yyvsp[-5].string));
				exit(1);
			}
			(yyval.insn)->addr = (yyvsp[-7].number);
			(yyval.insn) = NULL;
		}
	}
#line 2006 "parser.tab.c"
    break;

  case 31:
#line 321 "parser.y"
                                                              {
		int i;
		if ((yyvsp[-1].ainit).len < 0) {
			fprintf(stderr, "Error in line %d: empty array '%s'\n", yyget_lineno(), (yyvsp[-4].string));
			exit(1);
		}
		(yyval.insn) = new_insn_data((yyvsp[-1].ainit).len, NULL, NULL);
		(yyval.insn)->symbol = strdup((yyvsp[-4].string));
		if ((yyvsp[-1].ainit).len >= 0) {
			(yyval.insn)->initdata = calloc(1, (yyvsp[-1].ainit).len);
			for (i = 0; i < (yyvsp[-1].ainit).len; i++) {
				(yyval.insn)->initdata[i] = (yyvsp[-1].ainit).data[i];
			}
		}
		add_nametab_global((yyvsp[-4].string), (yyvsp[-5].number), (yyval.insn));
		if ((yyvsp[-6].number) >= 0) {
			if ((yyvsp[-1].ainit).len >= 0) {
				fprintf(stderr, "Error in line %d: Extern declaration of `%s' with initializer!\n", yyget_lineno(), (yyvsp[-4].string));
				exit(1);
			}
			(yyval.insn)->addr = (yyvsp[-6].number);
			(yyval.insn) = NULL;
		}
	}
#line 2035 "parser.tab.c"
    break;

  case 32:
#line 347 "parser.y"
                                  { goto_ids=NULL; local_ids=NULL; (yyval.number) = (yyvsp[-1].number); }
#line 2041 "parser.tab.c"
    break;

  case 33:
#line 350 "parser.y"
                                                       {
		struct nametab_entry_s *e = add_nametab_global((yyvsp[-4].string), VARTYPE_FUNC, (yyval.insn));
		e->num_args = (yyvsp[-2].number);
		e->addr = new_insn(NULL, NULL);
		if ((yyvsp[-5].number) >= 0)
			e->addr->addr = (yyvsp[-5].number);
		else {
			e->is_forward_decl = true;
		}
		(yyval.insn) = NULL;
	}
#line 2057 "parser.tab.c"
    break;

  case 34:
#line 361 "parser.y"
                                                                                       {
		if ((yyvsp[-8].number) >= 0) {
			fprintf(stderr, "Error in line %d: Extern declaration of `%s' with implementation!\n", yyget_lineno(), (yyvsp[-7].string));
			exit(1);
		}
		int local_vars = (yyvsp[-2].number);
		struct evm_insn_s *alloc_local = NULL;
		while (local_vars > 0) {
			int this_num = local_vars > 8 ? 8 : local_vars;
			alloc_local = new_insn_op(0xf0 + (this_num-1), alloc_local, NULL);
			local_vars -= this_num;
		}
		(yyval.insn) = new_insn(alloc_local, (yyvsp[-1].insn));
		struct evm_insn_s *last_insn = (yyval.insn);
		while (last_insn->right != NULL)
			last_insn = last_insn->right;
		if (last_insn->opcode != 0x9b && last_insn->opcode != 0x9c)
			(yyval.insn) = new_insn_op(0x9c, (yyval.insn), NULL);
		struct nametab_entry_s *e = find_global_nametab_entry((yyvsp[-7].string));
		if (e) {
			if (!e->is_forward_decl) {
				fprintf(stderr, "Error in line %d: Re-declaration of identifier `%s'\n", yyget_lineno(), (yyvsp[-7].string));
				exit(1);
			}
			if (e->num_args != (yyvsp[-5].number)) {
				fprintf(stderr, "Error in line %d: Declaration and implementation of `%s'\nhave a different number of arguments!\n", yyget_lineno(), (yyvsp[-7].string));
				exit(1);
			}
			e->is_forward_decl = false;
			(yyval.insn) = new_insn(e->addr, (yyval.insn));
			e->addr = (yyval.insn);
		} else {
			e = add_nametab_global((yyvsp[-7].string), VARTYPE_FUNC, (yyval.insn));
			e->num_args = (yyvsp[-5].number);
		}
		(yyval.insn)->symbol = strdup((yyvsp[-7].string));
		for (e = goto_ids; e != NULL; e = e->next) {
			if (e->is_forward_decl) {
				fprintf(stderr, "Error in line %d: Goto label `%s' used but not declared!\n", yyget_lineno(), e->name);
				exit(1);
			}
		}
		assert(loopctx_stack == NULL);
	}
#line 2106 "parser.tab.c"
    break;

  case 35:
#line 407 "parser.y"
                    {
		(yyval.number) = 0;
	}
#line 2114 "parser.tab.c"
    break;

  case 36:
#line 410 "parser.y"
               {
		add_nametab_local((yyvsp[0].string), -1);
		(yyval.number) = 1;
	}
#line 2123 "parser.tab.c"
    break;

  case 37:
#line 414 "parser.y"
                                 {
		add_nametab_local((yyvsp[0].string), -((yyvsp[-2].number) + 1));
		(yyval.number) = (yyvsp[-2].number) + 1;
	}
#line 2132 "parser.tab.c"
    break;

  case 38:
#line 420 "parser.y"
                    {
		(yyval.number) = 0;
	}
#line 2140 "parser.tab.c"
    break;

  case 39:
#line 423 "parser.y"
                              {
		(yyval.number) = (yyvsp[-1].number);
	}
#line 2148 "parser.tab.c"
    break;

  case 40:
#line 428 "parser.y"
                         {
		add_nametab_local((yyvsp[0].string), 0);
		(yyval.number) = 1;
	}
#line 2157 "parser.tab.c"
    break;

  case 41:
#line 432 "parser.y"
                                     {
		add_nametab_local((yyvsp[0].string), (yyvsp[-2].number));
		(yyval.number) = (yyvsp[-2].number) + 1;
	}
#line 2166 "parser.tab.c"
    break;

  case 42:
#line 436 "parser.y"
                                               {
		add_nametab_local((yyvsp[0].string), (yyvsp[-3].number));
		(yyval.number) = (yyvsp[-3].number) + 1;
	}
#line 2175 "parser.tab.c"
    break;

  case 43:
#line 442 "parser.y"
                    {
		(yyval.insn) = NULL;
	}
#line 2183 "parser.tab.c"
    break;

  case 44:
#line 445 "parser.y"
                                {
		(yyval.insn) = new_insn((yyvsp[-1].insn), (yyvsp[0].insn));
	}
#line 2191 "parser.tab.c"
    break;

  case 45:
#line 450 "parser.y"
                    {
		(yyval.insn) = NULL;
	}
#line 2199 "parser.tab.c"
    break;

  case 46:
#line 453 "parser.y"
                                 {
		(yyval.insn) = new_insn((yyvsp[-1].insn), (yyvsp[0].insn));
	}
#line 2207 "parser.tab.c"
    break;

  case 47:
#line 458 "parser.y"
                             {
		struct nametab_entry_s *e = find_nametab_goto((yyvsp[-2].string));
		if (e && e->is_forward_decl)
			e->is_forward_decl = false;
		else
			e = add_nametab_goto((yyvsp[-2].string));
		(yyval.insn) = new_insn(e->addr, (yyvsp[0].insn));
	}
#line 2220 "parser.tab.c"
    break;

  case 48:
#line 466 "parser.y"
                            {
		struct nametab_entry_s *e = find_nametab_goto((yyvsp[-1].string));
		if (!e) {
			e = add_nametab_goto((yyvsp[-1].string));
			e->is_forward_decl = true;
		}
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 1, e->addr, NULL, NULL);
	}
#line 2233 "parser.tab.c"
    break;

  case 49:
#line 474 "parser.y"
                           {
		(yyval.insn) = (yyvsp[-1].insn);
	}
#line 2241 "parser.tab.c"
    break;

  case 50:
#line 477 "parser.y"
                               {
		(yyval.insn) = (yyvsp[-1].insn);
	}
#line 2249 "parser.tab.c"
    break;

  case 51:
#line 480 "parser.y"
                                            {
		struct evm_insn_s *end = new_insn(NULL, NULL);
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 7, end, (yyvsp[-2].insn), new_insn((yyvsp[0].insn), end));
	}
#line 2258 "parser.tab.c"
    break;

  case 52:
#line 484 "parser.y"
                                                               {
		struct evm_insn_s *end = new_insn(NULL, NULL);
		struct evm_insn_s *wrap_else = new_insn((yyvsp[0].insn), end);
		struct evm_insn_s *wrap_then = new_insn_op_reladdr(0xa0 + 1, end, (yyvsp[-2].insn), wrap_else);
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 7, wrap_else, (yyvsp[-4].insn), wrap_then);
	}
#line 2269 "parser.tab.c"
    break;

  case 53:
#line 490 "parser.y"
                                                          {
		struct evm_insn_s *body = new_insn((yyvsp[-5].loopctx)->body_insn, (yyvsp[-5].loopctx)->continue_insn);
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 5, body, new_insn(body, (yyvsp[-2].insn)), NULL);
		(yyval.insn) = new_insn((yyval.insn), (yyvsp[-5].loopctx)->break_insn);
	}
#line 2279 "parser.tab.c"
    break;

  case 54:
#line 495 "parser.y"
                                               {
		struct evm_insn_s *end = new_insn(NULL, NULL);
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 7, end, (yyvsp[-2].insn),
				new_insn_op_reladdr(0xa0 + 1, (yyvsp[-2].insn), (yyvsp[0].loopctx)->body_insn, end));
		(yyval.insn) = new_insn(new_insn((yyvsp[0].loopctx)->continue_insn, (yyval.insn)), (yyvsp[0].loopctx)->break_insn);
	}
#line 2290 "parser.tab.c"
    break;

  case 55:
#line 501 "parser.y"
                                                                                               {
		struct evm_insn_s *end = (yyvsp[0].loopctx)->break_insn;
		struct evm_insn_s *loop = new_insn_op_reladdr(0xa0 + 7, end, (yyvsp[-4].insn),
				new_insn_op_reladdr(0xa0 + 1, (yyvsp[-4].insn), new_insn((yyvsp[0].loopctx)->body_insn,
				new_insn((yyvsp[0].loopctx)->continue_insn, (yyvsp[-2].insn))), end));
		(yyval.insn) = new_insn((yyvsp[-6].insn), loop);
	}
#line 2302 "parser.tab.c"
    break;

  case 56:
#line 508 "parser.y"
                                                                                    {
		(yyval.insn) = new_insn((yyvsp[-5].insn), new_insn_op_reladdr(0xa0 + 1, (yyvsp[0].loopctx)->body_insn,
				new_insn((yyvsp[0].loopctx)->body_insn, new_insn((yyvsp[0].loopctx)->continue_insn, (yyvsp[-2].insn))), NULL));
		(yyval.insn) = new_insn((yyval.insn), (yyvsp[0].loopctx)->break_insn);
	}
#line 2312 "parser.tab.c"
    break;

  case 57:
#line 513 "parser.y"
                      {
		if (!loopctx_stack) {
			fprintf(stderr, "Fond break outside loop in line %d!\n", yyget_lineno());
			exit(1);
		}
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 1, loopctx_stack->break_insn, NULL, NULL);
	}
#line 2324 "parser.tab.c"
    break;

  case 58:
#line 520 "parser.y"
                         {
		if (!loopctx_stack) {
			fprintf(stderr, "Fond continue outside loop in line %d!\n", yyget_lineno());
			exit(1);
		}
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 1, loopctx_stack->continue_insn, NULL, NULL);
	}
#line 2336 "parser.tab.c"
    break;

  case 59:
#line 527 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x9b, (yyvsp[-1].insn), NULL);
	}
#line 2344 "parser.tab.c"
    break;

  case 60:
#line 530 "parser.y"
                       {
		(yyval.insn) = new_insn_op(0x9c, NULL, NULL);
	}
#line 2352 "parser.tab.c"
    break;

  case 61:
#line 534 "parser.y"
           {
		struct loopcontext_s *ctx = malloc(sizeof(struct loopcontext_s));
		ctx->break_insn = new_insn(NULL, NULL);
		ctx->continue_insn = new_insn(NULL, NULL);
		ctx->next = loopctx_stack;
		loopctx_stack = ctx;
	}
#line 2364 "parser.tab.c"
    break;

  case 62:
#line 540 "parser.y"
                    {
		(yyval.loopctx) = loopctx_stack;
		loopctx_stack = loopctx_stack->next;
		(yyval.loopctx)->body_insn = (yyvsp[0].insn);
		(yyval.loopctx)->next = NULL;
	}
#line 2375 "parser.tab.c"
    break;

  case 63:
#line 548 "parser.y"
                       { (yyval.number) = 0x80; }
#line 2381 "parser.tab.c"
    break;

  case 64:
#line 549 "parser.y"
                       { (yyval.number) = 0x81; }
#line 2387 "parser.tab.c"
    break;

  case 65:
#line 550 "parser.y"
                       { (yyval.number) = 0x82; }
#line 2393 "parser.tab.c"
    break;

  case 66:
#line 551 "parser.y"
                       { (yyval.number) = 0x83; }
#line 2399 "parser.tab.c"
    break;

  case 67:
#line 552 "parser.y"
                       { (yyval.number) = 0x84; }
#line 2405 "parser.tab.c"
    break;

  case 68:
#line 553 "parser.y"
                       { (yyval.number) = 0x85; }
#line 2411 "parser.tab.c"
    break;

  case 69:
#line 554 "parser.y"
                       { (yyval.number) = 0x86; }
#line 2417 "parser.tab.c"
    break;

  case 70:
#line 555 "parser.y"
                       { (yyval.number) = 0x87; }
#line 2423 "parser.tab.c"
    break;

  case 71:
#line 556 "parser.y"
                       { (yyval.number) = 0x88; }
#line 2429 "parser.tab.c"
    break;

  case 72:
#line 557 "parser.y"
                       { (yyval.number) = 0x89; }
#line 2435 "parser.tab.c"
    break;

  case 73:
#line 560 "parser.y"
                        {
		(yyval.insn) = new_insn_op(0x9d, (yyvsp[0].insn), NULL);
	}
#line 2443 "parser.tab.c"
    break;

  case 74:
#line 563 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[0].insn), true, true, false);
	}
#line 2451 "parser.tab.c"
    break;

  case 75:
#line 566 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[0].insn), true, false, false);
	}
#line 2459 "parser.tab.c"
    break;

  case 76:
#line 569 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[-1].insn), false, true, false);
	}
#line 2467 "parser.tab.c"
    break;

  case 77:
#line 572 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[-1].insn), false, false, false);
	}
#line 2475 "parser.tab.c"
    break;

  case 78:
#line 575 "parser.y"
                                          {
		(yyval.insn) = generate_combined_assign((yyvsp[-2].insn), new_insn_op((yyvsp[-1].number), (yyvsp[0].insn), NULL), false, false);
	}
#line 2483 "parser.tab.c"
    break;

  case 79:
#line 578 "parser.y"
                              {
		(yyval.insn) = new_insn((yyvsp[0].insn), (yyvsp[-2].insn));
	}
#line 2491 "parser.tab.c"
    break;

  case 80:
#line 583 "parser.y"
                    { (yyval.insn) = NULL; }
#line 2497 "parser.tab.c"
    break;

  case 81:
#line 584 "parser.y"
                       { (yyval.insn) = (yyvsp[0].insn); }
#line 2503 "parser.tab.c"
    break;

  case 82:
#line 588 "parser.y"
                   { (yyval.number) = VARTYPE_ARRAY_8U; }
#line 2509 "parser.tab.c"
    break;

  case 83:
#line 589 "parser.y"
                   { (yyval.number) = VARTYPE_ARRAY_8S; }
#line 2515 "parser.tab.c"
    break;

  case 84:
#line 590 "parser.y"
                   { (yyval.number) = VARTYPE_ARRAY_16; }
#line 2521 "parser.tab.c"
    break;

  case 85:
#line 593 "parser.y"
                    { (yyval.insn) = NULL; }
#line 2527 "parser.tab.c"
    break;

  case 86:
#line 594 "parser.y"
                       { (yyval.insn) = (yyvsp[0].insn); }
#line 2533 "parser.tab.c"
    break;

  case 87:
#line 597 "parser.y"
               {
		struct nametab_entry_s *e = find_nametab_entry((yyvsp[0].string));
		if (!e) {
			fprintf(stderr, "Unkown identifier `%s' in line %d!\n", (yyvsp[0].string), yyget_lineno());
			exit(1);
		}
		switch (e->type)
		{
		case VARTYPE_LOCAL:
			(yyval.insn) = new_insn_op(0x40 + (e->index & 0x3f), NULL, NULL);
			break;
		case VARTYPE_GLOBAL:
			(yyval.insn) = new_insn_op_absaddr(0xe8 + 1, e->addr, NULL, NULL);
			break;
		case VARTYPE_GLOBAL_8U:
			(yyval.insn) = new_insn_op_absaddr(0xc8 + 1, e->addr, NULL, NULL);
			break;
		case VARTYPE_GLOBAL_8S:
			(yyval.insn) = new_insn_op_absaddr(0xd8 + 1, e->addr, NULL, NULL);
			break;
		default:
			fprintf(stderr, "Identifier `%s' used incorrectly in line %d!\n", (yyvsp[0].string), yyget_lineno());
			exit(1);
		}
	}
#line 2563 "parser.tab.c"
    break;

  case 88:
#line 622 "parser.y"
                                  {
		struct nametab_entry_s *e = find_global_nametab_entry((yyvsp[-3].string));
		if (!e) {
			fprintf(stderr, "Unkown global identifier `%s' in line %d!\n", (yyvsp[-3].string), yyget_lineno());
			exit(1);
		}
		switch (e->type)
		{
		case VARTYPE_ARRAY_8U:
			(yyval.insn) = new_insn_op_absaddr(0xc8 + 4, e->addr, (yyvsp[-1].insn), NULL);
			break;
		case VARTYPE_ARRAY_8S:
			(yyval.insn) = new_insn_op_absaddr(0xd8 + 4, e->addr, (yyvsp[-1].insn), NULL);
			break;
		case VARTYPE_ARRAY_16:
			(yyval.insn) = new_insn_op_absaddr(0xe8 + 4, e->addr, (yyvsp[-1].insn), NULL);
			break;
		default:
			fprintf(stderr, "Identifier `%s' used incorrectly in line %d!\n", (yyvsp[-3].string), yyget_lineno());
			exit(1);
		}
	}
#line 2590 "parser.tab.c"
    break;

  case 89:
#line 644 "parser.y"
                                              {
		(yyval.insn) = (yyvsp[-2].insn);
		if ((yyvsp[-1].insn)) {
			(yyval.insn) = new_insn((yyval.insn), (yyvsp[-1].insn));
			if ((yyvsp[-4].number) == VARTYPE_ARRAY_16) {
				(yyval.insn) = new_insn_op(0x90 + 1, (yyval.insn), NULL);
				(yyval.insn) = new_insn_op(0x80 + 5, (yyval.insn), NULL);
			}
			(yyval.insn) = new_insn_op(0x80, (yyval.insn), NULL);
		}
		switch ((yyvsp[-4].number))
		{
		case VARTYPE_ARRAY_8U:
			(yyval.insn) = new_insn_op(0xc8 + 2, (yyval.insn), NULL);
			break;
		case VARTYPE_ARRAY_8S:
			(yyval.insn) = new_insn_op(0xd8 + 2, (yyval.insn), NULL);
			break;
		case VARTYPE_ARRAY_16:
			(yyval.insn) = new_insn_op(0xe8 + 2, (yyval.insn), NULL);
			break;
		default:
			fprintf(stderr, "Pointer used incorrectly in line %d!\n", yyget_lineno());
			exit(1);
		}
	}
#line 2621 "parser.tab.c"
    break;

  case 90:
#line 672 "parser.y"
                                      {
		struct nametab_entry_s *e = find_global_nametab_entry((yyvsp[-3].string));
		if (!e || e->type != VARTYPE_FUNC) {
			fprintf(stderr, "Unkown function `%s' in line %d!\n", (yyvsp[-3].string), yyget_lineno());
			exit(1);
		}
		if (e->num_args != (yyvsp[-1].fc)->num) {
			fprintf(stderr, "Call of function `%s' with incorrect number of arguments in line %d!\n", (yyvsp[-3].string), yyget_lineno());
			exit(1);
		}
		struct evm_insn_s *popargs = NULL;
		while ((yyvsp[-1].fc)->num > 0) {
			int this_num = (yyvsp[-1].fc)->num > 8 ? 8 : (yyvsp[-1].fc)->num;
			popargs = new_insn_op(0xf8 + (this_num-1), popargs, NULL);
			(yyvsp[-1].fc)->num -= this_num;
		}
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 3, e->addr, (yyvsp[-1].fc)->insn, popargs);
	}
#line 2644 "parser.tab.c"
    break;

  case 91:
#line 690 "parser.y"
                                            {
		(yyval.insn) = new_insn_op_val(0x9a, (yyvsp[-1].fc)->num, (yyvsp[-1].fc)->insn, NULL);
		(yyval.insn) = new_insn_op(0xb0 + (yyvsp[-3].number), (yyval.insn), NULL);
	}
#line 2653 "parser.tab.c"
    break;

  case 92:
#line 694 "parser.y"
                                                            {
		struct evm_insn_s *popargs = NULL;
		while ((yyvsp[-1].fc)->num > 0) {
			int this_num = (yyvsp[-1].fc)->num > 8 ? 8 : (yyvsp[-1].fc)->num;
			popargs = new_insn_op(0xf8 + (this_num-1), popargs, NULL);
			(yyvsp[-1].fc)->num -= this_num;
		}
		(yyval.insn) = new_insn_op(0x9e, new_insn((yyvsp[-1].fc)->insn, (yyvsp[-4].insn)), popargs);
	}
#line 2667 "parser.tab.c"
    break;

  case 93:
#line 705 "parser.y"
                        {
		(yyval.insn) = (yyvsp[0].insn);
	}
#line 2675 "parser.tab.c"
    break;

  case 94:
#line 708 "parser.y"
                           {
		(yyval.insn) = (yyvsp[-1].insn);
	}
#line 2683 "parser.tab.c"
    break;

  case 95:
#line 711 "parser.y"
                   {
		(yyval.insn) = new_insn_op_val(0x9a, (yyvsp[0].number), NULL, NULL);
	}
#line 2691 "parser.tab.c"
    break;

  case 96:
#line 714 "parser.y"
                      {
		struct evm_insn_s *end = new_insn(NULL, NULL);
		struct evm_insn_s *data = new_insn_data(strlen((yyvsp[0].string)) + 1, NULL, end);
		data->initdata = (uint8_t*)(yyvsp[0].string);
		(yyval.insn) = new_insn_op_absaddr(0x9a, data, NULL, new_insn_op_reladdr(0xa0 + 1, end, NULL, data));
	}
#line 2702 "parser.tab.c"
    break;

  case 97:
#line 720 "parser.y"
                 {
		(yyval.insn) = new_insn_op_val(0x9a, yyget_lineno(), NULL, NULL);
	}
#line 2710 "parser.tab.c"
    break;

  case 98:
#line 723 "parser.y"
                 {
		struct evm_insn_s *here = new_insn(NULL, NULL);
		(yyval.insn) = new_insn_op_absaddr(0x9a, here, here, NULL);
	}
#line 2719 "parser.tab.c"
    break;

  case 99:
#line 727 "parser.y"
                 {
		(yyval.insn) = new_insn_op(0xae, NULL, NULL);
	}
#line 2727 "parser.tab.c"
    break;

  case 100:
#line 730 "parser.y"
                  {
		(yyval.insn) = new_insn_op(0xaf, NULL, NULL);
	}
#line 2735 "parser.tab.c"
    break;

  case 101:
#line 733 "parser.y"
               {
		/* convert store op to load op */
		struct evm_insn_s *insn = (yyvsp[0].insn);
		if (insn->opcode >= 0xc0 && insn->opcode < 0xf0) {
			insn->opcode -= 0x08;
		}
		else if (insn->opcode >= 0x40 && insn->opcode < 0x80) {
			insn->opcode -= 0x40;
		}
		else
			abort();
		(yyval.insn) = insn;
	}
#line 2753 "parser.tab.c"
    break;

  case 102:
#line 746 "parser.y"
                   {
		struct nametab_entry_s *e = find_nametab_entry((yyvsp[0].string));
		if (!e) {
			fprintf(stderr, "Unkown global identifier `%s' in line %d!\n", (yyvsp[0].string), yyget_lineno());
			exit(1);
		}
		switch (e->type)
		{
		case VARTYPE_FUNC:
		case VARTYPE_GLOBAL:
		case VARTYPE_GLOBAL_8U:
		case VARTYPE_GLOBAL_8S:
		case VARTYPE_ARRAY_8U:
		case VARTYPE_ARRAY_8S:
		case VARTYPE_ARRAY_16:
			(yyval.insn) = new_insn_op_absaddr(0x9a, e->addr, NULL, NULL);
			break;
		case VARTYPE_LOCAL:
			(yyval.insn) = new_insn_op(0xaf, NULL, NULL);
			(yyval.insn) = new_insn_op_val(0x9a, 2 * (e->index + 1), (yyval.insn), NULL);
			(yyval.insn) = new_insn_op(0x80 + 1, (yyval.insn), NULL);
			break;
		default:
			fprintf(stderr, "Identifier `%s' used incorrectly in line %d!\n", (yyvsp[0].string), yyget_lineno());
			exit(1);
		}
	}
#line 2785 "parser.tab.c"
    break;

  case 103:
#line 773 "parser.y"
                                 {
		(yyval.insn) = (yyvsp[0].insn);
	}
#line 2793 "parser.tab.c"
    break;

  case 104:
#line 776 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 0, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2801 "parser.tab.c"
    break;

  case 105:
#line 779 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 1, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2809 "parser.tab.c"
    break;

  case 106:
#line 782 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 2, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2817 "parser.tab.c"
    break;

  case 107:
#line 785 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 3, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2825 "parser.tab.c"
    break;

  case 108:
#line 788 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 4, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2833 "parser.tab.c"
    break;

  case 109:
#line 791 "parser.y"
                                      {
		(yyval.insn) = new_insn_op(0x80 + 5, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2841 "parser.tab.c"
    break;

  case 110:
#line 794 "parser.y"
                                      {
		(yyval.insn) = new_insn_op(0x80 + 6, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2849 "parser.tab.c"
    break;

  case 111:
#line 797 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 7, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2857 "parser.tab.c"
    break;

  case 112:
#line 800 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 8, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2865 "parser.tab.c"
    break;

  case 113:
#line 803 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0x80 + 9, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2873 "parser.tab.c"
    break;

  case 114:
#line 806 "parser.y"
                                       {
		(yyval.insn) = new_insn_op(0x80 + 10, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2881 "parser.tab.c"
    break;

  case 115:
#line 809 "parser.y"
                                      {
		(yyval.insn) = new_insn_op(0x80 + 11, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2889 "parser.tab.c"
    break;

  case 116:
#line 812 "parser.y"
                                 {
		if ((yyvsp[0].insn)->opcode == 0x9a) {
			(yyvsp[0].insn)->arg_val = ~(yyvsp[0].insn)->arg_val;
			(yyval.insn) = (yyvsp[0].insn);
		} else
			(yyval.insn) = new_insn_op(0x80 + 12, (yyvsp[0].insn), NULL);
	}
#line 2901 "parser.tab.c"
    break;

  case 117:
#line 819 "parser.y"
                                 {
		if ((yyvsp[0].insn)->opcode == 0x9a) {
			(yyvsp[0].insn)->arg_val = -(yyvsp[0].insn)->arg_val;
			(yyval.insn) = (yyvsp[0].insn);
		} else
			(yyval.insn) = new_insn_op(0x80 + 13, (yyvsp[0].insn), NULL);
	}
#line 2913 "parser.tab.c"
    break;

  case 118:
#line 826 "parser.y"
                                 {
		if ((yyvsp[0].insn)->opcode == 0x9a) {
			(yyvsp[0].insn)->arg_val = !(yyvsp[0].insn)->arg_val;
			(yyval.insn) = (yyvsp[0].insn);
		} else
			(yyval.insn) = new_insn_op(0x80 + 14, (yyvsp[0].insn), NULL);
	}
#line 2925 "parser.tab.c"
    break;

  case 119:
#line 833 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0xa8 + 0, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2933 "parser.tab.c"
    break;

  case 120:
#line 836 "parser.y"
                                     {
		(yyval.insn) = new_insn_op(0xa8 + 1, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2941 "parser.tab.c"
    break;

  case 121:
#line 839 "parser.y"
                                     {
		(yyval.insn) = new_insn_op(0xa8 + 2, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2949 "parser.tab.c"
    break;

  case 122:
#line 842 "parser.y"
                                     {
		(yyval.insn) = new_insn_op(0xa8 + 3, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2957 "parser.tab.c"
    break;

  case 123:
#line 845 "parser.y"
                                     {
		(yyval.insn) = new_insn_op(0xa8 + 4, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2965 "parser.tab.c"
    break;

  case 124:
#line 848 "parser.y"
                                  {
		(yyval.insn) = new_insn_op(0xa8 + 5, new_insn((yyvsp[-2].insn), (yyvsp[0].insn)), NULL);
	}
#line 2973 "parser.tab.c"
    break;

  case 125:
#line 851 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[0].insn), true, true, true);
	}
#line 2981 "parser.tab.c"
    break;

  case 126:
#line 854 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[0].insn), true, false, true);
	}
#line 2989 "parser.tab.c"
    break;

  case 127:
#line 857 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[-1].insn), false, true, true);
	}
#line 2997 "parser.tab.c"
    break;

  case 128:
#line 860 "parser.y"
                       {
		(yyval.insn) = generate_pre_post_inc_dec((yyvsp[-1].insn), false, false, true);
	}
#line 3005 "parser.tab.c"
    break;

  case 129:
#line 863 "parser.y"
                                                 {
		struct evm_insn_s *end = new_insn(NULL, NULL);
		struct evm_insn_s *wrap_false = new_insn((yyvsp[0].insn), end);
		struct evm_insn_s *wrap_true = new_insn_op_reladdr(0xa0 + 1, end, (yyvsp[-2].insn), wrap_false);
		(yyval.insn) = new_insn_op_reladdr(0xa0 + 7, wrap_false, (yyvsp[-4].insn), wrap_true);
	}
#line 3016 "parser.tab.c"
    break;

  case 130:
#line 871 "parser.y"
                    {
		(yyval.fc) = calloc(1, sizeof(struct func_call_args_desc_s));
	}
#line 3024 "parser.tab.c"
    break;

  case 131:
#line 874 "parser.y"
                   {
		(yyval.fc) = calloc(1, sizeof(struct func_call_args_desc_s));
		(yyval.fc)->insn = (yyvsp[0].insn);
		(yyval.fc)->num = 1;
	}
#line 3034 "parser.tab.c"
    break;

  case 132:
#line 879 "parser.y"
                                      {
		(yyval.fc) = (yyvsp[-2].fc);
		(yyval.fc)->insn = new_insn((yyvsp[0].insn), (yyval.fc)->insn);
		(yyval.fc)->num++;
	}
#line 3044 "parser.tab.c"
    break;


#line 3048 "parser.tab.c"

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
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
#line 885 "parser.y"


static struct evm_insn_s *generate_pre_post_inc_dec(struct evm_insn_s *lv,
		bool is_pre, bool is_inc, bool is_expr)
{
	struct evm_insn_s *action = new_insn_op(0x90 + 1, NULL, NULL);
	action = new_insn_op(is_inc ? 0x80 : 0x81 , action, NULL);

	return generate_combined_assign(lv, action, is_pre, is_expr);
}

static struct evm_insn_s *generate_combined_assign(struct evm_insn_s *lv,
		struct evm_insn_s *action, bool is_pre, bool is_expr)
{
	struct evm_insn_s *insn = lv;
	bool lvalue_prep = false;
	uint8_t store_opcode;

	/* convert lvalue to load op and save store opcode */
	store_opcode = insn->opcode;
	if (insn->opcode >= 0xc0 && insn->opcode < 0xf0) {
		insn->opcode -= 0x08;
	}
	else if (insn->opcode >= 0x40 && insn->opcode < 0x80) {
		insn->opcode -= 0x40;
	}
	else
		abort();

	if (insn->left) {
		/* inject dup */
		insn->left = new_insn_op(0xc5, insn->left, NULL);
		lvalue_prep = true;
	}

	/* copy old value for postfix */
	if (is_expr && !is_pre)
		insn = new_insn_op(0xc5 + (lvalue_prep ? 1 : 0)*8, insn, NULL);

	/* perform operation */
	insn = new_insn(insn, action);

	/* copy new value for prefix */
	if (is_expr && is_pre)
		insn = new_insn_op(0xc5 + (lvalue_prep ? 1 : 0)*8, insn, NULL);

	/* shuffle address to top if needed */
	if (lvalue_prep)
		insn = new_insn_op(0xc6, insn, NULL);

	/* perform store operation */
	insn = new_insn_op(store_opcode, insn, NULL);
	insn->has_arg_data = lv->has_arg_data;
	insn->arg_addr = lv->arg_addr;

	return insn;
}

static struct nametab_entry_s *add_nametab_goto(char *name)
{
	struct nametab_entry_s *e = find_nametab_goto(name);
	if (e) {
		fprintf(stderr, "Error in line %d: Re-declaration of goto label `%s'\n", yyget_lineno(), name);
		exit(1);
	}
	e = calloc(1, sizeof(struct nametab_entry_s));
	e->name = name;
	e->addr = new_insn(NULL, NULL);
	e->next = goto_ids;
	goto_ids = e;
	return e;
}

static struct nametab_entry_s *add_nametab_local(char *name, int index)
{
	struct nametab_entry_s *e = find_nametab_entry(name);
	if (e) {
		fprintf(stderr, "Error in line %d: Re-declaration of identifier `%s'\n", yyget_lineno(), name);
		exit(1);
	}
	e = calloc(1, sizeof(struct nametab_entry_s));
	e->name = name;
	e->type = VARTYPE_LOCAL;
	e->index = index;
	e->next = local_ids;
	local_ids = e;
	return e;
}

static struct nametab_entry_s *add_nametab_global(char *name, int type, struct evm_insn_s *addr)
{
	struct nametab_entry_s *e = find_global_nametab_entry(name);
	if (e) {
		fprintf(stderr, "Error in line %d: Re-declaration of identifier `%s'\n", yyget_lineno(), name);
		exit(1);
	}
	e = calloc(1, sizeof(struct nametab_entry_s));
	e->name = name;
	e->type = type;
	e->addr = addr;
	e->next = global_ids;
	global_ids = e;
	return e;
}

struct nametab_entry_s *find_nametab_backend(struct nametab_entry_s *tab, char *name)
{
	for (; tab != NULL; tab = tab->next) {
		if (!strcmp(tab->name, name))
			return tab;
	}
	return NULL;
}

struct nametab_entry_s *find_nametab_goto(char *name)
{
	return find_nametab_backend(goto_ids, name);
}

struct nametab_entry_s *find_nametab_entry(char *name)
{
	struct nametab_entry_s *tab = find_nametab_backend(local_ids, name);
	if (!tab)
		tab = find_nametab_backend(global_ids, name);
	return tab;
}

struct nametab_entry_s *find_global_nametab_entry(char *name)
{
	return find_nametab_backend(global_ids, name);
}

