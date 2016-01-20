/*
===============================================================================

        Lexical token scanning functions for ASSIST and SURE languages.

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
#include "rwdefs.h"
#include "errdefs.h"

#include "sys_ext.h"
#include "str_ext.h"
#include "lio_ext.h"
#include "pio_ext.h"
#include "lex_ext.h"
#include "user_ext.h"
#ifdef BUGGY
extern Boolean fatal_error;
#endif

/*===========================================================================*/

                /***************************************/
                /***************************************/
                /****                               ****/
                /****   LOW-LEVEL PARSING SECTION   ****/
                /****                               ****/
                /***************************************/
                /***************************************/

#include "lextabsP.h"
/**/
static char *allocated_memory;
/**/
static value_union_type tmpval;
/**/
static Boolean declare_at_top;
static Boolean is_first_token_on_line;
static Boolean searchid_abort_if_exists;
static short ee_abort_if_exists;
static Memix nele_ident_abort_if_exists;
static reserved_word_lookup_type *user_reserved_word_lookup_table;
static Memix max_num_count = 200;
static Memix max_idn_count = 400;
static Memix max_identifier_table_count = 600;/* max_num_count+max_idn_count */
/*===========================================================================*/
#define pre_define_log(name,val) \
        xref_setvalue(name); \
        ppp=searchid_dcl(name,BOOL_TYPE,0); \
        tmpval.bbb = (Boolean) val; \
        declare_computational_ident(ppp,BOOL_CONST_TYPE,&tmpval)
#ifdef FUTURE_MAYBE
#define pre_define_int(name,val) \
        ppp=searchid_dcl(name,INT_TYPE,0); \
        tmpval.iii = (int_type) val; \
        declare_computational_ident(ppp,INT_CONST_TYPE,&tmpval)
#define pre_define_real(name,val) \
        ppp=searchid_dcl(name,REAL_TYPE,0); \
        tmpval.rrr = (real_type) val; \
        declare_computational_ident(ppp,REAL_CONST_TYPE,&tmpval)
