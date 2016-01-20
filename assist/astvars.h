#if !defined(_ZZ_q_astvars_DEFINED)
#define _ZZ_q_astvars_DEFINED
/*===========================================================================*/
#include "lib_def.h"
#include "commonio.h"
#include "cm_types.h"
#include "asttypes.h"
/*===========================================================================*/
EXKW Boolean trimming_is_on,must_assist_alloc_for_parse;
EXKW state_space_picture_type *state_space_picture_ptr;
EXKW short assert_count,pruneif_count,deathif_count,trimdeath_count;
EXKW short total_prune_count;
EXKW tranto_clause_type *start_tranto_ptr;
EXKW instruction_type *initial_start_instr[RULE_OPCODE_INDEX_COUNT];
EXKW Memix function_body_size_used;
/*===========================================================================*/
#endif
