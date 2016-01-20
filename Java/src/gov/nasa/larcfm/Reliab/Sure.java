/**
  
Notices:


Copyright 2001 United States Government as represented by the Administrator of the National 
Aeronautics and Space Administration. 
All Rights Reserved.    

Disclaimers


No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, 
EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT
 THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT 
SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO 
THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY 
GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE 
PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, 
GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF 
PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES 
GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  
IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES
OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, 
RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED 
STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE 
EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, 
UNILATERAL TERMINATION OF THIS AGREEMENT.

*/

package gov.nasa.larcfm.Reliab;

import java.io.Console;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.util.ArrayList;

import gov.nasa.larcfm.Reliab.Front.trantype;


/** Holds transition information from one state to another
 *
 */
class TranToState {  
		int st;
		trantype tt;
		NumRec rateOrMean; 
		NumRec stdev;
		NumRec fract;
		
		TranToState(int st, trantype tt, NumRec rate, NumRec stdev, NumRec fract) {
			this.st = st;
			this.tt = tt;
			this.rateOrMean = rate;
			this.stdev = stdev;
			this.fract = fract;
		}
		
		public String toString() {
			return "<"+st+" "+tt+" "+rateOrMean.toString()+" "+stdev+" "+fract+">";
		}
}

class DeathStateInfo {
	
    int st;
    double lprob;
    double uprob;
    double lplace;
    double uplace;

	
	public DeathStateInfo(int st, double lprob, double uprob, double lplace, double uplace) {
		this.st = st;
		this.lprob = lprob;
		this.uprob = uprob;
		this.lplace = lplace;
		this.uplace = uplace;
	}
	public String toString() {
		return "<|"+st+" "+lprob+" "+uprob+" "+lplace+" "+uplace+"|>";
	}
}
	

public class Sure {
	
	enum Tranclass {class1,class2,class3};
	static final int extend_extra = 100;	  
	final static int lee = 1;	  
	final static double probNearZero = 1E-100;
	static double dpu;
	static double lbplace;
	static double ubplace;
	static boolean anyprune;
	//	  /* ============================= COMPUTATION SECTION ========================= */
	static double nearzero = 1.0000000000e-10; /* ----- for sum of exiting probs = 1 --- */ 
	static double LargePlace = 1.0e+08;
	static ArrayList<ArrayList<TranToState>> succst = new ArrayList<ArrayList<TranToState>>(1000);  
	static ArrayList<TranToState> holding = new ArrayList<TranToState>(1000); 
	static ArrayList<Boolean>  predst = new ArrayList<Boolean>(1000);
	static ArrayList<DeathStateInfo> fdeath  = new ArrayList<DeathStateInfo>(100);                     /* death state list */
	static ArrayList<DeathStateInfo> splist  = new ArrayList<DeathStateInfo>(100);                     /* state prob list */
	static int bigst;                       /* largest state entered so far */
	static int pathcount;                   /* number of paths */
	static int cnttrunc;                    /* number of loops truncated*/
	static int cntprune;                    /* number of paths pruned */
	static double probprune;                /* total prob pruned */
	static int truncdig;                    /* digits accuracy after trunc */
	static int wcnttrunc;                   /* number of loops truncated*/
	static int wcntprune;                   /* number of paths pruned */
	static double wprobprune;               /* total prob pruned */
	static double wprune;                   /* worst prune level */
	static int wptrunc;                     /* worst ptrunc value */
	static int wdig;                        /* worst q(t) digits accuracy */
	static double asprunepl;
	static double asprunepu;
	static double wasprunepl;
	static double wasprunepu;
	static double ubdig;                    /* upper bound q(t) accuracy */
	static double lbdig;                    /* lower bound q(t) accuracy */
	static boolean qtbad;                   /* q(t) comp. is inaccurate */
	static boolean recslow;                 /* recovery too slow */
	static boolean stdbig ;                 /* rec. std too big */
	static boolean ratebig;                 /* exp. rate too fast */
	static boolean deltabig;                /* delta > time */
	static boolean qttoolong;               /* Q(T) list is too long for ExpMat */
	static boolean fastzero;                /* fast transition w. rate 0 */
	static int numdeath;
	static int nextautop;
	static double sumofdeath;
	static double tentomwarn;
	
	public static boolean gui_active = false;   // can be changed by vSu	
	public static boolean autorun = false;
	public static String editorPaneContent = "";
	

	static String progname()   {
		String rtn = "WinSURE V1.4";
		print(rtn);
		return rtn;
	} /* progname */

	
	static void print(String str) {
		System.out.print(str);
		if (gui_active) {
			editorPaneContent = editorPaneContent + str;
		}

	}


	static void println(String str) {
		System.out.println(str);
		if (gui_active) {
			editorPaneContent = editorPaneContent + str + "\n";
		}
	}

	static void computestartup() {
		succst.clear();
		predst.clear();
		holding.clear();
		fdeath.clear(); 
		bigst= -1;                /* largest state entered so far */
		pathcount= 0;             /* number of paths */
		cnttrunc= 0;              /* number of loops truncated*/
		cntprune= 0;              /* number of paths pruned */
		probprune= 0.0;          /* total prob pruned */
		truncdig= 100;            /* digits accuracy after trunc */
		wcnttrunc= 0;             /* number of loops truncated*/
		wcntprune= 0;             /* number of paths pruned */
		nextautop = 0;
		wprobprune= 0.0;         /* total prob pruned */
		wprune= 0.0;             /* worst prune level */
		qtbad= false;             /* q(t) comp. is inaccurate */
		recslow= false;           /* recovery too slow */
		qttoolong = false;        /* Q(T) list is too long for ExpMat */
		stdbig = false;           /* rec. std too big */
		ratebig= false;           /* exp. rate too fast */
		deltabig= false;          /* delta > time */
		fastzero= false;          /* fast transition w. rate 0 */
	};

	static TranToState findhold(int froms) {
	     ArrayList<TranToState> al = succst.get(froms);
	     if (al == null) return null;
	     for (TranToState tts: al) {
	    	 if (tts.st == 0) {
	    	     //f.pln(" $$ findhold: froms = "+froms+" tts = "+tts);
	    		 return tts;
	    	 }
	     }
	     return null;
	  } /* findhold */


	static void printAllTrans() {
		f.pln(" $$");
		f.pln(" $$ printAllTrans: ---------------------------------------------------");
		f.pln(" $$ succst.size() = "+succst.size());
		//while (succst.iterator().hasNext())  {
		for (int i = 0; i < succst.size(); i++)	{  
			ArrayList<TranToState> al = succst.get(i);
			if (al != null) {
				int from = succst.indexOf(al);
				f.p(" FROM "+from+": ");
				for (int j = 0; j < al.size(); j++)	{  
					TranToState state = al.get(j);
					f.p(" "+state.toString()+"     ");
				}
				f.pln("");
			}
		}
	}

	  
	  static void printtran(int i1, int i2)  {
		 println(" $$$$$$$ printtran NOT YET IMPLEMENTED !!!!!!!!!! ");
	  } /* printtran */


    static boolean parsok(int froms, int tos) {
       if ((froms < 0)) return false;    
       if ((tos < -2)) return false;  
       return true;
    } /* parsok */

	
    static void extend_predst(int newsz) {
    	if (newsz > predst.size())
    		for (int j = predst.size(); j < newsz+extend_extra; j++)
    			predst.add(null);
    }
    
    
    static void extend_succst(int newsz) {
    	//f.pln(" IN: extend_succst: newsz = "+newsz+" succst.size() = "+succst.size());
    	if (newsz > succst.size())
    		for (int j = succst.size(); j < newsz+extend_extra; j++)    			
    			succst.add(null);
    	//f.pln(" OUT: extend_succst: newsz = "+newsz+" succst.size() = "+succst.size());
    }

    static boolean slower(NumRec r1, NumRec r2) {
    	double v1,v2;
    	if (r1.coef == 0.0) v1 = r1.base;
    	else /* heuristic */
    		v1 = r1.coef*1.0000000000e-04 + r1.base; 
    	if (r2.coef == 0.0) v2 = r2.base;
    	else /* heuristic */
    		v2 = r2.coef*1.0000000000e-04 + r2.base;
    	return (v1 < v2); 
    } /* slower */

