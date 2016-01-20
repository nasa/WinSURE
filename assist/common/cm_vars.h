#if !defined(_ZZ_q_cm_vars_DEFINED)
#define _ZZ_q_cm_vars_DEFINED
/*===========================================================================*/

#include "lib_def.h"

#include "cm_defs.h"
#include "cm_types.h"

EXKW char date_string[12];
EXKW char time_string[20];

                            /**************************/
                            /**************************/
                            /****                  ****/
                            /****   FILE HANDLES   ****/
                            /****                  ****/
                            /**************************/
                            /**************************/

EXKW FILE *inputfile;  /* file to read from (can be set to stdin) */
EXKW FILE *outputfile; /* file to write statistics to (usually stdout) */
EXKW FILE *errorfile;  /* file to write errors/warnings to (usually stderr) */
EXKW FILE *modfile;    /* file to write output model to */
EXKW FILE *listfile;   /* file to write program listing to */
EXKW FILE *objectfile; /* file to store object code in */
EXKW FILE *xreffile;   /* file to store cross reference entries in */
EXKW FILE *exefile;    /* scratch executuable file */
EXKW FILE *scrfile;    /* scratch text file */
EXKW FILE *bugfile;    /* scratch debug file (usually == listfile or stdout) */

EXKW pathname_type filename_prefix;
EXKW pathname_type input_filename;
EXKW pathname_type output_filename;
EXKW pathname_type mod_filename;
/* EXKW pathname_type log_filename; */
EXKW pathname_type list_filename;
EXKW pathname_type object_filename;
EXKW pathname_type xref_filename;
EXKW pathname_type exe_filename;
EXKW pathname_type tempinputfilename[SAVE_LEVELS];


                           /*****************************/
                           /*****************************/
                           /****                     ****/
                           /****   INT/REAL LIMITS   ****/
                           /****                     ****/
                           /*****************************/
                           /*****************************/

EXKW char maxint_str[MAX_INT_TYPE_NDIGS_P];
EXKW char neg_maxint_str[MAX_INT_TYPE_NDIGS_P];
EXKW int_type maxint;
EXKW real_type maxreal;
EXKW real_type ln_maxreal;
EXKW real_type ln_maxint;
EXKW value_union_type std_real_value;
EXKW value_union_type std_null_value;

                    /******************************************/
                    /******************************************/
                    /****                                  ****/
                    /****   IDENTIFIER AND NUMBER TABLES   ****/
                    /****                                  ****/
                    /******************************************/
                    /******************************************/

EXKW Memix nele_identifier_table;
EXKW Memix nele_ident_at_table_top;
EXKW identifier_info_type *identifier_table;
EXKW memsize_t identifier_table_bytes;
/**/
EXKW Memix nele_constant_table;
EXKW char *constant_table;
EXKW memsize_t constant_table_bytes;
EXKW ident_usage_flag_type *usage_table;

/*===========================================================================*/
#endif
