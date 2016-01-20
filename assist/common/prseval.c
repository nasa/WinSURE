/*
     Compile with one of the following:
            cc -DSURE  ...
            cc -DPAWS  ...
            cc -DASSIST  ...
*/
/*
===============================================================================

        Parsing functions for ASSIST and SURE compiler programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

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
#include "rwdefs.h"
#include "errdefs.h"
#include "prsdefs.h"
#include "prstypes.h"
#include "prsvars.h"

#include "str_ext.h"
#include "lio_ext.h"
#include "pio_ext.h"
#include "lex_ext.h"
#include "prs_ext.h"
#include "eval_ext.h"
#include "fact_ext.h"
#include "user_ext.h"
#include "gamma.h"

extern void eval_stack_push_error(void);
extern void eval_stack_pop_error(void);
extern short qq_do_op_val(expression_type *,short);
extern short qq_fast_do_op_val(expression_type *,short);
extern void qq_perform_eval_expr(unsigned char);
extern type_flagword_type qq_eval_expr
           (expression_type *,value_union_type *,Boolean,short,short,short);
extern type_flagword_type qq_fast_eval_expr
           (expression_type *,value_union_type *);

/*===========================================================================*/
static type_flagword_type push_comps[OP_PUSHIARY_9-OP_PUSHIARY_0+1];
static value_union_type push_values[OP_PUSHIARY_9-OP_PUSHIARY_0+1];
static value_union_type eval_stack_ele[EVAL_STACK_DIM];
static operand_type eval_stack_opnd[EVAL_STACK_DIM];
static type_flagword_type eval_stack_x_comp[EVAL_STACK_DIM];
static Stacktop eval_stack_top;
static short expr_line_number;
#include "prs_math.h"
#define INPV(op,dorou) op_routines[op] = dorou
/*===========================================================================*/

                  /*************************************/
                  /*************************************/
                  /***                               ***/
                  /***   UNARY OPERATION FUNCTIONS   ***/
                  /***                               ***/
                  /*************************************/
                  /*************************************/

/*===========================================================================*/
extern void do_op_inc(void);
extern void do_op_dec(void);
extern void do_op_not(void);
extern void do_op_neg(void);
extern void do_op_stnchr(void);
extern void do_op_stnboo(void);
extern void do_op_stnint(void);
extern void do_op_stnre(void);
extern void do_op_itor(void);
extern void do_op_btoi(void);
extern void do_op_sqrt(void);
extern void do_op_exp(void);
extern void do_op_ln(void);
extern void do_op_abs(void);
extern void do_op_sin(void);
extern void do_op_cos(void);
extern void do_op_tan(void);
extern void do_op_arcsin(void);
extern void do_op_arccos(void);
extern void do_op_arctan(void);
extern void do_op_fact(void);
extern void do_op_gam(void);
extern void do_op_size(void);
extern void do_op_count(void);
extern void do_op_imin(void);
extern void do_op_rmin(void);
extern void do_op_imax(void);
extern void do_op_rmax(void);
extern void do_op_isum(void);
extern void do_op_rsum(void);
extern void do_op_any(void);
extern void do_op_all(void);
/*===========================================================================*/

                  /**************************************/
                  /**************************************/
                  /***                                ***/
                  /***   BINARY OPERATION FUNCTIONS   ***/
                  /***                                ***/
                  /**************************************/
                  /**************************************/

/*===========================================================================*/
extern void do_op_or(void);
extern void do_op_xor(void);
extern void do_op_and(void);
extern void do_op_bool_eq(void);
extern void do_op_bool_ne(void);
extern void do_op_lt(void);
extern void do_op_gt(void);
extern void do_op_le(void);
extern void do_op_ge(void);
extern void do_op_eq(void);
extern void do_op_ne(void);
extern void do_op_add(void);
extern void do_op_sub(void);
extern void do_op_mul(void);
extern void do_op_mod(void);
extern void do_op_cyc(void);
extern void do_op_quo(void);
extern void do_op_dvd(void);
extern void do_op_rpowr(void);
extern void do_op_ipowi(void);
extern void do_op_rpowi(void);
extern void do_op_comb(void);
extern void do_op_perm(void);
extern void do_op_irowmin(void);
extern void do_op_rrowmin(void);
extern void do_op_icolmin(void);
extern void do_op_rcolmin(void);
extern void do_op_irowmax(void);
extern void do_op_rrowmax(void);
extern void do_op_icolmax(void);
extern void do_op_rcolmax(void);
extern void do_op_irowsum(void);
extern void do_op_rrowsum(void);
extern void do_op_icolsum(void);
extern void do_op_rcolsum(void);
extern void do_op_rowcount(void);
extern void do_op_colcount(void);
extern void do_op_rowany(void);
extern void do_op_colany(void);
extern void do_op_rowall(void);
extern void do_op_colall(void);
extern void do_op_ixdby(void);
/*===========================================================================*/

                 /***************************************/
                 /***************************************/
                 /***                                 ***/
                 /***   TERNARY OPERATION FUNCTIONS   ***/
                 /***                                 ***/
                 /***************************************/
                 /***************************************/

