#if !defined(_ZZ_q_alextabs_DEFINED)
#define _ZZ_q_alextabs_DEFINED
/*===========================================================================*/
#include "astrw.h"
#include "cm_types.h"
/*===========================================================================*/
reserved_word_lookup_type reserved_word_lookup_table[] =
   {  /* see ordering of statement names in "astrw.h" */
      /*----------------------------------- statement names first */
      "DEBUG$",RW_DEBUG,
      "C_OPTION",RW_CLOPT,
      "INPUT",RW_INPUT,
#if defined(ASSIST)
      "SPACE",RW_SPACE,
      "FUNCTION",RW_FUNCTION,
      "IMPLICIT",RW_IMPLICIT,
      "INTRINSIC",RW_INTRINSIC,
      "VARIABLE",RW_VARIABLE,
      "START",RW_START,
      "DEATHIF",RW_DEATHIF,
      "PRUNEIF",RW_PRUNEIF,
      "ASSERT",RW_ASSERT,
      "TRANTO",RW_TRANTO,
      "IF",RW_IF,
      "FOR",RW_FOR,
      "ONEDEATH",RW_ONEDEATH,
      "COMMENT",RW_COMMENT,
      "MODSTATS$",RW_MODSTATS,
      "ECHO",RW_ECHO,
      "TRIM",RW_TRIM,
#endif
#if defined(SURE) || defined(PAWS)
      "SHOW",RW_SHOW,
      "READ",RW_READ,
      "READ0",RW_READ0,
      "SAVE",RW_SAVE,
      "RUN",RW_RUN,
      "EXIT",RW_EXIT,
      "GET",RW_GET,
      "HELP",RW_HELP,
      "SCAN",RW_SCAN,
      "ZOOM",RW_ZOOM,
      "CALC",RW_CALC,
      "DISPLAY",RW_DISP,
      "LEE",RW_LEE,
      "CLEAR",RW_CLR,
      "PLOT",RW_PLOT,
      "PLOTINIT",RW_PLTI,
      "PLOT3D",RW_PLT3D,
      "ORPROB",RW_ORPROB,
      "INITIAL_STATE",RW_INIT,
      "PRUNESTATE",RW_PRUNE,
#endif
      /*----------------------------------- alternate spellings next */
#if defined(ASSIST)
      "PRUNIF",RW_PRUNEIF,
      "MOD_STATISTICS$",RW_MODSTATS,
#endif
#if defined(SURE) || defined(PAWS)
      "Q",RW_EXIT,
      "QUIT",RW_EXIT,
      "DISP",RW_DISP,
#endif
      /*----------------------------------- names of arithmetic keywords */
      "AND",RW_AND,
      "OR",RW_OR,
      "XOR",RW_XOR,
      "NOT",RW_NOT,
      "DIV",RW_QUO,
      "MOD",RW_MOD,
      "CYC",RW_CYC,
      /*----------------------------------- names of built-in functions */
      "SQRT",RW_SQRT,
      "EXP",RW_EXP,
      "LN",RW_LN,
      "ABS",RW_ABS,
      "SIN",RW_SIN,
      "COS",RW_COS,
      "TAN",RW_TAN,
      "ARCSIN",RW_ARCSIN,
      "ARCCOS",RW_ARCCOS,
      "ARCTAN",RW_ARCTAN,
      "FACT",RW_FACT,
      "COMB",RW_COMB,
      "PERM",RW_PERM,
      "GAM",RW_GAM,
#if defined(ASSIST)
      "SIZE",RW_SIZE,
      MIN_WORD,RW_MIN,
      MAX_WORD,RW_MAX,
      "SUM",RW_SUM,
      "COUNT",RW_COUNT,
#ifdef FUTURE
      "ROWSUM",RW_ROWSUM,
      "COLSUM",RW_COLSUM,
      "ROWCOUNT",RW_ROWCOUNT,
      "COLCOUNT",RW_COLCOUNT,
#endif
      "ANY",RW_ANY,
      "ALL",RW_ALL,
#ifdef FUTURE
      "ANYPOS",RW_ANYPOS,
      "ALLPOS",RW_ALLPOS,
      "OTHER$",RW_OTHER,
#endif
#endif
      /*----------------------------------- names of sensitive keywords */
      "FAST",RW_FAST,
#if defined(ASSIST)
      "BY",RW_BY,
      "THEN",RW_THEN,
      "ELSE",RW_ELSE,
#ifdef FUTURE
      "ELSIF",RW_ELSIF,
#endif
      "ENDIF",RW_ENDIF,
      "ENDFOR",RW_ENDFOR,
      "WITH",RW_WITH,
      "IN",RW_IN,
      "OF",RW_OF,
#endif
#if defined(SURE) || defined(PAWS)
      "END",RW_END,
      "TO",RW_TO,
      "INTO",RW_INTO,
      "UPTO",RW_UPTO,
      "DOWNTO",RW_DOWNTO,
#endif
/*
    ?????????????   certain keywords can be used as variables because
                    they occur only when neither a variable, nor constant,
                    nor expression can occur.   The problem is that the
                    lexical scanner does not know whether to turn these
                    words into reserved words or to store them into the
                    identifier table.   These keywords are therefore still
                    sensitive keywords.
*/
#ifdef FUTURE
      "\n", NOT_RESERVED_WORD,   /* must mark end of sensitive */
#endif
      /*----------------------------------- names of non-sensitive keywords */
#if defined(ASSIST)
      "ARRAY",RW_ARRAY,
      "ON",RW_ON,
      "OFF",RW_OFF,
      "FULL",RW_FULL,
#ifdef FUTURE
      "INTEGER",RW_INTEGER,
      "REAL",RW_REAL,
#endif
      "BOOLEAN",RW_BOOLEAN,
#endif
      "ARGV$",RW_CLINE_ARG,
      "NONE$",RW_DBG_NONE,
      "LEX$",RW_DBG_LEX,
      "PARSE$",RW_DBG_PRS,
      "PRS$",RW_DBG_PRS,
      "TRANTO$",RW_DBG_TRANTO,
      "XEQ$",RW_DBG_XEQ,
      "EXPR$",RW_DBG_XEQ,
      "ROU$",RW_DBG_ROU,
      "DATA_STRUCT$",RW_DBG_DATAST,
      "DST$",RW_DBG_DATAST,
      "EXPAND$",RW_DBG_EXPAND,
      "CODE$",RW_DBG_CODE,
      "ALL$",RW_DBG_ALL,
      "ABORT$",RW_DBG_ABORT,
      "\0", NOT_RESERVED_WORD   /* must be last */
    };
/*===========================================================================*/
#endif
