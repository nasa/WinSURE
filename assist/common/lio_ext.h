#if !defined(_ZZ_q_lio_DEFINED)
#define _ZZ_q_lio_DEFINED
/*===========================================================================*/

#include "commonio.h"

#ifdef _NO_PROTO
extern void check_errno();
extern FILE *open_input_file();
extern FILE *open_output_file();
extern FILE *open_binary_input_file();
extern FILE *open_binary_output_file();
extern flagtyp exists();
extern flagtyp okay_to_read();
extern flagtyp okay_to_write();
extern flagtyp okay_to_delete();
#else
extern void check_errno(short,char *);
extern FILE *open_input_file(char *);
extern FILE *open_output_file(char *);
extern FILE *open_binary_input_file(char *);
extern FILE *open_binary_output_file(char *);
extern flagtyp exists(char *);
extern flagtyp okay_to_read(char *);
extern flagtyp okay_to_write(char *);
extern flagtyp okay_to_delete(char *);
#endif

#ifdef _ZZ_q_cm_types_DEFINED
#ifdef _NO_PROTO
extern relative_address_type relative_identifier();
extern identifier_info_type *absolute_identifier();
#else
extern relative_address_type relative_identifier(identifier_info_type *);
extern identifier_info_type *absolute_identifier(relative_address_type);
#endif
#endif

/*===========================================================================*/
#endif
