#if !defined(_ZZ_q_asttypes_DEFINED)
#define _ZZ_q_asttypes_DEFINED
/*===========================================================================*/
#include "commonio.h"
#include "astdefs.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "prstypes.h"
/*===========================================================================*/
typedef unsigned_255_type flag_type;          /* 0..0xff */

typedef struct t__instruction_pointer_union
   {
       void *vvv;                      /* to cast to block_if_type, etc. */
       relative_address_type reladdr;  /* relative address of code */
   } instruction_pointer_union_type;

typedef struct t__instruction
   {
       instruction_pointer_union_type ptr;
       opcode_type opcode;      /* instruction operation code */
   } instruction_type;          /*    for_loop_type, assert_type, ... */

typedef struct t__block_if
   {
       expression_type *then_test;    /* boolean expression for THEN */
       instruction_type *then_clause; /* code for THEN clause */
       instruction_type *else_clause; /* code for ELSE clause */
   } block_if_type;

typedef struct t__calc_assign
   {
       identifier_info_type *idinfo;  /* <ident> := */
       expression_type *expr;         /*            <expr> */
   } calc_assign_type;

typedef struct t__set_range
   {
       expression_type *lower_bound;
       expression_type *upper_bound;
   } set_range_type;

typedef struct t__for_loop
   {
       identifier_info_type *ident;  /* index variable */
       set_range_type *set_ranges;   /* pointer to array of IN ranges */
       short set_range_count;        /* count of number of IN ranges */
       instruction_type *body;       /* pointer to BODY of loop */
   } for_loop_type;

#ifdef NOW_OBSOLETE
typedef struct t__state_space_var_info
   {
       identifier_info_type *id_info; /* ssv: name of variable */
       state_offset_type offinfo;     /* ssv: offset & length info */
   } state_space_var_info_type;
#endif
typedef struct t__state_space_picture
   {
       /*vars_union_type*/ union t__vars_union *varu;
       Boolean *is_nested;
       short nvaru;
   } state_space_picture_type;

typedef union t__vars_union
   {
#ifdef NOW_OBSOLETE
       state_space_var_info_type *variable_info;
#endif
       identifier_info_type *id_info;
       state_space_picture_type *nested_space_picture;
       relative_address_type relative_address;
   } vars_union_type;

/*???????????????????????????????????????????????????????????????????
We can get rid of "node_union_type" if "state_space_picture_type"
is used instead 
???????????????????????????????????????????????????????????????????*/
typedef union t__node_union
   {
#ifdef NOW_OBSOLETE
       state_space_var_info_type varinfo; /* 10 bytes */
#endif
       state_space_picture_type pix;      /* 10 bytes  (max(10,10)=10 bytes) */
   } node_union_type;
/*???????????????????????????????????????????????????????????????????
We can get rid of "node_union_type" if "state_space_picture_type"
is used instead 
???????????????????????????????????????????????????????????????????*/


typedef struct t__space_expression
   {
       expression_type *exprs;
       operand_type *vars;
       short n_vars;
   } space_expression_type;

typedef struct t__tranto_clause
   {
       space_expression_type sex;   /* list of space transition expressions */
       expression_type *rate_exprs; /* ptr to array of rate expressions */
       short n_rate_exprs;          /* count of rate expressions */
       short source_code_line_number;
   } tranto_clause_type;

typedef struct t__booltest
   {
       expression_type *expr;         /* boolean expr to ASSERT,DEATHIF,etc. */
       short source_code_line_number; /* line number in listing file */
       short lumping_sequence;        /* sequence index (0..n-1) in source */
   } booltest_type;                   /* e.g., first DEATHIF, second DEATHIF */

typedef booltest_type assert_type;
typedef booltest_type deathif_type;
typedef booltest_type pruneif_type;

typedef struct t__do_code_stuff
    {
         identifier_info_type *do_idinfo;
#ifdef BUGGY
short do_code_level;
#endif
         Subscript do_index;
    } do_code_stuff_type;
/*===========================================================================*/
#endif
