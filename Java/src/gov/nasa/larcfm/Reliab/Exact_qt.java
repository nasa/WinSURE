package gov.nasa.larcfm.Reliab;

import java.util.ArrayList;

public class Exact_qt {



//
///* .............................. exact7.p ....................... */
//      
///* ********************************************************************|
//|                                                                      |
//|      this module is intended to provide an efficient computation     |
//|      of the q(t) function as defined in the sure documentation.      | 
//|      the mathematical basis is essentially the same as that used     | 
//|      in the acd subroutine conexp (document n-3, volume ii, sec-     |
//|      tion d2.1).  considerable attention has been given to taking    |
//|      advantage of the triangular and bidiagonal character of the     |
//|      matrix a of coefficients arising in the pure markov model.      |
//|                                                                      |
//|      author/implementer :  r. l. causey                              |
//|                                                                      |
//| *********************************************************************/
//
//const
     final static int maxn = 100;
     final static int twicemaxn = 2*maxn;
     
     static double [] c_pade  = new double[10]; //  array[1..9] of double;

//
//type
//
//   index = 1..maxn;
//   diagonal = array[ 1..maxn ] of double;
//   squarematrix = array[ index,index ] of double;
//
//var   
     static int tclock;
//   /*a : array[index, 1..2*maxn] of double;*//*ultrix-3/88-phs*/
//   a : array[index, 1..twicemaxn] of double;/*ultrix-3/88-phs*/
	 static double[][] a = new double[maxn][2*maxn];
//   m  : squarematrix;
	 static double[][] m = new double[maxn][maxn];
     static double[][] p = new double[maxn][10];       // : array[ index, 2..9 ] of double;
     static double[] lambda = new double[maxn];
     static double[] alpha = new double[maxn];

     static int   n;                
     static int idig,jdig;
     static boolean qtlisttoobig;
//
//
//  static void exchange (int  row1, int row2) {
//  var  c : integer;  temp : double;
//  
//	  for (int c = k; c <= n+m; c++) { 
//		  double temp = a[row1][c];
//		  a[row1][c] = a[row2][c];
//		  a[row2][c] = temp; 
//	  } 
//  }    /* exchange */

//    static void print_m() {
//    	f.pln(" ========================== print_m ========================= n = "+n);
//        for (int i = 1; i <= n; i++)  {  
//            for (int j = 1; j <= n; j++)  { 
//            	f.p("  "+f.Em8(m[i][j]));
//            }
//            f.pln("");    
//        }
//    }
//    
//    static void print_lambda() {
//    	f.pln(" ========================== print_lambda ========================= n = "+n);
//    	for (int i = 1; i <= n; i++)  {  
//    		f.p("  "+f.Em8(lambda[i]));
//    	}
//    	f.pln("");    
//    }
//
//    static void print_alpha() {
//    	f.pln(" ========================== print_alpha ========================= n = "+n);
//    	for (int i = 1; i <= n; i++)  {  
//    		f.p("  "+f.Em8(alpha[i]));
//    	}
//    	f.pln("");    
//    }
//
//    static void print_a() {
//    	f.pln(" ========================== print_a ========================= n = "+n);
//        for (int i = 1; i <= n; i++)  {  
//            for (int j = 1; j <= 2*n; j++)  { 
//            	f.p("  "+f.Em8(a[i][j]));
//            }
//            f.pln("");    
//        }
//    }

    
	
