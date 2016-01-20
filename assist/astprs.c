/*
===============================================================================

        System Dependent functions required by ASSIST and SURE compiler
        programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

#include "common.h"
#include "lio_ext.h"
#include "io_ext.h"

#include "astrw.h"

#include "objdefs.h"
#include "objtypes.h"
#include "objvars.h"

#include "astdefs.h"
#include "asttypes.h"
#include "astvars.h"

#include "macvars.h"

#include "astr_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "fix_ext.h"
#include "obj_ext.h"
#include "mac_ext.h"
#include "aprs_ext.h"

#include "asttabsP.h"

#define MAX_SSV_COUNT_PER_NESTING_LEVEL 200
static Boolean flushed=FALSE;
static char ovf_msg[] =
       "BUFFER OVERFLOW WHEN PARSING SPACE PICTURE.  TRY USING 200 OR FEWER STATE-SPACE VARIABLES PER NESTING LEVEL.";

#define correct_section(nowmode,section_mode) \
        bitset(((nowmode) & MODE_SECT_MASK),((section_mode) & MODE_SECT_MASK))
/*===========================================================================*/
static Boolean option_def_no_skip = FALSE;
static short start_linnum;
static Boolean optnal_flg_set_eq_sign = FALSE;
static Boolean doing_start_statement;
static Boolean space_statement_missing;
static Boolean start_statement_missing;
static Boolean death_present;
static Boolean tranto_present;
static mode_flag_type this_mode;
static char *storage_arr;
static char *aux_storage_arr;
static Boolean *storage_nested;
static vars_union_type *storage_vars_ptrs;
static expression_type *storage_exprs;
static identifier_info_type **storage_idptrs;
static operation_type *storage_scrops;
static operand_type *storage_scropnds;
static instruction_type *storage_varinsts;
static calc_assign_type *storage_varcalcs;
static Subscript *storage_ixs;/* doubles for both ARRAY[a..n+a-1] and "n OF" */
static node_union_type *storage_nodes;
static relative_address_type *rule_instr_byteoffs;
static relative_address_type reladdr_start;
static opcode_type *rule_opcode_arr;
static Memix max_expect_ops_count=0;
static Memix max_expect_opnds_count=0;
static Memix max_many_ops_count=0;
static Memix max_many_opnds_count=0;
static Memix pix_node_ele_count;
static Memix pix_arr_ele_count;
static Memix rule_count;
static Memix scrops_count;
static Memix scropnds_count;
static Memix ixs_count;
static Memix idptrs_count;
static Memix exprs_count;
static Memix varinsts_count;
static long rule_opcode_sec_counts[RULE_OPCODE_INDEX_COUNT];
static relative_address_type cumpc[RULE_OPCODE_INDEX_COUNT];
static short rule_seq_ixs[RULE_OPCODE_INDEX_COUNT];
static identifier_info_type *space_var_list;
static space_expression_type this_sex;
static Memix prev_nele_identifier_table;
static expression_type expr_just_stored;
static token_info_type tok_saved_during_switch[SAVE_LEVELS];
/**/
#define space_pic (*state_space_picture_ptr)
/**/
char encoded_state_string[MAXNCH_ENCODED_STATE+1];
/**/
static char *real_tags[] =    { "Real constants", "Real variables" };
static char *integer_tags[] = { "Integer constants", "Integer variables" };
static char *Boolean_tags[] = { "Boolean constants", "Boolean variables" };
static char *soff_tags[] = {"State offset constants","State offset variables"};
static char *char_tags[] =    { "Character constants", "Character variables" };
/*===========================================================================*/

              /**********************************************/
              /**********************************************/
              /****                                      ****/
              /****   MISCELLANEOUS LOW-LEVEL ROUTINES   ****/
              /****                                      ****/
              /**********************************************/
              /**********************************************/

/*===========================================================================*/
#define reset_optional_equal_sign() optnal_flg_set_eq_sign=FALSE
#define set_optional_equal_sign() optnal_flg_set_eq_sign=TRUE
#define DIVIDE_PIX_STORAGE(vvv,typ) \
   vvv=(typ *) ppp; ppp += (((Memix) max_spacepix_count)*((Memix) sizeof(typ)))
#define DIVIDE_XPR_STORAGE(vvv,typ) \
   vvv=(typ *) ppp; ppp += (((Memix) max_expr_count)*((Memix) sizeof(typ)))
#define DIVIDE_XXPR_STORAGE(vvv,count,typ) \
   vvv=(typ *) ppp; ppp += (((Memix) max_expr_count) * \
                            ((Memix) count) * \
                            ((Memix) sizeof(typ)))
#define store_preamble_code(r) \
        store_in_object_file(OBJ_CODE_0,(void *) &r,sizeof(instruction_type),1)
#define STORE_CODE(r,ixkod) \
        cumpc[ixkod] += ((relative_address_type) (sizeof(instruction_type))); \
        store_in_object_file((object_table_type) (OBJ_CODE_0+1+ixkod), \
                             (void *) &r,sizeof(instruction_type),1)
#define store_varinf_data(dptr,n) \
        store_in_object_file(OBJ_VARINF,(void *) dptr, \
                             sizeof(vars_union_type),n)
#define store_expression_data(d,n) \
        store_in_object_file(OBJ_EXPR,(void *) &d,sizeof(expression_type),n)
#define store_picture_data(dptr,n) \
        store_in_object_file(OBJ_PIX,(void *) dptr, \
                             sizeof(state_space_picture_type),n)
#define store_booltest_data(d,n) \
        store_in_object_file(OBJ_BOOLTEST,(void *) &d,sizeof(booltest_type),n)
#define store_tranto_data(d,n) \
        store_in_object_file(OBJ_TRANTO,(void *) &d, \
                             sizeof(tranto_clause_type),n)
#define store_calc_data(d,n) \
        store_in_object_file(OBJ_CALC,(void *) &d, \
                             sizeof(calc_assign_type),n)
#define store_blockif_data(d,n) \
        store_in_object_file(OBJ_IF,(void *) &d,sizeof(block_if_type),n)
#define store_forloop_data(d,n) \
        store_in_object_file(OBJ_FOR,(void *) &d,sizeof(for_loop_type),n)
#define store_setrange_data(d,n) \
        store_in_object_file(OBJ_SETRNGE,(void *) &d,sizeof(set_range_type),n)
#define store_string_data(s) \
        store_in_object_file(OBJ_CHAR_DATA,(void *) (s), \
                             (size_t) (1+strlen(s)),1)
#define store_ops_data(d,n) \
        store_in_object_file(OBJ_OPS,(void *) (d),sizeof(operation_type),n)
#define store_scalar_data(o,d,type) \
        store_in_object_file(o,(void *) &d,sizeof(type),1)
#define store_array_data(o,d,type,n) \
        store_in_object_file(o,(void *) d,sizeof(type),n)
