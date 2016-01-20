#if !defined(_ZZ_q_asttabsP_DEFINED)
#define _ZZ_q_asttabsP_DEFINED
/*===========================================================================*/

#include "aprs_ext.h"
#include "astrw.h"

static char *code_titles[] =
    {   /* ordering for (1+(0..RULE_OPCODE_INDEX_COUNT)) */
        "model generation code, PREAMBLE section",
        "model generation code, ASSERT section",
        "model generation code, DEATHIF section",
        "model generation code, PRUNEIF section",
        "model generation code, TRANTO section",
        "model generation code, CALC-booltest section",
        "model generation code, CALC-transition section"
    };

static any_parse_statement_routine
   parse_statement_routine[RW_STATEMENT_COUNT] =
     {
         parse_DEBUG_statement,    /* RW_DEBUG */
         parse_CLOPT_statement,    /* RW_CLOPT */
         parse_INPUT_statement,    /* RW_INPUT */
         parse_SPACE_statement,    /* RW_SPACE */
         parse_FUNCTION_statement, /* RW_FUNCTION */
         parse_IMPLICIT_statement, /* RW_IMPLICIT */
         parse_IMPLICIT_statement, /* RW_INTRINSIC */
         parse_VARIABLE_statement, /* RW_VARIABLE */
         parse_START_statement,    /* RW_START */
         parse_DEATHIF_statement,  /* RW_DEATHIF */
         parse_PRUNEIF_statement,  /* RW_PRUNEIF */
         parse_ASSERT_statement,   /* RW_ASSERT */
         parse_TRANTO_statement,   /* RW_TRANTO */
         parse_IF_statement,       /* RW_IF */
         parse_FOR_statement,      /* RW_FOR */
         parse_ONEDEATH_def_stat,  /* RW_ONEDEATH */
         parse_COMMENT_def_stat,   /* RW_COMMENT */
         parse_MODSTATS_def_stat,  /* RW_MODSTATS */
         parse_ECHO_def_stat,      /* RW_ECHO */
         parse_TRIM_def_stat,      /* RW_TRIM */
     };

static mode_flag_type rw_okay_in_these_modes[RW_STATEMENT_COUNT] =
     {
         ANY_SECTION_MODE,                 /* RW_DEBUG */
         SETUP_SECTION_MODE,               /* RW_CLOPT */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_INPUT */
         SETUP_SECTION_MODE,               /* RW_SPACE */
         START_SECTION_MODE,               /* RW_FUNCTION */
         START_SECTION_MODE,               /* RW_IMPLICIT */
         START_SECTION_MODE,               /* RW_INTRINSIC */
         START_SECTION_MODE,               /* RW_VARIABLE */
         START_SECTION_MODE,               /* RW_START */
         ANY_POST_START_SECTION_MODE,      /* RW_DEATHIF */
         ANY_POST_START_SECTION_MODE,      /* RW_PRUNEIF */
         ANY_POST_START_SECTION_MODE,      /* RW_ASSERT */
         ANY_POST_START_SECTION_MODE,      /* RW_TRANTO */
         ANY_POST_START_SECTION_MODE,      /* RW_IF */
         ANY_POST_START_SECTION_MODE,      /* RW_FOR */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_ONEDEATH */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_COMMENT */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_MODSTATS */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_ECHO */
         ANY_PRE_RULE_SECTION_MODE,        /* RW_TRIM */
     };

static mode_flag_type rw_transitions_to_this_mode[RW_STATEMENT_COUNT] =
     {
         SAME_MODE,                 /* RW_DEBUG */
         SAME_MODE,                 /* RW_CLOPT */
         SAME_MODE,                 /* RW_INPUT */
         GOTO_START_SECTION_MODE,   /* RW_SPACE */
         SAME_MODE,                 /* RW_FUNCTION */
         SAME_MODE,                 /* RW_IMPLICIT */
         SAME_MODE,                 /* RW_INTRINSIC */
         SAME_MODE,                 /* RW_VARIABLE */
         GOTO_START_SECTION_MODE,   /* RW_START */
         GOTO_RULE_SECTION_MODE,    /* RW_DEATHIF */
         GOTO_RULE_SECTION_MODE,    /* RW_PRUNEIF */
         GOTO_RULE_SECTION_MODE,    /* RW_ASSERT */
         GOTO_RULE_SECTION_MODE,    /* RW_TRANTO */
         GOTO_RULE_SECTION_MODE,    /* RW_IF */
         GOTO_RULE_SECTION_MODE,    /* RW_FOR */
         SAME_MODE,                 /* RW_ONEDEATH */
         SAME_MODE,                 /* RW_COMMENT */
         SAME_MODE,                 /* RW_MODSTATS */
         SAME_MODE,                 /* RW_ECHO */
         SAME_MODE,                 /* RW_TRIM */
     };
/*===========================================================================*/
#endif
