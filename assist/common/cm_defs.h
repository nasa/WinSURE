#if !defined(_ZZ_q_cm_defs_DEFINED)
#define _ZZ_q_cm_defs_DEFINED
/*===========================================================================*/

#define MIN_WORD "MIN"
#define MAX_WORD "MAX"

#include "commonio.h"

#define NEGFIX_TAB_DELTA 4
#define DFLT_TAB_DELTA 8
#define MAX_TAB_DELTA 16
#define MAXSSV ((ssvar_value_type) (MAXSHORT))

#define ZERO 0.000000000000
#define ONE  1.000000000000

#define SIMPLE_IDENTIFIER ((Subscript) -1)

#define NUMBER_TABLE_PREFIX_CHAR '#'
#define NUMBER_TABLE_INT_ZERO_IDENT "#0"
#define NUMBER_TABLE_REAL_ZERO_IDENT "#0.0"
#define NUMBER_TABLE_BOOL_ZERO_IDENT "#FALSE"

#define EXPECTED_TOKENS_PER_CALL_PARM 8
#define EVAL_STACK_DIM 512 /* 512*9 = 4+1/2K allows 512 levels of precedence */

#define STRLP  "("
#define STRRP  ")"
#define STRLB  "["
#define STRRB  "]"

#define COMMA ','
#define SEMICOLON ';'
#define COLON ':'
#define OPENPAREN '('
#define CLOSEPAREN ')'
#define OPENCURLY '{'
#define CLOSECURLY '}'
#define OPENBRACKET '['
#define CLOSEBRACKET ']'
#define OPENBRACE '{'
#define CLOSEBRACE '}'
#define APOSTROPHE '\''
#define QUOTATION_MARK '"'
#define SLASH '/'
#define ASTERISK '*'
#define NEWLINE '\n'
#define TAB '\t'


              /**************************************/
              /**************************************/
              /****                              ****/
              /****   COMPUTATIONAL DATA TYPES   ****/
              /****                              ****/
              /**************************************/
              /**************************************/

/*
   Certain changes here may imply corresponding changes to tables
   in the specific code for TOTAL/ASSIST/SURE/STEM/PAWS
*/
#define COMPUTATIONAL_TYPE_COUNT 8
#define COMPUTATIONAL_TYPE_MASK ((type_flagword_type) 0x07)
#define ERROR_TYPE              ((type_flagword_type) 0x00)
#define CHAR_TYPE               ((type_flagword_type) 0x01)
#define BOOL_TYPE               ((type_flagword_type) 0x02)
#define INT_TYPE                ((type_flagword_type) 0x03)
#define REAL_TYPE               ((type_flagword_type) 0x04)
#define SOFF_TYPE               ((type_flagword_type) 0x05)
#define Q_ADDR_TYPE             ((type_flagword_type) 0x07)

#define BtoI_CVT(ttt) ttt |= INT_TYPE

            /*******************************************/
            /*******************************************/
            /****                                   ****/
            /****   SPECIAL DATA TYPE ATTRITBUTES   ****/
            /****                                   ****/
            /*******************************************/
            /*******************************************/

#define SPECIAL_TYPE_MASK         ((type_flagword_type) 0xf8)
#define ANY_SPECIAL_VAR_TYPE_MASK ((type_flagword_type) 0x78)

#define EXPR_VARIABLE_TYPE        ((type_flagword_type) 0x08)  /* temporary */
#define FOR_LOOP_INDEX_TYPE       ((type_flagword_type) 0x10)  /* for index */
#define SSVAR_TYPE                ((type_flagword_type) 0x20)  /* ssv */
#define FUNCTION_TYPE             ((type_flagword_type) 0x40)  /* function */
#define ARRAY_TYPE                ((type_flagword_type) 0x80)  /* [] */

                 /*********************************/
                 /*********************************/
                 /****                         ****/
                 /****   COMBINED DATA TYPES   ****/
                 /****                         ****/
                 /*********************************/
                 /*********************************/

#define VARIABLE_TYPE     EXPR_VARIABLE_TYPE
#define IMPLICIT_TYPE     ((type_flagword_type) 0x48) /* temporary function */
#define IMPLICIT_BIT      ((type_flagword_type) 0x08) /* temporary function */
#define FUNC_IMPL_MASK    ((type_flagword_type) 0x48)
#define BOOL_CONST_TYPE   BOOL_TYPE
#define INT_CONST_TYPE    INT_TYPE
#define REAL_CONST_TYPE   REAL_TYPE

#define CHAR_ARRAY_TYPE ((type_flagword_type) 0x81)
#define BOOL_ARRAY_TYPE ((type_flagword_type) 0x82)
#define INT_ARRAY_TYPE  ((type_flagword_type) 0x83)
#define REAL_ARRAY_TYPE ((type_flagword_type) 0x84)