    // the transitions should be ordered from slow to fast.  Thus, will traverse slower transitions first    
    static void addInOrder(ArrayList<TranToState> al, TranToState e) {
    	boolean added = false;
    	for (int i = 0; i < al.size(); i++) {
    		if (slower(e.rateOrMean,al.get(i).rateOrMean)) {
    			al.add(i,e);
    			added = true;
    			break;
    		}
    	}
    	if (!added) al.add(e);
    	//f.pln(" $$$$$$$$$ addInOrder: al = "+al);
    }

     // The order of the list determines the order  of traversal.
     // Put slow exponentials at front to optimize AUTOPRUNE    
	  static void enter(int froms, int tos, trantype tt,  NumRec v1, NumRec v2, NumRec v3) {		  
		  //f.pln(" $$$$$ DEBUG: ................. enter transition from "+froms+" to "+tos+" tt = "+tt+" v1 = "+v1+" v2 = "+v2+" v3 = "+v3);
	      //f.pln(" $$$$$ specval = "+Front.specval);
		  if (! parsok(froms,tos)) { 
			  println(" *** ILLEGAL STATE NUMBER");    
			  Front.esyn1 = true;
		  } else {
			  if (tos == -1 && findhold(froms) != null) {
				  println(" *** STATE "+froms+ " HOLDING TIME ALREADY ENTERED.");
			  } else {
				  if (froms > bigst) bigst = froms;
				  if (tos > bigst) bigst = tos;
				  //f.pln(" $$$$$$$$$$$$ enter SET bigst = "+bigst);
				  extend_predst(tos+1);
				  predst.set(tos,true);
				  TranToState qd = new TranToState(tos,tt,v1,v2,v3);
				  extend_succst(froms+1);
				  ArrayList<TranToState> al = succst.get(froms);
				  if (al == null) al = new ArrayList<TranToState>(10);
				  //al.add(qd);
				  addInOrder(al,qd);
				  succst.set(froms,al);
			  }
		  }

	  } /* enter */

	  static void clear() {
		  bigst = -1; 
		  succst.clear();
		  holding.clear();
		  predst.clear();
	  } /* clear */


	  static void printinitlist(ArrayList<Initialrec> ilp) {
		  println("   INITIAL STATE PROBABILITIES  ");
		  println("  ------------------------------ ");
		  //while ilp <> nil do with ilp^ do
		  for (int i = 0; i < ilp.size(); i++)	{
			  Initialrec ilpArrow = ilp.get(i);
			  println("  "+ilpArrow.st+": ("+f.Em5(ilpArrow.pbl.cmp())+", "+f.Em5(ilpArrow.pbu.cmp())+")");
		  }
		  println("");
	  } /* printinitlist */


	  static void hdr1(){
		  if (Front.listlevel > 0)  { 
			  if (Front.rectype == lee) println(" ----- LEE STATISTICAL ANALYSIS MODE -----");
			  println(""); 
			  print(" ");
		  }
          //f.pln(".............. listlevel = "+Front.listlevel);
		  println(" Model = "+Front.lastfread+"   TIME = "+f.Fm2(Front.getSpecialVar("TIME",10.0)));
		  if (Front.listlevel == 1) { 
			  println("");
			  if (Front.specid.equals("")) print("              ");
			  else print("  "+f.padLeft(Front.specid,10)+"  ");
			  println("  LOWERBOUND    UPPERBOUND    COMMENTS                 RUN #"+Front.runno);
			  println(" -----------   -----------   -----------    ---------------------------------");
		  }
	  } /* hdr1 */

	   
	  static void hdr2()  {
		  println("");
		  if (!Front.specid.equals("")) { 
			  println("");
			  println("               "+Front.specid+" = "+f.Em5(Front.specval));
		  };
		  println("");
		  if (Front.initlist.size() > 0) printinitlist(Front.initlist);
		  println(" DEATHSTATE    LOWERBOUND    UPPERBOUND     COMMENTS                 RUN #"+Front.runno);
		  println(" ----------   -----------   -----------    ---------------------------------");
	  }
   
	  static void hdr3()  {
		  println("");
		  if (!Front.specid.equals(""))  {
			  println("  **********   "+Front.specid+" = "+Front.specval+"   **********   ");
		  }
		  if (Front.initlist.size() > 0) printinitlist(Front.initlist);

		  println("");
		  println("  LOWERBOUND    UPPERBOUND       PATH");
		  println(" -----------   -----------       ----");
	  }; /* hdr3 */

	   
	  // -1 = no start state found,  -rtn = more than one start state found
	  static int getstart() {
		  int rtn = -1;
		  for (int i = 0; i < predst.size(); i++) {
			  if (predst.get(i) == null) {
				  //f.pln(" .................. succst.size() = "+succst.size());
				  if (i >= succst.size()) extend_succst(predst.size()+100);
				  if (succst.get(i) != null) {
					  if (rtn >= 0) return -rtn;  // return - first one found
					  else {
						  //f.pln(" $$$$ ............ getstart: i = "+i);
						  Front.firstst = i;
						  rtn = i;
					  }
				  }
			  }
		  }
		  //f.pln(" $$$ getstart: predst.size() = "+predst.size()+" rtn = "+rtn);
		  return rtn;
	  }; /* getstart */

	  static void clear_trunc_prune_stats()  {
		  wcnttrunc = 0; 
		  wcntprune = 0; 
		  wprune = 0.0; 
		  wptrunc = 100000;
		  wprobprune= 0.0; 
		  wdig = 100; 
	  };


	  static int digits(double d, double place) {
		  /* digits(1.0000000000e-14,-18) = 4 */
		  if (d > 10*Front.minreal)      
			  return (int) ((Math.log(d)/Math.log(10.0)) - place + 0.5);   
		  else
			  return (int) (Front.tinyexp - place + 0.5);
	  }


	  static double place(double v, int dig) {
		  double ans;
		  /* place(1.5600000000e-08,dig) ~= -8 -dig */
		  if (v > Front.minreal)
			  ans = (Math.log(v)/Math.log(10.0)) - dig;
		  else
			  ans = -LargePlace;
		  return ans;
		  /*      println(" >>> place: v="+v+"+dig="+dig+" returns("+ans+")"); */
	  };

	  // returns lb,ub,lacc,uacc
	  static Quad<Double,Double,Integer,Integer> calcstprob(ArrayList<HistoryTrans> hp, double cumet, double delta,
			  /* var*/ double lb, double ub) {

		  double tt;
		  double tmd;
		  double lambda;
		  double eqt;
		  double eqtmd;
		  double qoft = cumet; 
		  double llsum = 0.0; 
		  int k = 0; 
		  double qtl = 1.0;
		  ArrayList<HistoryTrans> eh = hp; /* skip past current state (death st.), history is in reverse order */
		  for (int i = 1; i < eh.size(); i++) {   // do not process first entry -- the death state
			  HistoryTrans hArrow = eh.get(i);	
			  //f.pln(" ####  calcstprob: hArrow =  "+hArrow);
			  if (hArrow.cl == Tranclass.class1) { 
				  lambda = hArrow.lam_alpha_fract;
				  k = k + 1; 
				  llsum = llsum + lambda + hArrow.gam_muh;
				  tt = lambda*(Front.timet-delta)/k;
				  //f.pln(k+" ................................ calcstprob: tt = "+tt+" lambda = "+lambda);
				  if (tt <= 1.0)
					  qtl = qtl*tt;
				  else if (qtl <= Front.maxreal/tt)  /* will not overflow */
					  qtl = qtl*tt;
				  if (lambda*Front.timet > 1) ratebig = true;
			  };
			  //h = h^.next;
		  }
		  //
		  int qtcalc = (int) (Front.getdef("QTCALC",Front.defqtcalc));
		  tmd = Front.timet-delta;
		  //f.pln(" ####\n calcstprob: k = "+k+" tmd = "+tmd+" delta = "+delta+" qtl = "+qtl+" llsum = "+llsum);
		  double qoftdel;
		  double qtlterm;
		  if (tmd <= 0) {
			  qoftdel = 0.0; 
			  qtlterm = 0.0; 
		  } else if (llsum > (k+1)/tmd) {
			  qoftdel = 0.0; 
			  qtlterm = 1.0; 
		  } else { 
			  qtlterm = tmd*llsum/(k+1);
			  qoftdel = qtl*(1.0 - qtlterm);
		  }
		  //f.pln(" #### calcstprob: qoftdel = "+qoftdel+" qtlterm = "+qtlterm);
		  if (qoftdel < 0.0) qoftdel = 0.0; 
		  if (qoft > 1.0) qoft = 1.0;

		  int uacc = 10000; 
		  int lacc = 10000;  /* indicates algebraic q(T) */
		  if (qtcalc == 0) {
			  if (qtlterm > 0.1) qtbad = true;
		  } else { 
			  if ((qtcalc == 2) && (qtlterm < 0.1)) {/* use algebraic */
				  //f.pln(" ###### calcstprob: USE ALGEBRAIC: qtlterm = "+qtlterm);
			  } else {
				  Pair<Double,Integer> peqt = Exact_qt.exactqt(eh,Front.timet,qoft,uacc);
				  eqt = peqt.first;
				  uacc = peqt.second;
				  if (uacc < 0){ 
					  eqt = qoft; 
					  uacc = 10000;
					  qttoolong = true;
				  }
				  peqt = Exact_qt.exactqt(eh,Front.timet-delta,qoft,lacc);
				  eqtmd = peqt.first;
				  lacc = peqt.second;
				  if (lacc < 0) {
					  eqtmd = qoftdel; lacc = 10000;
					  qttoolong = true;
				  }
				  //f.pln(" eqt = "+eqt+" eqtmd = "+eqtmd);
				  if (Front.listlevel >= 6) {
					  println(""); 
					  print(" EXACT Q(T) = "+eqt+"; "+uacc+ " DIGITS ACCURACY");
					  println(" EXACT Q(T-^) = "+eqtmd+"; "+lacc+ " DIGITS ACCURACY");
				  };
				  qoft = eqt; qoftdel = eqtmd; 
			  }
		  }
		  ub = ub*qoft; 
		  lb = lb*qoftdel;
		  if (ub > 1.0) ub = 1.0;
		  if (lb < 0.0) lb = 0.0;	 
		  //f.pln(" ###### calcstprob: qoft = "+qoft+" qoftdel = "+qoftdel+" lb =  "+lb+" ub = "+ub);
		  return new Quad<Double,Double,Integer,Integer>(lb,ub,lacc,uacc);
	  } /* calcstprob */

