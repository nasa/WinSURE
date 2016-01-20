#include "cm_stdio.h"

/*#define COMPARISON_LOOP_MAX 128*/
#define COMPARISON_LOOP_MAX 32

#define MAX_STATE_KNT 102400

#ifdef MANY_TRANTOS
#define MAX_TRANTO_KNT 0x200000
#else
#define MAX_TRANTO_KNT 100000
#endif

#define OPENPAREN '('
#define STAR '*'
#define EQUALS '='
#define CLOSEPAREN ')'

typedef void (*any_process_routine)(char *);

typedef struct qqstate
   {
     char *indiv_comments;
     char *coll_comments;
   } state_info_type;

typedef struct qqtto
   {
     int start_statenum;
     int dest_statenum;
     int tranto_repeat;
     char *rate;
   } transition_info_type;

extern void abort_program(void);
extern void slow_memmove(char *,char *,int);
extern char *malloc_copy(char *);
extern char *strip_number(char *,int *);
extern char *strip_state_comments(char *,char *,int);
extern char *strip_state(char *,int *);
extern char *cvt_1_to_coll(char *,char *);
extern char *cvt_to_coll(char *,char *);
extern void RemoveTrailingNewlineWhitespace(char *);
extern Boolean text_is_blank(char *);
extern int parse_file(FILE *,FILE *,any_process_routine);
extern void parse_ind_vs_coll_argv(int,char **);
extern void init_ind_vs_coll(Boolean);
extern void translate(char *,char *,char *);
extern void translate_if_maybe(char *,char *,char *);
extern void combine_1_difference(char *,int,int);
extern void combine_1_sum(char *,int,int);
extern void combine_1_left_factor(char *,int,char *,int);
extern void combine_1_right_factor(char *,char *,int,int);
extern void combine_val_2_front(char *);
extern void combine(char *);
extern void aux_trn_file(char *);
extern void out_rate(FILE *fp,char *,int,int);
extern int tto_cmp_eqvs
       (int,int,int,int,transition_info_type *,transition_info_type *);

#define IS_ARGV_OPTION(vvv) ((((vvv)[0])=='/')||(((vvv)[0])=='-'))

#ifdef IND_VS_COLL_ROUTINES
#define EXKW_IND
#else
#define EXKW_IND extern
#endif

#define MAX_TRN_KNT 100

EXKW_IND int aux_trn_knt;
EXKW_IND int state_knt;
EXKW_IND int tranto_knt;
EXKW_IND char *aux_from[MAX_TRN_KNT];
EXKW_IND char *aux_to[MAX_TRN_KNT];
EXKW_IND state_info_type states[MAX_STATE_KNT];
EXKW_IND transition_info_type trantos[MAX_TRANTO_KNT];
EXKW_IND Boolean combine_products;
EXKW_IND Boolean debug;

#define FREEIF(ppp) if (ppp) free(ppp)
