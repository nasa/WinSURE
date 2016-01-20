#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* $Log$
 */
/*
===============================================================================

        Original rewrite in "C" by:
              David P. Boerschlein (Lockheed Engineering & Sciences Corp).

        Modification History:
              3/91   Original rewrite completed (David Boerschlein)

===============================================================================
*/

#include "commonio.h"

short siglen(char *str,short nch)
/*
   This function computes the significant length of a string "nch" characters
   long.   If "nch" is specified as a negative number, then the string "str"
   must be null terminated.
*/
{
   register short ncols,col;
   /*-------------------------------------------------------*/
   ncols=nch;
   if (ncols<0) ncols=strlen(str);
   for (col=ncols-1;col>=0;--col)
      if (!isspace(*(str+col))) return((short) (col+1));
   /*-------------------------------------------------------*/
   return(0);
}

flagtyp safe_strcat_truncated(char *dest,char *src,int dest_bufflen)
{
   int nchdest,nchsrc;
   /*-------------------------------------------------------*/
   nchdest = strlen(dest);
   nchsrc = strlen(src);
   if ((nchdest+nchsrc)<dest_bufflen) strcat(dest,src);
   else{
      strncat(dest,src,(int) (dest_bufflen-nchdest));
      *(dest+dest_bufflen-1) = '\0';
      return(f_TRUE);}
   /*-------------------------------------------------------*/
   return(f_FALSE);
}
