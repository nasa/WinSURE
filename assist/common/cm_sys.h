#define IMPRECISE_CLOCK /* RWB */
/*
    Symbols, some of which should be defined on command line:

              -DUNIX

    define BUGGY when debugging.
    define INTERNAL_BUGGY when an "INTERNAL ERROR" error message occurs.
    define TYPE_BUGGY when debugging in order to force strong
           type checking at expense of data storage size.
    define TEST_BUGGY when partially debugging.
    define PRODUCTION when minimal debugging will be available only
           with the DEBUG$ statement in the program (ASSIST/SURE/PAWS)
           in question.

*/


#if !defined(_ZZ_q_cm_sys_DEFINED)
#define _ZZ_q_cm_sys_DEFINED
/*===========================================================================*/
#if defined(WPT_MODAL)||defined(WPT_PREFERRED)||defined(WPT_MODALPANEL)
????? The cm_sys.h file is incompatible with TAE. ?????
#else
/*===========================================================================*/

#include "cm_stdio.h"

#define NO_PARENS_IN_MODFILE_COMMENTS
#define QUESTIONABLE
#define HASH_BUGGY
/*  #define OLD_COMPARISONS */
/*  #define BUGGY       */
/*  #define TYPE_BUGGY  */
/*  #define TEST_BUGGY  */
/*  #define PRODUCTION  */
/*  ----------------------------------   IN CASE IMPLICIT CALLED W/ PARMS */
/*  #define IMPLICITS_USUALLY_NUMERIC */


                    /*******************************/
                    /*******************************/
                    /****                       ****/
                    /****   SYSTEM DEFINITION   ****/
                    /****                       ****/
                    /*******************************/
                    /*******************************/

#if defined(BUGGY)&&(!defined TEST_BUGGY)
#define TEST_BUGGY
#endif

                 /************************************/
                 /************************************/
                 /****                            ****/
                 /****   SYSTEM DEPENDENT STUFF   ****/
                 /****                            ****/
                 /************************************/
                 /************************************/

#ifdef NOTHING_MACHINE_KIND
#define FLOAT_64_BIT
#define INT_64_BIT
/**/
#endif
/*------------------------------  */
      //#if defined(MSDOS) || defined(LINUX)
#define FLOAT_32_BIT
#define INT_32_BIT
/**/
#include <limits.h>
#include <float.h>
/* <<RWB>> #include <unixio.h> */
#define MAXSHORT         SHRT_MAX
      //#define MAXLONG          LONG_MAX
#define BITSPERBYTE      CHAR_BIT
//#define MAXFLOAT         FLT_MAX
#define MAXDOUBLE        DBL_MAX
#define MINDOUBLE        DBL_MIN
      //#endif

             /*******************************************/
             /*******************************************/
             /****                                   ****/
             /****   MISCELLANEOUS DEPENDENT STUFF   ****/
             /****                                   ****/
             /*******************************************/
             /*******************************************/

/*------------------------------   FORM FEED */
#define FORM_FEED "\x0c"
#define FORM_FEED_ON_FIRST_PAGE FALSE

/*------------------------------   DEPENDENT DEFS */
#define BITS_PER_BYTE ((unsigned int) BITSPERBYTE)
#if BITSPERBYTE < 8
typedef unsigned short unsigned_255_type;
#else
typedef unsigned char unsigned_255_type; /* big enough for 0..255 */
#endif
typedef unsigned char unsigned_31_type;  /* big enough for 0..31 */

#ifdef STORAGE
unsigned short masks[] =
   {
      0x0000, 0x0001, 0x0003, 0x0007,
      0x000f, 0x001f, 0x003f, 0x007f,
      0x00ff, 0x01ff, 0x03ff, 0x07ff,
      0x0fff, 0x1fff, 0x3fff, 0x7fff,
      0xffff
   };
#else
extern unsigned short masks[];
#endif

/*---------------------------------------------  real limits */

#if defined(FLOAT_64_BIT) 
typedef float real_type;
#define MAX_REAL_TYPE ((real_type) MAXFLOAT)
#define MIN_REAL_TYPE ((real_type) MINFLOAT)
#endif

#if defined(FLOAT_32_BIT)
typedef double real_type;
#define MAX_REAL_TYPE ((real_type) MAXDOUBLE)
#define MIN_REAL_TYPE ((real_type) MINDOUBLE)
#endif

/*---------------------------------------------  integer limits */

//#define MAX_INT_TYPE ((int_type) MAXLONG)
#define MAX_INT_TYPE 2147483647
#if defined(INT_64_BIT) 
#define MAX_INT_TYPE_NDIGS   21 /* one char longer than encoded MAXINT */
#define MAX_INT_TYPE_NDIGS_P 22 /* two chars longer than encoded MAXINT */
typedef int int_type;
#endif

