#if !defined(_ZZ_q_prstabsP_DEFINED)
#define _ZZ_q_prstabsP_DEFINED
/*===========================================================================*/

#include "cm_defs.h"
#include "tokdefs.h"
#include "rwdefs.h"

#include "cm_types.h"
#include "prsdefs.h"
#include "prstypes.h"

static operation_type give_relation_op_table[TK_RELATION_9-TK_RELATION_0+1] =
   {  /* see ordering of TK_RELATION_0..TK_RELATION_9 */
      OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE 
   };

static type_flagword_type
   give_literal_type_table[TK_LITERAL_9-TK_LITERAL_0+1] =
   {  /* see ordering of TK_LITERAL_0..TK_LITERAL_9 */
      BOOL_TYPE, CHAR_TYPE, INT_TYPE, REAL_TYPE
   };

#define TAB_QQQ() \
      1, REAL_TYPE, REAL_TYPE,  OP_SQRT,   OP_NIX,   "SQRT", \
      1, REAL_TYPE, REAL_TYPE,  OP_EXP,    OP_NIX,   "EXP", \
      1, REAL_TYPE, REAL_TYPE,  OP_LN,     OP_NIX,   "LN", \
      1, REAL_TYPE, REAL_TYPE,  OP_ABS,    OP_NIX,   "ABS", \
      1, REAL_TYPE, REAL_TYPE,  OP_SIN,    OP_NIX,   "SIN,", \
      1, REAL_TYPE, REAL_TYPE,  OP_COS,    OP_NIX,   "COS,", \
      1, REAL_TYPE, REAL_TYPE,  OP_TAN,    OP_NIX,   "TAN", \
      1, REAL_TYPE, REAL_TYPE,  OP_ARCSIN, OP_NIX,   "ARCSIN", \
      1, REAL_TYPE, REAL_TYPE,  OP_ARCCOS, OP_NIX,   "ARCCOS", \
      1, REAL_TYPE, REAL_TYPE,  OP_ARCTAN, OP_NIX,   "ARCTAN", \
      1, INT_TYPE,  INT_TYPE,   OP_FACT,   OP_NIX,   "FACT", \
      1, REAL_TYPE, REAL_TYPE,  OP_GAM,    OP_NIX,   "GAM", \
      2, INT_TYPE,  INT_TYPE,   OP_COMB,   OP_NIX,   "COMB", \
      2, INT_TYPE,  INT_TYPE,   OP_PERM,   OP_NIX,   "PERM"

#define TAB_QQQ_XXX() \
      1,       ARRAY_TYPE,      INT_TYPE,  OP_SIZE,  OP_SIZE, "SIZE", \
  VARIAB_LENG, BOOL_ARRAY_TYPE, INT_TYPE,  OP_COUNT, OP_BtoI, "COUNT", \
  VARIAB_LENG, ARRAY_TYPE,      EMPTY_TYPE,OP_IMIN,  OP_RMIN, MIN_WORD, \
  VARIAB_LENG, ARRAY_TYPE,      EMPTY_TYPE,OP_IMAX,  OP_RMAX, MAX_WORD, \
  VARIAB_LENG, ARRAY_TYPE,      EMPTY_TYPE,OP_ISUM,  OP_RSUM, "SUM", \
  VARIAB_LENG, BOOL_ARRAY_TYPE, BOOL_TYPE, OP_ANY,   OP_NIX,  "ANY", \
  VARIAB_LENG, BOOL_ARRAY_TYPE, BOOL_TYPE, OP_ALL,   OP_NIX,  "ALL"

static built_in_parm_info_type
  usual_builtin_op_info_table[RW_BUILTIN_9-RW_BUILTIN_0+1] =
   {  /* see ordering of RW_BUILTIN_0..RW_BUILTIN_9 */
      TAB_QQQ(), TAB_QQQ_XXX()
   };

static char builtin_name_table[OP_BUILTIN_COUNT][8] =
   {  /* see ordering of OP_BUILTIN_0..OP_BUILTIN_9 */
     /*70*/ "SQRT",
            "EXP",
            "LN",
            "ABS",
            "SIN",
            "COS",
            "TAN",
            "ARCSIN",
            "ARCCOS",
            "ARCTAN",
            "FACT",
            "GAM",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "SIZE",
     /*80*/ "COUNT", /* COUNT1 */
            MIN_WORD,   /* MIN1 (I) */
            MIN_WORD,   /* MIN1 (R) */
            MAX_WORD,   /* MAX1 (I) */
            MAX_WORD,   /* MAX1 (R) */
            "SUM",   /* SUM1 (I) */
            "SUM",   /* SUM1 (R) */
            "ANY",   /* ANY1 (R) */
            "ALL",   /* ALL1 (R) */
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
     /*90*/ "COUNT",
            MIN_WORD,   /* (I) */
            MIN_WORD,   /* (R) */
            MAX_WORD,   /* (I) */
            MAX_WORD,   /* (R) */
            "SUM",   /* (I) */
            "SUM",   /* (R) */
            "ANY",
            "ALL",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
     /*a0*/ "COMB",
            "PERM",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
     /*b0*/ "COUNT", /* ROWCOUNT */
            "COUNT", /* COLCOUNT */
            MIN_WORD,  /* ROWMIN (I) */
            MIN_WORD,  /* COLMIN (I) */
            MIN_WORD,  /* ROWMIN (R) */
            MIN_WORD,  /* COLMIN (R) */
            MAX_WORD,  /* ROWMAX (I) */
            MAX_WORD,  /* COLMAX (I) */
            MAX_WORD,  /* ROWMAX (R) */
            MAX_WORD,  /* COLMAX (R) */
            "SUM",  /* ROWSUM (I) */
            "SUM",  /* COLSUM (I) */
            "SUM",  /* ROWSUM (R) */
            "SUM",  /* COLSUM (R) */
            "ANY",   /* ROWANY */
            "ANY",   /* COLANY */
     /*c0*/ "ALL",   /* ROWALL */
            "ALL",   /* COLALL */
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
            "--FUT--",
     /*d0*/ "COUNT", /* LISCOUNT */
            MIN_WORD,   /* LISMIN (I) */
            MIN_WORD,   /* LISMIN (R) */
            MAX_WORD,   /* LISMAX (I) */
            MAX_WORD,   /* LISMAX (R) */
            "SUM",   /* LISSUM (I) */
            "SUM",   /* LISSUM (R) */
            "ANY",   /* LISANY */
            "ALL"    /* LISALL */
   };

#define builtin_return_type(t) (t)

/*===========================================================================*/
#endif
