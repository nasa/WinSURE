/*
     Compile with one of the following:
            cc -DSURE  ...
            cc -DPAWS  ...
            cc -DASSIST  ...
*/
/*
===============================================================================

        Macro Parsing functions for ASSIST and SURE compiler programs.

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

#include "astrw.h"

#include "objdefs.h"
#include "objtypes.h"
#include "objvars.h"

#include "astdefs.h"
#include "asttypes.h"

#include "macvars.h"

#include "astr_ext.h"
#include "aprs_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "obj_ext.h"
#include "mac_ext.h"

static Boolean gotten;

#define BODY_INDENT 2
#define is_at_first_body_token(mip) \
        ( (*(mip)).now_body_ix == (*(mip)).ini_body_ix )
#define is_at_last_body_token(mip) \
        ( (*(mip)).now_body_ix == ((*(mip)).ovf_body_ix-1) )
/*===========================================================================*/
short fetch_next_passed_token
      (macro_expansion_info_type *macinf,short nowix,short nowknt)
{
   register short newknt;
   token_info_type *tokinf;
   /*-------------------------------------------------------*/
   tokinf = (*macinf).passed_token_list + 
            ((Memix) *((*macinf).passed_token_offset +
                       ((Memix) nowix))) +
            ((Memix) nowknt);
   insert_token_info_as_this(tokinf,(*macinf).pos,(*macinf).linnum);
   newknt=nowknt+1;
   /*-------------------------------------------------------*/
   return(newknt);
}
/*===========================================================================*/
short fetch_next_body_token(macro_expansion_info_type *macinf,short nowix)
{
   register short newix;
   token_info_type *tokinf;
   /*-------------------------------------------------------*/
   tokinf = function_body_storage + (*macinf).now_body_ix;
   insert_token_info_as_this(tokinf,(*macinf).pos,(*macinf).linnum);
   newix=nowix+1;
   /*-------------------------------------------------------*/
   return(newix);
}
/*===========================================================================*/
void get_tok_after_matching_paren(char *id,Boolean matching_already_scanned)
{
   token_info_type save_this;
   macro_expansion_info_type save_macinf;
   macro_expansion_info_type *macinf;
   macro_expansion_info_type *back_macinf;
   Boolean rrr,repeat,popit,found,was_impl;
   short lev;
   /*-------------------------------------------------------*/
   sys_memcpy(&save_this,&this,sizeof(token_info_type));
   repeat=(Boolean) (macro_stack_top>=0);
   found=matching_already_scanned;
   if (repeat){
      macinf = macro_stack + ((Memix) macro_stack_top);
      sys_memcpy(&save_macinf,macinf,sizeof(macro_expansion_info_type));
      popit=FALSE;
      lev=0;
#ifdef BUGGY
fprintf(bugfile,
">>>scanning single expansion token:: this(0)=(%s,tok=%d,rw=%d),",
this.id,(int) this.tok,(int) this.rw);
fprintf(bugfile," r=%d,p=%d,w=%d\n",(int) repeat,(int) popit,(int) was_impl);
#endif
      if (this.tok==TK_LP) ++lev;
      while (repeat && (!popit)){
         if (found) repeat=FALSE;
         rrr=advance_one_expansion_token(macinf,&popit,&was_impl,FALSE);
#ifdef BUGGY
fprintf(bugfile,
">>>scanning single expansion token:: this=(%s,tok=%d,rw=%d),",
this.id,(int) this.tok,(int) this.rw);
fprintf(bugfile," r=%d,p=%d,w=%d\n",(int) repeat,(int) popit,(int) was_impl);
#endif
         if ((!rrr)&&(!found)){
            if (this.tok==TK_LP) ++lev;
            if (this.tok==TK_RP){
               --lev;
               if (lev==0) found=TRUE;}
            repeat=TRUE;}}
      if (popit && (macro_stack_top>0)){
         back_macinf = macro_stack + ((Memix) (macro_stack_top-1));
         fetch_next_body_token(back_macinf,(*back_macinf).now_body_ix);}
      else if (popit)
         tok_after_matching_right_paren(this.id,found);
      sys_memcpy(macinf,&save_macinf,sizeof(macro_expansion_info_type));}
   else
      tok_after_matching_right_paren(this.id,found);
   strcpy(id,this.id);
   sys_memcpy(&this,&save_this,sizeof(token_info_type));
#ifdef BUGGY
fprintf(bugfile,"\nget_tok_after_matching_paren (id=%s)::>>>\n",id);
#endif
}      
/*===========================================================================*/
token give_token_after_matching(rwtype *rwans)
{
/*
   This function scans ahead through the input file looking for the token
   following the matching right parenthesis.   It is called when parsing
   a boolean expression in order to determine whether an expression in
   parentheses is numeric or boolean.
*/
   char id[IDENT_MAXNCH_P];
   char idx[IDENT_MAXNCH_P];
   char *ppp;
   token tok;
   rwtype rw;
   /*-------------------------------------------------------*/
   if (inputfile==stdin){
      error_this_pos(E_NOTBOOLITEM);
      *rwans=RW_NULL;
      return(TK_COMMA);}
   get_token_after_matching_paren(id);
   rw=NOT_RESERVED_WORD;
   if (isalpha(*id)){
      for (ppp=id;((*ppp)!='\0')&&(isalnum((*ppp)));++ppp); *ppp='\0';
      interpret_alpha_token(id,&tok,&rw);
      if (tok==TK_RW) tok=give_rw_operation_token(rw);}
   else if (isdigit(*id))
      tok=TK_REAL;
   else{
      strcpy(idx,id); idx[2]='\0';
      tok=lookup_operator_token(idx);
      if (tok==TK_NULL){
         idx[1]='\0'; tok=lookup_operator_token(idx);}}
   /*-------------------------------------------------------*/
   *rwans=rw;
   return(tok);
}
/*===========================================================================*/
Boolean advance_one_expansion_token
   (macro_expansion_info_type *macinf,Boolean *popit_ptr,
    Boolean *was_implicit_loner_token,Boolean substitute)
{
   short nowknt,ovfknt,nowix;
   Boolean repeat_attempt,popit,xpnbug,already,many,putnew;
   token_info_type *tokinf;
   /*-------------------------------------------------------*/
   xpnbug = (Boolean) ((substitute) && (bitset(debug_flagword,DBG_XPN)));
   putnew=already=repeat_attempt=popit=FALSE;
   nowix=(*macinf).now_passed_ix;
   *was_implicit_loner_token=FALSE;
   if (nowix>=0){
      /*--------------------------------------- next token of passed parm */
      nowknt=(*macinf).now_passed_count;
      ovfknt=(*macinf).passed_token_counts[nowix];
      if ((nowknt>0)&&(nowknt<ovfknt)){
         tokinf = (*macinf).passed_token_list + 
                  ((Memix) *((*macinf).passed_token_offset +
                             ((Memix) nowix))) +
                  ((Memix) (nowknt-1));
         if ((is_an_implicit_function((*tokinf).tok,(*tokinf).id_info_ptr)) &&
             (((*(*tokinf).id_info_ptr).ptr.parameter_count)<=0))
             *was_implicit_loner_token=TRUE;}
      if (nowknt<ovfknt){
         /*--------------------------  fetch next */
         (*macinf).now_passed_count =
               fetch_next_passed_token(macinf,nowix,nowknt);}
      else{
         /*--------------------------  no more to fetch */
         (*macinf).now_passed_count=(-1);
         (*macinf).now_passed_ix=(-1);
         if (ovfknt>1){
            strcpy(this.id,STRRP);
            this.tok=TK_RP; this.rw=NOT_RESERVED_WORD;}
         else repeat_attempt=TRUE;}}
   else if ((*macinf).now_body_ix<0){
      /*--------------------------------------- insert OPENPAREN */
      repeat_attempt=TRUE;
      (*macinf).now_body_ix = (-1-((*macinf).now_body_ix));}
   else if ((*macinf).now_body_ix<(*macinf).ovf_body_ix){
      /*--------------------------------------- fetch next body token */
      many = (Boolean) ((*macinf).ovf_body_ix > 2);
      if ((*macinf).now_body_ix>0){
         tokinf = function_body_storage + (*macinf).now_body_ix - 1;
         if ((is_an_implicit_function((*tokinf).tok,(*tokinf).id_info_ptr)) &&
             (((*(*tokinf).id_info_ptr).ptr.parameter_count)<=0))
             *was_implicit_loner_token=TRUE;}
      if (xpnbug && many){
         if (is_at_first_body_token(macinf)) putnew=TRUE;
         else if (is_at_last_body_token(macinf)){
             echo_inserted_newline_indent(-2);}}
      (*macinf).now_body_ix =
         fetch_next_body_token(macinf,(*macinf).now_body_ix);}
   else{
      /*--------------------------------------- pop */
      repeat_attempt=popit=TRUE;}
   if ((this.tok==TK_MACPARM)&&(!repeat_attempt)&&(substitute)){
      /*--------------------------------------- substitute macro parameter */
#ifdef BUGGY
if ((*this.id)!='$'){
fprintf(stderr,"*** ERR, not really a macro parm: %s\n",this.id);
abort_program();}
#endif
      (*macinf).now_passed_ix=give_short(atol(this.id+1),E_INTERNAL);
      (*macinf).now_passed_ix--;
      (*macinf).now_passed_count=0;
      if ((*macinf).passed_token_counts[(*macinf).now_passed_ix]>1){
         strcpy(this.id,STRLP); this.tok=TK_LP; this.rw=NOT_RESERVED_WORD;}
      else{
         repeat_attempt=TRUE;}}
   if ((xpnbug) && (!already) && (!repeat_attempt) && (substitute)){
      echo_inserted_token_to_listing(this.id);}
   if (putnew) echo_inserted_newline_indent(2);
   /*-------------------------------------------------------*/
   *popit_ptr=popit;
   return(repeat_attempt);
}
/*===========================================================================*/
void advance_one_token()
/*
   This function advances one token.   The token will come from off of the
   macro expansion stack "macro_stack" unless the stack is empty.  If the
   stack is empty, then the token will come from the input stream.

   Organization of the macro stack:

       The macro stack consists of a stack of macro expansion information 
       records.   Each macro expansion information record contains an
       array of lists of calling expression tokens as well as a pointer
       to the list of tokens making up the body of the macro.

   For example:
       FUNCTION F(X) = X * (X-2.0*X);
       FUNCTION G(A,B,C) = A * (B - C);
       ....
       + G(F(2.0+QQQ),F(2.0-QQQ),F((Q1+Q2)/2.0))

       during parse of F(2.0+QQQ):

              top-->  ( body--> ($1*($1-2.0*$1))  ;  $1--> 2.0+QQQ )
                      ( body--> ($1*($2-$3))      ;  $1--> F(2.0+QQQ),
                                                     $2--> F(2.0-QQQ),
                                                     $3--> F((Q1+Q2)/2.0) )

   When a parameter reference, such as $1, is encountered, the "now_passed_ix"
   is changed from negative one to the index of the parameter (0 for $1, 1 for
   $2, 2 for $3, etc.).   When all tokens for the parameter have been
   exhaused, then the index is changed back to negative one and the next token
   is taken from the body once again.   When all tokens for the body have
   been exhausted, then the stack is popped.

   Parentheses are inserted when the count of the number of tokens making up
   a calling parameter is greater than one.   For example, the following
   translations will be performed:

          F(MU)    ----->   MU*(MU-2.0)        (no parens added since MU
                                                is only one token long)
          F(A+B)   ----->   (A+B)*((A+B)-2.0)  (parens added since A+B is
                                                three tokens (more than one)
                                                long)
    
*/
{
   macro_expansion_info_type *macinf;
   Boolean repeat,popit,was_implicit_loner_token;
   /*-------------------------------------------------------*/
   if (gotten){
      last_gotten_token=this.tok;
      last_gotten_idinfo=this.id_info_ptr;}
   this_looked_up=gotten=FALSE;
   was_implicit_loner_token=FALSE;
   popit=FALSE;
   sys_memcpy((char *) &ago,(char *) &prev,sizeof(token_info_type));
   sys_memcpy((char *) &prev,(char *) &this,sizeof(token_info_type));
   repeat=(Boolean) (macro_stack_top>=0);
   while (repeat){
      macinf = macro_stack + ((Memix) macro_stack_top);
      repeat=advance_one_expansion_token
                (macinf,&popit,&was_implicit_loner_token,TRUE);
      if (popit) pop_macro_body();
      if (!repeat) goto done_point;
      repeat=(Boolean) (macro_stack_top>=0);}
   if ((is_an_implicit_function(last_gotten_token,last_gotten_idinfo)) &&
       (((*last_gotten_idinfo).ptr.parameter_count)<=0))
      was_implicit_loner_token=TRUE;
   gotten=TRUE;
   this.tok=gettoken(&this.rw,&this.id[0],&this.linnum,&this.pos);
   this.id_info_ptr=(identifier_info_type *) NULL;
   /*-------------------------------------------------------*/
done_point:
   /*-----------------------------------------  error if left paren */
   if ((was_implicit_loner_token) && (this.tok==TK_LP)){
      error_this_pos(E_NOCALLINGALLOWED);
      skip_nested_when_skipping(TK_RP,TRUE);}
   return;
}
/*===========================================================================*/
void insert_token_info_as_this
     (token_info_type *tokinf,short use_pos,short use_linnum)
