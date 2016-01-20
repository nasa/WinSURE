#define SLOW_MEMMOVE /* RWB */
/*
===============================================================================

        Low level string manipulation functions required by ASSIST and SURE
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

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "cm_vars.h"
#include "cm_prsvars.h"
#include "pass.h"

#include "errdefs.h"

#include "sys_ext.h"
#include "str_ext.h"
#include "user_ext.h"

#include "strtabsP.h"

static Boolean show_severity;
static char fmt_nnlx[14];
static char fmt_5l[14];
static char fmt_lf[10];
static char fmt_lfs[8][10];
static char fmt_le[10];
static char fmt_l[10];
static char fmt_l_ltol[22];
static char max_fmt_l[14];
static char blank_tab_buff[MAX_TAB_DELTA];
static short tab_delta=4;
static double veps,oveps;

char encoded_aux_val[42];
char enk_5_or_more_str[MAX_INT_TYPE_NDIGS_P];
/*===========================================================================*/
#define IUPR(ch) ((int) ((unsigned char) toupper(ch)))
#define ICVT(ch) ((islower((int) (ch))) ? IUPR(ch) : (int) (ch))
#define CDEFS() \
        unsigned char register *p1; \
        unsigned char register *p2; \
        int register ord
#define CBODY() \
        ord = (ICVT(*p1)-ICVT(*p2)); \
        if (ord<0) return(-1); \
        if (ord>0) return(1)