	  static void addtree(ArrayList<DeathStateInfo> root, int cs, double lprob, double uprob, int  lacc, int uacc) {
		  if (Front.listlevel >= 6) {
			  println(" add "+lprob+" "+uprob+" to "+cs);
		  }
		  //f.pln(" $$$ add "+lprob+" "+uprob+" to "+cs);
		  boolean found = false;
		  for (int j = 0; j < root.size(); j++) {
			  DeathStateInfo lpArrow = root.get(j);
			  if (lpArrow.st == cs) {   // this death state has already been visited => update
				  lpArrow.lprob = lpArrow.lprob + lprob;
				  lpArrow.uprob = lpArrow.uprob + uprob;
				  lpArrow.lplace = Math.max(lpArrow.lplace,place(lprob,lacc));
				  lpArrow.uplace = Math.max(lpArrow.uplace,place(uprob,uacc));
				  //             f.pln(" >>> lp^.lplace,lp^.uplace "+lp^.lplace,lp^.uplace);	
				  found = true;
				  break;
			  }
		  }
		  if (! found) {
			  DeathStateInfo lpArrow = new DeathStateInfo(cs, lprob, uprob,place(lprob,lacc),place(lprob,lacc) );
			  root.add(lpArrow);
		  }
	  } /* addtree */

	  static void addtodeath(int ds, ArrayList<HistoryTrans> hp, double lb, double ub, int lacc, int uacc, int autoprune) {
		  if (Front.listlevel >= 4) { 
			  print(f.Em5(lb)+"  "+f.Em5(ub));
			  if (qtbad) print(" .. Q(T) INACCURATE");
			  qtbad = false; 
			  if (uacc < 10000) print("       <Expmat>");
		  }; 
		  //f.pln(" $$$$$$$$$$$$$ addtodeath: ds = "+ds+" lb = "+lb+" ub = "+ub);
		  addtree(fdeath,ds,lb,ub,lacc,uacc);
		  if (autoprune == 1) { 
			  numdeath = numdeath + 1; 
			  sumofdeath = sumofdeath + ub;
			  if ((numdeath + cntprune) >= nextautop) {
				  nextautop = nextautop*2;    /* V8.0 */
				  double stmp = sumofdeath*(tentomwarn)-probprune;
				  //f.pln(" ************* addtodeath: "+sumofdeath+" "+(tentomwarn)+" probprune = "+probprune+" xxx = "+xxx);
				  if (stmp > 0.0) 	{
					  if (bigst > 100)
						  Front.prune_d = stmp/bigst;
					  else
						  Front.prune_d = stmp/5000;
				  } else {
					  Front.prune_d = probNearZero;
				  }
				  if (Front.listlevel > 4) println(" prune set to "+Front.prune_d);
				  //f.pln(" $$$$ addtodeath: prune changed to "+ Front.prune_d+" probprune = "+probprune); 
			  }
		  }
		  if (Front.listlevel >= 4) {
			  print(" --->  "); 
			  int lc = 1;
			  for (int k = 0; k < hp.size(); k++) {
				  HistoryTrans hpArrow = hp.get(k);
				  print(hpArrow.st+" "); 
				  if (lc % 10 == 0) { 
					  println(""); print(" "); 
				  };
				  lc = lc + 1;
			  };
			  println("");
		  }
	  } /* addtodeath */

	  // return rtn = true iff all transitions slow from state cs,  tsum = sum of the rates from state cs
	  // return tsum = -1 if error condition
	  static Pair<Boolean,Double> allslow(int cs) { // , var tsum: double) {
		  ArrayList<TranToState> lp = succst.get(cs);
		  boolean rtn = true; 
		  double tsum = 0.0;
		  //	        while lp<> nil do 
		  for (int i = 0; i < lp.size(); i++) {
			  TranToState lpArrow = lp.get(i);
			  if (lpArrow.st >= 0) {
				  if (Front.rectype != lee) { // case lp^.tt of
					  if (lpArrow.tt == trantype.expo || lpArrow.tt == trantype.fastexpo) {
						  if (lpArrow.rateOrMean.cmp() < 0) { 
							  println(" *** ERROR: TRANSITION WITH NEGATIVE RATE FROM STATE "+cs); 
							  //goto 1000;
							  return new Pair<Boolean,Double>(false,-1.0);
						  }
					  } else  if (lpArrow.tt == trantype.recov) {
						  if (lpArrow.rateOrMean.cmp() < 0) { 
							  println(" *** ERROR: TRANSITION WITH NEGATIVE MEAN FROM STATE "+cs); 
							  return new Pair<Boolean,Double>(false,-1.0);
							  //goto 1000;
						  }
						  if (lpArrow.stdev.cmp() < 0) {
							  println(" *** ERROR: TRANSITION WITH NEGATIVE STANDARD DEVIATION FROM STATE "+cs);
							  //goto 1000;
							  return new Pair<Boolean,Double>(false,-1.0);
						  }
						  if (lpArrow.fract.cmp() < 0) {
							  println(" *** ERROR: TRANSITION WITH NEGATIVE TRANSITION PROBABILITY FROM STATE "+cs); 
							  //goto 1000;
							  return new Pair<Boolean,Double>(false,-1.0);
						  }
					  }
				  }; /* case */
				  if (lpArrow.tt == trantype.expo) {
					  tsum = tsum+lpArrow.rateOrMean.cmp();     /* valid iff rtn */
				  } else rtn = false;
			  } // (lp^.st >= 0)
			  //	lp = lpArrow.next;
		  }// for
		  return new Pair<Boolean,Double>(rtn,tsum);
	  } /* allslow */

