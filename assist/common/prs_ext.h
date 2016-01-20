#if !defined(_ZZ_q_prs_ext_DEFINED)
#define _ZZ_q_prs_ext_DEFINED
/*===========================================================================*/

#include "cm_types.h"
#include "prstypes.h"

/*---------------------------------------------   conversion section */
#ifdef _NO_PROTO
extern Memix init_prslib();
extern void re_init_prslib();
extern Boolean is_bool_relation();
extern Boolean is_relation();
extern Boolean is_bool_op();
extern Boolean is_arith_op();
extern Boolean is_add_op();
extern Boolean is_mpy_op();
extern Boolean is_eq_relation();
extern Boolean is_ineq_relation();
extern Boolean is_not_op();
extern Boolean is_cat_op();
extern Boolean is_and_op();
extern Boolean is_or_op();
extern Boolean is_pow_op();
extern Boolean is_sign_op();
extern Boolean is_value_token();
extern Boolean is_literal_token();
extern Boolean is_unary_op();
operation_type give_relation_op();
operation_type give_add_op();
operation_type give_cat_op();
operation_type give_not_op();
operation_type give_and_op();
operation_type give_or_op();
operation_type give_sign_op();
operation_type give_unary_op();
operation_type give_infix_pow_op();
operation_type give_literal_type();
operation_type give_bool_relation_op();
extern operation_type give_pow_op();
extern operation_type give_mpy_op();
extern operation_type give_any_precedence_op();
#else
extern Memix init_prslib(void);
extern void re_init_prslib(Memix *,Stacktop *);
extern Boolean is_bool_relation(token);
extern Boolean is_relation(token);
extern Boolean is_bool_op(token);
extern Boolean is_arith_op(token);
extern Boolean is_add_op(token);
extern Boolean is_mpy_op(token);
extern Boolean is_eq_relation(token);
extern Boolean is_ineq_relation(token);
extern Boolean is_not_op(token);
extern Boolean is_cat_op(token);
extern Boolean is_and_op(token);
extern Boolean is_or_op(token);
extern Boolean is_pow_op(token);
extern Boolean is_sign_op(token);
extern Boolean is_value_token(token);
extern Boolean is_literal_token(token);
extern Boolean is_unary_op(token);
operation_type give_relation_op(token);
operation_type give_add_op(token);
operation_type give_cat_op(token);
operation_type give_not_op(token);
operation_type give_and_op(token);
operation_type give_or_op(token);
operation_type give_sign_op(token);
operation_type give_unary_op(token);
operation_type give_infix_pow_op(token);
operation_type give_literal_type(token);
operation_type give_bool_relation_op(token);
extern operation_type give_pow_op(token,type_flagword_type,type_flagword_type);
extern operation_type give_mpy_op(token);
extern operation_type give_any_precedence_op(token,rwtype,token);
#endif
/*---------------------------------------------   toolbox section */
#ifdef _NO_PROTO
extern type_flagword_type recurse_the_parse();
extern Boolean adjust_after_parse();
extern void test_total_for_fits();
extern token_info_type *allocate_for_parse_stage();
extern void init_prslib_counts();
extern void free_after_parse_stage();
extern void dummy_token();
extern void skip_to_next_specified();
extern void skip_one_when_skipping();
extern Boolean skip_nested_when_skipping();
extern void skip_to_lower_precedence();
extern int precedence_cmp();
extern void skip_to_next_semicolon();
extern void skip_to_next_comma_or();
extern void parse_must_be_semicolon();
extern operation_type give_promote_op();
extern Boolean promote_to_same_type();
extern Memix increment_postfix_operation();
extern Memix increment_infix_operation();
extern Memix increment_operand();
extern void make_room_and_stuff_into_expr();
extern short save_operator_and_advance_1_tok();
extern short calc_operation_text_leng();
extern void show_operation_text();
extern void show_infix_or_postfix();
extern void show_message();
extern void save_postfix();
extern Memix save_infix();
extern void lookup_ident();
extern void save_operand();
extern void next_term_or_factor();
extern void set_independent_of_variable();
extern void start_independent_of_check();
extern void finish_independent_of_check();
extern void not_independent_of();
#else
extern type_flagword_type recurse_the_parse(any_parse_routine,char *);
extern Boolean adjust_after_parse(binary_operand_item_info_type *);
extern void test_total_for_fits(long);
extern token_info_type *allocate_for_parse_stage
          (Memix,Memix,Memix,
           Memix,Memix,Memix *,Stacktop *,
           macro_expansion_info_type **);
