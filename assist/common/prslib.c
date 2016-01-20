/*
===============================================================================

        Parsing functions for ASSIST and SURE compiler programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)


===============================================================================
*/

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "cm_vars.h"
#include "cm_prsvars.h"
#include "pass.h"

#include "tokdefs.h"
#include "errdefs.h"
#include "rwdefs.h"

#include "prsdefs.h"
#include "prstypes.h"
#include "prsvars.h"

#include "sys_ext.h"
#include "str_ext.h"
#include "lio_ext.h"
#include "pio_ext.h"
#include "lex_ext.h"
#include "prs_ext.h"
#include "eval_ext.h"
#include "user_ext.h"

#define DBG_RCRS_NCH 2

static short recursion_level;
static Memix max_ops_count,max_opnd_count;
static Boolean parsing_rate_expression=FALSE;
static Boolean reals_allowed=TRUE;
static Boolean reals_in_error_shown=FALSE;
static Boolean of_currently_okay,scalar_array_okay;
static identifier_info_type *independent_of;
static Boolean is_independent_of,is_bool_arr_name,is_num_arr_name;
static operation_type wild_okay_opcode=OP_NIX;
/*===========================================================================*/

                   /********************************/
                   /********************************/
                   /****                        ****/
                   /****   CONVERSION SECTION   ****/
                   /****                        ****/
                   /********************************/
                   /********************************/