	  // returns the number of visits to state cs in path h
      static int returncnt(int cs, ArrayList<HistoryTrans> h) {
      //var c: integer;
         int c = 0;
         for (int i = 0; i < h.size(); i++) {
        	 HistoryTrans hArrow = h.get(i);
        	 if (hArrow.st == cs) c++;
         }
         return c;
      }; /* returncnt */

	  
      static boolean truncorprune(int cs, double pmax, ArrayList<HistoryTrans> hp, int ptrunc) {
    	  boolean stop = false;
      	  int retc = returncnt(cs,hp);
    	  //f.pln(" $$$$$ truncorprune: retc = "+retc+" Front.ptrunc = "+Front.ptrunc);
    	  if (retc >= ptrunc) {/* test for truncation  */ 
    		  cntprune = cntprune + 1; 
    		  cnttrunc = cnttrunc + 1;
    		  stop = true;
    		  probprune = probprune + pmax;
//    		  if (Front.stopflag) {
//    			  println(" LOOP UNFOLDED "+ptrunc+" TIMES");
//    			  println(" SINCE NEGATIVE TRUNC SPECIFIED, CALCULATION TERMINATED");
//    			  Thread.dumpStack();
//    			  System.exit(0);
//    			  //goto 1000;
//    		  }
    		  if (Front.listlevel >= 6)
    			  println(" LOOP AT STATE "+cs+" TRUNCATED -- "+pmax); 
    	  } else {                    /* ---- test for pruning ---- */ 
    		  if (pmax <= Front.prune_d) { 
    			  //f.pln(" $$$$$ truncorprune: pmax = "+pmax+"  Front.prune = "+Front.prune);
    			  cntprune = cntprune + 1; 
    			  stop = true; 
    			  probprune = probprune + pmax;
    			  if (Front.listlevel >= 5)
    				  println(" PATH PRUNED AT "+cs+" "+pmax);  
				  //f.pln("\n $$$ PATH PRUNED AT "+cs+" "+pmax);  
    		  }
    	  }
    	  //f.pln(" $$$$$ truncorprune: pmax = "+pmax+"  probprune = "+probprune+" stop = "+stop);
    	  return stop;
      } /* truncorprune */
//
	  
      // used in conjunction with autofast
      static void autoprocess(int cs, double timet) {
    	  ArrayList<TranToState> p = succst.get(cs);
    	  if (p == null) return;
    	  for (int i = 0; i < p.size(); i++) {                      	  
    		  TranToState tts = p.get(i);
    		  //f.pln(" $$ autoprocess: "+cs+" tts = "+tts);
    		  double cmprate = tts.rateOrMean.cmp();
    		  if (cmprate*timet > 100.0) {
    			  if (tts.tt == trantype.expo) tts.tt = trantype.fastexpo;                  
    			  //f.pln(" "+cs+" -> "+tts.st+" CHANGED TO FAST "+cmprate);
    		  } else {
    			  if (tts.tt == trantype.fastexpo) tts.tt = trantype.expo;
    			  //f.pln(" "+cs+" -> "+tts.st+" CHANGED TO SLOW "+cmprate);
    		  }
    	  }
      } /* autoprocess */

