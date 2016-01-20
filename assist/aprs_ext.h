#if !defined(_ZZ_q_aprs_ext_DEFINED)
#define _ZZ_q_aprs_ext_DEFINED
/*===========================================================================*/
#include "prstypes.h"
#include "asttypes.h"
#include "objtypes.h"
/*===========================================================================*/
extern relative_address_type store_operand_data(operand_type *,short);
extern void assist_alloc_for_parse(void);
extern void assist_finish_varinsts(void);
extern void assist_store_space_and_misc(void);
extern void assist_free_after_parse(void);
extern void qq_store_space_picture(state_space_picture_type **);
extern void store_expr_data_only(expression_type *);
extern void print_constant_definition(FILE *,char *,expression_type *);
extern relative_address_type store_expr_struct_only(void);
extern relative_address_type store_this_expr(void);
extern relative_address_type store_given_expr(expression_type *);
extern Memix increment_scrops_ele(short);
extern Memix increment_scropnds_ele(short);
extern Memix increment_ixs_ele(short);
extern Memix increment_idptrs_ele(short);
extern Memix increment_exprs_ele(short);
extern Memix increment_pixarr_ele(short);
extern Memix increment_node_ele(short);
extern Memix increment_rule_ele(short);
extern Memix store_scratch_exprs(Memix,Memix,
                                 relative_address_type *,
                                 relative_address_type *,
                                 ident_usage_flag_type);
extern void remember_expr_ixs(Memix *,Memix *,Memix *,Memix *);
extern void restore_expr_ixs(Memix,Memix,Memix,Memix);
extern void append_expr_to_this(expression_type *);
extern void save_expr_into_scratch(expression_type *,identifier_info_type *);
extern void save_this_scratch_expr(identifier_info_type *);
#ifdef BUGGY
extern int bug_node_number(node_union_type *);
#endif
extern short max_encoded_space_bits_strlen(void);
extern short qq_max_enk_space_bits_strlen(state_space_picture_type *);
extern char *encode_space_bits(state_bitstring_type);
extern short qq_encode_space_bits
       (state_bitstring_type,state_space_picture_type *,short);
extern void print_space_picture(FILE *,state_space_picture_type *);
#define bug_print_space_picture(pix) print_space_picture(bugfile,pix)
/*----------------------------------------  Statement parsing */
extern void parse_constant_definition_stat(void);
extern void parse_constant_def_clause(char *,short,Boolean,type_flagword_type);
type_flagword_type parse_simple_def_expr(Boolean *,Boolean,Boolean,short *);
extern void parse_ONEDEATH_def_stat(void);
extern void parse_COMMENT_def_stat(void);
extern void parse_MODSTATS_def_stat(void);
extern void parse_ECHO_def_stat(void);
extern void parse_TRIM_def_stat(void);
extern void parse_INPUT_statement(void);
extern void parse_SPACE_statement(void);
extern void adjust_picture_after_move_down(state_space_picture_type *);
extern state_space_picture_type *parse_space_picture
       (bitsize_type old_bitoff,bitsize_type *new_bitoff);
extern void parse_array_state_space_var(identifier_info_type *);
extern Memix qq_parse_space_expr(state_space_picture_type *,Memix,Boolean);
extern void copy_subscript_to_space_list(Subscript);
extern void save_and_re_init(type_flagword_type);
extern void parse_space_expr_list(space_expression_type *);
extern void qq_parse_space_expr_list(space_expression_type *);
extern void parse_space_expression(Boolean);
extern void parse_FUNCTION_statement(void);
extern void parse_IMPLICIT_statement(void);
extern void parse_VARIABLE_statement(void);
extern void parse_START_statement(void);
extern void save_rule(opcode_type,relative_address_type);
extern void parse_ASSERT_statement(void);
extern void parse_DEATHIF_statement(void);
extern void parse_PRUNEIF_statement(void);
extern void parse_TRANTO_statement(void);
extern void parse_IF_statement(void);
extern void parse_FOR_statement(void);
extern void parse_quoted_SURE_statement(void);
extern void parse_DEBUG_statement(void);
extern void parse_CLOPT_statement(void);
extern void parse_tranto_clause(void);
extern Boolean parse_reserved_word_stat(void);
extern void parse_a_statement(Boolean,token);
extern void parse_setup_section(void);
extern void parse_start_section(void);
extern void cat_rule_terminator(char *,rwtype);
#ifdef OBSOLETE
extern void touch_all_variable_usages(instruction_type *,short);
extern void touch_btest(booltest_type *,ident_usage_flag_type);
extern void touch_tranto(tranto_clause_type *,ident_usage_flag_type);
extern void touch_blockif(block_if_type *,ident_usage_flag_type);
extern void touch_for(for_loop_type *,ident_usage_flag_type);
#endif
extern void touch_expr_in_table
       (expression_type *,ident_usage_flag_type,Boolean *);
extern void touch_expr(expression_type *,ident_usage_flag_type);
extern Boolean parse_rule_stat_seq(relative_address_type *,
                                   relative_address_type *,
                                   rwtype,rwtype,short,char *,
                                   Boolean,Boolean);
extern void parse_rule_section(void);
extern void parse_program(void);
extern void single_code_map_section(object_table_type);
extern void application_map_sections(void);
extern void extract_opcode_and_section
       (opcode_type,short,opcode_type *,short *);
extern opcode_type give_temporary_opcode(opcode_type,short);
extern void show_prs_sizes_on_both(void);
extern void extract_map_sections(void);
extern void do_switch_to_stdin(char *,short);
extern void do_switch_back_to_inputfile(short);
extern Memix increment_body(void);
extern void save_function_body_token(token_info_type *);
extern void user_show_extra_after_error(operand_type,int_type,int_type);
/*---------------------------------------------   macros */
#define store_this_expr_data_only() store_expr_data_only(&this_expr)
/*===========================================================================*/
#endif
