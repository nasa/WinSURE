#if !defined(_ZZ_q_astrw_DEFINED)
#define _ZZ_q_astrw_DEFINED
/*===========================================================================*/

                   /*********************************/
                   /*********************************/
                   /****                         ****/
                   /****   RESERVED WORD TYPES   ****/
                   /****                         ****/
                   /****                         ****/
                   /****  PREDEFINED  CONSTANTS  ****/
                   /****           AND           ****/
                   /****  DESCRIPTIVE OPERATORS  ****/
                   /****       ARE OMITTED       ****/
                   /****                         ****/
                   /*********************************/
                   /*********************************/

/*
     Note:  Reserved words are partitioned as follows:

                  0-127  reserved for user applications
                128-255  reserved for common library/expression parsing


    Note: holes can be used to reserve room for adding new reserved
          words, however there can, for no language (SURE,ASSIST,PAWS,etc),
          be any holes in the range 0..RW_STATEMENT_COUNT.
          See "*tabs.h" files for details.
*/

/*--------------------------------------- statement names first */
#define RW_DEBUG      ((rwtype) 0) /* ASSIST/SURE/PAWS/etc */
#define RW_CLOPT      ((rwtype) 1) /* ASSIST/SURE/PAWS/etc */
#define RW_INPUT      ((rwtype) 2) /* ASSIST/SURE/PAWS/etc */
#ifdef ASSIST
#define RW_SPACE      ((rwtype) 3) /* ASSIST ONLY */
#define RW_FUNCTION   ((rwtype) 4) /* ASSIST ONLY */
#define RW_IMPLICIT   ((rwtype) 5) /* ASSIST ONLY */
#define RW_INTRINSIC  ((rwtype) 6) /* ASSIST ONLY */
#define RW_VARIABLE   ((rwtype) 7) /* ASSIST ONLY */
#define RW_START      ((rwtype) 8) /* ASSIST ONLY */
#define RW_DEATHIF    ((rwtype) 9) /* ASSIST ONLY */
#define RW_PRUNEIF    ((rwtype) 10) /* ASSIST ONLY */
#define RW_ASSERT     ((rwtype) 11)/* ASSIST ONLY */
#define RW_TRANTO     ((rwtype) 12)/* ASSIST ONLY */
#define RW_IF         ((rwtype) 13)/* ASSIST ONLY */
#define RW_FOR        ((rwtype) 14)/* ASSIST ONLY */
#define FIRST_OPTION_DEF_RW RW_ONEDEATH
#define RW_ONEDEATH   ((rwtype) 15)/* ASSIST ONLY */
#define RW_COMMENT    ((rwtype) 16)/* ASSIST ONLY */
#define RW_MODSTATS   ((rwtype) 17)/* ASSIST ONLY */
#define RW_ECHO       ((rwtype) 18)/* ASSIST ONLY */
#define RW_TRIM       ((rwtype) 19)/* ASSIST ONLY */
#define RW_STATEMENT_COUNT 20   /* 0..19 for total of 20 */
#endif
#if defined(SURE)||defined(PAWS)
#define RW_SHOW       ((rwtype) 3) /* SURE/PAWS/etc. but not ASSIST */
#define RW_READ       ((rwtype) 4) /* SURE/PAWS/etc. but not ASSIST */
#define RW_SAVE       ((rwtype) 5) /* SURE/PAWS/etc. but not ASSIST */
#define RW_RUN        ((rwtype) 6) /* SURE/PAWS/etc. but not ASSIST */
#define RW_EXIT       ((rwtype) 7) /* SURE/PAWS/etc. but not ASSIST */
#define RW_GET        ((rwtype) 8) /* SURE/PAWS/etc. but not ASSIST */
#define RW_HELP       ((rwtype) 9) /* SURE/PAWS/etc. but not ASSIST */
#define RW_SCAN       ((rwtype) 10)/* SURE/PAWS/etc. but not ASSIST */
#define RW_ZOOM       ((rwtype) 11)/* SURE/PAWS/etc. but not ASSIST */
#define RW_CALC       ((rwtype) 12)/* SURE/PAWS/etc. but not ASSIST */
#define RW_DISP       ((rwtype) 13)/* SURE/PAWS/etc. but not ASSIST */
#define RW_LEE        ((rwtype) 14)/* SURE/PAWS/etc. but not ASSIST */
#define RW_CLR        ((rwtype) 15)/* SURE/PAWS/etc. but not ASSIST */
#define RW_PLOT       ((rwtype) 16)/* SURE/PAWS/etc. but not ASSIST */
#define RW_PLTI       ((rwtype) 17)/* SURE/PAWS/etc. but not ASSIST */
#define RW_PLT3D      ((rwtype) 18)/* SURE/PAWS/etc. but not ASSIST */
#define RW_ORPROB     ((rwtype) 19)/* SURE/PAWS/etc. but not ASSIST */
#define RW_INIT       ((rwtype) 20)/* SURE/PAWS/etc. but not ASSIST */
#define RW_READ0      ((rwtype) 21)/* SURE/PAWS/etc. but not ASSIST */
#define RW_PRUNE      ((rwtype) 22)/* SURE/PAWS/etc. but not ASSIST */
#define RW_STATEMENT_COUNT 23  /* 0..22 for total of 23 */
#endif
/*--------------------------------------- names of keywords */
#define RW_FAST       ((rwtype) 70)
#ifdef ASSIST
#define RW_BY         ((rwtype) 71)/* ASSIST only */
#define RW_THEN       ((rwtype) 72)/* ASSIST only */
#define RW_ELSE       ((rwtype) 73)/* ASSIST only */
#define RW_ELSIF      ((rwtype) 74)/* ASSIST only */
#define RW_ENDIF      ((rwtype) 75)/* ASSIST only */
#define RW_ENDFOR     ((rwtype) 76)/* ASSIST only */
#endif
#if defined(SURE)||defined(PAWS)
#define RW_END        ((rwtype) 81)/* SURE/PAWS/etc. but not ASSIST */
#define RW_TO         ((rwtype) 82)/* SURE/PAWS/etc. but not ASSIST */
#define RW_INTO       ((rwtype) 83)/* SURE/PAWS/etc. but not ASSIST */
#define RW_UPTO       ((rwtype) 84)/* SURE/PAWS/etc. but not ASSIST */
#define RW_DOWNTO     ((rwtype) 85)/* SURE/PAWS/etc. but not ASSIST */
#endif
/*--------------------------------------- names of keywords */
#ifdef ASSIST
#endif
/*--------------------------------------- */
#define RW_CLINE_ARG  ((rwtype) 101)
/*===========================================================================*/
#include "rwdefs.h"
/*===========================================================================*/
#endif