      static Triple<Double,Double,Double> fastonpath(int cs, double lb, double ub, double hmean, double hmom2, double ffract,
    		                  double fmom2, double sumfaste, double delta, double sumslows, double deltaatcs, 
    		                 TranToState pArrow, HistoryTrans hArrow) {
          double a;
          double amean;
          double amom2;
          double afract;
          double astd;
          double f1;
          double ri;
          //f.pln(" $$$$ fastonpath: BEGIN delta = "+delta+" deltaatcs = "+deltaatcs);
          if (pArrow.tt == trantype.recov) { 
        	  afract = pArrow.fract.cmp();
        	  amean = pArrow.rateOrMean.cmp(); 
        	  astd = pArrow.stdev.cmp();
        	  amom2 = amean*amean + astd*astd;
          } else { /* fast exponential */
        	  a = pArrow.rateOrMean.cmp(); 
        	  afract = a*hmean;
        	  if (afract == 0.0) {
        		  amean = 1.0000000000e-04;
        		  astd = 0.0000000000e+00;
        		  amom2 = 1.0000000000e-08;
        		  fastzero = true;
        	  } else { 
        		  amean = a*hmom2/(2.0*afract);
        		  amom2 = (2.0*amean-a*ffract*fmom2/afract)/sumfaste;
        		  double sqrtarg = amom2 - amean*amean;
        		  if (sqrtarg < 0) {
        			  println(" *** ERROR: calculation of fast exponential led to sqrt argument < 0");
        			  sqrtarg = 0;
        		  }
        		  astd = Math.sqrt(sqrtarg);
        	  }
          }
         if (Front.listlevel >= 6)
            println(" "+cs+"+"+pArrow.st+" = < "+amean+"+ "+astd+"+ "+afract+" >  "); 

         ub = ub*afract;         
         ri = Math.pow(2.0*amom2*Front.timet,1.0/3.0);
         
         if (ri <= Front.minreal) { 
        	 f1 = 0.0;       
            println(" *** ERROR: INSTANTANEOUS TRANSITION AT STATE "+cs); 
            //goto 1000;
            return new Triple<Double,Double,Double>(-1.0,-1.0,-1.0);
            }
         else
            f1 = ( 1.0 - sumslows*amean - amom2/(ri*ri) );
         if (f1 <= 0.5) { 
        	if (f1 <= 0) f1 = 0.0;
            if (Front.listlevel >= 6) {
              println(" *** F1 NEGATIVE "+cs+" TO "+pArrow.st);
              println(" SUMSLOWS = "+sumslows);
              println(" RI = "+ri);
              println(" F1 = "+f1);
            }
            if (sumslows*Front.timet > 0.2) ratebig = true;
            if (amean > 0.2*Front.timet) recslow = true;
            else if (astd > 0.2*Front.timet) stdbig = true;
            };
         lb = lb*afract*f1;
         delta = deltaatcs + ri;
         
         hArrow.cl = Tranclass.class2;
         hArrow.lam_alpha_fract = afract;
         //f.pln(" $$$$ fastonpath: END delta = "+delta);
     	 return new Triple<Double,Double,Double>(lb,ub,delta);
      } /* fastonpath */
//
      
      
      static String strHistoryList(ArrayList<HistoryTrans> hp) {
    	  String rtn ="";
    	  for (int i = 0; i < hp.size(); i++) {
    		 rtn = rtn +" "+hp.get(i);
    	  }
   	      return rtn;
      }
      
       
	 static Pair<Double,Double> traverse(int cs, ArrayList<HistoryTrans> hp, int numet,
	                        double cumet, double lb, double ub, double delta, int ptrunc, int autofast, int autoprune) {
		    boolean goto999 = false;		    
		    //f.pln(" ..... traverse: cs = "+cs+" lb= "+lb+" ub="+ub+" delta="+delta+" cumet = "+cumet); 
	        if (Front.listlevel >= 6) {
	        	println(" ..... traverse: cs,lb,ub,delta = "+cs+" "+lb+" "+ub+" "+delta+" cumet = "+cumet);   
	        }
	        double par_prevlprob = 0.0;   
	        double par_prevuprob = 0.0;
	        //f.pln("$$$$$ ENTER traverse: cs =  "+cs+" bigst = "+bigst+" succst.size() = "+succst.size());
	        ArrayList<TranToState> p = succst.get(cs);
	        //f.pln("\n ++++++++++++++++++ ENTER traverse: cs =  "+cs+" succst.get(cs) =  p = "+p);
	        if (autofast > 0) autoprocess(cs,Front.timet);
        	HistoryTrans nhp = new HistoryTrans(cs,Tranclass.class1,0.0,0.0);
        	hp.add(0,nhp);                         // add to frontprin
	        if (p == null) {  /* at a death state */
	        	//f.pln(" :: "+strHistoryList(hp));
	        	Quad<Double,Double,Integer,Integer> qcstp = calcstprob(hp,cumet,delta,lb,ub); //,delta,lb,ub,ixx,iyy);
	        	lb = qcstp.first;
	        	ub = qcstp.second;
	        	int ixx = qcstp.third;
	        	int iyy = qcstp.fourth;
			    //f.pln(" ..... traverse: cs = "+cs+" lb= "+lb+" ub="+ub+" delta="+delta+" cumet = "+cumet); 
	        	addtodeath(cs,hp,lb,ub,ixx,iyy,autoprune);
	        	pathcount = pathcount + 1;
	        } else if (truncorprune(cs,cumet*ub,hp, ptrunc)) {
		        //dispose(hp); 
	        	goto999 = true;
	        } else {
	        	Pair<Boolean,Double> pas = allslow(cs);
	        	boolean bas = pas.first;
	        	double tsum = pas.second;
	        	if (bas) { /* -- all transitions from cs slow -- */
	        		double oldcum = cumet; 
	        		int oldnumet = numet;
	        		//while p <> nil do        /* oldcum = value prior to this tran. */
	        		for (int j = 0; j < p.size(); j++) {
	        			TranToState pArrow = p.get(j);
	        			//f.pln(" traverse cs = "+cs+" j = "+j+"  pArrow = "+pArrow);
	        			double rate = pArrow.rateOrMean.cmp(); 
	        			double lt = rate*Front.timet;
	        			//f.pln(" traverse cs = "+cs+" rate = "+rate);
	        			if (lt < 1.0)  {            /* trick: treat fast exp.   */
	        				numet = oldnumet + 1; /* as instantaneous */ 
	        				cumet = lt*oldcum/numet;
	        			}
	        			else cumet = oldcum;
	        			if (cumet > 1.0) cumet = 1.0;
	        			nhp.cl = Tranclass.class1;
	        			nhp.lam_alpha_fract = rate;
	        			nhp.gam_muh = tsum - rate;
	        	        //f.pln(" ...............CALL-slow traverse cs: hp = "+strHistoryList(hp));
	        	        Pair<Double,Double> ptrav = traverse(pArrow.st,hp,numet,cumet,lb,ub,delta,ptrunc,autofast,autoprune);
	        	        //f.pln(" .................AFTER-slow traverse cs: hp = "+strHistoryList(hp));
	        			par_prevlprob = par_prevlprob + ptrav.first;
	        			par_prevuprob = par_prevuprob + ptrav.second;
	        		}
	        		//f.pln(" @@@@ AFTER class 1: cs = "+cs+" par_prevlprob = "+par_prevlprob+" par_prevuprob = "+par_prevuprob);
	        		cumet = oldcum; 
	        	} else {    
	        		//holding(); /* ----- need state holding time information ----- */       		
	        		double hmean = 0; 
	        		double hmom2 = 0; 
	        		double sumfract = 0.0; 
	        		double sumslows = 0.0; 
	        		double sumfaste = 0.0;
	        		ArrayList<TranToState> lp = succst.get(cs);        	
	        		for (int i = 0; i < lp.size(); i++) {
	        			TranToState lpArrow = lp.get(i);
	        			//f.p(" traverse(holding): i = "+i+" lpArrow = "+lpArrow);
	        			if (lpArrow.tt == trantype.recov) { /* fast recovery transition */
	        				double frac = lpArrow.fract.cmp();
	        				double mn = lpArrow.rateOrMean.cmp(); 
	        				double std = lpArrow.stdev.cmp();
	        				hmean = hmean + frac*mn; 
	        				sumfract = sumfract+frac; 
	        				hmom2 = hmom2 + frac*(std*std+mn*mn);
	        			} else if (lpArrow.tt == trantype.fastexpo) {
	        				sumfaste = sumfaste + lpArrow.rateOrMean.cmp();
	        			}  else {                /* slow transition */
	        				sumslows = sumslows + lpArrow.rateOrMean.cmp();
	        			}
	        		}// for
	        	    /* calcholding */

	        		double ffract = 0.0;
        	        double fmom2 = -1.0;
	        		if (sumfaste > 0) {   /* --- have fast exponentials --- */
	        			ffract = sumfract;
		        		double fmean;
	        			if (ffract > 0) {
	        				fmean = hmean/ffract;
	        			    fmom2 = hmom2/ffract;
	        			} else {
	        			   fmean = 0.0; 
	        			   fmom2 = 0.0;
	        			}
	        			hmean = (1-ffract)/sumfaste; 
	        			hmom2 = 2.0*(1.0-ffract*(sumfaste*fmean+1.0))/(sumfaste*sumfaste); 
	        			if ((hmom2 < 0) || (ffract > 1.0)) {
	        				Front.erun = true; 
	        				hmean = 0.0; hmom2 = 0.0;
	        				println(" *** ERROR: INCONSISTENT SPECIFICATION OF FAST TRANSITIONS AT STATE "+cs); 
	        				//goto 1000;
	        				return new Pair<Double,Double>(-1.0,-1.0);
	        			}
	        			if (Math.abs(1.0 - ffract) < nearzero) {
	        				println(" *** ERROR: THE FAST EXPONENTIALS HAVE ZERO PROBABILITY OF OCCURRENCE AT STATE "+cs);
	        				//goto 1000;
	        				return new Pair<Double,Double>(-1.0,-1.0);
	        			}
	        		} /* if (sumfaste > 0 */
	        		else if (Math.abs(1.0-sumfract) > nearzero) {
	        			Front.erun = true; 
	        			println(" *** ERROR: SUM OF EXITING PROBABILITIES IS NOT 1 AT "+cs+" ("+sumfract+")");
	        			//goto 1000;
	        			return new Pair<Double,Double>(-1.0,-1.0);
	        		}

	        		double hsigsq = hmom2 - hmean*hmean;
	        		if (Front.listlevel >= 6) {
	        			println(" @"+cs+" = < "+hmean+"+ "+Math.sqrt(hsigsq)+" >   SUMSLOWS = "+sumslows);
	        		}
	        		double oldlb = lb; 
	        		double oldub = ub; 
	        		double deltaatcs = delta;
	        		for (int j = 0; j < p.size(); j++) {
	        			TranToState pArrow = p.get(j);
	        			lb = oldlb; 
	        			ub = oldub;
	        			if (pArrow.tt == trantype.recov || pArrow.tt == trantype.fastexpo)    { //fastonpath(pArrow, hp);	        				
	        				Triple<Double,Double,Double> tfast = fastonpath(cs, lb, ub, hmean, hmom2, ffract,
	        						fmom2, sumfaste, delta, sumslows, deltaatcs, 
	        						pArrow, nhp);
	        				lb = tfast.first;
	        				ub = tfast.second;
	        				delta = tfast.third;
	        			} else  {                                // slowonpath             
	        				double alpha = pArrow.rateOrMean.cmp();
	        				if (Front.listlevel >= 6)
	        					println(" "+cs+"+"+pArrow.st+" = "+alpha);
	        				ub = ub*alpha*hmean;
	        				double f2;
	        				double sj;
	        				if (hmean <= Front.minreal) {
	        					f2 = 0.0;
	        					println(" *** ERROR: INSTANTANEOUS RECOVERY AT STATE "+cs); 
	        					//goto 1000;
	        					return new Pair<Double,Double>(-1.0,-1.0);
	        				} else { 
	        					sj = Math.sqrt(Front.timet*hmom2/hmean);
	        					f2 = hmean - hmom2*( sumslows/2.0 + 1.0/sj );
	        				}
	        				if (f2 <= 0.5*hmean) {
	        					if (f2 < 0) f2 = 0.0;
	        					if (sumslows*Front.timet > 0.2) ratebig = true;
	        					if (hmean > 0.2*Front.timet) recslow = true;
	        					else if (hsigsq > 0.2*Front.timet*hmean) stdbig = true;
	        					if (Front.listlevel >= 6) {
	        						println(" *** F2 NEGATIVE "+cs+" TO "+pArrow.st);
	        						println(" SUMSLOWS = "+sumslows);
	        						println(" HMEAN = "+hmean);
	        						println(" SJ = "+sj);
	        						println(" F2 = "+f2);
	        						println(" H_SIG_SQ = "+hsigsq);
	        					}
	        				} /* if (f2 < 0 */
	        				lb = lb*alpha*f2;
	        				delta = deltaatcs + sj;
	        				nhp.cl = Tranclass.class3; 
	        				nhp.lam_alpha_fract = alpha; 
	        				nhp.gam_muh = hmean;
	        			}
	        			if ((delta >= Front.timet)) deltabig = true;  
	        	        //f.pln(" ...............CALL-fast traverse cs: hp = "+strHistoryList(hp));
	        			Pair<Double,Double> ptrav = traverse(pArrow.st,hp,numet,cumet,lb,ub,delta,ptrunc,autofast,autoprune);
	        			//f.pln(" ................. AFTER-fast traverse cs: hp = "+strHistoryList(hp));
	        			par_prevlprob = par_prevlprob + ptrav.first;
	        			par_prevuprob = par_prevuprob + ptrav.second;
	        			// p = p^.next;
	        		} // for
	        		lb = oldlb; 
	        		ub = oldub; 
	        		delta = deltaatcs;
	        	} /* if /*/
	        	if (Front.listlevel >= 3) { 
			       //f.pln(" @@@@@@@@@ BEFORE  traverse: cs = "+cs+" lb = "+lb+" ub = "+ub+ " par_prevlprob = "+par_prevlprob+" par_prevuprob = "+par_prevuprob);
	 	           Quad<Double,Double,Integer,Integer> qcstp = calcstprob(hp,cumet,delta,lb,ub); //,delta,lb,ub,ixx,iyy);
		           lb = qcstp.first;
		           ub = qcstp.second;
		           int ixx = qcstp.third;
		           int iyy = qcstp.fourth;
		           //f.pln(" @@@@@@ traverse: listlevel >= 3 lb = "+lb+" ub = "+ub+" par_prevlprob = "+par_prevlprob);
		           double ppu = ub - par_prevlprob;
		           double ppl = lb - par_prevuprob;
		           //f.pln(" @@@@@@ traverse: listlevel >= 3 ppl = "+ppl+" ppu = "+ppu);
		           if (ppl < 0.0) ppl = 0.0;
		           addtree(splist,cs,ppl,ppu,ixx,iyy);
		           if ((Front.listlevel >= 6)) { 
		        	   f.pln("");
		        	   f.pln(" ---------------------------------------------- ");
		        	   println(" in prob at "+cs+" is "+lb+"   "+ub);
		        	   println(" - stuff = "+par_prevlprob+" "+par_prevuprob);
		        	   println(" ppl,ppu = "+ppl+" "+ppu );
		           }
	        	}
	        } /* if /*/
	        if (!goto999) {
	           par_prevlprob = lb; 
	           par_prevuprob = ub;
	        }
	        hp.remove(0);
	      //f.pln(" ................. AT END ------------- traverse cs: p = "+p+" hp = "+strHistoryList(hp));
	      //f.pln(" ................. AT END ------------- traverse cs = "+cs+" lb = "+lb+" ub = "+ub);
	           return new Pair<Double,Double>(par_prevlprob,par_prevuprob);
	     } /* traverse */

