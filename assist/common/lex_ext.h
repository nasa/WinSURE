#if !defined(_ZZ_q_lex_ext_DEFINED)
#define _ZZ_q_lex_ext_DEFINED
/*===========================================================================*/

#include "cm_types.h"

#ifdef _NO_PROTO
extern void init_lexlib();
extern void re_init_lexlib();
extern Memix set_idn_num_counts();
extern void lookup_as_symbolic_token();
extern token lookup_operator_token();
extern token give_rw_operation_token();
extern int_type decode_expected_integer();
extern real_type decode_expected_real();
extern void decode_number();
extern rwtype which_reserved_word();
extern token load_char_string_token();
extern void interpret_alpha_token();
extern token gettoken_only();
extern token dumtoken();
extern token gettoken_work();
extern void showtoken();
extern void allocate_identifier_table();
extern void free_identifier_table();
extern identifier_info_type *next_id_ptr();
extern void move_top_identifiers_down();
extern void preset_searchid_abort_if_exists();
extern void set_searchid_abort_if_exists();
extern void reset_searchid_abort_if_exists();
extern identifier_info_type *qqsearchid();
extern void declare_computational_ident();
extern void abort_on_table_overflow();
extern void *save_value_in_number_table();
extern identifier_info_type *qqsearchid_new();
extern identifier_info_type *searchid_old();
extern identifier_info_type *searchid_dummy();
extern identifier_info_type *searchid_oldssv();
extern identifier_info_type *searchid_dummy_or_oldssv();
extern identifier_info_type *searchid_oldssvconst();
extern identifier_info_type *searchid_dcl();
extern identifier_info_type *searchid_dcl_at_top();
extern identifier_info_type *searchid_silent();
extern void deactivate_identifier_level();
extern Boolean id_unique_first_n();
extern void push_ident_scope();
extern void pop_ident_scope();
extern int_type array_builtin_err();
extern void arraymin();
extern void arrayrowmin();
extern void arraycolmin();
extern void arraymax();
extern void arrayrowmax();
extern void arraycolmax();
extern void arraysum();
extern void arrayrowsum();
extern void arraycolsum();
extern void arraycount();
extern void arrayrowcount();
extern void arraycolcount();
extern void count_the_sub_array();
extern void min_the_sub_array();
extern void max_the_sub_array();
extern void sum_the_sub_array();
extern identifier_info_type *already_declared_err();
extern char *statement_name_reserved_word();
extern Subscript compute_offset();
#else
extern void init_lexlib(reserved_word_lookup_type *);
extern void re_init_lexlib(void);
extern Memix set_idn_num_counts(Memix,Memix);
extern void lookup_as_symbolic_token(char *);
extern token lookup_operator_token(char *);
extern token give_rw_operation_token(rwtype);
extern int_type decode_expected_integer(char *,token,int_type);
extern real_type decode_expected_real(char *,token,real_type);
extern void decode_number(token,char *,int_type *,real_type *);
extern rwtype which_reserved_word(char *,Boolean);
extern token load_char_string_token(FILE *,char *,char *,char);
extern void interpret_alpha_token(char *,token *,rwtype *);
extern token gettoken_only(char *,char *,rwtype *,short *,short *);
extern token dumtoken(rwtype *,char *,short *,short *,type_flagword_type);
extern token gettoken_work(rwtype *,char *,short *,short *);
extern void showtoken(token,rwtype,char *);
extern void allocate_identifier_table(Memix);
extern void free_identifier_table(void);
extern identifier_info_type *next_id_ptr(void);
extern void move_top_identifiers_down(void);
extern void preset_searchid_abort_if_exists(void);
extern void set_searchid_abort_if_exists(short);
extern void reset_searchid_abort_if_exists(void);
extern identifier_info_type *qqsearchid(char *,Boolean);
extern void declare_computational_ident
       (identifier_info_type *,type_flagword_type,value_union_type *);
extern void abort_on_table_overflow(void);
extern void *save_value_in_number_table(value_union_type *,type_flagword_type);
extern identifier_info_type *qqsearchid_new(char *,type_flagword_type,short);
extern identifier_info_type *searchid_old(char *,type_flagword_type,short);
extern identifier_info_type *searchid_dummy(char *,type_flagword_type,short);
extern identifier_info_type *searchid_oldssv(char *,type_flagword_type,short);
extern identifier_info_type
       *searchid_dummy_or_oldssv(char *,type_flagword_type,short);
extern identifier_info_type
       *searchid_oldssvconst(char *,type_flagword_type,short);
extern identifier_info_type *searchid_dcl(char *,type_flagword_type,short);
extern identifier_info_type 
       *searchid_dcl_at_top(char *,type_flagword_type,short);
extern identifier_info_type 
       *searchid_silent(char *,type_flagword_type,short,Boolean);
extern void deactivate_identifier_level(short);
extern Boolean id_unique_first_n(identifier_info_type *,int);
extern void push_ident_scope(void);
extern void pop_ident_scope(void);
extern int_type array_builtin_err(short,char *,char *);
extern void arraymin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowmin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolmin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraymax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowmax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolmax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraysum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowsum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolsum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowcount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolcount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void count_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void min_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void max_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void sum_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern identifier_info_type *already_declared_err
       (identifier_info_type *,char *,type_flagword_type,short);
extern char *statement_name_reserved_word(rwtype);
extern Subscript compute_offset(Subscript,Subscript,identifier_info_type *);
#endif
/*==================================================================*/
#define bug_print_idinfo(idinfo,aa,bb) print_idinfo(bugfile,idinfo,aa,bb)
#define bug_print_identifier_table() print_identifier_table(bugfile,FALSE)
/*==================================================================*/
#define xref_declaration(look) add_xref_item(look,'D')
#define xref_setvalue(look) add_xref_item(look,'S')
#define xref_utilize(look) add_xref_item(look,'U')
#define XREF_FIX_UNSET_ETC(ch) \
        if ((ch)=='D') undcl=FALSE; \
        if ((ch)=='S') unset=FALSE; \
        if ((ch)=='U') unuse=FALSE
#define xref_label(ch) (((ch)=='D') ? "DCL" : (((ch)=='S') ? "SET" : "USE"))
/*===========================================================================*/
#endif
