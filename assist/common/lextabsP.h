#if !defined(_ZZ_q_lextabsP_DEFINED)
#define _ZZ_q_lextabsP_DEFINED
/*===========================================================================*/

#include "lib_def.h"
#include "cm_defs.h"
#include "tokdefs.h"
#include "rwdefs.h"

#include "cm_types.h"

static char numtab_bool_zero[] = NUMBER_TABLE_BOOL_ZERO_IDENT;
static char numtab_int_zero[]  = NUMBER_TABLE_INT_ZERO_IDENT;
static char numtab_real_zero[] = NUMBER_TABLE_REAL_ZERO_IDENT;

static struct qqxxqqyy
   { char *idp; token ans; } stnlup[3] =
       {
           { &numtab_bool_zero[0], TK_BOOL },
           { &numtab_int_zero[0],  TK_INT },
           { &numtab_real_zero[0], TK_REAL }
       };
#define STNLUP_BOOL 0
#define STNLUP_INT  1
#define STNLUP_REAL 2

static rw_operator_lookup_type rw_operator_lookup_table[] =
   {
      RW_NOT,  TK_NOT,
      RW_AND,  TK_AND,
      RW_OR,   TK_OR,
      RW_XOR,  TK_XOR,
      RW_QUO,  TK_QUO,
      RW_MOD,  TK_MOD,
      RW_CYC,  TK_CYC,
      NOT_RESERVED_WORD,  TK_NULL   /* must be last */
   };

static token_lookup_type token_lookup_table[] = 
   {
      /*--------------------  two character tokens must come first */
      "==",  TK_BOOL_EQ,
#ifdef QUESTIONABLE
      "~~",  TK_BOOL_NE,
#endif
      "<>",  TK_NE,
      "<=",  TK_LE,
      ">=",  TK_GE,
      "**",  TK_POW,
      "++",  TK_INC_1,
      "--",  TK_DEC_1,
      "..",  TK_SUBRNGE,
      /*--------------------  single character tokens must come last */
      "[",   TK_LB,
      "]",   TK_RB,
      "(",   TK_LP,
      ")",   TK_RP,
      "^",   TK_CONCAT,
      "=",   TK_EQ,
      "+",   TK_ADD,
      "-",   TK_SUB,
      "*",   TK_MUL,
      "/",   TK_DVD,
      "&",   TK_AND,
      "|",   TK_OR,
#ifdef QUESTIONABLE
      "~",   TK_NOT,
#endif
      "<",   TK_LT,
      ">",   TK_GT,
      ",",   TK_COMMA,
      ";",   TK_SEMI,
      ":",   TK_COLON,
      "@",   TK_AT,
      "\0",  TK_NULL    /* MUST BE LAST */
   };

/*===========================================================================*/
#endif