/*===========================================================================*/
relative_address_type store_operand_data(operand_type *d,short n)
{
   relative_address_type reladdr,ans;
   short i,iput,nput;
#ifdef BUGGY
short jj;
#endif
   operand_type tarr[64];
   /*-------------------------------------------------------*/
   nput=64;
   ans=NIX_ADDR;
   for (i=0;i<n;i+=64){
      if ((i+nput)>n) nput=n-i;
      for (iput=0;iput<nput;++iput){
         tarr[iput] = (operand_type) relative_identifier(d[i+iput]);}
      reladdr = store_in_object_file
         (OBJ_OPERANDS,(void *) tarr,sizeof(operand_type),nput);
      if (i==0) ans=reladdr;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void assist_alloc_for_parse()
/*
   This function allocates enough memory for the parse stage of the
   program execution.   The limits can be controled via the following
   command line options:
               MSDOS         UNIX                  MEANING
            ---------     -----------    -------------------------------------
             /PIC=n         -PIC=n       space <PIC>ture count limit
              /E=n           -E=n        scratch expression/array count limit
*/
{
   long total,xtotal;
   memsize_t bytes;
   char *ppp;
   /*-------------------------------------------------------*/
   must_assist_alloc_for_parse=FALSE;
   sys_memset(rule_seq_ixs,0,(size_t) (RULE_OPCODE_INDEX_COUNT*sizeof(short)));
   doing_start_statement=FALSE;
   reset_optional_equal_sign();
   if (!pipe_model_to_stdout){
#if defined(MSDOS)
      modfile=open_output_file(mod_filename);
#else
      modfile=open_output_using_big_buffer(mod_filename,(size_t) 0x800);
#endif
      outputfile=stdout;
   }
   allocate_identifier_table(max_identifier_table_alloc);
   max_expect_ops_count=max_ops_count/4;
   max_expect_opnds_count=max_opnd_count/4;
#ifdef MSDOS
   if ((((long) (2*max_expr_count))*((long) max_expect_ops_count))>
       (((long) MAXSHORT))){
      show_err_at_pos_on_both(E_EXPROVF,0,
                              "COMMAND LINE OPTIONS TOO BIG.",CHAR_TYPE);
      show_err_at_pos_on_both(E_EXPRSTKOVF,0,
                              "COMMAND LINE OPTIONS TOO BIG.",CHAR_TYPE);
      abort_program();}
#endif
   max_many_ops_count=2*max_expect_ops_count;
   max_many_opnds_count=max_expect_opnds_count;
   function_body_storage =
       allocate_for_parse_stage(max_body_count,max_nest_count,max_parm_count,
                                max_ops_count,max_opnd_count,
                                &function_body_count,&macro_stack_top,
                                &macro_stack);
   total = sizeof(Boolean) +
           sizeof(vars_union_type) +
           sizeof(node_union_type);
   xtotal = sizeof(expression_type) +
            sizeof(identifier_info_type *) +
            sizeof(instruction_type) +
            ((long) max_many_ops_count)*((long) sizeof(operation_type)) +
            ((long) max_many_opnds_count)*((long) sizeof(operand_type));
   total *= (long) max_spacepix_count;
   xtotal *= (long) max_expr_count;
#ifdef BUGGY
fprintf(stderr,"total bytes=%d\n",total);
fprintf(stderr,"x-total bytes=%d\n",xtotal);
#endif
   total += xtotal;

/* #ifdef BUGGY */
 fprintf(stderr,"grand-total bytes=%d\n",total); 
/* #endif */

   test_total_for_fits(total);
   bytes=(memsize_t) total;
   ppp = storage_arr = (char *) g_malloc(bytes);
   if (storage_arr == ((char *) NULL)){
      show_err_on_both(E_NOMEMORYP,"(astparse storage)",CHAR_TYPE);
      abort_program();}
   DIVIDE_PIX_STORAGE(storage_nodes,node_union_type);
   DIVIDE_PIX_STORAGE(storage_vars_ptrs,vars_union_type);
   DIVIDE_PIX_STORAGE(storage_nested,Boolean);
   DIVIDE_XPR_STORAGE(storage_exprs,expression_type);
   DIVIDE_XPR_STORAGE(storage_idptrs,identifier_info_type *);
   DIVIDE_XPR_STORAGE(storage_ixs,Subscript);
   DIVIDE_XXPR_STORAGE(storage_scrops,max_many_ops_count,operation_type);
   DIVIDE_XXPR_STORAGE(storage_scropnds,max_many_opnds_count,operand_type);
   DIVIDE_XPR_STORAGE(storage_varinsts,instruction_type);
   DIVIDE_XPR_STORAGE(storage_varcalcs,calc_assign_type);
   varinsts_count=rule_count=scrops_count=scropnds_count=
   ixs_count=idptrs_count=exprs_count=pix_node_ele_count=pix_arr_ele_count=0;
   /*-------------------------------------------  adjust final count */
   max_many_ops_count *= max_expr_count;
   max_many_opnds_count *= max_expr_count;
#ifdef BUGGY
fprintf(stderr,"max_many_ops_count=%d\n",(int) max_many_ops_count);
fprintf(stderr,"max_many_opnds_count=%d\n",(int) max_many_opnds_count);
#endif
   /*-------------------------------------------  now allocate for rules */
   total=((long) max_rule_count)*
         ((long) (sizeof(opcode_type) +
                  sizeof(instruction_pointer_union_type)));
   bytes=(memsize_t) total;
   ppp = aux_storage_arr = (char *) g_malloc(bytes);
   g_malloc_abort_if_error((void *) ppp);
   /*-------------------------------------------  divy up for rules */
   rule_instr_byteoffs = (relative_address_type *) ppp;
   ppp += ((Memix) max_rule_count) *
          ((Memix) sizeof(instruction_pointer_union_type));
   rule_opcode_arr = (opcode_type *) ppp;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void assist_finish_varinsts()
{
   short kkk;
   Memix ixid;
   /*-------------------------------------------------------*/
   for (kkk=0;kkk<varinsts_count;++kkk){
      ixid = relative_identifier(storage_varcalcs[kkk].idinfo);
#ifdef OBSOLETE
/*??????*/ printf("varcalcs[%d]=\"%s\"  utab[ixid=%d]=0x%02x\n",(int) kkk,(*storage_varcalcs[kkk].idinfo).name,(int) ixid,(unsigned int) usage_table[ixid]);
#endif
      if (bitset(usage_table[ixid],USAGE_IN_TST)){
         ++rule_opcode_sec_counts[OPCODE_CALC];
         STORE_CODE(storage_varinsts[kkk],OPCODE_CALC);}
      if (bitset(usage_table[ixid],USAGE_IN_TTO)){
         ++rule_opcode_sec_counts[OPCODE_CALC_T];
         STORE_CODE(storage_varinsts[kkk],OPCODE_CALC_T);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void assist_store_space_and_misc()
{
   instruction_type inst;
   /*-------------------------------------------------------*/
   if (state_space_picture_ptr!=((state_space_picture_type *) NULL))
      qq_store_space_picture(&state_space_picture_ptr);
   store_in_object_file(OBJ_OPREC,(void *) &option_rec,sizeof(option_rec),1);
   inst.opcode=OPCODE_SPACE;
   inst.ptr.reladdr = (relative_address_type) state_space_picture_ptr;
   store_preamble_code(inst);
   inst.opcode=OPCODE_START;
   inst.ptr.reladdr = reladdr_start;
   store_preamble_code(inst);
   inst.opcode=OPCODE_END;
   inst.ptr.reladdr = NIX_ADDR;
   store_preamble_code(inst);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void assist_free_after_parse()
{
   /*-------------------------------------------------------*/
   g_free(storage_arr);
   g_free(aux_storage_arr);
   aux_storage_arr=storage_arr=(char *) NULL;
   storage_nodes=(node_union_type *) NULL;
   storage_vars_ptrs=(vars_union_type *) NULL;
   storage_nested=(Boolean *) NULL;
   storage_exprs=(expression_type *) NULL;
   storage_idptrs=(identifier_info_type * *) NULL;
   storage_ixs=(Subscript *) NULL;
   storage_scrops=(operation_type *) NULL;
   storage_scropnds=(operand_type *) NULL;
   rule_instr_byteoffs = (relative_address_type *) NULL;
   rule_opcode_arr = (opcode_type *) NULL;
   varinsts_count=rule_count=scrops_count=scropnds_count=
   ixs_count=idptrs_count=exprs_count=pix_node_ele_count=pix_arr_ele_count=0;
   free_after_parse_stage(&macro_stack,&function_body_storage);
   free_identifier_table();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void qq_store_space_picture(state_space_picture_type **pixptr)
{
   Memix iii,nnn;
   vars_union_type *varptr;
   relative_address_type reladdr;
   /*-------------------------------------------------------*/
#ifdef PTRBUGGY
fprintf(bugfile,"storing picture: ");
bug_print_space_picture(*pixptr);
#endif
   for (nnn=(**pixptr).nvaru,iii=0;iii<nnn;++iii){
      varptr=(**pixptr).varu+iii;
      if (*((**pixptr).is_nested+iii)){
         qq_store_space_picture(&(*varptr).nested_space_picture);}
      else{
         reladdr = relative_identifier((*varptr).id_info);
         reladdr |= RELADDR_FLAG;
#ifdef PTRBUGGY
fprintf(bugfile,"\nidentifier_table=%d@%d@%08lx, idinf=%08lx\n",
(int) max_identifier_table_count,
(int) nele_identifier_table,
(long) identifier_table,
(long) (*varptr).id_info);
fprintf(bugfile,"relid=%x\n",(int) reladdr);
#endif
         (*varptr).id_info = (identifier_info_type *) reladdr;}}
   (**pixptr).varu = (vars_union_type *)
             store_varinf_data((**pixptr).varu,nnn);
   (**pixptr).is_nested = (Boolean *)
      store_in_object_file(OBJ_BOOL_DATA,(**pixptr).is_nested,
                           sizeof(Boolean),nnn);
   *pixptr = (state_space_picture_type *) store_picture_data(*pixptr,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void store_expr_data_only(expression_type *expr_p)
/*
   This function stores an expression and its operation and operand lists
   into the object file.   The addresses of the operation and operand lists
   are changed to pointer difference units and cast back to their respective
   types.
*/
{
   /*-------------------------------------------------------*/
   sys_memcpy(&expr_just_stored,expr_p,sizeof(expression_type));
   expr_just_stored.postfix_ops = (operation_type *)
      store_ops_data(expr_just_stored.postfix_ops,
                     expr_just_stored.n_postfix_ops);
   expr_just_stored.infix_ops = (operation_type *)
      store_ops_data(expr_just_stored.infix_ops,
                     expr_just_stored.n_infix_ops);
   expr_just_stored.operands = (operand_type *)
      store_operand_data(expr_just_stored.operands,
                         expr_just_stored.n_operands); 
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void print_constant_definition(FILE *fp,char *id,expression_type *expr)
{
   short indentamt;
   /*-------------------------------------------------------*/
   fprintf(fp,"%s = ",id);
   indentamt=(short) (3+strlen(id));
   if ((*expr).in_error)
      fprintf(fp,"???");
   else{
      print_infix_expr(fp,expr,indentamt,indentamt,80,TRUE,
                          wrap_long_constant_expressions);}
   fprintf(fp,";\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
relative_address_type store_expr_struct_only()
{
   register relative_address_type byte_offset;
   /*-------------------------------------------------------*/
   byte_offset=store_expression_data(expr_just_stored,1);
   /*-------------------------------------------------------*/
   return(byte_offset);
}
/*===========================================================================*/
relative_address_type store_this_expr()
{
   register relative_address_type byte_offset;
   /*-------------------------------------------------------*/
   store_expr_data_only(&this_expr);
   byte_offset=store_expr_struct_only();
   /*-------------------------------------------------------*/
   return(byte_offset);
}
/*===========================================================================*/
relative_address_type store_given_expr(expression_type *expr_p)
{
   register relative_address_type byte_offset;
   /*-------------------------------------------------------*/
   store_expr_data_only(expr_p);
   byte_offset=store_expr_struct_only();
   /*-------------------------------------------------------*/
   return(byte_offset);
}
/*===========================================================================*/
Memix increment_scrops_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=scrops_count;
   scrops_count += ((Memix) count);
   if (scrops_count > max_many_ops_count){
      error_this_pos(E_EXPROVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_scropnds_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=scropnds_count;
   scropnds_count += ((Memix) count);
   if (scropnds_count > max_many_opnds_count){
      error_this_pos(E_EXPROVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_ixs_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=ixs_count;
   ixs_count += ((Memix) count);
   if (ixs_count > max_expr_count){
      error_this_pos(E_EXPROVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_idptrs_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=idptrs_count;
   idptrs_count += ((Memix) count);
   if (idptrs_count > max_expr_count){
      error_this_pos(E_EXPROVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_exprs_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=exprs_count;
   exprs_count += ((Memix) count);
   if (exprs_count > max_expr_count){
      error_this_pos(E_EXPROVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_pixarr_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=pix_arr_ele_count;
   pix_arr_ele_count += ((Memix) count);
   if (pix_arr_ele_count > max_spacepix_count){
      error_this_pos(E_SPACEPIXOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_node_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=pix_node_ele_count;
   pix_node_ele_count += ((Memix) count);
   if (pix_node_ele_count > max_spacepix_count){
      error_this_pos(E_SPACEPIXOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_rule_ele(short count)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=rule_count;
   rule_count += ((Memix) count);
   if (rule_count > max_rule_count){
      error_this_pos(E_RULEOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix store_scratch_exprs(Memix ixexpr0,Memix ixidptrs0,
                          relative_address_type *radexprs,
                          relative_address_type *radidptrs,
                          ident_usage_flag_type usg)
{
   Memix ixexpr,diff;
   expression_type *expr_p;
   relative_address_type temp,ans;
   Boolean with_id;
   short n;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"storing scratch expressions:::\n");
#endif
   diff=0;
   with_id = (Boolean) (ixidptrs0<idptrs_count);
   expr_p=storage_exprs+ixexpr0;
   ans=NIX_ADDR;
   for (ixexpr=ixexpr0;ixexpr<exprs_count;++ixexpr){
      touch_expr(expr_p,usg);
      temp=store_given_expr(expr_p++);
      if (ixexpr==ixexpr0) ans=temp;}
   *radexprs=ans;
   *radidptrs=NIX_ADDR;
   if (with_id){
      diff=(idptrs_count-ixidptrs0);
      n=give_short(diff,E_EXPRSTKOVF);
      *radidptrs=store_operand_data(storage_idptrs+ixidptrs0,n);}
   /*-------------------------------------------------------*/
   return(diff);
}
/*===========================================================================*/
void remember_expr_ixs(Memix *ixscrops,
                       Memix *ixscropnds,
                       Memix *ixidptrs,
                       Memix *ixexprs)
{
   /*-------------------------------------------------------*/
   *ixscrops = scrops_count;
   *ixscropnds = scropnds_count;
   *ixidptrs = idptrs_count;
   *ixexprs = exprs_count;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void restore_expr_ixs(Memix ixscrops,
                      Memix ixscropnds,
                      Memix ixidptrs,
                      Memix ixexprs)
{
   /*-------------------------------------------------------*/
   scrops_count = ixscrops;
   scropnds_count = ixscropnds;
   idptrs_count = ixidptrs;
   exprs_count = ixexprs;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void append_expr_to_this(expression_type *expr)
{
   /*-------------------------------------------------------*/
   sys_memcpy(this_expr.postfix_ops+this_expr.n_postfix_ops,
              (*expr).postfix_ops,
              (size_t) ((*expr).n_postfix_ops*sizeof(operation_type)));
   sys_memcpy(this_expr.infix_ops+this_expr.n_infix_ops,
              (*expr).infix_ops,
              (size_t) ((*expr).n_infix_ops*sizeof(operation_type)));
   sys_memcpy(this_expr.operands+this_expr.n_operands,
              (*expr).operands,
              (size_t) ((*expr).n_operands*sizeof(operand_type)));
   this_expr.n_postfix_ops += (*expr).n_postfix_ops;
   this_expr.n_infix_ops += (*expr).n_infix_ops;
   this_expr.n_operands += (*expr).n_operands;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void save_expr_into_scratch(expression_type *expr,identifier_info_type *idinfo)
{
   Memix ixpost,ixin,ixopnd,ixexpr;
   Memix ixid=0;
   expression_type *new_expr;
   Boolean with_ident;
   /*-------------------------------------------------------*/
   with_ident=(Boolean) (idinfo != ((identifier_info_type *) NULL));
   ixpost=increment_scrops_ele((*expr).n_postfix_ops);
   ixin=increment_scrops_ele((*expr).n_infix_ops);
   ixopnd=increment_scropnds_ele((*expr).n_operands);
   ixexpr=increment_exprs_ele(1);
   if (with_ident) ixid=increment_idptrs_ele(1);
   sys_memcpy(storage_scrops+ixpost,(*expr).postfix_ops,
              (size_t) ((*expr).n_postfix_ops*sizeof(operation_type)));
   sys_memcpy(storage_scrops+ixin,(*expr).infix_ops,
              (size_t) ((*expr).n_infix_ops*sizeof(operation_type)));
   sys_memcpy(storage_scropnds+ixopnd,(*expr).operands,
              (size_t) ((*expr).n_operands*sizeof(operand_type)));
   new_expr=storage_exprs+ixexpr;
   sys_memcpy(new_expr,expr,sizeof(expression_type));
   if (with_ident) *(storage_idptrs+ixid) = idinfo;
   (*new_expr).postfix_ops=storage_scrops+ixpost;
   (*new_expr).infix_ops=storage_scrops+ixin;
   (*new_expr).operands=storage_scropnds+ixopnd;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/void save_this_scratch_expr(identifier_info_type *idinfo)
{
   /*-------------------------------------------------------*/
   save_expr_into_scratch(&this_expr,idinfo);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#ifdef BUGGY
int bug_node_number(node_union_type *nodeptr)
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans=(nodeptr-storage_nodes);
   /*-------------------------------------------------------*/
   return((int) ans);
}
#endif
/*===========================================================================*/
short max_encoded_space_bits_strlen()
{
   register short nchars;
   register state_space_picture_type *pixptr;
   /*-------------------------------------------------------*/
   pixptr = state_space_picture_ptr;
   nchars = qq_max_enk_space_bits_strlen(pixptr);       /* recursive version */
   nchars += 4;   /*  for "(* " and " *)"  less "(" and ")" accounted for by */
   /*-------------------------------------------------------*/
   return(nchars);
}
/*===========================================================================*/
short qq_max_enk_space_bits_strlen(state_space_picture_type *pixptr)
{
   short ixvr;
   identifier_info_type *idinfo;
   vars_union_type *vrptr;
   short nchars,nc;
   char xxxbuff[22];
   /*-------------------------------------------------------*/
   nchars=1+(*pixptr).nvaru;   /*  for "(" and "," ... and  ")"   */
   for (ixvr=0;ixvr<(*pixptr).nvaru;++ixvr){
      vrptr = (*pixptr).varu + ixvr;
      if (*((*pixptr).is_nested+ixvr))
         nchars +=
            qq_max_enk_space_bits_strlen((*vrptr).nested_space_picture);
      else{
         idinfo = (*vrptr).id_info;
         sprintf(xxxbuff,"%d",(int) (*(*idinfo).ptr.sss).maxval);
         nc = 1+strlen(xxxbuff);
         nc *= (short) dim_range((*idinfo).index.dims.first);
         nc *= (short) dim_range((*idinfo).index.dims.second);
         --nc;
#if defined(FUTURE_MAYBE) && defined(NEGATIVE_SS_VALS_ALLOWED)
         if ((*(*idinfo).ptr.sss).minval<0) ++nchars;
#endif
         nchars += nc;}}
   /*-------------------------------------------------------*/
   return(nchars);
 }
/*===========================================================================*/
char *encode_space_bits(state_bitstring_type bits)
{
   short nch;
   /*-------------------------------------------------------*/
   *encoded_state_string='\0';
   nch=qq_encode_space_bits(bits,state_space_picture_ptr,0);
   /*-------------------------------------------------------*/
   return(encoded_state_string);
}
/*===========================================================================*/
short qq_encode_space_bits
       (state_bitstring_type bits,state_space_picture_type *pixptr,short pos)
{
   short ixvar;
   vars_union_type *varptr;
   identifier_info_type *idinf;
   type_flagword_type comp;
   Subscript ix,jx,off;
   ssvar_value_type sval;
   short newpos;
   char tmpenk[MAX_INT_TYPE_NDIGS_P];
   /*-------------------------------------------------------*/
   newpos=pos;
   strcpy(encoded_state_string+newpos,STRLP);
   newpos++;
   for (ixvar=0;ixvar<(*pixptr).nvaru;++ixvar){
      if (ixvar>0){
         strcpy(encoded_state_string+newpos,",");
         newpos++;}
      varptr = (*pixptr).varu + ixvar;
      if (*((*pixptr).is_nested+ixvar))
         newpos =
             qq_encode_space_bits(bits,(*varptr).nested_space_picture,newpos);
      else{
         idinf = (*varptr).id_info;
         comp = (*idinf).flags & COMPUTATIONAL_TYPE_MASK;
         off = 0;
         for_all_dims(ix,(*idinf).index.dims.first){
             for_all_dims(jx,(*idinf).index.dims.second){
                if (off>0){
                   strcpy(encoded_state_string+newpos,",");
                newpos++;}
                sval=state_space_arr_value(bits,(*idinf).ptr.sss,off);
                if (comp==BOOL_TYPE){
                   strcpy(encoded_state_string+newpos,((sval!=0) ? "T" : "F"));
                   newpos++;}
                else{
                   sprintf(tmpenk,"%d",(int) sval);
                   strcpy(encoded_state_string+newpos,tmpenk);
                   newpos += strlen(tmpenk);}
             off++;}}}}
   strcpy(encoded_state_string+newpos,STRRP);
   newpos++;
   /*-------------------------------------------------------*/
   return(newpos);
}
/*===========================================================================*/
void print_space_picture(FILE *fp,state_space_picture_type *pixptr)
{
   Memix iii,nnn;
   vars_union_type *varptr;
   identifier_info_type *idinf;
   state_offset_type *offinf;
   char extra[34];
   Boolean is_array,is_double;
   /*-------------------------------------------------------*/
#ifdef BUGGY
   fprintf(fp,"[%08lx]",(long) pixptr);
   fprintf(fp,"{%d}",bug_node_number((node_union_type *) pixptr));
#endif
   fprintf(fp,"%c",OPENPAREN);
   for (nnn=(*pixptr).nvaru,iii=0;iii<nnn;++iii){
      if (iii>0) fprintf(fp,",");
      varptr=(*pixptr).varu+iii;
      if (*((*pixptr).is_nested+iii))
         bug_print_space_picture((*varptr).nested_space_picture);
      else{
         idinf = (*varptr).id_info;
#ifdef BUGGY
fprintf(fp,"{%d}",(int) relative_identifier(idinf));
#endif
         fprintf(fp,"%s",(*idinf).name);
         DECIDE_IS_ARRAY_IS_DOUBLE(idinf);
         if (is_array){
            *extra='\0';
            if (is_double)
               sprintf(extra,",%d..%d",
                       (int) (*idinf).index.dims.second.lower,
                       (int) (*idinf).index.dims.second.upper);
            fprintf(fp,"[%d..%d%s]",
                    (int) (*idinf).index.dims.first.lower,
                    (int) (*idinf).index.dims.first.upper,extra);}
         offinf = (*(*varptr).id_info).ptr.sss;
         fprintf(fp,":%d..%d,%d@%d",
                 (int) (*offinf).minval,(int) (*offinf).maxval,
                 (int) (*offinf).bit_length,(int) (*offinf).bit_offset);}}
   fprintf(fp,"%c",CLOSEPAREN);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/

                     /*******************************/
                     /*******************************/
                     /****                       ****/
                     /****   STATEMENT PARSING   ****/
                     /****                       ****/
                     /*******************************/
                     /*******************************/

/*===========================================================================*/
void parse_constant_definition_stat()
/*
   This function parses a constant definition statement.  Upon entry, the
   current token must be the identifier being defined.   Upon exit, it will
   be the semicolon which terminated the statement.
*/
{
   token_info_type saved;
   short op_chix;
   Boolean is_bool;
   char tmpmsg[90];
   /*-------------------------------------------------------*/
   if (!correct_section(this_mode,ANY_PRE_RULE_SECTION_MODE)){
      sprintf(tmpmsg,"(constant definition of %s = <expr>)",this.id);
      show_err_at_pos_on_both(E_WRONGSECTION,this.pos,tmpmsg,CHAR_TYPE);}
   op_chix=save_operator_and_advance_1_tok(&saved);
   if (this.tok==TK_EQ) advance_one_token();
   else show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,"=",CHAR_TYPE);
   if (is_bool=this_tok_is_rw(RW_BOOLEAN)) advance_one_token();
   parse_constant_def_clause(saved.id,saved.pos,is_bool,EMPTY_TYPE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_constant_def_clause(char *id,short pos,Boolean is_bool,
                               type_flagword_type extra_types)
/*
   This function parses a constant defintion clause of one of the following
   formats:
              ARRAY ( <list-of-expressions> ) ;
              [ <list-of-expressions> ] ;
              [ <list-of-expressions> ] ... [ <list-of-expressions> ] ;
              <single-expression> ;

   The parsing occurs in two stages so that arrays will occupy contiguous
   storage and to prevent the user from inadvertantly using a recursive
   array declaration such as:
             OMEGAS = [ 6.5*LAMBDA, 2.2*OMEGAS[3], 1.0-LAMBDA ];
   All three expressions are parsed and saved before any of them are
   evaluated and the identifiers are declared.   The above will therefore
   result in an error message that OMEGAS[3] is not yet defined.   The user
   must rewrite the above as follows:
             LAMINV = 1.0 - LAMBDA;
             OMEGAS = [ 6.5*LAMBDA, 2.2*LAMINV, LAMINV ];
*/
{
   type_flagword_type comp,promotedcomp,evalcomp;
   Boolean is_array,is_double,is_parenkind,err,first,constant;
   token until_tok;
   short ee,ixof,of_count,knt,bgn_pos;
   Memix iii,count,cumcount;
   Memix times,itime,maxcount,ixsub,ixixs,prev_dim,curr_dim;
   value_union_type value;
   void *vvv;
   void *oldvvv;
   identifier_info_type *idinfo;
   identifier_info_type *scratch_idinfo;
   char scratch_id[IDENT_MAXNCH_P];
   char *until_str;
   expression_type *now_expr;
   Subscript *now_ix;
   relative_address_type reladdr;
   Memix holdops,holdopnds,holdidptrs,holdexprs;
   Memix holdixs;
   calc_assign_type varcalc;
   /*-------------------------------------------------------*/
   prev_dim=(-1);
   constant = (Boolean) (extra_types==EMPTY_TYPE);
   /*---------------------------------------  save expression state */
   remember_expr_ixs(&holdops,&holdopnds,&holdidptrs,&holdexprs);
   holdixs=ixs_count;
   is_array=is_parenkind=FALSE;
   until_tok=TK_SEMI;
   maxcount=1;
   bgn_pos=this.pos;
   /*---------------------------------------  maybe start an array */
   if ((this.tok==TK_RW)&&(this.rw==RW_ARRAY)){
      maxcount=32000;
      is_parenkind=is_array=TRUE;
      until_tok=TK_RP;
      until_str=STRRP;
      advance_one_token();
      if (this.tok==TK_LP) advance_one_token();
      else error_this_pos(E_NOTOPENPAREN);}
   else if (this.tok==TK_LB){
      until_tok=TK_RB;
      until_str=STRRB;
      maxcount=32000;
      is_array=TRUE;
      advance_one_token();}
   else until_str=";";
   is_double = (Boolean) (this.tok==TK_LB);

                 /*******************************************/
                 /*******************************************/
                 /****                                   ****/
                 /****   PARSE THE CONSTANT DEFINITION   ****/
                 /****                                   ****/
                 /*******************************************/
                 /*******************************************/

   cumcount=0;
   comp=promotedcomp=EMPTY_TYPE;
   for (times=0,first=TRUE;
            (
               (first) ||
#ifdef I_DONT_KNOW
               (this_is_not(until_tok)) ||
#endif
               ((is_double)&&(this.tok==TK_COMMA))
            );
        first=FALSE,++times){
      if (this.tok==TK_COMMA){
         advance_one_token();
         if (first) error_this_pos(E_MISSINGEXPR);}
      while (this.tok==TK_COMMA){
         advance_one_token();
         error_this_pos(E_MISSINGEXPR);}
      if (is_double){
         if (this.tok==TK_LB) advance_one_token();
         else error_this_pos(E_NOTOPENBRACKET);}
      /*------------------------------------  parse list of 1 or more exprs */
      curr_dim=0;
      for (count=0;(count<maxcount)&&(this_is_not(until_tok));++count){
         if (count>0){
            if (this.tok==TK_COMMA) advance_one_token();
            else error_this_pos(E_NOTCOMMA);}
         comp=parse_simple_def_expr(&err,is_bool,is_array,&of_count);
         if (of_count==0) of_count=1;
         /*-----------------------  adjust type */
         if (promotedcomp==EMPTY_TYPE) promotedcomp=comp;
         /*-----------------------  error if type mismatch unless int-->real */
         if ((promotedcomp==REAL_TYPE)&&(comp==INT_TYPE));
         else if (promotedcomp!=comp){
            if ((promotedcomp==INT_TYPE)&&(comp==REAL_TYPE)) ee=E_CANNOTMIX;
            else ee=E_TYPEMISMATCH;
            set_arrow_sides(TRUE,FALSE);
            this_expr_error_this_pos(ee);
            reset_arrow_sides();}
         /*-----------------------  save expression & "n OF ..." count */
         ixixs=increment_ixs_ele(1);
         *(storage_ixs+ixixs)=(Subscript) of_count;
         curr_dim += ((Memix) of_count);
         save_this_scratch_expr((identifier_info_type *) NULL);}
      if (count<=0) error_this_pos(E_MISSINGEXPR);
      cumcount += count;
      /*------------------------------------  skip trailing array bracket */
      if (!first){
         if (curr_dim != prev_dim) error_this_pos(E_DIMINCONSISTANT);}
      if (curr_dim>prev_dim) prev_dim = curr_dim;
      if (until_tok==TK_SEMI);
      else if (this.tok==until_tok) advance_one_token();
      else{
         show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,until_str,CHAR_TYPE);
         skip_to_next_semicolon(FALSE);}}
   /*---------------------------------------  check closing double bracket */
   if ((is_double) && (this.tok==TK_RB)) advance_one_token();
   else if (is_double) error_this_pos(E_NOTCLOSEBRACKET);
   /*---------------------------------------  skip to semicolon */
   if (first) error_this_pos(E_MISSINGEXPR);
   if (this.tok!=TK_SEMI) error_this_pos(E_NOTSEMI);
   skip_to_next_semicolon(FALSE);

     /***********************************************************************/
     /***********************************************************************/
     /****                                                               ****/
     /****   DECLARE INDIVIDUAL NAMED CONSTANTS FROM SAVED EXPRESSIONS   ****/
     /****                                                               ****/
     /***********************************************************************/
     /***********************************************************************/

   /*------------------------------------  expand & evaluate expressions */
/*???*/ if (bitset(debug_flagword,DBG_LEX)) fprintf(stderr,"cumcount=%d\n",(int) cumcount);
   if (prev_dim<=0) prev_dim=1;
   curr_dim=prev_dim;
   oldvvv=(void *) NULL;
   for (iii=ixsub=0;iii<cumcount;++iii){
      now_expr=storage_exprs+holdexprs+iii;
      now_ix=storage_ixs+holdixs+iii;
      of_count=(short) (*now_ix);
/*???*/ if (bitset(debug_flagword,DBG_LEX)) fprintf(stderr,"iii=%d of_count=%d\n",(int) iii,(int) of_count);
      /*-----------------------  compute the value */
      if ((*now_expr).in_error || (!constant)){
         evalcomp=comp;
         standard_value(&value,evalcomp);}
      else{
         evalcomp=evaluate_expression(now_expr,&value,FALSE,0,-1,0,constant);
         if ((promotedcomp==REAL_TYPE)&&(evalcomp==INT_TYPE))
            value.rrr=(real_type) value.iii;
         else if (promotedcomp!=evalcomp)
            standard_value(&value,promotedcomp);}
/*???*/ if (bitset(debug_flagword,DBG_LEX)) fprintf(stderr,"of_count=%d\n",(int) of_count);
      for (ixof=0;ixof<of_count;++ixof,++ixsub){
         /*---------------  store the value */
         vvv=save_value_in_number_table(&value,promotedcomp);
         if (oldvvv==((void *) NULL)) oldvvv=vvv;
         /*---------------  declare ARR_* variables or simple identifier */
         if (is_double){
            itime=ixsub/curr_dim;
            count=ixsub%curr_dim;
            sprintf(scratch_id,"%s_%d_%d",id,(int) (1+itime),(int) (1+count));}
         else if (is_array) sprintf(scratch_id,"%s_%d",id,(int) (1+ixsub));
         else strcpy(scratch_id,id);
         xref_setvalue(scratch_id);
         scratch_idinfo=searchid_dcl(scratch_id,promotedcomp,pos);
         (*scratch_idinfo).flags |= extra_types;
         if (!id_unique_first_n(scratch_idinfo,SURE_CONSTANT_TRUNC_NCHARS))
            show_err_at_pos_on_both(E_SUREIDNOTUNIQUE,this.pos,scratch_id,
                                    CHAR_TYPE);
         knt=give_short(ixsub,E_EXPROPNDOVF);
         (*scratch_idinfo).ptr.vvv=arr_addr(oldvvv,knt,promotedcomp);
         /*---------------  echo to model file */
/*???*/ if (bitset(debug_flagword,DBG_LEX)) fprintf(stderr,"promotedcomp=%d\n",(int) promotedcomp);
         if (!constant){
            reladdr = store_given_expr(now_expr);
            varcalc.idinfo = (identifier_info_type *)
                 relative_identifier(scratch_idinfo);
            varcalc.expr = (expression_type *) reladdr;
            storage_varinsts[varinsts_count].opcode = OPCODE_CALC;
            storage_varinsts[varinsts_count].ptr.reladdr =
                 store_calc_data(varcalc,1);
            storage_varcalcs[varinsts_count].idinfo = scratch_idinfo;
            storage_varcalcs[varinsts_count].expr = now_expr;
            varinsts_count++;}
         else if ((promotedcomp==INT_TYPE)||(promotedcomp==REAL_TYPE)){
            print_constant_definition(modfile,scratch_id,now_expr);}}}
/*???*/ if (bitset(debug_flagword,DBG_LEX)) fprintf(stderr,"iii out=%d\n",(int) iii);
#ifdef BUGGY
fprintf(bugfile,"Point AAA:\n");
bug_print_identifier_table();
#endif

                    /**************************************/
                    /**************************************/
                    /****                              ****/
                    /****   DECLARE THE ARRAY ITSELF   ****/
                    /****                              ****/
                    /**************************************/
                    /**************************************/

   count=curr_dim;
   /*---------------------------------------  declare the array itself */
   if (is_array){
      xref_setvalue(id);
      idinfo = searchid_dcl(id,promotedcomp,pos);
      (*idinfo).flags |= extra_types;
      (*idinfo).ptr.vvv=oldvvv;
      if (is_double){
         (*idinfo).index.dims.first.lower = (Subscript) 1;
         (*idinfo).index.dims.first.upper = give_Subscript(times,E_INTERNAL);
         (*idinfo).index.dims.second.lower = (Subscript) 1;
         (*idinfo).index.dims.second.upper = give_Subscript(count,E_INTERNAL);}
      else{
         (*idinfo).index.dims.first.lower = (Subscript) 1;
         (*idinfo).index.dims.first.upper = give_Subscript(count,E_INTERNAL);
         simplify((*idinfo).index.dims.second);}
      times *= count;
      if (times>256){
        simplify((*idinfo).index.dims.second);
        set_arrow_sides(TRUE,FALSE);
        error_this_pos(E_DIMPRODHUGE);}
      (*idinfo).flags = promotedcomp | ARRAY_TYPE;}
#ifdef BUGGY
fprintf(bugfile,"Point BBB:\n");
bug_print_identifier_table();
#endif
   /*---------------------------------------  restore expression state */
   if (constant)
      restore_expr_ixs(holdops,holdopnds,holdidptrs,holdexprs);
   ixs_count=holdixs;
   if ((!is_bool) && (promotedcomp == BOOL_TYPE))
      show_err_at_pos_on_both(E_BOOLTOKENMISSING,bgn_pos,NULL,EMPTY_TYPE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
type_flagword_type parse_simple_def_expr
           (Boolean *errptr,Boolean is_bool,Boolean is_array,short *of_count)
{
   type_flagword_type comp;
   /*-------------------------------------------------------*/
   parsing_convar_def=TRUE;
   if (is_bool) comp=init_and_parse_bool_expr(errptr,is_array,FALSE,of_count);
   else comp=init_and_parse_expression(errptr,is_array,FALSE,of_count);
   parsing_convar_def=FALSE;
   comp &= COMPUTATIONAL_TYPE_MASK; 
  /*-------------------------------------------------------*/
   return(comp);
}
/*===========================================================================*/
void parse_ONEDEATH_def_stat()
{
   /*-------------------------------------------------------*/
   if (!option_def_no_skip) advance_one_token();
   parse_flag_set_spec(&option_rec.onedeath,FALSE,optnal_flg_set_eq_sign);
   /*-------------------------------------------------------*/
}

/*===========================================================================*/
void set_ONEDEATH_default(int dv)     /* ADDED RWB05 */
{
  option_rec.onedeath = dv;
}


void set_COMMENTS_default(int dv)     /* ADDED RWB05 */
{
  option_rec.comment = dv;
}


/*===========================================================================*/
void parse_COMMENT_def_stat()
{
   /*-------------------------------------------------------*/
   if (!option_def_no_skip) advance_one_token();
   parse_flag_set_spec(&option_rec.comment,FALSE,optnal_flg_set_eq_sign);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_MODSTATS_def_stat()
{
   /*-------------------------------------------------------*/
   if (!option_def_no_skip) advance_one_token();
   parse_flag_set_spec(&option_rec.comment_stats,FALSE,optnal_flg_set_eq_sign);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_ECHO_def_stat()
{
   /*-------------------------------------------------------*/
   if (!option_def_no_skip) advance_one_token();
   parse_flag_set_spec(&option_rec.echo,FALSE,optnal_flg_set_eq_sign);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_TRIM_def_stat()
{
   short holdpos;
   identifier_info_type *idinfo;
   Boolean err;
   short of_count;
   type_flagword_type tyans,comp;
   void *vvv;
   value_union_type value;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"parsing TRIM definition::\n");
#endif
   if (!option_def_no_skip) advance_one_token();
   parse_flag_set_spec(&option_rec.trim,TRUE,optnal_flg_set_eq_sign);
#ifdef BUGGY
fprintf(bugfile,"option_rec.trim=%d, this.tok=%d, this.rw=%d\n",
(int) option_rec.trim,(int) this.tok,(int) this.rw);
#endif
   if (option_rec.trim<=0){
      if (this_tok_is_rw(RW_WITH))
         error_msg_this_pos(E_KEYWCONTEXT,"WITH CLAUSE INVALID WHEN TRIM OFF.",
                            CHAR_TYPE);}
   else{
      holdpos=this.pos;
      if (this_tok_is_rw(RW_WITH)){
         advance_one_token();
         tyans=parse_simple_def_expr(&err,FALSE,FALSE,&of_count);
         if (tyans==INT_TYPE) save_postfix(OP_ItoR);
         else if (tyans!=REAL_TYPE) this_expr_error_this_pos(E_NOTREAL);
         if (this_expr.in_error)
            standard_value(&value,comp=REAL_TYPE);
         else
            comp=evaluate_expression(&this_expr,&value,FALSE,0,-1,0,TRUE);
         vvv=save_value_in_number_table(&value,REAL_TYPE);
         xref_setvalue("TRIMOMEGA");
         idinfo=searchid_dcl("TRIMOMEGA",REAL_TYPE,holdpos);
         (*idinfo).ptr.vvv=vvv;
         print_constant_definition(modfile,"TRIMOMEGA",&this_expr);}
      else{
         idinfo = qqsearchid("TRIMOMEGA",FALSE);
         if (idinfo == ((identifier_info_type *) NULL)){
            do_switch_to_stdin("TRIMOMEGA? ",SAVE_LEVEL_1);
            parse_constant_def_clause("TRIMOMEGA",holdpos,FALSE,EMPTY_TYPE);
            do_switch_back_to_inputfile(SAVE_LEVEL_1);}}}
   skip_to_next_semicolon(FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_INPUT_statement()
/*
   This routine parses an input statement for a list of input items.
   If a trailing space is missing when a prompt message is supplied, the
   message is padded with a ": " unless it ends in a colon, question, or
   greater-then.  The prompt is always padded with a trailing space if one
   is missing.  Thus:
             AAA                 will prompt "AAA? "
             "Enter AAA":AAA     will prompt "Enter AAA: "
             "Enter AAA>":AAA    will prompt "Enter AAA> "
             "Enter AAA?":AAA    will prompt "Enter AAA? "
             "Enter AAA:":AAA    will prompt "Enter AAA: "
             "Enter AAA#":AAA    will prompt "Enter AAA#: "
             "Enter AAA> ":AAA   will prompt "Enter AAA> "
             "Enter AAA? ":AAA   will prompt "Enter AAA? "
             "Enter AAA: ":AAA   will prompt "Enter AAA: "
             "Enter AAA# ":AAA   will prompt "Enter AAA# "
*/
{
   short nch;
   Boolean empty;
   char buffer[IDENT_MAXNCH_P+94];
   char *prompt_ptr;
   char ch;
   token_info_type saved;
   Boolean is_bool,for_option;
   any_parse_statement_routine prs_rou;
   /*-------------------------------------------------------*/
   advance_one_token();
   prs_rou = ((any_parse_statement_routine) NULL);
   for (empty=TRUE;this_is_not_semi();empty=FALSE){
      if (!empty){
         if (this.tok==TK_COMMA) advance_one_token();
         else error_this_pos(E_NOTCOMMA);}
      /*---------------------------------------------   make prompt message */
      prompt_ptr=buffer;
      for_option=FALSE;
      if (is_bool=this_tok_is_rw(RW_BOOLEAN)){
         /*---------------------------------   BOOLEAN IDENT */
         advance_one_token();
         if (this.tok==TK_ID) sprintf(prompt_ptr,"%s? ",this.id);
         else{
            error_this_pos(E_NOTINPUTPROMPT);
            *prompt_ptr='\0';}}
      else if (this.tok==TK_ID)
         /*---------------------------------   IDENT */
         sprintf(prompt_ptr,"%s? ",this.id);
      else if (this.tok==TK_CHAR){
         /*---------------------------------   "Prompt":IDENT */
         if (strlen(scratch_string_buffer)>90){
            show_err_at_pos_on_both(E_PROMPTTRUNC,this.pos,
                                    scratch_string_buffer,CHAR_TYPE);
            *(scratch_string_buffer+90)='\0';}
         strcpy(prompt_ptr,scratch_string_buffer);
         advance_one_token();
         if (this.tok==TK_COLON) advance_one_token();
         else{
            error_this_pos(E_NOTCOLON);
            if (!((this.tok==TK_SEMI)||(is_value_token(this.tok))))
               advance_one_token();}
         if (is_bool=this_tok_is_rw(RW_BOOLEAN)) advance_one_token();
         if (this.tok==TK_ID);
         else if ((this.tok==TK_RW)&&(this_tok_is_option_def())){
            prs_rou = (*(parse_statement_routine + this.rw));
            for_option=TRUE;}
         else if (this.tok==TK_RW) error_this_pos(E_KEYWCONTEXT);
         else error_this_pos(E_NOTINPUTPROMPT);}
      else if (this.tok==TK_RW){
         /*---------------------------------   error unless option def */
         if (this_tok_is_option_def()){
#ifdef BUGGY
fprintf(bugfile,"...///... inputting option def\n");
#endif
            sprintf(prompt_ptr,"%s (ON/OFF/FULL)? ",
                               statement_name_reserved_word(this.rw));
            prs_rou = (*(parse_statement_routine + this.rw));
            for_option=TRUE;}
         else{
#ifdef BUGGY
fprintf(bugfile,"...///... inputting other reserved word\n");
#endif
            error_this_pos(E_KEYWCONTEXT);
            *prompt_ptr='\0';}}
      else{
         /*---------------------------------   error */
         error_this_pos(E_NOTINPUTPROMPT);
         *prompt_ptr='\0';}
      save_operator_and_advance_1_tok(&saved);
      /*---------------------------------------------  adjust prompt suffix */
      nch=strlen(prompt_ptr);
      if (!is_iolib_space(ch=(*(prompt_ptr+nch-1)))){
         if (!((ch=='>')||(ch==':')||(ch=='?')))
            strcat(prompt_ptr,": ");
         else strcat(prompt_ptr," ");}
      /*---------------------------------------------  prompt for input expr*/
#ifdef BUGGY
fprintf(bugfile,"...///... doing the input\n");
#endif
      do_switch_to_stdin(prompt_ptr,SAVE_LEVEL_0);
      if (for_option){
         set_optional_equal_sign();
         option_def_no_skip = TRUE;
         prs_rou();
         option_def_no_skip = FALSE;
         reset_optional_equal_sign();}
      else parse_constant_def_clause(saved.id,saved.pos,is_bool,EMPTY_TYPE);
      do_switch_back_to_inputfile(SAVE_LEVEL_0);
      /*---------------------------------------------   skip to comma/semi */
#ifdef BUGGY
fprintf(bugfile,"...this.tok=%d\n",(int) this.tok);
#endif
      if (this.tok==TK_LB){
         show_err_at_pos_on_both(E_NOTSCALAR,this.pos,prev.id,CHAR_TYPE);
         skip_to_next_comma_or(TRUE,TK_RB);}
      else skip_to_next_comma_or(FALSE,TK_SEMI);}
   if (empty)
      show_wrn_on_both(E_EMPTYLIST,NULL_PTR,EMPTY_TYPE,SERIOUS_WARN_LEV);
   skip_to_next_semicolon(FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_SPACE_statement()
/*
   This function parses the SPACE statement.   It assumes that
   parse_space_picture will add the identifiers for the space variables
   into the identifier table in sequence.
*/
{
   bitsize_type bitlen;
#ifdef BUGGY
identifier_info_type *idinfo;
identifier_info_type *idovf;
#endif
   /*-------------------------------------------------------*/
   advance_one_token();
   if (this.tok==TK_EQ)
      advance_one_token();
   else
      show_wrn_on_both(E_MISSINGTOKEN,"=",CHAR_TYPE,SERIOUS_WARN_LEV);
   if (this.tok==TK_LP){
      state_space_picture_ptr=parse_space_picture((bitsize_type) 0,&bitlen);
      space_var_list=next_id_ptr();
      prev_nele_identifier_table=nele_identifier_table;
      move_top_identifiers_down();
      adjust_picture_after_move_down(state_space_picture_ptr);
#ifdef BUGGY
idovf=next_id_ptr();
for (idinfo=space_var_list;idinfo<idovf;++idinfo){
if (!bitset((*idinfo).flags,SSVAR_TYPE)){
fprintf(stderr,"Not a state-space variable in parse_SPACE_statement\n");
show_err_at_pos_on_both(E_INTERNAL,this.pos,(*idinfo).name,CHAR_TYPE);}}
#endif
      skip_to_next_semicolon(FALSE);}
   else{
      error_this_pos(E_NOTOPENPAREN);
      skip_to_next_semicolon(TRUE);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void adjust_picture_after_move_down(state_space_picture_type *pixptr)
{
   Memix iii,nnn,lookix;
   vars_union_type *varptr;
   identifier_info_type *idinf;
   /*-------------------------------------------------------*/
   for (nnn=(*pixptr).nvaru,iii=0;iii<nnn;++iii){
      varptr=(*pixptr).varu+iii;
      if (*((*pixptr).is_nested+iii))
         adjust_picture_after_move_down((*varptr).nested_space_picture);
      else{
         idinf = (*varptr).id_info;
         lookix = (Memix) relative_identifier(idinf);
         if (lookix >= prev_nele_identifier_table){
             lookix = max_identifier_table_count-lookix-1;
             lookix += prev_nele_identifier_table;
             idinf = absolute_identifier((relative_address_type) lookix);
             (*varptr).id_info = idinf;}}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
state_space_picture_type *parse_space_picture
      (bitsize_type old_bitoff,bitsize_type *new_bitoff)
/*
   This routine parses a state-space picture.   It recurses when there
   is a nested state-space picture in the picture.

   Upon entry, the current token must be the left parenthesis token which
   begins the space picture.   Upon exit, the current token will be the
   token following the matching right parenthesis token.
*/
{
   Boolean nested[MAX_SSV_COUNT_PER_NESTING_LEVEL];
   vars_union_type tvars[MAX_SSV_COUNT_PER_NESTING_LEVEL];
   node_union_type node;
   node_union_type *nodeptr;
   value_union_type value;
   Memix count,nowaddr;
   bitsize_type bitoff,temp;
   state_space_picture_type *answer;
   short knt;
   identifier_info_type *idinfo;
   Boolean is_array,is_double;
   /*-------------------------------------------------------*/
   advance_one_token();
   count = 0;
   bitoff=old_bitoff;
   /*----------------------------------------------------------- parse space */
   while (this_is_not_list_terminator()){
      if (this.tok==TK_LP){
         /*------------------------------------------------- nested */
         if (count>=MAX_SSV_COUNT_PER_NESTING_LEVEL) goto buff_ovf;
         nested[count]=TRUE;
         tvars[count].nested_space_picture=parse_space_picture(bitoff,&bitoff);
         count++;}
      else if (this.tok==TK_ID){
         if (count>=MAX_SSV_COUNT_PER_NESTING_LEVEL) goto buff_ovf;
         /*------------------------------------------------- next ident */
         default_irange(&value.sss,bitoff,0);
         idinfo=searchid_dcl_at_top(this.id,INT_SSVAR_TYPE,this.pos);
         (*idinfo).flags = INT_SSVAR_TYPE;
         (*idinfo).ptr.vvv = save_value_in_number_table(&value,SOFF_TYPE);
         advance_one_token();
         if (this.tok==TK_COLON){
            /*---------------------------------  : ... */
            advance_one_token();
            if (is_value_token(this.tok) || (this.tok==TK_SUB))
               /*---------------------  : <i-range> */
               parse_value_irange((*idinfo).ptr.sss);
            else if (this_tok_is_rw(RW_BOOLEAN)){
               /*---------------------  : BOOLEAN */
               make_boolean((*idinfo).ptr.sss,&(*idinfo).flags);
               advance_one_token();}
            else if (this_tok_is_rw(RW_ARRAY)){
               /*---------------------  : ARRAY [ <i-range> ] ... */
               parse_array_state_space_var(idinfo);}}
         else if ((this.tok!=TK_COMMA)&&(this.tok!=TK_RP)){
            /*---------------------------------  NOT : <i-range> */
            error_this_pos(E_NOTCOLON);}
         /*------------------------------------------------- store new ssv */
         nested[count]=FALSE;
         tvars[count].id_info = idinfo;
         count++;
         DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
         /*------------------------------------------------- adjust bitoff */
         if (!is_array)
            bitoff += (*(*idinfo).ptr.sss).bit_length;
         else{
            temp = dim_range((*idinfo).index.dims.first);
            temp *= dim_range((*idinfo).index.dims.second);
            bitoff += (temp*((*(*idinfo).ptr.sss).bit_length));}}
      else{
         /*------------------------------------------------- ident missing */
         error_this_pos(E_NOTIDENT);
         if ((this.tok!=TK_RP)&&(this.tok==TK_COMMA)) advance_one_token();}
      if (this.tok==TK_COMMA){
         advance_one_token();
         if (!this_is_not_list_terminator()) error_this_pos(E_NOTIDENT);}
      else if (this.tok!=TK_RP) error_this_pos(E_NOTCLOSEPAREN);}
   if (this.tok==TK_RP) advance_one_token();
   /*----------------------------------------------------------- fill picture*/
   if (count>((Memix) MAXSHORT/sizeof(vars_union_type)))
      error_this_pos(E_SPACEPIXOVF);
   if (count<=0) error_this_pos(E_EMPTYPIXLIST);
   knt=(short) count;
   nowaddr=increment_pixarr_ele(knt);
   node.pix.nvaru = knt;
   node.pix.varu = storage_vars_ptrs + nowaddr;
   node.pix.is_nested = storage_nested + nowaddr;
   sys_memcpy(node.pix.is_nested,nested,(size_t) (count*sizeof(Boolean)));
   sys_memcpy(node.pix.varu,tvars,(size_t) (count*sizeof(vars_union_type)));
   /*----------------------------------------------------------- store it */
   nowaddr = increment_node_ele(1);
   nodeptr = storage_nodes + nowaddr;
   answer = &(*nodeptr).pix;
   sys_memcpy(nodeptr,&node,sizeof(node_union_type));
   /*-------------------------------------------------------*/
   *new_bitoff=bitoff;
#ifdef BUGGY
   bug_print_space_picture(answer); fprintf(bugfile,"\n");
#endif
   /*-------------------------------------------------------*/
   return(answer);
buff_ovf:
#ifdef INTERNAL_BUGGY
fprintf(stderr,"Buffer overflow when attempting to \"parse_space_picture\"\n");
#endif
   show_err_on_both(E_INTERNAL,(void *) ovf_msg,CHAR_TYPE);
   abort_program();
   return(NULL);
}
/*===========================================================================*/
void parse_array_state_space_var(identifier_info_type *id_inf)
/*
   This function parses an array state-space variable of syntax:
         : ARRAY [ <i-range> ]
         : ARRAY [ <i-range> ] OF <i-range>
         : ARRAY [ <i-range> ] OF BOOLEAN
         : ARRAY [ <i-range> , <i-range> ]
         : ARRAY [ <i-range> , <i-range> ] OF <i-range>
         : ARRAY [ <i-range> , <i-range> ] OF BOOLEAN
*/
{
   state_offset_type toff;
   state_offset_type *offinf;
   long product;
   /*-------------------------------------------------------*/
   offinf = (*id_inf).ptr.sss;
   advance_one_token();
   (*id_inf).flags |= ARRAY_TYPE;
   if (this.tok==TK_LB){
      /*-------------------------  range present */
      advance_one_token();
      parse_subscript_irange(&toff);
      (*id_inf).index.dims.first.lower=(Subscript) toff.minval;
      (*id_inf).index.dims.first.upper=(Subscript) toff.maxval;
      if (this.tok==TK_COMMA){
         product = (long) (toff.maxval-toff.minval+1);
         advance_one_token();
         parse_subscript_irange(&toff);
         (*id_inf).index.dims.second.lower=(Subscript) toff.minval;
         (*id_inf).index.dims.second.upper=(Subscript) toff.maxval;
         product *= (long) (toff.maxval-toff.minval+1);
         if (product>256){
            simplify((*id_inf).index.dims.second);
            set_arrow_sides(TRUE,FALSE);
            error_this_pos(E_DIMPRODHUGE);}}
      if (this.tok==TK_COMMA)
         show_err_at_pos_on_both(E_TOOMANYSUBS,this.pos,
                                 (*id_inf).name,CHAR_TYPE);
      if (this.tok!=TK_RB) error_this_pos(E_NOTCLOSEBRACKET);
      skip_to_next_specified(TRUE,TK_RB);
      if (this.tok==TK_RB) advance_one_token();}
   else if ((this.tok==TK_COMMA)||(this.tok==TK_RP)||
            (this_tok_is_rw(RW_OF))){
      /*-------------------------  range missing */
      (*id_inf).index.dims.first.lower=1;
      (*id_inf).index.dims.first.upper=16;
      error_this_pos(E_NOARRAYRANGE);}
   else
      error_this_pos(E_NOTOPENBRACKET);
   /*---------------------------------  OF ... */
   if (this_tok_is_rw(RW_OF)){
      advance_one_token();
      if (this_tok_is_rw(RW_BOOLEAN)){
         make_boolean(offinf,&(*id_inf).flags);
         advance_one_token();}
      else
         parse_value_irange(offinf);}
   else if ((this.tok!=TK_COMMA)&&(this.tok!=TK_RP)){
      show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,"OF",CHAR_TYPE);
      parse_value_irange(offinf);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
static expression_type subscript_expr;
/*===========================================================================*/
void copy_subscript_to_space_list(Subscript nowsub)
{
   value_union_type value;
   char scratch_id[IDENT_MAXNCH_P];
   /*-------------------------------------------------------*/
   sprintf(scratch_id,"%c%d",NUMBER_TABLE_PREFIX_CHAR,(int) nowsub);
   *subscript_expr.operands = searchid_silent(scratch_id,INT_TYPE,0,FALSE);
   if ((*(*subscript_expr.operands)).ptr.vvv == ((void *) NULL)){
      value.iii=(int_type) nowsub;
      (*(*subscript_expr.operands)).ptr.vvv =
               save_value_in_number_table(&value,INT_TYPE);}
   subscript_expr.source_code_line_number=this.linnum;
   save_expr_into_scratch(&subscript_expr,(identifier_info_type *) NULL);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Memix qq_parse_space_expr(state_space_picture_type *pic,
                          Memix offset,
                          Boolean empty_expressions_are_okay)
{
/*
   This function is the recursive workhorse for parse_space_expression.
   Upon entry, the current token must be the opening paren of the space
   expression.   Upon exit, the current token will be the token following
   the matching right paren.
*/
   Subscript nowsub,sub0,sub9;
   Subscript nowsec,sec0,sec9;
   short of_count;
   Memix ident_off;
   Memix ix,nix;
   type_flagword_type compq,tyid,compid;
   Boolean err,is_array,is_double,is_empty,boolq;
   identifier_info_type *idinfo;
   char scratch[90];
   char *ppp;
   value_union_type value;
   static operation_type val_oper = OP_VAL;
   static operand_type val_opnd;
   ssvar_value_type ssv_value;
   int_type tval;
   Boolean *bvalp;
   vars_union_type *varptr;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"Now parsing space expr(pic=\"");
bug_print_space_picture(pic);
fprintf(bugfile,"\",offset=%d,emptyok=%c)\n",
(int) offset,((empty_expressions_are_okay) ? 'T' : 'F'));
#endif
   subscript_expr.infix_ops = subscript_expr.postfix_ops = &val_oper;
   subscript_expr.operands = &val_opnd;
   subscript_expr.n_operands =  subscript_expr.n_postfix_ops = 
                                subscript_expr.n_infix_ops = 1;
   subscript_expr.in_error=FALSE;
   subscript_expr.rtntype=INT_TYPE;
   ident_off=offset;
   of_count=0;
#ifdef BUGGY
fprintf(bugfile,"looking for left paren: ");
showtoken(this.tok,this.rw,this.id);
fprintf(bugfile,"\n");
#endif
   if (this.tok==TK_LP) advance_one_token();
   else error_this_pos(E_NOTOPENPAREN);
   compq=0;
   for (ix=0,nix=(*pic).nvaru;ix<nix;++ix){
      idinfo = space_var_list + ident_off;
      xref_setvalue((*idinfo).name);
      if ((ix>0)&&(of_count<=0)){
         if (this.tok==TK_COMMA) advance_one_token();
         else error_this_pos(E_NOTCOMMA);}
      if ((*((*pic).is_nested+ix))){
         /*-------------------------------------   nested expression */
         if (of_count>0){
            value.iii=of_count;
            show_tail_err_at_pos_on_both(E_OFTOOBIG,this.pos,&value.iii,
                                         INT_TYPE,"REPETITIONS IGNORED");
            of_count=0;}
#ifdef BUGGY
fprintf(bugfile,"parsing nested now::");s
showtoken(this.tok,this.rw,this.id);
fprintf(bugfile,"\n");
#endif
         varptr = (*pic).varu+ix;
         ident_off=qq_parse_space_expr((*varptr).nested_space_picture,
                                       ident_off,empty_expressions_are_okay);}
      else{
         /*-------------------------------------   see if array */
         tyid=(*idinfo).flags;
         compid = (tyid & COMPUTATIONAL_TYPE_MASK);
         DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
         sec0=sec9=sub0=sub9=1;
         if (is_array){
            sub0=(*idinfo).index.dims.first.lower;
            sub9=(*idinfo).index.dims.first.upper;
            if (is_double){
               sec0=(*idinfo).index.dims.second.lower;
               sec9=(*idinfo).index.dims.second.upper;}}
         ++ident_off;
         /*-------------------------------------   parse each array element */
         for (nowsub=sub0;nowsub<=sub9;++nowsub)
         for (nowsec=sec0;nowsec<=sec9;++nowsec){
            /*---------------------   copy subscript to space list */
            if (is_array) copy_subscript_to_space_list(nowsub);
            if (is_double) copy_subscript_to_space_list(nowsec);
            /*---------------------   parse more if no "n OF .." leftovers */
            is_empty=FALSE;
            if (of_count<=0){
               if ((nowsub>sub0)||(nowsec>sec0)){
                  if (this.tok==TK_COMMA) advance_one_token();
                  else error_this_pos(E_NOTCOMMA);}
               if ((this.tok==TK_COMMA)||(this.tok==TK_RP)){
                  is_empty=TRUE;
                  of_count=1;
                  if (!empty_expressions_are_okay)
                     this_expr_error_this_pos(E_MISSINGEXPR);}
               else{
                  boolq=(Boolean) (compid == BOOL_TYPE);
#ifdef BUGGY
fprintf(bugfile,"compid=%d, boolq=%d\n",(int) compid,(int) boolq);
#endif
                  compq=parse_simple_def_expr(&err,boolq,TRUE,&of_count);
#ifdef BUGGY
fprintf(bugfile,"of_count=%d\n",(int) of_count);
#endif
                  if (of_count<=0) of_count=1;}}
            /*---------------------   error if expr vs. ssv type mismatch */
#ifdef BUGGY
fprintf(bugfile,"tyid=%02x, compq=%02x, compid=%02x, name=%s\n",
(unsigned int) tyid,(unsigned int) compq,(unsigned int) compid,(*idinfo).name);
#endif
            if (compq != compid){
               strcpy(scratch,(*idinfo).name);
               ppp=scratch+strlen(scratch);
               if (is_array){
                  if (is_double)
                       sprintf(ppp,"[%d,%d]",(int) nowsub,(int) nowsec);
                  else sprintf(ppp,"[%d]",(int) nowsub);
                  ppp=scratch+strlen(scratch);}
               sprintf(ppp," IS %s SSV.",
                       ((compid==BOOL_TYPE) ? "A BOOLEAN" : "AN INTEGER"));
               this_expr.in_error=TRUE;
               show_err_at_pos_on_both(E_TYPEMISMATCH,this.pos,
                                       scratch,CHAR_TYPE);}
            /*---------------------   copy expression to space list */
            if (!is_empty) save_this_scratch_expr(idinfo);
            if (doing_start_statement && (!this_expr.in_error)){
               boolq = (Boolean) ((this_expr.rtntype & COMPUTATIONAL_TYPE_MASK)
                                  == BOOL_TYPE);
               ssv_value = eval_for_expected_ssv(&this_expr,(int_type) 0,
                                                 FALSE,0,0,TRUE,boolq);
               if (ssvar_vs_range_err(ssv_value,(*idinfo).ptr.sss)){
                  bvalp=(Boolean *) &tval;
                  tval=ssv_value;
                  if (boolq) *bvalp = (Boolean) ssv_value;
                  show_rule_info_after_error(idinfo,nowsub,nowsec,
                                             (void *) &tval,
                                             "START",start_linnum,
                                             ((state_bitstring_type) NULL));}}
            /*---------------------   */
            --of_count;}}}
#ifdef BUGGY
fprintf(bugfile,"looking for right paren: ");
showtoken(this.tok,this.rw,this.id);
fprintf(bugfile,"\n");
#endif
   if (this.tok==TK_RP) advance_one_token();
   else error_this_pos(E_NOTCLOSEPAREN);
   /*-------------------------------------------------------*/
   return(ident_off);
} 
/*===========================================================================*/
typedef struct t__elist
   {  /* ordering of shorts is important */
      short iii;     /* first.lower */
      short iiiend;  /* first.upper */
      short jjj;     /* second.lower */
      short jjjend;  /* second.upper */
      short knt;
      short *which;
      identifier_info_type *idinfo;
      dim_pair_type q1st; /* first */
      dim_pair_type q2nd; /* second */
      dim_pair_type *q_1st_or_2nd;
      Boolean const1;
      Boolean const2;
      Boolean *qconst;
      Boolean is_var;
   } elist_type;
static elist_type elist;
/*===========================================================================*/
void save_and_re_init(type_flagword_type tyans)
{
   /*-------------------------------------------------------*/
   save_this_scratch_expr(((identifier_info_type *) NULL));
   if ((!this_expr.in_error)&&
       (!bitset(tyans,ANY_SPECIAL_VAR_TYPE_MASK))){
      *elist.which = eval_for_expected_integer
             (&this_expr,0,FALSE,0,-1,0,FALSE);
      *(elist.which+1) = (*elist.which);
      *elist.qconst=TRUE;}
   else{
      elist.is_var=TRUE;
      *elist.which = (*elist.q_1st_or_2nd).lower;
      *(elist.which+1) = (*elist.q_1st_or_2nd).upper;}
   elist.which += 2;
   elist.q_1st_or_2nd++;
   elist.knt++;
   elist.qconst++;
   init_expression(FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_space_expr_list(space_expression_type *sex)
{
   /*-------------------------------------------------------*/
   qq_parse_space_expr_list(sex);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void qq_parse_space_expr_list(space_expression_type *sex)
{
   Memix holdops,holdopnds,holdidptrs,holdexprs;
   Memix knt;
#ifdef BUGGY
Memix kkk;
FILE *fp;
#endif
   Boolean boolq,is_array,is_double,is_okay_array,err,do_make,shown;
   operation_type makeop,ixdop;
   short junk;
   type_flagword_type tyid,tyans;
   token_info_type saved;
   relative_address_type reladdr_1,reladdr_2;
   state_offset_type *offinf;
   unsigned char ssbits_storage[1024];  /* no more than 1024 SSV's ??? */
   bitsize_type pos,posknt;
   identifier_info_type *varname_idinfo;
   short varname_pos,iii,jjj;
   int_type iv;
   char *ppp;
   char varname_text
          [2+IDENT_MAXNCH_P+MAX_INT_TYPE_NDIGS_P+MAX_INT_TYPE_NDIGS_P];
   char *varname_subs;
   elist_type hold_elist;
   char tailmsg[200];
   /*-------------------------------------------------------*/
   memcpy((char *) &hold_elist,(char *) &elist,sizeof(elist_type));
   sys_memset(ssbits_storage,0,(size_t) 1024);
   remember_expr_ixs(&holdops,&holdopnds,&holdidptrs,&holdexprs);
   while (this.tok==TK_ID){
      xref_setvalue(this.id);
      this.id_info_ptr = qqsearchid(this.id,FALSE);
      if (this.id_info_ptr != NULL) this_looked_up=TRUE;
      lookup_ident();
      varname_idinfo = this.id_info_ptr;
      tyid=(*varname_idinfo).flags;
      offinf = (*varname_idinfo).ptr.sss;
      strcpy(varname_text,(*varname_idinfo).name);
      varname_subs = varname_text + strlen(varname_text);
      varname_pos=this.pos;
      DECIDE_IS_ARRAY_IS_DOUBLE(varname_idinfo);
      if (!bitset(tyid,SSVAR_TYPE)){
         error_this_pos(E_NOTSSV);
         skip_to_next_comma_or(TRUE,TK_RW);}
      else{
         is_okay_array=FALSE;
         boolq=(Boolean) ((tyid & COMPUTATIONAL_TYPE_MASK) == BOOL_TYPE);
         save_operator_and_advance_1_tok(&saved);
         elist.idinfo = varname_idinfo;
         elist.which = &elist.iii; 
         elist.qconst = &elist.const1;
         elist.const2 = elist.const1 = FALSE;
         elist.knt=0;
         elist.q_1st_or_2nd = &elist.q1st;
         dim_pair_cpy(&elist.q1st,&(*varname_idinfo).index.dims.first);
         dim_pair_cpy(&elist.q2nd,&(*varname_idinfo).index.dims.second);
         elist.iii = elist.iiiend = (*elist.q_1st_or_2nd).lower;
         elist.jjj = elist.jjjend = (*(elist.q_1st_or_2nd+1)).lower;
         elist.is_var=FALSE;
         pos = 0;
         posknt = compute_offset((*varname_idinfo).index.dims.first.upper,
                                 (*varname_idinfo).index.dims.second.upper,
                                 varname_idinfo); ++posknt;
         if (this.tok==TK_LB){
            if (is_array){
               is_okay_array=TRUE;
               init_expression(FALSE);
               tyans=qq_parse_subscript(TRUE,save_and_re_init);
               save_and_re_init(tyans);}
            else error_this_pos(E_IXONSCALAR);}
         else if (is_array)
            error_this_pos(E_NOTOPENBRACKET);
         shown=FALSE;
         for (iii=elist.iii;iii<=elist.iiiend;++iii){
            for (jjj=elist.jjj;jjj<=elist.jjjend;++jjj){
               pos = compute_offset(iii,jjj,varname_idinfo);
               if ((pos<0)||(pos>=(((*offinf).bit_length)*posknt))){
                  if (elist.const1 &&
                      (
                       (elist.iii<(*varname_idinfo).index.dims.first.lower)
                       ||
                       (elist.iii>(*varname_idinfo).index.dims.first.upper)
                      )){iv=iii;
                         error_msg_this_pos(E_DIMOUTOFBOUNDS,&iv,INT_TYPE);}
                  else if (elist.const2 &&
                      (
                       (elist.jjj<(*varname_idinfo).index.dims.second.lower)
                       ||
                       (elist.jjj>(*varname_idinfo).index.dims.second.upper)
                      )){iv=iii;
                         error_msg_this_pos(E_DIMOUTOFBOUNDS,&iv,INT_TYPE);}
                  else if (SERIOUS_WARN_LEV<=levels_of_warnings && (!flushed)){
                     flushed=TRUE;
#ifdef BUGGY
                     for (fp=stderr;fp!=NULL;){
                        fprintf(fp,"%s[%d..%d,%d..%d]\n",
                                    (*varname_idinfo).name,
                                    (*varname_idinfo).index.dims.first.lower,
                                    (*varname_idinfo).index.dims.first.upper,
                                    (*varname_idinfo).index.dims.second.lower,
                                    (*varname_idinfo).index.dims.second.upper);
                        fprintf(fp,"%s[%d..%d,%d..%d]\n",
                                    "elist",
                                    elist.q1st.lower,
                                    elist.q1st.upper,
                                    elist.q2nd.lower,
                                    elist.q2nd.upper);
                        fprintf(fp,"pos=%d/%d,bl=%d,pk=%d,  ",
                                    (int) pos,(int) posknt,
                                    (int) (*offinf).bit_length,
                                    (int) posknt);
                        fprintf(fp,"%d[%d..%d],",(int) iii,
                                   (int) elist.iii,(int) elist.iiiend);
                        fprintf(fp,"%d[%d..%d]\n",(int) jjj,
                                   (int) elist.jjj,(int) elist.jjjend);
                        fp=((fp==stderr) ? (listfile) : (NULL));}
#endif
                     strcpy(tailmsg,"Algorithm to check for TRANTO destin");
                     strcat(tailmsg,"ation duplication errors flushed in:");
                     strcat(tailmsg," \"qq_parse_space_expr_list\"");
                     tail_warn_this_pos(E_INTERNAL,SERIOUS_WARN_LEV,tailmsg);}
                  pos=0;}
               pos += (*offinf).bit_offset;
               ppp = varname_subs;
               if (is_array){
                  strcpy(ppp,STRLB); ppp++;
                  sprintf(ppp,"%d",(int) iii); ppp += strlen(ppp);
                  if (is_double) sprintf(ppp,",%d",(int) jjj);
                  strcat(ppp,STRRB);}
               if (bitset(debug_flagword,DBG_LEX)){
                  fprintf(stderr,"[iii,jjj]=[%d,%d]/[%d,%d]  ",
                         (int) iii,(int) jjj,
                         (int) elist.iiiend,(int) elist.jjjend);
                  fprintf(stderr,"levels=%d,  elist.is_var=%d\n",
                         (int) levels_of_warnings,(int) elist.is_var);}
               if ((MINOR_WARN_LEV<=levels_of_warnings)||(!elist.is_var)){
                  if (!gen_suppress){
                      if ((!shown) && (test_bit(ssbits_storage,pos))){
                         show_wrn_at_pos_on_both(E_TWICEINTTO,varname_pos,
                                                 varname_text,CHAR_TYPE,
                                                 SERIOUS_WARN_LEV);
                         shown=TRUE;}}}
               set_bit(ssbits_storage,pos);}}
         do_make=FALSE;
         makeop=OP_NIX;
         if (this.tok==TK_EQ) advance_one_token();
         else if (this.tok==TK_DEC_1){
            makeop=OP_DEC;
            do_make=TRUE;}
         else if (this.tok==TK_INC_1){
            makeop=OP_INC;
            do_make=TRUE;}
         else show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,"=",CHAR_TYPE);
         if (do_make){
            if (is_okay_array){
               ixdop=OP_IXDBY;
               if (elist.knt==2){
                  ixdop=OP_IXDBY2;
                  append_expr_to_this(storage_exprs+exprs_count-2);
                  save_infix(OP_I_CMMA);}
               append_expr_to_this(storage_exprs+exprs_count-1);
               make_inc_or_dec_array_ele_expr(saved.id_info_ptr,makeop,ixdop);}
            else
               make_inc_or_dec_variable_expr(saved.id_info_ptr,makeop);
            tyans = (*saved.id_info_ptr).flags & COMPUTATIONAL_TYPE_MASK;
            if (tyans != INT_TYPE) error_this_pos(E_NOTINTEGER);
            advance_one_token();
            tyans=INT_TYPE;}
         else
            tyans=parse_simple_def_expr(&err,boolq,FALSE,&junk);
         save_this_scratch_expr(saved.id_info_ptr);}
      if (this.tok==TK_COMMA) advance_one_token();}
   knt=store_scratch_exprs(holdexprs,holdidptrs,&reladdr_1,&reladdr_2,
                           USAGE_IN_TTO);
   if (knt<=0) error_this_pos(E_EMPTYLIST);
#ifdef BUGGY
fprintf(bugfile,"\n\ndump: scropnds_count=%d, holdopnds=%d\n",
(int) scropnds_count,(int) holdopnds);
for (kkk=0;kkk<knt;++kkk){
fprintf(bugfile,"   [%d] = ",(int) (holdopnds+kkk));
bug_print_idinfo(*(storage_scropnds+holdopnds+kkk),10,12);
fprintf(bugfile,"\n");}
fprintf(bugfile,"\n\n");
#endif
   restore_expr_ixs(holdops,holdopnds,holdidptrs,holdexprs);
   memcpy((char *) &elist,(char *) &hold_elist,sizeof(elist_type));
   /*-------------------------------------------------------*/
   (*sex).exprs = (expression_type *) reladdr_1;
   (*sex).vars = (operand_type *) reladdr_2;
   (*sex).n_vars = give_short(knt,E_PARMCOUNTOVF);
   return;
}
/*===========================================================================*/
void parse_space_expression(Boolean empty_are_ok)
/*
   This function parses the space expression and stores it in the structure
   pointed to by sex.   Upon entry, the current token must be the opening
   paren of the space expression.   Upon exit, the current token will be
   the token following the matching right paren.

   Note that the space expression does not contain any nested pointers.
   This information can be obtained from the space picture so it was omitted
   from the space expression for computational efficiency during rule 
   generation.   When a state is printed, the space picture is referenced
   in order to print parentheses in the correct positions.

*/
{
   Memix idknt,knt;
   Memix holdops,holdopnds,holdidptrs,holdexprs;
   relative_address_type reladdr_1,reladdr_2;
   /*-------------------------------------------------------*/
   remember_expr_ixs(&holdops,&holdopnds,&holdidptrs,&holdexprs);
   idknt=qq_parse_space_expr(state_space_picture_ptr,0,empty_are_ok);
   knt = store_scratch_exprs(holdexprs,holdidptrs,&reladdr_1,&reladdr_2,
                             USAGE_IN_TTO);
   restore_expr_ixs(holdops,holdopnds,holdidptrs,holdexprs);
   this_sex.n_vars = give_short(knt,E_PARMCOUNTOVF);
   this_sex.exprs = (expression_type *) reladdr_1;
   this_sex.vars = (operand_type *) reladdr_2;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_FUNCTION_statement()
{
   /*-------------------------------------------------------*/
   parse_regular_def();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_IMPLICIT_statement()
{
   /*-------------------------------------------------------*/
   parse_implicit_def();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_VARIABLE_statement()
{
   short parmcount;
   Subscript old_count;
   token_info_type nameinfo;
   Boolean is_bool;
   identifier_info_type *ppp0,*pppovf;
   /*-------------------------------------------------------*/
   ppp0 = identifier_table + nele_identifier_table;
   parmcount =
       parse_mac_parmlst_pushing_scope("VARIABLE",TRUE,FALSE,searchid_oldssv,
                                       &nameinfo,&old_count);
   if (is_bool=this_tok_is_rw(RW_BOOLEAN)) advance_one_token();
   parse_constant_def_clause(nameinfo.id,nameinfo.pos,is_bool,
                             VARIABLE_TYPE);
   pppovf = identifier_table + nele_identifier_table;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_START_statement()
{
   tranto_clause_type block;
   /*-------------------------------------------------------*/
   advance_one_token();
   if (this.tok==TK_EQ) advance_one_token();
   else show_wrn_on_both(E_MISSINGTOKEN,"=",CHAR_TYPE,SERIOUS_WARN_LEV);
   block.rate_exprs = (expression_type *) NIX_MEMIX;
   block.n_rate_exprs = 0;
   block.source_code_line_number = start_linnum = this.linnum;
   if (state_space_picture_ptr != ((state_space_picture_type *) NULL)){
      doing_start_statement=TRUE;
      parse_space_expression(FALSE);
      doing_start_statement=FALSE;}
   sys_memcpy(&block.sex,&this_sex,sizeof(space_expression_type));
   reladdr_start = (relative_address_type) store_tranto_data(block,1);
   skip_to_next_semicolon(FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define PARSE_SINGLE_EXPR_STAT_DEFS(type) \
        Boolean err; \
        short ofc; \
        relative_address_type byte_offset; \
        type block
#define PARSE_SINGLE_EXPR_STAT_KIND(op) \
        advance_one_token(); \
        block.lumping_sequence = rule_seq_ixs[op]++; \
        block.source_code_line_number=this.linnum; \
        init_and_parse_bool_expr(&err,FALSE,FALSE,&ofc); \
        byte_offset=store_this_expr(); \
        block.expr=(expression_type *) byte_offset; \
        touch_expr(&this_expr,USAGE_IN_TST); \
        byte_offset=store_booltest_data(block,1); \
        save_rule(op,byte_offset)
/*===========================================================================*/
void save_rule(opcode_type op,relative_address_type byte_offset)
{
   Memix ix;
   opcode_type tmpop;
   short iiians;
#ifdef BUGGY
Memix iii;
#endif
   /*-------------------------------------------------------*/
   ix=increment_rule_ele(1);
   *(rule_opcode_arr+ix) = op;
   *(rule_instr_byteoffs+ix) = byte_offset;
   if (op>=OPCODE_REG_MASK){
      extract_opcode_and_section(op,(short) -1,&tmpop,&iiians);
#ifdef BUGGY
if ((iiians<0)||(iiians>=RULE_OPCODE_INDEX_COUNT)){
#ifdef INTERNAL_BUGGY
fprintf(stderr,"cannot \"save_rule\" because IF or FOR improperly bit-packed");
#endif
error_this_pos(E_INTERNAL);}
else
#endif
      ++rule_opcode_sec_counts[iiians];}
   else if (op<RULE_OPCODE_INDEX_COUNT) ++rule_opcode_sec_counts[op];
#ifdef BUGGY
fprintf(bugfile,"save_rule:\n");
for (iii=0;iii<=ix;++iii){
fprintf(bugfile,"     saved[%d]=(",(int) iii);
fprintf(bugfile,"%02x->",(int) *(rule_opcode_arr+iii));
extract_opcode_and_section(*(rule_opcode_arr+iii),(short) -1,&tmpop,&iiians);
fprintf(bugfile,"%02x,%d;",(int) tmpop,(int) iiians);
if (tmpop==OPCODE_ASSERT) fprintf(bugfile,"ASSERT");
if (tmpop==OPCODE_DEATHIF) fprintf(bugfile,"DEATHIF");
if (tmpop==OPCODE_PRUNEIF) fprintf(bugfile,"PRUNEIF");
if (tmpop==OPCODE_TRANTO) fprintf(bugfile,"TRANTO");
if (tmpop==OPCODE_CALC) fprintf(bugfile,"CALC");
if (tmpop==OPCODE_START) fprintf(bugfile,"START");
if (tmpop==OPCODE_SPACE) fprintf(bugfile,"SPACE");
if (tmpop==OPCODE_BLOCK_IF) fprintf(bugfile,"IF");
if (tmpop==OPCODE_FOR_LOOP) fprintf(bugfile,"LOOP");
if (tmpop==OPCODE_BEGIN) fprintf(bugfile,"BEGIN");
if (tmpop==OPCODE_END) fprintf(bugfile,"END");
if (tmpop==OPCODE_GOTO) fprintf(bugfile,"GOTO");
if (tmpop==OPCODE_GOSUB) fprintf(bugfile,"GOSUB");
if (tmpop==OPCODE_RETURN) fprintf(bugfile,"RETURN");
if (iiians==OPCODE_ASSERT) fprintf(bugfile," [ASSERT section]");
if (iiians==OPCODE_DEATHIF) fprintf(bugfile," [DEATHIF section]");
if (iiians==OPCODE_PRUNEIF) fprintf(bugfile," [PRUNEIF section]");
if (iiians==OPCODE_TRANTO) fprintf(bugfile," [TRANTO section]");
if (iiians==OPCODE_CALC) fprintf(bugfile," [CALC-booltest section]");
if (iiians==OPCODE_CALC_T) fprintf(bugfile," [CALC-tranto section]");
fprintf(bugfile,",%08lx)\n",(long) *(rule_instr_byteoffs+iii));}
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_ASSERT_statement()
/*
   This function parses an ASSERT statement.   Upon entry, the current token
   must be the reserved word "ASSERT".  Upon exit, the current token will be
   the token following the boolean-expression for the assertion.
*/
{
   PARSE_SINGLE_EXPR_STAT_DEFS(assert_type);
   /*-------------------------------------------------------*/
   PARSE_SINGLE_EXPR_STAT_KIND(OPCODE_ASSERT);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_DEATHIF_statement()
{
   PARSE_SINGLE_EXPR_STAT_DEFS(deathif_type);
   /*-------------------------------------------------------*/
   death_present=TRUE;
   PARSE_SINGLE_EXPR_STAT_KIND(OPCODE_DEATHIF);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_PRUNEIF_statement()
{
   PARSE_SINGLE_EXPR_STAT_DEFS(pruneif_type);
   /*-------------------------------------------------------*/
   PARSE_SINGLE_EXPR_STAT_KIND(OPCODE_PRUNEIF);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_TRANTO_statement()
/*
   This function parses a TRANTO statement clause including the rate
   expression in the by clause.   Upon entry the current token must be the
   reserved word "TRANTO".   Upon exit, the current token will be the token
   immediately following the rate expression in the BY clause.

            TRANTO (NFP+1,NR-1) BY ...
            TRANTO NFP++,NR-- BY ...
*/
{
   /*-------------------------------------------------------*/
   parse_tranto_clause();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_IF_statement()
/*
   This function parses a BLOCK-IF or a IF-TRANTO statement beginning with
   the keyword "IF".   Upon entry, the current token must be the keyword
   "IF".   Upon exit, the current token will be the token following the
   "ENDIF" or the token following the rate expression in the "BY" clause of
   the tranto clause.
*/
{
   short ofc,iii;
   Boolean err,some,missing;
   block_if_type block;
   relative_address_type byte_offset;
   relative_address_type pc[RULE_OPCODE_INDEX_COUNT];   /* THEN CLAUSE */
   relative_address_type pc0[RULE_OPCODE_INDEX_COUNT];  /* THEN CLAUSE */
   relative_address_type epc[RULE_OPCODE_INDEX_COUNT];  /* ELSE CLAUSE */
   relative_address_type epc0[RULE_OPCODE_INDEX_COUNT]; /* ELSE CLAUSE */
   size_t ncop;
   char tagname[XREF_IDENT_MAXNCH_P];
   ident_usage_flag_type usg;
   Boolean *utab;
   /*-------------------------------------------------------*/
   advance_one_token();
   ncop=(size_t) (RULE_OPCODE_INDEX_COUNT*sizeof(relative_address_type));
   sys_memset(pc,(int) 0,ncop);
   sys_memset(pc0,(int) 0,ncop);
   sys_memset(epc,(int) 0,ncop);
   sys_memset(epc0,(int) 0,ncop);
   /*---------------------------------------   clear for preliminary touch */
   utab = (Boolean *) g_malloc(max_identifier_table_alloc);
   if (utab==((Boolean *) NULL)){
      error_this_pos(E_NOMEMORYP); abort_program();}
   /*----------------------------------------  parse THEN test */
   init_and_parse_bool_expr(&err,FALSE,FALSE,&ofc);
   block.then_test = (expression_type *) store_this_expr();
   /*---------------------------------------   preliminary touch */
   touch_expr_in_table(&this_expr,USAGE_SCRATCH,utab);
   /*----------------------------------------  either THEN or TRANTO */
   *tagname = '\0';
   if (this_tok_is_rw(RW_THEN)){
      sprintf(tagname,"<IF-THEN/LINE:%d>",this.linnum);
      xref_declaration(tagname); 
      advance_one_token();
      missing=parse_rule_stat_seq(pc0,pc,RW_ELSE,RW_ENDIF,-1,tagname,FALSE,FALSE);
#ifdef BUGGY
fprintf(bugfile,"after THEN looking for ELSE,");
showtoken(this.tok,this.rw,this.id);
#endif
      if ((!missing) && (this_tok_is_rw(RW_ELSE))){
         xref_utilize(tagname);
         advance_one_token();
         parse_rule_stat_seq(epc0,epc,RW_ENDIF,RW_UNUSED,-1,tagname,FALSE,FALSE);}}
   else if (this_tok_is_rw(RW_TRANTO)){
      parse_rule_stat_seq(pc0,pc,RW_UNUSED,RW_UNUSED,1,"",TRUE,FALSE);}
   else
      error_this_pos(E_NOTTHENORTRANTO);
#ifdef BUGGY
fprintf(bugfile,"done parsing nested, this.tok=");
if (this_tok_is_rw(RW_ELSE)) fprintf(bugfile,"TK_RW [RW_ELSE]\n");
else if (this_tok_is_rw(RW_ENDIF)) fprintf(bugfile,"TK_RW [RW_ENDIF]\n");
else if (this.tok==TK_SEMI) fprintf(bugfile,"TK_SEMI\n");
else fprintf(bugfile,"%d [%d]\n",(int) this.tok,(int) this.rw);
#endif
   /*----------------------------------------  multiple copies of BLOCK IF */
   usg = 0;
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii){
      /*------------------------------ iii in [ASSERT,DEATHIF,PRUNEIF,TRANTO]*/
      some=FALSE;
      block.then_clause = block.else_clause = (instruction_type *) NIX_ADDR;
      if (pc[iii]>pc0[iii]){
         block.then_clause = (instruction_type *) pc0[iii];
         some=TRUE;}
      if (epc[iii]>epc0[iii]){
         block.else_clause = (instruction_type *) epc0[iii];
         some=TRUE;}
      /*------------------------------  save copy if THEN or ELSE for iii */
      if (some){
         if (iii==((short) OPCODE_TRANTO)) usg |= USAGE_IN_TTO;
         else usg |= USAGE_IN_TST;
         byte_offset=store_blockif_data(block,1);
         save_rule(give_temporary_opcode(OPCODE_BLOCK_IF,iii),byte_offset);}}
   /*---------------------------------------   re-touch */
   for (iii=0;iii<max_identifier_table_alloc;++iii){
      if (is_a_variable(identifier_table+iii)){
         if (bitset(utab[iii],USAGE_SCRATCH)){
#ifdef OBSOLETE
/*??????*/ printf("re-touching... usage_table[%2d] = 0x%02x | 0x%02x\n",iii,(unsigned int) usage_table[iii],(unsigned int) usg);
#endif
             usage_table[iii] |= usg;}}}
   g_free(utab);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_FOR_statement()
/*
   This function parses the FOR LOOP statement beginning with the
   reserved word FOR and ending with the reserved word ENDFOR.  Upon entry,
   the current token must be the reserved word "FOR".  Upon exit, the current
   token will be the token following the "ENDFOR" which terminated the
   for loop.
*/
{
   short ofc,iii;
   char scratch_id[IDENT_MAXNCH_P];
   type_flagword_type type,comp;
   Boolean err,some,old_style,sglval;
   for_loop_type block;
   set_range_type set;
   relative_address_type reladdr_pairs[2*MAX_FOR_LOOP_PAIR];
   Memix ixpair,pairmax;
   relative_address_type byte_offset,reladdr_1,reladdr_2,reladdr,*rad;
   relative_address_type pc[RULE_OPCODE_INDEX_COUNT];   /* FOR LOOP BODY */
   relative_address_type pc0[RULE_OPCODE_INDEX_COUNT];  /* FOR LOOP BODY */
   size_t ncop;
   identifier_info_type *loopident;
   char tagname[XREF_IDENT_MAXNCH_P];
   ident_usage_flag_type usg;
   Boolean *utab;
   /*-------------------------------------------------------*/
   sprintf(tagname,"<FOR-LOOP/LINE:%d>",this.linnum);
   xref_declaration(tagname); 
   advance_one_token();
   ncop=(size_t) (RULE_OPCODE_INDEX_COUNT*sizeof(relative_address_type));
   sys_memset(pc,(int) 0,ncop);
   sys_memset(pc0,(int) 0,ncop);
   /*---------------------------------------   now inside loop */
   push_ident_scope();
   /*---------------------------------------   loop index */
   if (this.tok==TK_ID)
      loopident=searchid_dummy(this.id,INT_TYPE,this.pos);
   else{
      sprintf(scratch_id,"IERR_$_%d\n",(int) nesting_scope_level);
      loopident=searchid_dummy(scratch_id,INT_TYPE,this.pos);
      error_this_pos(E_NOTIDENT);}
   save_int_if_not_yet_in_num_tab((*loopident).ptr.vvv);
   (*loopident).flags |= FOR_LOOP_INDEX_TYPE;
   block.ident=loopident;
   advance_one_token();
   /*---------------------------------------   check for "=" VS. IN */
   if (this.tok==TK_LB) error_this_pos(E_IXONSCALAR);
   old_style=(Boolean) (this.tok==TK_EQ);
   if (old_style) warn_this_pos(E_OLDSYNTAX,MINOR_WARN_LEV);
   if ((!old_style)&&(!this_tok_is_rw(RW_IN))) error_this_pos(E_NOTIN);
   else advance_one_token();
   /*---------------------------------------   open bracket */
   if ((this.tok==TK_LB)&&(old_style)){
      error_this_pos(E_NOTIN);
      old_style=FALSE;}
   if ((!old_style)&&(this.tok==TK_LB)) advance_one_token();
   else if (!old_style) error_this_pos(E_NOTOPENBRACKET);
   /*---------------------------------------   clear for preliminary touch */
   utab = (Boolean *) g_malloc(max_identifier_table_alloc);
   if (utab==((Boolean *) NULL)){
      error_this_pos(E_NOMEMORYP); abort_program();}
   /*---------------------------------------   parse ranges */
   pairmax = (old_style ? 1 : MAX_FOR_LOOP_PAIR);
   for (ixpair=0;((ixpair<pairmax)&&(this_is_not(TK_RB)));++ixpair){
#ifdef BUGGY
fprintf(bugfile,"ixpair=%d, this=(%s,t=%d,r=%d)\n",
(int) ixpair,this.id,(int) this.tok,(int) this.rw);
#endif
      if (ixpair>0){
         if (this.tok==TK_COMMA) advance_one_token();
         else error_this_pos(E_NOTCOMMA);}
      sglval=FALSE;
      type=init_and_parse_expression(&err,FALSE,FALSE,&ofc);
      touch_expr_in_table(&this_expr,USAGE_SCRATCH,utab);
      comp = type & COMPUTATIONAL_TYPE_MASK;
      if (comp!=INT_TYPE) error_this_pos(E_NOTINTEGER);
      reladdr_2=reladdr_1=store_this_expr();
      if (old_style && (this.tok!=TK_COMMA)) error_this_pos(E_NOTCOMMA);
      else if (old_style) advance_one_token();
      else if (this.tok==TK_SUBRNGE) advance_one_token();
      else if ((this.tok==TK_COMMA)||(this.tok==TK_RB)){
         reladdr_2=reladdr_1;
         sglval=TRUE;}
      if (!sglval){
         type=init_and_parse_expression(&err,FALSE,FALSE,&ofc);
         touch_expr_in_table(&this_expr,USAGE_SCRATCH,utab);
         comp = type & COMPUTATIONAL_TYPE_MASK;
         if (comp!=INT_TYPE) error_this_pos(E_NOTINTEGER);
         reladdr_2=store_this_expr();}
      *(reladdr_pairs+((Memix) 2)*ixpair)=reladdr_1;
      *(reladdr_pairs+((Memix) 2)*ixpair+((Memix) 1))=reladdr_2;}
   /*---------------------------------------   error if no expressions */
#ifdef BUGGY
fprintf(bugfile,"ixpair=%d, this=(%s,t=%d,r=%d)\n",
(int) ixpair,this.id,(int) this.tok,(int) this.rw);
#endif
   if (ixpair<=0) error_this_pos(E_MISSINGEXPR);
   /*---------------------------------------   close bracket */
   if (!old_style){
      if (this.tok==TK_RB) advance_one_token();
      else error_this_pos(E_NOTCLOSEBRACKET);}
   /*---------------------------------------   store the set ranges */
   pairmax=ixpair;
   block.set_range_count=give_short(pairmax,E_INTERNAL);
   for (ixpair=0,rad=reladdr_pairs;ixpair<pairmax;++ixpair){
      set.lower_bound = (expression_type *) *rad++;
      set.upper_bound = (expression_type *) *rad++;
      reladdr=store_setrange_data(set,1);
      if (ixpair==0) block.set_ranges=(set_range_type *) reladdr;}
   /*---------------------------------------   parse the body */
   parse_rule_stat_seq(pc0,pc,RW_ENDFOR,RW_UNUSED,-1,tagname,FALSE,FALSE);
   /*---------------------------------------   store the for loop */
   usg = 0;
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii){
      some=FALSE;
      if (pc[iii]>pc0[iii]) some=TRUE;
      if (some){
         if (iii==((short) OPCODE_TRANTO)) usg |= USAGE_IN_TTO;
         else usg |= USAGE_IN_TST;
         block.body = (instruction_type *) pc0[iii];
         block.ident = (identifier_info_type *) relative_identifier(loopident);
         byte_offset=store_forloop_data(block,1);
         save_rule(give_temporary_opcode(OPCODE_FOR_LOOP,iii),byte_offset);}}
   /*---------------------------------------   re-touch */
   for (iii=0;iii<max_identifier_table_alloc;++iii){
      if (is_a_variable(identifier_table+iii)){
         if (bitset(utab[iii],USAGE_SCRATCH)){
#ifdef OBSOLETE
/*??????*/ printf("re-touching... usage_table[%2d] = 0x%02x | 0x%02x\n",iii,(unsigned int) usage_table[iii],(unsigned int) usg);
#endif
             usage_table[iii] |= usg;}}}
   g_free(utab);
   /*---------------------------------------   no longer inside loop */
   pop_ident_scope();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_quoted_SURE_statement()
{
   /*-------------------------------------------------------*/
   if (!correct_section(this_mode,ANY_PRE_RULE_SECTION_MODE))
      error_this_pos(E_WRONGSECTION);
   fprintf(modfile,"%s\n",scratch_string_buffer);
   advance_one_token();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_DEBUG_statement()
{
   /*-------------------------------------------------------*/
   advance_one_token();
   if (this.tok==TK_RW){
      if (this.rw==RW_ON) debug_flagword = DBG_ALL;
      else if (this.rw==RW_OFF) debug_flagword = DBG_NONE;
      else if (this.rw==RW_FULL) debug_flagword = DBG_ALL;
      else if (this.rw==RW_CLINE_ARG){
         parse_CLOPT_statement();
         goto done_pt;}
      else if ((this.rw<RW_DBG_NONE)||(this.rw>RW_DBG_ALL))
         error_this_pos(E_KEYWCONTEXT);
      else if (this.rw==RW_DBG_NONE) debug_flagword = DBG_NONE;
      else if (this.rw==RW_DBG_ALL) debug_flagword = DBG_ALL;
      else if (this.rw==RW_DBG_ABORT) abort_program();
      else 
         debug_flagword |= (debug_flag_type) 
               (DBG_ONES_BIT << (this.rw-RW_DBG_NONE-1));
      advance_one_token();}
   else if (this.tok==TK_SEMI)
      debug_flagword = DBG_ALL;
   else 
      error_this_pos(E_NOKEYWORDSEMI);
   skip_to_next_semicolon(TRUE);
done_pt:
   decide_bugfile();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_CLOPT_statement()
{
   char arg[90];
   char *argv[2];
   char *ppp;
   /*-------------------------------------------------------*/
   if (!must_assist_alloc_for_parse) warn_this_pos(E_CLOPT,SERIOUS_WARN_LEV);
   advance_one_token();
   *arg=COMMANDLINE_OPTION_PREFIX;
   *(arg+1)='\0';
   while (this_is_not(TK_SEMI)){
      ppp=this.id;
      if ((*ppp)==NUMBER_TABLE_PREFIX_CHAR) ++ppp;
      strcat(arg,ppp);
      advance_one_token();}
#ifdef BUGGY
fprintf(bugfile,"C_OPTION for \"%s\"\n",arg);
#endif
   *argv = language_command;
   *(argv+1) = arg; parse_command_line_options(2,argv);
   compute_dependent_options();
   decide_bugfile();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_tranto_clause()
/*
   This function parses a TRANTO statement clause including the rate
   expression in the by clause.   Upon entry the current token must be the
   reserved word "TRANTO".   Upon exit, the current token will be the token
   immediately following the rate expression in the BY clause.

            TRANTO (NFP+1,NR-1) BY ...
            TRANTO NFP++,NR-- BY ...
            TRANTO X[I]++,NR-- BY ...
*/
{
   tranto_clause_type block;
   short iii;
   Boolean is_fast,is_angle,err,node_format;
   type_flagword_type comp;
   relative_address_type reladdr;
   /*-------------------------------------------------------*/
   tranto_present=TRUE;
   block.source_code_line_number=this.linnum;
   /*-------------------------------------------  skip over "TRANTO" keyword */
   advance_one_token();
   /*-------------------------------------------  parse space expression(s) */
   node_format=(Boolean) (this.tok==TK_LP); 
   if (node_format){
      parse_space_expression(TRUE);
      sys_memcpy(&block.sex,&this_sex,sizeof(space_expression_type));}
   else parse_space_expr_list(&block.sex);
   /*-------------------------------------------  skip over "BY" keyword */
   if (this_tok_is_rw(RW_BY)) advance_one_token();
   else error_this_pos(E_NOTBY);
   /*-------------------------------------------  begin rate expressions */
   block.rate_exprs=(expression_type *) storage_exprs+exprs_count;
   block.n_rate_exprs=0;
   /*-------------------------------------------  look for "FAST" keyword */
   is_fast=this_tok_is_rw(RW_FAST);
   if (is_fast) advance_one_token();
   /*-------------------------------------------  look for "<" left angle */
   is_angle=(Boolean) (this.tok==TK_LT);
   if (is_angle && is_fast) error_this_pos(E_NOTWITHFAST);
   if (is_angle) advance_one_token();
   /*-------------------------------------------  parse "BY" expression list */
   for (iii=0;(iii<3)&&(is_angle||(iii==0))&&(this_is_not(TK_GT));++iii){
      if (iii>0){
         if (this.tok==TK_COMMA) advance_one_token();
         else error_this_pos(E_NOTCOMMA);}
      comp=init_and_parse_rate_expr(&err);
      touch_expr(&this_expr,USAGE_IN_TTO);
      reladdr=store_this_expr();
      if (iii==0) block.rate_exprs = (expression_type *) reladdr;}
   /*-------------------------------------------  look for ">" right angle */
   if (is_angle && (this.tok!=TK_GT))
      error_this_pos(E_NOTCLOSEANGLE);
   else if ((!is_angle) && (this.tok==TK_GT))
      show_err_at_pos_on_both(E_NOTOPENANGLE,this.pos,
                              "NO MATCHING LEFT \"<\" FOR THIS \">\"",
                              CHAR_TYPE);
   if ((is_angle) && ((iii<2)||(iii>3))) error_this_pos(E_RATECOUNTANGLE);
   else if ((!is_angle)&&(iii!=1)) error_this_pos(E_RATECOUNT);
   if (this.tok==TK_GT) advance_one_token();
   /*-------------------------------------------  finish */
   block.n_rate_exprs=iii;
   if (is_angle) block.n_rate_exprs += BY_RATE_WITH_ANGLE;
   if (is_fast) block.n_rate_exprs += BY_FAST_RATE;
   /*-------------------------------------------  store */
   reladdr=store_tranto_data(block,1);
   save_rule(OPCODE_TRANTO,reladdr);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean parse_reserved_word_stat()
{
   mode_flag_type okaymode;
   short ee;
   Boolean mode_in_error;
   rwtype rw;
   /*-------------------------------------------------------*/
   okaymode=EMPTY_MODE;
   rw=this.rw;
   if (this_tok_is_rw_stat()) okaymode = rw_okay_in_these_modes[rw];
   if (okaymode==EMPTY_MODE){
      error_this_pos(E_NOTSTMT);
      skip_to_next_semicolon(TRUE);}
   else{
      mode_in_error=TRUE;
      ee=E_NOTRULESECTION;
      if (correct_section(this_mode,SETUP_SECTION_MODE)) ee=E_NOTSETUPSECTION;
      if (correct_section(this_mode,START_SECTION_MODE)) ee=E_NOTSTARTSECTION;
      if (!correct_section(this_mode,okaymode))
         show_err_at_pos_on_both(ee,this.pos,(void *) NULL,EMPTY_TYPE);
      else
         mode_in_error=FALSE;
      if ((rw_transitions_to_this_mode[rw]==GOTO_RULE_SECTION_MODE) &&
          (!correct_section(this_mode,RULE_SECTION_MODE))){
         /*-------------------------------------  GOTO RULE before parsing */
         this_mode=rw_transitions_to_this_mode[rw];
         if (space_statement_missing){
            space_statement_missing=FALSE;
            show_err_at_pos_on_both(E_NOSPACE,0,(void *) NULL,EMPTY_TYPE);}
         if (start_statement_missing){
            start_statement_missing=FALSE;
            show_err_at_pos_on_both(E_NOSTART,0,(void *) NULL,EMPTY_TYPE);}
         return(TRUE);}
      else{
         /*-------------------------------------  parse */
         if (rw==RW_SPACE) space_statement_missing=FALSE;
         if (rw==RW_START) start_statement_missing=FALSE;
         parse_statement_routine[rw]();
         if (rw_transitions_to_this_mode[rw]!=SAME_MODE){
            this_mode=rw_transitions_to_this_mode[rw];}}}
   /*-------------------------------------------------------*/
   return(FALSE);
}
/*===========================================================================*/
void parse_a_statement(Boolean retain_semicolon,token tok_retain)
{
   /*-------------------------------------------------------*/
   flushed=FALSE;
   if (must_assist_alloc_for_parse &&
       (!this_tok_is_rw(RW_CLOPT)) &&
       (!this_tok_is_rw(RW_DEBUG)))
       assist_alloc_for_parse();
   start_independent_of_check();
   /*---------------------------------------  */
#ifdef BUGGY
bug_print_identifier_table();
#endif
   if (this.tok==TK_ID)
      /*---------------------------  constant def statement */
      parse_constant_definition_stat();
   else if (this.tok==TK_CHAR){
      /*---------------------------  quoted SURE statement (no semicolon) */
      parse_quoted_SURE_statement();
      return;}
   else if (this.tok==TK_RW){
      /*---------------------------  other statement */
      if (parse_reserved_word_stat()) goto finish;}
   else if (this.tok!=TK_SEMI){
      /*---------------------------  invalid statement */
      error_this_pos(E_NOTSTMT);
      skip_to_next_semicolon(TRUE);}
   /*---------------------------------------  check for semicolon */
   if (this.tok==tok_retain);
   else if (!retain_semicolon){
      if (this_tok_is_rw_stat_or_eoseq())
         show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,";",CHAR_TYPE);
      else{
         parse_must_be_semicolon();
         advance_one_token();}}
   /*---------------------------------------  */
finish:
   finish_independent_of_check();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_setup_section()
{
   /*-------------------------------------------------------*/
   while ((this.tok!=TK_EOF)&&(correct_section(this_mode,SETUP_SECTION_MODE)))
      parse_a_statement(FALSE,TK_UNUSED);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_start_section()
{
   /*-------------------------------------------------------*/
   while ((this.tok!=TK_EOF)&&(correct_section(this_mode,START_SECTION_MODE)))
      parse_a_statement(FALSE,TK_UNUSED);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void cat_rule_terminator(char *orlist,rwtype rw)
{
   /*-------------------------------------------------------*/
   if (rw==RW_ENDIF) strcat(orlist,"ENDIF");
   if (rw==RW_ELSE) strcat(orlist,"ELSE");
   if (rw==RW_ENDFOR) strcat(orlist,"ENDFOR");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean parse_rule_stat_seq(relative_address_type *pc0,
                            relative_address_type *pc,
                            rwtype quit1,rwtype quit2,short n,
                            char *tagname,
                            Boolean retain_semicolon,Boolean toplevel)
/*
   This function parses a <rule-stat-seq>, storing the parsed instruction
   data into the object file and saving the parsed instructions themselves
   in scratch memory on the ready instruction lists:
            rule_opcode_arr[]
            rule_instr_byteoffs[]

   Upon entry, the current token will be the first token of the first
   statement in the <rule-stat-seq> or the first token of the single statement
   when both (n==1) and (retain_semicolon==TRUE).

   Upon exit, the current token will be the first token following the
   quit (quit1 or quit2) token or the first token following the semicolon
   on the single statement when (n==1) unless:
       (*) both quit1 and quit2 are applicable and the quit1 token
           terminates the sequence in which case the current token
           will be the quit1 token.
       (*) the semicolon is retained in which case the current token
           will be the retained token.

   Inputs:
       quit1     --- Specification of a token which will terminate
                     parsing of the <rule-stat-seq>.   Specified as
                     RW_UNUSED if not applicable.   If both quit1 and
                     quit2 are applicable, then the quit1 token is retained
                     if encountered.
       quit2     --- Specification of another token which will terminate
                     parsing of the <rule-stat-seq>.   Specified as
                     RW_UNUSED if not applicable.   Must be applicable
                     only if quit1 is also applicable.
       n         --- Specifies number of statements which are contained
                     in the <rule-stat-seq>.   Specified as n=1 for
                     IF <expr> TRANTO ...;  and as n=(-1) for sequences
                     with an undetermined number of statements contained.
       tagname   --- Specifies identifier to reference when quit1 is
                     encountered and quit2 is RW_UNUSED.   Specify as ""
                     if not applicable.
       retain_   --- Flag which tells whether or not to retain the semicolon
       semicolon     terminating the last statement in the <rule-stat-seq>.
                     If retained, another routine which expects to find
                     the semicolon will in fact find it.

   Outputs:
       pc0[]     --- Array of relative addresses pointing to where
                     each of the sections [ASSERT,DEATHIF,PRUNEIF,TRANTO,CALC,CALC_T]
                     begin for this <rule-stat-seq>.
       pc[]      --- Array of relative addresses pointing to where
                     each of the sections [ASSERT,DEATHIF,PRUNEIF,TRANTO,CALC,CALC_T]
                     end for this <rule-stat-seq>.

   Globals:
       rule_count ---
                     count of elements in rule_opcode_arr[] and in
                     rule_instr_byteoffs[].   It acts like a top-of-stack
                     pointer.
       rule_opcode_arr[]  ---
                     Stack of opcodes containing the instructions at the
                     current and at all invoking levels of nested rule
                     statement sequences.   During parsing of the new
                     nested <rule-stat-seq> level, instructions are pushed
                     onto the stack.  After parsing the <rule-stat-seq>,
                     only those instructions which were added at the new
                     level are unloaded and stored into the object file
                     and the stack is restored to its original state by
                     setting rule_count back to the value it had upon entry.
       rule_instr_byteoffs[]  ---
                     Stack of relative address byte-offsets which correspond
                     to the instructions in rule_opcode_arr[].
       rule_opcode_sec_counts[]  ---
                     Array of counts of occurrences of statements in
                     sections [ASSERT,DEATHIF,PRUNEIF,TRANTO,CALC,CALC_T].
                     Upon entry this array contains the counts encountered for
                     the invoking <rule-stat-seq> level up to the time of the
                     invocation.  It is immediately pushed onto the stack
                     and zeroed.  Upon exit, the stack values are popped
                     and added to the counts found during parsing the nested
                     <rule-stat-seq>.
*/
{
   Boolean missing_semi;
   long old_rule_opcode_sec_counts[RULE_OPCODE_INDEX_COUNT];
   short jjj,iii,iiians,nnn;
   Memix old_rule_count;
   long ix,ixe;
   instruction_type inst;
   opcode_type tmpop;
   Boolean op_match,logic_op,section_match;
   size_t ncop;
   char orlist[92];
   /*-------------------------------------------------------*/
   missing_semi=FALSE;
   ncop=(size_t) (sizeof(long)*RULE_OPCODE_INDEX_COUNT);
   if (bitset(debug_flagword,DBG_DST)) bug_print_identifier_table();
   old_rule_count=rule_count;
   sys_memcpy(old_rule_opcode_sec_counts,rule_opcode_sec_counts,ncop);
   sys_memset(rule_opcode_sec_counts,(int) 0,ncop);
#ifdef BUGGY
for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii)
fprintf(bugfile,
"get ready ... iii=%d, cumpc[%d]=%d, old[%d]=%d now[%d]=%d\n",
(int) iii,(int) iii,(int) cumpc[iii],
(int) iii,(int) old_rule_opcode_sec_counts[iii],
(int) iii,(int) rule_opcode_sec_counts[iii]);
#endif
   for (jjj=0,nnn=n;correct_section(this_mode,RULE_SECTION_MODE)&&(jjj!=nnn)&&
                    (!this_tok_is_rw(quit1))&&
                    (!this_tok_is_rw(quit2))&&
                    (this.tok!=TK_EOF);++jjj){
#ifdef BUGGY
fprintf(bugfile,"Statement for ::: ");
showtoken(this.tok,this.rw,this.id);
#endif
      parse_a_statement(retain_semicolon,
                        (token) ((retain_semicolon) ? TK_SEMI : TK_UNUSED));}
#ifdef BUGGY
fprintf(bugfile,"No longer a statement for ::: ");
showtoken(this.tok,this.rw,this.id);
fprintf(bugfile,
" ::: quit=%d,%d  retain=%d\n",
(int) quit1,(int) quit2,(int) retain_semicolon);
#endif
   if (this.tok==TK_EOF){
      *orlist='\0';
      if (quit1!=RW_UNUSED) cat_rule_terminator(orlist,quit1);
      if (quit2!=RW_UNUSED){
         strcat(orlist," OR ");
         cat_rule_terminator(orlist,quit2);}
      if ((*orlist)!='\0'){
         strcat(orlist," MISSING.");
         show_err_at_pos_on_both(E_UNTERMBODY,this.pos,orlist,CHAR_TYPE);}}
   else if (this_tok_is_rw(quit1) && (quit2==RW_UNUSED)){
      if ((*tagname)!='\0') xref_utilize(tagname);
      advance_one_token();}
   else if (this_tok_is_rw(quit2) && (quit2!=RW_UNUSED)){
      if ((*tagname)!='\0') xref_utilize(tagname);
      advance_one_token();
      if (this_tok_is_rw(quit1)) missing_semi=TRUE;}
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii) pc0[iii]=cumpc[iii];
   if (toplevel) assist_finish_varinsts();
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii){
#ifdef BUGGY
fprintf(bugfile,
"go ... iii=%d, cumpc[%d]=%d, old[%d]=%d now[%d]=%d\n",
(int) iii,(int) iii,(int) cumpc[iii],
(int) iii,(int) old_rule_opcode_sec_counts[iii],
(int) iii,(int) rule_opcode_sec_counts[iii]);
#endif
      if (rule_opcode_sec_counts[iii]>0L){
         /*-----------------------------  store body as subroutine */
         ixe=rule_count;
         for (ix=old_rule_count;ix<ixe;++ix){
            extract_opcode_and_section(*(rule_opcode_arr+ix),iii,
                                       &tmpop,&iiians);
#ifdef BUGGY
fprintf(bugfile,"tmpop=%d, iii=%d\n",(int) tmpop,(int) iii);
#endif
            op_match = (Boolean) (tmpop==((opcode_type) iii));
            logic_op = (Boolean)
                           (tmpop>=((opcode_type) RULE_OPCODE_INDEX_COUNT));
            section_match = (Boolean) (iii==iiians);
            if ((op_match || logic_op) && (section_match)){
               inst.opcode=tmpop;
               inst.ptr.reladdr = *(rule_instr_byteoffs+ix);
               if (section_match){
                  STORE_CODE(inst,iiians);}}}
         /*-----------------------------  RETURN from body */
         inst.opcode=OPCODE_RETURN;
         inst.ptr.reladdr=NIX_ADDR;
         STORE_CODE(inst,iii);}
      pc[iii]=cumpc[iii];}
   rule_count=old_rule_count;
#ifdef BUGGY
fprintf(bugfile,"Done for ::: ");
showtoken(this.tok,this.rw,this.id);
#endif
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii)
      rule_opcode_sec_counts[iii] += old_rule_opcode_sec_counts[iii];
   /*-------------------------------------------------------*/
   return(missing_semi);
}
/*===========================================================================*/
#ifdef OBSOLETE
void touch_all_variable_usages(instruction_type *inst,short iii)
{
   ident_usage_flag_type usg;
   /*-------------------------------------------------------*/
   usg = USAGE_IN_TST;
   if (iii==((short) OPCODE_TRANTO)) usg=USAGE_IN_TTO;
   switch ((*inst).opcode){
   case OPCODE_ASSERT:
   case OPCODE_DEATHIF:
   case OPCODE_PRUNEIF:
        touch_btest((booltest_type *) (*inst).ptr.vvv,usg);
        break;
   case OPCODE_TRANTO:
        touch_tranto((tranto_clause_type *) (*inst).ptr.vvv,usg);
        break;
   case OPCODE_BLOCK_IF:
        touch_blockif((block_if_type *) (*inst).ptr.vvv,usg);
        break;
   case OPCODE_FOR_LOOP:
        touch_for((for_loop_type *) (*inst).ptr.vvv,usg);
        break;
   }
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
#ifdef OBSOLETE
void touch_btest(booltest_type *btest,ident_usage_flag_type usg)
{
   /*-------------------------------------------------------*/
   touch_expr((*btest).expr,usg);
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
#ifdef OBSOLETE
void touch_tranto(tranto_clause_type *tto,ident_usage_flag_type usg)
{
   short kkk,nnn;
   expression_type *e;
   operand_type *ooo;
   Boolean is_array,is_double;
   /*-------------------------------------------------------*/
   ooo=(*tto).sex.vars;
   for (kkk=0,nnn=(*tto).sex.n_vars,e=(*tto).sex.exprs;kkk<nnn;++kkk,++e){
      touch_expr(e,usg);
      DECIDE_IS_ARRAY_IS_DOUBLE((*ooo));
      if (is_array){
         ++e;
         touch_expr(e,usg);
         if (is_double){
            ++e;
            touch_expr(e,usg);}}
      ++ooo;}
   for (kkk=0,nnn=(*tto).n_rate_exprs,e=(*tto).rate_exprs;kkk<nnn;++kkk,++e){
      touch_expr(e,usg);}
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
#ifdef OBSOLETE
void touch_blockif(block_if_type *bif,ident_usage_flag_type usg)
{
   /*-------------------------------------------------------*/
   touch_expr((*bif).then_test,usg);
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
#ifdef OBSOLETE
void touch_for(for_loop_type *f,ident_usage_flag_type usg)
{
   short kkk,nnn;
   set_range_type *s;
   /*-------------------------------------------------------*/
   for (kkk=0,nnn=(*f).set_range_count,s=(*f).set_ranges;kkk<nnn;++kkk,++s){
      touch_expr((*s).lower_bound,usg);
      touch_expr((*s).upper_bound,usg);}
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
void touch_expr_in_table
    (expression_type *expr,ident_usage_flag_type usg,Boolean *utab)
{
   Boolean *holdutab;
   /*-------------------------------------------------------*/
   holdutab = usage_table;
   usage_table = utab;
   touch_expr(expr,usg);
   usage_table = holdutab;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void touch_expr(expression_type *expr,ident_usage_flag_type usg)
{
   Memix iii,nnn,jjj;
   Memix ixid;
   operand_type nowopnd;
   /*-------------------------------------------------------*/
   nnn=(*expr).n_operands;
   for (iii=0;iii<nnn;++iii){
      nowopnd = (*expr).operands[iii];
      ixid = relative_identifier(nowopnd);
#ifdef OBSOLETE
/*??????*/ printf("touching... usage_table[%2d] = 0x%02x | 0x%02x",ixid,(unsigned int) usage_table[ixid],(unsigned int) usg);
#endif
      usage_table[ixid] |= usg;
#ifdef OBSOLETE
/*??????*/ printf(" = 0x%02x (\"%s\")\n",(unsigned int) usage_table[ixid],(*nowopnd).name);
#endif
      if (is_a_variable(nowopnd)){
          for (jjj=0;jjj<varinsts_count;++jjj){
             if (storage_varcalcs[jjj].idinfo == nowopnd)
                 touch_expr(storage_varcalcs[jjj].expr,usg);}}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_rule_section()
{
   relative_address_type pc0[RULE_OPCODE_INDEX_COUNT];
   relative_address_type pc[RULE_OPCODE_INDEX_COUNT];
   short iii;
   Boolean some;
   instruction_type inst;
   size_t ncop;
   /*-------------------------------------------------------*/
   ncop=(size_t) (RULE_OPCODE_INDEX_COUNT*sizeof(relative_address_type));
   sys_memset(cumpc,(int) 0,ncop);
   sys_memset(rule_opcode_sec_counts,(int) 0,ncop);
   sys_memcpy(pc,cumpc,ncop);
   sys_memcpy(pc0,cumpc,ncop);
   parse_rule_stat_seq(pc0,pc,RW_UNUSED,RW_UNUSED,-1,"",FALSE,TRUE);
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii){
      some=FALSE;
      if (pc[iii]>pc0[iii]) some=TRUE;
      if (!some){
         inst.opcode=OPCODE_RETURN;
         inst.ptr.reladdr=NIX_ADDR;
         STORE_CODE(inst,iii);}
      inst.opcode=OPCODE_BEGIN;
      inst.ptr.reladdr=pc0[iii];
      store_preamble_code(inst);}
#ifdef BUGGY
fprintf(bugfile,"Very Done for ::: ");
showtoken(this.tok,this.rw,this.id);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_program()
{
   Memix iii;
   /*-------------------------------------------------------*/
   space_statement_missing=start_statement_missing=TRUE;
   death_present=tranto_present=FALSE;
   this_mode=SETUP_SECTION_MODE;
   for (iii=0;iii<RULE_OPCODE_INDEX_COUNT;++iii) rule_opcode_sec_counts[iii]=0;
   parse_setup_section();
   parse_start_section();
   parse_rule_section();
   if (!death_present) warn_this_pos(E_NODEATHS,DEFAULT_WARN_LEV);
   if (!tranto_present) error_this_pos(E_NOTRANTOS);
   list_page_need(1); fprintf(listfile,"\n");
#ifdef BUGGY
fprintf(bugfile,"To abort maybe ::: ");
showtoken(this.tok,this.rw,this.id);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void single_code_map_section(object_table_type ooo)
{
   for_all_defs;
   instruction_type *insts,*instsovf,inst;
   opcode_type op;
   Boolean begin;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"section for ooo=%d, OBJ_CODE_0=%d\n",
(int) ooo,(int) OBJ_CODE_0);
#endif
   op=0;
   new_map_section(code_titles[ooo-OBJ_CODE_0],((void *) NULL));
   for_all_struct_storage(ooo,insts,1,instruction_type){
      instsovf = (instruction_type *) ovfu.ccc;
      for_all_on_line(insts,instsovf){
         sys_memcpy(&inst,(insts+jjj),sizeof(instruction_type));
         begin=FALSE;
         if (inst.opcode==OPCODE_ASSERT) fprintf(listfile,"ASSERT");
         else if (inst.opcode==OPCODE_DEATHIF) fprintf(listfile,"DEATHIF");
         else if (inst.opcode==OPCODE_PRUNEIF) fprintf(listfile,"PRUNEIF");
         else if (inst.opcode==OPCODE_TRANTO) fprintf(listfile,"TRANTO");
         else if (inst.opcode==OPCODE_CALC) fprintf(listfile,"CALC");
         else if (inst.opcode==OPCODE_START) fprintf(listfile,"START");
         else if (inst.opcode==OPCODE_SPACE) fprintf(listfile,"SPACE");
         else if (inst.opcode==OPCODE_BLOCK_IF) fprintf(listfile,"IF");
         else if (inst.opcode==OPCODE_FOR_LOOP) fprintf(listfile,"LOOP");
         else if (inst.opcode==OPCODE_BEGIN){
            fprintf(listfile,"BEGIN");
            begin=TRUE;}
         else if (inst.opcode==OPCODE_END) fprintf(listfile,"END");
         else if (inst.opcode==OPCODE_GOTO) fprintf(listfile,"GOTO");
         else if (inst.opcode==OPCODE_GOSUB) fprintf(listfile,"GOSUB");
         else if (inst.opcode==OPCODE_RETURN) fprintf(listfile,"RETURN");
         else fprintf(listfile,"????");
         if (inst.ptr.vvv != ((void *) NULL)){
            fprintf(listfile," ");
            fshew_map_memix(listfile,inst.ptr.vvv);
            if (begin){
              fprintf(listfile," ! ");
              if (op==OPCODE_ASSERT) fprintf(listfile,"ASSERT");
              else if (op==OPCODE_DEATHIF) fprintf(listfile,"DEATHIF");
              else if (op==OPCODE_PRUNEIF) fprintf(listfile,"PRUNEIF");
              else if (op==OPCODE_TRANTO) fprintf(listfile,"TRANTO");
              else if (op==OPCODE_CALC) fprintf(listfile,"CALC-booltest");
              else if (op==OPCODE_CALC_T) fprintf(listfile,"CALC-transition");
              fprintf(listfile," section");
              ++op;}}}
      fprintf(listfile,"\n");}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void application_map_sections()
{
   for_all_defs;
   object_table_type ooo,oooend;
   set_range_type *sets,*setsovf,set;
   state_space_picture_type *pixs,*pixsovf;
   booltest_type *tests,*testsovf,test;
   tranto_clause_type *tnts,*tntsovf,tnt;
   block_if_type *bifs,*bifsovf,bif;
   for_loop_type *forngs,*forngsovf,forng;
   vars_union_type *vrus,*vrusovf,vru;
   calc_assign_type *clcs,*clcsovf,clc;
   char symbolic[60];
   /*-------------------------------------------------------*/
   /*-------------------------------------  VAR INF */
   if (memory_bytes_required[OBJ_VARINF]>0){
      new_map_section("Space variable information",((void *) NULL));
      for_all_struct_storage(OBJ_VARINF,vrus,4,vars_union_type){
         vrusovf = (vars_union_type *) ovfu.ccc;
         for_all_on_line(vrus,vrusovf){
            if (jjj>0) fprintf(listfile,"   ");
            sys_memcpy(&vru,(vrus+jjj),sizeof(vars_union_type));
            if ((vru.id_info>=identifier_table)&&
                (vru.id_info<(identifier_table+nele_identifier_table))){
                sprintf(symbolic,"<%d>",
                        (int) relative_identifier(vru.id_info));
                sys_memset(symbolic+strlen(symbolic),(int) ' ',HEX_DEC_PLACES);
                *(symbolic+HEX_DEC_PLACES)='\0';
                fprintf(listfile,symbolic);}
            else fshew_map_memix(listfile,vru.nested_space_picture);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  SET RANGE */
   if (memory_bytes_required[OBJ_SETRNGE]>0){
      new_map_section("FOR set range expression pointers",((void *) NULL));
      for_all_struct_storage(OBJ_SETRNGE,sets,2,set_range_type){
         setsovf = (set_range_type *) ovfu.ccc;
         for_all_on_line(sets,setsovf){
            if (jjj>0) fprintf(listfile,"   ");
            sys_memcpy(&set,(sets+jjj),sizeof(set_range_type));
            fprintf(listfile,STRLP);
            fshew_map_memix(listfile,set.lower_bound);
            fprintf(listfile," .. ");
            fshew_map_memix(listfile,set.upper_bound);
            fprintf(listfile,STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  PICTURE */
   if (memory_bytes_required[OBJ_PIX]>0){
      new_map_section("state-space PICTURE data",((void *) NULL));
      for_all_struct_storage(OBJ_PIX,pixs,1,state_space_picture_type){
         pixsovf = (state_space_picture_type *) ovfu.ccc;
#ifdef BUGGY
fprintf(bugfile,"(*pixs).varu=%08lx",(long) (*pixs).varu);
fprintf(bugfile,"(*pixs).is_nested=%08lx",(long) (*pixs).is_nested);
fprintf(bugfile,"(*pixs).nvaru=%d",(int) (*pixs).nvaru);
#endif
         for_all_on_line(pixs,pixsovf){
            print_space_picture(listfile,pixs+jjj);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  BOOLEAN TEST DATA */
   if (memory_bytes_required[OBJ_BOOLTEST]>0){
      new_map_section("ASSERT/DEATHIF/PRUNEIF boolean tests",((void *) NULL));
      for_all_struct_storage(OBJ_BOOLTEST,tests,4,booltest_type){
         testsovf = (booltest_type *) ovfu.ccc;
         for_all_on_line(tests,testsovf){
            if (jjj>0) fprintf(listfile,"   ");
            sys_memcpy(&test,(tests+jjj),sizeof(booltest_type));
            fprintf(listfile,"%sexpr=",STRLP);
            fshew_map_memix(listfile,test.expr);
            fprintf(listfile,",line#%d%s",
                    (int) test.source_code_line_number,STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  TRANTO DATA */
   if (memory_bytes_required[OBJ_TRANTO]>0){
      new_map_section("TRANTO clause data structures",((void *) NULL));
      for_all_struct_storage(OBJ_TRANTO,tnts,1,tranto_clause_type){
         tntsovf = (tranto_clause_type *) ovfu.ccc;
         for_all_on_line(tnts,tntsovf){
            sys_memcpy(&tnt,(tnts+jjj),sizeof(tranto_clause_type));
            fprintf(listfile,"%sTRANTO %sexprs=",STRLP,STRLP);
            fshew_map_memix(listfile,tnt.sex.exprs);
            fprintf(listfile,",vars=");
            fshew_map_memix(listfile,tnt.sex.vars);
            fprintf(listfile,",#vars=%d%s BY ",(int) tnt.sex.n_vars,STRRP);
            if (bitset(tnt.n_rate_exprs,BY_FAST_RATE))
               fprintf(listfile,"FAST ");
            if (bitset(tnt.n_rate_exprs,BY_RATE_WITH_ANGLE))
               fprintf(listfile,"<");
            fprintf(listfile,"%d@",
                    (int) (tnt.n_rate_exprs & RATE_EXPRESSION_COUNT_MASK));
            fshew_map_memix(listfile,tnt.rate_exprs);
            if (bitset(tnt.n_rate_exprs,BY_RATE_WITH_ANGLE))
               fprintf(listfile,">");
            fprintf(listfile," (line#%d)%s",
                    (int) tnt.source_code_line_number,STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  BLOCK IF DATA */
   if (memory_bytes_required[OBJ_IF]>0){
      new_map_section("block and tranto IF data structures",((void *) NULL));
      for_all_struct_storage(OBJ_IF,bifs,1,block_if_type){
         bifsovf = (block_if_type *) ovfu.ccc;
         for_all_on_line(bifs,bifsovf){
            sys_memcpy(&bif,(bifs+jjj),sizeof(block_if_type));
            fprintf(listfile,"%sIF ",STRLP);
            fshew_map_memix(listfile,bif.then_test);
            fprintf(listfile," THEN GOSUB ");
            fshew_map_memix(listfile,bif.then_clause);
            if (bif.else_clause != ((instruction_type *) NULL)){
               fprintf(listfile," ELSE GOSUB ");
               fshew_map_memix(listfile,bif.else_clause);}
            fprintf(listfile,"%s",STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  FOR RANGE DATA */
   if (memory_bytes_required[OBJ_FOR]>0){
      new_map_section("FOR range data",((void *) NULL));
      for_all_struct_storage(OBJ_FOR,forngs,1,for_loop_type){
         forngsovf = (for_loop_type *) ovfu.ccc;
         for_all_on_line(forngs,forngsovf){
            sys_memcpy(&forng,(forngs+jjj),sizeof(for_loop_type));
#ifdef BUGGY
fprintf(bugfile,
"forng=(ident=%08lx,sets=%08lx,count=%d,body=%08lx)\n",
(long) forng.ident,
(long) forng.set_ranges,
(int) forng.set_range_count,
(long) forng.body);
#endif
            fprintf(listfile,"%s%s<%d> IN ",STRLP,(*forng.ident).name,
                    (int) relative_identifier(forng.ident));
            fprintf(listfile,"%s%d@",STRLB,(int) forng.set_range_count);
            fshew_map_memix(listfile,forng.set_ranges);
            fprintf(listfile,"%s",STRRB);
            fprintf(listfile," GOSUB ");
            fshew_map_memix(listfile,forng.body);
            fprintf(listfile,"%s",STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  CALC DATA */
   if (memory_bytes_required[OBJ_CALC]>0){
      new_map_section("CALC assignment data structures",((void *) NULL));
      for_all_struct_storage(OBJ_CALC,clcs,1,calc_assign_type){
         clcsovf = (calc_assign_type *) ovfu.ccc;
         for_all_on_line(clcs,clcsovf){
            sys_memcpy(&clc,(clcs+jjj),sizeof(calc_assign_type));
            fprintf(listfile,"%sCALC %s<%d> = @",
                             STRLP,(*clc.idinfo).name,
                             (int) relative_identifier(clc.idinfo));
            fshew_map_memix(listfile,clc.expr); 
         fprintf(listfile,"%s",STRRP);}
         fprintf(listfile,"\n");}}
   /*-------------------------------------  CODE */
   oooend=OBJ_CODE_0+((object_table_type) RULE_OPCODE_INDEX_COUNT);
   for (ooo=OBJ_CODE_0;ooo<=oooend;++ooo) single_code_map_section(ooo);
   /**/
   fprintf(listfile,"\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void extract_opcode_and_section
      (opcode_type temp,short iii,opcode_type *opcode,short *section)
{
   register opcode_type sec,ans;
   /*-------------------------------------------------------*/
   ans = temp;
   sec = ((opcode_type) ((ans >> OPCODE_SEC_SHIFT) & OPCODE_REG_MASK));
   ans = ((opcode_type) (ans & OPCODE_REG_MASK));
   /*-------------------------------------------------------*/
   *opcode = ans;
   *section = ((sec>0) ? ((short) (sec-1)) : iii);
}
/*===========================================================================*/
opcode_type give_temporary_opcode(opcode_type opcode,short section)
{
   register opcode_type sec,ans;
   /*-------------------------------------------------------*/
   sec = (opcode_type) (1+section);
   ans = (opcode_type) ((sec << OPCODE_SEC_SHIFT) | opcode);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void show_prs_sizes_on_both()
/*
   This function displays the minimum parse stage storage requirements
   for the ASSIST/SURE/PAWS source file which has just been parsed.
   Statistics ar printed only if at least one of the following debug
   flags is set:
                  DEBUG$ LEX$;
                  DEBUG$ PARSE$;
*/
{
   static char fmt_counts[] = "%04d %s ELEMENT%s USED.\n";
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_LEX|DBG_PRS)){
      list_page_need(6);
      both_printf("\n");
      show_with_fmt_on_both(fmt_counts,(short) function_body_size_used,
                            "FUNCTION BODY STORAGE");
      show_with_fmt_on_both(fmt_counts,(short) operand_max_size_used,
                            "OPERAND LIST");
      show_with_fmt_on_both(fmt_counts,(short) infix_max_size_used,
                            "INFIX LIST");
      show_with_fmt_on_both(fmt_counts,(short) postfix_max_size_used,
                            "POSTFIX STACK");}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void expression_map_sections()
{
   for_all_defs;
   Memix diff;
   short nch,nadd;
   operation_type *ops,*opsovf,op;
   operand_type *opnds,*opndsovf,opnd;
   expression_type *xprs,*xprsovf,xpr;
   char symbolic[90+MAX_INT_TYPE_NDIGS];
   /*-------------------------------------------------------*/
   /*-------------------------------------  EXPRS */
   new_map_section("Expressions",((void *) NULL));
   for_all_struct_storage(OBJ_EXPR,xprs,1,expression_type){
      xprsovf = (expression_type *) ovfu.ccc;
      for_all_on_line_set(xprs,xprsovf,3){
         if (jjj>0) fprintf(listfile,"   ");
         sys_memcpy(&xpr,(xprs+jjj),sizeof(expression_type));
         fprintf(listfile,"[post=(");
         fprintf_map_memix(listfile,(char *) xpr.postfix_ops);
         fprintf(listfile,",%d),in=(",(int) xpr.n_postfix_ops);
         fprintf_map_memix(listfile,(char *) xpr.infix_ops);
         fprintf(listfile,",%d),op=(",(int) xpr.n_infix_ops);
         fprintf_map_memix(listfile,(char *) xpr.operands);
         fprintf(listfile,",%d),",(int) xpr.n_operands);
         fprintf(listfile,"\n                 line#=%d,err=%c,",
                 (int) xpr.source_code_line_number,
                 ((xpr.in_error) ? 'T' : 'F'));
         make_symbolic_type_list(symbolic,xpr.rtntype);
         fprintf(listfile,"returns(0x%02x,(%s))]\n",
                 (unsigned int) xpr.rtntype,symbolic);
         fprintf(listfile,"                 ");
         list_page_need(print_infix_expr(listfile,&xpr,17,17,
                                         list_line_width,FALSE,FALSE));}
      fprintf(listfile,"\n");}
   /*-------------------------------------  OPERANDS */
#ifdef BUGGY
print_identifier_table(bugfile,TRUE);
#endif
   new_map_section("Expression operands",((void *) NULL));
   for_all_struct_storage(OBJ_OPERANDS,opnds,4,operand_type){
      opndsovf = (operand_type *) ovfu.ccc;
      for_all_on_line(opnds,opndsovf){
         opnd = *(opnds+jjj);
#ifdef BUGGY
fprintf(bugfile,"...idtab=%08lx,  opnd=%08lx, jjj=%ld\n",
(long) identifier_table,(long) opnd,(long) jjj);
#endif
         diff = relative_identifier(opnd);
         strcpy(symbolic,(*opnd).name);
         sprintf(symbolic+70,"<%d>",(int) diff); nch=strlen(symbolic+70);
         *(symbolic+17-nch)='\0';
         strcat(symbolic,symbolic+70);
         nadd=18-(nch=strlen(symbolic));
         if (nch>0){
            sys_memmove(symbolic+nadd,symbolic,(nch+1));
            sys_memset(symbolic,(int) ' ',nadd);}
         fprintf(listfile,symbolic);}
      fprintf(listfile,"\n");}
#ifdef BUGGY
print_identifier_table(bugfile,TRUE);
#endif
   /*-------------------------------------  OPS */
   new_map_section("Expression operations",((void *) NULL));
   for_all_struct_storage(OBJ_OPS,ops,32,operation_type){
      opsovf = (operation_type *) ovfu.ccc;
      for_all_on_line(ops,opsovf){
         if ((jjj>0)&&((jjj%8)==0)) fprintf(listfile,"   ");
         else if (jjj>0) fprintf(listfile," ");
         op = *(ops+jjj);
         show_operation_text(listfile,op,TRUE);}
      fprintf(listfile,"\n");}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void do_switch_to_stdin(char *prompt,short ixlevel)
{
   /*-------------------------------------------------------*/
   sys_memcpy(&tok_saved_during_switch[ixlevel],&this,sizeof(token_info_type));
   switch_to_stdin(prompt,ixlevel);
   advance_one_token();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void do_switch_back_to_inputfile(short ixlevel)
{
   /*-------------------------------------------------------*/
   switch_back_to_inputfile(ixlevel);
   sys_memcpy(&this,&tok_saved_during_switch[ixlevel],sizeof(token_info_type));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Memix increment_body()
/*
   This function increments the body index for the function definition
   storage array.
*/
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans = function_body_count;
   ++function_body_count;
   if (function_body_count>function_body_size_used)
      function_body_size_used=function_body_count;
   if (ans == max_body_count){
      error_this_pos(E_FUNCBODYOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void save_function_body_token(token_info_type *tokinf)
/*
   This function saves the given token information on the function body list.
   It is used to retain all FUNCTION/IMPLICIT body tokens during the parse
   stage as well as to save all tokens for all calling parameters during
   subsequent expansion from an expression.
*/
{
   Memix ixop;
   token_info_type *what;
   /*-------------------------------------------------------*/
   if ((ixop = increment_body()) < max_body_count){
      what = function_body_storage + ixop;
      sys_memcpy((char *) what,(char *) tokinf,sizeof(token_info_type));}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void user_show_extra_after_error(operand_type l_opnd,int_type iii,int_type jjj)
{
   show_rule_info_after_error(l_opnd,iii,jjj,NULL,NULL,0,NULL);
}
/*===========================================================================*/
void logfile_storage_map(char *allstorage)
{
   object_table_type ooo_off;
   short need,ixj;
   for_all_defs_no_struct;
   char *sss;
   char holdch;
   void *vvv;
   /*-------------------------------------------------------*/
   set_map_addr0(section_start_addrs[0]);
   need=(short) (nele_identifier_table+20);
   list_page_need_test_only(need);
   list_page_need(20);
   fprintf(listfile,"\n\n\n\n\n\n");
   for (need=0;need<2;++need)
   list_page_indented_line(38,"***********************\n");
   list_page_indented_line(38,"***                 ***\n");
   list_page_indented_line(38,"***   STORAGE MAP   ***\n");
   list_page_indented_line(38,"***                 ***\n");
   for (need=0;need<2;++need)
   list_page_indented_line(38,"***********************\n");
   fprintf(listfile,"\n");
   new_map_section("Identifier table",identifier_table);
   print_identifier_table(listfile,TRUE);
   vvv=(void *) allstorage;
   for (ooo_off=0,ixj=0;ixj<2;++ixj,ooo_off+=COMPUTATIONAL_TYPE_COUNT){
      /*-------------------------------------  REAL DATA */
      if (memory_bytes_required[ooo_off+OBJ_REAL_DATA]>0){
         new_map_section(real_tags[ixj],vvv);
         vvv=(void *) NULL;
         for_all_storage(ooo_off+OBJ_REAL_DATA,ptru.rrr,2){
            for_all_on_line(ptru.rrr,ovfu.rrr){
               if (jjj>0) fprintf(listfile,", ");
               sss=enk_aux_val((void *) (ptru.rrr+jjj),REAL_TYPE);
               fprintf(listfile,"%s",sss);}
            fprintf(listfile,"\n");}}
      /*-------------------------------------  INT DATA */
      if (memory_bytes_required[ooo_off+OBJ_INT_DATA]>0){
         new_map_section(integer_tags[ixj],vvv);
         vvv=(void *) NULL;
         for_all_storage(ooo_off+OBJ_INT_DATA,ptru.iii,4){
            for_all_on_line(ptru.iii,ovfu.iii){
               if (jjj>0) fprintf(listfile,", ");
               sss=enk_aux_val((void *) (ptru.iii+jjj),INT_TYPE);
               fprintf(listfile,"%s",sss);}
            fprintf(listfile,"\n");}}
      /*-------------------------------------  BOOL DATA */
      if (memory_bytes_required[ooo_off+OBJ_BOOL_DATA]>0){
         new_map_section(Boolean_tags[ixj],vvv);
         vvv=(void *) NULL;
         for_all_storage(ooo_off+OBJ_BOOL_DATA,ptru.bbb,32){
            for_all_on_line(ptru.bbb,ovfu.bbb){
               if ((jjj>0)&&((jjj%8)==0)) fprintf(listfile,"   ");
               holdch = (char) ((*(ptru.bbb+jjj)) ? 'T' : 'F');
               fprintf(listfile,"%c",holdch);}
            fprintf(listfile,"\n");}}
      /*-------------------------------------  SOFF DATA */
      if (memory_bytes_required[ooo_off+OBJ_SOFF_DATA]>0){
         new_map_section(soff_tags[ixj],vvv);
         vvv=(void *) NULL;
         for_all_storage(ooo_off+OBJ_SOFF_DATA,ptru.sss,4){
            for_all_on_line(ptru.sss,ovfu.sss){
               if (jjj>0) fprintf(listfile,"   ");
               sss=enk_aux_val_just((void *) (ptru.sss+jjj),SOFF_TYPE);
               fprintf(listfile,"%s",sss);}
            fprintf(listfile,"\n");}}
      /*-------------------------------------  CHAR DATA */
      if (memory_bytes_required[ooo_off+OBJ_CHAR_DATA]>0){
         new_map_section(char_tags[ixj],vvv);
         vvv=(void *) NULL;
         for_all_storage(ooo_off+OBJ_CHAR_DATA,ptru.ccc,32){
            for_all_on_line(ptru.ccc,ovfu.ccc){
               if ((jjj>0)&&((jjj%8)==0)) fprintf(listfile,"   ");
               holdch = *(ptru.ccc+jjj);
               if (!(isascii(holdch) && isprint(holdch))) holdch='@';
               fprintf(listfile,"%c",holdch);}
            fprintf(listfile,"\n");}}}
   /*-------------------------------------  application */
   expression_map_sections();
   /*-------------------------------------  application */
   application_map_sections();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