extern void init_prslib_counts(Memix *,Stacktop *);
extern void free_after_parse_stage(macro_expansion_info_type **,
                                   token_info_type **);
extern void dummy_token(type_flagword_type);
extern void skip_to_next_specified(Boolean,token);
extern void skip_one_when_skipping(Boolean);
extern Boolean skip_nested_when_skipping(token,Boolean);
extern void skip_to_lower_precedence(operation_type);
extern int precedence_cmp(operation_type,operation_type);
extern void skip_to_next_semicolon(Boolean);
extern void skip_to_next_comma_or(Boolean,token);
extern void parse_must_be_semicolon(void);
extern operation_type give_promote_op
       (type_flagword_type,type_flagword_type,short);
extern Boolean promote_to_same_type
       (binary_operand_pair_info_type *,short,Boolean);
extern Memix increment_postfix_operation(void);
extern Memix increment_infix_operation(void);
extern Memix increment_operand(void);
extern void make_room_and_stuff_into_expr(short,operation_type);
extern short save_operator_and_advance_1_tok(token_info_type *);
extern short calc_operation_text_leng(operation_type,Boolean,Boolean *);
extern void show_operation_text(FILE *,operation_type,Boolean);
extern void show_infix_or_postfix(operation_type *,short,char *);
extern void show_message(char *);
extern void save_postfix(operation_type);
extern Memix save_infix(operation_type);
extern void lookup_ident(void);
extern void save_operand(operand_type);
extern void next_term_or_factor(binary_operand_pair_info_type *);
extern void set_independent_of_variable(identifier_info_type *);
extern void start_independent_of_check(void);
extern void finish_independent_of_check(void);
extern void not_independent_of(void);
#endif
/*---------------------------------------------   expression parsing section */
#ifdef _NO_PROTO
extern void init_expression();
extern void make_inc_or_dec_variable_expr();
extern void make_inc_or_dec_array_ele_expr();
extern Boolean qq_promote_and_save();
extern void promote_and_save();
extern void promote_and_save_bool();
extern void promote_and_save_multiplicative();
extern void promote_and_save_power();
extern type_flagword_type parse_left_to_right();
extern void expand_tokens_if_function();
extern type_flagword_type parse_subscript();
extern type_flagword_type qq_parse_subscript();
extern type_flagword_type parse_builtin();
extern type_flagword_type parse_bin_numeric_item();
extern type_flagword_type parse_array_name();
extern type_flagword_type parse_array_name_aux();
extern type_flagword_type qq_parse_array_name();
extern type_flagword_type parse_numeric_item();
extern type_flagword_type parse_factor();
extern type_flagword_type parse_term();
extern type_flagword_type parse_term_expr();
extern type_flagword_type init_and_parse_rate_expr();
extern type_flagword_type init_and_parse_expression();
extern type_flagword_type init_and_parse_bool_expr();
extern type_flagword_type parse_numeric_expr_silently();
extern type_flagword_type parse_numeric_sub_expr_in_bool();
extern type_flagword_type parse_numeric_expression();
extern type_flagword_type parse_finish_numeric_comparison();
extern type_flagword_type parse_finish_bool_comparison();
extern type_flagword_type parse_simple_bool_item();
extern type_flagword_type parse_bool_item();
extern type_flagword_type parse_bool_factor();
extern type_flagword_type parse_bool_term();
extern type_flagword_type parse_bool_term_expr();
extern type_flagword_type parse_boolean_expression();
#else
extern void init_expression(Boolean);
extern void make_inc_or_dec_variable_expr
       (identifier_info_type *,operation_type);
extern void make_inc_or_dec_array_ele_expr
       (identifier_info_type *,operation_type,operation_type);
extern Boolean qq_promote_and_save(binary_operand_pair_info_type *,
                                   short,operation_type,token);
extern void promote_and_save(binary_operand_pair_info_type *,
                             short,operation_type,token);
