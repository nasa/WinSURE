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

#include "common.h"

#include "astrw.h"

#include "objdefs.h"
#include "objtypes.h"

#include "astr_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "fix_ext.h"
#include "obj_ext.h"

static short last_token_gotten_from_linnum=0;
static short last_token_gotten_from_pos=0;

typedef struct t__cross_reference_entry
   {
      unsigned short linnum;
      unsigned short pos;
      char name[XREF_IDENT_MAXNCH_P];
      char refcode;
   } cross_reference_entry_type;

/*===========================================================================*/
void re_init_astlex()
{
   /*-------------------------------------------------------*/
   last_token_gotten_from_linnum=last_token_gotten_from_pos=0;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void store_identifier_table()
{
   identifier_info_type *idinfo,*idovf;
   relative_address_type reladdr;
   long count;
   object_table_type ooo,ooostore;
   type_flagword_type sc;
   Memix ixid;
   Boolean is_array,is_double;
   /*-------------------------------------------------------*/
   idinfo=identifier_table;
   idovf=idinfo+nele_identifier_table;
   for (ixid=0;idinfo<idovf;++idinfo,++ixid){
      ooo = cvt_to_object_table_type((*idinfo).flags);
      if (bitset((*idinfo).flags,SSVAR_TYPE)) ooo = OBJ_SOFF_DATA;
      sc = ooo;
      if (sc == OBJ_SOFF_DATA)
         count = 1;
      else{
         DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
         count = 1;
         if (is_array){
            count = dim_range((*idinfo).index.dims.first);
            if (is_double) count *= dim_range((*idinfo).index.dims.second);}}
      Set_ooostore();
      if ((*idinfo).ptr.vvv==NULL) reladdr=NIX_ADDR;
      else reladdr =
         store_in_object_file(ooostore,(*idinfo).ptr.vvv,type_size(sc),count);
      (*idinfo).ptr.relative_address = reladdr;}
   store_in_object_file(OBJ_IDTABLE,(void *) identifier_table,
                        sizeof(identifier_info_type),nele_identifier_table);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define NEW_XREF_LINE(nam,rfc) \
        fprintf(listfile,newline_fmt); \
        list_page_need(1); \
        fprintf(listfile,name_fmt,nam,rfc)
void do_xref_map()
{
   int_type pos;
   int knt,nnn,nnn_less,iii,jjj,kkk,iii_src,iii_dest,new_iii;
   size_t eachsize;
   cross_reference_entry_type *item_arr;
   cross_reference_entry_type *item_iii;
   cross_reference_entry_type *item_jjj;
   char blankname[XREF_IDENT_MAXNCH_P];
   static char newline_fmt[] = "\n";
   static char name_fmt[] = "%s%s   ";
   static char pos_fmt[] = "%d c%d%s";
   char enkpos[XREF_IDENT_MAXNCH_P+25];
   char last_refcode,curr_refcode;
   Boolean undcl,unset,unuse,special;
   /*-------------------------------------------------------*/
   if (!xref_map) return;
   list_page_need_test_only(32000);
   sys_memset(blankname,(int) ' ',(size_t) XREF_ID_TRUNC_NCH);
   *(blankname+XREF_ID_TRUNC_NCH) = '\0';
   /*--------------------------------------------   size the file */
   pos=ftell(xreffile);
   knt=eachsize=sizeof(cross_reference_entry_type);
   knt=(int) (pos/((int_type) knt));
   /*--------------------------------------------   close & re-open to read */
   fclose(xreffile);
   xreffile=open_binary_input_file(xref_filename);
   /*--------------------------------------------   allocate */
   item_arr = (cross_reference_entry_type *) calloc((size_t) knt,eachsize);
   if (item_arr==((cross_reference_entry_type *) NULL)) return;
   /*--------------------------------------------   read file */
   nnn=fread(item_arr,eachsize,(size_t) knt,xreffile);
   if (nnn!=knt) goto free_point;
   /*--------------------------------------------   sort */
   qsort((char *) item_arr,(size_t) knt,eachsize,cmp_xref_entries);
   /*--------------------------------------------   remove duplicates */
   for (nnn_less=nnn-1,iii_src=iii_dest=0;iii_src<nnn;++iii_dest){
      if (iii_src != iii_dest)
         sys_memcpy(item_arr+iii_dest,item_arr+iii_src,eachsize);
      while ((iii_src<nnn) &&
             (memcmp(item_arr+iii_dest,item_arr+iii_src,eachsize)==0))
         ++iii_src;}
   nnn=iii_dest;
   /*--------------------------------------------   print_map */
   for (iii=0;iii<nnn;++iii){
      item_iii = item_arr + iii;
      undcl=unset=unuse=TRUE;
      last_refcode = (*item_iii).refcode;
      NEW_XREF_LINE((*item_iii).name,xref_label(last_refcode));
      special = (Boolean) (*(*item_iii).name == '<');
      for (new_iii=iii-1,jjj=iii,kkk=0;jjj<nnn;++jjj,++kkk){
         new_iii=jjj;
         item_jjj = item_arr + jjj;
         curr_refcode = (*item_jjj).refcode;
         if (memcmp((*item_iii).name,(*item_jjj).name,XREF_ID_TRUNC_NCH)==0){
            XREF_FIX_UNSET_ETC(curr_refcode);
            if (last_refcode != curr_refcode){
               kkk=0;
               NEW_XREF_LINE(blankname,xref_label(curr_refcode));}
            if ((kkk>0)&&((kkk%5)==0)){
               NEW_XREF_LINE(blankname,"   ");}
            sprintf(enkpos,pos_fmt,
                    (int) (*item_jjj).linnum,(int) ((*item_jjj).pos+1),
                    blankname);
            *(enkpos+10) = '\0';
            fprintf(listfile,"%s",enkpos);}
         else{
            new_iii=jjj-1;
            jjj=nnn;}
         last_refcode = curr_refcode;}
      iii=new_iii;
      if (undcl && (!special)) { NEW_XREF_LINE(blankname,"*** UNDCL ***"); }
      if (unset && (!special)) { NEW_XREF_LINE(blankname,"*** UNSET ***"); }
      if (unuse && (!special)) { NEW_XREF_LINE(blankname,"*** UNUSE ***"); }
      fprintf(listfile,"\n");
      list_page_need_test_only(5); list_page_need(1);}
   /*--------------------------------------------   free */
free_point:
   free(item_arr);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define XREF_ID_TRUNC_NCH 23
int cmp_xref_entries(void *a,void *b)
{
   register cross_reference_entry_type *aaa,*bbb;
   register int ans;
   /*-------------------------------------------------------*/
   aaa = (cross_reference_entry_type *) a;
   bbb = (cross_reference_entry_type *) b;
#ifdef BUGGY
fprintf(bugfile,"Comparing ");
fprintf(bugfile,"(\"%s\",%c,%d,%d) ",
(*aaa).name,(*aaa).refcode,(int) (*aaa).linnum,(int) (*aaa).pos);
fprintf(bugfile,"with ");
fprintf(bugfile,"(\"%s\",%c,%d,%d) ",
(*bbb).name,(*bbb).refcode,(int) (*bbb).linnum,(int) (*bbb).pos);
fprintf(bugfile,"\n");
#endif
   *((*aaa).name+XREF_ID_TRUNC_NCH) = '\0';
   *((*bbb).name+XREF_ID_TRUNC_NCH) = '\0';
   if ((ans=strcmp((*aaa).name,(*bbb).name))!=0) return(ans);
   if ((*aaa).refcode < (*bbb).refcode) return(-1);
   if ((*aaa).refcode > (*bbb).refcode) return(1);
   if ((*aaa).linnum < (*bbb).linnum) return(-1);
   if ((*aaa).linnum > (*bbb).linnum) return(1);
   if ((*aaa).pos < (*bbb).pos) return(-1);
   if ((*aaa).pos > (*bbb).pos) return(1);
   /*-------------------------------------------------------*/
   return(0);
}
/*===========================================================================*/
void add_xref_item(char *look,char code)
{
   cross_reference_entry_type item;
   int nch;
   /*-------------------------------------------------------*/
   if (!xref_map) return;
   if (((*look)=='$')&&(isascii(*(look+1)))&&(isdigit(*(look+1)))) return;
   if ((*look)==NUMBER_TABLE_PREFIX_CHAR) return;
   item.linnum=last_token_gotten_from_linnum;
   item.pos=last_token_gotten_from_pos;
   nch=strlen(look);
   if (nch>XREF_IDENT_MAXNCH_P) nch=XREF_IDENT_MAXNCH_P;
   sys_memcpy(item.name,look,nch);
   while (nch<XREF_IDENT_MAXNCH_P) *(item.name+(nch++)) = ' ';
   item.refcode=code;
   fwrite(&item,sizeof(cross_reference_entry_type),(size_t) 1,xreffile);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#define IFSHEW(rrr,msg) if (rw==rrr) {okay=TRUE; fprintf(bugfile,msg);}
/*===========================================================================*/
#if !defined(PRODUCTION)
void shew_logical_rw(rwtype rw)
{
   Boolean okay;
   /*-------------------------------------------------------*/
   okay=FALSE;
   IFSHEW(RW_DEBUG," [RW_DEBUG]")
   IFSHEW(RW_DBG_NONE," [RW_DBG_NONE]")
   IFSHEW(RW_DBG_LEX," [RW_DBG_LEX]")
   IFSHEW(RW_DBG_PRS," [RW_DBG_PRS]")
   IFSHEW(RW_DBG_TRANTO," [RW_DBG_TRANTO]")
   IFSHEW(RW_DBG_XEQ," [RW_DBG_XEQ]")
   IFSHEW(RW_DBG_ROU," [RW_DBG_ROU]")
   IFSHEW(RW_DBG_DATAST," [RW_DBG_DATAST]")
   IFSHEW(RW_DBG_EXPAND," [RW_DBG_EXPAND]")
   IFSHEW(RW_DBG_CODE," [RW_DBG_CODE]")
   IFSHEW(RW_DBG_ALL," [RW_DBG_ALL]")
   IFSHEW(RW_DBG_ABORT," [RW_DBG_ABORT]")
   IFSHEW(RW_INPUT," [RW_INPUT]")
#ifdef ASSIST
   IFSHEW(RW_SPACE," [RW_SPACE]")
   IFSHEW(RW_FUNCTION," [RW_FUNCTION]")
   IFSHEW(RW_IMPLICIT," [RW_IMPLICIT]")
   IFSHEW(RW_VARIABLE," [RW_VARIABLE]")
   IFSHEW(RW_INTRINSIC," [RW_?INTRINSIC?]")
   IFSHEW(RW_START," [RW_START]")
   IFSHEW(RW_DEATHIF," [RW_DEATHIF]")
   IFSHEW(RW_PRUNEIF," [RW_PRUNEIF]")
   IFSHEW(RW_ASSERT," [RW_ASSERT]")
   IFSHEW(RW_TRANTO," [RW_TRANTO]")
   IFSHEW(RW_IF," [RW_IF]")
   IFSHEW(RW_FOR," [RW_FOR]")
   IFSHEW(RW_ONEDEATH," [RW_ONEDEATH]")
   IFSHEW(RW_COMMENT," [RW_COMMENT]")
   IFSHEW(RW_MODSTATS," [RW_MODSTATS]")
   IFSHEW(RW_ECHO," [RW_ECHO]")
   IFSHEW(RW_TRIM," [RW_TRIM]")
#endif
#if defined(SURE)||defined(PAWS)
   IFSHEW(RW_SHOW," [RW_SHOW]")
   IFSHEW(RW_READ," [RW_READ]")
   IFSHEW(RW_SAVE," [RW_SAVE]")
   IFSHEW(RW_RUN," [RW_RUN]")
   IFSHEW(RW_EXIT," [RW_EXIT]")
   IFSHEW(RW_GET," [RW_GET]")
   IFSHEW(RW_HELP," [RW_HELP]")
   IFSHEW(RW_SCAN," [RW_SCAN]")
   IFSHEW(RW_ZOOM," [RW_ZOOM]")
   IFSHEW(RW_CALC," [RW_CALC]")
   IFSHEW(RW_DISP," [RW_DISP]")
   IFSHEW(RW_LEE," [RW_LEE]")
   IFSHEW(RW_CLR," [RW_CLR]")
   IFSHEW(RW_PLOT," [RW_PLOT]")
   IFSHEW(RW_PLTI," [RW_PLTI]")
   IFSHEW(RW_PLT3D," [RW_PLT3D]")
   IFSHEW(RW_ORPROB," [RW_ORPROB]")
   IFSHEW(RW_INIT," [RW_INIT]")
   IFSHEW(RW_READ0," [RW_READ0]")
   IFSHEW(RW_PRUNE," [RW_PRUNE]")
#endif
   IFSHEW(RW_SQRT," [RW_SQRT]")
   IFSHEW(RW_EXP," [RW_EXP]")
   IFSHEW(RW_LN," [RW_LN]")
   IFSHEW(RW_ABS," [RW_ABS]")
   IFSHEW(RW_SIN," [RW_SIN]")
   IFSHEW(RW_COS," [RW_COS]")
   IFSHEW(RW_TAN," [RW_TAN]")
   IFSHEW(RW_ARCSIN," [RW_ARCSIN]")
   IFSHEW(RW_ARCCOS," [RW_ARCCOS]")
   IFSHEW(RW_ARCTAN," [RW_ARCTAN]")
   IFSHEW(RW_FACT," [RW_FACT]")
#ifdef ASSIST
   IFSHEW(RW_MIN," [RW_MIN]")
   IFSHEW(RW_MAX," [RW_MAX]")
   IFSHEW(RW_SUM," [RW_SUM]")
   IFSHEW(RW_COUNT," [RW_COUNT]")
   IFSHEW(RW_ANY," [RW_ANY]");
   IFSHEW(RW_ALL," [RW_ALL]");
#ifdef FUTURE
   IFSHEW(RW_ROWSUM," [RW_ROWSUM]");
   IFSHEW(RW_COLSUM," [RW_COLSUM]");
   IFSHEW(RW_ROWCOUNT," [RW_ROWCOUNT]");
   IFSHEW(RW_COLCOUNT," [RW_COLCOUNT]");
   IFSHEW(RW_ANYPOS," [RW_ANYPOS]");
   IFSHEW(RW_ALLPOS," [RW_ALLPOS]");
   IFSHEW(RW_OTHER," [RW_OTHER$]");
#endif
#endif
   IFSHEW(RW_SIZE," [RW_SIZE]")
   IFSHEW(RW_COMB," [RW_COMB]")
   IFSHEW(RW_PERM," [RW_PERM]")
   IFSHEW(RW_GAM," [RW_GAM]")
   IFSHEW(RW_FAST," [RW_FAST]")
   IFSHEW(RW_AND," [RW_AND]")
   IFSHEW(RW_OR," [RW_OR]")
   IFSHEW(RW_XOR," [RW_XOR]")
   IFSHEW(RW_NOT," [RW_NOT]")
   IFSHEW(RW_QUO," [RW_QUO]")
   IFSHEW(RW_MOD," [RW_MOD]")
   IFSHEW(RW_CYC," [RW_CYC]")
#ifdef ASSIST
   IFSHEW(RW_BY," [RW_BY]")
   IFSHEW(RW_THEN," [RW_THEN]")
   IFSHEW(RW_ELSE," [RW_ELSE]")
#ifdef FUTURE
   IFSHEW(RW_ELSIF," [RW_ELSIF]")
#endif
   IFSHEW(RW_ENDIF," [RW_ENDIF]")
   IFSHEW(RW_ENDFOR," [RW_ENDFOR]")
   IFSHEW(RW_WITH," [RW_WITH]")
   IFSHEW(RW_IN," [RW_IN]")
   IFSHEW(RW_OF," [RW_OF]")
#endif
#if defined(SURE)||defined(PAWS)
   IFSHEW(RW_TO," [RW_TO]")
   IFSHEW(RW_INTO," [RW_INTO]")
   IFSHEW(RW_UPTO," [RW_UPTO]")
   IFSHEW(RW_DOWNTO," [RW_DOWNTO]")
#endif
#ifdef ASSIST
   IFSHEW(RW_ARRAY," [RW_ARRAY]")
   IFSHEW(RW_ON," [RW_ON]")
   IFSHEW(RW_OFF," [RW_OFF]")
   IFSHEW(RW_FULL," [RW_FULL]")
#ifdef FUTURE
   IFSHEW(RW_INTEGER," [RW_INTEGER]")
   IFSHEW(RW_REAL," [RW_REAL]")
#endif
   IFSHEW(RW_BOOLEAN," [RW_BOOLEAN]")
#endif
   IFSHEW(RW_NULL," [RW_NULL]")
   if (!okay) fprintf(bugfile," [unknown rwtype]");
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
#if !defined(PRODUCTION)
void shew_logical_token(token tok)
{
   /*-------------------------------------------------------*/
   if (tok==TK_NULL)          fprintf(bugfile,"    TK_NULL:");
   else if (tok==TK_ID)       fprintf(bugfile,"      TK_ID:");
   else if (tok==TK_MACPARM)  fprintf(bugfile," TK_MACPARM:");
   else if (tok==TK_BOOL)     fprintf(bugfile,"    TK_BOOL:");
   else if (tok==TK_CHAR)     fprintf(bugfile,"    TK_CHAR:");
   else if (tok==TK_INT)      fprintf(bugfile,"     TK_INT:");
   else if (tok==TK_REAL)     fprintf(bugfile,"    TK_REAL:");
   else if (tok==TK_BOOL_EQ)  fprintf(bugfile," TK_BOOL_EQ:");
   else if (tok==TK_BOOL_NE)  fprintf(bugfile," TK_BOOL_NE:");
   else if (tok==TK_EQ)       fprintf(bugfile,"      TK_EQ:");
   else if (tok==TK_NE)       fprintf(bugfile,"      TK_NE:");
   else if (tok==TK_LT)       fprintf(bugfile,"      TK_LT:");
   else if (tok==TK_GT)       fprintf(bugfile,"      TK_GT:");
   else if (tok==TK_LE)       fprintf(bugfile,"      TK_LE:");
   else if (tok==TK_GE)       fprintf(bugfile,"      TK_GE:");
   else if (tok==TK_NOT)      fprintf(bugfile,"     TK_NOT:");
   else if (tok==TK_AND)      fprintf(bugfile,"     TK_AND:");
   else if (tok==TK_OR)       fprintf(bugfile,"      TK_OR:");
   else if (tok==TK_XOR)      fprintf(bugfile,"     TK_XOR:");
   else if (tok==TK_NEG)      fprintf(bugfile,"     TK_NEG:");
   else if (tok==TK_INC_1)    fprintf(bugfile,"   TK_INC_1:");
   else if (tok==TK_DEC_1)    fprintf(bugfile,"   TK_DEC_1:");
   else if (tok==TK_ADD)      fprintf(bugfile,"     TK_ADD:");
   else if (tok==TK_SUB)      fprintf(bugfile,"     TK_SUB:");
   else if (tok==TK_MUL)      fprintf(bugfile,"     TK_MUL:");
   else if (tok==TK_DVD)      fprintf(bugfile,"     TK_DVD:");
   else if (tok==TK_MOD)      fprintf(bugfile,"     TK_MOD:");
   else if (tok==TK_CYC)      fprintf(bugfile,"     TK_CYC:");
   else if (tok==TK_QUO)      fprintf(bugfile,"     TK_QUO:");
   else if (tok==TK_POW)      fprintf(bugfile,"     TK_POW:");
   else if (tok==TK_CONCAT)   fprintf(bugfile,"  TK_CONCAT:");
   else if (tok==TK_COMMA)    fprintf(bugfile,"   TK_COMMA:");
   else if (tok==TK_SEMI)     fprintf(bugfile,"    TK_SEMI:");
   else if (tok==TK_LP)       fprintf(bugfile,"      TK_LP:");
   else if (tok==TK_RP)       fprintf(bugfile,"      TK_RP:");
   else if (tok==TK_LB)       fprintf(bugfile,"      TK_LB:");
   else if (tok==TK_RB)       fprintf(bugfile,"      TK_RB:");
   else if (tok==TK_AT)       fprintf(bugfile,"      TK_AT:");
   else if (tok==TK_COLON)    fprintf(bugfile,"   TK_COLON:");
   else if (tok==TK_SUBRNGE)  fprintf(bugfile," TK_SUBRNGE:");
   else if (tok==TK_RW)       fprintf(bugfile,"      TK_RW:");
   else if (tok==TK_EOF)      fprintf(bugfile,"     TK_EOF:");
   else                       fprintf(bugfile,"  (unknown):");
   /*-------------------------------------------------------*/
}
#endif
/*===========================================================================*/
short print_idinfo(FILE *fp,identifier_info_type *idinfo,int col0,int indent)
{
   object_table_type ooo;
   char *msg;
   Boolean is_array,is_scalar,is_double;
   void *vvv;
   short iii,jjj,kkk,mmm,nnn,col,www,wix,extra;
   char symbolic[90];
   /*-------------------------------------------------------*/
   col=col0;
   extra=0;
   DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
   is_scalar = (Boolean) (!is_array);
   fprintf(fp,"%c",OPENPAREN);
   ++col;
   if (bitset(debug_flagword,DBG_XEQ)){
      col += (3+HEX_DEC_PLACES);
      fprintf_hex_pointer(fp,(*idinfo).ptr.vvv);
      fprintf(fp,"-->");}
   if ((*idinfo).ptr.vvv==((void *) NULL)){
      fprintf(fp,"NULL"); col += 4;}
   else{
      ooo = cvt_to_object_table_type((*idinfo).flags);
      mmm=nnn=1;
      if (is_array && (ooo!=OBJ_SOFF_DATA)){
         mmm = dim_range((*idinfo).index.dims.first);
         nnn = dim_range((*idinfo).index.dims.second);}
      if (is_array && (ooo!=OBJ_SOFF_DATA)){
         fprintf(fp,"%c",OPENBRACKET); ++col;}
      for (www=kkk=iii=0;iii<mmm;++iii){
         if (iii>0) {
            fprintf(fp,","); ++col;
            if ((col+nnn*www)>88){
               col=indent; fprintf(fp,"\n"); ++extra;
               for (wix=0;wix<indent;++wix) fprintf(fp," ");}}
         if (is_double && (ooo!=OBJ_SOFF_DATA)){
            fprintf(fp,"%c",OPENBRACKET); ++col;}
         for (jjj=0;jjj<nnn;++jjj,++kkk){
            if (jjj>0) {fprintf(fp,","); ++col;}
            vvv=arr_addr((*idinfo).ptr.vvv,kkk,ooo);
            msg=enk_aux_val(vvv,ooo);
            www=strlen(msg);
            if ((col+www)>88){
               col=www+indent; fprintf(fp,"\n"); ++extra;
               for (wix=0;wix<indent;++wix) fprintf(fp," ");}
            else col += www;
            fprintf(fp,"%s",msg);}
         if (is_double && (ooo!=OBJ_SOFF_DATA)) fprintf(fp,"%c",CLOSEBRACKET);}
      if (is_array && (ooo!=OBJ_SOFF_DATA)) fprintf(fp,"%c",CLOSEBRACKET);}
   if (col>40){
      col=indent; fprintf(fp,"\n"); ++extra;
      for (wix=0;wix<indent;++wix) fprintf(fp," ");}
   if (is_scalar) {fprintf(fp,",SCALAR"); col+=7;}
   else if (is_double)
      fprintf(fp,",ARRAY[%d..%d,%d..%d]",
                   (int) (*idinfo).index.dims.first.lower,
                   (int) (*idinfo).index.dims.first.upper,
                   (int) (*idinfo).index.dims.second.lower,
                   (int) (*idinfo).index.dims.second.upper);
   else
      fprintf(fp,",ARRAY[%d..%d]",
                   (int) (*idinfo).index.dims.first.lower,
                   (int) (*idinfo).index.dims.first.upper);
   fprintf(fp,",%d",(int) (*idinfo).scope_level);
   fprintf(fp,",\"%s\"",(*idinfo).name);
   if (bitset(debug_flagword,DBG_XEQ))
      fprintf(fp,",0x%02x",(unsigned int) (*idinfo).flags);
   make_symbolic_type_list(symbolic,(*idinfo).flags);
   fprintf(fp,",[%s]%c",symbolic,CLOSEPAREN);
   /*-------------------------------------------------------*/
   return(extra);
}
/*===========================================================================*/
void pnt_pnt_id_tab_msg
     (FILE *fp,Memix iii,identifier_info_type *idinfo,Boolean map_format)
{
   char msg[44];
   int nch;
   short extra;
   /*-------------------------------------------------------*/
   if (map_format){
      if (fp==listfile) list_page_need(1);
      sprintf(msg,"   <%3d> = ",(int) iii);}
   else sprintf(msg,">>>identifier_table[%d]=",(int) iii);
   fprintf(fp,msg); nch=strlen(msg);
   extra=print_idinfo(fp,idinfo,nch,nch+2);
   fprintf(fp,"\n");
   list_page_need(extra);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void print_identifier_table(FILE *fp,Boolean map_format)
{
   identifier_info_type *idinfo;
   Memix iii;
   /*-------------------------------------------------------*/
   idinfo=identifier_table;
   for (iii=0;iii<nele_identifier_table;++iii,++idinfo)
      pnt_pnt_id_tab_msg(fp,iii,idinfo,map_format);
   idinfo=identifier_table+
          (iii=max_identifier_table_count-nele_ident_at_table_top);
   for (;iii<max_identifier_table_count;++iii,++idinfo)
      pnt_pnt_id_tab_msg(fp,iii,idinfo,map_format);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void show_rule_info_after_error
     (identifier_info_type *idinfo, /* NULL if N/A */
      int_type index1,              /* ignored unless "idinfo" is for array */
      int_type index2,              /* ignored unless "idinfo" is for array */
      void *value,                  /* ignored when "idinfo" is NULL */
      char *what,                   /* NULL if no "what on line #" message */
      short linnum,                 /* ignored if "what" is NULL */
      state_bitstring_type bits)    /* NULL if no state to print out */
/*
   This function assumes that "value" is a number and not a state offset.
*/
{
   static char xmsg[513];
   char *enkmsg;
   int_type ival;
   Boolean somewhat,somebits,is_array,is_double;
   char lll[24],rrr[24];
   /*-------------------------------------------------------*/
   *xmsg='\0';
   somewhat = (Boolean) (what!=((char *) NULL));
   somebits = (Boolean) (bits!=((state_bitstring_type) NULL));
   if (idinfo!=((identifier_info_type *) NULL)){
      DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
      if (is_array){
         *rrr='\0';
         if (index1 == WILD_INDEX) strcpy(lll,"*");
         else if (index1 == NULL_INDEX) *lll = '\0';
         else sprintf(lll,"%d",(int) index1);
         if (is_double){
            if (index2 == WILD_INDEX) strcpy(rrr,",*");
            else if (index1 == NULL_INDEX) strcpy(rrr,",");
            else sprintf(rrr,",%d",(int) index2);}
         sprintf(xmsg,"%s[%s%s]",(*idinfo).name,lll,rrr);}
      else
         sprintf(xmsg,"%s",(*idinfo).name);
      if (value != NULL){
         enkmsg=enk_aux_val(value,(*idinfo).flags);
         strcat(xmsg,"=");
         strcat(xmsg,enkmsg);}
      if (somewhat || somebits) strcat(xmsg,", ");}
   if (somewhat){
      strcat(xmsg,STRLP);
      strcat(xmsg,what);
      strcat(xmsg," on line ");
      ival = linnum;
      if (ival<0) ival=(-ival);
      enkmsg=enk_aux_val(&ival,INT_TYPE);
      strcat(xmsg,enkmsg);
      strcat(xmsg,STRRP);
      if (somebits) strcat(xmsg,", ");}
   wrapping_message_on_both(xmsg);
   if (somebits){
      enkmsg=encode_space_bits(bits);
      wrapping_message_on_both(enkmsg);}
   both_printf("\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
token gettoken(rwtype *rw,char *id,short *linnum,short *pos)
{
   token anstok;
   /*-------------------------------------------------------*/
   anstok=gettoken_work(rw,id,linnum,pos);
   last_token_gotten_from_linnum=(*linnum);
   last_token_gotten_from_pos=(*pos);
   /*-------------------------------------------------------*/
   return(anstok);
}
/*===========================================================================*/
