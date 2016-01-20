/*
===============================================================================

        Low level string manipulation functions required by ASSIST and SURE
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

#include "common.h"

#include "user_ext.h"
#include "astr_ext.h"

/*===========================================================================*/
Boolean pack_state_space_value_err(ssvar_value_type val,
                                   state_bitstring_type ssbits,
                                   state_offset_type *offinfo)
/*
   This function packs the state-space variable subscript "val" into the
   bitstring "ssbits" according to the offset information specified in
   "offinfo".   The minimum value for the subscript range is subtracted
   from the value "val" to obtain the relative index value before masking
   it into the bitstring.  If the subscript "val" does not fall between
   the minimum and maximum subscript values, inclusive, then an error message
   is displayed and the minimum subscript value is packed so that error
   recovery will be complete.

   For example, if the space is 7501..7504 and val=7503, then the relative
   index will be computed as 2 (7503-(minval=7501)) or 10 binary for the bit
   pattern which will be masked into the bitstring "ssbits".
*/
{
   ssvar_value_type relative_val,vvv;
   bitsize_type uval,byteoff,temp;
   bitsize_type bitoff,bitlen,bitava,ns,nrem;
   bitsize_type ByteSize;
   unsigned char *ppp;
   Boolean in_error;
   /*-------------------------------------------------------*/
   ByteSize = BITS_PER_BYTE;
#ifdef BUGGY
fprintf(bugfile,
"packing: val=%d, offinfo=(%d-%d,%d@%d), ByteSize=%d, BITS_PER_BYTE=%d aaa=%x\n",
(int) val,
(*offinfo).minval,
(*offinfo).maxval,
(*offinfo).bit_length,
(*offinfo).bit_offset,
(int) ByteSize,
(int) BITS_PER_BYTE,
(int) (&ByteSize));
#endif
   bitlen = (*offinfo).bit_length;
   byteoff = (*offinfo).bit_offset / ByteSize;
   bitoff = (*offinfo).bit_offset % ByteSize;
   bitava = ByteSize - bitoff;
   /*---------------------------------------------------  test val vs range */
   vvv=val;
   if (in_error=ssvar_vs_range_err(val,offinfo)) vvv=0;
   /*---------------------------------------------------  compute index */
   relative_val = vvv - (*offinfo).minval;
   uval = (bitsize_type) relative_val;
   /*---------------------------------------------------  mask into val */
   ppp = (unsigned char *) ssbits;
   ppp += byteoff;
   if (bitava >= (*offinfo).bit_length){
      /*---------------------------------- does not straddle two bytes */
      ns = bitava - bitlen;
      /*----------------  mask out old */
      temp = masks[bitlen];
      temp = ~(temp << ns);
      (*ppp) &= temp;
      /*----------------  mask in new */
      temp = uval & masks[bitlen];
      temp = temp << ns;
      (*ppp) |= temp;}
   else{
      /*---------------------------------- straddles two bytes */
      nrem = ns = bitlen - bitava;
      /*----------------  mask old out of 1st byte */
      temp = ~(masks[bitava]);
      (*ppp) &= temp;
      /*----------------  mask new into 1st byte */
      temp = uval >> ns;
      temp &= masks[bitava];
      (*ppp) |= temp;
#ifdef BIGSPACE
      /*----------------  mask new into middle byte */
      if (nrem > ByteSize){
         ++ppp;
         nrem = ns = ns - ByteSize;
         temp = uval >> ns;
         temp &= masks[ByteSize];
         *ppp = temp;}
#endif
      /*----------------  go to final byte */
      ++ppp;
      ns = ByteSize - ns;
      /*----------------  mask old out of final byte */
      temp = ~(masks[nrem] << ns);
      (*ppp) &= temp;
      /*----------------  mask new into final byte */
      temp = uval & masks[nrem];
      temp = temp << ns;
      (*ppp) |= temp;}
   /*-------------------------------------------------------*/
   return(in_error);
}

/*===========================================================================*/
void set_bit(unsigned char *sss,bitsize_type bitoff)
{
   unsigned char *ppp;
   bitsize_type ByteSize;
   bitsize_type byteoff,ns;
   unsigned char ccc;
   /*-------------------------------------------------------*/
   ByteSize = BITS_PER_BYTE;
   byteoff = bitoff / ByteSize;
   bitoff = bitoff % ByteSize;
   ppp = sss + byteoff;
   ns = ByteSize - bitoff - 1;
   ccc = (unsigned char) (masks[1]);
   ccc = ((unsigned char) (ccc << ns));
   *ppp = ((unsigned char) (*ppp | ccc));
   /*-------------------------------------------------------*/
}


/*===========================================================================*/
void clear_bit(unsigned char *sss,bitsize_type bitoff)   // RWB 8/01
{
   unsigned char *ppp;
   bitsize_type ByteSize;
   bitsize_type byteoff,ns;
   unsigned char ccc;

   ByteSize = BITS_PER_BYTE;
   byteoff = bitoff / ByteSize;
   bitoff = bitoff % ByteSize;

   ppp = sss + byteoff;
   ns = ByteSize - bitoff - 1;
   ccc = (unsigned char) (masks[1]);
   ccc = ((unsigned char) (ccc << ns));
   ccc = ~ccc;

   *ppp = ((unsigned char) (*ppp & ccc));

   /*-------------------------------------------------------*/
}



