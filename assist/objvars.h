#if !defined(_ZZ_q_objvars_DEFINED)
#define _ZZ_q_objvars_DEFINED
/*===========================================================================*/

           /************************************************************/
           /************************************************************/
           /****                                                    ****/
           /****   MEMORY LAYOUT POINTERS AND RELATED INFORMATION   ****/
           /****                                                    ****/
           /************************************************************/
           /************************************************************/

/*===========================================================================*/

#include "objtypes.h"

EXKW memsize_t memory_bytes_required[MAX_OBJ_COUNTER_DIM];
EXKW char *section_start_addrs[MAX_OBJ_COUNTER_DIM];
EXKW object_table_type memory_sort_order[MAX_OBJ_COUNTER_DIM];

/*===========================================================================*/
#endif
