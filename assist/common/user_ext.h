  /*******************************************************************/
  /*******************************************************************/
  /****                                                           ****/
  /****   USER SUPPLIED ERROR ROUTINES FOR USE BY ALL LIBRARIES   ****/
  /****                                                           ****/
  /*******************************************************************/
  /*******************************************************************/

#if !defined(_ZZ_q_user_ext_DEFINED)
#define _ZZ_q_user_ext_DEFINED
/*===========================================================================*/

#ifdef _NO_PROTO
extern void abort_program();
extern void abort_program_quietly();
#else
extern void abort_program(void);
extern void abort_program_quietly(void);
#endif

/*===========================================================================*/
#if !(defined(WPT_MODAL)||defined(WPT_PREFERRED)||defined(WPT_MODALPANEL))

#include "cm_types.h"

#ifdef _NO_PROTO
/*------------------------------------------------------------------*/
extern void show_err_on_both();
extern void show_wrn_on_both();
extern void show_err();
extern void reset_arrow_sides();
extern void set_arrow_sides();

extern ssvar_value_type state_space_arr_value();
extern ssvar_value_type state_space_value();

extern void add_xref_item();
extern void enk_line_and_pos();
extern void give_ch_info();
extern char *encode_space_bits();
extern void user_advance_1_char();
extern Boolean test_if_char_is_first_on_line();
extern void advance_one_token();
extern void get_tok_after_matching_paren();
extern token give_token_after_matching();
extern void user_show_extra_after_error();
extern void expand_implicit();
extern void expand_regular();
extern short print_idinfo();

#if !defined(PRODUCTION)
extern void shew_logical_rw();
extern void shew_logical_token();
#endif
/*------------------------------------------------------------------*/
#else
/*------------------------------------------------------------------*/
extern void show_err_on_both(short,void *,type_flagword_type);
extern void show_wrn_on_both(short,void *,type_flagword_type,short);
extern void show_err(FILE *,short,void *,type_flagword_type);
extern void reset_arrow_sides(void);
extern void set_arrow_sides(Boolean,Boolean);

extern ssvar_value_type state_space_arr_value
       (state_bitstring_type,state_offset_type *,Subscript);
extern ssvar_value_type state_space_value
       (state_bitstring_type,state_offset_type *);

extern void add_xref_item(char *,char);
extern void enk_line_and_pos(char *,char);
extern void give_ch_info(short *,short *,char *,char *);
extern char *encode_space_bits(state_bitstring_type);
extern void user_advance_1_char(scanning_character_info_type *);
extern Boolean test_if_char_is_first_on_line(void);
extern void advance_one_token(void);
extern void get_tok_after_matching_paren(char *,Boolean);
extern token give_token_after_matching(rwtype *);
extern void user_show_extra_after_error
       (identifier_info_type *,int_type,int_type);
extern void expand_implicit(void);
extern void expand_regular(void);
extern short print_idinfo(FILE *,identifier_info_type *,int,int);

#if !defined(PRODUCTION)
extern void shew_logical_rw(rwtype);
extern void shew_logical_token(token);
#endif
/*------------------------------------------------------------------*/
#endif

#define get_lookahead_token(tmpid) \
        get_tok_after_matching_paren(tmpid,TRUE)
#define get_token_after_matching_paren(tmpid) \
        get_tok_after_matching_paren(tmpid,FALSE)

#endif
/*===========================================================================*/
#endif
