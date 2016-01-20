#include "lib_def.h"
#include "cm_sys.h"
#include "cm_defs.h"

#include "gammalim.h"
#include "gamma.h"

static real_type zero = 0.000000000000000;
static real_type pos_one = 1.000000000000000;
static real_type neg_one = (-1.000000000000000);
static real_type pos_four = 4.00000000000000;
static real_type pos_twelve = 12.0000000000000;
static real_type pos_three_half = 1.5000000000000;
static real_type pos_one_half = 0.5000000000000;
static real_type pos_two = 2.000000000000;
static real_type pos_thousand = 1000.000000000000;

int cgamma(real_type p,real_type *gamma_out)
{ return(qq_gamma(p,gamma_out,FALSE)); }


int lngamma(real_type p,real_type *gamma_out)
{ return(qq_gamma(p,gamma_out,TRUE)); }


int qq_gamma(real_type p,real_type *gamma_out,Boolean for_lngamma)
/*
      C3.6
      C3.7
 
 -CGAMMA--------S-------LIBRARY 3--------------------------------------
 -LNGAMA
 
  FUNCTION    cgamma - GAMMA FUNCTION OF A REAL ARGUMENT P
              lngama - NATURAL LOG OF THE GAMMA FUNCTION OF A REAL
                         ARGUMENT P
  USAGE              - IERR=cgamma(P,GAMMA)
                     - IERR=lngama(P,GAMMA)
  PARAMETERS  P      - INPUT ARGUMENT
              GAMMA - OUTPUT RESULT. FOR CGAMMA, GAMMA IS THE VALUE
                    OF THE GAMMA FUNCTION. FOR LNGAMA, GAMMA IS
                    THE VALUE OF THE LOG OF THE ABSOLUTE VALUE
                    OF THE GAMMA FUNCTION.
              IERR    - ERROR PARAMETER.
                      IERR=1 INDICATES P WAS ZERO OR A NEGATIVE
                           INTEGER , OR P WAS GREATER THAN OR EQUAL
                           TO 177.803  OR LESS THAN OR EQUAL TO -176.803
                           FOR ENTRY CGAMMA OR ABS(P) WAS GREATER THAN
                           OR EQUAL TO 1.723E319 FOR ENTRY LNGAMA.
                           GAMMA IS RETURNED AS THE LARGEST MACHINE
                           NUMBER WITH APPROPRIATE SIGN.
  PRECISION          - SINGLE ON CDC, DOUBLE OTHERWISE
  LANGUAGE           - FORTRAN TRANSLATED TO "C"
 ----------------------------------------------------------------------
    LATEST REVISION     - MAY 1, 1978
    INITIAL CONVERSION  - FEB 1, 1991
 
*/
{
/*
                  COEFFICIENTS FOR MINIMAX
                  APPROXIMATION TO GAMMA(P),
                  2.0 .LE. P .LE. 3.0
*/
   static real_type p5[] =
      {
          3.4109112397125E+01,
          -4.8341273405598E+01,
          4.3005887829594E+02,
          -5.5688734338586E+01,
          2.0585220673644E+03,
          7.7192407739800E-01,
          -3.1721064346240E+00
      };
   static real_type q5[] =
      {
          2.4455138217658E+02,
          -1.0174768492818E+03,
          1.1615998272754E+03,
          2.0512896777972E+03,
          6.8080353498091E-01,
          -2.5386729086746E+01
      };
/*
                  COEFFICIENTS FOR MINIMAX
                  APPROXIMATION TO LN(GAMMA(P)),
                  0.5 .LE. P .LE. 1.5
*/
   static real_type p1[] =
      {
          2.4317524352442E+02,
          -2.6172185838561E+02,
          -9.2226137288015E+02,
          -5.1763834980232E+02,
          -7.7410640713329E+01,
          -2.2088439972161E+00,
          4.1208431858477E+00,
          8.5689820628313E+01
      };
   static real_type q1[] =
      {
          3.7783724848239E+02,
          9.5132359767970E+02,
          8.4607553620207E+02,
          2.6230834702694E+02,
          2.4435196625063E+01,
          4.0977929210926E-01,
          4.5646771875859E+01
      };
/*
                  COEFFICIENTS FOR MINIMAX
                  APPROXIMATION TO LN(GAMMA(P)),
                  1.5 .LE. P .LE. 4.0
*/
   static real_type p2[] =
      {
          5.268983255914E+03,
          1.9553605540630E+04,
          1.2043173809871E+04,
          -2.0648294205325E+04,
          -1.5086302287667E+04,
          -1.5138318341150E+03,
          5.1550576176408E+00,
          3.7751067979721E+02
      };
   static real_type q2[] =
      {
          3.0399030414394E+03,
          2.2029562144156E+04,
          5.7120255396025E+04,
          5.2622863838411E+04,
          1.4402090371700E+04,
          6.9832741405735E+02,
          1.2890931890129E+02
      };
/*
                  COEFFICIENTS FOR MINIMAX
                  APPROXIMATION TO LN(GAMMA(P)),
                  4.0 .LE. P .LE. 12.0
*/
   static real_type p3[] = 
      {
          -1.9718301158609E+07,
          -8.7316754382383E+07,
          1.1193853542998E+08,
          4.8180771027736E+08,
          -2.4483217690328E+08,
          -2.4079869801733E+08,
          -1.0377016517329E+04,
          -9.8271022814204E+05
      };
   static real_type q3[] =
      {
          -3.1140628473406E+05,
          -1.0485775830499E+07,
          -1.1192541162633E+08,
          -4.0443592829143E+08,
          -4.3537071480437E+08,
          -7.9026111141876E+07,
          -2.0152751955004E+03
      };
/*
                  COEFFICIENTS FOR MINIMAX
                  APPROXIMATION TO LN(GAMMA(P)),
                  12.0 .LE. P
*/
   static real_type p4[] =
      {
          9.1893853320467E-01,
          8.3333333333267E-02,
          -2.7777776505151E-03,
          7.9358449768E-04,
          -5.82399983E-04
      };
/**/
   register short j;
   int ierr;
   Boolean negative,incomplete;
   long i,ir;
   real_type sign,gammavalue,t,a,b,top,den,rr;
   /*------------------------------------------------------------------*/
   rr = 0;
   if (for_lngamma) goto s1;
   ierr = 0;
   sign = neg_one;
   gammavalue = zero;
   negative = FALSE;
   incomplete = TRUE;
   t = p;
   if (t > zero) goto s15;
   if (-t > zero) goto s5;
   ierr = 1;
   negative = TRUE;
   goto s125;
s1:
   gammavalue = zero;
   negative = FALSE;
   incomplete = FALSE;
   t = p;
   if (t > zero) goto s20;
   if (t < zero) goto s5;
   ierr = 1;
   goto s125;
/*                                 ARGUMENT IS NEGATIVE                 */
s5:
   negative = TRUE;
   t = (-t);
   rr = ir = ((long) (t));
   sign = pos_one;
   if ((ir % 2) == 0) sign = neg_one;
   rr = t-rr;
   if (rr != zero) goto s10;
   ierr = 1;
   if (incomplete) goto s125;
   negative = FALSE;
   goto s125;
/*                                 ARGUMENT IS NOT A NEGATIVE INTEGER   */
s10:
   rr = PI/sin(rr*PI)*sign;
   t = t + pos_one;
   if (incomplete) goto s15;
   rr = log(fabs(rr));
   goto s20;
/*                                 LOOKING FOR GAMMA(T), T .GE 0.0      */
s15:
    if (t < ARGLIMIT) goto s40;
    ierr = 1;
    goto s125;
/*                                 LOOKING FOR LN(GAMMA(T)),T .GT. 0.0 */
s20:
   if (t < MAX_REAL_TYPE) goto s25;
   ierr = 1;
   negative = FALSE;
   goto s125;
s25:
   ierr = 0;
   if (t <= pos_four) goto s30;
   if (t <= pos_twelve) goto s100;
   goto s110;
s30:
   if (t >= pos_three_half) goto s90;
   if (t >= pos_one_half) goto s75;
   b = (-log(t));
   if (t <= MACHINE_PRECISION) goto s35;
   a = t;
   t = t + pos_one;
   goto s80;
s35:
   gammavalue = b;
   goto s9005;
s40:
   ierr = 0;
   if (t > pos_twelve) goto s110;
   i = (long) t;
   a = pos_one;
   if (i > 2) goto s55;
   i++;
   if (i==1) goto s45;
   if (i==2) goto s50;
   if (i==3) goto s65;
/*                                 0.0 .LT. T .LT. 1.0                  */
s45:
   a = a/(t*(t+pos_one));
   t = t+pos_two;
   goto s65;
/*                                 1.0 .LE. T .LT. 2.0                  */
s50:
   a = a/t;
   t = t+pos_one;
   goto s65;
/*                                 3.0 .LE. T .LE. 12.0                 */
s55:
   for (j=3;j<=i;++j){
      t = t+neg_one;
      a = a*t;}
/*                                 2.0 .LE. T .LE. 3.0                  */
/*                                 EVALUATE MINIMAX APPROXIMATION FOR   */
/*                                 GAMMA.                               */
s65:
   top = p5[5]*t+p5[6];
   den = t+q5[5];
   for (j=0;j<5;++j){
      top = top*t+p5[j];
      den = den*t+q5[j];}
   gammavalue = (top/den)*a;
   if (negative) gammavalue = rr/gammavalue;
   goto s9005;
/*                                 0.5 .LE. T .LT. 1.5                  */
/*                                 EVALUATE MINIMAX APPROXIMATION FOR   */
/*                                 LN(GAMMA(P)).                        */
s75:
   b = 0;
   a = (t-pos_one_half)-pos_one_half;
s80:
   top = p1[6]*t+p1[7];
   den = t+q1[6];
   for (j=0;j<6;++j){
      top = top*t+p1[j];
      den = den*t+q1[j];}
   gammavalue = (top/den)*a+b;
   if (negative) gammavalue = rr-gammavalue;
   goto s9005;
/*                                 1.5 .LE. T .LE. 4.0                  */
/*                                 EVALUATE MINIMAX APPROXIMATION FOR   */
/*                                 LN(GAMMA(P)).                        */
s90:
   a = (t-pos_one)-pos_one;
   top = p2[6]*t+p2[7];
   den = t+q2[6];
   for (j=0;j<6;++j){
      top = top*t+p2[j];
      den = den*t+q2[j];}
   gammavalue = (top/den)*a;
   if (negative) gammavalue = rr-gammavalue;
   goto s9005;
/*                                 4.0 .LT. T .LE. 12.0                 */
/*                                 EVALUATE MINIMAX APPROXIMATION FOR   */
/*                                 LN(GAMMA(P)).                        */
s100:
   top = p3[6]*t+p3[7];
   den = t+q3[6];
   for (j=0;j<6;++j){
      top = top*t+p3[j];
      den = den*t+q3[j];}
   gammavalue = top/den;
   if (negative) gammavalue = rr-gammavalue;
   goto s9005;
/*                                 12.0 .LT. P .LT. 1.723E+319          */
/*                                 EVALUATE MINIMAX APPROXIMATION FOR   */
/*                                 LN(GAMMA(P)).                        */
s110:
   top = log(t);
   top = (t-pos_three_half)*(top-pos_one)+top-pos_three_half;
   if (t > INVERSE_MACHINE_PRECISION) goto s115;
   t = pos_one/t;
   b = t*t;
   top = (((p4[4]*b+p4[3])*b+p4[2])*b+p4[1])*t+p4[0]+top;
s115:
   gammavalue = top;
   if (incomplete) goto s120;
   if (negative) gammavalue = rr-gammavalue;
   goto s9005;
s120:
   gammavalue = exp(gammavalue);
   if (negative) gammavalue = rr/gammavalue;
   goto s9005;
s125:
   gammavalue = MAX_REAL_TYPE;
   if (fmod(p,pos_two) == zero) gammavalue = -gammavalue;
   /*------------------------------------------------------------------*/
s9005:
   *gamma_out = gammavalue;
   return(ierr);
}
int igamma(real_type x,real_type p,
           real_type *prob,real_type *gami,real_type *gamc)