	 static Pair<Double,Double> travlee(int cs, ArrayList<HistoryTrans> hp, int numet, double cumet, double lb, double ub, double delta, 
			                            int ptrunc, int autofast, int autoprune) {
		 double ht1 = 0;
		 double ht2 = 0;
		 double ht3 = 0;
		 double alpha;
		 boolean goto999 = false;		    
		 //f.pln(" ..... ENTER travlee: cs = "+cs+" lb= "+lb+" ub="+ub+" delta="+delta+" cumet = "+cumet); 
		 //f.pln(" ..... ENTER travlee: hp = "+strHistoryList(hp));
		 if (Front.listlevel >= 6) {
			 println(" ..... travlee: cs,lb,ub,delta = "+cs+" "+lb+" "+ub+" "+delta+" cumet = "+cumet);   
		 }
		 double par_prevlprob = 0.0;   
		 double par_prevuprob = 0.0;
		 //f.pln("$$$$$ ENTER travlee: cs =  "+cs+" bigst = "+bigst+" succst.size() = "+succst.size());
		 ArrayList<TranToState> p = succst.get(cs);
		 //f.pln("\n ++++++++++++++++++ ENTER travlee: cs =  "+cs+" succst.get(cs) =  p = "+p);
		 if (autofast > 0) autoprocess(cs,Front.timet);
		 HistoryTrans nhp = new HistoryTrans(cs,Tranclass.class1,0.0,0.0);
		 hp.add(0,nhp);                         // add to frontprin
		 if (p == null) {  /* at a death state */
			 //f.pln(" :: "+strHistoryList(hp));
			 Quad<Double,Double,Integer,Integer> qcstp = calcstprob(hp,cumet,delta,lb,ub); //,delta,lb,ub,ixx,iyy);
			 lb = qcstp.first;
			 ub = qcstp.second;
			 int ixx = qcstp.third;
			 int iyy = qcstp.fourth;
			 //f.pln(" ..... travlee: cs = "+cs+" lb= "+lb+" ub="+ub+" delta="+delta+" cumet = "+cumet); 
			 addtodeath(cs,hp,lb,ub,ixx,iyy,autoprune);
			 pathcount = pathcount + 1;
		 } else if (truncorprune(cs,cumet*ub,hp, ptrunc)) {
			 goto999 = true;
		 } else {
			 Pair<Boolean,Double> pas = allslow(cs);
			 boolean bas = pas.first;
			 double tsum = pas.second;
			 if (bas) { /* -- all transitions from cs slow -- */
				 double oldcum = cumet; 
				 int oldnumet = numet;
				 //while p <> nil do        /* oldcum = value prior to this tran. */
				 for (int j = 0; j < p.size(); j++) {
					 TranToState pArrow = p.get(j);
					 //f.pln(" travlee cs = "+cs+" j = "+j+"  pArrow = "+pArrow);
					 double rate = pArrow.rateOrMean.cmp(); 
					 double lt = rate*Front.timet;
					 //f.pln(" travlee cs = "+cs+" rate = "+rate);
					 if (lt < 1.0)  {            /* trick: treat fast exp.   */
						 numet = oldnumet + 1; /* as instantaneous */ 
						 cumet = lt*oldcum/numet;
					 }
					 else cumet = oldcum;
					 if (cumet > 1.0) cumet = 1.0;
					 nhp.cl = Tranclass.class1;
					 nhp.lam_alpha_fract = rate;
					 nhp.gam_muh = tsum - rate;
					 //f.pln(" ...............CALL-slow travlee cs = "+cs+" hp = "+strHistoryList(hp));
					 Pair<Double,Double> ptrav = travlee(pArrow.st,hp,numet,cumet,lb,ub,delta, ptrunc, autofast, autoprune);
					 //f.pln(" .................AFTER-slow travlee cs: hp = "+strHistoryList(hp));
					 par_prevlprob = par_prevlprob + ptrav.first;
					 par_prevuprob = par_prevuprob + ptrav.second;
					 // p = p^.next;
				 }
				 //f.pln(" @@@@ AFTER class 1: cs = "+cs+" par_prevlprob = "+par_prevlprob+" par_prevuprob = "+par_prevuprob);

				 cumet = oldcum; 

			 } else {
				 double oldlb = lb; 
				 double oldub = ub;
	        	 double deltaatcs = delta;
				 TranToState fh = findhold(cs);
				 if (fh != null) {
					 ht1 = fh.rateOrMean.cmp(); 
					 ht2 = fh.stdev.cmp(); 
					 ht3 = fh.fract.cmp();
				 } else 	{ 
					 println(" *** ERROR: HOLDING TIME AT "+cs+" NOT DEFINED"); 
					 return new Pair<Double,Double>(-1.0,-1.0);
					 // goto 1000;
				 };
				 delta = delta + ht2;
				 //f.pln(" $$ delta = "+delta+" ht2 = "+ht2);
				 ArrayList<TranToState> lp = succst.get(cs);        	
				 double sumlam = 0.0; 
				 double sumfract = 0.0;
				 //f.pln("\n travlee: sumfract = "+sumfract);
				 for (int i = 0; i < lp.size(); i++) {
					 TranToState lpArrow = lp.get(i);
					 //f.p(" travlee(holding): i = "+i+" lpArrow = "+lpArrow);
					 if (lpArrow.st > 0) {                                           // used to be >= 0
						 if (lpArrow.tt == trantype.recov) { /* recovery type */
							 double frac = lpArrow.fract.cmp();
							 sumfract = sumfract + frac;
							 //f.pln(" travlee: frac = "+frac+" sumfract = "+sumfract);
						 } else {
							 sumlam = sumlam + lpArrow.rateOrMean.cmp();
						 }
					 }
				 }

				 if (Math.abs(1.0-sumfract) > 1.0000000000e-10) {
					 println(" *** ERROR: SUM OF EXITING "+"PROBABILITIES IS NOT 1 AT "+cs+" ("+sumfract+")");
					 return new Pair<Double,Double>(-1.0,-1.0);
					 //goto 1000;
				 };
				 //while (p <> nil) { /* -- process all transitions from cs -- */
				 for (int j = 0; j < p.size(); j++) {
					 TranToState pArrow = p.get(j);
					 lb = oldlb; 
					 ub = oldub;
					 if (pArrow.st > 0) {                          // used to be >= 0
						 if (pArrow.tt == trantype.recov) {
							 double frac = pArrow.fract.cmp();
							 ub = ub*frac;
							 lb = lb*Math.exp(-sumlam*ht2)*(frac - 1.0 + ht3);
							 nhp.cl = Tranclass.class2; 
							 nhp.lam_alpha_fract = frac; 
						 } else {                /* --- slow on path --- */
							 alpha = pArrow.rateOrMean.cmp();
							 ub = ub*alpha*(ht3*ht1 + (1.0-ht3)*(ht2 + 1.0/sumlam));
							 lb = lb*alpha*Math.exp(-sumlam*ht2)*(ht3*ht1+ht2*(1.0-ht3));
							 nhp.cl = Tranclass.class3; 
							 nhp.lam_alpha_fract = alpha; 
							 nhp.gam_muh = ht1;
						 }
						 //f.pln(" ...............CALL-fast travlee cs = "+cs+" hp = "+strHistoryList(hp));
						 Pair<Double,Double> ptrav = travlee(pArrow.st,hp,numet,cumet,lb,ub,delta,ptrunc,autofast,autoprune);
						 //f.pln(" ................. AFTER-fast travlee cs: hp = "+strHistoryList(hp));
						 par_prevlprob = par_prevlprob + ptrav.first;
						 par_prevuprob = par_prevuprob + ptrav.second;
					 } // for
		        		lb = oldlb; 
		        		ub = oldub; 
		        		delta = deltaatcs;
				 }
			 }
		 }

		 if (!goto999) {
			 par_prevlprob = lb; 
			 par_prevuprob = ub;
		 }
		 hp.remove(0);
		 //f.pln(" ................. AT END ------------- travlee cs: p = "+p+" hp = "+strHistoryList(hp));
		 //f.pln(" ................. AT END ------------- travlee cs = "+cs+" lb = "+lb+" ub = "+ub);
		 //f.pln(" ..... EXIT travlee: cs = "+cs+" lb= "+lb+" ub="+ub+" delta="+delta+" cumet = "+cumet); 
		 return new Pair<Double,Double>(par_prevlprob,par_prevuprob);

} /* travlee */

//	  /*   static void underflowchk(y: double);
//	     var uf: integer;
//	     {
//	        if (y <= minreal)
//	           uf = 0
//	        else
//	           uf = trunc( (ln(y) - ln(minreal) )/ln(10.0) );
//	        if (uf < 6)
//	           print(" .. ~ "+uf:1+" DIGITS ACCURACY: UNDERFLOW");
//	     }; */ /* underflowchk */
//
	     static void prstlbub (double slprob, double suprob, double ldig, double udig) {
	        print(" "+f.Em5(slprob)+"   "+f.Em5(suprob));
	        /* underflowchk(suprob); */
	        if (qtbad) println(" .. Q(T) INACCURATE");
	        qtbad = false; 
	        if ((ldig < 100.0) || (udig < 100.0)) { 
	           print("    <ExpMat");
	           if ((Front.listlevel >= 4) || (ldig <= 6) || (udig <= 6))
	              print(" - "+(int)ldig+","+(int)udig);
	           print(">");
	           };
	        println("");	        
	     }; /* prstlbub */
	     