extern void promote_and_save_bool(binary_operand_pair_info_type *,
                                  short,operation_type,token);
extern void promote_and_save_multiplicative
     (binary_operand_pair_info_type *,short,operation_type,token);
extern void promote_and_save_power(binary_operand_pair_info_type *,
                                   short,operation_type,token);
extern type_flagword_type parse_left_to_right
   (any_parse_routine,any_parse_routine,char *,char *,
    any_is_op_routine,any_give_op_routine,any_promote_routine,Boolean);
extern void expand_tokens_if_function(void);
extern type_flagword_type parse_subscript(void);
extern type_flagword_type qq_parse_subscript(Boolean,any_flagword_routine);
extern type_flagword_type parse_builtin(void);
extern type_flagword_type parse_bin_numeric_item(void);
extern type_flagword_type parse_array_name(void);
extern type_flagword_type parse_array_name_aux(void);
extern type_flagword_type qq_parse_array_name(Boolean,Boolean);
extern type_flagword_type parse_numeric_item(void);
extern type_flagword_type parse_factor(void);
extern type_flagword_type parse_term(void);
extern type_flagword_type parse_term_expr(void);
extern type_flagword_type init_and_parse_rate_expr(Boolean *);
extern type_flagword_type
       init_and_parse_expression(Boolean *,Boolean,Boolean,short *);
extern type_flagword_type
       init_and_parse_bool_expr(Boolean *,Boolean,Boolean,short *);
extern type_flagword_type parse_numeric_expr_silently(void);
extern type_flagword_type parse_numeric_sub_expr_in_bool(void);
extern type_flagword_type parse_numeric_expression(void);
extern type_flagword_type parse_finish_numeric_comparison(void);
extern type_flagword_type parse_finish_bool_comparison(void);
extern type_flagword_type parse_simple_bool_item(void);
extern type_flagword_type parse_bool_item(void);
extern type_flagword_type parse_bool_factor(void);
extern type_flagword_type parse_bool_term(void);
extern type_flagword_type parse_bool_term_expr(void);
extern type_flagword_type parse_boolean_expression(void);
#endif
/*---------------------------------------------   expression eval section */
#ifdef _NO_PROTO
extern int_type test_value_0_to_max();
extern short eval_for_expected_count();
extern ssvar_value_type eval_for_expected_ssv();
extern int_type fast_eval_for_expected_integer();
extern int_type eval_for_expected_integer();
extern Boolean fast_eval_for_expected_boolean();
extern Boolean eval_for_expected_boolean();
extern short find_postfix_sub_expr();
extern short skip_over_infix_expr();
extern short find_postfix_finish();
extern void eval_sub_infix_expr();
extern short print_infix_expr();
#else
extern int_type test_value_0_to_max(int_type,int_type);
extern short eval_for_expected_count
       (expression_type *,int_type,Boolean,short,short,Boolean);
extern ssvar_value_type eval_for_expected_ssv
       (expression_type *,int_type,Boolean,short,short,Boolean,Boolean);
extern int_type fast_eval_for_expected_integer(expression_type *);
extern int_type eval_for_expected_integer
       (expression_type *,int_type,Boolean,short,short,short,Boolean);
extern Boolean fast_eval_for_expected_boolean(expression_type *);
extern Boolean eval_for_expected_boolean(expression_type *,Boolean,Boolean);
extern short find_postfix_sub_expr(expression_type *,short);
extern short skip_over_infix_expr
       (operation_type **,short *,Boolean,operation_type,operation_type,
        expression_type *,short);
extern short find_postfix_finish(expression_type *,short,short,operation_type);
extern void eval_sub_infix_expr
       (expression_type *,short *,operation_type **,operation_type *);
extern short print_infix_expr
       (FILE *,expression_type *,short,short,short,Boolean,Boolean);
