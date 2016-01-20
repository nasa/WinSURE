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
              10/01  BUG FIX BY RWB line 603: fixex bug where certain 
                     death states were not properly aggregated   

===============================================================================
*/

#include "common.h"

#include "objdefs.h"
#include "objtypes.h"
#include "objvars.h"

#include "astdefs.h"
#include "asttypes.h"
#include "astvars.h"

#include "astr_ext.h"
#include "io_ext.h"
#include "alex_ext.h"
#include "obj_ext.h"
#include "mac_ext.h"
#include "aprs_ext.h"
#include "agen_ext.h"

#define XTRFRONT 1  /* extra number of special states in "state_storage" */
#define BYTES_PER_PACKED_INTEGER 3

#define SLOW 0x01
#define FAST 0x02
#define BOTH_FAST_AND_SLOW 0x03
/*===========================================================================*/
/*
   In addition to packing/unpacking the integer beginning at the byte pointed
   to by "ppp", these macros also adjust the pointer "ppp" to point past the
   packed integer upon completion.   The pointer "ppp" should always be a
   copy of the original pointer.
*/
#define extract_packed_integer(ppp,big) \
        big = (*ppp++); \
        big = big << BITS_PER_BYTE; big += (*ppp++); \
        big = big << BITS_PER_BYTE; big += (*ppp++)
#define insert_packed_integer(ppp,big) \
        *(ppp+2) = (unsigned char) (big & (*(masks+BITS_PER_BYTE))); \
        *(ppp+1) = (unsigned char) ((big >> BITS_PER_BYTE) & \
                                    (*(masks+BITS_PER_BYTE))); \
        *ppp = (unsigned char) ((big >> (BITS_PER_BYTE+BITS_PER_BYTE)) & \
                                (*(masks+BITS_PER_BYTE))); \
        ppp += BYTES_PER_PACKED_INTEGER
