#if defined(MSDOS)||defined(UNIX)||defined(LINUX)
extern char *mktemp(char *);    /* ????? */
#endif
/*
===============================================================================

        Input/output functions required to parse ASSIST and SURE 
        compiler programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

//typedef  long off_t;    /* RWB */

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "cm_vars.h"
#include "cm_prsvars.h"
#include "pass.h"

#include "errdefs.h"
#include "tokdefs.h"

#include "user_ext.h"
#include "sys_ext.h"
#include "str_ext.h"
#include "lio_ext.h"
#include "pio_ext.h"
#include "lex_ext.h"

static unsigned char *big_output_file_buffer;
static unsigned char *big_input_file_buffer;
static scanning_character_info_type charinfo;
static Boolean quoting_allowed=FALSE;          /* some future option maybe */

static char full_tail_msg_buff[FULL_TAIL_MSG_BUFFLEN];

char tempnam_buffer[36];

/*===========================================================================*/
void init_prsiolib(scanning_character_info_type *pair)
{
   /*-------------------------------------------------------*/
   charinfo.current_ch_lno = charinfo.current_ch_pos =
   charinfo.lookahead_ch_lno = charinfo.lookahead_ch_pos = 0;
   charinfo.current_ch = charinfo.lookahead_ch = ' ';
   sys_memcpy(pair,&charinfo,sizeof(scanning_character_info_type));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void restore_prsiolib(scanning_character_info_type *pair)
{
   /*-------------------------------------------------------*/
   sys_memcpy(&charinfo,pair,sizeof(scanning_character_info_type));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
FILE *open_input_using_big_buffer(char *filename)
{
   off_t bytes;
   stat_t statinf;
   int fnum;
   FILE *ans;
   /*-------------------------------------------------------*/
   /*------------------------------------  open & get file size */
#ifdef BUGGY
fprintf(stderr,"open file=\"%s\"\n",filename);
#endif
   ans=fopen_read_only(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   fnum = fileno(ans);
   fstat(fnum,&statinf);
   bytes=statinf.st_size;
#ifdef BUGGY
if (bytes<80){
fprintf(stderr,"**** error, statinf.st_size=0x%lx\n",(unsigned long) statinf.st_size);
bytes=(off_t) 0x4000;}
#endif
#ifdef MSDOS
   if (bytes>65530L) bytes=65530L;
#endif
   fclose(ans);
   /*------------------------------------  re-open and allocate buffer */
   ans=fopen_read_only(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   big_input_file_buffer=(unsigned char *) g_malloc(bytes);
   if (big_input_file_buffer==((unsigned char *) NULL))
      big_input_file_buffer=(unsigned char *)
         g_malloc(bytes=((off_t) 0x4000)); /* 16K */
   /*------------------------------------  use big buffer instead of system */
   if (big_input_file_buffer!=((unsigned char *) NULL)){
      setvbuf(ans,(char *) big_input_file_buffer,_IOFBF,(size_t) bytes);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
FILE *open_output_using_big_buffer(char *filename,size_t bytes)
{
   FILE *ans;
   /*-------------------------------------------------------*/
   ans=open_output_file(filename);
   big_output_file_buffer=(unsigned char *) g_malloc(bytes);
   if (big_output_file_buffer!=((unsigned char *) NULL)){
      setvbuf(ans,(char *) big_output_file_buffer,_IOFBF,(size_t) bytes);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void free_big_input_buffer()
{
   /*-------------------------------------------------------*/
   if (big_input_file_buffer!=((unsigned char *) NULL))
      g_free(big_input_file_buffer);
   big_input_file_buffer = ((unsigned char *) NULL);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void safe_fprintf(FILE *fp,char *line)
/*
   This function prints a character string, substituting question marks
   for non-printing characters.
*/
{
   char register *ppp;
   int register iii;
   /*-------------------------------------------------------*/
   for (ppp=line;(*ppp)!='\0';++ppp){
      iii = (*ppp);
      safe_putc(iii,fp);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void make_temporary_filename(char *filename)
{
   char *tfnm;
   /*-------------------------------------------------------*/
   strcpy(tempnam_buffer,"QQXXXXXX");
   tfnm = mktemp(tempnam_buffer);
   strcpy(filename,tfnm);
#ifdef BUGGY
fprintf(bugfile,"temorary file name = \"%s\"\n",filename);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void cleanup_temporary_filename(char *filename)
{
   int ierr;
   /*-------------------------------------------------------*/
   if ((*filename)=='\0') return;
#if defined(VMS) && defined(OBSOLETE)
strcat(filename,".;*");
#endif
   ierr=remove(filename);
#ifdef BUGGY
fprintf(bugfile,
">>> cleaning up temporary filename=\"%s\", ierr=%d\n",
tempinputfilename[0],ierr);
#endif
   *filename = '\0';
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
short adjust_wrapping_column(FILE *fp,short need,
                             short indent,short nowcol,short ncols,
                             short *linecount)
{
   short col,jjj;
   /*-------------------------------------------------------*/
   col=nowcol;
   if ((nowcol+need)>ncols){
      *linecount += 1;
      fprintf(fp,"\n");
      for (jjj=0;jjj<indent;++jjj) fprintf(fp," ");
      col=indent;}
   col += need;
   /*-------------------------------------------------------*/
   return(col);
}
/*===========================================================================*/
#define Q_INIT(buffer,nchars) \
   ppp=buffer; \
   pppovf=ppp+((Memix) nchars)
#define STOR_FIRST_CH(buffer,nchars) \
   Q_INIT(buffer,nchars); \
   *ppp++ = equiv_text_char((int) charinfo.current_ch); \
   advance_1_char()
#define STOR_CH_N_FETCH() \
      *ppp++ = equiv_text_char((int) charinfo.current_ch); \
      advance_1_char()
#define STOR_LOOP(func) \
   while ((ppp<pppovf) && func(charinfo.current_ch)){ \
      STOR_CH_N_FETCH();}
#define Q_STOR_CH_N_FETCH() \
      if ((quoting_allowed) && (charinfo.current_ch=='\\')) advance_1_char(); \
      *ppp++ = equiv_text_char((int) charinfo.current_ch); \
      advance_1_char()
#define Q_STOR_LOOP(func) \
   while ((ppp<pppovf) && func(charinfo.current_ch)){ \
      Q_STOR_CH_N_FETCH();}
#define TRUNCATE_LOOP(func) \
   truncated=FALSE; \
   while (func(charinfo.current_ch)){ \
      truncated=TRUE; \
      advance_1_char();}
/*===========================================================================*/
Boolean two_in_a_row(char c1,char c2)
/*
   This function decides if the current and lookahead characters are
   equal to "c1" and "c2", respectively.   If "c2" is the null '\0'
   character, then the function reverts to a one-in-a-row function
   looking for current character equal to "c1".
*/
{
   Boolean register ans;
   /*-------------------------------------------------------*/
   ans=(Boolean) ((charinfo.current_ch==c1)&&
                  ((c2=='\0')||(charinfo.lookahead_ch==c2)));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void make_two_in_a_row(char *s)
/*
   This function concatenates the current character with the lookup character
   and stores the result in the string "s" (at least 3 chars long).
*/
{
   /*-------------------------------------------------------*/
   *s = charinfo.current_ch;
   *(s+1) = charinfo.lookahead_ch;
   *(s+2) = '\0';
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
token get_ident(FILE *fp,char *id)
/*
   This function reads an identifier from the input stream.   It is assumed
   that "current_ch" contains the first character of the identifier.   Upon
   exit,   "current_ch"  will contain the first character which follows the 
   identifier.
*/
{
   int_type val;
   token answer;
   char *ppp,*pppovf;
   Boolean truncated;
   short hold;
   /*-------------------------------------------------------*/
   STOR_FIRST_CH(id,IDENT_MAXNCH);
   /*---------------------------------  string of ident-valid-chars */
   answer=TK_ID;
   STOR_LOOP(ident_valid_character);
   /*---------------------------------  TO+ or TO* or DOWNTO- or DOWNTO/  */
#ifdef SIGNED_TO_WORDS
   if (signed_to_word(id,charinfo.current_ch)){
      STOR_CH_N_FETCH();}
#endif
   /*---------------------------------  null terminated */
   *ppp='\0';
   /*---------------------------------  truncate when identifier too long */
   hold=arrow_position;
   TRUNCATE_LOOP(ident_valid_character);
   if (truncated){
      val=IDENT_MAXNCH;
      arrow_position=hold;
      show_tail_wrn_on_both(E_IDTRUNC,(void *) &val,INT_TYPE,"CHARS MAX",
                            DEFAULT_WARN_LEV);}
   /*---------------------------------  capitalize the identifier */
   if (capise_mode) capise_string(id);
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
token get_digit_string(FILE *fp,char *enkval,Boolean dotokay)
/*
   This function reads a digit string from the input stream.   A prefix of
   "#" is used.   The digit string is not converted to a number.
   It is assumed that "current_ch" contains the first character of the 
   number string.   Upon exit, "current_ch"  will contain the first
   character which follows the number string.
*/
{
   int_type val;
   token answer;
   char *ppp,*pppovf;
   Boolean truncated;
   char ch;
   char lup_str[3];
   short hold;
   /*-------------------------------------------------------*/
   answer=TK_INT;
   if (charinfo.current_ch=='.') answer=TK_REAL;
   ch=charinfo.current_ch;
   charinfo.current_ch=NUMBER_TABLE_PREFIX_CHAR;
   STOR_FIRST_CH(enkval,IDENT_MAXNCH);
   *ppp++ = ch;
   /*---------------------------------  string of digits */
   STOR_LOOP(isdigit);
   /*---------------------------------  optional decimal point */
   make_two_in_a_row(lup_str);
   if ((dotokay) && (ppp<pppovf) && (charinfo.current_ch=='.') && 
       (lookup_operator_token(lup_str)==TK_NULL)){
      STOR_CH_N_FETCH();
      if (answer<TK_REAL) answer=TK_REAL;}
   /*---------------------------------  string of digits */
   STOR_LOOP(isdigit);
   /*---------------------------------  optional E or E+ or E- */
   if ((ppp<pppovf) && (exponent_valid_character(charinfo.current_ch))){
      STOR_CH_N_FETCH();
      if (answer<TK_REAL) answer=TK_REAL;
      if ((ppp<pppovf) && 
          ((charinfo.current_ch=='+')||(charinfo.current_ch=='-'))){
         STOR_CH_N_FETCH();}}
   /*---------------------------------  optional string of digits */
   STOR_LOOP(isdigit);
   /*---------------------------------  null terminated */
   *ppp='\0';
   /*---------------------------------  truncate when digit string too long */
   hold=arrow_position;
   TRUNCATE_LOOP(isdigit);
   if (truncated){
      val=IDENT_MAXNCH-1;
      arrow_position=hold;
      show_tail_wrn_on_both
           (E_NUMTRUNC,(void *) &val,INT_TYPE,"CHARS MAX",SERIOUS_WARN_LEV);}
   /*---------------------------------  capitalize the digit string */
   if (capise_mode) capise_string(enkval);
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
#define is_not_given_quotech(ccc) (((ccc)!=quotech)&&(!end_of_file_reached))
/*===========================================================================*/
type_flagword_type get_literal_text(FILE *fp,char *buff,char quotech)
/*
   This function reads a quoted character string which can end with
   any given ascii character "quotech".   It is assumed that "current_ch"
   contains the first character following the opening "quotech".   Upon exit,
   "current_ch" will contain the first character following the closing
   "quotech" which terminated the literal text.
*/
{
   type_flagword_type answer;
   char *ppp,*pppovf;
   Boolean truncated;
   short hold,start_lno;
   char tmpmsg[30];
   /*-------------------------------------------------------*/
   start_lno=charinfo.current_ch_lno;
   Q_INIT(buff,STRING_MAXNCH);
   /*---------------------------------  string of text chars */
   answer=CHAR_TYPE;
   Q_STOR_LOOP(is_not_given_quotech);
   if (end_of_file_reached) goto eof_pt;
   /*---------------------------------  null terminated */
   *ppp='\0';
   /*---------------------------------  truncate when identifier too long */
   hold=arrow_position;
   if (hold>max_input_line_length) hold=max_input_line_length;
   TRUNCATE_LOOP(is_not_given_quotech);
   if (end_of_file_reached) goto eof_pt;
   if (truncated){
      arrow_position=hold;
      show_wrn_on_both(E_STRINGBIG,(void *) buff,CHAR_TYPE,SERIOUS_WARN_LEV);}
   /*---------------------------------  skip over closing quote character */
   advance_1_char();
   /*-------------------------------------------------------*/
   return(answer);
eof_pt:
   sprintf(tmpmsg," (%c on line#%d)",quotech,(int) start_lno);
   show_err_on_both(E_UNTERMQUOTE,tmpmsg,CHAR_TYPE);
   return(answer);
}
/*===========================================================================*/
void advance_1_char()
{
   /*-------------------------------------------------------*/
   user_advance_1_char(&charinfo);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean advance_over_spacing_eof()
{
   /*-------------------------------------------------------*/
   while (is_iolib_space(charinfo.current_ch)&&(!end_of_file_reached)){
      advance_1_char();}
   if (is_iolib_space(charinfo.current_ch)&&(end_of_file_reached))
      return(TRUE);
   /*-------------------------------------------------------*/
   return(FALSE);
}
/*===========================================================================*/
Boolean skip_over_comment(char bgnch,char endch,short start_lno)
/*
   This function skips over the comment beginning with "bgnch" and ending
   with "endch".   If "endch" is a '}' character, then "bgnch" should be
   a '{' character and the delimiters will be one character long.   If
   "endch" is NOT a '}', then an '*' is used as the second character of
   the opening comment sequence and the first character of the closing
   comment sequence.   For example:

             bgnch   endch   comment format
             -----   -----   --------------
               (       )       (*      *)    (two characters long)
               [       ]       [*      *]    (two characters long)
               #       #       #*      *#    (two characters long)
               $       $       $*      *$    (two characters long)
               |       |       |*      *|    (two characters long)
             -----   -----   --------------
               {       }       {        }    (single character long)
        
*/
{
   char trmch,midch;
   Boolean single_char,answer;
   char tmpmsg[50];
   /*-------------------------------------------------------*/
   skipping_inside_comment=TRUE;
   advance_1_char();
   midch=ASTERISK; trmch=endch;
   single_char=(Boolean) (endch==CLOSECURLY);
   if (single_char) {
      midch=CLOSECURLY; trmch='\0';}
   else advance_1_char();
   /*--------------------------------- skip over comment text */
   while ((!two_in_a_row(midch,trmch))&&(!end_of_file_reached))
      advance_1_char();
   /*--------------------------------- skip over comment terminator */
   if (end_of_file_reached){
      /*----------------- end of file, no need to skip */
      if (single_char)
         sprintf(tmpmsg," (\"%c\" on line#%d)",OPENCURLY,(int) start_lno);
      else
         sprintf(tmpmsg," (\"%c%c\" on line#%d)",bgnch,midch,(int) start_lno);
      show_err_on_both(E_UNTERMCOMMENT,tmpmsg,CHAR_TYPE);
      answer=TRUE;}
   else{
      /*----------------- skip over terminator (1 or 2 chars long) */
      advance_1_char(); if (!single_char) advance_1_char();
      answer=FALSE;}
   skipping_inside_comment=FALSE;
   /*-------------------------------------------------------*/
   return(answer);
}
/*===========================================================================*/
void show_err_at_pos_on_both
     (short ee,short pos,void *aux,type_flagword_type auxtyp)
/*
   This function displays the error message corresponding to "ee" on both the
   output and listing files.  The message arrow will point to source code 
   column "pos".
*/
{
   short hold;
   /*-------------------------------------------------------*/
   hold = arrow_position;
   arrow_position = pos;
   show_err_on_both(ee,aux,auxtyp);
   arrow_position = hold;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_wrn_at_pos_on_both
     (short ee,short pos,void *aux,type_flagword_type auxtyp,short level)
/*
   This function displays the warning message corresponding to "ee" on both the
   output and listing files.  The message arrow will point to source code 
   column "pos".
*/
{
   short hold;
   /*-------------------------------------------------------*/
   hold = arrow_position;
   arrow_position = pos;
   show_wrn_on_both(ee,aux,auxtyp,level);
   arrow_position = hold;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void setup_for_tail_msg(void *aux,type_flagword_type auxtyp,char *tail)
{
   char *msg_ptr;
   /*-------------------------------------------------------*/
   msg_ptr = enk_aux_val(aux,auxtyp);
   strcpy(full_tail_msg_buff," ");
   safe_strcat_truncated(full_tail_msg_buff+1,msg_ptr,FULL_TAIL_MSG_BUFFLEN);
   safe_strcat_truncated(full_tail_msg_buff," ",FULL_TAIL_MSG_BUFFLEN);
   safe_strcat_truncated(full_tail_msg_buff,tail,FULL_TAIL_MSG_BUFFLEN);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_tail_err_on_both
         (short ee,void *aux,type_flagword_type auxtyp,char *tail)
/*
   This function displays the error message corresponding to "ee" on both the
   output and listing files.
*/
{
   /*-------------------------------------------------------*/
   setup_for_tail_msg(aux,auxtyp,tail);
   show_err_on_both(ee,(void *) full_tail_msg_buff,CHAR_TYPE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_tail_wrn_on_both
         (short ee,void *aux,type_flagword_type auxtyp,char *tail,short level)
/*
   This function displays the warning message corresponding to "ee" on both the
   output and listing files.
*/
{
   setup_for_tail_msg(aux,auxtyp,tail);
   show_wrn_on_both(ee,(void *) full_tail_msg_buff,CHAR_TYPE,level);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_tail_err_at_pos_on_both
     (short ee,short pos,void *aux,type_flagword_type auxtyp,char *tail)
/*
   This function displays the error message corresponding to "ee" on both the
   output and listing files.  The message arrow will point to source code 
   column "pos".
*/
{
   short hold;
   /*-------------------------------------------------------*/
   hold = arrow_position;
   arrow_position = pos;
   show_tail_err_on_both(ee,aux,auxtyp,tail);
   arrow_position = hold;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_tail_wrn_at_pos_on_both
     (short ee,short pos,void *aux,type_flagword_type auxtyp,short level,
      char *tail)
/*
   This function displays the warning message corresponding to "ee" on both the
   output and listing files.  The message arrow will point to source code 
   column "pos".
*/
{
   short hold;
   /*-------------------------------------------------------*/
   hold = arrow_position;
   arrow_position = pos;
   show_tail_wrn_on_both(ee,aux,auxtyp,tail,level);
   arrow_position = hold;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