#endif
/*---------------------------------------------   miscellaneous section */
#ifdef _NO_PROTO
extern void make_boolean();
extern void default_irange();
extern void parse_irange();
extern void parse_int_for_flag_set_spec();
extern void parse_flag_set_spec();
extern void show_this_expr();
#else
extern void make_boolean(state_offset_type *,type_flagword_type *);
extern void default_irange(state_offset_type *,bitsize_type,bitsize_type);
extern void parse_irange(state_offset_type *,ssvar_value_type);
extern void parse_int_for_flag_set_spec(unsigned_31_type *,Boolean);
extern void parse_flag_set_spec(unsigned_31_type *,Boolean,Boolean);
extern void show_this_expr(void);
#endif
/*---------------------------------------------   token testing macros */
#define this_tok_is_option_def() \
        ((this.tok==TK_RW) && \
         (this.rw>=FIRST_OPTION_DEF_RW)&&(this.rw<RW_STATEMENT_COUNT))
#define this_tok_is_rw(qrw) ((Boolean) ((this.tok==TK_RW)&&(this.rw==(qrw))))
#define this_tok_is_rw_stat() \
        ((Boolean) ((this.tok==TK_RW)&&(this.rw < RW_STATEMENT_COUNT)))
#define this_tok_is_rw_stat_or_eoseq() \
        ((Boolean) ((this.tok==TK_RW)&& \
                    ((this.rw < RW_STATEMENT_COUNT)||(this.rw==RW_ENDIF)|| \
                     (this.rw==RW_ELSE)||(this.rw==RW_ENDFOR))))
#define this_is_not_semi() \
        ((Boolean) ((this.tok!=TK_SEMI)&&(this.tok!=TK_EOF)))
#define this_is_not(t) \
        ((Boolean) ((this.tok!=TK_SEMI)&&(this.tok!=(t))&&(this.tok!=TK_EOF)))
#define this_is_not_param_terminator(t) \
        ((Boolean) ((this.tok!=TK_SEMI)&&(this.tok!=(t))&& \
                    (this.tok!=TK_COMMA)&&(this.tok!=TK_EOF)))
#define this_is_not_list_terminator() this_is_not(TK_RP)
/*---------------------------------------------   error/warning macros */
#define warn_this_pos(ee,lv) \
        show_wrn_at_pos_on_both(ee,this.pos,this.id,CHAR_TYPE,lv)
#define this_expr_error_this_pos(ee) \
        (this_expr.in_error=TRUE, \
         show_err_at_pos_on_both(ee,this.pos,this.id,CHAR_TYPE))
#define error_msg_this_pos(ee,qptr,qty) \
        show_err_at_pos_on_both(ee,this.pos,qptr,qty)
#define error_this_pos(ee) \
        show_err_at_pos_on_both(ee,this.pos,this.id,CHAR_TYPE)
#define warn_saved_pos(ee,lv) \
        show_wrn_at_pos_on_both(ee,saved.pos,saved.id,CHAR_TYPE,lv)
#define error_saved_pos(ee) \
        show_err_at_pos_on_both(ee,saved.pos,saved.id,CHAR_TYPE)
#define tail_warn_this_pos(ee,lv,t) \
        show_tail_wrn_at_pos_on_both(ee,this.pos,this.id,CHAR_TYPE,lv,t)
#define tail_error_this_pos(ee,t) \
        show_tail_err_at_pos_on_both(ee,this.pos,this.id,CHAR_TYPE,t)
#define tail_error_saved_pos(ee,t) \
        show_tail_err_at_pos_on_both(ee,saved.pos,saved.id,CHAR_TYPE,t)
#define val_error_this_pos(ee,v,t) \
        value=v; \
        show_tail_err_at_pos_on_both(ee,this.pos,(void *) &value,INT_TYPE,t)
#define val_error_arrowpos(ee,v,t) \
        value=v; \
        show_tail_err_at_pos_on_both(ee,arrowpos,(void *) &value,INT_TYPE,t)
#define for_all_ptrs(ooo,ppp,pppovf,type) \
        for (ppp=(type *) section_start_addrs[ooo], \
             pppovf=(type *) (((char *) section_start_addrs[ooo]) + \
                              memory_bytes_required[ooo]); \
             ppp<pppovf;++ppp)
/*===========================================================================*/
#define is_a_multi_arg_builtin_function(t,r) \
        (((t)==TK_RW)&&((r)>=RW_VARIAB_0)&&((r)<=RW_VARIAB_9))
#define is_a_variable(p) \
        (bitset((*(p)).flags,VARIABLE_TYPE) && \
         (!bitset((*(p)).flags,FUNCTION_TYPE)))
