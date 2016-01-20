#if !defined(_ZZ_q_prstypes_DEFINED)
#define _ZZ_q_prstypes_DEFINED
/*===========================================================================*/

#include "lib_def.h"
#include "cm_types.h"
#include "prsdefs.h"

typedef unsigned_255_type operation_type;
typedef identifier_info_type *operand_type;

typedef struct t__expression
   {
       operation_type *postfix_ops;   /* 4 */
       operation_type *infix_ops;     /* 4 */
       operand_type *operands;        /* 4 */
       short n_postfix_ops;           /* 2 */
       short n_infix_ops;             /* 2 */
       short n_operands;              /* 2 */
       short source_code_line_number; /* 2 */
       Boolean in_error;              /* 1 */
       type_flagword_type rtntype;    /* 1 total = 22 */
   } expression_type;


typedef struct t__binary_operand_item_info
   {
      short ixpo;
      short ixin;
      type_flagword_type type;
      type_flagword_type comp;
      type_flagword_type spec;
   } binary_operand_item_info_type;

typedef struct t__binary_operand_pair_info
   {
      binary_operand_item_info_type item[2];
      type_flagword_type ans;
      type_flagword_type spcans;
   } binary_operand_pair_info_type;

typedef struct t__built_in_parm_info
   {
       short parameter_count;
       type_flagword_type parameter_type;
       type_flagword_type return_type;
       operation_type opcode;
       operation_type aux_opcode;
       char label[16];
   } built_in_parm_info_type;


typedef struct t__macro_expansion_info
   {
       token_info_type *passed_token_list;
       unsigned short *passed_token_offset;
       unsigned short *passed_token_counts;
       short now_passed_ix;
       short now_passed_count;
       short passed_parameter_count;
       short ini_body_ix;
       short now_body_ix;
       short ovf_body_ix;
       short pos;
       short linnum;
   } macro_expansion_info_type;

#ifdef _NO_PROTO
typedef void (*any_void_routine)();
typedef void (*any_flagword_routine)();
typedef void (*any_parse_statement_routine)();
typedef type_flagword_type (*any_parse_routine)();
typedef Boolean (*any_is_op_routine)();
typedef operation_type (*any_give_op_routine)();
typedef void (*any_promote_routine)();
#else
typedef void (*any_void_routine)(void);
typedef void (*any_flagword_routine)(type_flagword_type);
typedef void (*any_parse_statement_routine)(void);
typedef type_flagword_type (*any_parse_routine)(void);
typedef Boolean (*any_is_op_routine)(token);
typedef operation_type (*any_give_op_routine)(token);
typedef void (*any_promote_routine)
    (binary_operand_pair_info_type *,short,operation_type,token);
#endif

/*===========================================================================*/
#endif
