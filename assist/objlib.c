/*
===============================================================================

        Binary object file handling functions required by ASSIST and SURE
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

#include "common.h"

#define ALIGN_STORAGE
#include "objdefs.h"
#include "objtypes.h"
#include "objvarsP.h"

#include "astr_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "fix_ext.h"
#include "obj_ext.h"

#ifdef BUGGY
   /*??????*/ expression_type *expr_arr;
#endif
/*===========================================================================*/

                /***************************************/
                /***************************************/
                /****                               ****/
                /****   LOW-LEVEL PARSING SECTION   ****/
                /****                               ****/
                /***************************************/
                /***************************************/

/*===========================================================================*/
void init_objlib()
/*
   This function initializes variables needed by the lex scanner library.
   Call this routine first.
*/
{
   object_table_type ooo;
   /*-------------------------------------------------------*/
   for (ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo) memory_sort_order[ooo]=ooo;
   /**/
   memory_sort_order[1] = OBJ_REAL_DATA;
   memory_sort_order[2] = OBJ_INT_DATA;
   memory_sort_order[3] = OBJ_BOOL_DATA;
   memory_sort_order[4] = OBJ_SOFF_DATA;
   memory_sort_order[5] = OBJ_CHAR_DATA;
   /**/
   memory_sort_order[1+COMPUTATIONAL_TYPE_COUNT] = OBJ_REAL_VARDATA;
   memory_sort_order[2+COMPUTATIONAL_TYPE_COUNT] = OBJ_INT_VARDATA;
   memory_sort_order[3+COMPUTATIONAL_TYPE_COUNT] = OBJ_BOOL_VARDATA;
   memory_sort_order[4+COMPUTATIONAL_TYPE_COUNT] = OBJ_SOFF_VARDATA;
   memory_sort_order[5+COMPUTATIONAL_TYPE_COUNT] = OBJ_CHAR_VARDATA;
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void re_init_objlib()
/*
   This function initializes quantities for lexical scanning of another
   input file.
*/
{
   object_table_type ooo;
   /*-------------------------------------------------------*/
   for (ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo)
      memory_bytes_required[ooo] = ((memsize_t) 0);
   rewrite_object_header();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Memix cum_aligned_object_header()
{
   Memix cumaddr;
   register short ooo;
   /*-------------------------------------------------------*/
#ifdef OBSOLETE
   lastooo=(-1);
#endif
   for (cumaddr=ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo){
#ifdef MUST_ALIGN
#ifdef OBSOLETE
       hold=cumaddr;
#endif
       PAD_REQUIRED(cumaddr,align_boundaries[ooo]);
#ifdef OBSOLETE
       if ((lastooo>=0) && (cumaddr>hold))
           memory_bytes_required[lastooo] += (cumaddr-hold);
       if (memory_bytes_required[ooo]>0) lastooo=ooo;
#endif
#endif
       cumaddr += memory_bytes_required[ooo];}
   /*-------------------------------------------------------*/
   return(cumaddr);
}
/*===========================================================================*/
void rewrite_object_header()
{
   real_type trash;
   /*-------------------------------------------------------*/
   store_in_object_file(OBJ_EOF,&trash,0,0);
   /*--------------------------------------  rewrite the header */
   rewind(objectfile);
   store_in_object_file(OBJ_HEADER,memory_bytes_required,
                        sizeof(memsize_t),MAX_OBJ_COUNTER_DIM);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void load_obj_file_allocating_memory(Boolean ignore_modfile_stuff)
{
   object_table_type objtyp,ooo,oooivt;
   memsize_t amt;
   size_t siz;
   Memix count;
   Memix cumaddr;
   Boolean again;
   char *data_ptr;
   char *allstorage;
   char *ppp,*pppivt;
#ifdef BUGGY
expression_type *texp;
#endif
   long j;
   relative_address_type reloff[MAX_OBJ_COUNTER_DIM];
   char tmpmsg[42];
   /*-------------------------------------------------------*/
   sure_declarations = sure_tail_buffer = ((char *) NULL);
   allstorage = ((char *) NULL);
   sys_memset(reloff,(int) 0,
              (sizeof(relative_address_type)*MAX_OBJ_COUNTER_DIM));
   objectfile=open_binary_input_file(object_filename);
#ifdef BUGGY
objtyp=0x00;
#endif
   for (again=TRUE;again;){
      /*--------------------------------------   read next table header */
      fetch_table_front(&objtyp,&count,&siz);
      /*--------------------------------------   point to data area */
      if (objtyp==OBJ_EOF) again=FALSE;
      else{
         /*------------------------------   point to data area */
         data_ptr = ((char *) NULL);
         if (objtyp==OBJ_HEADER)
            data_ptr = (char *) memory_bytes_required;
         else if (objtyp==OBJ_IDTABLE){
#if defined(BUGGY)||defined(ALLOC_BUGGY)
fprintf(bugfile,"IDTABLE: ");
#endif
            identifier_table = (identifier_info_type *)
                                   g_malloc(count*((long) siz));
            g_malloc_abort_if_error((void *) identifier_table);
            data_ptr = (char *) identifier_table;}
#ifdef OBSOLETE
/*.....*/         else if (objtyp==OBJ_NUMTABLE)
/*.....*/#if defined(BUGGY)||defined(ALLOC_BUGGY)
/*.....*/{
/*.....*/fprintf(bugfile,"   NUMTABLE: ");
/*.....*/#endif
/*.....*/            data_ptr = constant_table = (char *)
/*.....*/                 g_malloc(count*((long) siz));
/*.....*/#if defined(BUGGY)||defined(ALLOC_BUGGY)
/*.....*/}
/*.....*/#endif
#endif
         else if (objtyp==OBJ_OPREC)
            data_ptr = (char *) &option_rec;
         else if (objtyp==OBJ_VERBATIM_HEAD){
            data_ptr = (char *) g_malloc(1L+count*((long) siz));
            g_malloc_abort_if_error((void *) data_ptr);}
         else if (objtyp==OBJ_VERBATIM_TAIL){
            data_ptr = sure_tail_buffer = (char *)
                           g_malloc(1L+count*((long) siz));
            g_malloc_abort_if_error((void *) data_ptr);}
         else if (objtyp >= MAX_OBJ_COUNTER_DIM){
            sprintf(tmpmsg,"BAD \".aobj\" FILE (objtyp=0x%02x)",
                            (unsigned int) objtyp);
            show_err_on_both(E_INTERNAL,tmpmsg,CHAR_TYPE);
            abort_program();}
         else{
            data_ptr = section_start_addrs[objtyp] + reloff[objtyp];
            reloff[objtyp] += ((Memix) count)*((Memix) siz);}
         if (data_ptr==((char *) NULL)){
            show_err_on_both(E_NOMEMORYR,"(data_ptr)",CHAR_TYPE);
            abort_program();}
#ifdef BUGGY
fprintf(bugfile,"   count=%ld,size=%ld\n",(long) count,(long) siz);
#endif
         /*------------------------------   load from file to data area */
         for (ppp=data_ptr,j=0L;j<count;++j,ppp += ((Memix) siz))
            fread(ppp,(size_t) siz,(size_t) 1,objectfile);
#ifdef BUGGY
fprintf(bugfile,"   count=%ld,size=%ld\n",(long) count,(long) siz);
#endif
         /*------------------------------   adjust/fixup data */
         if (objtyp==OBJ_IDTABLE) fixup_identifier_table(count);
         else if (objtyp==OBJ_VERBATIM_HEAD){
            if (modfile == ((FILE *) NULL)) sure_declarations = data_ptr;
            else{
               if (!ignore_modfile_stuff) fprintf(modfile,"%s",data_ptr);
               g_free(data_ptr);}}
         else if (objtyp==OBJ_HEADER){
            amt = cum_aligned_object_header();
#ifdef BUGGY
for (ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo)
fprintf(stderr,"amt[%d]=%ld\n",(int) ooo,(long) memory_bytes_required[ooo]);
fprintf(stderr,"amt=%ld\n",(long) amt);
#endif
#ifdef BUGGY
fprintf(bugfile,"allstorage: ");
#endif
            allstorage = (char *) g_malloc(amt);
            if (allstorage==((char *) NULL)){
               show_err_on_both(E_NOMEMORYR,"(allstorage)",CHAR_TYPE);
               abort_program();}
            set_map_addr0(allstorage);
            for (cumaddr=ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo){
               oooivt = memory_sort_order[ooo];
#ifdef MUST_ALIGN
               PAD_REQUIRED(cumaddr,align_boundaries[oooivt]);
#endif
               pppivt=allstorage+cumaddr;
               section_start_addrs[oooivt] = pppivt;
               cumaddr += memory_bytes_required[oooivt];}
            allvarstorage=section_start_addrs[COMPUTATIONAL_TYPE_COUNT];
            allvarstorage_size=
              section_start_addrs[2*COMPUTATIONAL_TYPE_COUNT]-allvarstorage;}}}
   allvarstorage_copy = (char *) g_malloc(allvarstorage_size);
   g_malloc_abort_if_error((void *) allvarstorage_copy);
   /*--------------------------------------------------------  final fixup */
#ifdef BUGGY
   /*??????*/ expr_arr = (expression_type *) section_start_addrs[OBJ_EXPR];
/*???????*/ fprintf(stderr,"POINT OA: %08x (%08x)\n",(int) expr_arr,(int) (*expr_arr).postfix_ops);
#endif
   fixup_expressions();
   fixup_user();
   /*--------------------------------------------------------  listing map */
#if (!defined(STANDALONE))
   if (memory_map) logfile_storage_map(allstorage);
#endif
   /*-------------------------------------------------------*/
}    
/*===========================================================================*/
void free_object_file_memory()
{
   object_table_type ooo;
   /*-------------------------------------------------------*/
   g_free(allvarstorage_copy);
   g_free(section_start_addrs[0]);
   for (ooo=0;ooo<MAX_OBJ_COUNTER_DIM;++ooo)
      section_start_addrs[ooo] = ((char *) NULL);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void fetch_table_front(object_table_type *objtyp,Memix *nnn,size_t *siz)
{
   int_type ival;
   /*-------------------------------------------------------*/
   fread((char *) &ival,sizeof(int_type),1,objectfile);
   *objtyp = (object_table_type) ival;
   if ((*objtyp)==OBJ_EOF) return;
   fread((char *) &ival,sizeof(int_type),1,objectfile);
   *nnn = (long) ival;
   fread((char *) &ival,sizeof(int_type),1,objectfile);
   *siz = (size_t) ival;
#ifdef BUGGY
/*????????*/ fprintf(bugfile,">>> table_front -- %02x nnn=%d siz=%d\n",(int) *objtyp,(int) *nnn,(int) *siz);
#endif
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
void store_table_front(object_table_type objtyp,Memix nnn,size_t siz)
{
   int_type ival;
   /*-------------------------------------------------------*/
   ival=(int_type) objtyp;
   fwrite((char *) &ival,sizeof(int_type),1,objectfile);
   ival=(int_type) nnn;
   fwrite((char *) &ival,sizeof(int_type),1,objectfile);
   ival=(int_type) siz;
   fwrite((char *) &ival,sizeof(int_type),1,objectfile);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
relative_address_type store_in_object_file
     (object_table_type objtyp,void *data_ptr,size_t siz,long count)
{
   relative_address_type ans;
   char *ppp;
   long j;
#ifdef BUGGY
File_offset_t pos,opos,diff;
#endif
   /*-------------------------------------------------------*/
   ans=NIX_ADDR;
   if (objtyp<MAX_OBJ_COUNTER_DIM){
      ans=memory_bytes_required[objtyp];
      memory_bytes_required[objtyp] +=
          (((memsize_t) siz)*((memsize_t) count));}
#ifdef BUGGY
opos=ftell(objectfile);
#endif
   store_table_front(objtyp,count,(size_t) siz);
   for (ppp=data_ptr,j=0L;j<count;++j,ppp += ((Memix) siz))
      fwrite(ppp,(size_t) siz,(size_t) 1,objectfile);
#ifdef BUGGY
pos=ftell(objectfile);
diff = pos-opos;
if (diff !=
(File_offset_t) (3*sizeof(int_type)+(siz*((File_offset_t) count)))){
fprintf(bugfile,"pos=%ld  opos=%ld  diff=%ld\n",
(long) pos,(long) opos,(long) diff);
show_err_on_both(E_INTERNAL,
"(object file not open in binary mode)",CHAR_TYPE);
abort_program();}
#endif
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
object_table_type cvt_to_object_table_type(type_flagword_type type)
{
   object_table_type ans;
   type_flagword_type comp;
   /*-------------------------------------------------------*/
   comp = type & COMPUTATIONAL_TYPE_MASK;
   if (bitset(type,SSVAR_TYPE)) ans = OBJ_SOFF_DATA;
   else ans = (object_table_type) comp;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
