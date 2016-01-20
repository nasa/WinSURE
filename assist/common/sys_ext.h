#if !defined(_ZZ_q_sys_ext_DEFINED)
#define _ZZ_q_sys_ext_DEFINED
/*===========================================================================*/

#ifdef _NO_PROTO
extern void init_syslib();
extern void re_init_syslib();
extern void init_elapsed_time();
extern void shew_elps();
extern void show_elapsed_processing_time();
extern void show_elapsed_generation_time();
extern void show_aggr_knt();
extern void make_date_and_time();

extern void g_malloc_abort_if_error();
#if (!defined(MSDOS))
extern void *g_malloc();
extern void *g_realloc();
#endif

#else

extern void init_syslib(void);
extern void re_init_syslib(void);
extern void init_elapsed_time(void);
extern void shew_elps(FILE *,char *);
extern void show_elapsed_processing_time(void);
extern void show_elapsed_generation_time(void);
extern void show_aggr_knt(FILE *,long,long,long,char *,char *,char *);
extern void make_date_and_time(void);

extern void g_malloc_abort_if_error(void *);
#if (!defined(MSDOS))
extern void *g_malloc(int);
extern void *g_realloc(void *,int,int);
#endif

#endif
/*===========================================================================*/
#endif
