#if !defined(_ZZ_q_objvarsP_DEFINED)
#define _ZZ_q_objvarsP_DEFINED
/*===========================================================================*/

           /************************************************************/
           /************************************************************/
           /****                                                    ****/
           /****   MEMORY LAYOUT POINTERS AND RELATED INFORMATION   ****/
           /****                                                    ****/
           /************************************************************/
           /************************************************************/

#include "objvars.h"

#ifdef MUST_ALIGN
#ifdef ALIGN_STORAGE
static align_boundaries[MAX_OBJ_COUNTER_DIM] =
   {
        4, 1, 1, 4,  /* UNKNOWN,CHAR,BOOL,INT */
        8, 4, 4, 4,  /* REAL,SOFF,FUT,FUT */
        4, 1, 1, 4,  /* UNKNOWN,CHAR,BOOL,INT */
        8, 4, 4, 4,  /* REAL,SOFF,FUT,FUT */
        4, 4, 1, 4,  /* EXPR, OPERANDS, OPS, application0 */
        4, 4, 4, 4,
        4, 4, 4, 4,  /* 0x18 */
        4, 4, 4, 4,
        4, 4, 4, 4,  /* 0x20 */
        4, 4, 4, 4,
        4, 4, 4, 4,  /* 0x28 */
        4, 4, 4, 4
   };
#endif
#endif
/*===========================================================================*/
#endif
