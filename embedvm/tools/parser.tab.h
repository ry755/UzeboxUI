/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 124 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
