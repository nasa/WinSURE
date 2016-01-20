#if !defined(_ZZ_q_agen_ext_DEFINED)
#define _ZZ_q_agen_ext_DEFINED
/*===========================================================================*/
#include "commonio.h"
#include "asttypes.h"
#define set_death_state_bit(sss) set_bit(sss,(bitsize_type) (state_bit_count))
#define clear_death_state_bit(sss) clear_bit(sss,(bitsize_type) (state_bit_count))
/*===========================================================================*/
extern void init_hash_function(void);
extern void divvy_up_extension_storage(void);
extern Boolean state_is_flagged(state_bitstring_type);
extern long state_hash_index(state_bitstring_type);
extern unsigned char *link_to_a_brand_new_bucket(unsigned char **,Memix *);
extern Memix add_a_new_state(state_bitstring_type,long);
extern unsigned char *use_bucket(long,Memix *,Memix *);
extern unsigned char *use_extension_bucket(Memix,Memix *);
extern void increment_packed_count(unsigned char *);
extern long extract_packed_value(unsigned char *);
extern void insert_packed_value(unsigned char *,long);
#ifdef BUGGY
extern void show_bucket(FILE *,long);
#endif
extern state_bitstring_type find_state(state_bitstring_type,Memix *,long *);
extern Boolean booltest_opcode(void *,booltest_type **);
extern Boolean perform_transition(tranto_clause_type *,state_bitstring_type);
extern state_bitstring_type point_to_state(Memix);
extern flag_type fastslow(tranto_clause_type *);
extern void append_transition_state
       (char *,Memix,state_bitstring_type,Boolean,Boolean,Boolean);
extern void print_transition
       (FILE *,Memix,Memix,state_bitstring_type,state_bitstring_type,
        tranto_clause_type *);
extern Boolean qq_do_code
  (instruction_type *,tranto_clause_type *,
   flag_type,Boolean,Boolean,short *,do_code_stuff_type *);
extern Memix do_code(instruction_type *,tranto_clause_type *);
extern short do_tranto_code(instruction_type *,flag_type,Boolean,Boolean);
extern Memix process_state(/*Memix,*/state_bitstring_type,tranto_clause_type *,
                           Boolean,Boolean,Boolean);
extern void generate_model(void);
extern void show_state_counts(void);
extern int long_compare(void *,void *);
extern void show_bucket_statistics(void);
/*===========================================================================*/
#endif
