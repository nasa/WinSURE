#if !defined(_ZZ_q_str_ext_DEFINED)
#define _ZZ_q_str_ext_DEFINED
/*===========================================================================*/

#include "commonio.h"


#if !defined(WITH_TAE_DCLS)
#include "cm_types.h"
#endif


             /***********************************************/
             /***********************************************/
             /****                                       ****/
             /****   PROTOTYPES FOR THE STRING LIBRARY   ****/
             /****                                       ****/
             /***********************************************/
             /***********************************************/

#ifdef _NO_PROTO
extern void init_strlib();
extern void re_init_strlib();
extern int i_strcmp();
extern int i_strncmp();
extern void capise_string();
#else
extern void init_strlib(void);
extern void re_init_strlib(void);
extern int i_strcmp(char *,char *);
extern int i_strncmp(char *,char *,int);
extern void capise_string(char *);
#endif

#ifdef _NO_PROTO
extern void append_suffix_to_prefix();
extern void append_suffix_to_filename();
#else
extern void append_suffix_to_prefix(char *,char *,char *);
extern void append_suffix_to_filename(char *,char *);
#endif

#ifdef _NO_PROTO
/*--------------------------------------------------------*/
#if !defined(WITH_TAE_DCLS)
extern Boolean string_is_too_big_int_type();
extern Boolean is_a_blank_line();
#ifdef SIGNED_TO_WORDS
extern Boolean signed_to_word();
#endif
extern Boolean ident_valid_character();
extern Boolean exponent_valid_character();
extern void value_to_memory();
extern size_t type_size();
extern void standard_value();
extern void *arr_addr();
extern char *enk_value_union_val();
extern char *enk_aux_val();
extern char *enk_aux_val_just();
extern char *enk_ptr_union_value();
extern Boolean ssvar_vs_range_err();
extern void encode_warning_and_severity();
extern void encode_arrow_to_error();
extern int_type decode_integer();
extern real_type decode_real();
extern short translate_tab_characters();
extern short strip_off_trailing_space();
extern void make_promotion_message();
extern void make_symbolic_type_list();
extern void fprintf_hex_pointer();
extern void fprintf_hex_reladdr();
extern void fprintf_hex_memix();
extern char *enk_int_5_wide();
#ifdef SLOW_MEMMOVE
extern void slow_memmove();
#endif
extern void init_error_words();
extern Boolean remove_extent();
#endif
#else
/*--------------------------------------------------------*/
#if !defined(WITH_TAE_DCLS)
extern Boolean string_is_too_big_int_type(char *);
extern Boolean is_a_blank_line(char *);
#ifdef SIGNED_TO_WORDS
extern Boolean signed_to_word(char *,char);
#endif
extern Boolean ident_valid_character(char);
extern Boolean exponent_valid_character(char);
extern void value_to_memory
       (pointer_union_type *,value_union_type *,type_flagword_type);
extern size_t type_size(type_flagword_type);
extern void standard_value(value_union_type *,type_flagword_type);
extern void *arr_addr(void *,short,type_flagword_type);
extern char *enk_value_union_val
       (value_union_type *,type_flagword_type,state_bitstring_type);
extern char *enk_aux_val(void *,type_flagword_type);
extern char *enk_aux_val_just(void *,type_flagword_type);
extern char *enk_ptr_union_value(pointer_union_type *,type_flagword_type,
                                 int_type,state_bitstring_type);
extern Boolean ssvar_vs_range_err(ssvar_value_type,state_offset_type *);
extern void encode_warning_and_severity(short,char *);
extern void encode_arrow_to_error(char *,short *,short *,Boolean,Boolean);
extern int_type decode_integer(char *);
extern real_type decode_real(char *);
extern short translate_tab_characters(char *buffer,short nch);
extern short strip_off_trailing_space(char *buffer);
extern void make_promotion_message
       (char *,type_flagword_type,type_flagword_type);
extern void make_symbolic_type_list(char *,type_flagword_type);
extern void fprintf_hex_pointer(FILE *,void *);
extern void fprintf_hex_reladdr(FILE *,relative_address_type);
extern void fprintf_hex_memix(FILE *,Memix);
extern char *enk_int_5_wide(int_type);
#ifdef SLOW_MEMMOVE
extern void slow_memmove(char *,char *,int);
#endif
extern void init_error_words(short,Boolean,Boolean);
extern Boolean remove_extent(char *,char *);
#endif
/*--------------------------------------------------------*/
#endif
                   /***********************************/
                   /***********************************/
                   /****                           ****/
                   /****   IMPLEMENTED AS MACROS   ****/
                   /****                           ****/
                   /***********************************/
                   /***********************************/

#define equiv_text_char(ch) \
        ((char) ((isascii(ch)&&isspace(ch)) ? ' ' : (ch)))
/*
   nullify_string turns "s" into the emptystring and returns the value zero.
*/
#define nullify_string(s) (((*s)='\0'),(0))
/*
   add_newline appends a newline character to "s" and increments "n".
*/
#define add_newline(s,w) ((*(s+(w++))='\n'),(*(s+w)='\0'))
/*
   determine if word is plural (more than one) and return "S" or emptystring.
*/
#define yield_S_if_plural(n) (((n)>1) ? "S" : "")

/*===========================================================================*/
#endif