/*
     C3.8

   FUNCTION            - INCOMPLETE GAMMA FUNCTION AND PROBABILITY
                           DISTRIBUTION FUNCTION
   USAGE             - CALL IGAMMA (X,P,PROB,GAMI,GAMC,IER)
   PARAMETERS   X      - VALUE TO WHICH GAMMA IS TO BE INTEGRATED
                P      - INPUT GAMMA PARAMETER
                PROB   - OUTPUT PROBABILITY = INTEGRAL OF GAMMA(P) TO X
               GAMI  - THE INCOMPLETE FUNCTION
               GAMC  - THE COMPLETE FUNCTION
                IER    - ERROR INDICATOR
                         IER=1 INDICATES  X IS LESS THAN ZERO
                         IER=2 INDICATES  P IS LESS THAN OR EQUAL TO
                             ZERO OR GREATER THAN 1000.
   PRECISION           - SINGLE
   REQD. ROUTINES      - CGAMMA,IGAMMA
   LANGUAGE            - FORTRAN
.......................................................................
   LATEST REVISION     - JUNE 13, 1973       ***  NASA - LRC
   INITIAL CONVERSION  - FEB 1, 1991
*/
{
   real_type v[8];
   real_type *v1;
   real_type cnt,ycnt,pnlg,ax,y,z,cut,big,ratio,reduc;
   int ierr;
   short j;
   /*------------------------------------------------------------------*/
   v1 = &v[2];
/*                                 TEST  X AND  P */
   *prob = zero;
   if (x  >= zero) goto s5;
   return(ierr=1);
s5:
   if ((p > zero) && (p <= pos_thousand)) goto s10;
   return(ierr=2);
s10:
   ierr = 0;
   if (x == zero) goto s9005;
/*                                 DEFINE LOG-GAMMA AND INITIALIZE */
   ierr=lngamma(p,&pnlg);
   if (ierr  !=  0) goto s9010;
   cnt = p * log(x);
   ycnt = x + pnlg;
   if ((cnt-ycnt)  > -LN_MAX_REAL_TYPE) goto s15;
   ax = zero;
   goto s20;
s15:
   ax = exp(cnt-ycnt);
s20:
   big =  SQRT_MAX_REAL_TYPE;
   cut =  1.0E-8;
/*                                 CHOOSE ALGORITHMIC METHOD */
   if ((x <= 1.0) || (x < p )) goto s40;
/*                                 CONTINUED FRACTION EXPANSION */
   y = pos_one - p;
   z = x  + y + pos_one;
   cnt = zero;
   v[0] = pos_one;
   v[1] = x;
   v[2] = x + pos_one;
   v[3] = z * x;
   *prob = v[2]/v[3];
s25:
   cnt = cnt + pos_one;
   y = y + pos_one;
   z = z + pos_two;
   ycnt = y * cnt;
   v[4] = v1[0] * z - v[0] * ycnt;
   v[5] = v1[1] * z - v[1] * ycnt;
   if (v[5] == zero) goto s50;
   ratio = v[4]/v[5];
   reduc = fabs((*prob)-ratio);
   if (reduc  >  cut) goto s30;
   if (reduc  <= (ratio*cut)) goto s35;
s30:
   *prob = ratio;
   goto s50;
s35:
   *prob = pos_one - (*prob) * ax;
   goto s9005;
/*                                 SERIES EXPANSION */
s40:
   ratio =  p;
   cnt = pos_one;
   *prob = pos_one;
s45:
   ratio = ratio + pos_one;
   cnt = cnt *  x/ratio;
   *prob += cnt;
   if (cnt  >  cut) goto s45;
   *prob = (*prob) * ax / p;
   goto s9005;
s50:
   for (j=0;j<4;++j) v[j] = v1[j];
   if (fabs(v[4]) < big) goto s25;
/*                                 SCALE TERMS DOWN TO PREVENT OVERFLOW */
   for (j=0;j<4;++j) v[j] = v[j]/big;
   goto s25;
s9005:
   ierr = cgamma(p,gamc);
   if (ierr == 1) ierr = 2;
   if (ierr != 0) goto s9010;
   if (x == zero) goto s9010;
   *gami = (*gamc) - (*prob)*(*gamc);
s9010:
   /*------------------------------------------------------------------*/
   return(ierr);
}