/*===========================================================================*/
extern void do_op_ixdby2(void);
/*===========================================================================*/

         /*******************************************************/
         /*******************************************************/
         /***                                                 ***/
         /***   VARIABLE ARGUMENT COUNT OPERATION FUNCTIONS   ***/
         /***                                                 ***/
         /*******************************************************/
         /*******************************************************/

/*===========================================================================*/
extern void do_op_liscount(void);
extern void do_op_ilismin(void);
extern void do_op_rlismin(void);
extern void do_op_ilismax(void);
extern void do_op_rlismax(void);
extern void do_op_ilissum(void);
extern void do_op_rlissum(void);
extern void do_op_lisany(void);
extern void do_op_lisall(void);
/*===========================================================================*/

                       /***************************/
                       /***************************/
                       /***                     ***/
                       /***   COMMON ROUTINES   ***/
                       /***                     ***/
                       /***************************/
                       /***************************/

static any_void_routine op_routines[256];
/*===========================================================================*/
void init_prseval()
{
   register short i;
   /*-------------------------------------------------------*/
   for (i=0;i<256;++i) op_routines[i] = ((any_void_routine) NULL);
   /*---------------------------------------------  push values */
   push_values[OP_PZ-OP_PUSHIARY_0].iii = 0;
   push_values[OP_PBZ-OP_PUSHIARY_0].bbb = FALSE;
   push_values[OP_PRZ-OP_PUSHIARY_0].rrr = 0.0000000000000000000000000000;
   push_values[OP_PU-OP_PUSHIARY_0].iii = 1;
   push_values[OP_PBU-OP_PUSHIARY_0].bbb = TRUE;
   push_values[OP_PRU-OP_PUSHIARY_0].rrr = 1.0000000000000000000000000000;
   push_comps[OP_PZ-OP_PUSHIARY_0] = INT_TYPE;
   push_comps[OP_PBZ-OP_PUSHIARY_0] = BOOL_TYPE;
   push_comps[OP_PRZ-OP_PUSHIARY_0] = REAL_TYPE;
   push_comps[OP_PU-OP_PUSHIARY_0] = INT_TYPE;
   push_comps[OP_PBU-OP_PUSHIARY_0] = BOOL_TYPE;
   push_comps[OP_PRU-OP_PUSHIARY_0] = REAL_TYPE;
   /*---------------------------------------------  UNARY operations */
   INPV(OP_INC,do_op_inc);
   INPV(OP_DEC,do_op_dec);
   INPV(OP_NOT,do_op_not);
   INPV(OP_NEG,do_op_neg);
   INPV(OP_STNCHR,do_op_stnchr);
   INPV(OP_STNBOO,do_op_stnboo);
   INPV(OP_STNINT,do_op_stnint);
   INPV(OP_STNRE,do_op_stnre);
   INPV(OP_ItoR,do_op_itor);
   INPV(OP_BtoI,do_op_btoi);
   INPV(OP_SQRT,do_op_sqrt);
   INPV(OP_EXP,do_op_exp);
   INPV(OP_LN,do_op_ln);
   INPV(OP_ABS,do_op_abs);
   INPV(OP_SIN,do_op_sin);
   INPV(OP_COS,do_op_cos);
   INPV(OP_TAN,do_op_tan);
   INPV(OP_ARCSIN,do_op_arcsin);
   INPV(OP_ARCCOS,do_op_arccos);
   INPV(OP_ARCTAN,do_op_arctan);
   INPV(OP_GAM,do_op_gam);
   INPV(OP_FACT,do_op_fact);
   INPV(OP_SIZE,do_op_size);
   INPV(OP_COUNT,do_op_count);
   INPV(OP_IMIN,do_op_imin);
   INPV(OP_RMIN,do_op_rmin);
   INPV(OP_IMAX,do_op_imax);
   INPV(OP_RMAX,do_op_rmax);
   INPV(OP_ISUM,do_op_isum);
   INPV(OP_RSUM,do_op_rsum);
   INPV(OP_ANY,do_op_any);
   INPV(OP_ALL,do_op_all);
   /*---------------------------------------------  BINARY operations */
   INPV(OP_OR,do_op_or);
   INPV(OP_XOR,do_op_xor);
   INPV(OP_AND,do_op_and);
   INPV(OP_BOOL_EQ,do_op_bool_eq);
   INPV(OP_BOOL_NE,do_op_bool_ne);
   INPV(OP_LT,do_op_lt);
   INPV(OP_GT,do_op_gt);
   INPV(OP_LE,do_op_le);
   INPV(OP_GE,do_op_ge);
   INPV(OP_EQ,do_op_eq);
   INPV(OP_NE,do_op_ne);
   INPV(OP_ADD,do_op_add);
   INPV(OP_SUB,do_op_sub);
   INPV(OP_MUL,do_op_mul);
   INPV(OP_MOD,do_op_mod);
   INPV(OP_CYC,do_op_cyc);
   INPV(OP_QUO,do_op_quo);
   INPV(OP_DVD,do_op_dvd);
   INPV(OP_RPOWR,do_op_rpowr);
   INPV(OP_IPOWI,do_op_ipowi);
   INPV(OP_RPOWI,do_op_rpowi);
   INPV(OP_COMB,do_op_comb);
   INPV(OP_PERM,do_op_perm);
   INPV(OP_IROWMIN,do_op_irowmin);
   INPV(OP_RROWMIN,do_op_rrowmin);
   INPV(OP_ICOLMIN,do_op_icolmin);
   INPV(OP_RCOLMIN,do_op_rcolmin);
   INPV(OP_IROWMAX,do_op_irowmax);
   INPV(OP_RROWMAX,do_op_rrowmax);
   INPV(OP_ICOLMAX,do_op_icolmax);
   INPV(OP_RCOLMAX,do_op_rcolmax);
   INPV(OP_IROWSUM,do_op_irowsum);
   INPV(OP_RROWSUM,do_op_rrowsum);
   INPV(OP_ICOLSUM,do_op_icolsum);
   INPV(OP_RCOLSUM,do_op_rcolsum);
   INPV(OP_ROWCOUNT,do_op_rowcount);
   INPV(OP_COLCOUNT,do_op_colcount);
   INPV(OP_ROWANY,do_op_rowany);
   INPV(OP_COLANY,do_op_colany);
   INPV(OP_ROWALL,do_op_rowall);
   INPV(OP_COLALL,do_op_colall);
#ifdef FUTURE
   INPV(OP_ANYPOS,do_op_anypos);
   INPV(OP_ALLPOS,do_op_allpos);
   INPV(OP_OTHER,do_op_other);
#endif
   INPV(OP_IXDBY,do_op_ixdby);
   /*---------------------------------------------  TERNARY operations */
   INPV(OP_IXDBY2,do_op_ixdby2);
   /*---------------------------------------------  variable arglist opers. */
   INPV(OP_LISCOUNT,do_op_liscount);
   INPV(OP_ILISMIN,do_op_ilismin);
   INPV(OP_RLISMIN,do_op_rlismin);
   INPV(OP_ILISMAX,do_op_ilismax);
   INPV(OP_RLISMAX,do_op_rlismax);
   INPV(OP_ILISSUM,do_op_ilissum);
   INPV(OP_RLISSUM,do_op_rlissum);
   INPV(OP_LISANY,do_op_lisany);
   INPV(OP_LISALL,do_op_lisall);
#ifdef FUTURE
   INPV(OP_ANYPOS,do_op_anypos);
   INPV(OP_ALLPOS,do_op_allpos);
   INPV(OP_OTHER,do_op_other);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void eval_err(short ee,char *txt)
{
   char msg[100];
   /*-------------------------------------------------------*/
   sprintf(msg,"%s(expression on line#%d)",txt,(int) expr_line_number);
   show_err_at_pos_on_both(ee,this.pos,msg,CHAR_TYPE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define XMSG_BUFFLEN 50
#ifdef BUGGY
#define EVAL_STRCAT(dest,src) safe_strcat_truncated(dest,src,XMSG_BUFFLEN)
#else
#define EVAL_STRCAT(dest,src) strcat(dest,src)
#endif
/*===========================================================================*/
real_type real_eval_err(short ee,real_type x,char *s,real_type y)
/*
   This routine displays the error message along with the encoded
   string "x s y" ("%e%s%e" format).   The "expr_line_number" is also
   displayed as part of the error message.
*/
{
   char xmsg[XMSG_BUFFLEN];
   real_type vvv;
   /*-------------------------------------------------------*/
   *xmsg='\0';
   if (s!=((char *) NULL)){
      vvv=x; EVAL_STRCAT(xmsg,enk_aux_val(&vvv,REAL_TYPE));
      EVAL_STRCAT(xmsg,s);
      vvv=y; EVAL_STRCAT(xmsg,enk_aux_val(&vvv,REAL_TYPE));
      EVAL_STRCAT(xmsg,", ");}
   eval_err(ee,xmsg);
   /*-------------------------------------------------------*/
   return(ZERO);
}
/*===========================================================================*/
int_type int_eval_err(short ee,int_type x,char *s,int_type y)
/*
   This routine displays the error message along with the encoded
   string "x s y" ("%d%s%d" format).   The "expr_line_number" is also
   displayed as part of the error message.
*/
{
   char xmsg[XMSG_BUFFLEN];
   int_type vvv;
   /*-------------------------------------------------------*/
   *xmsg='\0';
   if (s!=((char *) NULL)){
      vvv=x; EVAL_STRCAT(xmsg,enk_aux_val(&vvv,INT_TYPE));
      EVAL_STRCAT(xmsg,s);
      vvv=y; EVAL_STRCAT(xmsg,enk_aux_val(&vvv,INT_TYPE));
      EVAL_STRCAT(xmsg,", ");}
   eval_err(ee,xmsg);
   /*-------------------------------------------------------*/
   return((int_type) 0);
}
/*===========================================================================*/
int_type ixtoy(int_type x,int_type y)
/*
   This function raises an integer to an integer power via a series of
   safe multiplications.
*/
{
   register int_type ans,i,n;
   /*-------------------------------------------------------*/
   if (y<0) return((int_type) 0);
   else if (y==0) return((int_type) 1);
   ans=1;
   for (i=0,n=y;i<y;++i) ans=impy(ans,x);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
real_type rtoi(real_type x,int_type y)
/*
   This function raises a real to an integer power via a series of
   safe multiplications.
*/
{
   register int_type i,n;
   register real_type ans;
   /*-------------------------------------------------------*/
   if (y<0) return(ZERO);
   else if (y==0) return(ONE);
   ans=ONE;
   for (i=0,n=y;i<n;++i) ans=rmpy(ans,x);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void eval_stack_push_error()
{
   char msg[512];
   char tmsg[12];
   /*-------------------------------------------------------*/
   strcpy(msg,"Expression evaluation stack overflow.  ");
   strcat(msg,"Try simplifying expression via use of named constants or ");
   strcat(msg,"re-build ");
   strcat(msg,language);
   strcat(msg," with a larger value for EVAL_STACK_DIM.  ");
   strcat(msg,"(expression on line#");
   sprintf(tmsg,"%d",(int) expr_line_number);
   strcat(msg,tmsg);
   strcat(msg,")");
   show_err_at_pos_on_both(E_INTERNAL,this.pos,msg,CHAR_TYPE);
   abort_program();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void eval_stack_pop_error()
{
   char msg[512];
   char tmsg[12];
   /*-------------------------------------------------------*/
   strcpy(msg,"Cannot pop empty expression stack. (expression on line#");
   sprintf(tmsg,"%d",(int) expr_line_number);
   strcat(msg,tmsg);
   strcat(msg,")");
   show_err_at_pos_on_both(E_INTERNAL,this.pos,msg,CHAR_TYPE);
   abort_program();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
static value_union_type l_val,r_val,c_val,m_val[VARIAB_MAX_ARGS];
static operand_type l_opnd,r_opnd,c_opnd,m_opnd[VARIAB_MAX_ARGS];
static type_flagword_type l_comp,r_comp,c_comp,m_comp[VARIAB_MAX_ARGS];
static type_flagword_type l_x_comp,r_x_comp,c_x_comp,m_x_comp[VARIAB_MAX_ARGS];
static Boolean is_ssv,is_int,is_bool;
static any_void_routine rou_to_perform;
/*===========================================================================*/
#define PPEV(vv,xc,opnd) \
        (enk_aux_val((void *) ((opnd==NULL) ? (vv) : (opnd)), \
                     ((opnd==NULL) ? (xc) : (Q_ADDR_TYPE))))
/*===========================================================================*/
#define empty_eval_stack() eval_stack_top=(-1)
static Boolean trace_evaluation=FALSE;
static Boolean trace_for_const=FALSE;
/*===========================================================================*/
#define qq_push_eval_stack(val,x_comp,qopnd) \
        ++eval_stack_top; \
        if (eval_stack_top>=EVAL_STACK_DIM) eval_stack_push_error(); \
        ixtop=((Memix) eval_stack_top); \
        *(eval_stack_opnd+ixtop) = qopnd; \
        *(eval_stack_x_comp+ixtop) = x_comp; \
        val_union_cpy(eval_stack_ele+ixtop,val)
#define push_eval_stack(val,x_comp,qopnd) \
        if (trace_evaluation) \
           fprintf(bugfile,"push: %s, %04x, %s\n\n", \
                   PPEV((void *) (val),x_comp,qopnd),(int) x_comp, \
                   (*(qopnd)).name); \
        qq_push_eval_stack(val,x_comp,qopnd)
#define brief_push_eval_stack(val,x_comp) \
        if (trace_evaluation) \
           fprintf(bugfile,"push: %s, %04x\n\n", \
                   PPEV((void *) (val),x_comp,NULL),(int) x_comp); \
        qq_push_eval_stack(val,x_comp,NULL)
/*===========================================================================*/
static char RightsidePopFmt[] = "popR: %s, %04x%s%s\n";
static char LeftsidePopFmt[]  = "popL: %s, %04x%s%s\n";
static char CenterPopFmt[]    = "popC: %s, %04x%s%s\n";
static char MultiPopFmt[]     = "popM: %s, %04x%s%s\n";
/**/
#define qq_pop_eval_stack(val,x_comp,qopnd,fmt) \
        if (eval_stack_top<0) eval_stack_pop_error(); \
        ixtop=(Memix) eval_stack_top; \
        val_union_cpy(val,eval_stack_ele+ixtop); \
        qopnd = *(eval_stack_opnd+ixtop); \
        x_comp = *(eval_stack_x_comp+ixtop); \
        --eval_stack_top
#define pop_eval_stack(val,x_comp,qopnd,fmt) \
        qq_pop_eval_stack(val,x_comp,qopnd,fmt); \
        if (trace_evaluation) \
           fprintf(bugfile,fmt, \
                   PPEV((void *) (val),x_comp,qopnd),(int) x_comp, \
                   ((qopnd==NULL) ? "" : ", "), \
                   ((qopnd==NULL) ? "" : (*(qopnd)).name))
/*===========================================================================*/

                  /*************************************/
                  /*************************************/
                  /***                               ***/
                  /***   UNARY OPERATION FUNCTIONS   ***/
                  /***                               ***/
                  /*************************************/
                  /*************************************/

/*===========================================================================*/
void do_op_inc(){++l_val.iii;}
void do_op_dec(){--l_val.iii;}
void do_op_not(){l_val.bbb = (Boolean) (!l_val.bbb);}
void do_op_neg()
{if (is_int) l_val.iii=(-l_val.iii); else l_val.rrr=(-l_val.rrr);}
void do_op_stnchr(){l_val.ccc=' ';l_comp=CHAR_TYPE;}
void do_op_stnboo(){l_val.bbb=FALSE;l_comp=BOOL_TYPE;}
void do_op_stnint(){l_val.iii=0;l_comp=INT_TYPE;}
void do_op_stnre(){l_val.rrr=ZERO;l_comp=REAL_TYPE;}
void do_op_itor(){l_val.rrr=l_val.iii;l_comp=REAL_TYPE;}
void do_op_btoi(){l_val.iii=(int_type) l_val.bbb;l_comp=INT_TYPE;}
void do_op_sqrt(){l_val.rrr=rsqrt(l_val.rrr);}
void do_op_exp(){l_val.rrr=rexp(l_val.rrr);}
void do_op_ln(){
l_val.rrr=rln(l_val.rrr);
if (errno==EDOM) RERR(E_LNOFNEG,l_val.rrr,NULL,l_val.rrr);}
void do_op_abs()
{if (is_int) l_val.iii=labs(l_val.iii); else l_val.rrr=fabs(l_val.rrr);}
void do_op_sin(){l_val.rrr=sin(l_val.rrr);}
void do_op_cos(){l_val.rrr=cos(l_val.rrr);}
void do_op_tan(){l_val.rrr=tan(l_val.rrr);
if (errno==ERANGE) RERR(E_TANSING,l_val.rrr,NULL,l_val.rrr);}
void do_op_arcsin(){l_val.rrr=r_asin(l_val.rrr);
if (errno==ERANGE) RERR(E_BADARCSIN,l_val.rrr,NULL,l_val.rrr);}
void do_op_arccos(){l_val.rrr=r_acos(l_val.rrr);
if (errno==ERANGE) RERR(E_BADARCCOS,l_val.rrr,NULL,l_val.rrr);}
void do_op_arctan(){l_val.rrr=atan(l_val.rrr);}
void do_op_fact(){l_val.iii = ifact(l_val.iii);}
void do_op_gam(){int ierr; real_type ans;
ierr=cgamma(l_val.rrr,&ans);
if (ierr!=0){RERR(E_REALOVF,0,NULL,0);}
l_val.rrr=ans;}
void do_op_size(){
l_val.iii = ((int_type) dim_range((*l_opnd).index.dims.first)) *
            ((int_type) dim_range((*l_opnd).index.dims.second));
l_comp=INT_TYPE;}
void do_op_count(){
arraycount(l_opnd,&l_val,l_comp,eval_state_ssbits); l_comp=INT_TYPE;}
void do_op_imin(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraymin(l_opnd,&l_val,INT_TYPE,eval_state_ssbits);}
void do_op_rmin(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraymin(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits);}
void do_op_imax(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraymax(l_opnd,&l_val,INT_TYPE,eval_state_ssbits);}
void do_op_rmax(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraymax(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits);}
void do_op_isum(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraysum(l_opnd,&l_val,INT_TYPE,eval_state_ssbits);}
void do_op_rsum(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraysum(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits);}
void do_op_any()
{do_op_count(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=BOOL_TYPE;}
void do_op_all(){int_type prod; do_op_count();
prod = ((int_type) dim_range((*l_opnd).index.dims.first)) *
       ((int_type) dim_range((*l_opnd).index.dims.second));
l_val.bbb = (Boolean) (l_val.iii==prod); l_comp=BOOL_TYPE;}
/*===========================================================================*/

                  /**************************************/
                  /**************************************/
                  /***                                ***/
                  /***   BINARY OPERATION FUNCTIONS   ***/
                  /***                                ***/
                  /**************************************/
                  /**************************************/

/*===========================================================================*/
void do_op_or(){l_val.bbb = (Boolean) (l_val.bbb || r_val.bbb);}
void do_op_xor()
{l_val.bbb=(Boolean) ((l_val.bbb || r_val.bbb)&&(!(l_val.bbb && r_val.bbb)));}
void do_op_and(){l_val.bbb = (Boolean) (l_val.bbb && r_val.bbb);}
void do_op_bool_eq(){l_val.bbb = (Boolean) (l_val.bbb == r_val.bbb);}
void do_op_bool_ne(){l_val.bbb = (Boolean) (l_val.bbb != r_val.bbb);}
void do_op_lt(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii < r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr < r_val.rrr);}
void do_op_gt(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii > r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr > r_val.rrr);}
void do_op_le(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii <= r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr <= r_val.rrr);}
void do_op_ge(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii >= r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr >= r_val.rrr);}
void do_op_eq(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii == r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr == r_val.rrr);}
void do_op_ne(){l_comp=BOOL_TYPE;
if (is_int) l_val.bbb=(Boolean) (l_val.iii != r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr != r_val.rrr);}
void do_op_add(){
if (is_int) l_val.iii=iadd(l_val.iii,r_val.iii);
else l_val.rrr=radd(l_val.rrr,r_val.rrr);}
void do_op_sub(){
if (is_int) l_val.iii=isub(l_val.iii,r_val.iii);
else l_val.rrr=rsub(l_val.rrr,r_val.rrr);}
void do_op_mul(){
if (is_int) l_val.iii=impy(l_val.iii,r_val.iii);
else l_val.rrr=rmpy(l_val.rrr,r_val.rrr);}
void do_op_mod(){l_val.iii=imod(l_val.iii,r_val.iii);}
void do_op_cyc(){l_val.iii=icyc(l_val.iii,r_val.iii);}
void do_op_quo(){l_val.iii=iquo(l_val.iii,r_val.iii);}
void do_op_dvd(){l_val.rrr=rdvd(l_val.rrr,r_val.rrr);}
void do_op_rpowr(){l_val.rrr=rxtoy(l_val.rrr,r_val.rrr);}
void do_op_ipowi(){l_val.iii=ixtoy(l_val.iii,r_val.iii);}
void do_op_rpowi(){l_val.rrr=rtoi(l_val.rrr,r_val.iii);}
void do_op_comb(){l_val.iii = icomb(l_val.iii,r_val.iii);}
void do_op_perm(){l_val.iii = iperm(l_val.iii,r_val.iii);}
void do_op_irowmin(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arrayrowmin(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowmin(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arrayrowmin(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolmin(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraycolmin(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolmin(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraycolmin(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_irowmax(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arrayrowmax(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowmax(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arrayrowmax(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolmax(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraycolmax(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolmax(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraycolmax(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_irowsum(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arrayrowsum(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowsum(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arrayrowsum(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolsum(){if (l_comp!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
arraycolsum(l_opnd,&l_val,INT_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolsum(){if (l_comp!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
arraycolsum(l_opnd,&l_val,REAL_TYPE,eval_state_ssbits,(short) r_val.iii);}
void do_op_rowcount(){
arrayrowcount(l_opnd,&l_val,l_comp,eval_state_ssbits,r_val.iii);
l_comp=INT_TYPE;}
void do_op_colcount(){
arraycolcount(l_opnd,&l_val,l_comp,eval_state_ssbits,r_val.iii);
l_comp=INT_TYPE;}
void do_op_rowany()
{do_op_rowcount(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=BOOL_TYPE;}
void do_op_colany()
{do_op_colcount(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=BOOL_TYPE;}
void do_op_rowall()
{do_op_rowcount();
l_val.bbb = (Boolean) (l_val.iii==dim_range((*l_opnd).index.dims.second));
l_comp=BOOL_TYPE;}
void do_op_colall(){do_op_colcount();
l_val.bbb = (Boolean) (l_val.iii==dim_range((*l_opnd).index.dims.first));
l_comp=BOOL_TYPE;}
/*---------------------*/
void do_op_ixdby()
{
#if (defined(INT_32_BIT) || defined(INT_16_BIT))
   FAST_STATE_OFFSET_REGS();
#endif
   ssvar_value_type stateval;
   state_offset_type stateoff;
   register int_type tmp_off;
   l_x_comp=0;
   tmp_off=0;
   if ((r_val.iii<(*l_opnd).index.dims.first.lower)||
       (r_val.iii>(*l_opnd).index.dims.first.upper)){
      eval_err(E_DIMOUTOFBOUNDS,"");
      user_show_extra_after_error(l_opnd,r_val.iii,NULL_INDEX);}
   else
      tmp_off = r_val.iii - ((int_type) (*l_opnd).index.dims.first.lower);
   if (is_ssv){
      FAST_STATE_OFFSET_COPY(&stateoff,(*l_opnd).ptr.sss);
      stateoff.bit_offset +=
         ((bitsize_type) ((stateoff.bit_length * tmp_off)));
      stateval=state_space_value(eval_state_ssbits,&stateoff);
      if (l_comp==BOOL_TYPE) l_val.bbb=((stateval==0) ? FALSE : TRUE);
      else l_val.iii = (int_type) stateval;}
   else if (is_int) l_val.iii = *(l_val.ptr.iii+tmp_off);
   else if (is_bool) l_val.bbb = *(l_val.ptr.bbb+tmp_off);
   else l_val.rrr = *(l_val.ptr.rrr+tmp_off);
}
/*===========================================================================*/

                 /***************************************/
                 /***************************************/
                 /***                                 ***/
                 /***   TERNARY OPERATION FUNCTIONS   ***/
                 /***                                 ***/
                 /***************************************/
                 /***************************************/

/*===========================================================================*/
void do_op_ixdby2()
{
#if (defined(INT_32_BIT) || defined(INT_16_BIT))
FAST_STATE_OFFSET_REGS();
#endif
   ssvar_value_type stateval;
   state_offset_type stateoff;
   register int_type tmp_off;
   l_x_comp=0;
   tmp_off=0;
   if ((c_val.iii<(*l_opnd).index.dims.first.lower)||
       (c_val.iii>(*l_opnd).index.dims.first.upper)){
      eval_err(E_DIMOUTOFBOUNDS,"");
      user_show_extra_after_error(l_opnd,c_val.iii,NULL_INDEX);}
   else if ((r_val.iii<(*l_opnd).index.dims.second.lower)||
            (r_val.iii>(*l_opnd).index.dims.second.upper)){
      eval_err(E_DIMOUTOFBOUNDS,"");
      user_show_extra_after_error(l_opnd,NULL_INDEX,r_val.iii);}
   else{
      tmp_off = compute_offset(c_val.iii,r_val.iii,l_opnd);}
   if (is_ssv){
      FAST_STATE_OFFSET_COPY(&stateoff,(*l_opnd).ptr.sss);
      stateoff.bit_offset +=
           ((bitsize_type) ((stateoff.bit_length * tmp_off)));
      stateval=state_space_value(eval_state_ssbits,&stateoff);
      if (l_comp==BOOL_TYPE) l_val.bbb=((stateval==0) ? FALSE : TRUE);
      else l_val.iii = (int_type) stateval;}
   else if (is_int) l_val.iii = *(l_val.ptr.iii+tmp_off);
   else if (is_bool) l_val.bbb = *(l_val.ptr.bbb+tmp_off);
   else l_val.rrr = *(l_val.ptr.rrr+tmp_off);
}
/*===========================================================================*/

         /*******************************************************/
         /*******************************************************/
         /***                                                 ***/
         /***   VARIABLE ARGUMENT COUNT OPERATION FUNCTIONS   ***/
         /***                                                 ***/
         /*******************************************************/
         /*******************************************************/

/*===========================================================================*/
void do_op_liscount()
{
   int_type iii,knt;
   for (knt=iii=0;iii<c_val.iii;++iii){
      if ((*(m_opnd+iii))==NULL){
         if ((*(m_val+iii)).bbb) ++knt;}
      else{
         arraycount(*(m_opnd+iii),&l_val,*(m_comp+iii),eval_state_ssbits);
         knt += l_val.iii;}}
   l_val.iii = knt;
   l_comp=INT_TYPE;
}
/*---------------------*/
void do_op_ilismin()
{
   register int_type temp;
   register int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         temp = (*(m_val+iii)).iii;
         if ((iii==0)||(temp<cum)) cum = temp;}
      else{
         arraymin(*(m_opnd+iii),&l_val,INT_TYPE,eval_state_ssbits);
         temp = l_val.iii;
         if ((iii==0)||(temp<cum)) cum = temp;}}
   l_val.iii = cum;
   l_comp=INT_TYPE;
}
/*---------------------*/
void do_op_rlismin()
{
   register real_type temp;
   register int_type iii;
   register real_type cum;
   for (iii=0,cum=ZERO;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         temp = (*(m_val+iii)).rrr;
         if ((iii==0)||(temp<cum)) cum = temp;}
      else{
         arraymin(*(m_opnd+iii),&l_val,REAL_TYPE,eval_state_ssbits);
         temp = l_val.rrr;
         if ((iii==0)||(temp<cum)) cum = temp;}}
   l_val.rrr = cum;
   l_comp=REAL_TYPE;
}
/*---------------------*/
void do_op_ilismax()
{
   register int_type temp;
   register int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         temp = (*(m_val+iii)).iii;
         if ((iii==0)||(temp>cum)) cum = temp;}
      else{
         arraymax(*(m_opnd+iii),&l_val,INT_TYPE,eval_state_ssbits);
         temp = l_val.iii;
         if ((iii==0)||(temp>cum)) cum = temp;}}
   l_val.iii = cum;
   l_comp=INT_TYPE;
}
/*---------------------*/
void do_op_rlismax()
{
   register real_type temp;
   register int_type iii;
   register real_type cum;
   for (iii=0,cum=ZERO;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         temp = (*(m_val+iii)).rrr;
         if ((iii==0)||(temp>cum)) cum = temp;}
      else{
         arraymax(*(m_opnd+iii),&l_val,REAL_TYPE,eval_state_ssbits);
         temp = l_val.rrr;
         if ((iii==0)||(temp>cum)) cum = temp;}}
   l_val.rrr = cum;
   l_comp=REAL_TYPE;
}
/*---------------------*/
void do_op_ilissum()
{
   int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=INT_TYPE) IERR(E_NOTINTEGER,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         cum += (*(m_val+iii)).iii;}
      else{
         arraysum(*(m_opnd+iii),&l_val,INT_TYPE,eval_state_ssbits);
         cum += l_val.iii;}}
   l_val.iii = cum;
   l_comp=INT_TYPE;
}
/*---------------------*/
void do_op_rlissum()
{
   int_type iii;
   real_type cum;
   for (iii=0,cum=ZERO;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=REAL_TYPE) RERR(E_NOTREAL,0,NULL,0);
      if ((*(m_opnd+iii))==NULL){
         cum += (*(m_val+iii)).rrr;}
      else{
         arraysum(*(m_opnd+iii),&l_val,REAL_TYPE,eval_state_ssbits);
         cum += l_val.rrr;}}
   l_val.rrr = cum;
   l_comp=REAL_TYPE;
}
/*---------------------*/
void do_op_lisany()
{
   do_op_liscount();
   l_val.bbb = (Boolean) (l_val.iii>0);
   l_comp=BOOL_TYPE;
}
/*---------------------*/
void do_op_lisall()
{
   int_type iii;
   for (iii=0;iii<c_val.iii;++iii){
      if ((*(m_opnd+iii))==NULL){
         l_val.bbb = (*(m_val+iii)).bbb;}
      else{
         l_opnd = (*(m_opnd+iii));
         l_comp = (*(m_comp+iii));
         val_union_cpy(&l_val,m_val+iii);
         do_op_all();}
      if (!l_val.bbb) goto done_pt;}
done_pt:
   l_comp=BOOL_TYPE;
}
/*===========================================================================*/
#define is_done_single_qq(oo) \
        ((((oo)==OP_IXDBY) || ((oo)==OP_CONCAT)) && (eval_stack_top==0))
#define is_done_double_qq(oo) \
        (((oo)==OP_IXDBY2) && (eval_stack_top==1))
#define sub_and_is_done(oo) \
        (sub_index_expr && (is_done_single_qq(oo) || is_done_double_qq(oo)))
/**/
/*===========================================================================*/
#define BAD_UNARY_IX ((unsigned char) 0)
#define BAD_BINARY_IX ((unsigned char) 1)
#define BAD_TERNARY_IX ((unsigned char) 2)
#define BAD_VARARG_IX ((unsigned char) 3)
void qq_perform_eval_expr(unsigned char errmsg_ix)
/*.......char *errmsg */
{
   static char *errmsg[] = 
      {
         "BAD UNARY OP",
         "BAD BINARY OP",
         "BAD TERNARY OP",
         "BAD VARIABLE ARGUMENT LENGTH BUILT-IN OP"
      };
   /*-------------------------------------------------------*/
   is_int=(Boolean) (l_comp==INT_TYPE);
   is_bool=(Boolean) (l_comp==BOOL_TYPE);
   is_ssv=FALSE;
   if (l_opnd != NULL) is_ssv=bitset((*l_opnd).flags,SSVAR_TYPE);
   if (rou_to_perform==((any_void_routine) NULL)){
     show_err_at_pos_on_both(E_INTERNAL,this.pos,errmsg[errmsg_ix],CHAR_TYPE);}
   else rou_to_perform();
   l_x_comp &= SPECIAL_TYPE_MASK;
   l_x_comp |= l_comp;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define do_op_val(eee,ooo) qq_do_op_val(eee,ooo)
/*===========================================================================*/
short qq_do_op_val(expression_type *expr,short oix_in)
{
#include "doopval.c"
}
/*===========================================================================*/
type_flagword_type qq_eval_expr
              (expression_type *expr,value_union_type *val,
               Boolean sub_index_expr,short ix0,short ix9,short oix0)
{
#include "evalexpr.c"
}
/*===========================================================================*/
type_flagword_type evaluate_expression
                      (expression_type *expr,value_union_type *val,
                       Boolean sub_index_expr,short ix0,short ix9,short oix0,
                       Boolean for_const)
{
   type_flagword_type ans;
   /*-------------------------------------------------------*/
   trace_evaluation = bitset(debug_flagword,DBG_KOD);
   trace_for_const = for_const;
   if (trace_evaluation){
      fprintf(bugfile,"\nevaluate_expression(,,%c,%d,%d,%d,%c) :: \n",
              (sub_index_expr ? 'T' : 'F'),(int) ix0,(int) ix9,(int) oix0,
              (trace_for_const ? 'T' : 'F'));
      show_infix_or_postfix((*expr).postfix_ops,(*expr).n_postfix_ops,
                            "postfix");}
   if ((*expr).in_error){
      show_err_at_pos_on_both(E_INTERNAL,this.pos,
                              "Attempt to evaluate an expression in error.",
                              CHAR_TYPE);
      l_comp=((*expr).rtntype & COMPUTATIONAL_TYPE_MASK);
      standard_value(val,l_comp);
      return((*expr).rtntype);}
   ans = qq_eval_expr(expr,val,sub_index_expr,ix0,ix9,oix0);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
#define QQ_RUNNING_FAST
#undef push_eval_stack
#undef brief_push_eval_stack
#undef pop_eval_stack
#define push_eval_stack(val,x_comp,qopnd) qq_push_eval_stack(val,x_comp,qopnd)
#define brief_push_eval_stack(val,x_comp) qq_push_eval_stack(val,x_comp,NULL)
#define pop_eval_stack(val,x_comp,qopnd,fmt) \
        qq_pop_eval_stack(val,x_comp,qopnd,fmt)
#undef do_op_val
#define do_op_val(eee,ooo) qq_fast_do_op_val(eee,ooo)
/*===========================================================================*/
short qq_fast_do_op_val(expression_type *expr,short oix_in)
{
#include "doopval.c"
}
/*===========================================================================*/
type_flagword_type qq_fast_eval_expr
              (expression_type *expr,value_union_type *val)
{
#include "evalexpr.c"
}
/*===========================================================================*/
type_flagword_type fast_evaluate_full_expression
                      (expression_type *expr,value_union_type *val)
{
   type_flagword_type ans;
   /*-------------------------------------------------------*/
   trace_evaluation = FALSE;
   trace_for_const = FALSE;
   ans = qq_fast_eval_expr(expr,val);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