#if defined(INT_32_BIT) 
#define VFY_FOUR_BYTE_ALIGN
#define MAX_INT_TYPE_NDIGS   11 /* one char longer than encoded MAXINT */
#define MAX_INT_TYPE_NDIGS_P 12 /* two chars longer than encoded MAXINT */
typedef long int_type;
#endif

/*---------------------------------------------  formats */

#define HEX_FLAG_TYPE_FORMAT "02X"
#define HEX_INT_TYPE_FORMAT "08lX"
#define HEX_DEC_PLACES 8
#define INT_TYPE_FORMAT "ld"
#define REAL_TYPE_FORMAT "lf"
#define EXP_TYPE_FORMAT "le"
#define REAL_DEC_PLACES 15

/*---------------------------------------------  unix clock stuff */

#ifdef UNIX
#include </usr/include/unistd.h>
#endif

/*---------------------------------------------  clock() function ticks */

#if defined(UNIX)||defined(LINUX)
  #define IMPRECISE_CLOCK
  #if !defined(CLK_TCK)
    #define CLK_TCK (1000000)  
  #endif
  typedef struct stat stat_t;
#endif



            /**********************************************/
            /**********************************************/
            /****                                      ****/
            /****   OPERATING SYSTEM DEPENDENT STUFF   ****/
            /****      (ENTRYNAME/PATHNAME FORMAT)     ****/
            /****                                      ****/
            /**********************************************/
            /**********************************************/


#if defined(UNIX)||defined(LINUX)
#define FILE_DIRECTORY_TERMINATOR '/'     /* eg. /usr/people/etc/file.ext */
#define AUX_FILE_DIREC_TERMINATOR '/'     /* eg. /usr/people/etc/file.ext */
#define COMMANDLINE_OPTION_PREFIX '-'
#endif

#ifdef MSDOS
#define FILE_DIRECTORY_TERMINATOR '\\'    /* eg. C:\DIR\FILE.EXT */
#define AUX_FILE_DIREC_TERMINATOR ':'     /* eg. C:FILE.EXT */
#define COMMANDLINE_OPTION_PREFIX '/'
#endif

#if defined(MSDOS)
#define ENTRYNAME_MAXLEN 32 // RWB changed from 12
#define MAXNCH_PATH 256     // RWB changed from 128
#define MAXNCH_PATH_DIM 258 /* 2+MAXNCH_PATH */
#endif

#if defined(UNIX)||defined(LINUX)
#define ENTRYNAME_MAXLEN 32
#define MAXNCH_PATH 128
#define MAXNCH_PATH_DIM 130 /* 2+MAXNCH_PATH */
#endif

            /**********************************************/
            /**********************************************/
            /****                                      ****/
            /****   OPERATING SYSTEM DEPENDENT STUFF   ****/
            /****       (OPERATING SYSTEM PROMPT)      ****/
            /****                                      ****/
            /**********************************************/
            /**********************************************/

#if defined(MSDOS)
#define SYSTEM_PROMPT "> "
#endif

#if defined(LINUX) || defined(UNIX)
#define SYSTEM_PROMPT "$ "
#endif

              /******************************************/
              /******************************************/
              /****                                  ****/
              /****   SOME OPERATING SYSTEMS LIMIT   ****/
              /****   EXTENTS TO THREE CHARACTERS.   ****/
              /****   "ABC.ALOG" WOULD BE ILLEGAL.   ****/
              /****                                  ****/
              /******************************************/
              /******************************************/


#if defined(MSDOS)
#define OPTION_WITH_SLASH
#endif

#if defined(MSDOS)
#define THREE_CHARACTER_EXTENTS
#endif

              /******************************************/
              /******************************************/
              /****                                  ****/
              /****   ALL OPERATING SYSTEMS LIMIT    ****/
              /****    THE NUMBER OF BYTES WHICH     ****/
              /****   CAN BE ALLOCATED IN A SINGLE   ****/
              /****         CALL TO malloc()         ****/
              /****                                  ****/
              /******************************************/
              /******************************************/

#define BUCKET_WIDTH 51                    /* number of states/bucket */
#define BUCKET_COUNT 216091                /* hash bucket count */
#define EXTENSION_BUCKET_COUNT 1000        /* initial extension bucket count */

#ifdef MSDOS
#define MSDOS_HUGE_BUFFER_BYTES ((long) 0x00028000)
typedef long Memix;     /* must be signed because of pointer subtraction */
typedef long Stacktop;  /* must be signed because of (-1) for emptystack */
typedef long File_offset_t;
typedef unsigned long memsize_t;
#else
typedef int Memix;     /* must be signed because of pointer subtraction */
typedef int Stacktop;  /* must be signed because of (-1) for emptystack */
typedef int File_offset_t;
typedef unsigned int memsize_t;
#endif