   static void  dinvn (int  n,/*no. rows*/ int m /*no. of columns of n*/ ) {

     /* this procedure assumes that m <= n and that the global array  a
       ( n by 2n ) contains  d  in the first n columns and  n  in 
       columns n+1, ... ,n+m .  it uses jordan elimination to reduce
       the first n columns of  a  to  i [ in actuality, the zeros &
       ones are not stored ] .  if (m = n and  n = i , the result is
       d reciprocal in the second n columns of  a .  otherwise, we
       leave  d reciprocal times  n  in the columns of  a  which were
       occupied by  n . */  
  
        /* dinvn */ 
        for  (int k = 1; k <= n; k++) {   /* k is the pivot index */
        	/* locate pivot row at index p. */ 
        	double max = Math.abs( a[k][k] ); 
        	int p = k;
        	for (int i = k+1; i <= n; i++) {  
        		if (Math.abs( a[i][k] ) > max ) {
        			max = Math.abs( a[i][k] );  
        			p = i;
        		}
        	}
        	if (p > k )  {
        		//exchange( k,p );
        		for (int c = k; c <= n+m; c++) { 
        			double temp = a[k][c];
        			a[k][c] = a[p][c];
        			a[p][c] = temp; 
        		} 
        	}

        	for (int j = k+1; j <=n+m; j++) {       /* perform row division. */
        		a[k][j] = a[k][j]/a[k][k];
        	}   

        	for(int r = 1; r <= n; r++) {          /* jordan elimination */
        		if (r != k ) {
        			if (a[r][k] != 0.0) {
        				for (int j = k+1; j <= n+m; j++) {
        					a[r][j] = a[r][j] - a[r][k]*a[k][j];
        				}
        			}
        		}
        	}
        }
   } /* dinvn */
//
   
   static boolean odd(int k) {
	   return (k % 2 != 0);
   }
//
   static void expofm() {  /* n.b. inputs are: n, alpha, lambda & m. */

    /* this routine computes the [9,9] pade  approximation p[9,9]
       of m .  m is replaced by the result.  the global array  a  is
       used for storage and as a parameter to the procedure  dinvn 
       which computes the matrix product: (d inverse) times n . 
       here  n  and  d  denote the numerator and denominator respec-
       tively of the pade rational fraction.  */

      int i, j, k;
      double dsum, nsum, t;
   
      /* expofm */

   /* calculate the pade  n and d  one column at a time. */

      for  (j = 1; j <= n-1; j++) {   /* first n-1 columns */ 
    	  /* calculate  jth  column of the powers of m. */
    	  /* first, get jth column of  m**2  in  p[.2]. */
    	  for (i = 1; j <= j-1; j++) { 
    		  p[i][2] = 0.0; /* above-diagonal zeros */
    	  }
    	  p[j][2] =  alpha[j]*alpha[j];           /* diagonal */
    	  for (i = j+1; i <= n-1; i++) {      /* compute sub-diagonal elements  */
    		  p[i][2] = lambda[i-1]*m[i-1][j] + alpha[i]*m[i][j];
    	  }
    	  p[n][2] = lambda[n-1]*m[n-1][j];

    	  
    	  /* now for the powers  3, ... ,9  of m */
    	  for (k = 3; k <= 9; k++) { 
    		  /* get jth column of  m**k . */
    		  /* supply zeros above-diagonal: */
    		  for (i = 1; i <= j-1; i++) { 
    			  p[i][k] = 0.0;
    		  }
    		  p[j][k] = alpha[j]*p[j][k-1];       /* diagonal */
    		  for (i = j+1; i <= n-1; i++) {       /* { sub-diagonal */
    			  p[i][k] = lambda[i-1]*p[i-1][k-1] + alpha[i]*p[i][k-1];
    		  }
    		  p[n][k] = lambda[n-1]*p[n-1][k-1];  /* sub-diagonals of powers */  
    	   	  //f.pln(" ^^^^ p[n][k] = "+p[n][k]);
    	   	 
    	  }
    	  /* now compute  jth  column of  n  and  d . */
    	  for ( i = 1; i <= j-1; i++) {            /* super-diagonal zeros */
    		  a[i][j] = 0.0;  
    		  a[i][n+j] = 0.0;
    	  }
    	  for  (i = j; i <= n; i++){ 
    		  dsum = 0.0; 
    		  nsum = 0.0;
    		  for (k = 9; k >= 2; k--) { //  downto 2 do 
    			  /* adding the non-linear terms */
    			  nsum = nsum + c_pade[k]*p[i][k];
    	          //f.pln(" ........ c_pade[k] = "+c_pade[k]);
    			  if (odd(k)) dsum = dsum - c_pade[k]*p[i][k]; 
    			  else dsum = dsum + c_pade[k]*p[i][k];
    	           //f.pln(" k = "+k+" dsum = "+dsum+" nsum = "+nsum+" p[i][k] = "+p[i][k]);

    		  };

    		  t = c_pade[1]*m[i][j];           /*now add in linear terms. */
    		  nsum = nsum + t;  
    		  dsum = dsum - t;
    		  if (j == i ) { /* add non-zero term from i */
    		    nsum = 1.0 + nsum;  
    		    dsum = 1.0 + dsum;
    		  }
              //f.pln(" i = "+i+" j = "+j+" t = "+t+" dsum = "+dsum+" nsum = "+nsum);
    		  a[i][j] = dsum;   
    		  a[i][n+j] = nsum;
    	  }// for
    	  
    	  //print_a();
    	  
      }                 /* of first  n-1  columns of  n  and  d */ 

      for (i = 1; i <= n-1; i++) {             /* column n  of  d  and  n  */
    	  a[i][n] = 0.0;   a[i][2*n] = 0.0;
      }
      a[n][n] = 1.0;   
      a[n][2*n] = 1.0;

      /*    for k = 1 to 2*n do
	 for i = 1 to n do System.out.println(" <<$$ i,k,a[i][k] = "+i,k,a[i][k]);  */

      /* this completes the formation of  n  and  d .  */ 
      /* to get the pade approximation  p[9,9], we invoke */

      dinvn( n,n );   /* this leaves p[9,9] in the right side of a . */

      /* copy result into m. */

      for (i = 1; i <= n; i++) {
    	  for (j = 1; j <= n; j++) { 
    		  m[i][j] = a[i][n+j];
    	  }
      }
   }   /* of  expofm */
//
   static void  squarem() {
	   //
	   //     /* this uses the first n columns of the global matrix  a  as
	   //        temporary storage.  advantage is taken of the fact that m
	   //        is lower triangular.                                      */

	   for (int j = 1; j <= n; j++) {
		   for (int i = j; i <= n; i++) {
			   double s = 0.0;
			   for (int  q = j; q <= i; q++) {	       
				   s = s + m[i][q]*m[q][j];
			   }	  
			   a[i][j] = s; 
		   }
	   }
	   for (int j = 1; j <= n; j++)  { /* now copy the result back to m. */
		   for (int i = j; i <= n; i++) {
			   m[i][j] = a[i][j];
		   }
	   }
   }  /* squarem */