#define is_a_builtin_function(t,r) \
        (((t)==TK_RW)&&((r)>=RW_BUILTIN_0)&&((r)<=RW_BUILTIN_9))
#define is_a_regular_function(t,p) \
        (((t)==TK_ID)&& \
         (p!=((identifier_info_type *) NULL))&& \
         (((*p).flags & FUNC_IMPL_MASK)==FUNCTION_TYPE))
#define is_an_implicit_function(t,p) \
        (((t)==TK_ID)&& \
         (p!=((identifier_info_type *) NULL))&& \
         (((*p).flags & FUNC_IMPL_MASK)==IMPLICIT_TYPE))
#define is_a_numeric_item(idptr) \
         (((*idptr).flags & COMPUTATIONAL_TYPE_MASK)>=INT_TYPE)
#define is_a_boolean_item(idptr) \
         (((*idptr).flags & COMPUTATIONAL_TYPE_MASK)==BOOL_TYPE)
/*===========================================================================*/
#define PARSE_VALUE(item) \
        save_infix(OP_VAL); \
        save_operand(this.id_info_ptr); \
        save_postfix(OP_VAL); \
        item=(*this.id_info_ptr).flags
#define PARSE_VALUE_1() \
        PARSE_VALUE(bopi.item[0].type); \
        AFTER_PARSE_1(bopi)
#define PARSE_VALUE_2() \
        PARSE_VALUE(bopi.item[1].type); \
        AFTER_PARSE_2(bopi)
#define AFTER_PARSE_1(bopi) \
        if (adjust_after_parse(&bopi.item[0])) is_var=TRUE
#define AFTER_PARSE_2(bopi) \
        if (adjust_after_parse(&bopi.item[1])) is_var=TRUE
#define DO_PARSE_1(rou,bopi,msg) \
        bopi.item[0].type=recurse_the_parse(rou,msg); \
        AFTER_PARSE_1(bopi)
#define DO_PARSE_2(rou,bopi,msg) \
        bopi.item[1].type=recurse_the_parse(rou,msg); \
        AFTER_PARSE_2(bopi)
#define DEFAULT_ANS_AND_VARIABLE_STATUS() \
        bopi.ans = bopi.item[0].comp; \
        bopi.spcans = bopi.item[0].spec; \
        is_var = (bitset(bopi.spcans,ANY_SPECIAL_VAR_TYPE_MASK))
#define PARSE_SUBSCRIPT_IF_PRESENT() \
        iz_arry = bitset((*prev.id_info_ptr).flags,ARRAY_TYPE); \
        if ((this.tok==TK_LP) && (iz_arry)) \
           error_this_pos(E_INDEXEDBYPARENS); \
        else if (this.tok==TK_LB){ \
           DO_PARSE_2(parse_subscript,bopi,"parse_subscript");} \
        else if (iz_arry) \
           show_tail_err_at_pos_on_both(E_NOTSCALAR,this.pos, \
                                        (*prev.id_info_ptr).name,CHAR_TYPE, \
                                        this.id)
#ifdef REDUNDANT_I_THINK
        if ((!reals_allowed)&&(!reals_in_error_shown)&& 
            (bopi.ans & COMPUTATIONAL_TYPE_MASK)==REAL_TYPE){ 
           error_this_pos(E_NOREALS); 
           reals_in_error_shown=TRUE;} 
#endif
#define adjust_ans_variable_status() \
        if (is_var) bopi.ans |= EXPR_VARIABLE_TYPE
/*===========================================================================*/
#define SAVE_OP_AND_ADVANCE_TOKEN() \
        op_chix=save_operator_and_advance_1_tok(&saved)
#define PROMOTE_AND_STORE_POSTFIX(op) \
        promote_and_save(&bopi,op_chix,op,TK_NULL)
#define SAVE_AND_RESTORE_THIS_VARS token_info_type sv_this
#define save_int_if_not_yet_in_num_tab(w) \
        if (w == ((void *) NULL)) \
            w = save_value_in_number_table(&std_null_value,INT_TYPE)
#define save_real_if_not_yet_in_num_tab(w) \
        if (w == ((void *) NULL)) \
            w = save_value_in_number_table(&std_real_value,REAL_TYPE)
/*===========================================================================*/
#endif