/*
   This function inserts a new token into the input stream.  It is used
   primarily for macro/implicit/regular function expansions.  If the token
   is an identifier, then it is not looked up in the table.  The position and
   line number are set to "use_pos" and "use_linnum" so that all errors will
   point to the source line macro/implicit/regular token which is currently
   being expanded.
*/
{
   /*-------------------------------------------------------*/
   sys_memcpy((char *) &this,(char *) tokinf,sizeof(token_info_type));
   this.id_info_ptr=(identifier_info_type *) NULL;
   this.pos=use_pos;
   this.linnum=use_linnum;
   this_looked_up=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void push_macro_body(macro_expansion_info_type *macinf,char *kind)
/*
   This function pushes a macro expansion reference onto the macro expansion
   stack.  When "DEBUG$ PARSE$;" or "DEBUG$ LEX$;" is set, it also prints
   all of the tokens for all of the calling parameters as well as all of 
   the tokens for the body of the function.   When in said debugging mode(s),
   the tokens are printed on the "bugfile", which is:
            stderr (video screen) if "DEBUG$ EXPAND$;" bit set.
            listfile (file.alog)  if "DEBUG$ EXPAND$;" bit not set.
*/
{
   token_info_type *nowinfo;
   short ixparm,ixbody,nix;
   unsigned short off,noff;
   char tag[50];
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_XPN)) echo_inserted_newarrow(BODY_INDENT);
   /*-----------------------------------------  push macro expansion info */
   ++macro_stack_top;
   if (((Memix) macro_stack_top)>=max_nest_count){
      error_this_pos(E_FUNCNESTOVF);
      abort_program();}
   sys_memcpy((char *) (macro_stack+((Memix) macro_stack_top)),(char *) macinf,
              sizeof(macro_expansion_info_type));
   /*-----------------------------------------  debug dump */
   if (bitset(debug_flagword,DBG_LEX|DBG_PRS)){
      for (ixparm=0,nix=(*macinf).passed_parameter_count;ixparm<nix;++ixparm){
         sprintf(tag,"  <EXPANSION.OF.%s.$%d>: ",kind,(int) (1+ixparm));
         off = *((*macinf).passed_token_offset + ((Memix) ixparm));
         noff = off + *((*macinf).passed_token_counts + ((Memix) ixparm));
         for (;off<noff;++off){
            nowinfo=(*macinf).passed_token_list+off;
            fprintf(bugfile,tag);
            showtoken((*nowinfo).tok,(*nowinfo).rw,(*nowinfo).id);}}
      nix=(*macinf).ovf_body_ix;
      for (ixbody=(-1-(*macinf).now_body_ix);ixbody<nix;++ixbody){
         nowinfo = function_body_storage + ((Memix) ixbody);
         fprintf(bugfile,"<EXPANSION.OF.%s.BODY>: ",kind);
         showtoken((*nowinfo).tok,(*nowinfo).rw,(*nowinfo).id);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void pop_macro_body()
/*
   This function pops a level off of the macro expansion stack.  It is called
   after all tokens for all calling parameters and all tokens for the body of
   the invoked macro have been exhausted.
*/
{
   macro_expansion_info_type *macinf;
   /*-------------------------------------------------------*/
   macinf = macro_stack + ((Memix) macro_stack_top);
   --macro_stack_top;
   counts_offset_storage_count = 
      (*macinf).passed_token_offset - offset_storage;
   function_body_count = (*macinf).passed_token_list - function_body_storage;
   if (bitset(debug_flagword,DBG_XPN)){
      if (macro_stack_top<0) echo_inserted_newline_final(-BODY_INDENT);
      else{
         --macinf;
         if (is_at_last_body_token(macinf))
            set_echo_inserted_indent(-BODY_INDENT);
         else echo_inserted_newline_indent(-BODY_INDENT);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
short save_one_calling_token(token_info_type *tokinf,Boolean xpnbug)
/*
   This function saves one calling token into function body storage.  It is
   used when parsing calling parameter lists for macro invocations.
*/
{
   /*-------------------------------------------------------*/
   save_function_body_token(tokinf);
   if (xpnbug) echo_inserted_token_to_listing(this.id);
   advance_one_token();
   /*-------------------------------------------------------*/
   return(1);
}
/*===========================================================================*/
short save_nested_calling_tokens(token until_tok,Boolean xpnbug)
/*
   This function saves nested calling tokens into function body storage.  It
   is used when a calling parameter to a function has nested parentheses
   (as in expressions enclosed in parentheses) or nested square brackets (as
   in array subscripts).
*/
{
   short knt;
   /*-------------------------------------------------------*/
   knt = save_one_calling_token(&this,xpnbug);
   while (this_is_not(until_tok)){
     if (this.tok==TK_LP) knt += save_nested_calling_tokens(TK_RP,xpnbug);
     else if (this.tok==TK_LB) knt += save_nested_calling_tokens(TK_RB,xpnbug);
     knt += save_one_calling_token(&this,xpnbug);}
   /*-------------------------------------------------------*/
   return(knt);
}
/*===========================================================================*/
short save_tokens_for_single_calling(token until_tok,Boolean xpnbug)
/*
   This function saves the tokens for a single calling parameter to a function.
   The tokens are saved into function body storage.
*/
{
   short knt;
   /*-------------------------------------------------------*/
   knt=0;
   while (this_is_not_param_terminator(until_tok)){
     if (this.tok==TK_LP) knt += save_nested_calling_tokens(TK_RP,xpnbug);
     else if (this.tok==TK_LB) knt += save_nested_calling_tokens(TK_RB,xpnbug);
     knt += save_one_calling_token(&this,xpnbug);}
   /*-------------------------------------------------------*/
   return(knt);
}
/*===========================================================================*/
void fill_in_macinf(short parmcount,macro_expansion_info_type *macinf)
{
   Memix ix;
   /*-------------------------------------------------------*/
   (*macinf).now_passed_ix=(-1);
   (*macinf).now_passed_count=(-1);
   (*macinf).passed_parameter_count = parmcount;
   (*macinf).ini_body_ix=((*this.id_info_ptr).index.body.lower);
   (*macinf).now_body_ix=(-1-((*this.id_info_ptr).index.body.lower));
   (*macinf).ovf_body_ix=(*this.id_info_ptr).index.body.upper;
   (*macinf).passed_token_offset =
             offset_storage + counts_offset_storage_count;
   (*macinf).passed_token_counts =
             counts_storage + counts_offset_storage_count;
   (*macinf).passed_token_list = function_body_storage + function_body_count;
   counts_offset_storage_count += ((Memix) parmcount);
   for (ix=0;ix<((Memix) parmcount);++ix)
             (*macinf).passed_token_offset[ix] =
             (*macinf).passed_token_counts[ix] = 0;
   (*macinf).pos=this.pos;
   (*macinf).linnum=this.linnum;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void expand_implicit()
/*
   This function parses the implicit function invocation.   Upon entry, the
   current token must be the identifier name of the implicit function being
   invoked.   Upon exit, the current token will be the first token of the
   expanded implicit body.
*/
{
   Boolean xpnbug;
   short parmcount;
   macro_expansion_info_type macinf;
   token_info_type thisfunc;
   char tmpid[IDENT_MAXNCH_P];
   /*-------------------------------------------------------*/
   xpnbug = (Boolean)
               ((macro_stack_top < 0) && (bitset(debug_flagword,DBG_XPN)));
   if (bitset(debug_flagword,DBG_PRS)&&bitset(debug_flagword,DBG_ROU))
      fprintf(bugfile,"::: expand_implicit()\n");
   /*-----------------------------------------  lookup identifier */
   this.id_info_ptr=searchid_old(this.id,UNKNOWN_TYPE,this.pos);
   sys_memcpy((char *) &thisfunc,(char *) &this,sizeof(token_info_type));
   /*-----------------------------------------  check for parameter version */
   if ((*this.id_info_ptr).ptr.parameter_count>0){
#ifdef OBSOLETE
      expand_macro("IMPLICIT",TK_LB,TK_RB,E_NOTOPENBRACKET,E_NOTCLOSEBRACKET);
#else
      expand_macro("IMPLICIT",TK_LP,TK_RP,E_NOTOPENPAREN,E_NOTCLOSEPAREN);
#endif
      goto done_point;}
   /*-----------------------------------------  check next token */
   get_lookahead_token(tmpid);
   if (strcmp(tmpid,STRLP)==0)
      show_err_at_pos_on_both(E_NOCALLINGALLOWED,
                              this.pos+strlen(this.id),this.id,CHAR_TYPE);
   /*-----------------------------------------  fill in macro information */
   parmcount=0;
   fill_in_macinf(parmcount,&macinf);
   if (xpnbug){
      echo_inserted_force_newline();
      echo_inserted_token_to_listing(this.id);}
   /*-----------------------------------------  push function invocation */
   push_macro_body(&macinf,"IMPLICIT");
   advance_one_token();
   /*-------------------------------------------------------*/
done_point:
   return;
}
/*===========================================================================*/
void expand_regular()
{
   /*-------------------------------------------------------*/
   expand_macro("FUNCTION",TK_LP,TK_RP,E_NOTOPENPAREN,E_NOTCLOSEPAREN);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void expand_macro
      (char *tag,token opentoken,token closetoken,short eeopen,short eeclose)
/*
   This function parses the regular function/implicit invocation with formal
   parameter.  Upon entry, the current token must be the identifier name of
   the regular function being invoked.   Upon exit, the current token will
   be the first token of the expanded regular body.
*/
{
   short parmcount,expect;
   Memix ix;
   unsigned short off,knt;
   macro_expansion_info_type macinf;
   Boolean first_time,xpnbug;
   token_info_type thisfunc;
   char tagmsg[IDENT_MAXNCH_P+12];
   /*-------------------------------------------------------*/
   xpnbug = (Boolean)
               ((macro_stack_top < 0) && (bitset(debug_flagword,DBG_XPN)));
   if (bitset(debug_flagword,DBG_PRS)&&bitset(debug_flagword,DBG_ROU))
      fprintf(bugfile,"::: expand_macro(%s)\n",tag);
   /*-----------------------------------------  compute parameter count */
   expect = parmcount = (*this.id_info_ptr).ptr.parameter_count;
   if (((Memix) macro_stack_top)>=(max_nest_count-1)) parmcount=0;
   /*-----------------------------------------  lookup identifier */
   this.id_info_ptr=searchid_old(this.id,UNKNOWN_TYPE,this.pos);
   sys_memcpy((char *) &thisfunc,(char *) &this,sizeof(token_info_type));
   sprintf(tagmsg,"(%s %s)",tag,this.id);
   /*-----------------------------------------  fill in macro information */
   fill_in_macinf(parmcount,&macinf);
   if (xpnbug){
      echo_inserted_force_newline();
      echo_inserted_token_to_listing(this.id);}
   /*-----------------------------------------  left paren or bracket */
   advance_one_token();
   if (this.tok==opentoken){
      if (xpnbug) echo_inserted_token_to_listing(this.id);
      advance_one_token();}
   else{
      tail_error_this_pos(eeopen,tagmsg);
      macinf.passed_parameter_count = 0;
      goto skip_pos;}
   /*-----------------------------------------  parse calling parameters */
   for (off=0,ix=0;(ix<((Memix) expect))&&
                   (this_is_not(closetoken));++ix){
      if (ix>0){
         if (xpnbug) echo_inserted_token_to_listing(this.id);
         if (this.tok==TK_COMMA) advance_one_token();
         else tail_error_this_pos(E_NOTCOMMA,tagmsg);}
      if (ix<((Memix) parmcount)){
         knt=save_tokens_for_single_calling(closetoken,xpnbug);
         macinf.passed_token_offset[ix]=off;
         macinf.passed_token_counts[ix]=knt;
#ifdef BUGGY
fprintf(stderr,"ix=%d, off=%d, knt=%d\n",(int) ix,(int) off,(int) knt);
#endif
         off += knt;}
      else skip_to_next_comma_or(FALSE,closetoken);}
   /*-----------------------------------------  error if too few */
   if (ix<((Memix) expect)){
      tail_error_this_pos(E_TOOFEWCALLING,tagmsg);}
   /*-----------------------------------------  error if too many */
   for (first_time=TRUE;this_is_not(closetoken);first_time=FALSE,++ix){
      if (first_time) tail_error_this_pos(E_TOOMANYCALLING,tagmsg);
      if (ix>0){
         if (this.tok==TK_COMMA) advance_one_token();
         else tail_error_this_pos(E_NOTCOMMA,tagmsg);}
      skip_to_next_comma_or(FALSE,closetoken);}
   /*-----------------------------------------  right paren or bracket */
   if (this.tok!=closetoken) tail_error_this_pos(eeclose,tagmsg);
   else if (xpnbug) echo_inserted_token_to_listing(this.id);
   /*-----------------------------------------  push function invocation */
skip_pos:
   push_macro_body(&macinf,tag);
   advance_one_token();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
short parse_macro_parmlist(token opentoken,token closetoken,short eeopen,
                           any_searchid_routine sid_parm)
/*
   This function parses a macro parameter list.   Upon entry, the current
   token must be the opening token.   Upon exit without errors, the current
   token will be the token past the matching closing token.
*/
{
   short parmcount;
   /*-------------------------------------------------------*/
   parmcount=0;
   if (this.tok==opentoken){
      advance_one_token();
      while (this_is_not(closetoken)){
         if (parmcount>0){
            if (this.tok==TK_COMMA) advance_one_token();
            else if (this.tok==TK_ID)
               show_err_at_pos_on_both(E_MISSINGTOKEN,this.pos,",",CHAR_TYPE);}
         if (this.tok==TK_ID){
            this.id_info_ptr=sid_parm(this.id,UNKNOWN_TYPE,this.pos);
            (*this.id_info_ptr).scope_level=SCOPE_DUMMY_PARAM_0+parmcount;
            if (parmcount==((short) max_parm_count))
               error_this_pos(E_PARMCOUNTOVF);
            ++parmcount;}
         else error_this_pos(E_NOTIDENT);
         advance_one_token();}
      if (this.tok==closetoken) advance_one_token();}
   else{
      error_this_pos(eeopen);}
   /*-------------------------------------------------------*/
   if (parmcount>max_parm_count) parmcount=max_parm_count;
   return(parmcount);
}
/*===========================================================================*/
short parse_mac_parmlst_pushing_scope(char *word,Boolean implicit,Boolean implicit_dummy,
                                      any_searchid_routine sid_parm,
                                      token_info_type *nameinfo,
                                      Subscript *old_count)
/*
   This function parses the portion of a macro/variable definition up to
   and including the equal sign which precedes the expression.   Upon entry,
   the current token must be the keyword for the declaration.   Upon exit,
   the current token will be the token following the semicolon which terminated
   the expression.
*/
{
   short parmcount;
   /*-------------------------------------------------------*/
   preset_searchid_abort_if_exists();
   /*-----------------------------------  skip keyword "FUNCTION"/"IMPLICIT" */
   advance_one_token();
   /*-----------------------------------  expect new function name */
   *old_count=give_Subscript(function_body_count,E_INTERNAL);
   if (this.tok==TK_ID)
      sys_memcpy(nameinfo,&this,sizeof(token_info_type));
   else{
      error_this_pos(E_NOTIDENT);
      strcpy((*nameinfo).id,"!!");
      safe_strcat_truncated((*nameinfo).id,word,IDENT_MAXNCH);
      (*nameinfo).tok=TK_ID;}
#ifdef BUGGY
fprintf(bugfile,"nameinfo=(%s)\n",(*nameinfo).id);
#endif
   advance_one_token();
   /*-----------------------------------  now inside function definition */
   push_ident_scope();
   /*-----------------------------------  parse parameter list(s) */
   if (implicit){
      parmcount=parse_macro_parmlist(TK_LB,TK_RB,E_NOTOPENBRACKET,sid_parm);
      parmcount=0;
#ifdef OBSOLETE
      if (this.tok==TK_LB) parmcount =
         parse_macro_parmlist(TK_LB,TK_RB,E_NOTOPENBRACKET,searchid_dummy);
#else
      if (this.tok==TK_LP){
         if (!implicit_dummy) error_this_pos(E_VARDUMMY);
         parmcount =
             parse_macro_parmlist(TK_LP,TK_RP,E_NOTOPENPAREN,searchid_dummy);}
#endif
   }
   else
      parmcount=parse_macro_parmlist(TK_LP,TK_RP,E_NOTOPENPAREN,sid_parm);
   /*-----------------------------------  parse = */
   if (this.tok==TK_EQ) advance_one_token();
   else error_this_pos(E_NOTEQUALSIGN);
#ifdef BUGGY
fprintf(bugfile,"nameinfo=(%s), debug_flagword=%x\n",
nameinfo.id,(int) debug_flagword);
#endif
   /*-------------------------------------------------------*/
   return(parmcount);
}
/*===========================================================================*/
void parse_macro_def(char *which,char *word,short eebad,Boolean implicit,
                     type_flagword_type whattype,
                     any_searchid_routine sid_parm,
                     any_searchid_routine sid_body)
/*
   This function parses the regular/implicit/macro function definition
   statement beginning with the keyword "FUNCTION" or "IMPLICIT". 
   Upon entry, the current token must be the keyword itself.   Upon exit,
   the current token will be the token past the  semicolon which terminated
   the definition.
*/
{
   identifier_info_type *funcptr;
   token_info_type *nowinfo;
   token_info_type tokinfo;
   short parmcount,ixp;
   int parmnum;
   token_info_type nameinfo;
   Subscript old_count,new_count;
   type_flagword_type fff;
   /*-------------------------------------------------------*/
   if (bitset(debug_flagword,DBG_PRS)&&bitset(debug_flagword,DBG_ROU))
      fprintf(bugfile,"::: %s()\n",which);
#ifdef BUGGY
fprintf(stderr,"sizeof(macro_expansion_info_type)=%d\n",
sizeof(macro_expansion_info_type));
#endif
   parmcount =
       parse_mac_parmlst_pushing_scope(word,implicit,TRUE,sid_parm,
                                       &nameinfo,&old_count);
   /*-----------------------------------  parse body */
   sys_memcpy((char *) &tokinfo,(char *) &this,sizeof(token_info_type));
   strcpy(tokinfo.id,STRLP); tokinfo.tok=TK_LP; tokinfo.rw=NOT_RESERVED_WORD;
   save_function_body_token(&tokinfo);
   while (this_is_not_semi()){
      if (is_value_token(this.tok)){
         if (is_literal_token(this.tok))
            this.id_info_ptr=searchid_silent(this.id,INT_TYPE,this.pos,FALSE);
         else
            this.id_info_ptr=sid_body(this.id,INT_TYPE,this.pos);
         if (this.tok==TK_ID){
            fff=(*this.id_info_ptr).flags;
            if (implicit){
               if (bitset(fff,SSVAR_TYPE)){
                  if ((*this.id_info_ptr).scope_level<SCOPE_DUMMY_PARAM_0){
                     error_this_pos(E_SSVNOTLISTED);}}
               if ((fff & IMPLICIT_TYPE)==IMPLICIT_TYPE){
                  for (ixp=(*this.id_info_ptr).index.body.lower;
                       ixp<(*this.id_info_ptr).index.body.upper;
                       ++ixp){
                     nowinfo=function_body_storage+((Memix) ixp);
                     if (bitset(debug_flagword,DBG_PRS))
                         fprintf(stderr,"\"%s\"\n",(*nowinfo).id);
                     if ((is_value_token((*nowinfo).tok))&&
                         bitset((*(*nowinfo).id_info_ptr).flags,SSVAR_TYPE)){
                         if ((*(*nowinfo).id_info_ptr).scope_level <
                             SCOPE_DUMMY_PARAM_0){
                             show_err_at_pos_on_both(E_SSVNOTLISTED,this.pos,
                                                     (*nowinfo).id,
                                                     CHAR_TYPE);}}}}}
            if (!((implicit)&&
                  (bitset((*this.id_info_ptr).flags,SSVAR_TYPE)))){
               if ((*this.id_info_ptr).scope_level>=SCOPE_DUMMY_PARAM_0){
                  parmnum = (*this.id_info_ptr).scope_level -
                            SCOPE_DUMMY_PARAM_0 + 1;
                  sprintf(this.id,"$%d",parmnum);
                  this.tok=TK_MACPARM;
                  this.id_info_ptr =
                       searchid_silent(this.id,UNKNOWN_TYPE,this.pos,FALSE);}
               else if ((bitset((*this.id_info_ptr).flags,FUNCTION_TYPE))&&
                        (!bitset((*this.id_info_ptr).flags,IMPLICIT_BIT)));
               else if (bitset((*this.id_info_ptr).flags,
                               ANY_SPECIAL_VAR_TYPE_MASK))
                   error_this_pos(eebad);}}}
      save_function_body_token(&this);
      advance_one_token();}
   sys_memcpy((char *) &tokinfo,(char *) &this,sizeof(token_info_type));
   strcpy(tokinfo.id,STRRP); tokinfo.tok=TK_RP; tokinfo.rw=NOT_RESERVED_WORD;
   save_function_body_token(&tokinfo);
   /*-----------------------------------  fill in miscellaneous info */
   new_count = give_Subscript(function_body_count,E_INTERNAL);
   /*-----------------------------------  no longer inside function def. */
   pop_ident_scope();
   /*-----------------------------------  declare ident after the fact */
#ifdef BUGGY
fprintf(bugfile,"nameinfo=(%s,(",nameinfo.id);
if (nameinfo.id_info_ptr != NULL)
bug_print_idinfo(nameinfo.id_info_ptr,
12+strlen(nameinfo.id),13+strlen(nameinfo.id));
fprintf(bugfile,"))\n");
#endif
   set_searchid_abort_if_exists(E_RECURSIVE);
   funcptr=searchid_dcl(nameinfo.id,whattype,nameinfo.pos);
   reset_searchid_abort_if_exists();
   (*funcptr).index.body.lower = old_count;
   (*funcptr).index.body.upper = new_count;
   (*funcptr).ptr.parameter_count = parmcount;
   /*-----------------------------------  show tokens in definition */
   if (bitset(debug_flagword,DBG_LEX|DBG_PRS)){
      for (parmcount=(*funcptr).index.body.lower;
           parmcount<(*funcptr).index.body.upper;
           ++parmcount){
          nowinfo=function_body_storage+((Memix) parmcount);
          fprintf(bugfile,"<DEFINITION.OF.%s.BODY>: ",word);
          showtoken((*nowinfo).tok,(*nowinfo).rw,(*nowinfo).id);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_regular_def()
/*
   This function parses the regular function definition statement beginning
   with the keyword "FUNCTION".   Upon entry, the current token must be the
   keyword itself.  Upon exit, the current token will be the token past the
   semicolon which terminated the definition.
*/
{
   /*-------------------------------------------------------*/
   parse_macro_def("parse_regular_def","REGULAR",E_BADREGTOKEN,FALSE,
                   FUNCTION_TYPE,searchid_dummy,searchid_old);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void parse_implicit_def()
/*
  This function parses the implicit function definition statement beginning
   with the keyword "IMPLICIT".   Upon entry, the current token must be the
   keyword itself.  Upon exit, the current token will be the token past the
   semicolon which terminated the definition.
*/
{
   /*-------------------------------------------------------*/
   if (this_tok_is_rw(RW_INTRINSIC))
      show_err_at_pos_on_both(E_KEYWCONTEXT,this.pos,
                              "DID YOU MEAN TO SAY \"IMPLICIT\"?",CHAR_TYPE);
   parse_macro_def("parse_implicit_def","IMPLICIT",0,TRUE,
                   IMPLICIT_TYPE,searchid_oldssv,searchid_oldssvconst);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
