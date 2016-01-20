#if !defined(_ZZ_q_mac_ext_DEFINED)
#define _ZZ_q_mac_ext_DEFINED
/*===========================================================================*/
#include "macvars.h"
#include "prstypes.h"
extern short fetch_next_passed_token(macro_expansion_info_type *,short,short);
extern short fetch_next_body_token(macro_expansion_info_type *,short);
extern void get_tok_after_matching_paren(char *,Boolean);
extern token give_token_after_matching(rwtype *);
extern Boolean advance_one_expansion_token
               (macro_expansion_info_type *,Boolean *,Boolean *,Boolean);
extern void advance_one_token(void);
extern void insert_token_info_as_this(token_info_type *,short,short);
extern void push_macro_body(macro_expansion_info_type *,char *);
extern void pop_macro_body(void);
extern short save_one_calling_token(token_info_type *,Boolean);
extern short save_nested_calling_tokens(token,Boolean);
extern short save_tokens_for_single_calling(token,Boolean);
extern void fill_in_macinf(short,macro_expansion_info_type *);
extern void expand_implicit(void);
extern void expand_regular(void);
extern void expand_macro(char *,token,token,short,short);
extern short parse_macro_parmlist(token,token,short,any_searchid_routine);
extern short parse_mac_parmlst_pushing_scope
             (char *,Boolean,Boolean,any_searchid_routine,token_info_type *,
              Subscript *);
extern void parse_macro_def(char *,char *,short,Boolean,type_flagword_type,
                            any_searchid_routine,any_searchid_routine);
extern void parse_regular_def(void);
extern void parse_implicit_def(void);
/*===========================================================================*/
#endif