/*===========================================================================*/
#ifdef BUGGY
static FILE *debugfile;
#endif
static double ddpow,ddpowlim;
static short longs_per_double = sizeof(double)/sizeof(long);
static short comment_maxleng;
/**/
static state_bitstring_type current_state,future_state;
static Memix current_statenum,future_statenum;
/**/
static Memix next_free_extension_link;
static Memix table_element_size;
static Memix main_table_bucket_size;
static Memix extension_table_bucket_size;
static long main_storage_size,total_storage_size,total_bits_size;
static long realloc_more_size,realloc_more_bits_size;
/**/
static Memix state_byte_count,state_bit_count;
/**/
static Boolean some_transitions_printed,final_trimming,any_trim0,*any_prune;
/**/
static long next_unique_state_counter,death0,prune0,state0,trim0;
static long trimdeath,prunedeath0;
static long deathovf,pruneovf,prunedeathovf;
static long aggr_deathknt,aggr_pruneknt,aggr_trimknt,state_count,tran_count;
static Memix ixassert,ixdeath,ixprune;
static Memix ix_adp; /* index for assert/deathif/pruneif */
static Memix start_statenum;
/**/
static long transition_count;
static long trans_count_every;
#define TRANS_COUNT_EVERY 1000
#define TRANS_COUNT_BIGEVERY 10000
/*----------------------------------------  global pointers */
static unsigned char *state_storage;
static unsigned char *bucket_storage;
static unsigned char *bucket_extension_storage;
static unsigned char *bucket_extension_ovf;
static unsigned char *next_free_extension_bucket;
/*===========================================================================*/
/*
   Table entry:

              |count|link||statenum|statenum|...|statenum|

   Extension table entry:

                    |link||statenum|statenum|...|statenum|

*/
/*===========================================================================*/
#define next_unique_state_number() next_unique_state_counter++
/*===========================================================================*/
void init_hash_function()
{
   identifier_info_type *idinfo,*idovf;
   short n,m,neach;
   long bytes,bytes0,bytes1;
   long statenum;


   //   register unsigned short jjj; // RWB

   /*-------------------------------------------------------*/
#ifdef BUGGY
debugfile=bugfile;
#endif
   trimming_is_on = (Boolean) (option_rec.trim>0);
   idovf=identifier_table+nele_identifier_table;
   any_trim0 = FALSE;
   if (pruneif_count <= 0) any_prune = (Boolean *) NULL;
   else{
       any_prune = (Boolean *) g_malloc(pruneif_count*sizeof(Boolean));
       g_malloc_abort_if_error((void *) any_prune);}
   /*-------------------------------------  sum lengths in state bit string */
   state_bit_count=0;
   for (idinfo=identifier_table;idinfo<idovf;++idinfo){
       if (bitset((*idinfo).flags,SSVAR_TYPE)){
          n = dim_range((*idinfo).index.dims.first);
          m = dim_range((*idinfo).index.dims.second);
          neach = (*(*idinfo).ptr.sss).bit_length;
          state_bit_count += (n*m*neach);}}
   /*-------------------------------------  round bit count up to byte count */
   if (option_rec.comment){
      comment_maxleng=max_encoded_space_bits_strlen();
      if ((comment_maxleng+3*MAX_INT_TYPE_NDIGS)>MAXNCH_MODEL_TRANSITION_LINE){
         warn_this_pos(E_COMMENTTURNEDOFF,DEFAULT_WARN_LEV);
         option_rec.comment=FALSE;}}
   /*-------------------------------------  round bit count up to byte count */
   n = (short) ((option_rec.onedeath>0)
                ? (state_bit_count)
                : (state_bit_count+1));
   state_byte_count = 1+(n-1)/8;
   if (state_byte_count<1) state_byte_count=1;
   if (state_byte_count>STRING_MAXNCH){
      sprintf(scratch_string_buffer,
              "Number of bits (%d) in state space exceeds limit (%d)",
              (int) state_bit_count,(int) (BITS_PER_BYTE * STRING_MAXNCH));
      show_err_at_pos_on_both(E_INTERNAL,0,scratch_string_buffer,CHAR_TYPE);
      abort_program();}
   /*-------------------------------------  initialize miscellanous stuff */
   ddpow=exp(((double) (BITS_PER_BYTE))*log(2.0000000000000));
   ddpowlim=(MAX_REAL_TYPE-ddpow)/ddpow;
   /*-------------------------------------  compute lumping state numbers */
   tran_count=aggr_deathknt=aggr_pruneknt=0;
   trimdeath_count=0;
   statenum = 1;
   /*------------------- death first */
   death0 = statenum;
   if (option_rec.onedeath) statenum += deathif_count;
   else deathovf = death0 = (-999);
   /*------------------- prune death next */
   prunedeath0 = statenum;
   statenum += pruneif_count;
   /*------------------- trim death next */
   if (trimming_is_on){
      trimdeath = statenum++;
      trimdeath_count = 1;}
   /*------------------- prune states next */
   if (trimming_is_on && (option_rec.trim==2)){
      prune0 = statenum;
      statenum += pruneif_count;}
   else prune0 = prunedeath0;
   /*------------------- trim state next */
   if (trimming_is_on) trim0 = statenum++;
   /*------------------- start state next */
   state0 = statenum;
   /*------------------- total prune count */
   total_prune_count = pruneif_count + trimdeath_count;
   /*------------------- compute loop terminating state numbers */
   if (option_rec.onedeath) deathovf = death0 + deathif_count;
   pruneovf = prune0 + pruneif_count;
   prunedeathovf = prunedeath0 + pruneif_count;
   next_unique_state_counter = state0;
   /*------------------- print state numbers to debug file */
   if (bitset(debug_flagword,(DBG_XEQ|DBG_XPN))){
      fprintf(bugfile,
              "option_rec.(onedeath=%d,trim=%d,comment=%d,stats=%d,echo=%d)\n",
              (int) option_rec.onedeath,
              (int) option_rec.trim,
              (int) option_rec.comment,
              (int) option_rec.comment_stats,
              (int) option_rec.echo);
      fprintf(bugfile,"trimming_is_on = %s\n",
                      (trimming_is_on ? "TRUE" : "FALSE"));
      fprintf(bugfile,"death0=(%d)\n",(int) death0);
      if (trimming_is_on){
         fprintf(bugfile,"trim0=(%d)\n",(int) trim0);
         fprintf(bugfile,"trimdeath=(%d)\n",(int) trimdeath);
         fprintf(bugfile,"prunedeath0=(%d)\n",(int) prunedeath0);}
      fprintf(bugfile,"prune0=(%d)\n",(int) prune0);
      fprintf(bugfile,"state0=(%d)\n",(int) state0);}
   /*-------------------------------------  compute table sizes */
   table_element_size =
       (BYTES_PER_PACKED_INTEGER);
   extension_table_bucket_size = 
       BYTES_PER_PACKED_INTEGER + (bucket_width * table_element_size);
   main_table_bucket_size =
       BYTES_PER_PACKED_INTEGER + extension_table_bucket_size;
   extension_bucket_count = EXTENSION_BUCKET_COUNT;
   realloc_more_size =
        ((long) REALLOC_BUCKETS_AT_A_TIME) *   /* buckets */
        ((long) extension_table_bucket_size);  /* bytes/bucket */
   realloc_more_bits_size =
        ((long) REALLOC_BUCKETS_AT_A_TIME) *   /* buckets */
        ((long) bucket_width) *                /* states/bucket */
        ((long) state_byte_count);             /* bytes/state */
   /*-------------------------------------  compute amount of memory needed */
   bytes = bytes0 =
           (((long) main_table_bucket_size) *   /* bytes/bucket */
            ((long) bucket_count));             /* buckets */
   bytes1 = ((long) state_byte_count) *         /* bytes/state */
            (((long) XTRFRONT) +                /* extra states at front */
             ((long) bucket_width) *            /* states/bucket */
             ((long) (bucket_count+extension_bucket_count))); /* buckets */
#ifdef MSDOS
   /* cannot realloc() with halloc on MSDOS so we must grab all we can get
      while the grabbing is good */
   //   extension_bucket_count = (MSDOS_HUGE_BUFFER_BYTES - bytes - bytes1) /
   //                            extension_table_bucket_size;
   //   if (MSDOS_HUGE_BUFFER_BYTES < (bytes+bytes1)){
   //      fprintf(stderr,"Cannot grab negative number of bytes\n");
   //      error_this_pos(E_INTERNAL);
   //      abort_program();}
#endif
   bytes += (((long) extension_table_bucket_size) *
            ((long) extension_bucket_count));
   main_storage_size=bytes0;
   total_storage_size=bytes;
   total_bits_size=bytes1;
#if defined(BUGGY)||defined(ALLOC_BUGGY)
fprintf(stderr,"main=%ld, extn=%ld, bytes=%ld, bytes1=%ld\n",
bytes0,(bytes-bytes0),bytes,bytes1);
#endif
   /*-------------------------------------  allocate and divvy up memory */
   bucket_storage = (unsigned char *) g_malloc(bytes);
   if (bucket_storage==((unsigned char *) NULL)) goto insufficient_memory;

   //   for (jjj=0;jjj<bytes/4;++jjj) // RWB
   //     { 
   //     fprintf(stderr,"**** *(bucket_storage+%i = %i\n",
   //             jjj,*(bucket_storage+jjj));
   //     *(bucket_storage+jjj) = 0;
   //
   //     } // RWB

   state_storage = (unsigned char *) g_malloc(bytes1);
   if (state_storage==((unsigned char *) NULL)) goto insufficient_memory;
   next_free_extension_link = 0;
   divvy_up_extension_storage();
   /*-------------------------------------  compute table sizes */
   /*-------------------------------------------------------*/
   return;
insufficient_memory:
#ifdef BUGGY
fprintf(bugfile,
"\ninit_hash_function: bytes=%ld  bytes1=%ld\n",bytes,bytes1);
#endif
   strcpy(this.id,"(init_hash_function)"); error_this_pos(E_NOMEMORYR);
   abort_program();
}
/*===========================================================================*/
void divvy_up_extension_storage()
{
   /*-------------------------------------------------------*/
   future_state = state_storage;
   bucket_extension_storage = bucket_storage + main_storage_size;
   bucket_extension_ovf = bucket_storage + total_storage_size;
   next_free_extension_bucket = bucket_extension_storage +
                                ((Memix) next_free_extension_link) *
                                ((Memix) extension_table_bucket_size);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean state_is_flagged(state_bitstring_type state)
{
   register Boolean ans;
   /*-------------------------------------------------------*/
   if (state==NULL) ans=TRUE;
   else if (option_rec.onedeath) ans = FALSE;
   else ans = test_bit(state,((bitsize_type) state_bit_count));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
long state_hash_index(state_bitstring_type bits)
/*
   This is the hash function which converts a given state "bits" into an
   index into the hash table.
*/
{
   register unsigned short iii,nnn;
   register unsigned long hashix;
   register state_bitstring_type tempbits;
   register double dd;
   register long *bigptr;
   double ddans;
   register unsigned long sum;
   /*-------------------------------------------------------*/
   tempbits = (state_bitstring_type) scratch_string_buffer;
   sys_memcpy(tempbits,bits,state_byte_count);
   clear_byte_tail(tempbits,(bitsize_type) state_bit_count);
   for (iii=0,nnn=(short) state_byte_count,dd=0.000000000000000;iii<nnn;++iii){
#ifdef BUGGY
fprintf(bugfile,"hashing iii=%d/%d  dd=%le\n",(int) iii,(int) nnn,dd);
#endif
      if (dd<ddpowlim) dd = dd * ddpow + ((double) (*(tempbits+iii)));}
   ddans=dd;
   bigptr=((long *) &ddans);
   for (sum=iii=0,nnn=longs_per_double;iii<nnn;++iii){
      sum += (*bigptr);
      ++bigptr;}
   hashix = ((unsigned long) bucket_count);
   hashix = sum % hashix;
#if defined(BUGGY)||defined(HASH_BUGGY)
if (hashix>=((unsigned long) bucket_count)){
fprintf(stderr,
"**** ERROR, Bad bucket index %d mapped to zero.\n",(int) hashix);
error_this_pos(E_INTERNAL);
hashix=0;}
#endif
   /*-------------------------------------------------------*/
   return (hashix);
}
/*===========================================================================*/
unsigned char *link_to_a_brand_new_bucket(unsigned char **link_ptr,Memix *adj)
/*
   This function returns a pointer to the beginning (link) in the new bucket
   which is assigned from the free extension bucket pool.   If no buckets
   in the extension bucket pool are free, then an attempt is made to
   allocate a block of several more free extension buckets to add to the
   pool befolre assigning the next free bucket.   If memory cannot be
   allocated, then an appropriate error message will be displayed and
   processing will terminate without completing the model file.

   Warning:  This routine invokes "realloc" and therefore, if the new
             storage happens to move, the link pointers may no longer
             point correctly.   Pointers should be re-calculated after
             using this function.   The index pointed to by "adj" will
             be set to the offset which must be added to each pointer
             address in order to maintain integrity.
*/
{
   unsigned char *ppp,*oldstg;
   unsigned char *newppp;
   long big;
   size_t old_siz;
   size_t old_bitsiz;
#ifdef BUGGY
   int jjj;
#endif
   /*-------------------------------------------------------*/
   *adj=0;
   if (next_free_extension_bucket >= bucket_extension_ovf){
#ifdef MSDOS
      goto insufficient_memory;
#else
#ifdef BUGGY
      fprintf(stderr,
              "realloc (%08x,%08x) ",
              (int) bucket_storage,
              (int) state_storage);
      for (jjj=0;jjj<state_byte_count;++jjj)
           fprintf(stderr,"%02x ",
                   (unsigned int) ((unsigned char) current_state[jjj]));
#endif
      old_siz = total_storage_size;
      old_bitsiz = total_bits_size;
      oldstg = bucket_storage;
      total_storage_size += realloc_more_size;
      total_bits_size += realloc_more_bits_size;
      bucket_storage = (unsigned char *) 
         g_realloc(bucket_storage,(size_t) total_storage_size,old_siz);
      if (bucket_storage==((unsigned char *) NULL)) goto insufficient_memory;
      state_storage = (unsigned char *)
         g_realloc(state_storage,(size_t) total_bits_size,old_bitsiz);
      if (state_storage==((unsigned char *) NULL)) goto insufficient_memory;
      divvy_up_extension_storage();
      *adj = bucket_storage-oldstg;
      *link_ptr += (*adj);
      current_state = point_to_state(current_statenum);
#ifdef BUGGY
      fprintf(stderr,
              " ---> (%08x,%08x) ",
              (int) bucket_storage,
              (int) state_storage);
      for (jjj=0;jjj<state_byte_count;++jjj)
           fprintf(stderr,"%02x ",
                   (unsigned int) ((unsigned char) current_state[jjj]));
      fprintf(stderr,"\n");
#endif
#endif
      }
   big=next_free_extension_link;
   newppp = next_free_extension_bucket;
   next_free_extension_bucket += extension_table_bucket_size;
   ++next_free_extension_link;
   ppp=(*link_ptr); insert_packed_integer(ppp,big);
   /*-------------------------------------------------------*/
#ifdef BUGGY
fprintf(bugfile,
"\nlinking to brand new bucket: total_storage_size=%ld  total_bits_size=%ld\n",
total_storage_size,total_bits_size);
#endif
   return(newppp);
insufficient_memory:
   fprintf(stderr,"total_storage_size=%d,total_bits_size=%d,old_siz=%d,old_bitsiz=%d\n"
          ,(int) total_storage_size,(int) total_bits_size,
           (int) old_siz,(int) old_bitsiz);
   strcpy(this.id,"(link_to_a_brand_new_bucket)"); error_this_pos(E_NOMEMORYR);
   abort_program();
   return(NULL);
}
/*===========================================================================*/
Memix add_a_new_state(state_bitstring_type state,long bucknum)
{
   Memix next_link,link0,length,ix,adjust;
   unsigned char *ppp,*link_ptr,*count_ptr;
   long big,statenum;
   /*-------------------------------------------------------*/
   ppp = use_bucket(bucknum,&length,&link0);
   link_ptr = ppp-BYTES_PER_PACKED_INTEGER;
   count_ptr = link_ptr-BYTES_PER_PACKED_INTEGER;
   /*-----------------------------------------------  trace bucket */
   next_link = link0;
   for (ix=0;ix<length;++ix){
      /*---------------------------------------  follow the link to entry */
      if ((ix>0)&&((ix%bucket_width)==0)){
         ppp = use_extension_bucket(next_link,&next_link);
         link_ptr = ppp-BYTES_PER_PACKED_INTEGER;}
      /*---------------------------------------  move to next entry */
      ppp += table_element_size;}
   /*-----------------------------------------------  extend bucket if must */
   if ((ix>0)&&((ix%bucket_width)==0)){
      link_ptr =link_to_a_brand_new_bucket(&link_ptr,&adjust);
      ppp = link_ptr + BYTES_PER_PACKED_INTEGER;
      count_ptr += adjust;}
   /*-----------------------------------------------  calc next state number */
   statenum = big = next_unique_state_number();
   /*-----------------------------------------------  add statenum to bucket */
   insert_packed_integer(ppp,big);
   increment_packed_count(count_ptr);
#ifdef BUGGY
fprintf(bugfile,
"addition complete for bucket#%ld, big=%ld\n",
(long) bucknum,(long) big);
show_bucket(bugfile,bucknum);
#endif
   /*-----------------------------------------------  add bit string */
   ppp = point_to_state(statenum);
   sys_memcpy(ppp,state,state_byte_count);
   /*-------------------------------------------------------*/
   return(statenum);
}
/*===========================================================================*/
unsigned char *use_bucket(long bucknum,Memix *count,Memix *nextlink)
/*
   This function returns a pointer to the first byte containing the bit
   string of the first statenum in the bucket indexed by "bucknum".
*/
{
   register unsigned char *ans;
   unsigned long big;
   /*-------------------------------------------------------*/
   ans = bucket_storage + (main_table_bucket_size*bucknum);
   extract_packed_integer(ans,big);
   *count = (Memix) big;
   extract_packed_integer(ans,big);
   *nextlink = (Memix) big;
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/
unsigned char *use_extension_bucket(Memix xbucknum,Memix *nextlink)
{
   unsigned char *ans;
   unsigned long big;
   /*-------------------------------------------------------*/
   ans = bucket_extension_storage + (extension_table_bucket_size*xbucknum);
   extract_packed_integer(ans,big);
   *nextlink = (Memix) big;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
void increment_packed_count(unsigned char *ptr)
{
   unsigned char *ppp;
   long big;
   /*-------------------------------------------------------*/
   ppp = ptr; extract_packed_integer(ppp,big);
   ++big;
   ppp = ptr; insert_packed_integer(ppp,big);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
long extract_packed_value(unsigned char *ptr)
{
   unsigned char *ppp;
   long big;
   /*-------------------------------------------------------*/
   ppp = ptr; extract_packed_integer(ppp,big);
   /*-------------------------------------------------------*/
   return(big);
}
/*===========================================================================*/
void insert_packed_value(unsigned char *ptr,long val)
{
   unsigned char *ppp;
   long big;
   /*-------------------------------------------------------*/
   big=val;
   ppp = ptr; insert_packed_integer(ppp,big);
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#ifdef BUGGY
void show_bucket(FILE *fp,long bucknum)
{
   unsigned char *ppp,*qqq;
   Memix ix,next_link;
   unsigned long big;
   Memix length,link0,statenum;
   /*-------------------------------------------------------*/
   fprintf(fp,"bucket#%ld:\n   ",bucknum);
   ppp = use_bucket(bucknum,&length,&link0);
   /*---------------------------------------------------  search */
   next_link = link0;
   for (ix=0;ix<length;++ix){
      /*---------------------------------------  follow the link to entry */
      if ((ix>0)&&((ix%bucket_width)==0)){
         fprintf(fp," ---> (%ld)\n   ",(long) next_link);
         ppp = use_extension_bucket(next_link,&next_link);}
      /*---------------------------------------  test this entry for match */
      statenum = (Memix) extract_packed_value(ppp);
      fprintf(fp,"%8ld",(long) statenum);
      /*---------------------------------------  move to next entry */
      ppp += table_element_size;}
   fprintf(fp," ---> (%ld)\n",(long) next_link);
   /*-------------------------------------------------------*/
}
#endif

/*===========================================================================*/
state_bitstring_type find_state(state_bitstring_type look,
                                Memix *statenum,long *bucknum)
/*
   This function returns the state which corresponds to the stored state
   matching "look".   If the "look" state is not found, then the NULL pointer
   will be returned.   If the "look" state is found, then "statenum" will
   also be returned.
*/
{
   unsigned char *ppp;
   Memix ix,next_link;
   state_bitstring_type ppptab;
   Memix length,link0;
   Boolean ppptab_death; // RWB


   /*-------------------------------------------------------*/
   *bucknum=state_hash_index(look);
   #ifdef BUGGY
     ppp = (unsigned char *) encode_space_bits(look);
     fprintf(bugfile,
     "looking for ... %s in bucket number %d\n",ppp,(int) *bucknum);
     show_bucket(bugfile,*bucknum);
   #endif

   ppp = use_bucket(*bucknum,&length,&link0);
   /*---------------------------------------------------  search */
   next_link = link0;
   for (ix=0;ix<length;++ix){
      /*---------------------------------------  follow the link to entry */
      if ((ix>0)&&((ix%bucket_width)==0))
         ppp = use_extension_bucket(next_link,&next_link);
      /*---------------------------------------  test this entry for match */
      *statenum = (Memix) extract_packed_value(ppp);
      ppptab = point_to_state(*statenum);

      /* ------------------------------------------------------------------ */
      /*  The following two lines added by Rick Butler on 8/10/01 to        */
      /*  fix bug where certain death states were not properly aggregated   */
      /* ------------------------------------------------------------------ */


      if ( ! option_rec.onedeath) 
         { 
         ppptab_death = (Boolean) state_is_flagged(ppptab);   // RWB
         if (ppptab_death) set_death_state_bit(look);         // RWB
	 else              clear_death_state_bit(look);       // RWB
         }

    #ifdef BUGGY   // RWB
      fprintf(bugfile," ++++++++++++ ppptab_death = %d \n",ppptab_death);
      fprintf(bugfile,"ix, *statenum = %d, %d \n",ix, *statenum);

      for (jjj=0;jjj<state_byte_count;++jjj)
        fprintf(bugfile,"%02x ", (unsigned int) ((unsigned char) ppptab[jjj]));
      fprintf(bugfile," ++++++++++++ ppptab \n");

      for (jjj=0;jjj<state_byte_count;++jjj)
        fprintf(bugfile,"%02x ", (unsigned int) ((unsigned char) look[jjj]));
      fprintf(bugfile," ++++++++++++ look \n");

      show_bucket(bugfile,*bucknum);

      // fprintf(bugfile,"1820 RWB: *ppptab,*look = %x,%x\n",*ppptab,*look);

     #endif      // RWB


      if (sys_memcmp(ppptab,look,state_byte_count)==0) return(ppptab);
      /*---------------------------------------  move to next entry */
      ppp += table_element_size;}
   /*---------------------------------------------------  not found */
   return((unsigned char *) NULL);
   /*-------------------------------------------------------*/
}

/*===========================================================================*/
Boolean booltest_opcode(void *vvv,booltest_type **btest)
{
   Boolean test;
   /*-------------------------------------------------------*/
   *btest = (booltest_type *) vvv;
#ifdef BUGGY
fprintf(bugfile,"btest=%08x\n",(int) btest);
fprintf(bugfile,"*btest=%08x\n",(int) *btest);
fprintf(bugfile,"(**btest).expr=%08x\n",(int) (**btest).expr);
#endif
   test = fast_eval_for_expected_boolean((**btest).expr);
   /*-------------------------------------------------------*/
   return(test);
}
/*===========================================================================*/
#define QQSHW_RULE_AFT_ERR(idi,vptr,isb,jsb,lno) \
        show_rule_info_after_error(idi,isb,jsb,vptr,"TRANTO",lno,NULL)
#define SHOW_RULE_AFT_ERR(vptr,isb,jsb) \
        QQSHW_RULE_AFT_ERR(idinfo,vptr,isb,jsb,(*expr).source_code_line_number)
/*===========================================================================*/
Boolean perform_transition(tranto_clause_type *tto,
                           state_bitstring_type dest_state)
{
   short ixvar,offset;
   expression_type *expr;
   identifier_info_type *idinfo;
   type_flagword_type comp;
   ssvar_value_type value;
   Boolean is_array,is_double;
   state_offset_type offinfo;
   int_type isub,jsub,ival;
   unsigned char *holdeval;
   Boolean state_is_in_error;
   short old_error_count;
   FAST_STATE_OFFSET_REGS();
   /*-------------------------------------------------------*/
#ifdef BUGGY
   if (transition_count>=62590){
       for (ixvar=0;ixvar<state_byte_count;++ixvar)
            fprintf(stderr,"%02x ",
                    (unsigned int) ((unsigned char) current_state[ixvar]));
       fprintf(stderr," ");}
#endif
   old_error_count = error_count;
#ifdef BUGGY
fprintf(bugfile,"perform tranto = %08x = ",(int) tto);
fshew_map_memix(bugfile,tto);
fprintf(bugfile,"\n");
#endif
   holdeval = eval_state_ssbits;
   eval_state_ssbits = current_state;
      sys_memcpy(dest_state,current_state,state_byte_count);
      expr=(*tto).sex.exprs;
      for (ixvar=0;ixvar<(*tto).sex.n_vars;++ixvar){
         idinfo = *((*tto).sex.vars + ixvar);
         FAST_STATE_OFFSET_COPY(&offinfo,(*idinfo).ptr.sss);
         comp = (*idinfo).flags & COMPUTATIONAL_TYPE_MASK;
         DECIDE_IS_ARRAY_IS_DOUBLE(idinfo);
         if (is_array){
             jsub = SIMPLE_IDENTIFIER;
             isub = fast_eval_for_expected_integer(expr);
             if ((isub<(*idinfo).index.dims.first.lower)||
                 (isub>(*idinfo).index.dims.first.upper)){
                error_this_pos(E_DIMOUTOFBOUNDS);
                SHOW_RULE_AFT_ERR(NULL,isub,NULL_INDEX);}
             ++expr;
             if (is_double){
                jsub = fast_eval_for_expected_integer(expr);
                if ((jsub<(*idinfo).index.dims.second.lower)||
                    (jsub>(*idinfo).index.dims.second.upper)){
                   error_this_pos(E_DIMOUTOFBOUNDS);
                   SHOW_RULE_AFT_ERR(NULL,NULL_INDEX,jsub);}
                ++expr;}
             offset = compute_offset(isub,jsub,idinfo);
             offinfo.bit_offset += offset*offinfo.bit_length;}
         else isub=jsub=(*idinfo).index.dims.first.lower;
         if (comp==BOOL_TYPE)
             value=(ssvar_value_type) fast_eval_for_expected_boolean(expr);
         else
             value = (ssvar_value_type) fast_eval_for_expected_integer(expr);
         if (pack_state_space_value_err(value,dest_state,&offinfo)){
            ival = value;
            SHOW_RULE_AFT_ERR(&ival,isub,jsub);}
         ++expr;}
   eval_state_ssbits = holdeval;
   state_is_in_error = (Boolean) (old_error_count < error_count);
#ifdef BUGGY
   if (transition_count>=62590){
       fprintf(stderr,"TRANTO ---> ");
       for (ixvar=0;ixvar<state_byte_count;++ixvar)
            fprintf(stderr,"%02x ",
                    (unsigned int) ((unsigned char) dest_state[ixvar]));
       fprintf(stderr,"\n");}
#ifdef VERY_BUGGY
   if (transition_count>=62590){
       fflush(stdout); fflush(stderr);
       if (dest_state[3] == 0x9d) *((char *) 0)='\0';}
#endif
#endif
   /*-------------------------------------------------------*/
   return (state_is_in_error);
}
/*===========================================================================*/
state_bitstring_type point_to_state(Memix statenum)
{
   register state_bitstring_type ans;
   /*-------------------------------------------------------*/
   if (statenum<state0)
      ans=NULL;
   else
      ans=state_storage+state_byte_count*((Memix) (statenum-state0+XTRFRONT));
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
flag_type fastslow(tranto_clause_type *tto)
{
   /*-------------------------------------------------------*/
   if (bitset((*tto).n_rate_exprs,BY_FAST_TRANSITION_MASK)) return(FAST);
   /*-------------------------------------------------------*/
   return(SLOW);
}
/*===========================================================================*/
void append_transition_state
     (char *s,Memix num,state_bitstring_type bits,
      Boolean showdeath,Boolean withcomma,Boolean adjust_count)
/*
   This function appends a transition state.   The "withcomma" flag implies
   that the state "num" is the source state.   If "withcomma" is FALSE, then
   the state "num" is the destination state.
*/
{
   char *ppp;
   int_type value;
   Boolean must_space,todeath,toprune,toprunedeath,totrim,totrimdeath;
   Boolean dest_side;
   char ch;
   /*-------------------------------------------------------*/
   dest_side = (Boolean) (!withcomma);
   value = num;
#ifdef NO_PARENS_IN_MODFILE_COMMENTS
   strcat(s," ");
#endif
   ppp = enk_int_5_wide(value);
#ifdef VARIABLE_LENGTH
   ppp = enk_aux_val((void *) &value,INT_TYPE);
#endif
   strcat(s,ppp); 
   todeath = (Boolean)
       (((option_rec.onedeath) && (num>=death0) && (num<deathovf))||
        ((!option_rec.onedeath) && (state_is_flagged(bits))));
   toprune = (Boolean) ((!todeath) && ((num>=prune0) && (num<pruneovf)));
   toprunedeath = (Boolean) ((!todeath) && (trimming_is_on) &&
                             ((num>=prunedeath0) && 
                              (num<prunedeathovf)));
   totrim = (Boolean) ((!todeath) && (trimming_is_on) && (num==trim0));
   totrimdeath = (Boolean)
      ((!todeath) && (trimming_is_on) && (num==trimdeath));
   if (dest_side && adjust_count){
      if (todeath) ++aggr_deathknt;
      if (toprune) ++aggr_pruneknt;
      if (totrim)  ++aggr_trimknt;}
   if (option_rec.comment){
      strcat(s,"(* ");
      if (bits==((state_bitstring_type) NULL)){
         ppp = &ch; ch='\0';}
      else{
         ppp=encode_space_bits(bits);
#ifdef NO_PARENS_IN_MODFILE_COMMENTS
         *(ppp+strlen(ppp)-1)='\0'; ++ppp;
#endif
         }
      strcat(s,ppp);
      must_space=FALSE;
      if ((dest_side)||(final_trimming)){
         if (showdeath){
            if (todeath) strcat(s," DEATH ");
            else if (toprune) strcat(s," PRUNE ");
            else if (toprunedeath) strcat(s," PRUN.D");
            else if (totrim) strcat(s," TRIM  ");
            else if (totrimdeath) strcat(s," TRIM.D");
            else must_space=TRUE;}}
      strcat(s," *)");
      if (withcomma) strcat(s,",");
      if (must_space) strcat(s,"       ");}
   else
      if (withcomma) strcat(s,",");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
#if defined(BUGGY)||defined(OLD_COMPARISONS)
#define COMMA_WRAP_COL 35
#define BY_WRAP_COL 70
#else
#define COMMA_WRAP_COL 35
#define BY_WRAP_COL 59
#endif
/*===========================================================================*/
void print_transition(FILE *fp,Memix srcnum,Memix destnum,
                      state_bitstring_type src,state_bitstring_type dest,
                      tranto_clause_type *tto)
{
   char msg[MAXNCH_MODEL_TRANSITION_LINE+2];
   short iex,nex,pos;
   expression_type *expr;
   Boolean showdeath,adjust,three_liner;
   /*-------------------------------------------------------*/
   if (destnum == trim0) any_trim0=TRUE;
   if ((destnum >= prune0) && (destnum < pruneovf))
      any_prune[destnum-prune0]=TRUE;
   ++transition_count;
   if (!no_statistics){
      if ((transition_count % trans_count_every) == 0){
         fprintf(stderr,"%4ld transitions processed.\n",transition_count);
         if (transition_count==TRANS_COUNT_EVERY)
             trans_count_every=TRANS_COUNT_EVERY;
         if (transition_count==TRANS_COUNT_BIGEVERY)
             trans_count_every=TRANS_COUNT_BIGEVERY;}}
   if ((*tto).source_code_line_number>0)
      (*tto).source_code_line_number = (-(*tto).source_code_line_number);
   some_transitions_printed=TRUE;
   adjust=(Boolean) (fp==modfile);
   if (adjust) ++tran_count;
   pos=0;
   *msg='\0';
   showdeath=(Boolean) option_rec.onedeath;
   append_transition_state(msg,srcnum,src,showdeath,TRUE,adjust);
   if (three_liner=((Boolean) (strlen(msg)>=COMMA_WRAP_COL))){
      pos=siglen_wrapping_message(fp,msg,0,9,80,0);
      fprintf(fp,"\n");
      *msg='\0';}
   append_transition_state(msg,destnum,dest,showdeath,FALSE,adjust);
   if (three_liner || (strlen(msg)>=BY_WRAP_COL)){
      pos=siglen_wrapping_message(fp,msg,0,9,80,0);
      fprintf(fp,"\n    ");
      pos=4;
      *msg='\0';}
   strcat(msg," = "); pos += 3;
   if (bitset((*tto).n_rate_exprs,BY_FAST_RATE)) strcat(msg,"FAST ");
   nex = (*tto).n_rate_exprs & RATE_EXPRESSION_COUNT_MASK;
   pos=wrapping_message(fp,msg,0,pos,80,0);
   if (bitset((*tto).n_rate_exprs,BY_RATE_WITH_ANGLE)) fprintf(fp,"<");
   for (expr=(*tto).rate_exprs,iex=0;iex<nex;++expr,++iex){
      if (iex>0){
         fprintf(fp,","); ++pos;}
      pos=print_infix_expr(fp,expr,0,pos,80,TRUE,FALSE);}
   if (bitset((*tto).n_rate_exprs,BY_RATE_WITH_ANGLE)) fprintf(fp,">");
   fprintf(fp,";\n");
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
Boolean qq_do_code(instruction_type *pc0,tranto_clause_type *per_tto,
                   flag_type fastslow_flag,
                   Boolean trim,Boolean fast,short *tranto_counter,
                   do_code_stuff_type *do_stuff_ptr)
/*
   This function performs the section of code pointed to by the give
   program counter ("pc0").   If "pc0" is the null pointer, as for an IF
   with no ELSE clause, then no code is executed and FALSE is returned.
   The return value indicates a death/prune state.
*/
{
   instruction_type *pc;
   booltest_type *btest;
   block_if_type *bif;
   for_loop_type *loop;
   tranto_clause_type *tto;
   set_range_type *set,*setovf;
   int_type iii,iiibgn,iiiend,*iii_ptr,iiisave,iiitemp;
   Boolean bans,rtnans;
   short counter;
   register opcode_type tmpop;
   do_code_stuff_type nest_do_stuff;
   instruction_type *nest_pc0;
   unsigned char *holdeval;
   Boolean in_error;
   type_flagword_type comp;
   value_union_type val;
   calc_assign_type *clc;
#ifdef BUGGY
char *qzq;
#endif
   /*-------------------------------------------------------*/
   *tranto_counter = 0;
   if (pc0==((instruction_type *) NULL)) return(FALSE);
   holdeval = eval_state_ssbits;
   eval_state_ssbits = current_state;
#ifdef BUGGY
(*do_stuff_ptr).do_code_level++;
#endif
   for (pc=pc0;TRUE;++pc){
      tmpop = (*pc).opcode;
      switch (tmpop)
      {
      case OPCODE_ASSERT:
         bans = booltest_opcode((*pc).ptr.vvv,&btest);
         if (!bans){
            warn_this_pos(E_ASSERT,SERIOUS_WARN_LEV);
            iiitemp=(*do_stuff_ptr).do_index;
            show_rule_info_after_error((*do_stuff_ptr).do_idinfo,
                                       NULL_INDEX,NULL_INDEX,
                                       &iiitemp,
                                       "ASSERT",
                                       (*btest).source_code_line_number,
                                       current_state);
            if (per_tto){
                QQSHW_RULE_AFT_ERR((*do_stuff_ptr).do_idinfo,
                                   NULL,NULL_INDEX,NULL_INDEX,
                                   (*per_tto).source_code_line_number);}
            if (ix_adp<0) ix_adp = (*btest).lumping_sequence;}
            break;
      case OPCODE_DEATHIF:
      case OPCODE_PRUNEIF:
         bans = booltest_opcode((*pc).ptr.vvv,&btest);
         if (bans && (ix_adp<0)) ix_adp = (*btest).lumping_sequence;
         if (bans) goto true_pt;
         break;
#ifdef OBSOLETE
         bans = booltest_opcode((*pc).ptr.vvv,&btest);
         if (bans && (ix_adp<0)) ix_adp = (*btest).lumping_sequence;
         if (bans) goto true_pt;
#endif
      case OPCODE_TRANTO:
         tto = (tranto_clause_type *) (*pc).ptr.vvv;
         if (bitset(fastslow(tto),fastslow_flag)){
            *tranto_counter += 1;
            in_error=perform_transition(tto,future_state);
            future_statenum=process_state(/*future_statenum,*/future_state,tto,
                                          trim,fast,in_error);
            print_transition(modfile,current_statenum,future_statenum,
                             current_state,future_state,tto);}
         break;
      case OPCODE_BLOCK_IF:
         bif = (block_if_type *) (*pc).ptr.vvv;
         bans = fast_eval_for_expected_boolean((*bif).then_test);
         sys_memcpy(&nest_do_stuff,do_stuff_ptr,sizeof(do_code_stuff_type));
         if (bans) nest_pc0 = (*bif).then_clause;
         else nest_pc0 = (*bif).else_clause;
         rtnans=qq_do_code(nest_pc0,per_tto,fastslow_flag,trim,fast,
                           &counter,&nest_do_stuff);
         *tranto_counter += counter;
         if (rtnans) goto true_pt;
         break;
      case OPCODE_FOR_LOOP:
         loop = (for_loop_type *) (*pc).ptr.vvv;
         iii_ptr = (*(*loop).ident).ptr.iii;
         iiisave = *iii_ptr;
         set = (*loop).set_ranges;
         setovf = set + (*loop).set_range_count;
         for (;set<setovf;++set){
            iiiend = fast_eval_for_expected_integer((*set).upper_bound);
            iiibgn = fast_eval_for_expected_integer((*set).lower_bound);
            for (iii=iiibgn;iii<=iiiend;++iii){
#ifdef BUGGY
fprintf(bugfile,"LOOP FOR %d IN [%d..%d], iii_ptr=%08x\n",
(int) iii,(int) iiibgn,(int) iiiend,(int) iii_ptr);
#endif
               *iii_ptr = iii;
               sys_memcpy(&nest_do_stuff,do_stuff_ptr,sizeof(do_code_stuff_type));
               nest_do_stuff.do_idinfo=(*loop).ident;
               nest_do_stuff.do_index=(Subscript) iii;
               rtnans=qq_do_code((*loop).body,per_tto,fastslow_flag,trim,fast,
                                 &counter,&nest_do_stuff);
               *tranto_counter += counter;
               if (rtnans){
                  *iii_ptr = iiisave;
                  goto true_pt;}}}
         *iii_ptr = iiisave;
         break;
      case OPCODE_RETURN:
         goto false_pt;
      case OPCODE_CALC:
      case OPCODE_CALC_T:
         clc = (calc_assign_type *) (*pc).ptr.vvv;
         comp = fast_evaluate_full_expression((*clc).expr,&val);
#ifdef OBSOLETE
/*??????*/ printf("CALC %s = ",(*((*clc).idinfo)).name);
/*??????*/ printf("[%d,%s,%lf]\n",val.iii,((val.bbb) ? "TRUE" : "FALSE"),val.rrr);
#endif
         value_to_memory(&(*((*clc).idinfo)).ptr,&val,comp);
         break;
      }}
   /*-------------------------------------------------------*/
false_pt:
#ifdef BUGGY
(*do_stuff_ptr).do_code_level--;
#endif
   eval_state_ssbits = holdeval;
   return(FALSE);
true_pt:
#ifdef BUGGY
(*do_stuff_ptr).do_code_level--;
#endif
   eval_state_ssbits = holdeval;
   return(TRUE);
}
/*===========================================================================*/
Memix do_code(instruction_type *pc0,tranto_clause_type *per_tto)
{
   short knt;
   static do_code_stuff_type do_stuff =
      {
          NULL, 0
      };
   /*-------------------------------------------------------*/
   ix_adp = (-1);
   qq_do_code(pc0,per_tto,0x00,FALSE,FALSE,&knt,&do_stuff);
   /*-------------------------------------------------------*/
   return(ix_adp);
}
/*===========================================================================*/
short do_tranto_code(instruction_type *pc0,
                     flag_type fastslow_flag,Boolean trim,Boolean fast)
{
   short knt;
   static do_code_stuff_type do_stuff =
      {
          NULL, 0
      };
   /*-------------------------------------------------------*/
   qq_do_code(pc0,NULL,fastslow_flag,trim,fast,&knt,&do_stuff);
   /*-------------------------------------------------------*/
   return(knt);
}
/*===========================================================================*/
Memix process_state(/*Memix statenum,*/state_bitstring_type state,
                    tranto_clause_type *per_tto,
                    Boolean trim,Boolean fast,Boolean in_error)
/*
   This function determines if "state" is a death, prune, or trim state.
   It also assigns the state a state number.   The number is assigned by
   direct computation if the state is a death, prune, or trim state.   The
   number is assigned via a hash table lookup if the state is a state in
   the output model.
*/
{
   Boolean prune_state,death_state;
   static Boolean already_warned_death=FALSE;
   static Boolean already_warned_prune=FALSE;
   Memix ans_statenum,holdcurrnum;
   state_bitstring_type found;
   long ans_bucknum;
   unsigned char *holdcurr;
#ifdef BUGGY
   char *ppp;
#endif
   /*-------------------------------------------------------*/
#ifdef BUGGY
   if (transition_count>=62590)
       fprintf(stderr,
               "process stateix#%d, state_byte_count=%d\n",
               (int) (state-state_storage)/5,(int) state_byte_count);
   ppp=encode_space_bits(state);
   fprintf(bugfile,">>> process_state");
   fprintf(bugfile,"(%s,per_tto,trim=%d,fast=%d,in_error=%d)\n",
                   ppp,(int) trim,(int) fast,(int) in_error);
#endif
   found = find_state(state,&ans_statenum,&ans_bucknum);
   death_state = state_is_flagged(state);
   prune_state = FALSE;
   if (found == NULL){
      holdcurr = current_state;
      holdcurrnum = current_statenum;
      current_state = state;
      memcpy(allvarstorage_copy,allvarstorage,allvarstorage_size);
         do_code(initial_start_instr[OPCODE_CALC],per_tto);
         ixassert=do_code(initial_start_instr[OPCODE_ASSERT],per_tto);
         ixdeath=do_code(initial_start_instr[OPCODE_DEATHIF],per_tto);
         if (ixdeath>=0) ixprune=(-1);
         else ixprune=do_code(initial_start_instr[OPCODE_PRUNEIF],per_tto);
      memcpy(allvarstorage,allvarstorage_copy,allvarstorage_size);
      current_state = holdcurr;
      current_statenum = holdcurrnum;
      prune_state = (Boolean) ((ixprune>=0)&&(!in_error));
      death_state = (Boolean) ((ixdeath>=0)||(in_error));

      #ifdef BUGGY
         fprintf(stderr,"^^^^ DID NOT FIND %i \n",current_statenum);
      #endif

      if (((death_state) || (prune_state)) && (fast) && (trimming_is_on)){
         if ((death_state) && (!already_warned_death)){
            already_warned_death=TRUE;
            warn_this_pos(E_RECOVERTODEATH,SERIOUS_WARN_LEV);}
         else if ((prune_state) && (!already_warned_prune)){
            already_warned_prune=TRUE;
            warn_this_pos(E_RECOVERTOPRUNE,SERIOUS_WARN_LEV);}}}
#ifdef BUGGY
fprintf(bugfile,"ixassert=%d,ixdeath=%d,ixprune=%d\n",
(int) ixassert,(int) ixdeath,(int) ixprune);
#endif
   if ((death_state) && (option_rec.onedeath)) ans_statenum = ixdeath+death0;
   else if ((prune_state)) ans_statenum = ixprune+prune0;
   else if ((trim) && (trimming_is_on)) ans_statenum = trim0;
   else{
        if (death_state) set_death_state_bit(state);
        else if ( ! option_rec.onedeath) clear_death_state_bit(state);  // RWB 
        if (found == NULL) ans_statenum = add_a_new_state(state,ans_bucknum);
        }
   /*-------------------------------------------------------*/
#ifdef BUGGY
   if (transition_count>=62590){
       fprintf(stderr,"ans_statenum=%d\n",(int) ans_statenum);}
#endif
   return(ans_statenum);
}
/*===========================================================================*/
void generate_model()
{
   Memix ready;
   short fast_count,slow_count,wrnlev;
   Memix statenum;
   Boolean plural,in_error;
   long ss,ssovf;
   int_type value;
   tranto_clause_type *tto0,*tto9,*tto;
   static operation_type opval_op = OP_VAL;
   static operand_type opval_opnd;
   static expression_type trimomega_expr =
      {
         &opval_op, &opval_op, &opval_opnd,
         1,1,1,
         MAXSHORT, FALSE, REAL_TYPE
      };
   static tranto_clause_type trimomega_tto =
      {
         { NULL, NULL, 0 },
         &trimomega_expr, 1, MAXSHORT
      };
   /*-------------------------------------------------------*/
   transition_count=0;
   trans_count_every=100;
   final_trimming=FALSE;
   executing_rules=TRUE;
   if (total_prune_count>0){
      plural=(Boolean) (total_prune_count>1);
      fprintf(modfile,"PRUNESTATES = %s",(plural ? STRLP : ""));
      for (ss=prunedeath0,ssovf=prunedeathovf+trimdeath_count;ss<ssovf;++ss){
         if (ss>prunedeath0) fprintf(modfile,",");
         fprintf(modfile,"%d",(int) ss);}
      fprintf(modfile,"%s;\n",(plural ? STRRP : ""));}
   fprintf(modfile,"\n\n");
   current_statenum = state0;
   current_state = point_to_state(current_statenum);
#ifdef OBSOLETE
/*?????*/printf("do_code(initial_start_instr[OPCODE_CALC_T]=0x%08lx)\n",(unsigned int) initial_start_instr[OPCODE_CALC_T]);
#endif
   /*------------------------------------------  transition to start state */
#ifdef OBSOLETE_MAYBE
   /* unnecessary: start state is disallowed from referencing variables */
   do_code(initial_start_instr[OPCODE_CALC_T],NULL);
#endif
   (*start_tranto_ptr).source_code_line_number =
      (-(*start_tranto_ptr).source_code_line_number);
   in_error=perform_transition(start_tranto_ptr,current_state);
   /*------------------------------------------  process the start state */
   start_statenum =
      process_state(/*current_statenum,*/current_state,start_tranto_ptr,
                    FALSE,FALSE,in_error);
   /*------------------------------------------  generate the model */
#ifdef BUGGY
fprintf(bugfile,"state0=%d  next_unique_state_counter=%d\n",
(int) state0,(int) next_unique_state_counter);
#endif
   for (ready=state0;ready<next_unique_state_counter;++ready){
#ifdef BUGGY
fprintf(bugfile,"LOOP: ready=%ld, next_unique_state_counter=%ld\n",
(long) ready,(long) next_unique_state_counter);
#endif
      current_state = point_to_state(ready);
      current_statenum = ready;
      if (!state_is_flagged(current_state)){
         some_transitions_printed=FALSE;
#ifdef OBSOLETE
/*?????*/printf("do_code(initial_start_instr[OPCODE_CALC_T]=0x%08lx)\n",(unsigned int) initial_start_instr[OPCODE_CALC_T]);
#endif
         do_code(initial_start_instr[OPCODE_CALC_T],NULL);
#ifdef OLD_COMPARISONS
if (trimming_is_on){
fast_count=do_tranto_code(initial_start_instr[OPCODE_TRANTO],FAST,FALSE,TRUE);
slow_count=do_tranto_code(initial_start_instr[OPCODE_TRANTO],
SLOW,(Boolean) (fast_count>0),FALSE);}
else{
do_tranto_code(initial_start_instr[OPCODE_TRANTO],
BOTH_FAST_AND_SLOW,FALSE,FALSE);}
#else
         fast_count = do_tranto_code(initial_start_instr[OPCODE_TRANTO],
                                     FAST,FALSE,TRUE);
         if (trimming_is_on){
            slow_count = do_tranto_code(initial_start_instr[OPCODE_TRANTO],
                                        SLOW,(Boolean) (fast_count>0),FALSE);}
         else{
            slow_count = do_tranto_code(initial_start_instr[OPCODE_TRANTO],
                                        SLOW,FALSE,FALSE);}
#endif
         if (!some_transitions_printed){
            value=current_statenum;
            show_wrn_at_pos_on_both(E_NOTRANTOSOUT,0,&value,
                                    INT_TYPE,SERIOUS_WARN_LEV);}}}
   /*------------------------------------------  print extra trim trans's */
   if (trimming_is_on){
      final_trimming=TRUE;
      *trimomega_expr.operands = qqsearchid("TRIMOMEGA",FALSE);
      if (any_trim0)
         print_transition(modfile,(Memix) trim0,(Memix) trimdeath,NULL,NULL,
                          &trimomega_tto);
      if (option_rec.trim==2){
         for (statenum=prune0;statenum<pruneovf;++statenum){
            if (any_prune[statenum-prune0])
               print_transition(modfile,(Memix) statenum,
                                        (Memix) (prunedeath0+statenum-prune0),
                                        NULL,NULL,&trimomega_tto);}}
      final_trimming=FALSE;}
   /*------------------------------------------  */
   tto0 = (tranto_clause_type *) section_start_addrs[OBJ_TRANTO];
   tto9 = (tranto_clause_type *) (((char *) section_start_addrs[OBJ_TRANTO]) +
                                  memory_bytes_required[OBJ_TRANTO]);
   for (tto=tto0;tto<tto9;++tto){
      if ((*tto).source_code_line_number>0){
         value=(*tto).source_code_line_number;
         wrnlev = DEFAULT_WARN_LEV;
         if (gen_suppress) wrnlev = MINOR_WARN_LEV; 
         show_wrn_on_both(E_TRANTONOTUSED,(void *) &value,INT_TYPE,wrnlev);}}
   /*------------------------------------------  */
   if (fatal_error)
      fprintf(modfile,"*** ERROR *** model file is erroneous.\n");
   executing_rules=FALSE;
   state_count = next_unique_state_counter-1;
   /*-------------------------------------------------------*/
   return;
}
/*===========================================================================*/
#define FPR_KNT(fp) \
   fprintf(fp,"%sNUMBER OF STATES IN MODEL = %d%s\n", \
           pre,(int) state_count,post); \
   fprintf(fp,"%sNUMBER OF TRANSITIONS IN MODEL = %d%s\n", \
           pre,(int) tran_count,post)
#define SAK(fp,ak,s0,ns,kd) \
        show_aggr_knt(fp,(long) ak,(long) s0,(long) ns,kd,pre,post)
#define SAKD(fp) SAK(fp,aggr_deathknt,death0,deathif_count,"DEATH STATES")
#define SAKP(fp) SAK(fp,aggr_pruneknt,prune0,pruneif_count,"PRUNE STATES")
#define SAKT(fp) SAK(fp,aggr_trimknt,trim0,trimdeath_count, \
                     "TRIMMED TRANSITIONS")
#define SBUK(fp) \
        fprintf(fp,"%d HASH BUCKETS OF SIZE %d ENTRIES EACH.\n", \
                (int) nof,(int) value)
/*===========================================================================*/
void show_state_counts()
{
   char *pre,*post;
   static char nullstr[]="";
   static char bgnstr[]="(* ";
   static char endstr[]=" *)";
   /*-------------------------------------------------------*/
   pre=post=nullstr;
   if (outputfile != ((FILE *) NULL)){
      FPR_KNT(outputfile);
      if (option_rec.onedeath) SAKD(outputfile);
      if (pruneif_count>0) SAKP(outputfile);
      if (trimdeath_count>0) SAKT(outputfile);}
   if (listfile != ((FILE *) NULL)){
      FPR_KNT(listfile);
      if (option_rec.onedeath) SAKD(listfile);
      if (pruneif_count>0) SAKP(listfile);
      if (trimdeath_count>0) SAKT(listfile);}
   pre=bgnstr; post=endstr;
   if ((option_rec.comment_stats) && (modfile != ((FILE *) NULL))){
      fprintf(modfile,"\n");
      FPR_KNT(modfile);
      if (option_rec.onedeath) SAKD(modfile);
      if (pruneif_count>0) SAKP(modfile);
      if (trimdeath_count>0) SAKT(modfile);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
int long_compare(void *a,void *b)
{
   long ia,ib;
   /*-------------------------------------------------------*/
   sys_memcpy(&ia,a,sizeof(long));
   sys_memcpy(&ib,b,sizeof(long));
   if (ia<ib) return(-1);
   if (ia>ib) return(1);
   /*-------------------------------------------------------*/
   return(0);
}
void show_bucket_statistics()
{
   long ixbuck,ixbuckovf,nof,value;
   Memix count,next_link;
   unsigned char *ppp;
   long *count_arr;
   /*-------------------------------------------------------*/
   count_arr = (long *) bucket_storage;
   for (ixbuck=0,ixbuckovf=bucket_count;ixbuck<ixbuckovf;++ixbuck){
      ppp=use_bucket(ixbuck,&count,&next_link);
      *(count_arr+ixbuck) = count;}
   qsort((char *) count_arr,(size_t) bucket_count,sizeof(long),long_compare);
   for (ixbuck=0;ixbuck<ixbuckovf;){
      value = *(count_arr+ixbuck);
      nof=ixbuck;
      for (;(ixbuck<ixbuckovf)&&((*(count_arr+ixbuck))==value);++ixbuck);
      nof=ixbuck-nof;
      if (outputfile != ((FILE *) NULL)) SBUK(outputfile);
      if (listfile != ((FILE *) NULL)) SBUK(listfile);}
   /*-------------------------------------------------------*/
}
/*===========================================================================*/

// added by RWB 10/5/99

void astgen_free()
{  
  // long bytes; 
  // int myadrix;


   free(any_prune);       any_prune = (Boolean *) NULL;


   //   bytes =  (((long) main_table_bucket_size) *   /* bytes/bucket */
   //         ((long) bucket_count));             /* buckets */
   //   bytes += (((long) extension_table_bucket_size) *
   //            ((long) extension_bucket_count));

   //   for (myadrix=0;myadrix<bytes;++myadrix)
   //  { *(bucket_storage+myadrix) = 0;
   //  }
   free(bucket_storage);  
   bucket_storage = (Boolean *) NULL;
   free(state_storage);  
   state_storage = (Boolean *) NULL;


   /* current_state = 0;
future_state = 0;
current_statenum = 0;
future_statenum = 0;
next_free_extension_link = 0;
table_element_size = 0;
main_table_bucket_size = 0;
extension_table_bucket_size = 0;
main_storage_size = 0;
total_storage_size = 0;
total_bits_size = 0;
realloc_more_size = 0;
realloc_more_bits_size = 0;
state_byte_count = 0;
state_bit_count = 0;
some_transitions_printed = 0;
final_trimming = 0;
any_trim0 = 0; */
// *any_prune = 0;

   /*
next_unique_state_counter = 0;
death0 = 0;
prune0 = 0;
state0 = 0;
trim0 = 0;
trimdeath = 0;
prunedeath0;
deathovf = 0;
pruneovf = 0;
prunedeathovf = 0;
aggr_deathknt = 0;
aggr_pruneknt = 0;
aggr_trimknt = 0;
state_count = 0;
tran_count;
ixassert = 0;
ixdeath = 0;
ixprune = 0;
ix_adp = 0; 
start_statenum =0;;
   */

}