    static double fastqt (ArrayList<HistoryTrans> qtlist, double t) {
//   label 999;
//   var   
//      i, j, s, w: integer;
//      q: hptr;
//      scalefactor: double;
//      anorm,ynorm,snorm,eanorm,ta,digc: double;
//
//     /* function  fastqt */ 

      n = 1;  
//      q = qtlist;  
      qtlisttoobig = false;
//      repeat /* acquire lambda & alphas from linked list. */
      for (int i = 1; i < qtlist.size(); i++) {                    // do not process first entry
    	 HistoryTrans qArrow = qtlist.get(i);
         if (n >= maxn-1) {
             if (Front.listlevel >= 6) f.pln(" >>> Q(T) LIST TOO LONG"); 
             n = maxn-1; 
             qtlisttoobig = true;
          	 f.pln(" $$$$$$$$$  Q(T) LIST TOO LONG");
             return 0;
         }
    	 //f.pln(" $$$ i = "+i+" qArrow = "+qArrow);
    	 if (qArrow.cl == Sure.Tranclass.class1) {
    		 lambda[n] = qArrow.lam_alpha_fract; // lam;
    		 alpha[n] = -qArrow.lam_alpha_fract - qArrow.gam_muh; 
    		 n = n + 1;
    	 };
      }//for

      
      for (int i = 1; i <= n-1; i++) { /* work henceforth with t*a but keep */
    	  /* same notation.                    */     
    	  lambda[i] = lambda[i]*t;
    	  alpha[i] = alpha[i]*t;
      }
      
  
      /* form the full matrix m = t*a in memory. */
      for (int i = 1; i <= n; i++) {
         for (int j = 1; j <= n; j++) {  
            m[i][j] = 0.0;
         }
      }
      
      
      double anorm = Math.abs(alpha[1]); 
      alpha[n] = 0.0;
      for (int i = 1; i <= n-1; i++)  {  
         m[i][i] = alpha[i];
         m[i+1][i] = lambda[i];
         double ta = Math.abs(lambda[i]) + Math.abs(alpha[i+1]);
         if (ta > anorm) anorm = ta;
/*         System.out.println(" m["+i:1+"+"+i:1+"] = "+m[i][i]);
         System.out.println(" m["+i+1:1+"+"+i:1+"] = "+m[i+1][i]); */
      }
 

      double digc = 24.00000000000e+00*n;
      if ((anorm > 1.0)) digc = digc*anorm;
      idig = 15 - (int) (Math.log(digc+0.5)/Math.log(10.0));
///*      System.out.println(" n,anorm,digc,idig = "+n,anorm,digc,idig); */
//
//
      /* now examine m for size of its components. */

      int s = 0;
      if ((anorm > 1.0)) {
         double scalefactor = 1.0;
         do {
            scalefactor = scalefactor * 2.0;
            s = s + 1;
         } while  (scalefactor <= anorm);
         scalefactor = 1.0 / scalefactor;
         for (int i = 1; i <= n-1; i++) {    
            alpha[i] = alpha[i]*scalefactor;
            lambda[i] = lambda[i]*scalefactor;
            m[i][i] = m[i][i]*scalefactor;
            m[i+1][i] = m[i+1][i]*scalefactor;
            }
         } 
 
 
      expofm();  /* approximate the exponential of m */     
      
//      print_m();

      
      double ynorm = 1.0;
      double eanorm = 0.0;
      for (int i = 1; i <= n; i++) {        /* exp. matrix is triangular */
    	  double ta = 0.0;
    	  for (int j = 1; j <= i; j++) {
    		  ta = ta + Math.abs(m[i][j]);
    	  }
    	  if (ta > eanorm) eanorm = ta;
      }
      double snorm = Math.abs(m[1][1]);     /* largest must be state 1 */
      // System.out.println(" eanorm,ynorm,snorm = "+eanorm:15,ynorm:5,snorm:15);  
      digc = n*eanorm*ynorm/snorm;
      jdig = idig - (int) (Math.log(digc)/Math.log(10.0) );

      if (s > 0)  for (int w = 1; w <= s; w++) squarem();

      //999:  fastqt = m[n][1];
      double rtn = m[n][1];
      //f.pln(" $$$$$$$$$$$ fast_qt s = "+s+" rtn = "+rtn);
      return rtn;
   }  /* fastqt */    

    
 static Pair<Double,Integer> exactqt(ArrayList<HistoryTrans> qtlist, double t, double uqt, int acc) {
//     double;
// var
//    ee: double;
//

    if (Front.listlevel >= 5) System.out.print(" UQT = "+uqt+" -- ");
    jdig = 16;
    double ee;
    if (uqt <= Front.minreal)
       ee = 0.0;
    else if (qtlist.size() == 0)
       ee = 1.0;
    else {
       tclock = (int) System.currentTimeMillis( );
       ee = fastqt(qtlist,t);
       /* System.out.println(" q(t) exec time = "+clock-tclock);  */
       }
    if (Front.listlevel >= 5 ) System.out.println(" EE = "+ee);
    if (jdig < 0 ) acc = 0;
    else acc = jdig;
    if (qtlisttoobig ) acc = -1;
	return new Pair<Double,Integer>(ee,acc);
 }
 
}