/*===========================================================================*/
void init_strlib()
{
   short register col;
   char register *ppp;
   short ix,j;
   /*-------------------------------------------------------*/
   capise_mode=TRUE;
   max_input_line_length=max_output_line_length=80;
   half_output_line_length=(max_output_line_length/2);
   /**/
   std_real_value.rrr = 0.00000000000000000000;
   std_null_value.iii = 0;
#if defined(INT_32_BIT) || defined(INT_16_BIT)
   std_null_value.pair.iiia = std_null_value.pair.iiib = 0;
#endif
   make_date_and_time();
#ifdef BUGGY
fprintf(stderr,"formats are \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
fmt_l_ltol,fmt_l,fmt_nnlx,fmt_lf,fmt_le,max_fmt_l);
#endif
   sprintf(fmt_l_ltol,"%%%s (%%%s to %%%s)",
           INT_TYPE_FORMAT,INT_TYPE_FORMAT,INT_TYPE_FORMAT);
#ifdef BUGGY
fprintf(stderr,"formats are \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
fmt_l_ltol,fmt_l,fmt_nnlx,fmt_lf,fmt_le,max_fmt_l);
#endif
   sprintf(fmt_l,"%%%s",INT_TYPE_FORMAT);
   sprintf(fmt_nnlx,"%%%s",HEX_INT_TYPE_FORMAT);
   sprintf(fmt_5l,"%%5%s",INT_TYPE_FORMAT);
#ifdef BUGGY
fprintf(stderr,"formats are \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
fmt_l_ltol,fmt_l,fmt_nnlx,fmt_lf,fmt_le,max_fmt_l);
#endif
   veps = 5.00000000000000*exp((1-REAL_DEC_PLACES)*log(10.0000000000));
   oveps = 1.0000000000000+veps;
   for (ix=(-3);ix<5;++ix)
      sprintf(fmt_lfs[3+ix],"%%%d.%d%s",REAL_DEC_PLACES-ix,REAL_DEC_PLACES-ix,
                                        REAL_TYPE_FORMAT);
   sprintf(fmt_lf,"%%%d.%d%s",REAL_DEC_PLACES,REAL_DEC_PLACES,
                              REAL_TYPE_FORMAT);
   sprintf(fmt_le,"%%%d.%d%s",REAL_DEC_PLACES,REAL_DEC_PLACES,EXP_TYPE_FORMAT);
   sprintf(max_fmt_l,"%%%d%s",MAX_INT_TYPE_NDIGS,INT_TYPE_FORMAT);
#ifdef BUGGY
fprintf(stderr,"formats are \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
fmt_l_ltol,fmt_l,fmt_nnlx,fmt_lf,fmt_le,max_fmt_l);
#endif
   /**/
   for (ppp=blank_tab_buff,col=0;col<MAX_TAB_DELTA;++col,++ppp) *ppp=' ';
   *ppp='\0';
   sprintf(maxint_str,max_fmt_l,MAX_INT_TYPE);
   sprintf(neg_maxint_str,max_fmt_l,(-MAX_INT_TYPE));
   /**/
   maxreal=MAX_REAL_TYPE;
   ln_maxreal=(real_type) log((double) (maxreal));
   maxint=MAX_INT_TYPE;
   ln_maxint=(real_type) log((double) (maxint));
#ifdef BUGGY
fprintf(stderr,"formats are \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
fmt_l_ltol,fmt_l,fmt_nnlx,fmt_lf,fmt_le,max_fmt_l);
#endif
   /**/
   *filename_prefix = '\0';
   *input_filename = '\0';
   *output_filename = '\0';
   *mod_filename = '\0';
   *list_filename = '\0';
   *object_filename = '\0';
   *xref_filename = '\0';
   *exe_filename = '\0';
   for (j=0;j<SAVE_LEVELS;++j) *tempinputfilename[j] = '\0';
   /**/
   init_error_words(DFLT_TAB_DELTA,FALSE,FALSE);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void re_init_strlib()
{
   /*-------------------------------------------------------*/
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int i_strcmp(char *s1,char *s2)
/*
   Case insensitive version of strcmp(s1,s2);
*/
{
   CDEFS();
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(stderr,"i_strcmp(%s,%s)\n",s1,s2);
#endif
   for (p1=(unsigned char *) s1,p2=(unsigned char *) s2;
        !((*p1=='\0')||(*p2=='\0'));++p1,++p2) { CBODY(); }
   if ((*p1)!='\0') return(1);
   if ((*p2)!='\0') return(-1);
   /*-------------------------------------------------------*/
   return(0);
}
/*===========================================================================*/
int i_strncmp(char *s1,char *s2,int n)
/*
   Case insensitive version of strncmp(s1,s2,n);
*/
{
   CDEFS();
   short register col;
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(stderr,"i_strncmp(%s,%s,%d)\n",s1,s2,n);
#endif
   for (col=0,p1=(unsigned char *) s1,p2=(unsigned char *) s2;
        (!((*p1=='\0')||(*p2=='\0')||(col>=n)));
        ++col,++p1,++p2){
       CBODY();}
   if (col>=n) return(0);
   if ((*p1)!='\0') return(1);
   if ((*p2)!='\0') return(-1);
   /*-------------------------------------------------------*/
   return(0);
}
/*===========================================================================*/
Boolean string_is_too_big_int_type(char *str)
/*
   This function tests a character string to determine if the value is
   in the valid integer range for the architecture being used.  See "sys.h"
   for the definition of the architecture.
*/
{
   char tmpstr[MAX_INT_TYPE_NDIGS_P];
   char *pppfrom;
   char *pppto,*pppneg;
   short col,ncols,nch;
   Boolean neg;
   int ord;
   /*-------------------------------------------------------*/
   if ((nch=(short) strlen(str))>MAX_INT_TYPE_NDIGS) return(TRUE);
   neg = (Boolean) ((*str)=='-');
   /*---------------------------------------------  compute length to copy */
   ncols=MAX_INT_TYPE_NDIGS-nch;
   pppfrom = str;
   if (neg){
      ++pppfrom;
      ++ncols;
      --nch;}
   /*---------------------------------------------  right justify string */
   for (pppto=tmpstr,col=0;col<ncols;++col) *pppto++ = '0';
   for (;col<MAX_INT_TYPE_NDIGS;++col) *pppto++ = *pppfrom++;
   if (neg){
      pppneg = tmpstr + (ncols-1);
      *pppneg = '-';}
   *pppto = '\0';
   /*---------------------------------------------  right justify string */
   if (neg) ord=strcmp(tmpstr,neg_maxint_str);
   else ord=strcmp(tmpstr,maxint_str);
   /*-------------------------------------------------------*/
   return ((Boolean) (ord>0));
}
/*===========================================================================*/
void capise_string(char *str)
/*
   This function converts each lower-case character in a string to upper-case.
*/
{
   char *ppp;
   /*-------------------------------------------------------*/
   for (ppp=str;(*ppp)!='\0';++ppp)
      if (islower(*ppp)) *ppp = (char) toupper(*ppp);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean is_a_blank_line(char *str)
{
   char register *ppp;
   /*-------------------------------------------------------*/
   for (ppp=str;(*ppp)!='\0';++ppp)
      if (!isspace(*ppp)) return(FALSE);
   /*-------------------------------------------------------*/
   return(TRUE);
}
/*===========================================================================*/
#ifdef SIGNED_TO_WORDS
Boolean signed_to_word(char *id,char ch)
/*
   This function determines if the identifier is either the word
   "TO" or "DOWNTO" and is immediately followed by (without embedded spaces)
   a plus or asterisk (if "TO") or dash or slash (if "DOWNTO").
*/
{
   Boolean register answer;
   /*-------------------------------------------------------*/
   answer=FALSE;
   if (strcmp(id,"TO")==0)
      answer=(Boolean) ((ch=='+')||(ch=='*'));
   else if (strcmp(id,"DOWNTO")==0)
      answer=(Boolean) ((ch=='-')||(ch=='/'));
   /*-------------------------------------------------------*/
   return(answer);
}
#endif
/*===========================================================================*/
Boolean ident_valid_character(char ch)
/*
   This function determines if the character is valid when embedded in an
   identifier name.
*/
{
   Boolean register valid;
   /*-------------------------------------------------------*/
   valid = (Boolean) ((isalnum(ch)) ||
                      (ch=='_') ||
                      (ch=='$'));  /* system reserved variables */
   /*-------------------------------------------------------*/
   return(valid);
}
/*===========================================================================*/
Boolean exponent_valid_character(char ch)
/*
   This function determines if the character is a valid letter for beginning
   an exponent in scientific notation (E,e,D,d).
*/
{
   Boolean register valid;
   /*-------------------------------------------------------*/
   valid = (Boolean) ((ch=='e') ||
                      (ch=='d') ||
                      (ch=='E') ||
                      (ch=='D'));  /* system reserved variables */
   /*-------------------------------------------------------*/
   return(valid);
}
/*===========================================================================*/
void value_to_memory(pointer_union_type *pptr,
                     value_union_type *value,
                     type_flagword_type comp)
{
   /*-------------------------------------------------------*/
#ifdef BUGGY
if (comp==CHAR_TYPE) fprintf(bugfile,"%c\n",(*value).ccc);
else if (comp==INT_TYPE) fprintf(bugfile,"%d\n",(int) (*value).iii);
else if (comp==BOOL_TYPE)
fprintf(bugfile,"%s\n",(((*value).bbb) ? "TRUE" : "FALSE"));
else if (comp==REAL_TYPE) fprintf(bugfile,"%f\n",(*value).rrr);
#endif
   if (comp==CHAR_TYPE) *(*pptr).ccc = (*value).ccc;
   else if (comp==INT_TYPE) *(*pptr).iii = (*value).iii;
   else if (comp==BOOL_TYPE) *(*pptr).bbb = (*value).bbb;
   else if (comp==REAL_TYPE) *(*pptr).rrr = (*value).rrr;
   else if (comp==SOFF_TYPE)
      sys_memcpy((*pptr).sss,&(*value).sss,sizeof(state_offset_type));
#ifdef BUGGY
   else show_err_on_both(E_INTERNAL,"(unknown type for value_to_memory)",
                         CHAR_TYPE);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
size_t type_size(type_flagword_type comp)
{
   size_t siz;
   /*-------------------------------------------------------*/
   if (comp==CHAR_TYPE) siz=sizeof(char);
   else if (comp==INT_TYPE) siz=sizeof(int_type);
   else if (comp==BOOL_TYPE) siz=sizeof(Boolean);
   else if (comp==REAL_TYPE) siz=sizeof(real_type);
   else if (comp==SOFF_TYPE) siz=sizeof(state_offset_type);
   else siz=0;
   /*-------------------------------------------------------*/
   return(siz);
}
/*===========================================================================*/
void standard_value(value_union_type *value,type_flagword_type comp)
{
   /*-------------------------------------------------------*/
   if (comp==CHAR_TYPE) (*value).ccc = ' ';
   else if (comp==INT_TYPE) (*value).iii = 0;
   else if (comp==BOOL_TYPE) (*value).bbb = FALSE;
   else if (comp==REAL_TYPE) (*value).rrr = ZERO;
   else if (comp==SOFF_TYPE)
      sys_memset(&(*value).sss,0,sizeof(state_offset_type));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void *arr_addr(void *ppp,short ix,type_flagword_type comp)
/* ??????   might want to make "Memix ix" instead of short ?????? */
{
   pointer_union_type ptr;
   /*-------------------------------------------------------*/
   ptr.vvv=ppp;
   if (comp==CHAR_TYPE) ptr.ccc += ((Memix) ix);
   else if (comp==INT_TYPE) ptr.iii += ((Memix) ix);
   else if (comp==BOOL_TYPE) ptr.bbb += ((Memix) ix);
   else if (comp==REAL_TYPE) ptr.rrr += ((Memix) ix);
   else if (comp==SOFF_TYPE) ptr.sss += ((Memix) ix);
   /*-------------------------------------------------------*/
   return(ptr.vvv);
}
/*===========================================================================*/
char *enk_value_union_val(value_union_type *vu,type_flagword_type auxtyp,
                          state_bitstring_type ssbits)
{
   register char *ans;
   pointer_union_type ptr;
   type_flagword_type comp;
   /*-------------------------------------------------------*/
   comp = auxtyp & COMPUTATIONAL_TYPE_MASK;
   if (comp==CHAR_TYPE) ptr.ccc = &(*vu).ccc;
   else if (comp==INT_TYPE) ptr.iii = &(*vu).iii;
   else if (comp==BOOL_TYPE) ptr.bbb = &(*vu).bbb;
   else if (comp==REAL_TYPE) ptr.rrr = &(*vu).rrr;
   else if (comp==SOFF_TYPE) ptr.sss = &(*vu).sss;
   ans=enk_ptr_union_value(&ptr,auxtyp,0,ssbits);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
char *enk_aux_val(void *aux,type_flagword_type auxtyp)
/*
   This function encodes the value denoted by "aux" and "auxtyp", returning
   a pointer to the encoded message.   If "auxtyp" happens to be CHAR_TYPE,
   then aux is returned, otherwise the "encoded_aux_val" buffer is used to
   store the encoded value and is then returned.
*/
{
   double d,absd;
   Boolean b;
   long l;
   type_flagword_type comp;
   state_offset_type *sss;
   short ix;
   /*-------------------------------------------------------*/
   comp = auxtyp & COMPUTATIONAL_TYPE_MASK;
   if (comp==CHAR_TYPE)
      return((char *) aux);
   else if (comp==BOOL_TYPE){
      b = *((Boolean *) aux);
      if (b) strcpy(encoded_aux_val,"TRUE");
      else strcpy(encoded_aux_val,"FALSE");}
   else if (comp==INT_TYPE){
      l = (long) *((int_type *) aux);
      sprintf(encoded_aux_val,fmt_l,l);}
   else if (comp==REAL_TYPE){
      d = (double) *((real_type *) aux);
      absd = fabs(d);
      if ((absd>=1000.0)||(absd<0.001)) sprintf(encoded_aux_val,fmt_le,d);
      else{
         ix = (short) (oveps*log10(absd)); ix += 4;
         sprintf(encoded_aux_val,fmt_lfs[ix],d);}}
   else if (comp==SOFF_TYPE){
      sss = (state_offset_type *) aux;
      sprintf(encoded_aux_val,"(%d..%d,%d@%d)",
              (int) ((*sss).minval),
              (int) ((*sss).maxval),
              (int) ((*sss).bit_length),
              (int) ((*sss).bit_offset));} 
   else if (comp==Q_ADDR_TYPE){
      l = (long) *((int_type *) aux);
      sprintf(encoded_aux_val,fmt_nnlx,l);}
   else
      *encoded_aux_val='\0';
   /*-------------------------------------------------------*/
   return(encoded_aux_val);
}
/*===========================================================================*/
char *enk_aux_val_just(void *aux,type_flagword_type auxtyp)
/*
   This function encodes the value denoted by "aux" and "auxtyp", returning
   a pointer to the encoded message.   If "auxtyp" happens to be CHAR_TYPE,
   then aux is returned, otherwise the "encoded_aux_val" buffer is used to
   store the encoded value and is then returned.   The maximum number of
   places is always used so that the values will line up in columns if the
   strings are printed.
*/
{
   double d,absd;
   Boolean b;
   long l;
   short i,n;
   type_flagword_type comp;
   state_offset_type *sss;
   /*-------------------------------------------------------*/
   comp = auxtyp & COMPUTATIONAL_TYPE_MASK;
   if (comp==CHAR_TYPE)
      return((char *) aux);
   else if (comp==BOOL_TYPE){
      b = *((Boolean *) aux);
      if (b) strcpy(encoded_aux_val,"TRUE ");
      else strcpy(encoded_aux_val,"FALSE");}
   else if (comp==INT_TYPE){
      l = (long) *((int_type *) aux);
      sprintf(encoded_aux_val,max_fmt_l,l);}
   else if (comp==REAL_TYPE){
      d = (double) *((real_type *) aux);
      absd = fabs(d);
      sprintf(encoded_aux_val,fmt_le,d);}
   else if (comp==SOFF_TYPE){
      sss = (state_offset_type *) aux;
      sprintf(encoded_aux_val,"(%d..%d,%d@%d)",
              (int) ((*sss).minval),
              (int) ((*sss).maxval),
              (int) ((*sss).bit_length),
              (int) ((*sss).bit_offset));
      n = strlen(encoded_aux_val);
      i = 20 - n;
      if (i>0){
         ++n;
         sys_memmove(encoded_aux_val+i,encoded_aux_val,n);
         sys_memset(encoded_aux_val,(int) ' ',i);}}
   else
      *encoded_aux_val='\0';
   /*-------------------------------------------------------*/
   return(encoded_aux_val);
}
/*===========================================================================*/
char *enk_ptr_union_value(pointer_union_type *pptr,type_flagword_type flags,
                          int_type arr_offset,
                          state_bitstring_type ssbits)
{
   register char *ans;
   type_flagword_type comp;
   Boolean bbb;
   int_type iii;
   void *vvv;
   /*-------------------------------------------------------*/
   comp = flags & COMPUTATIONAL_TYPE_MASK;
   if (bitset(flags,SSVAR_TYPE)){
      iii = state_space_arr_value(ssbits,(*pptr).sss,(Subscript) arr_offset);
      if (comp == BOOL_TYPE){
         bbb = (Boolean) iii;
         ans=enk_aux_val(&bbb,BOOL_TYPE);}
      else
         ans=enk_aux_val(&iii,INT_TYPE);}
   else{
      vvv=arr_addr((*pptr).vvv,(short) arr_offset,comp);
      ans=enk_aux_val(vvv,flags);}
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
Boolean ssvar_vs_range_err(ssvar_value_type val,state_offset_type *offinfo)
{
   Boolean in_error;
   char tmpmsg[50];
   /*-------------------------------------------------------*/
   in_error=FALSE;
   if ((val<(*offinfo).minval)||(val>(*offinfo).maxval)){
      in_error=TRUE;
      sprintf(tmpmsg,fmt_l_ltol,
              (long) val,(long) (*offinfo).minval,(long) (*offinfo).maxval);
      show_err_on_both(E_SSVALOUTOFRANGE,(void *) tmpmsg,CHAR_TYPE);}
   /*-------------------------------------------------------*/
   return(in_error);
}
/*===========================================================================*/
void encode_warning_and_severity(short level,char *wrnmsg)
{
   /*-------------------------------------------------------*/
   if (show_severity)
      sprintf(wrnmsg,
              "%s%c%s%c%d",
              unbracketed_wrn_word,warning_spacing_char,
              unbracketed_svr_word,warning_spacing_char,
              (int) level);
   else
      strcpy(wrnmsg,unbracketed_wrn_word);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void encode_arrow_to_error(char *s,short *nch_ptr,short *off_ptr,
                           Boolean point_left,Boolean point_right)
/*
   This function encodes the error pointer which points to the position
   on the listing line which is close to where the error occured.  The
   encoded arrow is of one of the following formats:
           "^ "      (returns nch=2)
           "<-^ "    (returns nch=4) (if point_left=TRUE)
           "^-> "    (returns nch=4) (if point_right=TRUE)
           "<-^-> "  (returns nch=6) (if both are true)
   The buffer for "s" must therefore be at least 7 chars long.
   If "s" is null, then the request is ignored.
*/
{
   short register n;
   /*-------------------------------------------------------*/
   *nch_ptr=0;
   *off_ptr=0;
   if (s==((char *) NULL)) return;
   n = 2;
   *s = '\0';
   if (point_left){
      *off_ptr += 2;
      n += 2;
      strcat(s,"<-");}
   strcat(s,"^");
   if (point_right){
      n += 2;
      strcat(s,"->");}
   strcat(s," ");
   *nch_ptr=n;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int_type decode_integer(char *id)
{
   /*-------------------------------------------------------*/
   return((int_type) atol(&id[1]));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
real_type decode_real(char *id)
{
   /*-------------------------------------------------------*/
   return((real_type) atof(&id[1]));
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void append_suffix_to_prefix(char *suffix,char *prefix,char *dest_pathname)
{
   /*-------------------------------------------------------*/
   strcpy(dest_pathname,prefix);
   strcat(dest_pathname,suffix);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void append_suffix_to_filename(char *suffix,char *dest_pathname)
{
   /*-------------------------------------------------------*/
   strcpy(dest_pathname,filename_prefix);
   strcat(dest_pathname,suffix);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
short translate_tab_characters(char *buffer,short nch)
/*
   This function translates tab ('\t') characters in the buffer by inserting
   the appropriate number of spaces.   It is assumed that the buffer length
   is at least "LINE_MAXNCH_P" and that the information in it will be 
   truncated to "LINE_MAXNCH+1" characters long.  It is also assumed that:
         (LINE_MAXNCH+1+tab_delta)<LINE_MAXNCH_P
*/
{
   char register *ppp;
   char *qqq;
   int new_nch,nins,ntail;
   /*-------------------------------------------------------*/
   new_nch=(int) nch;
   for (ppp=buffer;(*ppp)!='\0';++ppp){
      if ((*ppp)=='\t'){
         nins=(ppp-buffer);
         ntail=new_nch-nins;
         nins=tab_delta-(nins%tab_delta)-1;
         if (nins>0){
            qqq=ppp+nins;
            new_nch += nins;
            sys_memmove(qqq,ppp,(ntail+1));
            sys_memmove(ppp,blank_tab_buff,(nins+1));}
         else
            *ppp=' ';
         if (new_nch>LINE_MAXNCH){
            new_nch=LINE_MAXNCH+1;
            *(buffer+new_nch)='\0';}}}
   /*-------------------------------------------------------*/
   return((short) new_nch);
}
/*===========================================================================*/
short strip_off_trailing_space(char *buffer)
/*
*/
{
   short register nch;
   /*-------------------------------------------------------*/
   nch=siglen(buffer,(short) strlen(buffer));
   *(buffer+nch)='\0';
   /*-------------------------------------------------------*/
   return(nch);
}
/*===========================================================================*/
void make_promotion_message
        (char *ans,type_flagword_type from_comp,type_flagword_type to_comp)
{
   /*-------------------------------------------------------*/
   sprintf(ans,"%s TO %s",named_types[from_comp],named_types[to_comp]);
#ifdef BUGGY
sprintf(ans,"%s(%02x) TO %s(%02x)",
named_types[from_comp],(int) from_comp,
named_types[to_comp],(int) to_comp);
if ((from_comp>=0)&&(from_comp<=5)&&(to_comp>=0)&&(to_comp<=5)) return;
*ans = 0;
*ans = 1/(*ans);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void make_symbolic_type_list(char *ans,type_flagword_type type)
{
   type_flagword_type comp;
   Boolean is_const;
   /*-------------------------------------------------------*/
   comp = type & COMPUTATIONAL_TYPE_MASK;
   strcpy(ans,named_types[comp]);
   is_const=(Boolean) (!bitset(type,ANY_SPECIAL_VAR_TYPE_MASK));
   if (bitset(type,FOR_LOOP_INDEX_TYPE)) strcat(ans,",<for-index>");
   if (bitset(type,SSVAR_TYPE)) strcat(ans,",<ss-var>");
   if (bitset(type,FUNCTION_TYPE)){
      if (bitset(type,EXPR_VARIABLE_TYPE)) strcat(ans,",<implicit-func>");
      else strcat(ans,",<func>");}
   else if (bitset(type,EXPR_VARIABLE_TYPE)) strcat(ans,",<expr-var>");
   if (bitset(type,ARRAY_TYPE)) strcat(ans,",<array>");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fprintf_hex_pointer(FILE *fp,void *ppp)
{
   /*-------------------------------------------------------*/
   fprintf(fp,fmt_nnlx,(long) ppp);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fprintf_hex_reladdr(FILE *fp,relative_address_type reladdr)
{
   /*-------------------------------------------------------*/
   fprintf(fp,fmt_nnlx,(long) reladdr);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fprintf_hex_memix(FILE *fp,Memix mix)
{
   /*-------------------------------------------------------*/
   fprintf(fp,fmt_nnlx,(long) mix);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
char *enk_int_5_wide(int_type iii)
{
   char *ans;
   /*-------------------------------------------------------*/
   ans=enk_5_or_more_str;
   sprintf(ans,fmt_5l,(long) iii);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
#ifdef SLOW_MEMMOVE
void slow_memmove(char *dest,char *src,int nch)
{
   register char *psrc;
   register char *pdest,*povf;
   /*-------------------------------------------------------*/
   if (dest>src){
      for (pdest=dest+nch-1,psrc=src+nch-1;pdest>=dest;--pdest,--psrc){
         *pdest = *psrc;}}
   else{
      for (pdest=dest,psrc=src,povf=pdest+nch;pdest<povf;++pdest,++psrc){
         *pdest = *psrc;}}
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
void init_error_words(short tabdel,Boolean three_letters_only,Boolean ss)
{
   /*-------------------------------------------------------*/
   tab_delta=tabdel;
   show_severity=ss;
   if (three_letters_only){
      strcpy(unbracketed_err_word,"ERR");
      strcpy(unbracketed_wrn_word,"WRN");
      strcpy(unbracketed_svr_word,"LEV");
      warning_spacing_char='.';}
   else{
      strcpy(unbracketed_err_word,"ERROR");
      strcpy(unbracketed_wrn_word,"WARNING");
      strcpy(unbracketed_svr_word,"LEVEL");
      warning_spacing_char=' ';}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean remove_extent(char *suffix,char *path)
{
   register short iii,jjj;
   register short nch,nch_suff;
   /*-------------------------------------------------------*/
   nch=strip_off_trailing_space(path);
   for (iii=nch-1,jjj=strlen(suffix)-1;(iii>=0)&&(jjj>=0);--iii,--jjj){
      if (path[iii]!=suffix[jjj]) goto done;}
   ++iii;
   path[iii]='\0';
done:
#ifdef THREE_CHARACTER_EXTENTS
   nch=nch_suff=4;
#else
   nch=nch_suff=5;
#endif
   nch += strlen(path);
   if (nch>MAXNCH_PATH){
      show_err(stderr,E_FILENAMEBIG,(void *) path,CHAR_TYPE);
      goto too_long;}
   for (iii=(nch=strlen(path))-1;
        (iii>=0)&&
        ((*(path+iii))!=FILE_DIRECTORY_TERMINATOR)&&
        ((*(path+iii))!=AUX_FILE_DIREC_TERMINATOR);
        --iii); ++iii;
   if ((nch+nch_suff-iii)>ENTRYNAME_MAXLEN){
      show_err(stderr,E_FILENAMEBIG,
               (path+iii+ENTRYNAME_MAXLEN-nch_suff),CHAR_TYPE);
      goto too_long;}
   /*-------------------------------------------------------*/
   return(FALSE);
too_long:
   return(TRUE);
}
/*===========================================================================*/
