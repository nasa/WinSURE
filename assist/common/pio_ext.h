#if !defined(_ZZ_q_pio_DEFINED)
#define _ZZ_q_pio_DEFINED
/*===========================================================================*/

#include "cm_types.h"

#ifdef _NO_PROTO
extern void init_prsiolib();
extern void restore_prsiolib();
extern FILE *open_input_using_big_buffer();
extern FILE *open_output_using_big_buffer();
extern void free_big_input_buffer();
extern void safe_fprintf();
extern void make_temporary_filename();
extern void cleanup_temporary_filename();
extern short adjust_wrapping_column();
extern Boolean two_in_a_row();
extern void make_two_in_a_row();
extern token get_ident();
extern token get_digit_string();
extern type_flagword_type get_literal_text();
extern void advance_1_char();
extern Boolean advance_over_spacing_eof();
extern Boolean skip_over_comment();
extern void show_err_at_pos_on_both();
extern void show_wrn_at_pos_on_both();
extern void setup_for_tail_msg();
extern void show_tail_err_on_both();
extern void show_tail_wrn_on_both();
extern void show_tail_err_at_pos_on_both();
extern void show_tail_wrn_at_pos_on_both();
#else
extern void init_prsiolib(scanning_character_info_type *);
extern void restore_prsiolib(scanning_character_info_type *);
extern FILE *open_input_using_big_buffer(char *);
extern FILE *open_output_using_big_buffer(char *,size_t);
extern void free_big_input_buffer(void);
extern void safe_fprintf(FILE *,char *);
extern void make_temporary_filename(char *);
extern void cleanup_temporary_filename(char *);
extern short adjust_wrapping_column(FILE *,short,short,short,short,short *);
extern Boolean two_in_a_row(char,char);
extern void make_two_in_a_row(char *);
extern token get_ident(FILE *,char *);
extern token get_digit_string(FILE *,char *,Boolean);
extern type_flagword_type get_literal_text(FILE *,char *,char);
extern void advance_1_char(void);
extern Boolean advance_over_spacing_eof(void);
extern Boolean skip_over_comment(char,char,short);
extern void show_err_at_pos_on_both(short,short,void *,type_flagword_type);
extern void show_wrn_at_pos_on_both(short,short,void *,type_flagword_type,
                                    short);
extern void setup_for_tail_msg(void *,type_flagword_type,char *);
extern void show_tail_err_on_both(short,void *,type_flagword_type,char *);
extern void show_tail_wrn_on_both
                (short,void *,type_flagword_type,char *,short);
extern void show_tail_err_at_pos_on_both
       (short,short,void *,type_flagword_type,char *);
extern void show_tail_wrn_at_pos_on_both
       (short,short,void *,type_flagword_type,short,char *);
#endif
/*===========================================================================*/
#define safe_putc(c,fp) \
        ((isascii(c)&&((is_space_or_print(c)))) ? (putc(c,fp)) \
                                                : (putc('?',fp)))
#define is_iolib_space(ch) (((ch)==' ')||((ch)=='\t')||((ch)=='\n'))
/*===========================================================================*/
#endif