#if !defined(WITH_TAE_DCLS)
#define UNKNOWN_TYPE ERROR_TYPE
#endif
#define EMPTY_TYPE ERROR_TYPE
#define BOOL_SSVAR_TYPE ((type_flagword_type) 0x22) /* BOOL_TYPE|SSVAR_TYPE */
#define INT_SSVAR_TYPE  ((type_flagword_type) 0x23) /* INT_TYPE |SSVAR_TYPE */
#define X_COMP_TYPE_MASK ((type_flagword_type) 0x87)
                            /* ((COMPUTATIONAL_TYPE_MASK)|(ARRAY_TYPE)) */
#define NO_VALUE_TYPE_MASK ((type_flagword_type) 0x40)

       /*****************************************************/
       /*****************************************************/
       /****                                             ****/
       /****   IDENTIFIER AND SOURCE LINE BUFFER SIZES   ****/
       /****                                             ****/
       /*****************************************************/
       /*****************************************************/

#define IDENT_NCH_RESERVED 3
#define IDENT_MAXNCH 28                    /* length of user identifier */
#define IDENT_MAXNCH_I (IDENT_MAXNCH+3)    /* length of internal identifier */
#define IDENT_MAXNCH_P (IDENT_MAXNCH_I+1)  /* so it can be null terminated */

#define LINE_MINCH (IDENT_MAXNCH_P+6)
#define LINE_MAXNCH 256
#define LINE_MAXNCH_W_NEWLINE (LINE_MAXNCH+1)
#define LINE_MAXNCH_P (LINE_MAXNCH+MAX_TAB_DELTA+2)

#define STRING_MAXNCH 1025
#define STRING_MAXNCH_P (STRING_MAXNCH+1)  /* so it can be null terminated */

#define FULL_TAIL_MSG_BUFFLEN 256

     /******************************************************/
     /******************************************************/
     /****                                              ****/
     /****   FOR ARRAY INDEX VALUES IN ERROR MESSAGES   ****/
     /****                                              ****/
     /******************************************************/
     /******************************************************/

#define WILD_INDEX ((int_type) -999999999)
#define NULL_INDEX ((int_type) -888888888)

                   /***************************/
                   /***************************/
                   /****                   ****/
                   /****   DEBUG$  FLAGS   ****/
                   /****                   ****/
                   /***************************/
                   /***************************/

#define DBG_ONES_BIT (debug_flag_type) 0x01
#define DBG_NONE (debug_flag_type) 0x00
/*
   Must arrange in order of increasing power of two and leave no holes.
   Order must be the same as in RWDEFS.H
*/
#define DBG_LEX  ((debug_flag_type) 0x01)
#define DBG_PRS  ((debug_flag_type) 0x02)
#define DBG_TRN  ((debug_flag_type) 0x04)
#define DBG_XEQ  ((debug_flag_type) 0x08)
#define DBG_ROU  ((debug_flag_type) 0x10)
#define DBG_DST  ((debug_flag_type) 0x20)
#define DBG_XPN  ((debug_flag_type) 0x40)
#define DBG_KOD  ((debug_flag_type) 0x80)
/**/
/* ABORT$ does not have to be included since program is immediated aborted. */
#define DBG_ALL  (debug_flag_type) 0xff
#define DBG_MOST (debug_flag_type) 0xbf  /* all except DBG_XPN */
#define DBG_XEQX (debug_flag_type) 0xf7  /* all except DBG_XEQ */


                   /****************************/
                   /****************************/
                   /****                    ****/
                   /****   WARNING LEVELS   ****/
                   /****                    ****/
                   /****************************/
                   /****************************/

#define NONE_WARN_LEV         0
#define SERIOUS_WARN_LEV      1
#define DEFAULT_WARN_LEV      2
#define MINOR_WARN_LEV        3
#define ALL_WARN_LEV         99
/*.....#define DANGER_WRN_LEV       1 */
/*.....#define MAJOR_WRN_LEV        2 */
/*.....#define SEMI_MAJOR_WRN_LEV   3 */
/*.....#define REG_WRN_LEV          4 */
/*.....#define SEMI_MINOR_WRN_LEV   5 */
/*.....#define MINOR_WRN_LEV        6 */
/*.....#define PETTY_WRN_LEV        7 */

      /*************************************************************/
      /*************************************************************/
      /****                                                     ****/
      /****   I/O INPUT STATEMENT SCRATCH FILE NESTING LEVELS   ****/
      /****                                                     ****/
      /*************************************************************/
      /*************************************************************/

#define SAVE_LEVEL_0 0
#define SAVE_LEVEL_1 1
#define SAVE_LEVELS 2

/*===========================================================================*/
#endif