#endif
/*===========================================================================*/
void init_lexlib(reserved_word_lookup_type *lookup_table)
/*
   This function initializes variables needed by the lex scanner library.
   Call this routine first.
*/
{
   char *ppp;
   int_type value;
   /*-------------------------------------------------------*/
   user_reserved_word_lookup_table = lookup_table;
   value = MALLOC_MAX;
   ppp=enk_aux_val((void *) &value,INT_TYPE);
   sprintf(os_malloc_limit_msg,
           "BYTES. OPERATING SYSTEM MALLOC LIMIT=%s",ppp);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void re_init_lexlib()
/*
   This function initializes quantities for lexical scanning of another
   input file.
*/
{
   /*-------------------------------------------------------*/
   reset_searchid_abort_if_exists();
   nesting_scope_level = 0;
   /*-----------------------------  define pre-defined constants */
   declare_at_top=FALSE;
   nele_constant_table=0;
   nele_ident_at_table_top=nele_identifier_table=0;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Memix set_idn_num_counts(Memix idn_count,Memix num_count)
{
   /*-------------------------------------------------------*/
   max_idn_count = idn_count;
   max_num_count = num_count;
   max_identifier_table_count = max_num_count + max_idn_count;
   /*-------------------------------------------------------*/
   return (max_identifier_table_count);
}
/*===========================================================================*/
void lookup_as_symbolic_token(char *id)
{
   /*-------------------------------------------------------*/
   *(id+2)='\0';
   if (lookup_operator_token(id)==TK_NULL) *(id+1)='\0';
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
token lookup_operator_token(char *id)
/*
   This function looks up the operator in the table to determine which token
   it is.
*/
{
   token_lookup_type *ppp;
   /*-------------------------------------------------------*/
   for (ppp=token_lookup_table; (((*ppp).token[0])!='\0'); ++ppp)
      if (strcmp(id,(*ppp).token)==0) return((*ppp).value);
   /*-------------------------------------------------------*/
   return(TK_NULL);
}
/*===========================================================================*/
token give_rw_operation_token(rwtype rw)
/*
   This function decodes a reserved word token, translating it to an operation
   token if appropriate.   If the reserved word type is not the name of an
   operation (such as NOT, AND, OR), then TK_RW is returned.   This function
   should only be called when the token has already been pre-diagnosed as
   a TK_RW token.
*/
{
   rw_operator_lookup_type *rw_lookup_entry;
   /*-------------------------------------------------------*/
   for (rw_lookup_entry=rw_operator_lookup_table;
        (*rw_lookup_entry).rwsrc != NOT_RESERVED_WORD;
        ++rw_lookup_entry)
      if (rw==((*rw_lookup_entry).rwsrc))
        return((*rw_lookup_entry).tokdest);
   /*-------------------------------------------------------*/
   return(TK_RW);
}
/*===========================================================================*/
int_type decode_expected_integer(char *id,token tok,int_type err_dflt)
{
   int_type ans;
   /*-------------------------------------------------------*/
   if (tok==TK_INT)
      ans=decode_integer(id);
   else{
fprintf(stderr,"DEI\n");
      show_err_on_both(E_NOTINTEGER,(void *) id,CHAR_TYPE);
      ans=err_dflt;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
real_type decode_expected_real(char *id,token tok,real_type err_dflt)
{
   real_type ans;
   /*-------------------------------------------------------*/
   if (tok==TK_INT)
      ans=(real_type) decode_integer(id);
   else if (tok==TK_REAL)
      ans=decode_real(id);
   else{
      show_err_on_both(E_NOTREAL,(void *) id,CHAR_TYPE);
      ans=err_dflt;}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void decode_number(token tk,          /* token type (TK_INT or TK_REAL) */
                   char *id,          /* text of value to decode */
                   int_type *ival,    /* returned iff tk==TK_INT */
                   real_type *val)    /* returned iff tk==TK_REAL */
/*
   This function decodes the numeric value, be it an integer or be it
   a real.   The first character is assumed to not be part of the number.
*/
{
   /*-------------------------------------------------------*/
   if (tk==TK_INT) *ival=decode_integer(id);
   else if (tk==TK_REAL) *val=decode_real(id);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
rwtype which_reserved_word
     (char *id,          /* identifier to check if reserved word */
      Boolean sensitive) /* TRUE during non-expression parsing */
/*
   This function determines which reserved word corresponds to "id".  Zero
   is returned if "id" is not a reserved word.
*/
{
   reserved_word_lookup_type *ppp;
   /*--------------------------------------- names of commands first */
   ppp=user_reserved_word_lookup_table;
   for (;(*(*ppp).text)!='\0';++ppp){
      if ((!sensitive)&&((*(*ppp).text)=='\n')) return(NOT_RESERVED_WORD);
      if (strcmp(id,(*ppp).text)==0) return((*ppp).rw);}
   /*-------------------------------------------------------*/
   return(NOT_RESERVED_WORD);
}
/*===========================================================================*/
token load_char_string_token(FILE *fp,char *id,char *buff,char quoting_char)
/*
   This function loads the buffer with the text of the quoted character
   string.   It works for any quoting character, be it a quote or be it an
   apostrophe.
*/
{
   token answer;
   /*-------------------------------------------------------*/
   *id =  *(id+1) = quoting_char;
   *(id+2) = '\0';
   answer=TK_CHAR;
   advance_1_char();
   get_literal_text(fp,buff,quoting_char);
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
void interpret_alpha_token(char *id,token *tok,rwtype *rw)
{
   rwtype rwtemp;
   /*-------------------------------------------------------*/
   if ((rwtemp=which_reserved_word(id,TRUE))==NOT_RESERVED_WORD)
      *tok=TK_ID;
   else
      *tok=TK_RW;
   *rw=rwtemp;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
token gettoken_only(char *buff,        /* output iff literal character text */
                    char *id,          /* output always */
                    rwtype *rw,        /* output iff TK_RW returned */
                    short *linnum,     /* output always */
                    short *pos)        /* output always */
/*
   This function reads the next token from the current input stream.   It
   assumes that current_ch has already been set to the next un-interpreted
   character from the input stream.
*/
{
   Boolean again,two_char_symbol;
   token answer;
   short start_lno,start_pos;
   char curr,look;
   char sss[10];  /* e.g., "hex "ff"" */
   char *ppp;
   static char fmt[] = "hex \"%02x\"";
   /*-------------------------------------------------------*/
   is_first_token_on_line=test_if_char_is_first_on_line();
   if (end_of_file_reached) goto eof_pt;
   again=TRUE;
   while (again){
      /*--------------------------  set default return values */
      *buff = '\0';
      *rw = NOT_RESERVED_WORD;
      again=FALSE;
      /*--------------------------  skip over spacing */
      if (advance_over_spacing_eof()) goto eof_pt;
      /*--------------------------  test if "current_ch" is for first token */
      is_first_token_on_line=test_if_char_is_first_on_line();
      give_ch_info(&start_lno,&start_pos,&curr,&look);
      if (isalpha(curr)){
         answer=get_ident(inputfile,id);
         interpret_alpha_token(id,&answer,rw);}
      else if ((curr=='$')&&(isdigit(look))){
         answer=get_digit_string(inputfile,id,FALSE);
         for (ppp=id+1;*ppp!='\0';++ppp) *(ppp-1) = *ppp; *(ppp-1) = *ppp;
         answer=TK_MACPARM;}
      else if (isdigit(curr)){
         answer=get_digit_string(inputfile,id,TRUE);}
      else{
         if ((curr=='.')&&(isdigit(look))){
            show_wrn_on_both(E_NUMSTARTDOT,(void *) NULL,EMPTY_TYPE,
                             MINOR_WARN_LEV);
            answer=get_digit_string(inputfile,id,FALSE);}
         else if (curr==OPENCURLY){
            if (skip_over_comment(OPENCURLY,CLOSECURLY,start_lno)) goto eof_pt;
            again=TRUE;}
         else if (two_in_a_row(OPENPAREN,ASTERISK)){
            if (skip_over_comment(OPENPAREN,CLOSEPAREN,start_lno)) goto eof_pt;
            again=TRUE;}
#ifdef FUTURE
         else if (curr==APOSTROPHE){
            answer=load_char_string_token(inputfile,id,buff,APOSTROPHE);}
#endif
         else if (curr==QUOTATION_MARK){
            answer=load_char_string_token(inputfile,id,buff,QUOTATION_MARK);}
         else{
            make_two_in_a_row(id);
            answer=lookup_operator_token(id);
            two_char_symbol=TRUE;
            if (answer==TK_NULL){
               two_char_symbol=FALSE;
               *(id+1) = '\0';
               answer=lookup_operator_token(id);}
            if (answer==TK_NULL){
               if (isprint(*id)) ppp=id;
               else sprintf(ppp=sss,fmt,(unsigned int) *id);
               show_err_on_both(E_BADCHAR,ppp,CHAR_TYPE);}
            advance_1_char();
            if (two_char_symbol) advance_1_char();}}}
   /*-------------------------------------------------------*/
   *linnum = start_lno;
   *pos = start_pos;
   goto done_pt;
eof_pt:
   give_ch_info(linnum,pos,&curr,&look);
   *id = *buff = '\0';
   *rw = NOT_RESERVED_WORD;
   answer=TK_EOF;
done_pt:
   if (bitset(debug_flagword,DBG_LEX)) showtoken(answer,*rw,id);
   return(answer);
}
/*===========================================================================*/
token dumtoken(rwtype *rw,        /* rw output iff TK_RW returned */
               char *id,          /* ident or char representation of token */
               short *linnum,     /* line number of the token */
               short *pos,        /* position of the token */
               type_flagword_type comp) /* type of dummy numeric token */
{
   token register answer;
   short lupix;
   short start_lno,start_pos;
   char curr,look;
   /*-------------------------------------------------------*/
   give_ch_info(&start_lno,&start_pos,&curr,&look);
   if (comp==BOOL_TYPE) lupix=STNLUP_BOOL;
   else if (comp==REAL_TYPE) lupix=STNLUP_REAL;
   else lupix=STNLUP_INT;
   answer = stnlup[lupix].ans;
   strcpy(id,stnlup[lupix].idp);
   *rw=NOT_RESERVED_WORD;
   *linnum = start_lno;
   *pos = start_pos;
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
token gettoken_work(rwtype *rw,     /* rw output iff TK_RW returned */
                    char *id,       /* ident or char representation of token */
                    short *linnum,  /* line number of the token */
                    short *pos)     /* position of the token */
{
   token register answer;
   /*-------------------------------------------------------*/
   *rw = NOT_RESERVED_WORD;
   answer = gettoken_only(scratch_string_buffer,id,rw,linnum,pos);
   if (answer==TK_RW) answer = give_rw_operation_token(*rw);
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
void showtoken(token tok,         /* token to show */
               rwtype rw,         /* rw shown iff tok==TK_RW */
               char *id)          /* ident or char representation of token */
/*
    Note:  This will probably be a big routine because of all the quoted
           literals.   A production version is included which is smaller
           but its output is not as easy to read.
*/
{
#ifdef PRODUCTION
   static char fmt_aaa[] = "     tok=%d:";
   static char fmt_bbb[] = " [rw=%d]";
#endif
   /*-------------------------------------------------------*/
#ifdef PRODUCTION
   if (bugfile!=NULL) fprintf(bugfile,fmt_aaa,(int) tok);
#else
   shew_logical_token(tok);
#endif
   /*-------------------------------------------------------*/
   fprintf(bugfile," ");
   fprintf(bugfile,id);
   /*-------------------------------------------------------*/
   if (tok==TK_CHAR){
      fprintf(bugfile," [");
      fprintf(bugfile,scratch_string_buffer);
      fprintf(bugfile,"]");}
#ifdef PRODUCTION
   else if (tok==TK_RW){
      fprintf(bugfile,fmt_bbb,(int) rw);}
#else
   else if (tok==TK_RW) shew_logical_rw(rw);
#endif
   if (is_first_token_on_line) fprintf(bugfile," (first-on-line)");
   fprintf(bugfile,"\n");
   if (tok==TK_SEMI) fprintf(bugfile,"\n\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void allocate_identifier_table(Memix max_identifier_table_alloc)
{
   memsize_t nbytes;
   long temp,total;
   int_type value;
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(stderr,"sizeof(identifier_info_type)=%d\n",
(int) sizeof(identifier_info_type));
#endif
   total = 0;
   /*-------------------------------------  identifier table */
   temp = max_identifier_table_alloc;
   temp *= ((long) sizeof(identifier_info_type));
   identifier_table_bytes = (memsize_t) temp;
   total += temp;
   /*-------------------------------------  identifier usage flags */
   temp = max_identifier_table_alloc;
   temp *= ((long) sizeof(ident_usage_flag_type));
   total += temp;
   /*-------------------------------------  constant memory */
   temp = max_identifier_table_alloc;
   temp *= ((long) sizeof(real_type));
   constant_table_bytes = (memsize_t) temp;
   total += temp;
   /*-------------------------------------  test for bytes */
   if ((sizeof(void *) < sizeof(long))||(sizeof(memsize_t) < sizeof(long))){
     // show_err_on_both(E_REBUILDHUGE,language,CHAR_TYPE);   // RWB 10/19/2015

     //  fprintf(stderr," $$$$ sizeof(memsize_t) = %d \n",sizeof(memsize_t));
     //  fprintf(stderr," $$$$ sizeof(long) = %d\n ",sizeof(long));


       // abort_program();
   }
#if defined(INT_32_BIT) || defined(INT_16_BIT)
   if ((sizeof(value_union_type)) != (2*sizeof(long))){
      fprintf(stderr,"sizeof(state_offset_type)=%d\n",
              (int) sizeof(state_offset_type));
      fprintf(stderr,"sizeof(value_union_type)=%d,  2*sizeof(long)=%d\n",
              (int) sizeof(value_union_type),(int) (2*sizeof(long)));

      fprintf(stderr," $$$$>>>>.2 sizeof(memsize_t) = %d ",sizeof(memsize_t));


      show_err_on_both(E_REBUILDHUGE,language,CHAR_TYPE);
      abort_program();}
#endif
   if (total>MALLOC_MAX){
      value=total;
      show_tail_err_on_both(E_NOMEMORYP,&value,INT_TYPE,os_malloc_limit_msg);
      abort_program();}
   /*-------------------------------------  allocate memory */
   nbytes=(memsize_t) total;
#if defined(BUGGY)||defined(ALLOC_BUGGY)
fprintf(stderr,"total=%ld\n",(long) total);
#endif
   allocated_memory = (char *) g_malloc(nbytes);
#if defined(BUGGY)||defined(ALLOC_BUGGY)
fprintf(stderr,"allocated_memory = 0x%08lx[%d]\n",
(long) allocated_memory,(int) nbytes);
#endif
   if (allocated_memory==((char *) NULL)){
      show_err_on_both(E_NOMEMORYP,(void *) "(Identifier table)",CHAR_TYPE);
      exit(1);}
   identifier_table=(identifier_info_type *) allocated_memory;
   constant_table=allocated_memory+identifier_table_bytes;
   usage_table=(ident_usage_flag_type *) constant_table+constant_table_bytes;
   declare_at_top=FALSE;
   nele_constant_table=0;
   nele_ident_at_table_top=nele_identifier_table=0;
   pre_define_log("FALSE",FALSE);
   pre_define_log("TRUE",TRUE);
   rewind(xreffile);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void free_identifier_table(void)
{
   /*-------------------------------------------------------*/
   nele_identifier_table=(Memix) 0;
   identifier_table = ((identifier_info_type *) NULL);
   constant_table = ((char *) NULL);
   usage_table = ((ident_usage_flag_type *) NULL);
   g_free(allocated_memory);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
identifier_info_type *next_id_ptr()
{
   identifier_info_type *ans;
   /*-------------------------------------------------------*/
   ans = identifier_table + nele_identifier_table;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void move_top_identifiers_down()
{
   Memix iii,jjj,nnn;
   identifier_info_type *id_src;
   identifier_info_type *id_dest;
   identifier_info_type tempid;
   /*-------------------------------------------------------*/
#ifdef BUGGY
   bug_print_identifier_table();
#endif
   nnn=nele_ident_at_table_top/2;
   /*--------------------------------------  invert */
   for (iii=0;iii<nnn;++iii){
      jjj=nele_ident_at_table_top-1-iii;
      id_src=identifier_table+max_identifier_table_count-1-iii;
      id_dest=identifier_table+max_identifier_table_count-1-jjj;
      sys_memcpy(&tempid,id_src,sizeof(identifier_info_type));
      sys_memcpy(id_src,id_dest,sizeof(identifier_info_type));
      sys_memcpy(id_dest,&tempid,sizeof(identifier_info_type));}
#ifdef BUGGY
fprintf(bugfile,"nnn=%d\n",(int) nnn);
   bug_print_identifier_table();
#endif
   /*--------------------------------------  invert complete, now fast move */
   id_dest=identifier_table+nele_identifier_table;
   id_src=identifier_table+max_identifier_table_count-nele_ident_at_table_top;
   sys_memmove((char *) id_dest,(char *) id_src,
               (nele_ident_at_table_top*sizeof(identifier_info_type)));
   /*--------------------------------------  adjust counts */
   nele_identifier_table += nele_ident_at_table_top;
   nele_ident_at_table_top = 0;
#ifdef BUGGY
   bug_print_identifier_table();
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void preset_searchid_abort_if_exists()
{
   /*-------------------------------------------------------*/
   nele_ident_abort_if_exists = nele_identifier_table;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void set_searchid_abort_if_exists(short ee)
{
   /*-------------------------------------------------------*/
   ee_abort_if_exists=ee;
   searchid_abort_if_exists=TRUE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void reset_searchid_abort_if_exists()
{
   /*-------------------------------------------------------*/
   nele_ident_abort_if_exists=0;
   searchid_abort_if_exists=FALSE;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
identifier_info_type *qqsearchid(char *look,Boolean both_ways)
{
   identifier_info_type *ppp,*pppovf;
   /*-------------------------------------------------------*/
   if (both_ways){
      pppovf = identifier_table + max_identifier_table_count;
      ppp = pppovf - nele_ident_at_table_top;
      for (;ppp<pppovf;++ppp){
         if (strcmp(look,(*ppp).name)==0) return(ppp);}}
   ppp = identifier_table + (nele_identifier_table - 1);
   for (;ppp>=identifier_table;--ppp){
      if (strcmp(look,(*ppp).name)==0) return(ppp);}
   /*-------------------------------------------------------*/
   return ((identifier_info_type *) NULL);
}
/*===========================================================================*/
void declare_computational_ident(identifier_info_type *ppp,
                                 type_flagword_type typ,
                                 value_union_type *value_ptr)
/*
   This routine declares a new identifier in the identifier table.  No test
   is made to see if the identifier is already present.  It already present,
   a new version of the identifier will be pushed onto the identifier stack.

   It is assumed that the computational types (1..COMUTATIONAL_TYPE_MASK)
   correspond precisely to the first few object table types.
   This routine cannot be used to declare a state-space-variable.
*/
{
   int_type bytes;
   type_flagword_type ttt;
   /*-------------------------------------------------------*/
   ttt = typ & COMPUTATIONAL_TYPE_MASK;
   simplify((*ppp).index.dims.first);
   simplify((*ppp).index.dims.second);
   (*ppp).scope_level = nesting_scope_level;
   (*ppp).flags = typ;
   bytes = (int_type) type_size(ttt);
#ifdef BUGGY
   if ((bytes==0)||(bitset(typ,SSVAR_TYPE))){
      fprintf(stderr,"[ERROR] INTERNAL ERROR: Bad type = 0x%04x  (declare_computational_ident)\n",(int) typ);
      fatal_error=TRUE;
      ttt=REAL_TYPE;
      bytes=sizeof(real_type);}
#endif
   (*ppp).ptr.vvv = save_value_in_number_table(value_ptr,ttt);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void abort_on_table_overflow()
{
   char aux[60];
   /*-------------------------------------------------------*/
   sprintf(aux," (currently: -N=%d -I=%d)",
           (int) max_num_count,(int) max_idn_count);
   show_err_on_both(E_TABLEOVF,aux,CHAR_TYPE);
   abort_program();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void *save_value_in_number_table(value_union_type *valp,type_flagword_type typ)
/*
   This function saves a value into the number table.
*/
{
   void *ans;
   type_flagword_type comp;
   Memix bytes,align;
   pointer_union_type xptr;
   /*-------------------------------------------------------*/
   comp = typ & COMPUTATIONAL_TYPE_MASK;
   bytes = type_size(comp);
#ifdef BUGGY
#ifdef NOW_OBSOLETE
   if ((bytes==0)||(bitset(typ,SSVAR_TYPE)))
#endif
   if (bytes==0)
      show_err_on_both
         (E_INTERNAL,"Not defined for SSV's/FUNCTION's/INTERNAL's (save_value_in_number_table)",
          CHAR_TYPE);
#endif
   if (bytes==0) return((void *) NULL);
   align=0;
   if (nele_constant_table>0){
      align = 1+(nele_constant_table-1)/bytes;
      align *= bytes;}
   xptr.vvv = ans = (void *) (constant_table+align);
   nele_constant_table = align+bytes;
   if (nele_constant_table>((Memix) constant_table_bytes))
      abort_on_table_overflow();
   value_to_memory(&xptr,valp,comp);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
identifier_info_type *qqsearchid_new
      (char *look,type_flagword_type type,short errpos)
{
   identifier_info_type *ppp;
   register int_type temp;
   register Boolean is_too_many;
   /*-------------------------------------------------------*/
   ppp = qqsearchid(look,TRUE);
#ifdef BUGGY
fprintf(bugfile,
"searchid_new for (%s,TRUE) returned ppp=%x [%d]\n",
look,(int) ppp,(int) (ppp-identifier_table));
#endif
   if (ppp == ((identifier_info_type *) NULL)){
      xref_declaration(look);
      if ((nele_identifier_table+nele_ident_at_table_top) >= 
           max_identifier_table_count)
         abort_on_table_overflow();
      if (declare_at_top){
         ppp = identifier_table + max_identifier_table_count - 1
                                - nele_ident_at_table_top;
         ++nele_ident_at_table_top;}
      else{      
         ppp = identifier_table + nele_identifier_table;
         ++nele_identifier_table;}
      (*ppp).ptr.vvv = (void *) NULL;
      simplify((*ppp).index.dims.first);
      simplify((*ppp).index.dims.second);
      (*ppp).scope_level=nesting_scope_level;
      strcpy((*ppp).name,look);
      (*ppp).flags=type;}
   else{
      temp = (int_type) (ppp-identifier_table);
      is_too_many = (Boolean) (temp >= ((int_type) nele_ident_abort_if_exists));
      if (searchid_abort_if_exists && is_too_many){
         show_err_at_pos_on_both(ee_abort_if_exists,0,(*ppp).name,CHAR_TYPE);
         abort_program();}
      else if ((*ppp).scope_level >= 0)
         ppp=already_declared_err(ppp,look,type,errpos);}
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_old
   (char *lookid,type_flagword_type type,short errpos)
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = qqsearchid(lookid,FALSE);
   xref_utilize(lookid);
   if (ppp == ((identifier_info_type *) NULL)){
      show_err_at_pos_on_both(E_UNDEFIDENT,errpos,(void *) lookid,CHAR_TYPE);
      ppp = qqsearchid_new(lookid,type,errpos);}
   else if ((*ppp).scope_level<0){
#ifdef BUGGY
fprintf(bugfile,"in searchid_old...\n");
#endif
#ifdef ERRONEOUS
      xref_setvalue(lookid);
      (*ppp).scope_level=nesting_scope_level;
#endif
      show_err_at_pos_on_both(E_SCOPE,errpos,(void *) lookid,CHAR_TYPE);}
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_dummy
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier, adding it if missing.
   If identifier scope is no longer active, then it is re-activated, otherwise
   it cannot yet exist.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = qqsearchid_new(lookid,type,errpos);
   xref_setvalue(lookid);
   if ((*ppp).scope_level<0){
       (*ppp).scope_level=nesting_scope_level;
       if (type!=UNKNOWN_TYPE) (*ppp).flags=type;}
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_oldssv
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier.   The identifier must
   already be in the table and must be a state-space-variable.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = searchid_old(lookid,type,errpos);
   if (!(bitset((*ppp).flags,SSVAR_TYPE)))
      show_err_at_pos_on_both(E_NOTSSVINIMPL,errpos,(void *) lookid,CHAR_TYPE);
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_dummy_or_oldssv
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier.   The identifier must
   already be in the table and must be a state-space-variable.
*/
{
   identifier_info_type *ppp;
   any_searchid_routine doit;
   /*-------------------------------------------------------*/
   ppp=qqsearchid(lookid,TRUE);
   if (ppp==((identifier_info_type *) NULL)) doit=searchid_dummy;
   else if (bitset((*ppp).flags,SSVAR_TYPE)) doit=searchid_oldssv;
   else doit=searchid_dummy;
   ppp=doit(lookid,type,errpos);
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_oldssvconst
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier.   The identifier must
   already be in the table and must be a state-space-variable or a named
   constant.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = searchid_old(lookid,type,errpos);
   if (bitset((*ppp).flags,SSVAR_TYPE)) goto done_pos;
   if (!(bitset((*ppp).flags,ANY_SPECIAL_VAR_TYPE_MASK))) goto done_pos;
   if (bitset((*ppp).flags,FUNCTION_TYPE)) goto done_pos;
   show_err_at_pos_on_both(E_NOTSSVORCONST,errpos,(void *) lookid,CHAR_TYPE);
   /*-------------------------------------------------------*/
done_pos:
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_dcl
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier, adding it if missing.
   If identifier scope is no longer active, then it is re-activated, otherwise
   it cannot yet exist.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = qqsearchid_new(lookid,type,errpos);
   if ((*ppp).scope_level<0) ppp=already_declared_err(ppp,lookid,type,errpos);
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_dcl_at_top
      (char *lookid,type_flagword_type type,short errpos)
/*
   Search the identifier table for an identifier, adding it if missing.
   If identifier scope is no longer active, then it is re-activated, otherwise
   it cannot yet exist.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   declare_at_top=TRUE;
   ppp = searchid_dcl(lookid,type,errpos);
   declare_at_top=FALSE;
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
identifier_info_type *searchid_silent
     (char *lookid,type_flagword_type type,short errpos,
      Boolean cannot_be_unscoped)
/*
   Search the identifier table for an identifier, adding it if missing.
   If identifier scope is no longer active, then it is re-activated.
*/
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = qqsearchid(lookid,FALSE);
   if (ppp == ((identifier_info_type *) NULL))
      ppp = qqsearchid_new(lookid,type,errpos);
   else if ((*ppp).scope_level<0){
      if (cannot_be_unscoped){
#ifdef BUGGY
fprintf(bugfile,"in searchid_silent...\n");
#endif
         show_err_at_pos_on_both(E_SCOPE,errpos,(void *) lookid,CHAR_TYPE);}
      xref_setvalue(lookid);
      (*ppp).scope_level=nesting_scope_level;}
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
void deactivate_identifier_level(short lev)
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = identifier_table + (nele_identifier_table - 1);
   for (;(ppp>=identifier_table);--ppp){
      if ((*ppp).scope_level>=lev){
         if (bitset((*ppp).flags,SSVAR_TYPE))
            (*ppp).scope_level = nesting_scope_level;
         else
            (*ppp).scope_level = -((*ppp).scope_level);}}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean id_unique_first_n(identifier_info_type *idlook,int nchars)
{
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   for (ppp=identifier_table;ppp<idlook;++ppp){
      if (i_strncmp((*ppp).name,(*idlook).name,nchars)==0) return(FALSE);}
   /*-------------------------------------------------------*/
   return(TRUE);
}
/*===========================================================================*/
void push_ident_scope()
/*
   This function pushes a new variable definition level as inside one of
   the following constructs:
         BEGIN  ...  END
         IF ...  THEN ... ELSEIF ... ELSE ... ENDIF
         FOR ...  ENDFOR
         FOR ...  WHILE  ...  ADJUST;
*/
{
   /*-------------------------------------------------------*/
   ++nesting_scope_level;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void pop_ident_scope()
{
/*
   This function pops the old variable definition level and deactivates all
   of the variables which were defined (declared) at that level.
*/
   /*-------------------------------------------------------*/
   if (nesting_scope_level <= 0){
      show_err_on_both(E_INTERNAL,(void *)
                       "(Popping empty scope stack)",CHAR_TYPE);
      abort_program();}
   deactivate_identifier_level(nesting_scope_level);
   --nesting_scope_level;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int_type array_builtin_err(short ee,char *ident,char *funcname)
{
   char msg[90];
   /*-------------------------------------------------------*/
   sprintf(msg,"%s (BUILT-IN FUNCTION %s)",ident,funcname);
   show_err_on_both(ee,msg,CHAR_TYPE);
   /*-------------------------------------------------------*/
   return((int_type) 0);
}
/*===========================================================================*/
#define CALC_ROW_SSS_EEE() \
   sss = dim_offset(row,short,(*idinf).index.dims.first); \
   sss *= (nnn=dim_range((*idinf).index.dims.second)); \
   eee = sss + nnn; \
   nnn *= dim_range((*idinf).index.dims.first); \
   if (sss>nnn) sss=nnn; \
   if (eee>nnn) eee=nnn
#define CALC_COL_SSS_DDD() \
   sss = dim_offset(col,short,(*idinf).index.dims.second); \
   nnn = ddd = dim_range((*idinf).index.dims.second); \
   nnn *= dim_range((*idinf).index.dims.first); \
   if (sss>nnn) sss=nnn
#define CALC_FULL_ARRY() \
        nnn=1; \
        iii = dim_range((*idinf).index.dims.first); \
        iii *= dim_range((*idinf).index.dims.second); \
        if (iii>nnn) nnn=iii
/*===========================================================================*/
void arraymin(identifier_info_type *idinf,value_union_type *answer,
              type_flagword_type flags,state_bitstring_type ssbits)
{
   register short iii,nnn;
   /*-------------------------------------------------------*/
   CALC_FULL_ARRY();
   min_the_sub_array(idinf,answer,flags,ssbits,0,nnn,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arrayrowmin(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short row)
{
   register short sss,eee,nnn;
   /*-------------------------------------------------------*/
   CALC_ROW_SSS_EEE();
   min_the_sub_array(idinf,answer,flags,ssbits,sss,eee,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraycolmin(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short col)
{
   register short sss,ddd,nnn;
   /*-------------------------------------------------------*/
   CALC_COL_SSS_DDD();
   min_the_sub_array(idinf,answer,flags,ssbits,sss,nnn,ddd);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraymax(identifier_info_type *idinf,value_union_type *answer,
              type_flagword_type flags,state_bitstring_type ssbits)
{
   register short iii,nnn;
   /*-------------------------------------------------------*/
   CALC_FULL_ARRY();
   max_the_sub_array(idinf,answer,flags,ssbits,0,nnn,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arrayrowmax(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short row)
{
   register short sss,eee,nnn;
   /*-------------------------------------------------------*/
   CALC_ROW_SSS_EEE();
   max_the_sub_array(idinf,answer,flags,ssbits,sss,eee,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraycolmax(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short col)
{
   register short sss,ddd,nnn;
   /*-------------------------------------------------------*/
   CALC_COL_SSS_DDD();
   max_the_sub_array(idinf,answer,flags,ssbits,sss,nnn,ddd);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraysum(identifier_info_type *idinf,value_union_type *answer,
              type_flagword_type flags,state_bitstring_type ssbits)
{
   register short iii,nnn;
   /*-------------------------------------------------------*/
   CALC_FULL_ARRY();
   sum_the_sub_array(idinf,answer,flags,ssbits,0,nnn,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arrayrowsum(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short row)
{
   register short sss,eee,nnn;
   /*-------------------------------------------------------*/
   CALC_ROW_SSS_EEE();
   sum_the_sub_array(idinf,answer,flags,ssbits,sss,eee,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraycolsum(identifier_info_type *idinf,value_union_type *answer,
                 type_flagword_type flags,state_bitstring_type ssbits,
                 short col)
{
   register short sss,ddd,nnn;
   /*-------------------------------------------------------*/
   CALC_COL_SSS_DDD();
   sum_the_sub_array(idinf,answer,flags,ssbits,sss,nnn,ddd);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraycount(identifier_info_type *idinf,value_union_type *answer,
                type_flagword_type flags,state_bitstring_type ssbits)
{
   register short iii,nnn;
   /*-------------------------------------------------------*/
   CALC_FULL_ARRY();
   count_the_sub_array(idinf,answer,flags,ssbits,0,nnn,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arrayrowcount(identifier_info_type *idinf,value_union_type *answer,
                   type_flagword_type flags,state_bitstring_type ssbits,
                   short row)
{
   register short sss,eee,nnn;
   /*-------------------------------------------------------*/
   CALC_ROW_SSS_EEE();
   count_the_sub_array(idinf,answer,flags,ssbits,sss,eee,1);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void arraycolcount(identifier_info_type *idinf,value_union_type *answer,
                   type_flagword_type flags,state_bitstring_type ssbits,
                   short col)
{
   register short sss,ddd,nnn;
   /*-------------------------------------------------------*/
   CALC_COL_SSS_DDD();
   count_the_sub_array(idinf,answer,flags,ssbits,sss,nnn,ddd);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void min_the_sub_array(identifier_info_type *idinf,
                       value_union_type *answer,
                       type_flagword_type flags,
                       state_bitstring_type ssbits,
                       short ix0,short ix9,short ixdel)
{
   register short iii;
   type_flagword_type comp;
   Boolean is_ssv;
   register int_type *pppiii;
   register real_type *ppprrr;
   register int_type imin,itemp;
   register real_type rmin,rtemp;
   /*-------------------------------------------------------*/
   is_ssv = bitset((*idinf).flags,SSVAR_TYPE);
   comp = flags & COMPUTATIONAL_TYPE_MASK;
   if (is_ssv){
      if (ssbits == NULL)
         (*answer).iii=array_builtin_err(E_SCOPE,(*idinf).name,MIN_WORD);
      else if (comp==INT_TYPE){
         imin = state_space_arr_value(ssbits,(*idinf).ptr.sss,ix0);
         for (iii=ix0+1;iii<ix9;iii+=ixdel){
            itemp = state_space_arr_value(ssbits,(*idinf).ptr.sss,iii);
            if (itemp < imin) imin=itemp;}
         (*answer).iii = imin;}
      else
         (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,MIN_WORD);}
   else if (comp==INT_TYPE){
      pppiii=(*idinf).ptr.iii+ix0;
      imin = (*pppiii);
      for (iii=ix0;iii<ix9;iii+=ixdel,pppiii+=ixdel){
         itemp = (*pppiii);
         if (itemp < imin) imin=itemp;}
      (*answer).iii=imin;}
   else if (comp==REAL_TYPE){
      ppprrr=(*idinf).ptr.rrr+ix0;
      rmin = (*ppprrr);
      for (iii=ix0;iii<ix9;iii+=ixdel,ppprrr+=ixdel){
         rtemp = (*ppprrr);
         if (rtemp < rmin) rmin=rtemp;}
      (*answer).rrr=rmin;}
   else
      (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,MIN_WORD);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void max_the_sub_array(identifier_info_type *idinf,
                       value_union_type *answer,
                       type_flagword_type flags,
                       state_bitstring_type ssbits,
                       short ix0,short ix9,short ixdel)
{
   register short iii;
   type_flagword_type comp;
   Boolean is_ssv;
   register int_type *pppiii;
   register real_type *ppprrr;
   register int_type imax,itemp;
   register real_type rmax,rtemp;
   /*-------------------------------------------------------*/
   is_ssv = bitset((*idinf).flags,SSVAR_TYPE);
   comp = flags & COMPUTATIONAL_TYPE_MASK;
   if (is_ssv){
      if (ssbits == NULL)
         (*answer).iii=array_builtin_err(E_SCOPE,(*idinf).name,MAX_WORD);
      else if (comp==INT_TYPE){
         imax = state_space_arr_value(ssbits,(*idinf).ptr.sss,ix0);
         for (iii=ix0+1;iii<ix9;iii+=ixdel){
            itemp = state_space_arr_value(ssbits,(*idinf).ptr.sss,iii);
            if (itemp > imax) imax=itemp;}
         (*answer).iii = imax;}
      else
         (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,MAX_WORD);}
   else if (comp==INT_TYPE){
      pppiii=(*idinf).ptr.iii+ix0;
      imax = (*pppiii);
      for (iii=ix0;iii<ix9;iii+=ixdel,pppiii+=ixdel){
         itemp = (*pppiii);
         if (itemp > imax) imax=itemp;}
      (*answer).iii=imax;}
   else if (comp==REAL_TYPE){
      ppprrr=(*idinf).ptr.rrr+ix0;
      rmax = (*ppprrr);
      for (iii=ix0;iii<ix9;iii+=ixdel,ppprrr+=ixdel){
         rtemp = (*ppprrr);
         if (rtemp > rmax) rmax=rtemp;}
      (*answer).rrr=rmax;}
   else
      (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,MAX_WORD);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void sum_the_sub_array(identifier_info_type *idinf,
                       value_union_type *answer,
                       type_flagword_type flags,
                       state_bitstring_type ssbits,
                       short ix0,short ix9,short ixdel)
{
   register short iii;
   type_flagword_type comp;
   Boolean is_ssv;
   register int_type *pppiii;
   register real_type *ppprrr;
   register int_type isum;
   register real_type rsum;
   /*-------------------------------------------------------*/
   is_ssv = bitset((*idinf).flags,SSVAR_TYPE);
   comp = flags & COMPUTATIONAL_TYPE_MASK;
   if (is_ssv){
      if (ssbits == NULL)
         (*answer).iii=array_builtin_err(E_SCOPE,(*idinf).name,"SUM");
      else if (comp==INT_TYPE){
         for (isum=0,iii=ix0;iii<ix9;iii+=ixdel)
            isum += state_space_arr_value(ssbits,(*idinf).ptr.sss,iii);
         (*answer).iii = isum;}
      else
         (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,"SUM");}
   else if (comp==INT_TYPE){
      pppiii=(*idinf).ptr.iii+ix0;
      for (isum=0,iii=ix0;iii<ix9;iii+=ixdel,pppiii+=ixdel)
         isum += (*pppiii);
      (*answer).iii=isum;}
   else if (comp==REAL_TYPE){
      ppprrr=(*idinf).ptr.rrr+ix0;
      for (rsum=ZERO,iii=ix0;iii<ix9;iii+=ixdel,ppprrr+=ixdel)
         rsum += (*ppprrr);
      (*answer).rrr=rsum;}
   else
      (*answer).iii=array_builtin_err(E_NOTNUMBITEM,(*idinf).name,"SUM");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void count_the_sub_array(identifier_info_type *idinf,
                         value_union_type *answer,
                         type_flagword_type flags,
                         state_bitstring_type ssbits,
                         short ix0,short ix9,short ixdel)
{
   register short iii;
   type_flagword_type comp;
   Boolean is_ssv;
   register Boolean *pppbbb;
   register short knt;
   /*-------------------------------------------------------*/
   is_ssv = bitset((*idinf).flags,SSVAR_TYPE);
   comp = flags & COMPUTATIONAL_TYPE_MASK;
   if (is_ssv){
      if (ssbits == NULL)
         knt=array_builtin_err(E_SCOPE,(*idinf).name,"COUNT");
      else if (comp==BOOL_TYPE){
         for (knt=0,iii=ix0;iii<ix9;iii+=ixdel)
            if (state_space_arr_value(ssbits,(*idinf).ptr.sss,iii)!=0) ++knt;}
      else
         knt=array_builtin_err(E_NOTBOOLITEM,(*idinf).name,"COUNT");}
   else if (comp==BOOL_TYPE){
      pppbbb=(*idinf).ptr.bbb+ix0;
      for (knt=0,iii=ix0;iii<ix9;iii+=ixdel,pppbbb+=ixdel)
         if (*pppbbb) ++knt;}
   else
      knt=array_builtin_err(E_NOTBOOLITEM,(*idinf).name,"COUNT");
   /*-------------------------------------------------------*/
   (*answer).iii=knt;
}
/*===========================================================================*/
identifier_info_type *already_declared_err
     (identifier_info_type *idinfo,char *look,
      type_flagword_type type,short errpos)
{
   register short ee;
   char tempid[IDENT_MAXNCH_P+IDENT_MAXNCH];
   identifier_info_type *ppp;
   /*-------------------------------------------------------*/
   ppp = idinfo;
   ee = ((bitset((*ppp).flags,FOR_LOOP_INDEX_TYPE))
         ? E_SCOPEACTIVE : E_ALREADYDCL);
   show_err_at_pos_on_both(ee,errpos,(void *) look,CHAR_TYPE);
   if (declare_at_top){
      enk_line_and_pos(tempid,'/');
      safe_strcat_truncated(tempid,look,IDENT_MAXNCH_P);
      ppp = qqsearchid_new(tempid,type,errpos);}
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
char *statement_name_reserved_word(rwtype rw)
{
   register char *ppp;
   register reserved_word_lookup_type *rw_lookup_entry;
   /*-------------------------------------------------------*/
   rw_lookup_entry = user_reserved_word_lookup_table + rw;
   ppp = (*rw_lookup_entry).text;
   /*-------------------------------------------------------*/
   return (ppp);
}
/*===========================================================================*/
Subscript compute_offset(Subscript iii,Subscript jjj,
                         identifier_info_type *idinfo)
{
   long value;
   Boolean is_array,is_double;
   /*-------------------------------------------------------*/
   DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
   if (!is_array) return(0);
   value = dim_offset(iii,long,(*idinfo).index.dims.first);
   if (is_double){
      value *= dim_range((*idinfo).index.dims.second);
      value += dim_offset(jjj,long,(*idinfo).index.dims.second);}
   /*-------------------------------------------------------*/
   return((Subscript) value);
}
/*===========================================================================*/

