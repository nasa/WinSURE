/*
===============================================================================

        Low level input/output functions required by ASSIST and SURE 
        compiler programs.

        Originally conceived of and implemented in PASCAL by:
              Ricky W. Butler      (NASA Langley Research Center)
              Sally C. Johnson     (NASA Langley Research Center)

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"
#include "cm_types.h"
#include "cm_vars.h"

#include "errdefs.h"

#include "user_ext.h"
#include "lio_ext.h"

/*===========================================================================*/
void check_errno(short eedflt,char *filename)
/*
   This function prints the system error message after an i/o error has 
   just occurred.   Filename can be passed in as ((char *) NULL) if no
   filename is to be printed next to the error message.   If called, control
   will return to the operating system.
*/
{
   /*-------------------------------------------------------*/
   if (errno==ENOENT)
      show_err_on_both(E_FILENOTFOUND,(void *) filename,CHAR_TYPE);
   else if (errno==EACCES)
      show_err_on_both(E_NORIGHTS,(void *) filename,CHAR_TYPE);
   else if (errno==EMFILE)
      show_err_on_both(E_TOOMANYFILES,(void *) filename,CHAR_TYPE);
   else if (errno==EDQUOT)
      show_err_on_both(E_QUOTA,(void *) filename,CHAR_TYPE);
   else if (errno==ENOSPC)
      show_err_on_both(E_QUOTA,(void *) filename,CHAR_TYPE);
   else if (errno==EFBIG)
      show_err_on_both(E_QUOTA,(void *) filename,CHAR_TYPE);
   else if (errno==EROFS)
      show_err_on_both(E_READONLY,(void *) filename,CHAR_TYPE);
   else
      show_err_on_both(eedflt,(void *) filename,CHAR_TYPE);
   abort_program();
   /*-------------------------------------------------------*/
}
/*===========================================================================*/
FILE *open_input_file(char *filename)
/*
   This function attempts to open an input file.
*/
{
   FILE *ans;
   /*-------------------------------------------------------*/
   ans=fopen_read_only(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/
FILE *open_output_file(char *filename)
{
   FILE *ans;
   /*-------------------------------------------------------*/
   ans=fopen_update_write(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/
FILE *open_binary_input_file(char *filename)
{
   FILE *ans;
   /*-------------------------------------------------------*/
   ans=fopen_binary_text_read_only(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/
FILE *open_binary_output_file(char *filename)
{
   FILE *ans;
   /*-------------------------------------------------------*/
   ans=fopen_binary_text_update_write(filename);
   if (ans==((FILE *) NULL)) check_errno(E_CANNOTOPEN,filename);
   /*-------------------------------------------------------*/
   return (ans);
}
/*===========================================================================*/
flagtyp exists(char *path)
{
   register flagtyp does;
   /*-------------------------------------------------------*/
   does = (flagtyp) ((access(path,acc_F_OK))==0);
   /*-------------------------------------------------------*/
   return(does);
}
/*===========================================================================*/
flagtyp okay_to_read(char *path)
{
   register flagtyp does;
   /*-------------------------------------------------------*/
   does = (flagtyp) ((access(path,acc_R_OK))==0);
   /*-------------------------------------------------------*/
   return(does);
}
/*===========================================================================*/
flagtyp okay_to_write(char *path)
{
   register flagtyp does;
   /*-------------------------------------------------------*/
   does = (flagtyp) ((access(path,acc_W_OK))==0);
   /*-------------------------------------------------------*/
   return(does);
}
/*===========================================================================*/
flagtyp okay_to_delete(char *path)
{
   register flagtyp does;
   /*-------------------------------------------------------*/
   does = (flagtyp) ((access(path,acc_W_OK))==0);
   /*-------------------------------------------------------*/
   return(does);
}
/*===========================================================================*/
relative_address_type relative_identifier(identifier_info_type *idinfo)
{
   register relative_address_type ans;
   /*-------------------------------------------------------*/
   ans = idinfo - identifier_table;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
identifier_info_type *absolute_identifier(relative_address_type reladdr)
{
   register identifier_info_type *ans;
   /*-------------------------------------------------------*/
   ans = identifier_table + reladdr;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/