#ifdef MSDOS
//#define g_malloc(b) malloc((((b)+7L)/8L),(size_t) 8)
#define g_realloc(p,new,old) realloc(p,new)
#define cfree(p) free(p)
#define g_free(p) free(p)
#define g_malloc(b) malloc(b)
#include <malloc.h>
#endif

#ifdef OBSOLETE
#define g_malloc(b) malloc(b)
#define g_realloc(p,new,old) realloc(p,new)
#define g_free(p) free(p)
#endif

#if (!defined(MSDOS))
#define g_free(p) free(p)
#endif

#define MALLOC_MAX (long) 0x01000000   /* 16MB safe only when size_t is long */


                  /*********************************/
                  /*********************************/
                  /****                         ****/
                  /****   FILE OPENING MACROS   ****/
                  /****                         ****/
                  /*********************************/
                  /*********************************/

#define acc_F_OK 0
#define acc_X_OK 1
#define acc_W_OK 2
#define acc_R_OK 4

#define fopen_read_only(path) fopen(path,"r")  /* read-only */
#define fopen_update_write(path) fopen(path,"w+") /* truncate & rewrite */

#if defined(MSDOS)
#define fopen_binary_text_read_only(path) fopen(path,"rb")
#define fopen_binary_text_update_write(path) fopen(path,"wb+")
#else
#define fopen_binary_text_read_only(path) fopen(path,"r")
#define fopen_binary_text_update_write(path) fopen(path,"w+")
#endif

#ifdef MSDOS
#define File_offset_t long
#else
#define File_offset_t int
#endif


                /*************************************/
                /*************************************/
                /****                             ****/
                /****   SYSTEM PRINTER DEFAULTS   ****/
                /****                             ****/
                /*************************************/
                /*************************************/

#ifdef BUGGY
#define DEFAULT_LISTING_LINES_PER_PAGE 72
#else
#define DEFAULT_LISTING_LINES_PER_PAGE 58
#endif

                /**************************************/
                /**************************************/
                /****                              ****/
                /****   DEFINE THE "NULL OFFSET"   ****/
                /****                              ****/
                /**************************************/
                /**************************************/

#if defined(INT_64_BIT)
#define NIX_ADDR     ((relative_address_type) 0xffffffffffffffff)
#define NIX_MEMIX                    ((Memix) 0xffffffffffffffff)
#define RELADDR_FLAG ((relative_address_type) 0x8000000000000000)
#define NULL_MSG "<.NULL.POINTER.>"
#endif
//#if defined(INT_60_BIT)
//#define NIX_ADDR     ((relative_address_type) 0xfffffffffffffff)
//#define NIX_MEMIX                    ((Memix) 0xfffffffffffffff)
//#define RELADDR_FLAG ((relative_address_type) 0x800000000000000)
//#define NULL_MSG "<.NULL..POINT.>"
//#endif
#if !defined(NIX_ADDR)
#define NIX_ADDR     ((relative_address_type) 0xffffffff)
#define NIX_MEMIX                    ((Memix) 0xffffffff)
#define RELADDR_FLAG ((relative_address_type) 0x80000000)
#define NULL_MSG "<.NULL.>"
#endif

   /***********************************************************************/
   /***********************************************************************/
   /****                                                               ****/
   /****   EXTENSIONS TO "ctype.h" TO INCREASE RULE GENERATION SPEED   ****/
   /****                                                               ****/
   /***********************************************************************/
   /***********************************************************************/

#if !defined(ISSPDFND)
#define is_space_or_print(c) ((isspace(c))||(isprint(c)))
#endif

  /**************************************************************************/
  /**************************************************************************/
  /****                                                                  ****/
  /****   MATH FUNCTION EXTENSIONS IN SOME COMPLIERS BUT NOT IN OTHERS   ****/
  /****                                                                  ****/
  /**************************************************************************/
  /**************************************************************************/

#define labs(i) (((i) < ((long int) 0)) ? ((long int) -(i)) : ((long int) (i)))


                   /***************************************/
                   /***************************************/
                   /****                               ****/
                   /****   MOVE COMPATABILITY MACROS   ****/
                   /****                               ****/
                   /***************************************/
                   /***************************************/

#define sys_memmove(dest,src,nch) memmove(dest,src,nch)
#define sys_memcpy(dest,src,nch) memcpy(dest,src,nch)
#define sys_memcmp(dest,src,nch) memcmp(dest,src,nch)
#define sys_memset(dest,val,nch) memset(dest,val,nch)

typedef char pathname_type[MAXNCH_PATH_DIM];

#ifdef IMPRECISE_CLOCK
#ifdef _NO_PROTO
extern long my_clock();
#else
extern long my_clock(void);
#endif
#else
#define my_clock() clock()
#endif

/*===========================================================================*/
#endif
/*===========================================================================*/
#endif
#define my_clock() clock() /* RWB */
