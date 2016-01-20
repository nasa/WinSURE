#if !defined(_ZZ_q_prsvars_DEFINED)
#define _ZZ_q_prsvars_DEFINED
/*===========================================================================*/
/**/
#include "lib_def.h"
#include "prstypes.h"
/**/
EXKW char operation_text[4*256];
EXKW void *full_storage;
EXKW Boolean *garbage_postfix_ops;
EXKW Boolean *garbage_infix_ops;
EXKW Boolean *garbage_operands;
EXKW unsigned short *offset_storage;
EXKW unsigned short *counts_storage;
EXKW token_info_type this,prev,ago;
EXKW expression_type this_expr;
EXKW operand_type *this_expr_operands_qq;
EXKW operation_type *this_expr_ops_qq;
/**/
EXKW Memix counts_offset_storage_count;
EXKW state_bitstring_type eval_state_ssbits;
EXKW token last_gotten_token;
EXKW identifier_info_type *last_gotten_idinfo;
/**/
EXKW Boolean parsing_convar_def;
EXKW Boolean this_looked_up;
/**/
EXKW short operand_max_size_used;
EXKW short infix_max_size_used;
EXKW short postfix_max_size_used;
/*===========================================================================*/
#endif