	     static void printstates(ArrayList<DeathStateInfo> d) {
	    	 for (int i = 0; i < d.size(); i++) { 
	    		 DeathStateInfo dArrow = d.get(i);
	    		 double ldig = digits(dArrow.lprob,dArrow.lplace);
	    		 double udig = digits(dArrow.uprob,dArrow.uplace);
	    		 double slprob = dArrow.lprob;  
	    		 double suprob = dArrow.uprob;
	    		 if (Front.isprunest(dArrow.st)) { 
	    			 anyprune = true;
	    			 asprunepl = asprunepl + slprob;
	    			 asprunepu = asprunepu + suprob;
	    		 }  else {
	    			 dpu = dpu + dArrow.uprob;
	    			 Front.lbfail = Front.lbfail + dArrow.lprob;
	    			 lbplace = Math.max(lbplace,dArrow.lplace);
	    			 if (Front.listlevel >= 2) {
	    				 print("   "+f.Im4(dArrow.st)+"      ");
	    				 prstlbub(slprob, suprob, ldig, udig);
	    			 }
	    		 }
	    		 Front.ubfail = Front.ubfail + dArrow.uprob;
	    		 ubplace = Math.max(ubplace,dArrow.uplace);
	    		 if (Front.listlevel >= 3) {
	    			 //f.pln(" $$$$$$$$$$$$ write probs to initp!!! "+Front.firstinitp);
	    			 if (! Front.firstinitp) Front.initp.println(",");
	    			 Front.firstinitp = false;
	    			 Front.initp.print(" "+dArrow.st+": ("+f.Em5(slprob)+", "+f.Em5(suprob)+")");
	    		 };
	    	 }
	     } /* printstates */

	     static void printprunes(ArrayList<DeathStateInfo> p) {
	    	 for (int i = 0; i < p.size(); i++){
	    		 DeathStateInfo pArrow = p.get(i);
	    		 if (Front.isprunest(pArrow.st)) { 
	    			 print(" prune "+f.padLeft(""+pArrow.st,4)+"  ");
	    			 double ldig = digits(pArrow.lprob,pArrow.lplace);
	    			 double udig = digits(pArrow.uprob,pArrow.uplace);
	    			 double slprob = pArrow.lprob;  
	    			 double suprob = pArrow.uprob;
	    			 prstlbub(slprob, suprob, ldig, udig); 
	    		 }
	    	 }
	     } /* printprunes */

