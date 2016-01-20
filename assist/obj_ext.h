#if !defined(_ZZ_q_obj_ext_DEFINED)
#define _ZZ_q_obj_ext_DEFINED
/*===========================================================================*/
#include "objtypes.h"
#if (!defined(STANDALONE))
extern void logfile_storage_map(char *);  /* USER SUPPLIED */
#endif
/*===========================================================================*/
extern void init_objlib(void);
extern void re_init_objlib(void);
extern Memix cum_aligned_object_header(void);
extern void rewrite_object_header(void);
extern void load_obj_file_allocating_memory(Boolean);
extern void free_object_file_memory(void);
extern void fetch_table_front(object_table_type *,Memix *,size_t *);
extern void store_table_front(object_table_type,Memix,size_t);
extern relative_address_type store_in_object_file
       (object_table_type,void *,size_t,long);
extern object_table_type cvt_to_object_table_type(type_flagword_type);
/*===========================================================================*/
#define enk_aux_type(vvv,flg) enk_aux_val(vvv,cvt_to_object_table_type(flg))
#define for_all_defs_no_struct \
        Memix jjj,knt; \
        pointer_union_type ptru,ovfu
#define for_all_defs \
        Memix jjj,knt,meminc; \
        pointer_union_type ptru,ovfu
#define for_all_on_line_set(ppp,pppovf,lll) \
        list_page_need(lll); \
        map_page_addr(ptru.ccc); \
        if ((ppp+knt)>pppovf) knt=pppovf-ppp; \
        for (jjj=0;jjj<knt;++jjj)
#define for_all_on_line(ppp,pppovf) for_all_on_line_set(ppp,pppovf,1)
#define for_all_storage(o,ppp,n) \
        knt=n; \
        ovfu.ccc=section_start_addrs[o]+memory_bytes_required[o]; \
        for(ptru.ccc=section_start_addrs[o];ptru.ccc<ovfu.ccc;ppp+=n)
#define for_all_struct_storage(o,ppp,n,type) \
        knt=((Memix) n); \
        meminc=knt*((Memix) sizeof(type)); \
        ovfu.ccc=section_start_addrs[o]+memory_bytes_required[o]; \
        ppp = (type *) section_start_addrs[o]; \
        for (ptru.ccc=section_start_addrs[o]; \
             ptru.ccc<ovfu.ccc;ptru.ccc+=meminc,ppp+=n)
/*===========================================================================*/
#endif
