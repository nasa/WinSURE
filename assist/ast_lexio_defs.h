#if !defined(_ZZ_q_ast_lexio_defs_DEFINED)
#define _ZZ_q_ast_lexio_defs_DEFINED
/*===========================================================================*/

#include "io_ext.h"
#include "lex_ext.h"

#define EOF_CHAR (char) 0xff /* any unused control char (system dependent) */
#define PROMPT_STR "?"       /* interactive prompt */

                    /******************************/
                    /******************************/
                    /****                      ****/
                    /****   COMPILE  OPTIONS   ****/
                    /****                      ****/
                    /******************************/
                    /******************************/

#if defined(SURE) || defined(PAWS)
#define SIGNED_TO_WORDS
#endif

                  /*******************************/
                  /*******************************/
                  /****                       ****/
                  /****   IDENTIFIER  USAGE   ****/
                  /****                       ****/
                  /*******************************/
                  /*******************************/

#define USAGE_IN_TTO  ((ident_usage_flag_type) 0x01)
#define USAGE_IN_TST  ((ident_usage_flag_type) 0x02)
#define USAGE_IN_ALL  ((ident_usage_flag_type) 0x3f)
#define USAGE_SCRATCH ((ident_usage_flag_type) 0x40)

                   /***************************/
                   /***************************/
                   /****                   ****/
                   /****   MISCELLANEOUS   ****/
                   /****                   ****/
                   /***************************/
                   /***************************/

#define SCOPE_DUMMY_PARAM_0 30000  /* this allows for 2767 dummy parameters */
#define NULL_PTR ((void *) NULL)

#define XREF_ID_TRUNC_NCH 23
#define XREF_IDENT_MAXNCH 28
#define XREF_IDENT_MAXNCH_P (XREF_IDENT_MAXNCH+1)

#define MAX_FOR_LOOP_PAIR 64

#define EMPTY_OFFSET (relative_address_type) 0xffffffff
#define DFLT_MAX_ERR_COUNT 40
#define DFLT_MAX_WRN_COUNT 40
#define DFLT_MAX_ERRS_PER_LINE 5
#define DFLT_MAX_WRNS_PER_LINE 5
#define DFLT_MAX_LINE_LENG 132
#define MSG_ONLY_BUFFLEN 620 /* 15 lines at 40 chars each w/ 20 chars grace */
#define MSG_BUFFLEN (MSG_ONLY_BUFFLEN+STRING_MAXNCH_P)


/* #ifdef MSDOS */
/* #define REALLOC_BUCKETS_AT_A_TIME 100 */
/* #define MAX_IDN_DFLT 200 */
/* #define MAX_NUM_DFLT 50 */
/* #define MAX_OP_DFLT 50 */
/* #define MAX_XPR_DFLT 50 */
/* #define MAX_PRM_DFLT 32 */
/* #define MAX_NEST_DFLT 8 */
/* #define MAX_BODY_DFLT 256 */
/* #define MAX_RULE_DFLT 1024 */
/* #else */
/* #define REALLOC_BUCKETS_AT_A_TIME 1000 */
/* #define MAX_IDN_DFLT 400 */
/* #define MAX_NUM_DFLT 200 */
/* #define MAX_OP_DFLT 300 */
/* #define MAX_XPR_DFLT 300 */
/* #define MAX_PRM_DFLT 64 */
/* #define MAX_NEST_DFLT 16 */
/* #define MAX_BODY_DFLT 1024 */
/* #define MAX_RULE_DFLT 4096 */
/* #endif */

/* CHANGED BY RWB 10/2/04: */

#ifdef MSDOS
#define REALLOC_BUCKETS_AT_A_TIME 2000
#define MAX_IDN_DFLT 1000
#define MAX_NUM_DFLT 1000
#define MAX_OP_DFLT 50
#define MAX_XPR_DFLT 300
#define MAX_PRM_DFLT 128
#define MAX_NEST_DFLT 32
#define MAX_BODY_DFLT 4000
#define MAX_RULE_DFLT 10000
#else
#define REALLOC_BUCKETS_AT_A_TIME 2000
#define MAX_IDN_DFLT 1000
#define MAX_NUM_DFLT 1000
#define MAX_OP_DFLT 1000
#define MAX_XPR_DFLT 1000
#define MAX_PRM_DFLT 128
#define MAX_NEST_DFLT 32
#define MAX_BODY_DFLT 4000
#define MAX_RULE_DFLT 10000
#endif

/*===========================================================================*/
#endif