	     static void printopers(ArrayList<DeathStateInfo> p) {
	    	 for (int i = p.size()-1; i >= 0; i--){
	    		 DeathStateInfo pArrow = p.get(i);
	    		 double ldig = digits(pArrow.lprob,pArrow.lplace);
	    		 double udig = digits(pArrow.uprob,pArrow.uplace);
	    		 double slprob = pArrow.lprob;  
	    		 double suprob = pArrow.uprob;
		         print(" "+f.padLeft(""+pArrow.st,6)+"      ");
	    		 prstlbub(slprob, suprob, ldig, udig); 
	    		 // print to the initp FILE ------------
		         Front.initp.println(",");
		         Front.initp.print(" "+pArrow.st+": ("+f.Em5(slprob)+", "+f.Em5(suprob)+")");
	    	 }	    	 
	     } /* printopers */

	     
	  static void compute(int startstate, int ptrunc, int autofast, int autoprune, int warndig) {                  /* ----------------------------*/
         //f.pln(" $$$************** START compute(): timet =  "+Front.timet);
	     if (bigst == -1) {
	    	 println(" NO STATES ARE IN MODEL!!");
	    	 return;
	     }
	     /* compute */
	     pathcount = 0; 
	     ratebig = false; 
	     fastzero = false; 
	     cnttrunc = 0;  
	     cntprune = 0; 
	     probprune = 0.0; 
	     sumofdeath = 0;
	     numdeath = 0; 
	     nextautop = 1;
	     tentomwarn = 0.5*Math.exp(-warndig*Math.log(10.0));
	     if (Front.listlevel == 2 || Front.listlevel == 3) hdr2();
	     else if (Front.listlevel == 4) hdr3();
	     else if ((Front.listlevel > 4) && (!Front.specid.equals("")))
	    	 println(" "+Front.specid+" = "+f.Em5(Front.specval));

	     fdeath.clear();   //disposetree(fdeath); 
	     splist.clear();   //disposetree(splist);
	     ArrayList<HistoryTrans> historyList = new ArrayList<HistoryTrans>(100);
	     ArrayList<Initialrec> inlist = Front.initlist;
	     if (Front.rectype == lee) {
	    	 travlee(startstate,historyList,0,1.0,1.0,1.0,0.0, ptrunc,autofast,autoprune);
	     } else  {
	    	 if (inlist.size() > 0) {
	    		 for (int i = 0; i < inlist.size(); i++) {  
	    			 Initialrec inlistArrow = inlist.get(i);
	    			 //f.pln(" $$$$$$$$ inlistArrow = "+inlistArrow);
	    			 traverse(inlistArrow.st,historyList,0,1.0,inlistArrow.pbl.cmp(),inlistArrow.pbu.cmp(),0.0, ptrunc,autofast,autoprune);
	    		 } 
	    	 } else {  
	    		 Pair<Double,Double> ptrav = traverse(startstate,historyList,0,1.0,1.0,1.0,0.0,ptrunc,autofast,autoprune);
	    		 //f.pln(" ................. START: AFTER traverse cs: hp = "+strHistoryList(historyList));
	    	 }
	    	 //f.pln(" ^^^^^^^^^^ compute:  AFTER traverse cs: historyList = "+strHistoryList(historyList));
	     }
	     //	     /* ------ print out results ------ */
	     Front.lbfail = 0.0; 
	     Front.ubfail = 0.0;
	     lbplace = -LargePlace; 
	     ubplace = -LargePlace;
	     if (Front.listlevel >= 4) hdr2();
	     asprunepl = 0.0; 
	     asprunepu = 0.0; 
	     anyprune = false; 
	     dpu = 0.0;
	     if (Front.listlevel >= 3) { 
	    	 Front.firstinitp = true; 
	         Front.initp.println("INITIAL_PROBS(");
	     }
	     printstates(fdeath);
	     if (Front.prune_d == 0) {
	    	 probprune = probprune+Front.minreal*cntprune;  /* total possible underflow */
	     }
	     if (Front.listlevel >= 2) {	        
	    	 if (probprune > 0.0) {
	    		 print(" sure prune  ");
	    		 double slprob = 0.0;  
	    		 double suprob = probprune;
	    		 double ldig = 10000; 
	    		 double udig = 10000;
	    		 prstlbub(slprob, suprob, ldig, udig);
	    	 };
	    	 if (anyprune) {
	    		 println("              -----------   -----------");
	    		 println("   SUBTOTAL   "+f.Em5(Front.lbfail)+"  "+f.Em5(dpu));
	    		 println("");
	    		 println(" PRUNESTATE    LOWERBOUND    UPPERBOUND");
	    		 println(" ----------   -----------   -----------");
	    		 printprunes(fdeath);
	    		 println("              -----------   -----------");
	    		 println("   SUBTOTAL   "+f.Em5(asprunepl)+"   "+f.Em5(asprunepu));
	    		 println("");
	    	 } /* if (Front.listlevel >= 2 */
	     }
	     //
	     Front.ubfail = Front.ubfail + probprune;  /* add sure prune prob to ub total */
	     if (Front.ubfail > 1.0) Front.ubfail = 1.0;
	     /*pwarn = Front.ubfail < probprune*(10**warndig) ;*//*ultrix-3/88-phs*/
	     boolean pwarn = Front.ubfail < probprune*(Math.exp(Math.log(10.0)*warndig) );
	     if (Front.listlevel ==  1) {
	    	 if (Front.specid.equals("")) {
	    		 print("               "+f.Em5(Front.lbfail)+"   "+f.Em5(Front.ubfail));
	    	 } else {
	    		 print("  "+f.Em5(Front.specval)+"   "+f.Em5(Front.lbfail)+"   "+ f.Em5(Front.ubfail)); 
	    	 }
	    	 if (probprune > 0) {
	    		 print("    <prune ");
	    		 print(f.Em5(probprune)+">");
	    	 };
	    	 if (qtbad) print(" .. Q(T) INACCURATE");
	    	 qtbad = false; 
	     } else if (Front.listlevel >= 2 && Front.listlevel <= 4) {
	    	 println("");
	    	 print(" TOTAL        "+f.Em5(Front.lbfail)+"   "+f.Em5(Front.ubfail));
	     } else if (Front.listlevel == 5 || Front.listlevel == 6) {
	         String dashline = "-------------------------------------------------------";
	    	 println(dashline);
	    	 println(" TOTAL LOWER,UPPER = "+Front.lbfail+" "+Front.ubfail);
	    	 println(dashline);
	     }
	     double relerr;
	     if (Front.ubfail > 0.0) relerr = (Front.ubfail-Front.lbfail)/Front.ubfail;
	     else relerr = 0.0;
	     lbdig = digits(Front.lbfail,lbplace);
	     ubdig = digits(Front.ubfail,ubplace);
	     if (Front.listlevel > 0)  {
	    	 if (Front.ubfail > 0.0) {
	    		 relerr = (Front.ubfail-Front.lbfail)/Front.ubfail;
	    	 } else relerr = 0.0;
	    	 if ((lbdig < 100.0) || (ubdig < 100.0)) {
	    		 if ((probprune == 0) || (Front.listlevel > 1)) print("   ");
	    		 print(" <ExpMat");
	    		 if ((Front.listlevel >= 3) || (lbdig <= 6) || (ubdig <= 6))
	    			 print(" - "+(int) lbdig+","+(int) ubdig);
	    		 print(">");
	    	 }
	    	 if (relerr > 0.05) { /* greater than 5% difference */
	    		 if (recslow) print(" .. RECOVERY TOO SLOW");
	    		 if (ratebig) print(" .. RATE TOO FAST");
	    		 if (deltabig) print(" .. DELTA > TIME");
	    		 if (stdbig) print(" .. ST. DEV. TOO BIG");
	    		 if (qttoolong) print(" .. Q(T) LIST TOO LONG FOR EXPMAT");
	    	 };
	    	 if (fastzero) print(" .. FAST RATE = 0");
	    	 if (bigst < 0) print(" .. 0 STATES IN MODEL");
	    	 if (pwarn) print(" .. PRUNING TOO SEVERE");
	     }  else {
	        Front.erun = recslow || ratebig || stdbig || deltabig || qttoolong || (bigst<0) || pwarn;
	     }
	     recslow = false; 
	     ratebig = false; 
	     stdbig = false; 
	     qttoolong = false;  
	     deltabig = false; 

	     if (Front.listlevel >= 3) {      
	        println(""); println(""); println("");
	        println(" OPER-STATE    LOWERBOUND    UPPERBOUND");
	        println(" ----------   -----------   -----------");
	        printopers(splist);
	        Front.initp.println(""); 
	        Front.initp.println("  ); ");
	        }
	     if (Front.listlevel > 0) println("");
	     if (Front.listlevel >= 4) {
	        println(" ___________________________________________________________________________");
	     }
	     //f.pln(" ************** END compute() ");
  } /* compute */

	  static void execstats(int ptrunc, int autoprune, int warndig) {
		  int dig;
		  if (lbdig < ubdig) dig = (int) lbdig;
		  else dig = (int) ubdig;
		  if (dig < wdig) wdig = dig;
		  if (probprune > wprobprune) { 
			  wprobprune = probprune; wcntprune = cntprune;
		  };
		  if (Front.prune_d > wprune) wprune = Front.prune_d;   /* V8.0 */
		  if (asprunepu > wasprunepu) {
		     wasprunepu = asprunepu; wasprunepl = asprunepl;
		  }

		  if (Front.listlevel >= 2){ 
			  println("");
			  if (cnttrunc > 0) { 
				  println("  "+cnttrunc+" LOOP(S) TRUNCATED AT DEPTH "+ptrunc);
			  };
			  print("  "+pathcount+" PATH(S) TO DEATH STATES");
			  if (cntprune > 0){ 
				  print(", "+cntprune+" PATH(S) PRUNED");
			  if (autoprune == 0) 
				  print(" AT LEVEL "+Front.prune_d);
			  else { 
				  println(""); print("  HIGHEST PRUNE LEVEL = "+f.Em5(Front.prune_d));
			  }
			  /*         println("  SUM OF PRUNED STATES PROBABILITY = "+
	           probprune);    */
			  };
			  println("");
			  if (dig < 100.0)
			  { 
				  if (dig < warndig) print(" *** INSUFFICIENT NUMERICAL ACCURACY:");
				  println("  Q(T) ACCURACY >= "+dig+" DIGITS");
			  }
	        }
	  }; /* execstats */
   
	  static void worststats(int autoprune, int warndig) {
	     if (Front.listlevel == 1) {
	        print(" "+pathcount+" PATH(S) TO DEATH STATES");
	        if (wcntprune > 0) {
	           print(" "+wcntprune+" PATH(S) PRUNED");
	           if (autoprune == 0) print(" AT LEVEL "+wprune);
	           else { println(""); print(" HIGHEST PRUNE LEVEL = "+f.Em5(wprune));
	                };
	  /*         if (Front.listlevel = 1)
	              println(" SUM OF PRUNED STATES PROBABILITY = "+ wprobprune); */
	           };
	        println("");
	        if (wcnttrunc > 0)  {
	        	println(" "+wcnttrunc+" LOOP(S) TRUNCATED AT DEPTH "+
	           wptrunc);
	           }
	        if (! Front.emptyprune())
	           println(" ASSIST PRUNE STATE PROBABILITY IS IN ["+
	           wasprunepl+"+ "+wasprunepu+"]");
	        if (wdig < 100.0) { 
	           if (wdig < warndig) print(" *** INSUFFICIENT NUMERICAL ACCURACY:");
	           println(" Q(T) ACCURACY >= "+wdig+" DIGITS");
	           };
	        };
	  } /* worststats */

  
	  public static void runSure(String fileName) {
		  //f.pln(" $$ runSure: fileName = "+fileName+" autorun = "+autorun);
		  if (Front.openFile(fileName)) {
			  println(""); progname();
			  println("  NASA Langley Research Center");      
			  if (!fileName.equals("")) println("filename = "+fileName+"\n"); 
              Front.execSURE();
			  if (autorun) Front.runit();                                  // ADDED RWB July 2014 for debug
		  } else {
			  println(" $$ ERROR: openFile failed!");  
		  }
	  }
}