/*===========================================================================*/
#include "prstabsP.h"
/*===========================================================================*/
Memix init_prslib()
/*
   This routine initializes the values which need to be pre-set before
   parsing can begin.   In addition to setting counters to zero, a lookup
   table containing symbolic text corresponding to the different expression
   operators is built.   The table is brief, containing only two characters
   for each operator, but is referenced only when "DEBUG$ PARSE$;" is turned
   on.
*/
{
   /*-------------------------------------------------------*/
   operand_max_size_used=0;
   infix_max_size_used=0;
   postfix_max_size_used=0;
   init_prseval();
   /*------------------------------------- fill DEBUG$ PARSE$ lookup table */
   sys_memset(operation_text,(int) '\0',4*256);
   sys_memcpy(&operation_text[4*OP_OR],"OR",3);
   sys_memcpy(&operation_text[4*OP_XOR],"XOR",4);
   sys_memcpy(&operation_text[4*OP_AND],"AND",4);
#ifdef FUTURE
   sys_memcpy(&operation_text[4*OP_BOOL_EQ],"==",3);
   sys_memcpy(&operation_text[4*OP_BOOL_NE],"~~",3);
#endif
   sys_memcpy(&operation_text[4*OP_EQ],"=",2);
   sys_memcpy(&operation_text[4*OP_NE],"<>",3);
   sys_memcpy(&operation_text[4*OP_LT],"<",2);
   sys_memcpy(&operation_text[4*OP_GT],">",2);
   sys_memcpy(&operation_text[4*OP_LE],"<=",3);
   sys_memcpy(&operation_text[4*OP_GE],">=",3);
   sys_memcpy(&operation_text[4*OP_ADD],"+",2);
   sys_memcpy(&operation_text[4*OP_SUB],"-",2);
   sys_memcpy(&operation_text[4*OP_MUL],"*",2);
   sys_memcpy(&operation_text[4*OP_DVD],"/",2);
   sys_memcpy(&operation_text[4*OP_MOD],"MOD",4);
   sys_memcpy(&operation_text[4*OP_CYC],"CYC",4);
   sys_memcpy(&operation_text[4*OP_QUO],"DIV",4);
   sys_memcpy(&operation_text[4*OP_POW],"**",3);
   sys_memcpy(&operation_text[4*OP_RPOWR],"RR",3);
   sys_memcpy(&operation_text[4*OP_IPOWI],"II",3);
   sys_memcpy(&operation_text[4*OP_RPOWI],"RI",3);
   sys_memcpy(&operation_text[4*OP_CONCAT],"^",2);
   sys_memcpy(&operation_text[4*OP_IXDBY],"[]",3);
   sys_memcpy(&operation_text[4*OP_IXDBY2],"[,]",4);
   sys_memcpy(&operation_text[4*OP_I_LB],"[",2);
   sys_memcpy(&operation_text[4*OP_I_WILD],"*",2);
   sys_memcpy(&operation_text[4*OP_I_RB],"]",2);
   sys_memcpy(&operation_text[4*OP_PARENS],"()",3);
   sys_memcpy(&operation_text[4*OP_I_LP],"(",2);
   sys_memcpy(&operation_text[4*OP_I_RP],")",2);
   sys_memcpy(&operation_text[4*OP_I_CMMA],",",2);
   sys_memcpy(&operation_text[4*OP_INC],"++",3);
   sys_memcpy(&operation_text[4*OP_DEC],"--",3);
   sys_memcpy(&operation_text[4*OP_NOT],"~",2);
   sys_memcpy(&operation_text[4*OP_NEG],"-{U",4);
   sys_memcpy(&operation_text[4*OP_STNCHR],"}C",3);
   sys_memcpy(&operation_text[4*OP_STNBOO],"}B",3);
   sys_memcpy(&operation_text[4*OP_STNINT],"}I",3);
   sys_memcpy(&operation_text[4*OP_STNRE],"}R",3);
#ifdef FUTURE_MAYBE
   sys_memcpy(&operation_text[4*OP_StoI],"sI",3);
   sys_memcpy(&operation_text[4*OP_StoR],"sR",3);
   sys_memcpy(&operation_text[4*OP_StoB],"sB",3);
#endif
   sys_memcpy(&operation_text[4*OP_ItoR],"iR",3);
   sys_memcpy(&operation_text[4*OP_BtoI],"bI",3);
   sys_memcpy(&operation_text[4*OP_SQRT],"sqr",4);
   sys_memcpy(&operation_text[4*OP_EXP],"eX",3);
   sys_memcpy(&operation_text[4*OP_LN],"ln",3);
   sys_memcpy(&operation_text[4*OP_ABS],"||",3);
   sys_memcpy(&operation_text[4*OP_SIN],"s",2);
   sys_memcpy(&operation_text[4*OP_COS],"c",2);
   sys_memcpy(&operation_text[4*OP_TAN],"t",2);
   sys_memcpy(&operation_text[4*OP_ARCSIN],"s-",3);
   sys_memcpy(&operation_text[4*OP_ARCCOS],"c-",3);
   sys_memcpy(&operation_text[4*OP_ARCTAN],"t-",3);
   sys_memcpy(&operation_text[4*OP_FACT],"!F",3);
   sys_memcpy(&operation_text[4*OP_GAM],"|G",3);
   sys_memcpy(&operation_text[4*OP_SIZE],"[?]",4);
   sys_memcpy(&operation_text[4*OP_COUNT1],"#'",3);
   sys_memcpy(&operation_text[4*OP_IMIN1],"n'",3); 
   sys_memcpy(&operation_text[4*OP_RMIN1],"nn'",4);
   sys_memcpy(&operation_text[4*OP_IMAX1],"x'",3);
   sys_memcpy(&operation_text[4*OP_RMAX1],"xx'",4);
   sys_memcpy(&operation_text[4*OP_ISUM1],"+'",3);
   sys_memcpy(&operation_text[4*OP_RSUM1],"++'",4);
   sys_memcpy(&operation_text[4*OP_ANY1],"v'",3);
   sys_memcpy(&operation_text[4*OP_ALL1],"^'",3);
   sys_memcpy(&operation_text[4*OP_COUNT],"#.",3);
   sys_memcpy(&operation_text[4*OP_IMIN],"n.",3);
   sys_memcpy(&operation_text[4*OP_RMIN],"nn.",4);
   sys_memcpy(&operation_text[4*OP_IMAX],"x.",3);
   sys_memcpy(&operation_text[4*OP_RMAX],"xx.",4);
   sys_memcpy(&operation_text[4*OP_ISUM],"+.",3);
   sys_memcpy(&operation_text[4*OP_RSUM],"++.",4);
   sys_memcpy(&operation_text[4*OP_ANY],"v.",3);
   sys_memcpy(&operation_text[4*OP_ALL],"^.",3);
   sys_memcpy(&operation_text[4*OP_COMB],"!C",3);
   sys_memcpy(&operation_text[4*OP_PERM],"!P",3);
   sys_memcpy(&operation_text[4*OP_IROWMIN],"nr.",4);
   sys_memcpy(&operation_text[4*OP_RROWMIN],"nR.",4);
   sys_memcpy(&operation_text[4*OP_ICOLMIN],"nc.",4);
   sys_memcpy(&operation_text[4*OP_RCOLMIN],"nC.",4);
   sys_memcpy(&operation_text[4*OP_IROWMAX],"xr.",4);
   sys_memcpy(&operation_text[4*OP_RROWMAX],"xR.",4);
   sys_memcpy(&operation_text[4*OP_ICOLMAX],"xc.",4);
   sys_memcpy(&operation_text[4*OP_RCOLMAX],"xC.",4);
   sys_memcpy(&operation_text[4*OP_IROWSUM],"+r.",4);
   sys_memcpy(&operation_text[4*OP_RROWSUM],"+R.",4);
   sys_memcpy(&operation_text[4*OP_ICOLSUM],"+c.",4);
   sys_memcpy(&operation_text[4*OP_RCOLSUM],"+C.",4);
   sys_memcpy(&operation_text[4*OP_ROWCOUNT],"#r.",4);
   sys_memcpy(&operation_text[4*OP_COLCOUNT],"#c.",4);
#ifdef FUTURE
   sys_memcpy(&operation_text[4*OP_ANYPOS],"v+.",4);
   sys_memcpy(&operation_text[4*OP_ALLPOS],"^+.",4);
   sys_memcpy(&operation_text[4*OP_OTHER],"oth",4);
#endif
   sys_memcpy(&operation_text[4*OP_LISCOUNT],"#*",3);
   sys_memcpy(&operation_text[4*OP_ILISMIN],"n*",3);
   sys_memcpy(&operation_text[4*OP_RLISMIN],"nn*",4);
   sys_memcpy(&operation_text[4*OP_ILISMAX],"x*",3);
   sys_memcpy(&operation_text[4*OP_RLISMAX],"xx*",4);
   sys_memcpy(&operation_text[4*OP_ILISSUM],"+*",3);
   sys_memcpy(&operation_text[4*OP_RLISSUM],"++*",4);
   sys_memcpy(&operation_text[4*OP_LISANY],"v*",3);
   sys_memcpy(&operation_text[4*OP_LISALL],"^*",3);
   sys_memcpy(&operation_text[4*OP_NIX],"??",3);
   sys_memcpy(&operation_text[4*OP_INSVAL],"_V_",4);
   sys_memcpy(&operation_text[4*OP_VAL],"V",2);
   sys_memcpy(&operation_text[4*OP_PZ],".0.",4);
   sys_memcpy(&operation_text[4*OP_PBZ],".F.",4);
   sys_memcpy(&operation_text[4*OP_PRZ],".Z.",4);
   sys_memcpy(&operation_text[4*OP_PU],".1.",4);
   sys_memcpy(&operation_text[4*OP_PBU],".T.",4);
   sys_memcpy(&operation_text[4*OP_PRU],".U.",4);
   /*-------------------------------------------------------*/
   return((Memix) 0);
}
/*===========================================================================*/
void re_init_prslib(Memix *function_body_count,Stacktop *macro_stack_top)
/*
   The function re-initializes counters to zero when necessary to begin
   parsing another input file.
*/
{
   /*-------------------------------------------------------*/
   last_gotten_token=TK_NULL;
   parsing_convar_def=FALSE;
   eval_state_ssbits = ((state_bitstring_type) NULL);
   set_independent_of_variable((identifier_info_type *) NULL);
   init_prslib_counts(function_body_count,macro_stack_top);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define is_arithmetic(tok) (((tok)>=TK_ARITH_0)&&((tok)<=TK_ARITH_9))
#define is_boolean(tok) (((tok)>TK_OPERATION_0)&&((tok)<TK_ARITH_0))
/*===========================================================================*/
Boolean is_bool_relation(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = (Boolean) ((tok==TK_BOOL_EQ)||(tok==TK_BOOL_NE));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_relation(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok>=TK_RELATION_0)&&(tok<=TK_RELATION_9)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_bool_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = (Boolean)
            (
               (is_boolean(tok)) ||
               (is_bool_relation(tok))
            );
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_arith_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = (Boolean) 
            (
               (is_arithmetic(tok)) ||
               (is_relation(tok))
            );
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_add_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok==TK_ADD) || (tok==TK_SUB)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_mpy_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok==TK_MUL) || (tok==TK_DVD) ||
                     (tok==TK_QUO) || (tok==TK_MOD) ||
                     (tok==TK_CYC)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_eq_relation(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok>=TK_EQ_REL_0)&&(tok<=TK_EQ_REL_9)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_ineq_relation(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok>=TK_INEQ_REL_0)&&(tok<=TK_INEQ_REL_9)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_not_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) (tok==TK_NOT));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_cat_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) (tok==TK_CONCAT));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_and_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) (tok==TK_AND));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_or_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok==TK_OR)||(tok==TK_XOR)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_pow_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) (tok==TK_POW));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_sign_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) (tok==TK_SUB));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_value_token(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok>=TK_WITH_VALUE_0)&&(tok<=TK_WITH_VALUE_9)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_literal_token(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok>=TK_LITERAL_0)&&(tok<=TK_LITERAL_9)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean is_unary_op(token tok)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   ans = ((Boolean) ((tok==TK_SUB)||(tok==TK_NOT)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
/* 
   These functions return the operator corresponding to the given token which.
   The token must already be valid for the specicific function being invoked.
   The "is_..." functions can be used to test for conformance before invoking
   any of the "give_..." functionss.   For example:
       use "is_relation(tok)"  before "give_relation_op(tok)"
       use "is_add_op(tok)"    before "give_add_op(tok)"
       use "is_mpy_op(tok)"    before "give_mpy_op(tok)"
   e.g.:
       if (is_add_op(tok)) op=give_add_op(tok);
*/
/*===========================================================================*/
operation_type give_relation_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans=give_relation_op_table[tok-TK_RELATION_0];
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_add_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = ((tok==TK_ADD) ? OP_ADD : OP_SUB);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_cat_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = OP_CONCAT;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_not_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = OP_NOT;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_and_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = OP_AND;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_or_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = ((tok==TK_XOR) ? OP_XOR : OP_OR);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_sign_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = ((tok==TK_SUB) ? OP_NEG : OP_NIX);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_unary_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = ((tok==TK_NOT) ? OP_NOT : OP_NEG);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_infix_pow_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans =  ((tok==TK_POW) ? OP_POW : OP_NIX);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_literal_type(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = give_literal_type_table[tok-TK_LITERAL_0];
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_bool_relation_op(token tok)
{
   register operation_type ans;
   /*-------------------------------------------------------*/
   ans = ((tok==TK_BOOL_NE) ? OP_BOOL_NE : OP_BOOL_EQ);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_pow_op
               (token tok,type_flagword_type c1,type_flagword_type c2)
/*
   This function returns the postfix operator corresponding to the given
   token and the given computational types for the two operands.
   The token must be a valid power token (TK_POW).   Use "is_pow_op(tok)"
   before invoking this function to be safe.
*/
{
   operation_type ans;
   /*-------------------------------------------------------*/
   ans=OP_NIX;
   if (tok==TK_POW){
      if ((c1==REAL_TYPE)&&(c2==REAL_TYPE)) ans=OP_RPOWR;
      else if ((c1==REAL_TYPE)&&(c2==INT_TYPE)) ans=OP_RPOWI;
      else if ((c1==INT_TYPE)&&(c2==INT_TYPE)) ans=OP_IPOWI;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_mpy_op(token tok)
/*
   This function returns the multiplicative operator corresponding to the given
   token.   The token must be a valid mulop.   Use "is_mpy_op(tok)"
   before invoking this function to be safe.
*/
{
   operation_type ans;
   /*-------------------------------------------------------*/
   ans=OP_NIX;
   if (tok==TK_MUL) ans=OP_MUL;
   else if (tok==TK_DVD) ans=OP_DVD;
   else if (tok==TK_MOD) ans=OP_MOD;
   else if (tok==TK_CYC) ans=OP_CYC;
   else if (tok==TK_QUO) ans=OP_QUO;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
operation_type give_any_precedence_op(token tok,rwtype rw,token lasttok)
{
   operation_type ans;
   /*-------------------------------------------------------*/
   if (!is_value_token(lasttok) && (is_unary_op(tok)))
      ans=give_unary_op(tok);
   else if (is_or_op(tok)) ans=give_or_op(tok);
   else if (is_and_op(tok)) ans=give_and_op(tok);
   else if (is_relation(tok)) ans=give_relation_op(tok);
   else if (is_bool_relation(tok)) ans=give_bool_relation_op(tok);
   else if (is_add_op(tok)) ans=give_add_op(tok);
   else if (is_mpy_op(tok)) ans=give_mpy_op(tok);
   else if (is_pow_op(tok)) ans=give_infix_pow_op(tok);
   else if (is_cat_op(tok)) ans=give_cat_op(tok);
   else if (is_value_token(tok)) ans=OP_VAL;
   else if (is_a_builtin_function(tok,rw)) ans=OP_SQRT;
   else ans=OP_MIN_PREC;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/

       /*********************************************************/
       /*********************************************************/
       /****                                                 ****/
       /****   THE "TOOLBOX FOR EXPRESSION PARSER" SECTION   ****/
       /****                                                 ****/
       /*********************************************************/
       /*********************************************************/

/*
   The FOLLOW set of  ( <boolean-expression> )  is:
         =
         &
         AND
         |
         OR
         XOR
         ;
         ,
         )
         THEN
         BY
   The FOLLOW set of  ( <numeric-expression> )  is:
         The FOLLOW set of ( <boolean-expression> ) union with set of:
         **
         *
         /
         DIV
         MOD
         CYC
         +
         -
         >  (from rate expressions)
         =
         <>
         >
         <
         >=
         <=
         ;
         ,
         )
         ]
         ..
         BY
         OF

   When parsing a boolean expression, an expression in parens is therefore
   numeric whenever the token following the closing right paren is:

         **
         *          =
         /          <>
         DIV        <
         MOD        >
         CYC        <=
         +          >=
         -
*/
/*===========================================================================*/
type_flagword_type recurse_the_parse(any_parse_routine rou,char *msg)
/*
   This function increments the recursion level and invokes the parse
   routine "rou" and decrements the recursion level when done.   In debug
   "DEBUG$ PARSE$;" mode, it also prints out a message before and after 
   entering the routine.  The exit message includes information about the
   data type of the resultant expression.
*/
{
   unsigned short jjjmac,jjjend;
   char symbolic[90];
   type_flagword_type ans;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)){
      jjjend=(DBG_RCRS_NCH*recursion_level);
      for (jjjmac=0;jjjmac<jjjend;++jjjmac)
         fprintf(bugfile," ");
      fprintf(bugfile,"%s: beginning\n",msg);}
   ++recursion_level;
   ans = rou();
   --recursion_level;
   if (bitset(debug_flagword,DBG_PRS)){
      jjjend=(DBG_RCRS_NCH*recursion_level);
      for (jjjmac=0;jjjmac<jjjend;++jjjmac)
         fprintf(bugfile," ");
      make_symbolic_type_list(symbolic,ans);
      fprintf(bugfile,"%s: return(0x%02x,(%s))\n",msg,
              (unsigned int) ans,symbolic);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean adjust_after_parse(binary_operand_item_info_type *item)
/*
   This routine splits the type flag word into its computational and
   special parts and saves the current indicies into the postfix stack and
   infix list.   It is called after parsing the operand of a unary operation
   or after parsing the left or right operand of a binary operation.
*/
{
   Boolean is_var;
   /*-------------------------------------------------------*/
   (*item).comp = (*item).type & COMPUTATIONAL_TYPE_MASK;
   (*item).spec = (*item).type & SPECIAL_TYPE_MASK;
   (*item).ixpo = this_expr.n_postfix_ops;
   (*item).ixin = this_expr.n_infix_ops;
   is_var = (bitset((*item).type,ANY_SPECIAL_VAR_TYPE_MASK));
   /*-------------------------------------------------------*/
   return(is_var);
}
/*===========================================================================*/
void test_total_for_fits(long total)
{
   int_type value;
   /*-------------------------------------------------------*/
   if (total>MALLOC_MAX){
      value=total;
      show_tail_err_on_both(E_NOMEMORYP,&value,INT_TYPE,os_malloc_limit_msg);
      abort_program();}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
token_info_type *allocate_for_parse_stage
     (Memix max_body_count,Memix max_nest_count,Memix max_parm_count,
      Memix user_max_ops_count,Memix user_max_opnd_count,
      Memix *function_body_count,Stacktop *macro_stack_top,
      macro_expansion_info_type **macro_stack)
/*
   This function allocates enough memory for the parse stage of the
   program parsing.   The limits can be controled via the following
   command line options:
               MSDOS         UNIX                    MEANING
            ---------     -----------    -------------------------------------
              /B=n           -B=n        function <B>ody token count limit
              /P=n           -P=n        function <P>arameter list count limit
             /NEST=n        -NEST=n      function nesting limit
              /O=n           -O=n        operand/operator table limit
*/
{
   memsize_t bytes;
   long total,bytes_body,bytes_knts,bytes_ops,bytes_opnd,bytes_mac;
   token_info_type *function_body_storage;
   char *ppp;
   /*-------------------------------------------------------*/
   max_ops_count = user_max_ops_count;
   max_opnd_count = user_max_opnd_count;
#ifdef BUGGY
fprintf(stderr,"sizeof(short)=%d\n",(int) sizeof(short));
fprintf(stderr,"sizeof(token_info_type)=%d\n",
       (int) sizeof(token_info_type));
fprintf(stderr,"sizeof(token_info_type *)=%d\n",
       (int) sizeof(token_info_type *));
fprintf(stderr,"sizeof(operation_type)=%d\n",
       (int) sizeof(operation_type));
fprintf(stderr,"sizeof(operand_type)=%d\n",
       (int) sizeof(operand_type));
fprintf(stderr,"sizeof(macro_expansion_info_type)=%d\n",
       (int) sizeof(macro_expansion_info_type));
#endif
   bytes_body = ((long) sizeof(token_info_type)) *
                ((long) max_body_count);
   bytes_knts = ((long) sizeof(unsigned short)) *
                ((long) max_parm_count) *
                ((long) EXPECTED_TOKENS_PER_CALL_PARM) *
                ((long) max_nest_count);
   bytes_ops  = ((long) sizeof(operation_type)) *
                ((long) max_ops_count);
   bytes_opnd = ((long) sizeof(operand_type)) * 
                ((long) max_opnd_count);
   bytes_mac  = ((long) sizeof(macro_expansion_info_type)) *
                ((long) max_nest_count);
#ifdef VFY_FOUR_BYTE_ALIGN
   if ((bytes_ops%4)!=0) error_this_pos(E_OPTIONALIGN);
   if ((bytes_knts%4)!=0) error_this_pos(E_OPTIONALIGN);
   if ((bytes_mac%4)!=0) error_this_pos(E_OPTIONALIGN);
#endif
   /*-------------------------------------------   compute amt. of storage */
   total=bytes_body;
   test_total_for_fits(total);
   total = bytes_ops*2 + bytes_opnd + bytes_mac + bytes_knts*2;
   total += (max_ops_count*2 + max_opnd_count); /* for garbage collection */
#ifdef BUGGY
fprintf(stderr,"bytes_body=%d\n",(int) bytes_body);
fprintf(stderr,"bytes_ops=%d\n",(int) bytes_ops);
fprintf(stderr,"bytes_ops=%d\n",(int) bytes_ops);
fprintf(stderr,"bytes_opnd=%d\n",(int) bytes_opnd);
fprintf(stderr,"bytes_mac=%d\n",(int) bytes_mac);
fprintf(stderr,"bytes_knts=%d\n",(int) bytes_knts);
fprintf(stderr,"bytes_knts=%d\n",(int) bytes_knts);
fprintf(stderr,"total=%d\n",(int) total);
#endif
   test_total_for_fits(total);
   /*-------------------------------------------   allocate the storage */
   function_body_storage = ((token_info_type *) NULL);
   if (bytes_body > 0){
      bytes=(memsize_t) (bytes_body);
      function_body_storage = (token_info_type *) g_malloc(bytes);
      if (function_body_storage == ((token_info_type *) NULL)){
         show_err_on_both(E_NOMEMORYP,"(parse library storage -- func body)",
                          CHAR_TYPE);
         abort_program();}}
   /*------------------- body done, now do rest */
   bytes=(memsize_t) total;
   full_storage = (void *) g_malloc(bytes);
   if (full_storage == ((void *) NULL)){
      show_err_on_both(E_NOMEMORYP,"(parse library storage)",CHAR_TYPE);
      abort_program();}
   /*-------------------------------------------   divide the storage */
   ppp=(char *) full_storage;
   this_expr.postfix_ops = (operation_type *) ppp; ppp += bytes_ops;
   this_expr.infix_ops = (operation_type *) ppp; ppp += bytes_ops;
   this_expr.operands = (operand_type *) ppp; ppp += bytes_opnd;
   *macro_stack = (macro_expansion_info_type *) ppp; ppp += bytes_mac;
   offset_storage=(unsigned short *) ppp; ppp += bytes_knts;
   counts_storage=(unsigned short *) ppp; ppp += bytes_knts;
   garbage_postfix_ops = (Boolean *) ppp; ppp += max_ops_count;
   garbage_infix_ops = (Boolean *) ppp; ppp += max_ops_count;
   garbage_operands = (Boolean *) ppp; ppp += max_opnd_count;
   /*-------------------------------------------   initialize counts */
   init_prslib_counts(function_body_count,macro_stack_top);
   /*-------------------------------------------------------*/
   return(function_body_storage);
}
/*===========================================================================*/
void init_prslib_counts(Memix *function_body_count,Stacktop *macro_stack_top)
{
   /*-------------------------------------------------------*/
   counts_offset_storage_count=0;
   *function_body_count=0;
   *macro_stack_top=(-1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void free_after_parse_stage(macro_expansion_info_type **macro_stack,
                            token_info_type **function_body_storage)
/*
   This function frees storage allocated by "allocate_for_parse_stage()"
   so that it can be re-used during the generation stage.
*/
{
   /*-------------------------------------------------------*/
   g_free(full_storage);
   if (*function_body_storage != ((token_info_type *) NULL))
      g_free(*function_body_storage);
   full_storage = (void *) NULL;
   *function_body_storage = (token_info_type *) NULL;
   this_expr.postfix_ops = (operation_type *) NULL;
   this_expr.infix_ops = (operation_type *) NULL;
   this_expr.operands = (operand_type *) NULL;
   *macro_stack = (macro_expansion_info_type *) NULL;
   offset_storage=(unsigned short *) NULL;
   counts_storage=(unsigned short *) NULL;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void dummy_token(type_flagword_type comp)
/*
   This function inserts a dummy value token into the input stream.
   It is used when parameters are missing or when two operators occur in
   a row.  Its use can allow parsing to continue after a fatal error with
   a minimum of redundant error messages.
*/
{
   /*-------------------------------------------------------*/
   sys_memcpy((char *) &prev,(char *) &this,sizeof(token_info_type));
   this.tok=dumtoken(&this.rw,&this.id[0],&this.linnum,&this.pos,comp);
   this.id_info_ptr=(identifier_info_type *) NULL;
   this_looked_up=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void skip_to_next_specified(Boolean first_in,token look_tok)
/*
   This function skips extraneous text up to the next semicolon or "look_tok".
*/
{
   Boolean first;
   /*-------------------------------------------------------*/
   for (first=first_in;this_is_not(look_tok);first=FALSE){
      if (this.tok==TK_LP) first=skip_nested_when_skipping(TK_RP,first);
      else if (this.tok==TK_LB) first=skip_nested_when_skipping(TK_RB,first);
      skip_one_when_skipping(first);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void skip_one_when_skipping(Boolean first)
/*
   This function skips a single token when tokens are being skipped.
*/
{
   /*-------------------------------------------------------*/
   if (first) error_this_pos(E_SKIPPING);
   advance_one_token();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean skip_nested_when_skipping(token until_tok,Boolean first)
/*
   This function skips tokens beginning with "this.tok" which is either
   a left paren or a left bracket.   Upon exit, "this.tok" will be the
   matching right paren or right bracket.
*/
{
   register Boolean q;
   /*-------------------------------------------------------*/
   skip_one_when_skipping(first);
   while (this_is_not(until_tok)){
      if (this.tok==TK_LP) q=skip_nested_when_skipping(TK_RP,FALSE);
      else if (this.tok==TK_LB) q=skip_nested_when_skipping(TK_RB,FALSE);
      skip_one_when_skipping(FALSE);}
   /*-------------------------------------------------------*/
   return(FALSE);
}
/*===========================================================================*/
void skip_to_lower_precedence(operation_type ooo)
/*
   This routine skips extraneous text up to the next operator with precidence
   lower than "ooo".   It is assumed that the current token has already been
   interpreted.
*/
{
   Boolean first;
   /*-------------------------------------------------------*/
   for (first=TRUE;
        (precedence_cmp(give_any_precedence_op(this.tok,this.rw,prev.tok),
                        ooo)<0)&&
        (this_is_not_semi());
        first=FALSE){
      if (this.tok==TK_LP) first=skip_nested_when_skipping(TK_RP,first);
      else if (this.tok==TK_LB) first=skip_nested_when_skipping(TK_RB,first);
      skip_one_when_skipping(first);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int precedence_cmp(operation_type o1,operation_type o2)
/*
   This function compares the precedence of two operators.   The return
   value is:
          -1   iff  prec(o1) < prec(o2)
          0    iff  prec(o1) = prec(o2)
          +1   iff  prec(o1) > prec(o2)
*/
{
   register short vdiff,v1,v2;
   register int ans;
   /*-------------------------------------------------------*/
   v1 = (short) (o1 & OP_DIFF_PRECEDENCE_MASK);
   v2 = (short) (o2 & OP_DIFF_PRECEDENCE_MASK);
   vdiff = v2 - v1;
   if (vdiff<0) ans=(-1);
   else if (vdiff==0) ans=0;
   else ans=1;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void skip_to_next_semicolon(Boolean token_already_interpreted)
/*
   This routine skips extraneous text up to the next semicolon.  It is
   assumed that, "token_already_interpreted" is FALSE iff this.tok points
   to the first uninterpreted token.
*/
{
   /*-------------------------------------------------------*/
   if (token_already_interpreted){
      if (this_is_not(TK_SEMI)&&this_is_not(TK_LB)&&this_is_not(TK_LP))
         skip_one_when_skipping(FALSE);}
   skip_to_next_specified(TRUE,TK_SEMI);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void skip_to_next_comma_or(Boolean token_already_interpreted,token until_tok)
/*
   This routine skips extraneous text up to the next comma or the next user
   specified token.  It is assumed that, "token_already_interpreted" is
   FALSE iff this.tok points to the first uninterpreted token.
*/
{
   Boolean first;
   /*-------------------------------------------------------*/
   if (token_already_interpreted){
      if (this_is_not(TK_SEMI)&&this_is_not(TK_LB)&&this_is_not(TK_LP))
         skip_one_when_skipping(FALSE);}
   for (first=TRUE;this_is_not_param_terminator(until_tok);first=FALSE){
      if (this.tok==TK_LP) first=skip_nested_when_skipping(TK_RP,first);
      else if (this.tok==TK_LB) first=skip_nested_when_skipping(TK_RB,first);
      skip_one_when_skipping(first);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_must_be_semicolon()
/*
   This function parses the next token when it is expected to be a semicolon.
   It skips tokens up to the next semicolon.
*/
{
   /*-------------------------------------------------------*/
   if (this.tok==TK_EOF)
      show_err_at_pos_on_both(E_NOTSEMI,this.pos,"(end-of-file)",CHAR_TYPE);
   else if (this.tok!=TK_SEMI){
      error_this_pos(E_NOTSEMI);
      skip_to_next_semicolon(TRUE);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
operation_type give_promote_op(type_flagword_type from_comp,
                               type_flagword_type to_comp,
                               short op_column_pos)
/*
   This function returns the promotion unary operator required to convert
   from type "from_comp" to type "to_comp".   It is assumed that:

                   "from_comp" != "to_comp"
*/
{
   char xmsg[50];
   /*-------------------------------------------------------*/
   if ((from_comp==INT_TYPE)&&(to_comp==REAL_TYPE)){
      if ((!reals_allowed)&&(!reals_in_error_shown)){
         error_this_pos(E_NOREALS); 
         reals_in_error_shown=TRUE;} 
      return(OP_ItoR);}
   /*-------------------------------------------------------*/
   make_promotion_message(xmsg,from_comp,to_comp);
   show_err_at_pos_on_both(E_PROMOTE,op_column_pos,xmsg,CHAR_TYPE);
   if (to_comp==CHAR_TYPE) return(OP_STNCHR);
   if (to_comp==BOOL_TYPE) return(OP_STNBOO);
   if (to_comp==INT_TYPE) return(OP_STNINT);
   return(OP_STNRE);
   /*-------------------------------------------------------*/
/*
??????  need error detection for SSVAR prior to rule section.   Maybe do
??????  this when evaluating constants.   (but probably should never occur)
*/
}
/*===========================================================================*/
Boolean promote_to_same_type
     (binary_operand_pair_info_type *bopi_ptr,short op_chix,
      Boolean second_only)
/*
   This function promotes either the first or the second operand in a
   postfix expression so that the types will be the same.  It is assumed
   that from_comp != to_comp.
*/
{
   operation_type stuff_op;
   binary_operand_item_info_type *item1,*item2;
   Boolean promoted;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)) show_message("promote_to_same_type");
   promoted=FALSE;
   item1 = & (*bopi_ptr).item[0];
   item2 = & (*bopi_ptr).item[1];
   if ((*item1).comp != (*item2).comp){
      promoted=TRUE;
      if (((*item1).comp < (*item2).comp)&&(!second_only)){
         set_arrow_sides(TRUE,FALSE);
         stuff_op =
            give_promote_op((*item1).comp,(*item2).comp,op_chix);
         (*item1).comp = (*item2).comp;
         (*item1).spec = (*item2).spec;
         (*bopi_ptr).ans = (*item2).comp;
         (*bopi_ptr).spcans = (*item2).spec;
         make_room_and_stuff_into_expr((*item1).ixpo,stuff_op);}
      else{
         set_arrow_sides(FALSE,TRUE);
         stuff_op =
            give_promote_op((*item2).comp,(*item1).comp,op_chix);
         (*item2).comp = (*item1).comp;
         (*item2).spec = (*item1).spec;
         (*bopi_ptr).ans = (*item1).comp;
         (*bopi_ptr).spcans = (*item1).spec;
         make_room_and_stuff_into_expr((*item2).ixpo,stuff_op);}
#ifdef REDUNDANT_I_THINK
      if ((!reals_allowed)&&(!reals_in_error_shown)&&
          (((*bopi_ptr).ans & COMPUTATIONAL_TYPE_MASK) == REAL_TYPE)){
         error_this_pos(E_NOREALS);
         reals_in_error_shown=TRUE;}
#endif
      reset_arrow_sides();}
   /*-------------------------------------------------------*/
   return(promoted);
}
/*===========================================================================*/
Memix increment_postfix_operation()
/*
   This function increments the scratch postfix stack, testing for stack
   overflow.
*/
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans = this_expr.n_postfix_ops;
   ++this_expr.n_postfix_ops;
   if (this_expr.n_postfix_ops>postfix_max_size_used)
      postfix_max_size_used=this_expr.n_postfix_ops;
   if (ans == max_ops_count){
      error_this_pos(E_EXPRSTKOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_infix_operation()
/*
   This function increments the scratch infix list, testing for list
   overflow.
*/
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans = this_expr.n_infix_ops;
   ++this_expr.n_infix_ops;
   if (this_expr.n_infix_ops>infix_max_size_used)
      infix_max_size_used=this_expr.n_infix_ops;
   if (ans == max_ops_count){
      error_this_pos(E_EXPRLISTOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Memix increment_operand()
/*
   This function increments the operand list used for both the infix and
   postfix expressions.
*/
{
   Memix ans;
   /*-------------------------------------------------------*/
   ans = this_expr.n_operands;
   ++this_expr.n_operands;
   if (this_expr.n_operands>operand_max_size_used)
      operand_max_size_used=this_expr.n_operands;
   if (ans == max_opnd_count){
      error_this_pos(E_EXPROPNDOVF);
      abort_program();}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void make_room_and_stuff_into_expr(short ixwhere,operation_type stuff_op)
/*
   This function makes room in the postfix expression stack for a new
   operator and does an insert.  It is used when the type of the first
   operand must be promoted.
*/
{
   Memix new_ixop,iii,stuffpos;
   static Memix one=1;
   /*-------------------------------------------------------*/
   stuffpos=(Memix) ixwhere;
   if ((new_ixop = increment_postfix_operation()) < max_ops_count){
      for (iii=this_expr.n_postfix_ops-1;iii>stuffpos;--iii)
         this_expr.postfix_ops[iii]=this_expr.postfix_ops[iii-one];
      this_expr.postfix_ops[stuffpos]=stuff_op;}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define save_this() \
   sys_memcpy((char *) &sv_this,(char *) &this,sizeof(token_info_type))
#define restore_this() \
   sys_memcpy((char *) &this,(char *) &sv_this,sizeof(token_info_type))
/*===========================================================================*/
short save_operator_and_advance_1_tok(token_info_type *ans)
/*
   This function saves the current token (usually an operator token) and
   the current character column position and advances one token.
*/
{
   short op_chix;
   /*-------------------------------------------------------*/
   sys_memcpy((char *) ans,(char *) &this,sizeof(token_info_type));
   op_chix=this.pos;
   advance_one_token();
   /*-------------------------------------------------------*/
   return(op_chix);
}
/*===========================================================================*/
short calc_operation_text_leng(operation_type op,Boolean upto_curly,
                               Boolean *extra)
{
   register char *ppp;
   register char ovfchr;
   register char *ppp0;
   register short ans;
   /*-------------------------------------------------------*/
   ovfchr = (char) ((upto_curly) ? OPENCURLY : ' ');
   for (ppp0=ppp=operation_text+(((Memix) 4)*((Memix) op));
        ((*ppp)!='\0')&&((*ppp)!=ovfchr);++ppp);
   ans = (short) ((ppp-ppp0));
   /*-------------------------------------------------------*/
   *extra = (Boolean) (isascii(*ppp0) && isalpha(*ppp0));
   return(ans);
}
/*===========================================================================*/
void show_operation_text(FILE *fp,operation_type op,Boolean upto_curly)
{
   register char *ppp;
   register char ovfchr;
   /*-------------------------------------------------------*/
   ovfchr = (char) ((upto_curly) ? OPENCURLY : ' ');
   for (ppp=operation_text+(((Memix) 4)*((Memix) op));
        ((*ppp)!='\0')&&((*ppp)!=ovfchr);++ppp)
      fprintf(fp,"%c",*ppp);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_infix_or_postfix(operation_type *ops,short nnn,char *label)
/*
   This function displays either an infix list or a postfix stack.   It is
   useful when debugging the expression parsing functions.
*/
{
   register short ix,nix;
   register operation_type *tmpop;
   /*-------------------------------------------------------*/
   nix=(DBG_RCRS_NCH*recursion_level);
   for (ix=0;ix<nix;++ix) fprintf(bugfile," ");
   for (ix=0,nix=nnn,tmpop=ops;ix<nix;++ix,++tmpop){
      if (ix==0) fprintf(bugfile,"%s: ",label);
      else fprintf(bugfile," ");
      show_operation_text(bugfile,(*tmpop),FALSE);}
   fprintf(bugfile,"\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_message(char *msg)
/*
   This function displays either an infix list or a postfix stack.   It is
   useful when debugging the expression parsing functions.
*/
{
   register short ix,nix;
   /*-------------------------------------------------------*/
   nix=(DBG_RCRS_NCH*recursion_level);
   for (ix=0;ix<nix;++ix) fprintf(bugfile," ");
   fprintf(bugfile,"%s\n",msg);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void save_postfix(operation_type op)
/*
   This function adds an operation to the postfix operation stack.
*/
{
   Memix ixop;
   /*-------------------------------------------------------*/
   if ((ixop = increment_postfix_operation()) < max_ops_count)
      this_expr.postfix_ops[ixop] = op;
   if (bitset(debug_flagword,DBG_PRS))
      show_infix_or_postfix(this_expr.postfix_ops,this_expr.n_postfix_ops,
                            "postfix");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Memix save_infix(operation_type op)
/*
   This function adds an operation to the infix operation list.   During
   "DEBUG$ PARSE$;" debugging mode, detail information about the operation
   is displayed on the bugfile.
*/
{
   short col,colovf;
   Memix ixop;
   /*-------------------------------------------  debug print */
   if (bitset(debug_flagword,DBG_PRS)){
      if (op==OP_VAL){
         colovf=(DBG_RCRS_NCH*recursion_level);
         for (col=0;col<colovf;++col) fprintf(bugfile," ");
         show_operation_text(bugfile,op,FALSE);
         if (this.id_info_ptr==((identifier_info_type *) NULL))
            fprintf(bugfile,"NULL-ID-INFO");
         else
            bug_print_idinfo(this.id_info_ptr,0,2);
         fprintf(bugfile,"\n");}}
   /*-------------------------------------------  inc. infix list & save */
   if ((ixop = increment_infix_operation()) < max_ops_count)
      this_expr.infix_ops[ixop] = op;
   /*-------------------------------------------  debug show the list */
   if ((op!=OP_VAL)&&(op!=OP_INSVAL)&&(bitset(debug_flagword,DBG_PRS)))
      show_infix_or_postfix(this_expr.infix_ops,this_expr.n_infix_ops,
                            "infix");
   /*-------------------------------------------------------*/
   return(ixop);
}
/*===========================================================================*/
void lookup_ident()
/*
   This function looks up an identifier or a numeric value in the identifier
   table after it is known that
*/
{
   value_union_type value;
   type_flagword_type ttt,comp;
   Boolean literal,unknown;
   Boolean is_null,value_already;
   /*-------------------------------------------------------*/
   if ((this.id_info_ptr != ((identifier_info_type *) NULL))&&(this_looked_up))
      return;
   this_looked_up=TRUE;
   ttt=UNKNOWN_TYPE;
   literal=(Boolean) is_literal_token(this.tok);
   if (literal) ttt=give_literal_type(this.tok);
   value_already=FALSE;
   if (this.tok==TK_ID){
      if (parsing_rate_expression){
         xref_utilize(this.id);
         if (ttt==UNKNOWN_TYPE) ttt=REAL_TYPE;
         this.id_info_ptr=searchid_silent(this.id,ttt,this.pos,TRUE);}
      else{
         if (ttt==UNKNOWN_TYPE) ttt=INT_TYPE;
         this.id_info_ptr=searchid_old(this.id,ttt,this.pos);}}
   else if (is_value_token(this.tok)){
      is_null = (Boolean) 
                (this.id_info_ptr==((identifier_info_type *) NULL));
      comp = ((is_null) ? UNKNOWN_TYPE
                        : (*this.id_info_ptr).flags & COMPUTATIONAL_TYPE_MASK);
      value_already = (Boolean) ((!is_null) && (comp != UNKNOWN_TYPE));
      this.id_info_ptr=searchid_silent(this.id,ttt,this.pos,FALSE);}
#ifdef BUGGY
if (this.id_info_ptr == ((identifier_info_type *) NULL)){
fprintf(bugfile,
"lookup_ident() could not searchid_silent!!!\n");
abort_program();}
#endif
   unknown=(Boolean) ((*this.id_info_ptr).ptr.vvv == ((void *) NULL));
   if (unknown){
      ttt = (*this.id_info_ptr).flags;
      comp = (ttt & COMPUTATIONAL_TYPE_MASK);
      if (!bitset(ttt,NO_VALUE_TYPE_MASK)){
         if (literal){
            if ((this.tok==TK_INT)||(this.tok==TK_REAL)){
               decode_number(this.tok,this.id,&value.iii,&value.rrr);
               if (value_already)
                  show_err_at_pos_on_both
                       (E_INTERNAL,this.pos,
                        "Already has a type (lookup_ident)!!!",CHAR_TYPE);}
            else
               standard_value(&value,comp);}
         else standard_value(&value,comp);
         (*this.id_info_ptr).ptr.vvv =
            save_value_in_number_table(&value,comp);}}
   if (this.id_info_ptr==independent_of) not_independent_of();
#ifdef BUGGY
if (this.id_info_ptr == ((identifier_info_type *) NULL)){
fprintf(stderr,"lookup_ident() called but not a value token!!!\n");
abort_program();}
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void save_operand(operand_type opand)
/*
   This function saves the next operand on the operand list.
*/
{
   Memix ixop;
   /*-------------------------------------------------------*/
   if ((ixop = increment_operand()) < max_opnd_count)
      this_expr.operands[ixop] = opand;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void next_term_or_factor(binary_operand_pair_info_type *bopi_ptr)
/*
   This function copies the type information from the second operand to the 
   first operand.   It is called when several left-to-right associative
   operators of the same precedence occur in a row.
*/
{
   /*-------------------------------------------------------*/
   (*bopi_ptr).item[0].ixpo   = this_expr.n_postfix_ops;
   (*bopi_ptr).item[0].ixin   = this_expr.n_infix_ops;
   (*bopi_ptr).item[0].type   = (*bopi_ptr).item[1].type;
   (*bopi_ptr).item[0].comp   = (*bopi_ptr).item[1].comp;
   (*bopi_ptr).item[0].spec   = (*bopi_ptr).item[1].spec;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void set_independent_of_variable(identifier_info_type *idinfo)
{
   /*-------------------------------------------------------*/
   independent_of=idinfo;
   is_independent_of=TRUE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void start_independent_of_check()
{
   /*-------------------------------------------------------*/
   is_independent_of=TRUE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void finish_independent_of_check()
{
   /*-------------------------------------------------------*/
   if ((independent_of!=((identifier_info_type *) NULL))&&(is_independent_of))
      show_wrn_at_pos_on_both(E_INDEPENDENT,this.pos,
                              (*independent_of).name,CHAR_TYPE,
                              DEFAULT_WARN_LEV);
   is_independent_of=TRUE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void not_independent_of()
{
   /*-------------------------------------------------------*/
   is_independent_of=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/

               /****************************************/
               /****************************************/
               /****                                ****/
               /****   EXPRESSION PARSING SECTION   ****/
               /****                                ****/
               /****************************************/
               /****************************************/

/*===========================================================================*/
void init_expression(Boolean reals_ok)
/*
   This function erases and re-initializes the current expression being
   parsed.  It is called only from the top level (not for a sub-expression).
*/
{
   short col;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)){
      for (col=0;col<90;++col) fprintf(bugfile,"="); fprintf(bugfile,"\n");}
   recursion_level = 0;
   this_expr.n_postfix_ops = 0;
   this_expr.n_infix_ops = 0;
   this_expr.n_operands=0;
   this_expr.source_code_line_number = this.linnum;
   this_expr.in_error=FALSE;
   this_expr.rtntype=EMPTY_TYPE;
   reals_allowed=reals_ok;
   reals_in_error_shown=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void make_inc_or_dec_variable_expr
     (identifier_info_type *idinfo,operation_type op)
{
   /*-------------------------------------------------------*/
   init_expression(FALSE);
   save_infix(OP_VAL);
   save_operand(idinfo);
   save_postfix(OP_VAL);
   save_infix(op);
   save_postfix(op);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define SET_TAIL_N_CORRESP(op) \
        tail_opcode=(op); \
        corresponding_list_opcode = op; \
        if (variable_length) corresponding_list_opcode=list_version_op(op)
#define MKROOM(ele,ty,incrou,maxknt,www) \
        if ((ix=incrou())<maxknt) \
           sys_memmove(((char *) (ele+1)),((char *) (ele)),(ix*sizeof(ty))); \
        else this_expr.in_error=TRUE; \
        *ele = www
void make_inc_or_dec_array_ele_expr
     (identifier_info_type *idinfo,operation_type op,operation_type ixdby_op)
/*
   This function turns the current "this_expr", which is assumed to
   be the expression for the subscript of the array element, into
   an expression to increment or decrement the array element.
*/
{
   Memix ix;
   Boolean oerr;
   /*-------------------------------------------------------*/
   oerr=this_expr.in_error;
   MKROOM(this_expr.postfix_ops,operation_type,
          increment_postfix_operation,max_ops_count,OP_VAL);
   MKROOM(this_expr.infix_ops,operation_type,
          increment_infix_operation,max_ops_count,OP_I_LB);
   MKROOM(this_expr.infix_ops,operation_type,
          increment_infix_operation,max_ops_count,OP_VAL);
   MKROOM(this_expr.operands,operand_type,
          increment_operand,max_opnd_count,idinfo);
   if ((!oerr)&&(this_expr.in_error))
      error_this_pos(E_EXPROPNDOVF);
   else{
      save_infix(OP_I_RB);
      save_postfix(ixdby_op);
      save_infix(op);
      save_postfix(op);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean qq_promote_and_save(binary_operand_pair_info_type *bopi_ptr,
                            short op_chix,operation_type op,token dummy)
/*
   This function promotes the type of either the first or second operand
   in a binary operation pair and saves the operation on the postfix stack.
*/
{
   Boolean left_bad,right_bad;
   Boolean promoted;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)) show_message("promote_and_save");
   promoted=promote_to_same_type(bopi_ptr,op_chix,FALSE);
   left_bad = (Boolean) (((*bopi_ptr).item[0].comp != INT_TYPE)&&
                         ((*bopi_ptr).item[0].comp != REAL_TYPE));
   right_bad = (Boolean) (((*bopi_ptr).item[1].comp != INT_TYPE)&&
                          ((*bopi_ptr).item[1].comp != REAL_TYPE));
   if (left_bad || right_bad){
      set_arrow_sides(left_bad,right_bad);
      error_this_pos(E_NUMERICMISMATCH);
      reset_arrow_sides();}
   save_postfix(op);
   /*-------------------------------------------------------*/
   return(promoted);
}
/*===========================================================================*/
void promote_and_save(binary_operand_pair_info_type *bopi_ptr,
                      short op_chix,operation_type op,token dummy)
/*
   This function promotes the type of either the first or second operand
   in a binary operation pair and saves the operation on the postfix stack.
*/
{
   /*-------------------------------------------------------*/
   qq_promote_and_save(bopi_ptr,op_chix,op,dummy);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void promote_and_save_bool(binary_operand_pair_info_type *bopi_ptr,
                           short op_chix,operation_type op,token dummy)
/*
   This function promotes the type of either the first or second operand
   in a binary operation pair and saves the operation on the postfix stack.
*/
{
   Boolean left_bad,right_bad;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)) show_message("promote_and_save_bool");
   promote_to_same_type(bopi_ptr,op_chix,FALSE);
   left_bad = (Boolean) ((*bopi_ptr).item[0].comp != BOOL_TYPE);
   right_bad = (Boolean) ((*bopi_ptr).item[1].comp != BOOL_TYPE);
   if (left_bad || right_bad){
      set_arrow_sides(left_bad,right_bad);
      error_this_pos(E_NOTBOOLITEM);
      reset_arrow_sides();}
   save_postfix(op);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void promote_and_save_multiplicative
     (binary_operand_pair_info_type *bopi_ptr,short op_chix,
      operation_type nowop,token nowtok)
{
   type_flagword_type hold_type;
   Boolean left_bad,right_bad,integer_operator,promoted;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"\npromote_and_save_multiplicative");
fprintf(bugfile,
"([(%d,%d,%02x,%02x,%02x),(%d,%d,%02x,%02x,%02x),%02x,%02x],,,)\n",
(int) (*bopi_ptr).item[0].ixpo,
(int) (*bopi_ptr).item[0].ixin,
(int) (*bopi_ptr).item[0].type,
(int) (*bopi_ptr).item[0].comp,
(int) (*bopi_ptr).item[0].spec,
(int) (*bopi_ptr).item[1].ixpo,
(int) (*bopi_ptr).item[1].ixin,
(int) (*bopi_ptr).item[1].type,
(int) (*bopi_ptr).item[1].comp,
(int) (*bopi_ptr).item[1].spec,
(int) (*bopi_ptr).ans,
(int) (*bopi_ptr).spcans);
#endif
   if (bitset(debug_flagword,DBG_PRS)) show_message("promote_and_save_mult");
   integer_operator=(Boolean)
      ((nowop==OP_MOD)||(nowop==OP_CYC)||(nowop==OP_QUO));
   if (integer_operator){
      left_bad = (Boolean) ((*bopi_ptr).item[0].comp != INT_TYPE);
      right_bad = (Boolean) ((*bopi_ptr).item[1].comp != INT_TYPE);
      if (left_bad || right_bad){
         set_arrow_sides(left_bad,right_bad);
         show_err_at_pos_on_both(E_NOTINTEGER,op_chix,
                                 "MOD\\CYC\\DIV",CHAR_TYPE);
         reset_arrow_sides();}}
   else if (nowop==OP_DVD){ /* REAL DIVISION */
      hold_type=(*bopi_ptr).item[1].comp;
      (*bopi_ptr).item[1].comp=REAL_TYPE;
      promoted=promote_to_same_type(bopi_ptr,op_chix,FALSE);
      if (promoted) ++(*bopi_ptr).item[1].ixpo;
      (*bopi_ptr).item[1].comp=hold_type;}
   promote_and_save(bopi_ptr,op_chix,nowop,nowtok);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void promote_and_save_power(binary_operand_pair_info_type *bopi_ptr,
                            short op_chix,operation_type dummy,token startok)
{
   operation_type op;
   type_flagword_type hold_type,save_type;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)) show_message("promote_and_save_pow");
   save_type=(*bopi_ptr).item[0].comp;
   op=give_pow_op(startok,(*bopi_ptr).item[0].comp,(*bopi_ptr).item[1].comp);
   if (op==OP_NIX){
      hold_type=(*bopi_ptr).item[1].comp;
      (*bopi_ptr).item[1].comp=REAL_TYPE;
      promote_to_same_type(bopi_ptr,op_chix,FALSE);
      (*bopi_ptr).item[1].comp=hold_type;
      op=give_pow_op(startok,(*bopi_ptr).item[0].comp,
                             (*bopi_ptr).item[1].comp);
      if (op==OP_NIX){
         promote_to_same_type(bopi_ptr,op_chix,FALSE);
         op=give_pow_op(startok,(*bopi_ptr).item[0].comp,
                                (*bopi_ptr).item[1].comp);}}
   save_postfix(op);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
type_flagword_type parse_left_to_right
   (
      any_parse_routine first_rou,any_parse_routine second_rou,
      char *first_name,char *second_name,
      any_is_op_routine is_op,
      any_give_op_routine give_op,
      any_promote_routine promote_rou,
      Boolean many
   )
/*
   This function parses an expression from left to right.   Associativity
   is usually from left to right.   To obtain right to left associativity,
   use a recursive call on the "second_rou" parameter and specify "many"
   as FALSE.  If there is no associativity because the B.N.F. does not
   allow many successive uses of the operator, then specify "many" as
   FALSE.
*/
{
   token nowtok;
   operation_type nowop;
   token_info_type saved;
   short op_chix;
   binary_operand_pair_info_type bopi;
   Boolean is_var;
   /*-------------------------------------------------------*/
   DO_PARSE_1(first_rou,bopi,first_name);
   DEFAULT_ANS_AND_VARIABLE_STATUS();
   while (is_op(this.tok)){
      save_infix(nowop=give_op(nowtok=this.tok));
      /*------------------------  save this operator */
      SAVE_OP_AND_ADVANCE_TOKEN();
      /*------------------------  parse next */
      DO_PARSE_2(second_rou,bopi,second_name);
      /*------------------------  promote */
      promote_rou(&bopi,op_chix,nowop,nowtok);
      /*--------------------------------  move forwards ... */
      if (many) next_term_or_factor(&bopi);
      else goto loopend;}
loopend:
   /*---------------------------------------  check for variable vs constant */
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
void expand_tokens_if_function()
/*
   This function expands the token if the current token is either the name
   of a function or the name of an implicit.   Expansion continues and the
   macro expansion stack gets pushed as long as the first token of the
   expanded body is also the name of a function or an implicit.
*/
{
   Boolean repeat;
   /*-------------------------------------------------------*/
   for (repeat=TRUE;repeat;){
      repeat=FALSE;
      if (is_value_token(this.tok)){
         lookup_ident();
         if (is_an_implicit_function(this.tok,this.id_info_ptr)){
            repeat=TRUE;
            expand_implicit();}
         else if (is_a_regular_function(this.tok,this.id_info_ptr)){
            repeat=TRUE;
            expand_regular();}}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
type_flagword_type parse_subscript()
{
   type_flagword_type ans;
   /*-------------------------------------------------------*/
   ans=qq_parse_subscript(FALSE,(any_flagword_routine) NULL);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type qq_parse_subscript(Boolean only,any_flagword_routine saveit)
/*
   This function parses a subscript when the current token is already known
   to be an open bracket.   Upon exit, the current token will be the token
   immediately following the right bracket token which terminated the array
   subscript expression.
*/
{
   binary_operand_pair_info_type bopi;
   Boolean is_var,wild_present,repeat,wild[2];
   Boolean hold_allow,save_ok,hold_in_error,is_array,is_double,no_ident;
   Boolean arry_is_real,single_saving;
   operation_type hold_wild_okay_opcode,wok_opcode;
   short isub;
   identifier_info_type *arryname_idinfo;
   /*-------------------------------------------------------*/
   is_var=FALSE;
   save_ok=of_currently_okay;
   of_currently_okay=FALSE;
   wok_opcode=wild_okay_opcode;
   single_saving = (Boolean) (saveit != ((any_flagword_routine) NULL));
   /*----------------------------------------------  error if not array */
   arryname_idinfo = prev.id_info_ptr;
   no_ident = (Boolean) (arryname_idinfo==((identifier_info_type *) NULL));
   arry_is_real=is_array=is_double=FALSE;
   if (!no_ident){
      arry_is_real = (Boolean)
         ((((*arryname_idinfo).flags)&COMPUTATIONAL_TYPE_MASK)==REAL_TYPE);
      if (arry_is_real) ++wok_opcode;
      DECIDE_IS_ARRAY_IS_DOUBLE(arryname_idinfo);}
   if ((no_ident) || (!is_array))
      show_err_at_pos_on_both(E_IXONSCALAR,this.pos,prev.id,CHAR_TYPE);
   /*----------------------------------------------  [ */
   if (!only) save_infix(OP_I_LB);
   advance_one_token();
   /*----------------------------------------------  <numeric_expression> */
   (*(wild+1))=(*wild)=wild_present=FALSE;
   for (isub=0,repeat=TRUE;repeat;++isub){
      if (isub==1){
         if (!is_double)
            show_err_at_pos_on_both(E_NOTDOUBLE,this.pos,
                                    (*arryname_idinfo).name,CHAR_TYPE);
         if (single_saving) saveit(bopi.ans | EXPR_VARIABLE_TYPE);}
      if (this.tok==TK_MUL){
         if (wild_okay_opcode==OP_NIX) error_this_pos(E_WILDCONTEXT);
         wild[isub]=TRUE;
         if (wild_present) error_this_pos(E_DOUBLEWILD);
         wild_present=TRUE;
         bopi.item[0].type=INT_TYPE|EXPR_VARIABLE_TYPE; is_var=TRUE;
         AFTER_PARSE_1(bopi);
         save_infix(OP_I_WILD);
         advance_one_token();}
      else{
         /*----------------------------------*/
         hold_allow=reals_allowed; reals_allowed=FALSE;
         hold_in_error=reals_in_error_shown; reals_in_error_shown=FALSE;
         hold_wild_okay_opcode=wild_okay_opcode; wild_okay_opcode=OP_NIX;
         DO_PARSE_1(parse_numeric_expression,bopi,"parse_numeric_expression");
         reals_in_error_shown=hold_in_error;
         reals_allowed=hold_allow;
         wild_okay_opcode=hold_wild_okay_opcode;
         /*----------------------------------*/}
      DEFAULT_ANS_AND_VARIABLE_STATUS();
      if (bopi.item[0].comp==INT_TYPE);
      else{
         error_this_pos(E_NOTINTEGER);}
      if (this.tok==TK_COMMA){
         if (isub==1){
            error_this_pos(E_TOOMANYSUBS);
            repeat=FALSE;
            skip_to_lower_precedence(OP_I_CMMA);}
         else{
             if (!single_saving) save_infix(OP_I_CMMA);
             advance_one_token();}}
      else repeat=FALSE;}
   /*----------------------------------------------  ] */
   if ((isub<2)&&(is_double))
      show_err_at_pos_on_both(E_TOOFEWSUBS,this.pos,
                              (*arryname_idinfo).name,CHAR_TYPE);
   if (this.tok==TK_RB) advance_one_token();
   else error_this_pos(E_NOTCLOSEBRACKET);
   if (wild_present && (wild_okay_opcode!=OP_NIX)){
      if (this.tok==TK_RP);
      else if (this.tok==TK_COMMA);
      else{
         error_this_pos(E_NOTCOMMA);
         skip_to_next_comma_or(FALSE,TK_RP);}}
   if (!only){
      save_infix(OP_I_RB);
      if (wild_present){
         if (wild[0]) save_postfix(rowcol_version_op(wok_opcode,1));
         else save_postfix(rowcol_version_op(wok_opcode,0));}
      else{
         if (isub==2) save_postfix(OP_IXDBY2);
         else save_postfix(OP_IXDBY);}}
   of_currently_okay=save_ok;
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_builtin()
/*
   This function parses a reference to a built-in function when the current
   token is already known to be the reserved word name of a built-in function.
   Upon exit, the current token will be the right parenthesis which terminated
   the built-in function's parameter list.
*/
{
   short op_chix[VARIAB_MAX_ARGS],outpos,ix,tix,ttix,maxix;
   binary_operand_pair_info_type bopi;
   binary_operand_item_info_type boiis[VARIAB_MAX_ARGS];
   Boolean is_var,is_array;
   SAVE_AND_RESTORE_THIS_VARS;
   rwtype func_rw;
   token func_tok;
   built_in_parm_info_type *tableptr;
   type_flagword_type parm_comp,parm_spec,rtn_type;
   Boolean incorrect_count,count_less,do_skip,save_ok,do_an_exp,any_exprs;
   Boolean variable_length,array_and_index;
   char aux[82];
   operation_type tail_opcode,hold_opcode,corresponding_list_opcode,tmp_op;
   char tmpid[IDENT_MAXNCH_P];
   Memix infix_ix;
   operand_type tmpopnd;
   value_union_type tmpval;
   /*-------------------------------------------------------*/
   /*---------------------------------------  save builtin kind */
   func_rw=this.rw;
   func_tok=this.tok;
   tableptr = usual_builtin_op_info_table + (func_rw - RW_BUILTIN_0);
   advance_one_token();
   /*---------------------------------------  lookup expected param. info */
   /**/
   maxix=(*tableptr).parameter_count;
   variable_length = (Boolean) (maxix == VARIAB_LENG);
   array_and_index = (Boolean) (maxix == ARR_N_IX);
   if (variable_length) maxix=VARIAB_MAX_ARGS;
   if (array_and_index) maxix=2;
   /**/
   parm_comp = (*tableptr).parameter_type & COMPUTATIONAL_TYPE_MASK;
   parm_spec = (*tableptr).parameter_type & SPECIAL_TYPE_MASK;
   bopi.item[0].type = (*tableptr).parameter_type;
   bopi.item[0].comp = parm_comp;
   bopi.item[0].spec = parm_spec;
   /**/
   SET_TAIL_N_CORRESP((*tableptr).opcode);
   scalar_array_okay = (Boolean)
          (variable_length && bitset(parm_spec,ARRAY_TYPE));
   /*---------------------------------------  default result */
   rtn_type=builtin_return_type((*tableptr).return_type);
#ifdef FUTURE
future plans may call for making this less restrictive, depending upon
whether the function/implicit defines a constant expression.
#endif
   is_var=TRUE;
   do_skip=FALSE;
   /*---------------------------------------  must be parens, imply them */
   infix_ix=save_infix(tail_opcode);
   save_infix(OP_I_LP);
   /*---------------------------------------  parse inside parens */
   save_ok=of_currently_okay;
   any_exprs=FALSE;
   of_currently_okay=FALSE;
   if (this.tok==TK_LP){
      advance_one_token();
      for (ix=0;ix<maxix;++ix){
         if ((this.tok==TK_RP)||(this.tok==TK_SEMI)||(this.tok==TK_EOF)){
            if (variable_length) goto done_loop_point;
            count_less=incorrect_count=TRUE;
            goto done_count_point;}
         op_chix[ix]=this.pos;
         if (ix>0){
            save_infix(OP_I_CMMA);
            if (this.tok==TK_COMMA) advance_one_token();
            else error_this_pos(E_NOTCOMMA);}
         do_an_exp=TRUE;
         is_array=FALSE;
         if (this.tok==TK_ID){
            lookup_ident();
            is_array = bitset((*this.id_info_ptr).flags,ARRAY_TYPE);
            if (bitset(parm_spec,ARRAY_TYPE) && (is_array)){
               do_an_exp=FALSE;
               if (scalar_array_okay){
#ifdef OBSOLETE
if (this.tok!=TK_ID) do_an_exp=TRUE;
else
#endif
                  get_lookahead_token(tmpid);
                  if (((*tmpid)!=CLOSEPAREN)&&((*tmpid)!=COMMA))
                     do_an_exp=TRUE;}}}
         if (do_an_exp){
            any_exprs=TRUE;
            hold_opcode=wild_okay_opcode;
            if (variable_length)
               wild_okay_opcode = list_version_op((*tableptr).opcode);
            if (parm_comp==BOOL_TYPE){
               DO_PARSE_2(parse_boolean_expression,bopi,
                          "parse_boolean_expression");}
            else{
               DO_PARSE_2(parse_numeric_expression,bopi,
                          "parse_numeric_expression");}
            wild_okay_opcode=hold_opcode;}
         else if (!is_array){
            error_this_pos(E_NOTARRAY);}
         else{
            is_bool_arr_name = (Boolean)
               ((parm_comp==BOOL_TYPE)||(func_rw==RW_SIZE));
            is_num_arr_name = (Boolean) (parm_comp!=BOOL_TYPE);
            DO_PARSE_2(parse_array_name_aux,bopi,"parse_array_name");}
         if (parm_comp==UNKNOWN_TYPE){
            parm_comp=bopi.item[1].comp;
            bopi.item[0].comp =
               rtn_type = ((parm_comp == REAL_TYPE) ? REAL_TYPE : INT_TYPE);
            bopi.item[0].type |= rtn_type;
            if (rtn_type==REAL_TYPE){
               SET_TAIL_N_CORRESP((*tableptr).aux_opcode);}}
         if (variable_length){
            tmp_op = (*tableptr).opcode;
            if (bopi.item[1].comp==REAL_TYPE){
               tmp_op = (*tableptr).aux_opcode;
               SET_TAIL_N_CORRESP((*tableptr).aux_opcode);}
            if (!do_an_exp) save_postfix(tmp_op);
            if (bopi.item[1].comp==BOOL_TYPE){
               tmp_op = (*tableptr).aux_opcode;
               if (do_an_exp && (tmp_op!=OP_NIX)){
                  save_postfix((*tableptr).aux_opcode);
                  BtoI_CVT(bopi.item[1].type);
                  bopi.item[1].comp=INT_TYPE;}}
            bopi.item[1].ixpo=this_expr.n_postfix_ops;}
         if ((!reals_allowed)&&(!reals_in_error_shown)&&
             (rtn_type==REAL_TYPE)){
            set_arrow_sides(TRUE,FALSE);
            show_err_at_pos_on_both(E_NOREALS,this.pos,
                                    (*tableptr).label,CHAR_TYPE);
            reals_in_error_shown=TRUE;
            reset_arrow_sides();}
         if ((ix==1)&&(array_and_index)){
            if (bopi.item[1].comp!=INT_TYPE){
               error_this_pos(E_NOTINTEGER);}}
         else if (parm_comp!=bopi.item[1].comp){
            if (rtn_type < bopi.item[1].comp) rtn_type = bopi.item[1].comp;
            bopi.item[0].comp = rtn_type;
            bopi.item[0].type &= SPECIAL_TYPE_MASK;
            bopi.item[0].type |= rtn_type;
            for (tix=ix;tix>=0;--tix){
                if (tix<ix)
                   memcpy((char *) (&bopi.item[1]),(char *) (&boiis[tix]),
                          sizeof(binary_operand_item_info_type));
                if (promote_to_same_type(&bopi,op_chix[tix],TRUE)){
                   if (tix<ix) 
                      memcpy((char *) (&boiis[tix]),(char *) (&bopi.item[1]),
                             sizeof(binary_operand_item_info_type));
                   for (ttix=tix;ttix<ix;++ttix) boiis[ttix].ixpo++;
                   bopi.item[1].ixpo++;
                   rtn_type=parm_comp=bopi.item[1].comp;}}}
         memcpy((char *) (&boiis[ix]),(char *) (&bopi.item[1]),
                sizeof(binary_operand_item_info_type));}
done_loop_point:
      count_less=incorrect_count=FALSE;
      if (variable_length && (ix==0)) count_less=incorrect_count=TRUE;
done_count_point:
      outpos=this.pos;
      if (this.tok==TK_RP) advance_one_token();
      else do_skip=incorrect_count=TRUE;}
   else{
      ix=0;
      outpos=this.pos;
      error_this_pos(E_NOTOPENPAREN);
      incorrect_count=count_less=TRUE;}
   of_currently_okay=save_ok;
   /*---------------------------------------  */
   this_expr.infix_ops[infix_ix] = tail_opcode;
   if ((variable_length) && (ix==1)){
      this_expr.infix_ops[infix_ix] = tmp_op =
          cvt_infix_varying_to_single(tail_opcode);
      save_postfix(tmp_op);}
   if (tail_opcode==OP_COUNT){
      SET_TAIL_N_CORRESP(OP_ISUM);}
   /*---------------------------------------  insert varying parameter count */
   if ((ix>1)&&(variable_length)){
      save_infix(OP_INSVAL);
      sprintf(tmpid,"%c%d",NUMBER_TABLE_PREFIX_CHAR,(int) ix);
      tmpopnd=searchid_silent(tmpid,INT_TYPE,0,FALSE);
      save_operand(tmpopnd);
      if ((*tmpopnd).ptr.vvv == NULL){
         tmpval.iii=ix;
         (*tmpopnd).ptr.vvv=save_value_in_number_table(&tmpval,INT_TYPE);}
      save_postfix(OP_INSVAL);
      tail_opcode=corresponding_list_opcode;}
/*
   Test with:
       (*) One parameter and more than one parameter
       (*) COUNT,MIN,MAX,SUM,ANY,ALL
       (*) SCALARS,SINGLY ARRAYS,DOUBLY ARRAYS,EXPRESSIONS
*/
   /*---------------------------------------  insert omitted parameters */
   if (!variable_length){
      save_this();
      for (;ix<(*tableptr).parameter_count;++ix){
         if (ix>0) save_infix(OP_I_CMMA);
         dummy_token(parm_comp);
         lookup_ident();
         save_infix(OP_VAL);
         save_operand(this.id_info_ptr);
         save_postfix(OP_VAL);}
      restore_this();}
   /*---------------------------------------  must be parens, imply them */
   if ((ix>1)||(!variable_length)) save_postfix(tail_opcode);
   save_infix(OP_I_RP);
   /*---------------------------------------  now print postponed errors */
   if (incorrect_count){
      if (variable_length)
         sprintf(aux,"%s REQUIRES 1-%d PARAMETERS AND NO %s.",
                     (*tableptr).label,(int) maxix,
                     (count_less ? "FEWER" : "MORE"));
      else
         sprintf(aux,"%s REQUIRES EXACTLY %d PARAMETER%s AND NO %s.",
                     (*tableptr).label,(*tableptr).parameter_count,
                     yield_S_if_plural((*tableptr).parameter_count),
                     (count_less ? "FEWER" : "MORE"));
      show_err_at_pos_on_both((count_less ? E_NOTCOMMA : E_NOTCLOSEPAREN),
                              outpos,aux,CHAR_TYPE);}
   if (do_skip){
      skip_to_next_specified(FALSE,TK_RP);
      advance_one_token();}
   /*---------------------------------------  get ready to return */
   bopi.ans=rtn_type;
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_bin_numeric_item()
/*
   This function parses a <bin-numeric-item> as defined in the BNF syntax
   for the language.
*/
{
   token_info_type saved;
   short op_chix;
   binary_operand_pair_info_type bopi;
   Boolean is_var;
   Boolean save_ok,iz_arry;
   operation_type hold_wild_okay_opcode;
   /*-------------------------------------------------------*/
   bopi.ans=INT_TYPE;
   is_var=FALSE;
   expand_tokens_if_function();
   if (this.tok==TK_LP){
      /*----------------------------  expression in parentheses */
      save_ok=of_currently_okay;
      of_currently_okay=FALSE;
      save_infix(OP_I_LP);
      advance_one_token();
      hold_wild_okay_opcode=wild_okay_opcode; wild_okay_opcode=OP_NIX;
      DO_PARSE_1(parse_numeric_expression,bopi,"parse_numeric_expression");
      wild_okay_opcode=hold_wild_okay_opcode;
      DEFAULT_ANS_AND_VARIABLE_STATUS();
      save_infix(OP_I_RP);
      if (is_bool_op(this.tok)) error_this_pos(E_BOOLINARITH);
      if (this.tok==TK_RP) advance_one_token();
      else error_this_pos(E_NOTCLOSEPAREN);
      of_currently_okay=save_ok;}
   else if (is_a_builtin_function(this.tok,this.rw)){
      DO_PARSE_1(parse_builtin,bopi,"parse_builtin");
      DEFAULT_ANS_AND_VARIABLE_STATUS();}
   else if (is_value_token(this.tok)){
      /*----------------------------  */
      lookup_ident();
      if (!is_a_numeric_item(this.id_info_ptr)){
         error_this_pos(E_TYPEMISMATCH);
         skip_to_lower_precedence(OP_VAL);
         is_var=TRUE;}
      else{
         PARSE_VALUE_1();
         DEFAULT_ANS_AND_VARIABLE_STATUS();
         if ((!reals_allowed)&&(!reals_in_error_shown)&& 
             (bopi.ans & COMPUTATIONAL_TYPE_MASK)==REAL_TYPE){ 
            error_this_pos(E_NOREALS); 
            reals_in_error_shown=TRUE;} 
         SAVE_OP_AND_ADVANCE_TOKEN();
         if (is_cat_op(this.tok)){
            if ((saved.tok!=TK_ID)||
                (bitset(bopi.item[0].spec,ANY_SPECIAL_VAR_TYPE_MASK)))
               tail_error_saved_pos(E_NOTNAMEDCONSTANT,
                                    "BEFORE CONCATENATION OPERATOR");
            if (!parsing_rate_expression)
               tail_error_this_pos(E_NOTRATEEXPR,"(CONCATENATION)");
            save_infix(OP_CONCAT);
            op_chix=this.pos;
            advance_one_token();
            DO_PARSE_2(parse_bin_numeric_item,bopi,
                       "parse_bin_numeric_item");
            if (bopi.item[1].comp!=INT_TYPE){
               set_arrow_sides(FALSE,TRUE);
               show_err_at_pos_on_both(E_NOTINTEGER,op_chix,"^",CHAR_TYPE);
               reset_arrow_sides();}
            save_postfix(OP_CONCAT);}
         else{
            PARSE_SUBSCRIPT_IF_PRESENT();}}}
   else{
      if (this.tok==TK_RW) error_this_pos(E_KEYWCONTEXT);
      error_this_pos(E_NOTIDLIT);
      if (this.tok!=TK_SEMI) advance_one_token();}
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_array_name()
{
   scalar_array_okay=FALSE;
   return(qq_parse_array_name(FALSE,TRUE));
}
type_flagword_type parse_array_name_aux()
{
   Boolean izarr,iznum;
   izarr=is_bool_arr_name;
   iznum=is_num_arr_name;
   return(qq_parse_array_name(izarr,iznum));
}
type_flagword_type qq_parse_array_name(Boolean bool_arr,Boolean num_arr)
/*
   This function parses the name of an array for built-in functions which
   expect a whole array (such as SUM(ARR) and COUNT(BOOL_ARR)).
*/
{
   type_flagword_type type,ans;
   identifier_info_type *idinfo;
   register Boolean q;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,
"parse_array_name:: this.tok=%d, this.rw=%d, this.id=%s\n",
(int) this.tok,(int) this.rw,this.id);
#endif
   lookup_ident();
   idinfo=this.id_info_ptr;
   if ((is_a_numeric_item(idinfo) && (num_arr))||
       (is_a_boolean_item(idinfo) && (bool_arr))){
      PARSE_VALUE(type);
      if (!((scalar_array_okay)||(bitset(type,ARRAY_TYPE))))
         error_this_pos(E_NOTARRAY);
      advance_one_token();
      if (this.tok==TK_LB){
         show_err_at_pos_on_both(E_NOTWHOLEARRAY,this.pos,"[]",CHAR_TYPE);
         q=skip_nested_when_skipping(TK_RB,TRUE);}}
   else{
      error_this_pos(E_TYPEMISMATCH);
      if ((this.tok!=TK_RP)&&(this.tok!=TK_COMMA)) advance_one_token();}
   ans=(*idinfo).flags;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_numeric_item()
/*
   This function parses a <numeric-item> as defined in the BNF syntax
   for the language.
*/
{
   operation_type nowop;
   binary_operand_pair_info_type bopi;
   Boolean is_var;
   /*-------------------------------------------------------*/
   if (is_sign_op(this.tok)){
      save_infix(nowop=give_sign_op(this.tok));
      advance_one_token();
      DO_PARSE_1(parse_numeric_item,bopi,"parse_numeric_item");
#ifdef SHOULD_NOT_BE_REQUIRED
      if ((bopi.item[1].comp!=INT_TYPE)&&(bopi.item[1].comp!=REAL_TYPE))
         set_arrow_sides(FALSE,TRUE);
         show_err_at_pos_on_both(E_NOTNUMERIC,op_chix,"<sign>",CHAR_TYPE);
         reset_arrow_sides();}
#endif
      save_postfix(nowop);}
   else{
      DO_PARSE_1(parse_bin_numeric_item,bopi,"parse_bin_numeric_item");}
   DEFAULT_ANS_AND_VARIABLE_STATUS();
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_factor()
/*
   This function parses a <factor> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name1[] = "parse_numeric_item";
   static char name2[] = "parse_factor";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_numeric_item,parse_factor,name1,name2,
                           is_pow_op,give_infix_pow_op,promote_and_save_power,
                           FALSE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_term()
/*
   This function parses a <term> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name[] = "parse_factor";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_factor,parse_factor,name,name,
                           is_mpy_op,give_mpy_op,
                           promote_and_save_multiplicative,TRUE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_term_expr()
/*
   This function parses a <term-expr> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name[] = "parse_term";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_term,parse_term,name,name,
                           is_add_op,give_add_op,promote_and_save,TRUE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type init_and_parse_rate_expr(Boolean *errptr)
/*
   Note:  *errptr is TRUE whenever an error has occurred and therefore
          the expression cannot be evaluated without a possible core dump.
          Parsing can, however, continue without any ill effects as long
          as no attempt is made to evaluate the expression.
*/
{
   static Boolean of_okay=FALSE;
   short ofc;
   register type_flagword_type ans;
   /*-------------------------------------------------------*/
   parsing_rate_expression=TRUE;
   ans=init_and_parse_expression(errptr,of_okay,FALSE,&ofc);
   parsing_rate_expression=FALSE;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type init_and_parse_expression
     (Boolean *errptr,Boolean of_okay,Boolean empty_ok,short *of_count)
/*
   This function initializes the current expression to the empty expression
   and then parses a <numeric-expression> as defined in the BNF syntax
   for the language.

   Note:  *errptr is TRUE whenever an error has occurred and therefore
          the expression cannot be evaluated without a possible core dump.
          Parsing can, however, continue without any ill effects as long
          as no attempt is made to evaluate the expression.
*/
{
   register type_flagword_type ans;
#ifdef FUTURE_PUSH_OPTIONS
   register type_flagword_type anscomp;
#endif
   short old_error_count;
   Boolean reals_ok;
   /*-------------------------------------------------------*/
#ifdef BUGGY
   if (bitset(debug_flagword,DBG_PRS)&&bitset(debug_flagword,DBG_ROU))
      fprintf(bugfile,"::: init_and_parse_expression()\n");
#endif
   reals_ok = (Boolean) (parsing_rate_expression || parsing_convar_def);
   of_currently_okay=of_okay;
   old_error_count=error_count;
   init_expression(reals_ok);
   ans=recurse_the_parse(parse_numeric_expression,"parse_numeric_expression");
   if ((!empty_ok)&&(this_expr.n_operands<=0)) error_this_pos(E_MISSINGEXPR);
   *of_count=0;
   if (this_tok_is_rw(RW_OF)){
      if (ans!=INT_TYPE)
         error_this_pos(E_INTBEFOREOF);
      else
         *of_count=1;
      if (of_okay){
         *of_count=eval_for_expected_count(&this_expr,(int_type) 1,FALSE,0,0,TRUE);
         of_currently_okay=FALSE;
         advance_one_token();
         init_expression(reals_ok);
         ans=recurse_the_parse(parse_numeric_expression,"parse_numeric_expression");}
      else
         error_this_pos(E_KEYWCONTEXT);}
   *errptr = (Boolean) (error_count>old_error_count);
   if ((*errptr)) this_expr.in_error=TRUE;
   this_expr.rtntype=ans;
#ifdef FUTURE_PUSH_OPTIONS
   if (this_expr.n_postfix_ops != 1) goto done_point;
   if (this_expr.n_infix_ops != 1) goto done_point;
   if (this_expr.n_operands != 1) goto done_point;
   if (this_expr.postfix_ops[0] != OP_VAL) goto done_point;
   anscomp = ans & COMPUTATIONAL_TYPE_MASK;
   if (anscomp==INT_TYPE){
      if (strcmp((*this_expr.operands[0]).name,"0")==0)
          this_expr.postfix_ops[0] = OP_PZ;
      else if (strcmp((*this_expr.operands[0]).name,"1")==0)
          this_expr.postfix_ops[0] = OP_PU;}
   else if (anscomp==REAL_TYPE){
      if (strcmp((*this_expr.operands[0]).name,"0.0")==0)
          this_expr.postfix_ops[0] = OP_PRZ;
      else if (strcmp((*this_expr.operands[0]).name,"1.0")==0)
          this_expr.postfix_ops[0] = OP_PRU;}
done_point:
#endif
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_XEQ)) show_this_expr();
#ifdef BUGGY
   if (bitset(debug_flagword,DBG_PRS)&&bitset(debug_flagword,DBG_ROU))
      fprintf(bugfile,"::: init_and_parse_expression() [now done !!!]\n");
#endif
   return(ans);
}
/*===========================================================================*/
type_flagword_type init_and_parse_bool_expr
    (Boolean *errptr,Boolean of_okay,Boolean empty_ok,short *of_count)
/*
   This function initializes the current expression to the empty expression
   and then parses a <boolean-expression> as defined in the BNF syntax
   for the language.

   Note:  *errptr is TRUE whenever an error has occurred and therefore
          the expression cannot be evaluated without a possible core dump.
          Parsing can, however, continue without any ill effects as long
          as no attempt is made to evaluate the expression.
*/
{
   register type_flagword_type ans;
#ifdef FUTURE_PUSH_OPTIONS
   register type_flagword_type anscomp;
#endif
   short old_error_count;
   /*-------------------------------------------------------*/
   of_currently_okay=of_okay;
   old_error_count=error_count;
   init_expression(FALSE);
   ans=recurse_the_parse(parse_boolean_expression,"parse_boolean_expression");
   if ((!empty_ok)&&(this_expr.n_operands<=0)) error_this_pos(E_MISSINGEXPR);
   *of_count=0;
   if (this_tok_is_rw(RW_OF)){
      if (ans!=INT_TYPE)
         error_this_pos(E_INTBEFOREOF);
      else
         *of_count=1;
      if (of_okay){
         *of_count = eval_for_expected_count(&this_expr,(int_type) 1,FALSE,0,0,TRUE);
         of_currently_okay=FALSE;
         advance_one_token();
         init_expression(FALSE);
         ans=recurse_the_parse(parse_boolean_expression,"parse_boolean_expression");}
      else
         error_this_pos(E_KEYWCONTEXT);}
   *errptr = (Boolean) (error_count>old_error_count);
   if ((*errptr)) this_expr.in_error=TRUE;
   this_expr.rtntype=ans;
#ifdef FUTURE_PUSH_OPTIONS
   if (this_expr.n_postfix_ops != 1) goto done_point;
   if (this_expr.n_infix_ops != 1) goto done_point;
   if (this_expr.n_operands != 1) goto done_point;
   if (this_expr.postfix_ops[0] != OP_VAL) goto done_point;
   anscomp = ans & COMPUTATIONAL_TYPE_MASK;
   if (anscomp==BOOL_TYPE){
      if (strcmp((*this_expr.operands[0]).name,"FALSE")==0)
          this_expr.postfix_ops[0] = OP_PBZ;
      else if (strcmp((*this_expr.operands[0]).name,"TRUE")==0)
          this_expr.postfix_ops[0] = OP_PBU;}
done_point:
#endif
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_XEQ)) show_this_expr();
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_numeric_expr_silently()
/*
   This function parses a <numeric-expression> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   /*-------------------------------------------------------*/
   ans=parse_term_expr();
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_numeric_sub_expr_in_bool()
{
   register type_flagword_type ans;
   /*-------------------------------------------------------*/
   ans=parse_numeric_expr_silently();
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_numeric_expression()
{
   register type_flagword_type ans;
   /*-------------------------------------------------------*/
   ans=parse_numeric_expr_silently();
   if ((parsing_rate_expression) && (this.tok==TK_GT));
   else if (is_relation(this.tok)) error_this_pos(E_RELINNUMERIC);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_finish_numeric_comparison()
{
   type_flagword_type ans;
   Boolean isrel,isineq;
   /*-------------------------------------------------------*/
   ans=recurse_the_parse(parse_numeric_expr_silently,
                         "parse_numeric_expr_silently");
   isrel=is_relation(this.tok);
   isineq=is_ineq_relation(this.tok);
   if (isineq){
      error_this_pos(E_MULTIREL);
      skip_to_lower_precedence(OP_EQ);}
   else if (isrel){
      error_this_pos(E_MULTIEQ);
      skip_to_lower_precedence(OP_EQ);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_finish_bool_comparison()
{
   type_flagword_type ans;
   Boolean isrel;
   /*-------------------------------------------------------*/
   ans=recurse_the_parse(parse_bool_item,"parse_bool_item");
   isrel=is_bool_relation(this.tok);
   if (isrel){
      error_this_pos(E_MULTIBOOLEQ);
      skip_to_lower_precedence(OP_BOOL_EQ);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_simple_bool_item()
/*
   This function parses a <simple-bool-item> as defined in the BNF syntax
   for the language.
*/
{
   operation_type nowop;
   binary_operand_pair_info_type bopi;
   Boolean is_var;
   Boolean save_ok,iz_arry;
   operation_type hold_wild_okay_opcode;
   built_in_parm_info_type *tableptr;
   /*-------------------------------------------------------*/
   is_var=FALSE;
   expand_tokens_if_function();
   if (this.tok==TK_LP){
      /*----------------------------  expression in parentheses */
      save_ok=of_currently_okay;
      of_currently_okay=FALSE;
      save_infix(OP_I_LP);
      advance_one_token();
      hold_wild_okay_opcode=wild_okay_opcode; wild_okay_opcode=OP_NIX;
      DO_PARSE_1(parse_boolean_expression,bopi,"parse_boolean_expression");
      wild_okay_opcode=hold_wild_okay_opcode;
      DEFAULT_ANS_AND_VARIABLE_STATUS();
      save_infix(OP_I_RP);
      if (is_arith_op(this.tok)) error_this_pos(E_ARITHINBOOL);
      if (this.tok==TK_RP) advance_one_token();
      else error_this_pos(E_NOTCLOSEPAREN);
      of_currently_okay=save_ok;}
   else if (is_a_builtin_function(this.tok,this.rw)){
      tableptr = usual_builtin_op_info_table + (this.rw - RW_BUILTIN_0);
      if (((*tableptr).return_type & COMPUTATIONAL_TYPE_MASK) != BOOL_TYPE)
         error_this_pos(E_NOTBOOLITEM);
      DO_PARSE_1(parse_builtin,bopi,"parse_builtin");
      DEFAULT_ANS_AND_VARIABLE_STATUS();}
   else if (is_not_op(this.tok)){
      save_infix(nowop=give_not_op(this.tok));
      advance_one_token();
      DO_PARSE_1(parse_simple_bool_item,bopi,"parse_simple_bool_item");
      DEFAULT_ANS_AND_VARIABLE_STATUS();
#ifdef SHOULD_NOT_BE_REQUIRED
      if (bopi.item[1].comp!=BOOL_TYPE){
         set_arrow_sides(FALSE,TRUE);
         show_err_at_pos_on_both(E_NOTBOOL,op_chix,"NOT",CHAR_TYPE);
         reset_arrow_sides();}
#endif
      save_postfix(nowop);}
   else if (is_value_token(this.tok)){
      /*----------------------------  truth value or non-index-single */
      lookup_ident();
      if (!is_a_boolean_item(this.id_info_ptr)){
         error_this_pos(E_TYPEMISMATCH);
         skip_to_lower_precedence(OP_VAL);}
      else{
         PARSE_VALUE_1();
         DEFAULT_ANS_AND_VARIABLE_STATUS();
         advance_one_token();
         PARSE_SUBSCRIPT_IF_PRESENT();}}
   else{
      if (this.tok==TK_RW) error_this_pos(E_KEYWCONTEXT);
      error_this_pos(E_NOTIDLIT);
      if (this.tok!=TK_SEMI) advance_one_token();}
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_bool_item()
/*
   This function parses a <bool-item> as defined in the BNF syntax
   for the language.
*/
{
   operation_type nowop;
   short op_chix;
   binary_operand_pair_info_type bopi;
   built_in_parm_info_type *tableptr;
   Boolean is_var;
   Boolean numeric;
   token tmptok;
   rwtype tmprw;
   /*-------------------------------------------------------*/
   is_var=FALSE;
   expand_tokens_if_function();
   numeric=FALSE;
   if (this.tok==TK_LP){
      tmptok=give_token_after_matching(&tmprw);
#ifdef BUGGY
fprintf(bugfile,">>> token after matching = (tmptok=%d,tmprw=%d)\n",
(int) tmptok,(int) tmprw);
#endif
      numeric=(Boolean) (((tmptok==TK_RW)&&(tmprw==RW_OF)) ||
#ifdef IMPLICITS_USUALLY_NUMERIC
                         (tmptok==TK_LP)||
#endif
                         (is_arith_op(tmptok)) || is_relation(tmptok));}
   else if (is_value_token(this.tok)){
      lookup_ident();
      if (is_a_numeric_item(this.id_info_ptr)) numeric=TRUE;}
   else if (is_a_builtin_function(this.tok,this.rw)){
      numeric=TRUE;
      tableptr = usual_builtin_op_info_table + (this.rw - RW_BUILTIN_0);
      if (((*tableptr).return_type & COMPUTATIONAL_TYPE_MASK) == BOOL_TYPE)
         numeric=FALSE;}
   if (numeric){
      DO_PARSE_1(parse_numeric_sub_expr_in_bool,bopi,
                 "parse_numeric_sub_expr_in_bool");
      DEFAULT_ANS_AND_VARIABLE_STATUS();
      if (is_relation(this.tok)){
         save_infix(nowop=give_relation_op(this.tok));
         op_chix=this.pos;
         advance_one_token();
         DO_PARSE_2(parse_finish_numeric_comparison,bopi,
                    "parse_finish_numeric_comparison");
         PROMOTE_AND_STORE_POSTFIX(nowop);
         bopi.ans=BOOL_TYPE;
         if (is_eq_relation(nowop)){
            if (bopi.item[0].comp==REAL_TYPE)
               show_wrn_at_pos_on_both(E_REALEQ,op_chix,
                                       (void *) NULL,EMPTY_TYPE,
/* ???????? DEFAULT_WARN_LEV ???????? */
                                       MINOR_WARN_LEV);}}
      else if ((of_currently_okay)&&(this_tok_is_rw(RW_OF)))
         ;
      else{
         is_var=TRUE; bopi.ans=BOOL_TYPE;
         error_this_pos(E_NOTBOOLREL);
         skip_to_lower_precedence(TK_EQ);}}
   else{
      DO_PARSE_1(parse_simple_bool_item,bopi,"parse_simple_bool_item");
      DEFAULT_ANS_AND_VARIABLE_STATUS();}
   /*---------------------------------------  check for variable vs constant */
   adjust_ans_variable_status();
   /*-------------------------------------------------------*/
   return(bopi.ans);
}
/*===========================================================================*/
type_flagword_type parse_bool_factor()
/*
   This function parses a <bool-factor> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name[] = "parse_bool_item";
   static char name1[] = "parse_finish_bool_comparison";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_bool_item,parse_finish_bool_comparison,
                           name,name1,
                           is_bool_relation,give_bool_relation_op,
                           promote_and_save_bool,FALSE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_bool_term()
/*
   This function parses a <bool-term> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name[] = "parse_bool_factor";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_bool_factor,parse_bool_factor,name,name,
                           is_and_op,give_and_op,promote_and_save_bool,TRUE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_bool_term_expr()
/*
   This function parses a <bool-term-expr> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   static char name[] = "parse_bool_term";
   /*-------------------------------------------------------*/
   ans=parse_left_to_right(parse_bool_term,parse_bool_term,name,name,
                           is_or_op,give_or_op,promote_and_save_bool,TRUE);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
type_flagword_type parse_boolean_expression()
/*
   This function parses a <boolean-expression> as defined in the BNF syntax
   for the language.
*/
{
   register type_flagword_type ans;
   /*-------------------------------------------------------*/
   ans=parse_bool_term_expr();
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/

             /*******************************************/
             /*******************************************/
             /****                                   ****/
             /****   EXPRESSION EVALUATION SECTION   ****/
             /****                                   ****/
             /*******************************************/
             /*******************************************/

/*===========================================================================*/
int_type test_value_0_to_max(int_type aaa,int_type val_limit)
{
   int_type value,new_aaa;
   static char computed_msg[] = "IS THE COMPUTED VALUE.";
   /*-------------------------------------------------------*/
   new_aaa=aaa;
   set_arrow_sides(TRUE,FALSE);
   if (aaa>val_limit){
      val_error_this_pos(E_NUMBIG,aaa,computed_msg);
      new_aaa=val_limit;}
   if (aaa<((int_type) 0)){
      val_error_this_pos(E_NEGVALUE,aaa,computed_msg);
      new_aaa=0;}
   reset_arrow_sides();
   /*-------------------------------------------------------*/
   return(new_aaa);
}
/*===========================================================================*/
short eval_for_expected_count(expression_type *expr,
                              int_type dflt,
                              Boolean sub_index_expr,
                              short ix0,short oix0,
                              Boolean for_const)
{
   int_type aaa;
   /*-------------------------------------------------------*/
   aaa = eval_for_expected_integer
                (expr,dflt,sub_index_expr,ix0,-1,oix0,for_const);
   aaa=test_value_0_to_max(aaa,((int_type) (MAXSHORT)));
   /*-------------------------------------------------------*/
   return((short) aaa);
}
/*===========================================================================*/
ssvar_value_type eval_for_expected_ssv(expression_type *expr,
                                       int_type dflt,
                                       Boolean sub_index_expr,
                                       short ix0,short oix0,
                                       Boolean for_const,Boolean boolq)
{
   int_type aaa;
   /*-------------------------------------------------------*/
   if (boolq && (!sub_index_expr))
      aaa = (int_type) eval_for_expected_boolean(expr,(Boolean) dflt,for_const);
   else
      aaa = eval_for_expected_integer(expr,dflt,sub_index_expr,
                                      ix0,-1,oix0,for_const);
   aaa=test_value_0_to_max(aaa,((int_type) (MAXSSV)));
   /*-------------------------------------------------------*/
   return((ssvar_value_type) aaa);
}
/*===========================================================================*/
int_type fast_eval_for_expected_integer(expression_type *expr)
{
   register int_type ans;
   register type_flagword_type comp;
   value_union_type val;
   /*-------------------------------------------------------*/
   comp=fast_evaluate_full_expression(expr,&val);
   if (comp==INT_TYPE) ans=val.iii;
   else{
      eval_err(E_NOTINTEGER,"");
      ans=0;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
int_type eval_for_expected_integer(expression_type *expr,
                                   int_type dflt,
                                   Boolean sub_index_expr,
                                   short ix0,short ix9,short oix0,
                                   Boolean for_const)
/*
   This function evaluates a postfix expression or index sub-expression
   for an integer.   It the return type is not an integer, then an error
   message is displayed.
*/
{
   int_type ans;
   type_flagword_type comp;
   value_union_type val;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"eval_for_expected_integer::: ");
print_infix_expr(bugfile,expr,0,0,80,FALSE,FALSE);
fprintf(bugfile," :::\n");
#endif
   comp=evaluate_expression(expr,&val,sub_index_expr,ix0,ix9,oix0,for_const);
   ans=dflt;
   if (comp==INT_TYPE) ans=val.iii;
   else error_this_pos(E_NOTINTEGER);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean fast_eval_for_expected_boolean(expression_type *expr)
{
   register Boolean ans;
   register type_flagword_type comp;
   value_union_type val;
   /*-------------------------------------------------------*/
   comp=fast_evaluate_full_expression(expr,&val);
   if (comp==BOOL_TYPE) ans=val.bbb;
   else{
      eval_err(E_NOTBVAL,"");
      ans=FALSE;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean eval_for_expected_boolean
        (expression_type *expr,Boolean dflt,Boolean for_const)
/*
   This function evaluates a postfix expression for a boolean. 
   It the return type is not a boolean, then an error message is displayed.
*/
{
   Boolean ans;
   type_flagword_type comp;
   value_union_type val;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"eval_for_expected_boolean::: ");
print_infix_expr(bugfile,expr,0,0,80,FALSE,FALSE);
fprintf(bugfile," :::\n");
#endif
   comp=evaluate_expression(expr,&val,FALSE,0,-1,0,for_const);
   ans=dflt;
   if (comp==BOOL_TYPE) ans=val.bbb;
   else error_this_pos(E_NOTBVAL);
#ifdef BUGGY
print_infix_expr(bugfile,expr,0,0,80,FALSE,FALSE);
fprintf(bugfile," ::: eval_for_expected_boolean returns(%s)\n",
((ans) ? "TRUE" : "FALSE"));
#endif
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
short find_postfix_sub_expr(expression_type *expr,short oix)
/*
   This function returns the index in the postfix expression where
   the "oix"th operand appears, (indexed from 0).
*/
{
   register short knt,ixpost;
   register operation_type qqqop;
   /*-------------------------------------------------------*/
/*?????*/ if (bitset(debug_flagword,DBG_KOD)) fprintf(bugfile,"find_postfix_sub_expr(,%d)\n",(int) oix);
   for (ixpost=knt=0;ixpost<(*expr).n_postfix_ops;++ixpost){
      qqqop=(*((*expr).postfix_ops+((Memix) ixpost)));
/*?????*/ if (bitset(debug_flagword,DBG_KOD)) fprintf(bugfile,"  qqqop=%02x\n",(int) qqqop);
      if ((qqqop==OP_VAL)||(qqqop==OP_INSVAL)){
         if (knt==oix) return(ixpost);
         ++knt;}}
   /*-------------------------------------------------------*/
   return(ixpost);
}
/*===========================================================================*/
short skip_over_infix_expr
          (operation_type **tmpop_ptr,short *oix_ptr,
           Boolean multi,operation_type opend,operation_type nest_op,
           expression_type *expr,short ixpost)
{
   register short lev,nest_count,oix,ooo,ooo0;
   register operation_type *tmpop;
   register operation_type *posop;
   register operation_type op;
   /*-------------------------------------------------------*/
   ooo0 = oix = *oix_ptr;
   nest_count=0;
   tmpop = *tmpop_ptr;
#ifdef BAD
   for (lev=0;(multi || (oix==ooo0)) && ((lev>0)||((*tmpop)!=opend));++tmpop)
#endif
   for (lev=0;(multi) && ((lev>0)||((*tmpop)!=opend));++tmpop){
#ifdef BUGGY
fprintf(bugfile,
"skipping......... oix=%d, *tmpop=%02x\n",(int) oix,(int) *tmpop);
#endif
      op=(*tmpop);
      if ((op==OP_VAL)||(op==OP_INSVAL)) ++oix;
      if ((op==OP_I_RB)||(op==OP_I_RP)) --lev;
      if ((op==OP_I_LB)||(op==OP_I_LP)) ++lev;}
#ifdef BUGGY
fprintf(bugfile,
"done skipping.... oix=%d, *tmpop=%02x\n",(int) oix,(int) *tmpop);
#endif
   if (nest_op!=OP_NIX){
      for (posop=(*expr).postfix_ops+ixpost,ooo=ooo0;ooo<oix;++posop){
         op=(*posop);
         if ((op==OP_VAL)||(op==OP_INSVAL)) ++ooo;
#ifdef ERRONEOUS
if (cvt_to_infix_if_builtin(op)==nest_op) ++nest_count;
#endif
         if (op==nest_op) ++nest_count;}}
   /*-------------------------------------------------------*/
   *tmpop_ptr = tmpop;
   *oix_ptr = oix;
   return(nest_count);
}
/*===========================================================================*/
short find_postfix_finish(expression_type *expr,short ix0,short nskip,
                          operation_type nest_op)
/*
   This function returns the index in the posfix expression which is one
   past the postfix variable length operation which matches its infix
   equivalent.   The value of "nskip" must be the number of variable length
   builtin operations nested within the variable length builtin function
   in question inclusive of one additional for the variable length operation
   in question.

   Theorem:  The end of the postfix sub-expression is reached
             exactly one postfix operation past the nskip'th
             inserted count.

   Proof:  Exactly one count argument count parameter must be inserted by the
           parser for each variable length function.   This argument list
           length is always inserted as the last value in the list of
           arguments.    In a postfix expression, nested operations are
           always appear first.  The end of the postfix sub-expression must
           therefore follow the nskip'th inserted value.   This nskip'th
           inserted value corresponds to the builtin in question since
           there is exactly one inserted value for each nested variable
           length function.   Since the parser always inserts a constant
           value for the count (as opposed to an expression) and follows
           it immediately with the operation, the theorem follows.

*/
{
   register short knt,ixpost;
   register operation_type qqqop;
   /*-------------------------------------------------------*/
   qqqop=OP_NIX;
   for (knt=nskip,ixpost=ix0;(knt>0)&&(ixpost<(*expr).n_postfix_ops);++ixpost){
      qqqop=(*((*expr).postfix_ops+((Memix) ixpost)));
#ifdef ERRONEOUS
      qqqop=cvt_to_infix_if_builtin(qqqop);
#endif
#ifdef ERRONEOUS
      if (qqqop==op) --knt;
#endif
      if (qqqop==nest_op) --knt;}
   ++ixpost;
   /*-------------------------------------------------------*/
   return(ixpost);
}
/*===========================================================================*/
void eval_sub_infix_expr
            (expression_type *expr,
             short *oix_ptr,operation_type **tmpop_ptr,
             operation_type *op_out)
/*
   This function evaluates a sub-infix expression for a subscript or for
   the value to the right of a concatenation symbol.   The acceptable syntax
   is one of:
               V ^ V
               V ^ V [ , ]      (array values)
               V ^ B ( ... )    (built-in functions)
               V ^ ( ... )
               V [ ... ]
   where the elipsis (...) can be any integer expression and where the 
   "V" to the right of the carat (^) must evaluate to an integer.

   Upon entry, **tmpop_ptr must be either the infix concatenation or open
   bracket character.   Upon exit, **tmpop_ptr will be the infix "V" or the
   infix right paren or the infix right bracket.
*/
{
   operation_type *tmpop;
   short oix;
   register short ixpost;
   operation_type op,opend;
   int_type ival,jval;
   Boolean multi,is_double,can_extend;
   int_type *ans_ptr;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,"----- evaluating sub infix expression ------>\n");
fprintf(bugfile,"      "); print_infix_expr(bugfile,expr,0,0,80,FALSE,FALSE);
fprintf(bugfile,"\n");
fprintf(bugfile,"      ");
show_infix_or_postfix((*expr).infix_ops,(*expr).n_infix_ops,"infix");
fprintf(bugfile,"      ");
show_infix_or_postfix((*expr).postfix_ops,(*expr).n_postfix_ops,"postf");
fprintf(bugfile,"      oix=%d\n",(int) *oix_ptr);
#endif
   ans_ptr=eval_sub_infix_expr_ans;
   *ans_ptr=1; ans_ptr++;
   oix=(*oix_ptr);
   jval=SIMPLE_IDENTIFIER;
   /*----------------------------------  find postfix sub-expression */
   ixpost=find_postfix_sub_expr(expr,oix);
#ifdef BUGGY
fprintf(bugfile,"      ixpost=%d\n",(int) ixpost);
#endif
   /*----------------------------------  evaluate sub-infix-expression */
   ival=eval_for_expected_integer(expr,(int_type) 1,TRUE,ixpost,-1,oix,FALSE);
   is_double = (Boolean) 
       ((*((*expr).postfix_ops+((Memix) ee_ix_out)))==OP_IXDBY2);
   if (is_double) jval=ee_aux_val.iii;
   /*----------------------------------  determine how far to skip (infix) */
   tmpop = *tmpop_ptr;
   op=(*tmpop);
   opend=OP_NIX;
   multi=TRUE;
   if (op==OP_I_LB){
      opend=OP_I_RB;
      ++tmpop;}
   else{ /* op==OP_CONCAT */
      ++tmpop;
      op=(*tmpop);
      can_extend=(Boolean) (tmpop < ((*expr).infix_ops+(*expr).n_infix_ops-2));
      if (can_extend && is_builtin(op)){ /*  <builtin-func>  (  <expr>  ) */
         opend=OP_I_RP;
         tmpop += 2;}
      else if (op==OP_I_LP){ /*  (  <expr>  ) */
         opend=OP_I_RP;
         ++tmpop;}
      else if (can_extend && ((*(tmpop+1))==OP_I_LB)){ /* <array>  [ <expr> ] */
         ++oix;
         opend=OP_I_RB;
         tmpop += 2;}
      else{
         if ((op==OP_VAL)||(op==OP_INSVAL)) ++oix;
         multi=FALSE;}}
   /*----------------------------------  skip over infix sub-expression */
   skip_over_infix_expr(&tmpop,&oix,multi,opend,OP_NIX,expr,0);
   /*----------------------------------  return new values */
   *oix_ptr = oix;
   *op_out = (*tmpop);
   *tmpop_ptr = tmpop;
   /*-------------------------------------------------------*/
   *ans_ptr=ival;
   if (is_double){
      ++ans_ptr;
      *ans_ptr=jval;
      *eval_sub_infix_expr_ans=2;}
   return;
}
/*===========================================================================*/
short print_infix_expr(FILE *fp,expression_type *expr,
                       short indent,short nowcol,short ncols,
                       Boolean for_SURE,Boolean wrap_long)
/*
   This function prints the current expression on the file pointed to by fp.
   it returns the number of newline characters which it printed.   A newline
   character is NOT printed after printing the last line of the expression.
*/
{
   short oix,oix0;
   register short nix,jjj,nest_count,ixpost,ixtail;
   operation_type op;
   identifier_info_type *idinfo,*holdinfo;
   type_flagword_type comp;
   value_union_type value;
   int_type ival,jval,off;
   char *ppp;
   short linecount,col,need;
   operation_type *tmpop,*tmpopovf;
   Boolean is_array,extra,is_long,spacing;
   char enkxtr[MAX_INT_TYPE_NDIGS_P];
   char *prfx;
   static char prfx_empty[] = "";
   static char prfx_under[] = "_";
#define PRINT_INFIX_TEXT_PRINT(sss) \
        need=(short) strlen(sss); \
        col=adjust_wrapping_column(fp,need,indent,col,ncols,&linecount); \
        fprintf(fp,"%s",sss);
   /*-------------------------------------------------------*/
   col=nowcol;
   linecount=0;
   nix=(*expr).n_infix_ops;
   holdinfo = ((identifier_info_type *) NULL);
   is_long = FALSE;
   if (wrap_long){
      tmpop=(*expr).infix_ops;
      for (oix=0,tmpopovf=tmpop+nix;tmpop<tmpopovf;++tmpop){
           op = (*tmpop);
           if ((op==OP_INSVAL)||(op==OP_VAL)) oix++;}
      if (oix > 6) is_long=TRUE;}
   tmpop=(*expr).infix_ops;
   for (oix=0,tmpopovf=tmpop+nix;tmpop<tmpopovf;++tmpop){
      op = (*tmpop);
      if (op==OP_INSVAL) oix++;
      else if (op==OP_VAL){
         holdinfo = ((identifier_info_type *) NULL);
         idinfo = *((*expr).operands+((Memix) oix));
         ppp=(*idinfo).name;
         if ((*ppp)==NUMBER_TABLE_PREFIX_CHAR) ++ppp;
         is_array = (Boolean) (bitset((*idinfo).flags,ARRAY_TYPE));
         if ((for_SURE)&&(bitset((*idinfo).flags,ANY_SPECIAL_VAR_TYPE_MASK))){
            if (is_array) holdinfo=idinfo;
            else
               ppp=enk_ptr_union_value(&(*idinfo).ptr,(*idinfo).flags,
                                       (int_type) 0,eval_state_ssbits);}
         if (holdinfo == ((identifier_info_type *) NULL)){
            PRINT_INFIX_TEXT_PRINT(ppp);}
         oix++;}
      else if (is_builtin(op)){
         if ((for_SURE) && (builtin_invalid_in_SURE(op))){
            oix0=oix;
            ixpost=find_postfix_sub_expr(expr,oix); tmpop+=2;
            if (is_one_ary(op)){
               nest_count =
                    skip_over_infix_expr(&tmpop,&oix,TRUE,OP_I_RP,op,
                                         expr,ixpost);
               ixtail=find_postfix_finish(expr,ixpost,1+nest_count,op);
#ifdef OBSOLETE
if ((op==OP_ISUM1)||(op==OP_RSUM1)) --ixtail;
#endif
               --ixtail;}
            else{
               nest_count =
                    skip_over_infix_expr(&tmpop,&oix,TRUE,OP_I_RP,OP_INSVAL,
                                         expr,ixpost);
               ixtail=find_postfix_finish(expr,ixpost,nest_count,OP_INSVAL);}
            comp=evaluate_expression(expr,&value,FALSE,
                                     ixpost,ixtail,oix0,FALSE);
            ppp=enk_value_union_val((void *) &value,comp,eval_state_ssbits);
            PRINT_INFIX_TEXT_PRINT(ppp);}
         else{
            jjj=(short) (op-OP_BUILTIN_0);
            PRINT_INFIX_TEXT_PRINT(builtin_name_table[jjj]);}}
      else{
         need=calc_operation_text_leng(op,TRUE,&extra);
         spacing = (Boolean) ((extra) || (wrap_long && (op==OP_ADD)));
         if (spacing) need += 2;
         col=adjust_wrapping_column(fp,need,indent,col,ncols,&linecount);
         if ((for_SURE) && ((op==OP_CONCAT)||(op==OP_I_LB))){
            prfx = prfx_empty;
            if (op==OP_I_LB) prfx = prfx_under;
            eval_sub_infix_expr(expr,&oix,&tmpop,&op);
            *enkxtr='\0';
            if ((*eval_sub_infix_expr_ans)==2){
               jval=(*(eval_sub_infix_expr_ans+2));
               ppp=enk_aux_val(&jval,INT_TYPE);
               sprintf(enkxtr,"%s%s",prfx,ppp);}
            ival=(*(eval_sub_infix_expr_ans+1));
            if (holdinfo == ((identifier_info_type *) NULL)){
               ppp=enk_aux_val(&ival,INT_TYPE);
               fprintf(fp,"%s%s%s",prfx,ppp,enkxtr);}
            else{
               off = compute_offset(ival,jval,holdinfo);
               ppp=enk_ptr_union_value(&(*holdinfo).ptr,(*holdinfo).flags,
                                       off,eval_state_ssbits);
               fprintf(fp,"%s",ppp);}}
         else{
            if (spacing) fprintf(fp," ");
            show_operation_text(fp,op,TRUE);
            if (wrap_long && (op==OP_ADD)) col = ncols+1;
            else if (spacing) fprintf(fp," ");}
         holdinfo = ((identifier_info_type *) NULL);}}
   /*-------------------------------------------------------*/
   if (for_SURE) return(col);
   else return(linecount);
}
/*===========================================================================*/

      /****************************************************************/
      /****************************************************************/
      /****                                                        ****/
      /****   MISCELLANEOUS PARSING ROUTINES OF A GENERAL NATURE   ****/
      /****                                                        ****/
      /****************************************************************/
      /****************************************************************/

/*===========================================================================*/
void make_boolean(state_offset_type *offinfo,type_flagword_type *flags)
/*
   This function sets "offinfo" values to switch to BOOLEAN (0..1) type.
   It is called for the SPACE statement syntax "OF BOOLEAN" or ":BOOLEAN".
*/
{
   register type_flagword_type ttt;
   /*-------------------------------------------------------*/
   ttt = (*flags);
   ttt &= SPECIAL_TYPE_MASK;
   ttt |= BOOL_TYPE;
   *flags = ttt;
   (*offinfo).minval=0;
   (*offinfo).maxval=1;
   (*offinfo).bit_length=1;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void default_irange(state_offset_type *offinfo,
                    bitsize_type bitoff,bitsize_type off0)
/*
   This function sets "offinfo" values for the default <i-range> of 1..256.
   It is called after parsing the new state-space-variable in a SPACE 
   statement and before parsing the type/range of the values.
*/
{
   /*-------------------------------------------------------*/
   (*offinfo).minval = off0;
   (*offinfo).maxval = off0+((bitsize_type) 255);
   (*offinfo).bit_offset=bitoff;
   (*offinfo).bit_length=8;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_irange(state_offset_type *offinfo,ssvar_value_type maxdiff)
/*
   This function parses the <i-range> for a maximum difference of "maxdiff".
   The "bit_offset" field of the structure is not set.   It is assumed that
   the first token of the <i-range> has already been gotten.  Upon exit, 
   the first token after the <i-range> will have been gotten.   Usually,
   "maxdiff" should be specified as 255.

   Upon entry, the current token is the first token of the expression
   for the lower bound of the <i-range>.   Upon exit, the current token
   will be the first token past the end of the expression for the upper
   bound of the <i-range>.
*/
{
   ssvar_value_type ele1,ele2;
   int_type value;
   type_flagword_type typ;
   short dotdot_linnum;
   short dotdot_pos;
   short arrowpos,ofknt;
   bitsize_type vvv,nbits;
   Boolean err;
   char aux[50];
   /*---------------------------------------  <expression> */
   typ=init_and_parse_expression(&err,FALSE,FALSE,&ofknt);
   dotdot_linnum=this.linnum;
   arrowpos=dotdot_pos=this.pos;
   set_arrow_sides(TRUE,FALSE);
   ele1=(err ? 1
             : eval_for_expected_ssv(&this_expr,(int_type) 1,
               FALSE,0,0,TRUE,FALSE));
   reset_arrow_sides();
   /*---------------------------------------  .. (hopefully) */
   if (this.tok==TK_SUBRNGE){
      /*-----------------------  [ <expression> .. <expression> ] */
      advance_one_token();
      typ=init_and_parse_expression(&err,FALSE,FALSE,&ofknt);
      arrowpos = ((this.linnum==dotdot_linnum) ? dotdot_pos : this.pos);
      set_arrow_sides(FALSE,TRUE);
      ele2 = (err ? ele1
                  : eval_for_expected_ssv(&this_expr,ele1,
                                          FALSE,0,0,TRUE,FALSE));}
   else{
      /*-----------------------  [ <expression> ] */
      value=ele1;
      set_arrow_sides(FALSE,TRUE);
      show_wrn_at_pos_on_both(E_NOUPPERBOUND,this.pos,(void *) &value,
                              INT_TYPE,SERIOUS_WARN_LEV);
      skip_to_lower_precedence(OP_LOWOP);
      ele2=ele1;
      ele1=1;}
   /*---------------------------------------  errors for bad ranges */
   arrowpos = ((this.linnum==dotdot_linnum) ? dotdot_pos : this.pos);
   set_arrow_sides(TRUE,TRUE);
   if (ele1>ele2){
      sprintf(aux,"%d..%d",(int) ele1,(int) ele2);
      show_err_at_pos_on_both(E_BACKWDSRNGE,arrowpos,aux,CHAR_TYPE);
      ele2=ele1;}
   if ((ele2-ele1)>maxdiff){
      sprintf(aux,"%d (%d..%d)",(int) (maxdiff),(int) ele1,(int) ele2);
      show_err_at_pos_on_both(E_RNGEOVF,arrowpos,aux,CHAR_TYPE);
      ele2=ele1+maxdiff;}
#ifdef REDUNDANT
   /*---------------------------------------  negative indicies disallowed */
   if (ele1<0){
      set_arrow_sides(TRUE,FALSE);
      val_error_arrowpos(E_NEGVALUE,ele1,"COMPUTED");}
   if (ele2<0){
      set_arrow_sides(FALSE,TRUE);
      val_error_arrowpos(E_NEGVALUE,ele2,"COMPUTED");}
   reset_arrow_sides();
#endif
   /*---------------------------------------  save answer */
   (*offinfo).minval = ele1;
   (*offinfo).maxval = ele2;
   /*---------------------------------------  compute and save bit length */
   vvv = (bitsize_type) (ele2-ele1);
   for (nbits=0;vvv>0;++nbits) vvv = vvv >> ((unsigned) 1);
   (*offinfo).bit_length = ((bitsize_type) nbits);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_int_for_flag_set_spec(unsigned_31_type *what,Boolean with_clause)
{
   Boolean bad;
   /*-------------------------------------------------------*/
   if (this.tok==TK_INT){
      *what = 0;
      if (*(this.id)==NUMBER_TABLE_PREFIX_CHAR){
         bad=FALSE;
         if ((*(this.id+2))!='\0') bad=TRUE;
         else if ((*(this.id+1))=='0') *what = 0;
         else if ((*(this.id+1))=='1') *what = 1;
         else if ((*(this.id+1))=='2') *what = 2;
         else bad=TRUE;
         if (bad) show_err_at_pos_on_both
                       (E_BADVALUE,this.pos,this.id+1,CHAR_TYPE);}
      else error_this_pos(E_BADVALUE);
      advance_one_token();}
   else error_this_pos(E_NOTINTEGER);
   if ((with_clause)&&(this_tok_is_rw(RW_WITH))) return;
   skip_to_next_semicolon(FALSE);
   /*-------------------------------------------------------*/
   return;
}
/*===========================================================================*/
void parse_flag_set_spec
      (unsigned_31_type *what,Boolean with_clause,Boolean equal_sign_optional)
/*
   This function parses the flag set specification of the format:
         ON
         OFF
         FULL
         =0
         =1
         =2
   After completion, tokens are skipped up to the next semicolon unless
   "with_clause" is TRUE and a WITH clause follows.
*/
{
   /*-------------------------------------------------------*/
   *what = 1;
   if (this.tok==TK_EQ){
      /*-----------------------------------  <flag> = 0/1/2 */
      advance_one_token();
      parse_int_for_flag_set_spec(what,with_clause);
      return;}
   else if ((this.tok==TK_INT) && (equal_sign_optional)){
      /*-----------------------------------  <flag> = 0/1/2 */
      parse_int_for_flag_set_spec(what,with_clause);
      return;}
   else if (this.tok==TK_SEMI){
      /*-----------------------------------  <flag> ; */
      *what = 1;}
   else if (this.tok==TK_RW){
      /*-----------------------------------  <flag> ON/OFF/FULL  [WITH x] */
      if (this.rw==RW_OFF) *what = 0;
      else if (this.rw==RW_ON) *what = 1;
      else if (this.rw==RW_FULL) *what = 2;
      else if ((with_clause) && (this.rw==RW_WITH)){
         *what = 1;
         return;}
      else error_this_pos(E_KEYWCONTEXT);
      advance_one_token();
      if ((with_clause)&&(this_tok_is_rw(RW_WITH))) return;
      if (this.tok!=TK_SEMI)
         error_this_pos(E_NOTSEMI);}
   else{
      /*-----------------------------------  unable to parse */
      error_this_pos(E_NOTFLAGSET);}
   skip_to_next_semicolon(FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_this_expr()
{
   static char gdstr[] = "regular";
   static char instr[] = "  infix";
   static char postr[] = "postfix";
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_XEQX)) fprintf(bugfile,"\n");
   fprintf(bugfile,"%s: ",gdstr);
   print_infix_expr(bugfile,&this_expr,0,0,80,FALSE,FALSE);
   fprintf(bugfile,"\n");
   show_infix_or_postfix(this_expr.infix_ops,this_expr.n_infix_ops,instr);
   show_infix_or_postfix(this_expr.postfix_ops,this_expr.n_postfix_ops,postr);
   fprintf(bugfile,"\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