/*===========================================================================*/
unsigned char clear_byte_tail(unsigned char *sss,bitsize_type bitoff)
{
   register unsigned char *ppp;
   register bitsize_type ByteSize;
   register bitsize_type byteoff,ns;
   register unsigned char ccc,oldchar;
   /*-------------------------------------------------------*/
   ByteSize = BITS_PER_BYTE;
   byteoff = bitoff / ByteSize;
   bitoff = bitoff % ByteSize;
   ppp = sss + byteoff;
   ns = ByteSize - bitoff - 1;
   ccc = (unsigned char) (masks[ns]);
   ccc = (unsigned char) (~ ccc);
   oldchar = *ppp;
   *ppp = (*ppp & ccc);
   /*-------------------------------------------------------*/
   return(oldchar);
}
/*===========================================================================*/
Boolean test_bit(unsigned char *sss,bitsize_type bitoff)
{
   register Boolean ans;
   register unsigned char *ppp;
   register bitsize_type ByteSize;
   register bitsize_type byteoff,ns;
   register unsigned char ccc;
   /*-------------------------------------------------------*/
   ByteSize = BITS_PER_BYTE;
   byteoff = bitoff / ByteSize;
   bitoff = bitoff % ByteSize;
   ppp = sss + byteoff;
   ns = ByteSize - bitoff - 1;
   ccc = *ppp;
   ccc = (unsigned char) (ccc >> ns);
   ans = (unsigned char) (ccc & 0x01);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
ssvar_value_type state_space_value
     (state_bitstring_type ssbits,state_offset_type *offinfo)
/*
   This function masks off and returns the state-space variable subscript
   from the the bitstring "ssbits" according to the offset information 
   specified in "offinfo".  The mininum value for the subscript range is
   added to the masked off relative index value before it is returned.

   For example, if the space is 7501..7504 and the bit pattern is 10 (binary),
   then the value returned will be 7503 ((minval=7501)+((10(binary))=2)).
*/
{
   register ssvar_value_type ans;
   register unsigned char *ppp;
   register bitsize_type uval,byteoff,temp;
   register bitsize_type bitoff,bitlen,bitava,ns,nrem;
   register bitsize_type ByteSize;
   /*-------------------------------------------------------*/
   if (ssbits == ((state_bitstring_type) NULL)){
      show_err_on_both(E_EVALSSVPRS,(void *) NULL,EMPTY_TYPE);
      return(0);}
   ByteSize = BITS_PER_BYTE;
   bitlen = (*offinfo).bit_length;
   byteoff = (*offinfo).bit_offset / ByteSize;
   bitoff = (*offinfo).bit_offset % ByteSize;
   bitava = ByteSize - bitoff;
   /*---------------------------------------------------  mask out index */
   ppp = (unsigned char *) ssbits;
   ppp += byteoff;
#ifdef BUGGY
fprintf(bugfile,"offinfo=(%d-%d,%d@%d),byteoff=%d,bitoff=%d,bitava=%d\n",
(int) (*offinfo).minval,
(int) (*offinfo).maxval,
(int) (*offinfo).bit_length,
(int) (*offinfo).bit_offset,
(int) byteoff,
(int) bitoff,
(int) bitava);
#endif
   if (bitava >= (*offinfo).bit_length){
      /*---------------------------------- does not straddle two bytes */
      ns = bitava - bitlen;
      temp = ((*ppp) >> ns);
#ifdef BUGGY
fprintf(bugfile,"        ns=%d,temp=%d,",(int) ns,(int) temp);
#endif
      uval = temp & masks[bitlen];}
   else{
      /*---------------------------------- straddles two bytes */
      nrem = ns = bitlen - bitava;
      /*----------------  mask out of 1st byte */
      temp = ((*ppp) & masks[bitava]) << ns;
#ifdef BIGSPACE
      /*----------------  go to middle byte */
      if (nrem > ByteSize){
         ++ppp;
         nrem = ns = ns - ByteSize;
         uval = (*ppp);
         uval = uval << ns;
         temp |= uval;}
#endif
      /*----------------  go to final byte */
      ++ppp;
      ns = ByteSize - ns;
      /*----------------  mask out of final byte */
      uval = masks[nrem] & ((*ppp) >> ns);
      uval = uval | temp;}
   /*---------------------------------------------------  add to minval */
#ifdef BUGGY
fprintf(bugfile,"uval=%d\n",(int) uval);
#endif
   ans = uval;
   ans += (*offinfo).minval;
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
ssvar_value_type state_space_arr_value
     (state_bitstring_type ssbits,state_offset_type *offinfo,Subscript off)
{
   state_offset_type oif;
   register ssvar_value_type ans;
   FAST_STATE_OFFSET_REGS();
   /*-------------------------------------------------------*/
   FAST_STATE_OFFSET_COPY(&oif,offinfo);
   oif.bit_offset += oif.bit_length*off;
   ans = state_space_value(ssbits,&oif);
   /*-------------------------------------------------------*/
   return(ans);
}
/*===========================================================================*/
