package gov.nasa.larcfm.Reliab;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import javax.imageio.IIOException;
import javax.swing.JOptionPane;

enum rwtype {rwnull,rwto,rwrun,rwexit,rwdebug,rwshow,rwread,
             rwget,rwhelp,rwscan,rwinput,rwcalc,rwlee,
             rwclr,rwby,rwplot,rwif,rwthen,rworprob,
             rwinit, rwfast, rwread0, rwlinear, rwprune};

 


    class Idrec {		   
    	String id;
    	ValuRec val;   // coef, base, linear
    	rwtype rw;

    	Idrec() {
    		id = "";
    		val = new ValuRec(0.0,0.0,false);
    		rw = rwtype.rwnull;
    	}

    	Idrec(String ii, ValuRec vv, rwtype rw) {
    		id = ii;
    		val = new ValuRec(vv);
    		this.rw = rw;
    	}

    	public String toString() {
    		return "["+id+", "+val+", "+rw+"]";
    	}
    }

	
	class RunResults {  // was runresp
		String id;
		int runno;
		double lb;
		double ub;
		
		public RunResults(String id, int runno, double lb, double ub) {
			super();
			this.id = id;
			this.runno = runno;
			this.lb = lb;
			this.ub = ub;
		}

		public String toString() {
			return "runresp [id=" + id + ", runno=" + runno + ", lb=" + lb
					+ ", ub=" + ub + "]";
		}				
	}	
	
public class Front {
	static final char cheof = '$';
	static final char cheol = '\\';
    static final char cheoc = ';';
    static final int  idleng = 12;              /* size of identifier buffer */
    //static boolean    command_line_files = false;
        
    // SURE parameters
    //static int qtcalc;                    /* q(t) technique   0 = algebraic, 1 = Pade Approx,  2 = automated decision
    //static int start;                     /* start state */
    //static int ptrunc;                    /* trunc level */
    //static int warndig;                   /* prune warn dig. */
    //static int startstate;                /* start state */
    //static int autofast;                  /* AUTOFAST constant value */
    //static int autoprune;                 /* AUTOPRUNE constant value */
    //static int notrunc;                   /* NOTRUNC constant value */
	//static int maxpts = 10000;            /* max # points for plot routines */
    static int listlevel = 1;               /* list level */
    static int rectype;                     /* (white = 0,lee =1) */            

    
    final static double deflistlevel = 1;
    final static double defqtcalc = 2;      // default QTCALC,  0 = algebraic, 1 = Pade Approx,  2 = automated decision
    final static double defautofast = 0; 
    final static double defautoprune = 1;
    final static double defstart = -1;      // -1 is undefined,  #determinestart# searches for a start state
    final static double defptrunc = 3;
    final static double defnotrunc = 0;
    final static double defwarndig = 2;
    final static double defprune = Sure.probNearZero;
	static final double  maxreal = Double.MAX_VALUE;
	static final double  minreal = Double.MIN_VALUE;    
	static final double lnmaxreal = Math.log(maxreal);
	static final double lnminreal = Math.log(minreal);
    static int  tinyexp = -308;   /* smallest sun (VMS G_FLOATING) exponent */
	static char tab = '\t';

	static int maxvpr = 100;
	enum tokens {tknull, tkid, tkint, tkreal, tkeq, tkadd, tksub, tkmul,
		tklt, tkgt, tkle, tkge, tkcomma, tksemi, 
		tklp,tkrp,tkdiv,tkpow,tklb,tkrb,tkat,tkcol,tkrw,
		tkquo,tkmod,tkcyc,tkerr};

	   enum functypes {fabs, farccos, farcsin, farctan,fcomb,fcos,fexp,ffact,fgamma,
	                   fln,fperm,fsin,fsqrt,fnil}; 
	              
	   enum inmodes {interactive,batch,scratch};

	   enum debugflags {dbglex, dbgparse};
	   
	
	   /*--------------- lexical processing variables --------------- */

	   static int chcnt = 0;                   // current char. in line */
       static int insz = 0;                    // no. chars. in inbuf */
	   static tokens tkn;                      // token found by scanner */
       static String id;                       // id found by scanner */
	   static int intv;                        // integer value */
	   static double val;                      // real value */	   
	   static inmodes inmode;                  // interactive mode flag */
       static int  echo;                       // output echo flag */
	   static FileInputStream fis;             // = new FileInputStream(fin);
	   static BufferedReader rmdReader;        // = new BufferedReader(new InputStreamReader(fis));	   	  
	   static boolean eofFlag = false;         // end of file flag
 	   static String  inbuf;                   // buffer for input */
	   static char    ch;                      // character just scanned */
	   static char    chlast;                  // previous ch */
	   static int     linecnt; //= 0*/;        // current linecnt */
       static boolean lineechoed = false;      // line echoed to output */
       static boolean parl;                    // -l UNIX parameter: echo input from pipe */
       static boolean parn;                    // -n UNIX parameter: do not echo input from pipe */
       static boolean parr;                    // -r UNIX parameter: sure -r ex.mod */
       static boolean forcelinear;             // prohibit nonlinear files */

       static ArrayList<String> scrFile = new ArrayList<String>(10);
       static int scr_line = 0;

       /* -------------------- parser variables -------------------- */

       static Map<String, Idrec> hashtable = new HashMap<String, Idrec>(10);    // maps airport name to its next available time

       static Map<String, functypes> functable = new HashMap<String, functypes>(10);    // maps airport name to its next available time

       static ValuRec gval = new ValuRec(0.0,-1.0,true);                                      /* last value found by parser */
       static final ValuRec valm1 = new ValuRec(0.0,-1.0,true);  /* -1 of type valurec */
       static final ValuRec val0 =  new ValuRec(0.0,0.0,true);    /* zero of type valurec */
       static boolean errflag;                   /* error flag */
       static boolean errlast;  /*= false*/      /* error last statement */
       static int strstart;                  /* string start for graphics */
       static int firstst;                   /* first state number */
       static rwtype rw = rwtype.rwnull;                         /* last reserved word */
       static boolean geometric; /*= false*/;    /* increment method */
       static ValuRec scrval;                    /* for entry of nonlinear var. */
       static double speclow;       /* range of "variable,  note: called special */
       static double spechigh;
       static double byinc;
       static double byfact;   
       static int    numpoints;                 /* no. pts. to calculate */
       static String runnm;                             /* number of runs */
       static int nvpr;                      /* number of variables/run */
       static boolean orok; /*= true*/;          /* runs ok for orprob */
       static String lastfread;                  /* last file read */
       static boolean initflag;
       static int beginreadtm;
       // --- orprob variables ---
       static ArrayList<ArrayList<RunResults>> runvals = new ArrayList<ArrayList<RunResults>>(maxvpr);
       //static ArrayList<RunResults> [] runvals = (ArrayList<RunResults>[])new ArrayList[maxvpr];
       static ArrayList<ArrayList<RunResults>> endp = new ArrayList<ArrayList<RunResults>>(maxvpr);       
       //static ArrayList<RunResults> [] endp = (ArrayList<RunResults>[])new ArrayList[maxvpr];       
       static {
    	   for (int i = 0; i < maxvpr; i++) {
    		   runvals.add(null);
    		   endp.add(null);
    	   }
       }
       static double [] runvv = new double[maxvpr];                         // array[1..maxvpr] of double; -- value of variable ORPROB */


       /* ---------- calculator and graphics variables ---------- */

       static boolean calclast;                  /* true iff calc not run */
       static String calcfun;                   /* string: for plotting */
       static boolean disp;                  /* graphics flag */
       static boolean isop;                  /* flag for open gks window */
       static int plti;                      /* # of points in plot */
       static String pltid;                      /* x - axis label id */
       //         static double [] pltx = new double[maxpts];
       //         static double [] plty = new double[maxpts];
       //         static double [] pltz = new double[maxpts];            /* arrays of plot data */
       static boolean pltsaved;                  /* true iff plot+ called */
       static String zid = "";   // : idtype /*= '        '*/;      /* contour z var. id */
       static double zval;                         /* current z value */

       static ArrayList<Integer> asprunlist = new ArrayList<Integer>(10);                /* ASSIST prune list */


       // EXECUTION VARIABLES

       enum trantype {expo, fastexpo, recov};      

       static double  timet;                     /* mission time */
       static String  specid = "";               /* "variable" name */
       static boolean erun;                      /* errors during run */
       static double  prune_d;                   /* pruning prob. */
       static boolean nonlinear;                 /* nonlinear flag */
       static double  specval;                   /* special var value */
       static double  ubfail;                    /* upper bound value */
       static double  lbfail;                    /* lower bound value */
       static int     runno;                     /* run number */
       static boolean esyn1;                     /* syntax errs before run */
       //static boolean  stopflag;                  
       static String initpfname="";
       static PrintWriter initp;
       static boolean firstinitp = true;
       static ArrayList<Initialrec> initlist = new ArrayList<Initialrec>(10);
         
//         /* -------------- external declarations ------------------*/
//
//         function cgamma(x:double; var ans:double):integer; C;
//         function ifact(x:integer):integer; C;
//         function icomb(n,m:integer):integer; C;
//         function iperm(n,m:integer):integer; C;


       static boolean openFile(String inFile) {
    	   //f.pln(" $$$$ openFile: ENTER infile = "+inFile);
    	   try {
    		   if (inFile.equals("")) {
    			   InputStreamReader isr = new InputStreamReader(System.in);
    			   rmdReader = new BufferedReader(isr);
    			   Front.inmode = Front.inmodes.interactive;
    			   //f.pln(" $$$$$$$$$ rmdReader set to read from console interactively!");
    		   } else {
    			   File fin = new File(inFile);
    			   fis = new FileInputStream(fin);
    			   rmdReader = new BufferedReader(new InputStreamReader(fis));
    			   Front.inmode = Front.inmodes.batch;
    		   }
    		   Front.lastfread = inFile;
    	   } catch (IOException e) {
    		   Sure.println(" FILE "+inFile+" NOT FOUND!");
    		   closeRmdReader();
    		   return false;
    	   } 
    	   return true;
       }
	
    // loads inbuf,  sets insz and chcnt and eofFlag
    static void fillup(boolean ech) {
        //f.pln(" $$^^: fillup: ENTER inmode = "+inmode+" autorun = "+Sure.autorun+" eofFlag = "+eofFlag);    	
    	lineechoed = ech;
    	boolean emptyinbuf = true;
    	while (emptyinbuf && !eofFlag) {
    		try {
    			if (inmode == inmodes.interactive) {
    	    		Sure.print(linecnt+"? ");
    				if ((inbuf = rmdReader.readLine()) == null) {
    					eofFlag = true; 
   			    	} else {
    					inbuf = inbuf+cheoc+cheol;
    					insz = inbuf.length();
    					chcnt = 0;
    				}
    			} else {
    				if ((inbuf = rmdReader.readLine()) == null) {
    					eofFlag = true;  
    					//f.pln("$$$ fillup: inbuf = "+inbuf+" chcnt = "+chcnt+" insz = "+insz);
    				} else {
    					//inbuf = inbuf.toUpperCase();
    					insz = inbuf.length();
    					chcnt = 0;
    				}
    			}
    			linecnt++;
    		} catch (IOException x) {
    			System.err.format("fillup: IOException: %s%n", x);
    			Thread.dumpStack();
    			System.exit(0);
    			eofFlag = true;
    		}
    		emptyinbuf = (insz <= 0);
    	}
    	//f.pln("%%%% fillup: EXIT insz = "+insz+" chcnt = "+chcnt+" inbuf = "+inbuf);
    } // fillup 

	
//    static void fakerun() {
//    	f.pln(" $$$$ fakerun: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
//       chcnt = 0; insz = 5;
//       inbuf = "run"+cheoc+cheof;
//    }
	
    static void readch() {
    	//f.pln(" $$$ readch: IN ................... readch: chcnt = "+chcnt+" ch = "+ch+ " insz = "+insz+" inmode = "+inmode); 
    	if (inmode == inmodes.scratch) {
    		//f.pln(" readch: inmode = "+inmode+" scr_line = "+scr_line+" scr.size() = "+scr.size());	
    		if (forcelinear) {
    			Sure.println("*** ERROR: Model is non-linear. (LINEAR;)");
    			f.halt();
    		}
       		if (eofFlag) ch = cheof;
       		else if (chcnt +1 >= insz) {
       			if (scr_line < scrFile.size()) {
       				inbuf = scrFile.get(scr_line);
       				if (inbuf == null) { 
       					insz = 0;
          				eofFlag = true;
          				//f.pln(" $$$ readch SCR 0: %%%%%%%%%%%%%%%%%%%%%%%%%%% set ch = cheof !!!!!!!!!!!!!!!!!!!!");
           				ch = cheof;
       				} else {
       					//f.pln(" readch scr_line = "+scr_line+" inbuf = "+inbuf);    				 
       					insz = inbuf.length();       					
       				}
       				scr_line++;
       				chcnt = 0;
       			} else {
       				eofFlag = true;
      				//f.pln(" $$$ readch SCR 1: %%%%%%%%%%%%%%%%%%%%%%%%%%% set ch = cheof !!!!!!!!!!!!!!!!!!!!");     				 
       				ch = cheof;
       				scr_line = 0;
       			}
       		} else { 
    			chcnt = chcnt + 1;
    		} 
    	} else {
    		if (chcnt + 1 >= insz) {
    			fillup(false);
    			if (echo > 0 && inmode != inmodes.interactive) {
    				Sure.println(linecnt+": "+inbuf);        
    			}
    			chcnt = 0;
    			if (! forcelinear) {
    				scrFile.add(inbuf);
    			}
    		} else {
    			chcnt = chcnt + 1;
    		}     		
    	}
    	//f.pln(" $$$ readch_0: eofFlag = "+eofFlag+" insz = "+insz+" chcnt = "+chcnt+" ch = "+ch);
		if (eofFlag) {
			process_eof();
			return;
		} else {
			ch = inbuf.charAt(chcnt);
		}
		chlast = ch;
		ch = Character.toUpperCase(ch);
    	//f.pln(" $$$ readch: .......eofFlag = "+eofFlag+" chcnt = "+chcnt+" ch = "+ch);
    }// readch
    
    static void process_eof() {
    	//f.pln(" $$$$ process_eof: inmode == "+inmode+" Front.command_line_files = "+Front.command_line_files+"          eofFlag = "+eofFlag);
     	//f.pln(" $$$$ process_eof:inbuf = "+inbuf+" chcnt = "+chcnt+" insz = "+insz+" rw = "+rw);
    	if (!Sure.gui_active) {        // switch back to interactive after read command	
     		tkn = tokens.tksemi;
    		if (Sure.autorun) {
  				//f.pln(" $$$ process_eof 1: %%%%%%%%%%%%%%%%%%%%%%%%%%% set ch = cheof !!!!!!!!!!!!!!!!!!!!");     				 
       		    ch = cheof;
    		} else {
    			openFile("");
    	   		chcnt = 0; 
        		insz = 0; 
        		ch = ' '; 
		        Front.inmode = Front.inmodes.interactive;
	     		eofFlag = false;
    		}
     	} else {   // program initiated from GUI
    		if (inmode != inmodes.scratch) closeRmdReader();
			//f.pln(" $$$ process_eof 2: %%%%%%%%  inmode = "+inmode+" set ch = cheof !!!!!!!!!!!!!!!!!!!!");     				 				 
    		ch  = cheof;
    	}
    }
	
	static void clear() {
		//inmode = inmodes.interactive;
		linecnt = 0;
		chcnt = 0;
		hashtable.clear();
		errlast = false;
		orok = true;
		runno = 0;
		//autofast = 0;
		initlist.clear();   
		echo = 1;       
		esyn1 = false;   
		forcelinear = false;
		parn = false;
		parr = false;
		parl = false;
	}
    
    
	   /*VMS*/ /*[global]*/
	static void startup() {
		Exact_qt.c_pade[ 1 ] =   0.5;   
		Exact_qt.c_pade[ 2 ] =   1.1764705882352e-01;
		Exact_qt.c_pade[ 3 ] =   1.7156862745098e-02;
		Exact_qt.c_pade[ 4 ] =   1.7156862745098e-03;
		Exact_qt.c_pade[ 5 ] =   1.2254901960784e-04;
		Exact_qt.c_pade[ 6 ] =   6.2845651080945e-06;
		Exact_qt.c_pade[ 7 ] =   2.2444875386051e-07;
		Exact_qt.c_pade[ 8 ] =   5.1011080422845e-09;
		Exact_qt.c_pade[ 9 ] =   5.6678978247605e-11;
		enterfunc("ABS", functypes.fabs);
		enterfunc("ARCCOS", functypes.farccos);  
		enterfunc("ARCSIN", functypes.farcsin);  
		enterfunc("ARCTAN", functypes.farctan);  
		enterfunc("COMB", functypes.fcomb);    
		enterfunc("COS", functypes.fcos);     
		enterfunc("EXP", functypes.fexp);     
		enterfunc("FACT", functypes.ffact);    
		enterfunc("GAM", functypes.fgamma);    
		enterfunc("LN", functypes.fln);      
		enterfunc("PERM", functypes.fperm);    
		enterfunc("SIN", functypes.fsin);     
		enterfunc("SQRT", functypes.fsqrt);    
		lastfread ="";
        clear();
		Sure.computestartup();
	} // /*startup */

	
	
	 
	/* --------------------- lexical scanner --------------------------- */


	static void error(int e) {
		errflag = true;
		//f.pln(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEBUG error: e = "+e);
		if ((e != 0) && (! lineechoed)) {
			//Sure.print(linecnt+"? ");
			lineechoed = true;
			Sure.println(inbuf);
		}
		if (inmode != inmodes.scratch) {
			for (int i = 0; i <= chcnt; i++) Sure.print(" ");
			Sure.print("^ ");
	        esyn1 = true;
			switch (e) {
			case  1  : Sure.print("INPUT LINE TOO LONG "); break;
			case  2  : Sure.print("NUMBER TOO LONG "); break;
			case  3  : Sure.print("INTEGER EXPECTED"); break;
			case  4  : Sure.print("COMMA EXPECTED "); break;
			case  5  : Sure.print("= EXPECTED"); break;
			case  6  : Sure.print("REAL EXPECTED"); break;
			case  7  : Sure.print("SEMICOLON EXPECTED"); break;
			case  8  : Sure.print("IDENTIFIER NOT DEFINED"); break;
			case  9  : Sure.print("IDENTIFIER EXPECTED "); break;
			case  10 : Sure.print("ILLEGAL STATEMENT"); break;
			case  11 : Sure.print("ILLEGAL TRANSITION TO ITSELF"); break;
			case  12 : Sure.print("READ WHILE READING NOT ALLOWED"); break;
			case  13 : Sure.print("> EXPECTED"); break;
			case  14 : Sure.print("ONLY 1 VARIABLE ALLOWED"); break;
			case  15 : Sure.print("CONSTANT EXPECTED"); break;
			case  16 : Sure.print("ILLEGAL CHARACTER"); break;
			case  17 : Sure.print("FILE NAME TOO LONG"); break;
			case  18 : Sure.print("FILE NAME EXPECTED"); break;
			case  19 : Sure.print("FILE NOT FOUND"); break;
			case  20 : Sure.print("ILLEGAL USE OF *"); break;
			case  21 : Sure.print(") EXPECTED"); break;
			case  22 : Sure.print("ILLEGAL 'BY' VALUE"); break;
			case  23: Sure.print("DIVISION BY ZERO NOT ALLOWED"); break;

			case  24 : Sure.print("MUST BE IN 'READ' MODE"); break;
			case  25 : Sure.print("EXPRESSION CANNOT CONTAIN THE VARIABLE"); break;
			case  26 : Sure.print("] EXPECTED"); break;
			case  27 : Sure.print("THIS CONSTRUCT NOT PERMITTED IN WHITE MODE"); break;
			case  28 : Sure.print("THIS CONSTRUCT NOT PERMITTED IN LEE MODE"); break;
			case  29 : Sure.print("TOO MANY PARAMETERS"); break;
			case  30 : Sure.print("LEE @ REQUIRES THREE PARAMETERS"); break;
			case  31: Sure.print("ARGUMENT TO LN OR SQRT FUNCTION MUST BE > 0"); break;
			case  32: Sure.print("ARGUMENT TO EXP FUNCTION MUST BE < "+lnmaxreal); break;
			case  33: Sure.print("SUB-EXPRESSION TOO LARGE, i.e. > "+maxreal); break;
			case  34 : Sure.print("ARGUMENT TO STANDARD FUNCTION MISSING"); break;
			case  35 : Sure.print("( expected"); break;
			case  36 : Sure.print("< EXPECTED"); break;
			case  37 : Sure.print("NO SUCH RUN VALUE"); break;
			case  38 : Sure.print(" TO <filename> or ; EXPECTED"); break;
			case  39 : Sure.print(" ILLEGAL SPECIFICATION OF REAL "); break;
			case  40: Sure.print(" ILLEGAL SPECIFICATION OF INTEGER"); break;
			case  41 : Sure.print(" ILLEGAL SPECIFICATION OF PRUNESTATES"); break;
			case  42: Sure.print(" SUM OF PROBABILITIES MUST BE 1"); break;
			case  43 : Sure.print(": EXPECTED"); break;
			case  44 : Sure.print("INITIAL_PROBS parameter must be a constant"); break;
			case   45: Sure.print("INVALID ARGUMENT TO GAMMA FUNCTION."); break;
			case   46: Sure.print("COMBINATORIAL MAGNITUDE ERROR."); break;
			case   47: Sure.print("GAMMA MAGNITUDE ERROR."); break;
			case   48: Sure.print("CANNOT RAISE NEGATIVE NUMBER TO REAL POWER."); break;
			case   49: Sure.print("CANNOT RE-DEFINE RESERVED WORD."); break;
			case   50: Sure.print("CANNOT RE-DEFINE STANDARD FUNCTION."); break;
			case   51: Sure.print("CANNOT READ FILE.  PERMISSION DENIED."); break;
			case   52: Sure.print("CANNOT Sure.print FILE.  PERMISSION DENIED."); break;
			case   53 : Sure.print("NOT IMPLEMENTED"); break; 
			case   60: Sure.print("0 cannot be used as a state number in LEE mode"); break;
			case   61: Sure.print("TRUNC cannot be negative!"); break;
			default: Sure.print(""+e);
			} /* switch e */
			if (! (e == 46 || e == 47)) Sure.println(" ("+e+")");
	      } /* if inmode != scratch */
//	      else
//	         begin 
//	         erun = true;
//	         if e in [8,23,31,32,33,40,42,45,46,47,48,49,50,51,52] then begin
//	            Sure.print(' ':41,' *** ERROR - ');
//	            case e of
//	                8: Sure.println('IDENTIFIER NOT DEFINED');
//		       23: Sure.println('DIVISION BY ZERO');
//		       31: Sure.println('ILLEGAL LN OR SQRT ARGUMENT');
//		       32: Sure.println('EXP FUNCTION OVERFLOW');
//		       33: Sure.println('EXPRESSION OVERFLOW');
//	               40: Sure.println(' ILLEGAL SPECIFICATION OF INTEGER ');
//	               42: Sure.println(' SUM OF PROBABILITIES MUST BE 1');
//		       45: Sure.println('INVALID ARGUMENT TO GAMMA FUNCTION.');
//		       46: Sure.println('COMBINATORIAL MAGNITUDE ERROR.');
//		       47: Sure.println('GAMMA MAGNITUDE ERROR.');
//		       48: Sure.println('CANNOT RAISE NEGATIVE NUMBER TO REAL POWER.');
//		       49: Sure.println('CANNOT RE-DEFINE RESERVED WORD.');
//		       50: Sure.println('CANNOT RE-DEFINE STANDARD FUNCTION.');
//	               51: Sure.println('CANNOT READ FILE.  PERMISSION DENIED.');
//	               52: Sure.println('CANNOT Sure.print FILE.  PERMISSION DENIED.');
//	               /*otherwise ;*/
//	               end; /* case e*/
//	            end
//	         else
//	            Sure.println(' <<<< ERROR ',e:1,': should not occur in scratch mode >>>');
//	         if echo >= 2 then 
//	            Sure.println(' --- SCRATCH MODE ERROR # ',e:1,' ---');
//	         end;
//	      
	   } /* error */   


	   static void prval(ValuRec v) {	   
	      if (specid.equals("")) 
	    	  Sure.print(""+v.base);
	      else if (v.linear)
		  Sure.print(f.Em5(v.base)+" + "+f.Em5(v.coef)+'*'+specid);
	      else
	         Sure.print(" NONLINEAR F(VARIABLE)'");
	   } /* prval */

	   
	   static void printHashTable() {   // debug
		   f.p(" >>>> hashtable = ");
		   Set<String> ks = hashtable.keySet();
		   for (String s: ks) {
			   Idrec idinfo = hashtable.get(s);
               if (idinfo.rw == rwtype.rwnull) {
            	   f.p("  "+s+" = "+idinfo);
               }
		   }
		   f.pln("");
	   }

	   static void enterid(String id, ValuRec val, rwtype rwenter) {
		   //printHashTable();
		   //ValuRec val2 = new ValuRec(val.coef, val.base, val.linear);
		   Idrec idr = new Idrec(id.toUpperCase(),val,rwenter);
		   hashtable.put(id,idr);
		   //if (id.equals("DC")) // T184
		   //   f.pln(" $$$$$$>>> enterid put id = "+id+" idr = "+idr);
		   if (echo >= 2) {
			   Sure.println(" "+id+" ENTERED = "+val); // prval(val);
		   }
		   //printHashTable();
	   } /* enterid */
	   
	   
	   static void enterid(String id, ValuRec val) {
		   enterid(id,val,rwtype.rwnull);
	   } /* enterid */

		
	   static ValuRec searchid(String id) {
		   Idrec idinfo = hashtable.get(id.toUpperCase());
		   //if (id.equals("DC")) 
		   //f.pln(" ^^^^^^^^^^^^^^^^^^^ searchid for id = "+id+" idinfo = "+idinfo);
		   if (idinfo == null) {
			   rw = rwtype.rwnull;
			   return null;
		   } else {
			   rw = idinfo.rw;               // $$RWB: should I change this side effect?
			   //f.pln(" ^^^^^^^^^^^^^^^^^^^ idinfo.val = "+idinfo.val);
			   return new ValuRec(idinfo.val);    // return copy, see pure2.mod for example of failure without this
		   }
	   } /* searchid */

	   static double getdef(String id, double def) {
		      ValuRec v = searchid(id);
		      //f.pln(" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% getdef: v = "+v);
		      if (v != null) {
			     return v.base;
		      } else {
			     return def; 
		      }
		   } /* getdef */

	   
	   static void initsymtable() {
		   enterid("TO",val0,rwtype.rwto);
		   enterid("RUN",val0,rwtype.rwrun);
		   enterid("EXIT",val0,rwtype.rwexit);
		   enterid("DEBUG",val0,rwtype.rwdebug);
		   enterid("SHOW",val0,rwtype.rwshow);
		   enterid("READ",val0,rwtype.rwread);
		   enterid("HELP",val0,rwtype.rwhelp);
		   enterid("INPUT",val0,rwtype.rwinput);
		   enterid("CALC",val0,rwtype.rwcalc);
		   enterid("LEE",val0,rwtype.rwlee);
		   enterid("CLEAR",val0,rwtype.rwclr);
		   enterid("BY",val0,rwtype.rwby);
		   enterid("PLOT",val0,rwtype.rwplot);
		   enterid("IF",val0,rwtype.rwif);
		   enterid("THEN",val0,rwtype.rwthen);
		   enterid("ORPROB",val0,rwtype.rworprob);
		   enterid("FAST",val0,rwtype.rwfast);
		   enterid("INITIAL_PROB",val0,rwtype.rwinit); /* grep for this */
		   enterid("INITIAL_PROBS",val0,rwtype.rwinit); /* grep for this */
		   enterid("PRUNESTATES",val0,rwtype.rwprune);
		   enterid("READ0",val0,rwtype.rwread0);
		   enterid("LINEAR",val0,rwtype.rwlinear);
	   } /* initsymtable */


	   static boolean emptyprune() {
		   return asprunlist.size() == 0;
	      //return (asprunlist == nil);
	   }

	   static void clearprune() {
		   asprunlist.clear();
	   } /* clearprune */

	   static void enterprune(int i) {
	      //if (asprunlist.size() == 0) Sure.print("    PRUNE STATES ARE: ");
	      //else Sure.print(",");
	      //Sure.print(""+i);
	      asprunlist.add(i);
	   }


	   static boolean isprunest(int j) {
		   //f.p(" $$$ isprunest: j = "+j);
		   boolean rtn = false;
		   for (int i = 0; i < asprunlist.size(); i++) {
			   int st = asprunlist.get(i);
			   if (j == st) rtn = true;
		   }
		   //f.pln(" return "+rtn);
		   return rtn;
	   } /* isprunest */


	   
	   static void initLexical() {
		   ch = ' ';
		   chcnt = 0; 
		   insz = 0;
		   rw = rwtype.rwnull;
		   Front.eofFlag = false;
	   }
	   
	   static void initrun() {
		   Front.scrFile.clear();		   
		   geometric = false; 
		   prune_d = Sure.probNearZero; 
		   firstst = -1; 
		   nonlinear = false; 
		   forcelinear = false;
		   echo = 1; 
		   esyn1 = false; 
		   erun = false;
		   byfact = 0.0; 
		   byinc = 0.0; 
		   specid = ""; 
		   disp = false;   
		   rectype = 0; // white; 
		   calclast = false; 
		   clearprune();
		   initflag = false;
		   //f.pln(" initrun: $$$$$$$$$$$$$$$$$$$ rw = "+rw);
		   //qtcalc = 0; 
		   //start = 1;
		   //ptrunc = 3; 
		   //warndig = 2;
		   //pltsaved = false; 
	   } /* initrun */


	
	  private static boolean isDigit(char ch) {
		  return (ch >= '0' && ch <= '9');
	  }
	  
	
	  private static boolean isLetter(char ch) {
		  return (ch >= 'A' && ch <= 'Z') || ch == '_';
	  }

  static  void gettoken() {
	   /* -----------------------------------------------------------------

	                           lexical scanner

	     input:
	        reads commands from input file - either terminal or "read" file

	     output:
	        tkn -- next token found 
	        id  -- string of characters found if tkn = tkid
	        int -- integer value if tkn = tkint
	        val -- real value if tkn = tkreal   
	        rw -- reserved word type if tk = tkrw

	     note: ----- machine dependency ----- this routine uses the "c"
	     routine "readv" to convert a string of characters representing a number 
	     to the machine's internal floating point representation.  this routine 
	     may have to be "programmed" for another type of system.        
	     modified 1-7-88 to test if case more efficient by phs

	   ----------------------------------------------------------------- */

	  int   ival, jval;
	  String numbuf;                     
	  boolean  realflag; 
	  tkn = tokens.tknull;
	  //f.pln(" $$ ============== gettoken: ch = "+ch+" tkn = "+tkn);
	  while  (tkn == tokens.tknull)  {
		  while (ch == ' ' || ch == tab) { readch();}
		  //f.pln(" $$ ============ gettoken1 ch = "+ch+" tkn = "+tkn);
		  if (isLetter(ch)) {
			  tkn = tokens.tkid; 
			  int i = 0; 
			  id = "";
			  while (isLetter(ch) || isDigit(ch)) {
				  i = i + 1;
				  id = id+ch;
				  readch();
			  }
			  if (searchid(id) != null) {              /* side effect sets rw */
				  if (rw == rwtype.rwnull) tkn = tokens.tkid;
				  else tkn = tokens.tkrw;            
				  if (id.equals("MOD")) tkn = tokens.tkmod;
				  if (id.equals("DIV")) tkn = tokens.tkquo;
				  if (id.equals("CYC")) tkn = tokens.tkcyc;
			  }
		  } else if (isDigit(ch) || ch == '.') {
			  //	         '0','1','2','3','4','5','6','7','8','9','.' :
			  //	            begin
			  //		       /* for i = 1 to numleng do numbuf[i] = ' '; */
			  numbuf = ""; 
			  realflag = false;	  			  
			  if (ch == '.') { 
				  realflag = true; 
				  numbuf = "0";
			  } else {
				  realflag = false;
			  }
			  while (isDigit(ch)) { 
				  numbuf = numbuf + ch;
				  readch();
			  }
			  if (ch == '.') { 
				  realflag = true;
				  numbuf = numbuf + ch;
				  readch();
				  while (isDigit(ch)) { 
					  numbuf = numbuf + ch;
					  readch();
				  } /* while */
			  }/* if (ch = '.' */
			  if (ch == 'D' || ch == 'E')  {
				  realflag = true; 
				  numbuf = numbuf + "E";
				  readch();
				  if (ch == '+' || ch =='-') { 
					  numbuf = numbuf + ch;
					  readch();
				  } /* if (ch in ['+'...*/
				  while (isDigit(ch)) { /* get exponent */
					  numbuf = numbuf + ch;
					  readch();
				  } /* while */
				  //    numbuf[numleng] = '$';
			  } /* if ch in ['d'... */
			  if (realflag) {
				  tkn = tokens.tkreal;
				  //val = readvreal(numbuf,ierrf);			   
				  try {
					  val = Double.parseDouble(numbuf);
				  }
				  catch (NumberFormatException x) {
					  //f.pln(" gettoken: error(39): numbuf = "+numbuf);
					  error(39);
				  }
			  } else {
				  tkn = tokens.tkint;
				  //int = readvint(numbuf,ierrf);
				  try {
					  intv = Integer.parseInt(numbuf);
				  }
				  catch (NumberFormatException x) {
					  //f.pln(" gettoken: error(40): numbuf = "+numbuf);
					  error( 40 );
				  }
			  }
		  } else if (ch == ',') {
			  tkn = tokens.tkcomma;
			  readch();
		  } else if (ch == '=') {
			  tkn = tokens.tkeq;
			  readch();
		  } else if (ch == '+') {
			  tkn = tokens.tkadd;
			  readch();
		  } else if (ch == '-') {
			  tkn = tokens.tksub;
			  readch();
		  } else if (ch == '@') {
			  tkn = tokens.tkat;
			  readch();
		  } else if (ch == ':') {
			  tkn = tokens.tkcol;
			  readch();
		  } else if (ch == '*') {
			  readch();
			  if (ch == '*') {
				  tkn = tokens.tkpow;
				  readch();	               
			  } else {
				  tkn = tokens.tkmul;
			  }
			  //f.pln(" ?????????????????????????//// tkn = "+tkn);
		  } else if (ch == '^') {
			  tkn = tokens.tkpow;
			  readch();
		  } else if (ch == '/') {
			  tkn = tokens.tkdiv;
			  readch();
		  } else if (ch == '<') {
			  if (inbuf.charAt(chcnt+1) == '=') {
				  readch(); tkn = tokens.tkle;
			  } else {
				  tkn = tokens.tklt;
			  }
			  readch();
		  } else if (ch == '>') {
			  //f.pln(" @@@@@@@@@@@ inbuf = "+inbuf+" ch = "+ch+" inbuf.length() = "+inbuf.length()+" chcnt = "+chcnt);
			  if (inbuf.charAt(chcnt+1) == '=') {
				  readch(); tkn = tokens.tkge;
			  }
			  else tkn = tokens.tkgt;
			  readch();
		  } else if (ch == '(') {
			  //f.pln("  ENTER COMMENT ----------------------- ch = "+ch);
			  readch();
			  if (ch == '*') {
				  do 
					  readch();
				  while  (! (ch == '*' && inbuf.charAt(chcnt+1) == ')') );
				  readch();  // skip past '*'
				  readch();  // skip past ')'
				  tkn = tokens.tknull;            // don't exit loop,
				  //f.pln(" $$ gettoken: EXIT COMMENT VIA BREAK 1!");
			  } else {
				  tkn = tokens.tklp;
			  }
		  } else if (ch == '[') {
			  tkn = tokens.tklb;
			  readch();
		  } else if (ch == ']') {
			  tkn = tokens.tkrb;
			  readch();
		  } else if (ch == ')') {
			  tkn = tokens.tkrp;
			  readch();
		  } else if (ch == '{') {
			  do readch(); 
			  while (ch != '}');
			  readch();
			  //f.pln(" $$ gettoken: HIT BREAK 2!");
			  tkn = tokens.tknull;                // don't exit loop
		  } else if (ch == '#') { 
			  boolean uflag = true;
			  readch(); 
			  if (ch == 'U' || ch == 'L') {
				  if (ch == 'L') uflag = false;
				  readch();
			  }
			  ival = 0;
			  while (isDigit(ch)) {
				  ival = ival*10 + (int) (ch)- (int) ('0');
			  }
			  readch();
			  if (ival == 0) error(3); 
			  jval = 1;
			  if (ch == '[') {
				  readch();
				  jval = 0;
				  while (isDigit(ch)) {
					  { jval = jval*10 + (int) (ch)- (int) ('0');
					  readch();
					  }
					  if (ch != ']') error(26);
					  else readch();
				  }
				  val = -1.0;
				  if ((jval <= nvpr) && (jval <= maxvpr)) {
		             Sure.println(" gettoken !!!!!!!!!!!!!! NOT IMPLEMENTED YET !!!!!!!!!!!!!!!!!!!!");				  
//					  rp = runvals[jval];
//					  while (rp != nil) {
//						  if (rp^.runno = ival) {
//							  tkn = tokens.tkreal;
//							  if (uflag)val = rp^.ub;
//							  else val = rp^.lb;
//							  goto 2;
//						  }
//						  rp = rp^.next;
//					  } /* while */
				  }/* if jval */
			  }
		  } else if (ch == cheoc) {
			  tkn = tokens.tksemi;
			  readch();
		  } else if (ch == cheol) {
			  tkn = tokens.tksemi;
			  readch();
		  } else if (ch == cheof) {
			  //f.pln(" $$$$$$$ gettoken: ch == cheof, inmode = "+inmode);
			  tkn = tokens.tkrw;
			  rw = rwtype.rwexit;
		  } else { 
			  Sure.println(" $$$$  HIT ELSE CLAUSE OF gettoken(): ch = "+ch);
			  error(16); 
			  readch();
			  tkn = tokens.tkerr;
		  }
//		  if (tkn == tokens.tkint) f.pln(" = "+intv);
//		  else if (tkn == tokens.tkreal) f.pln(" = "+val);
//		  else if (tkn == tokens.tkid) f.pln("  = "+id);
//		  else f.pln("");
//		  f.pln("  ch = "+ch+" chcnt = "+chcnt+" chcnt = "+chcnt);
	  } // while tkn == tknull
//	  /f.pln("  ======================== EXIT gettoken: ch = "+ch+" tkn = "+tkn);
  }/* gettoken() */

  /* ------------------- parser routines --------------------- */

  static functypes findfunc(String id) {	  
	   return functable.get(id);
  }

  
  
  static void etrans(int i1, int i2, trantype tt,  ValuRec v1, ValuRec v2, ValuRec v3) {
	  NumRec xv1 = new NumRec(v1.base,v1.coef);
	  NumRec xv2 = new NumRec(v2.base,v2.coef);
	  NumRec xv3 = new NumRec(v3.base,v3.coef);
	  if (!(v1.linear && v2.linear && v3.linear)) {
		  nonlinear = true;
	  }
	  Sure.enter(i1,i2,tt,xv1,xv2,xv3);
  } /* etrans */



  static void enterfunc(String id, functypes fnty) {
     if ( echo >= 2 ) Sure.println(" function "+id+" ENTERED "); 
	   functable.put(id,fnty);
    if (echo >= 2) {
       Sure.println(" "+id+" ENTERED = "+val); // prval(val);
    }

  }; /* enterfunc */

  static void funktion(functypes functype, tokens lasttkn) {
	  //var 
	  //   fres: double;
	  //   ierflag: integer;
	  //
	  //{
	  double fres = 0.0;
	  //f.pln(" in funktion lasttkn = "+lasttkn);
	  gettoken();
	  if (tkn == tokens.tklp) { 
		  lasttkn = tkn; 
		  gettoken();
		  if ( ! inexprset(tkn) ) error(6);
		  expression(); 
		  if (gval.linear)
			  if (gval.coef == 0.0) {
				  if (functype == functypes.fabs) {
					  if ( gval.base < 0 )
						  fres = -gval.base;
					  else fres = gval.base;
				  } else if (functype == functypes.fexp) {
					  if (gval.base > lnmaxreal) error(32);
					  else if ( gval.base <= lnminreal)
						  fres = 0.0;
					  else fres = Math.exp(gval.base);
				  } else  if (functype == functypes.fln) {
					  if ( gval.base > 0 )  fres = Math.log(gval.base);
					  else error(31);
				  } else  if (functype == functypes.fsqrt) {
					  if ( gval.base >= 0 ) fres = Math.sqrt(gval.base);
					  else error(31);
				  } else  if (functype == functypes.fsin) {
					  fres = Math.sin(gval.base);
				  } else  if (functype == functypes.fcos) {
					  fres = Math.cos(gval.base);
				  } else  if (functype == functypes.farctan) {
					  fres = Math.atan(gval.base);
				  } else  if (functype == functypes.farcsin) { 
					  fres = Math.asin(gval.base);
				  } else  if (functype == functypes.farccos) { 
					  fres = Math.acos(gval.base); 

//            ffact: {
//                   WhichBuiltin = 'FACT';
//                   nval = CombinatorialRound(gval.base);
//	  fres = 0;
//	  if ( nval < 0 ) error(46)
//	  else fres = if (act(nval);
//                   };
//            functypes.fperm,functypes.fcomb: {
//                   WhichBuiltin = 'PERM';
//                   if ( functype = fcomb )
//                      WhichBuiltin = 'COMB';
//                   nval = CombinatorialRound(gval.base);
//                   if ( tkn == tokens.tkcomma )
//                     { gettoken(); 
//                     if ( ! (tkn in exprset) ) error(6)
//                     else
//                       {
//                       expression();
//                       if ( gval.linear )
//                         if ( gval.coef != 0.0 )
//                           gval.linear = false
//                         else 
//                           {
//                           mval=CombinatorialRound(gval.base);
//		  fres = 0;
//		  if ( (nval < 0) OR (mval < 0) OR (mval > nval) )
//			error(46)
//		  else if ( functype = fperm )
//                              fres = iperm(nval,mval)
//                           else
//                              fres = icomb(nval,mval);
//                           };
//	       }
//                     }
//                   else error(4);
//                   };
//            fgamma: { 
//                   ierflag = cgamma(gval.base,fres);
//                   if ( (ierflag != 0) ) error(45);
//                   };
				  }; /* case */
				  gval.base = fres;
			  }  else
				  gval.linear = false;
		  if ( tkn != tokens.tkrp ) error(21);
	  } else error(34);
  } /* funktion */

   static void constant() {
	   tokens lasttkn = tokens.tknull;
	   functypes functype;
	   gval = new ValuRec(0.0,0.0,true);
	   if ( tkn == tokens.tkint )  gval.base = intv;
	   else if ( tkn == tokens.tkreal ) gval.base = val;
	   else if ( tkn == tokens.tklp || tkn == tokens.tklb) { 
		   lasttkn = tkn; 
		   gettoken(); 
		   expression(); 
		   if ( lasttkn == tokens.tklp) { 
			   if ( tkn != tokens.tkrp ) error(21); 
		   } else {
			   if ( tkn != tokens.tkrb ) error(26);
		   }
	   } else if ( tkn == tokens.tkid )  { 
		   functype = findfunc(id);
		   /*                Sure.println(' FUNCTION = ',functype); */
		   if ( functype != null) funktion(functype,lasttkn);
		   else { 
			   if ( id == specid && inmode != inmodes.scratch ) 
				   gval.coef = 1.0;
			   else {
				   gval = searchid(id);
				   //f.pln("$$$$$ in constant SEARCHID for id = "+id+" gval = "+gval);
				   if (gval == null ) {
					   error(8);
				       gval = new ValuRec(0.0,0.0,true);   // prevent exceptions later! (ERRORS PRESENT SO ANSWER IS MEANINGLESS)
				   }
			   }
		   };
	   }
	   else error(15);   
	   if ( tkn != tokens.tksemi ) gettoken();
	   //f.pln(" $$$$ exit constant tkn = "+tkn+" gval = "+gval);
   } /* constant */


   static void testpow(double v1, double v2) {
	   /* test v1**v2; ------ notice side-effect ----- */
//	   if ( v1 <= minreal ) { 
//		   error(48); 
//		   v1 = 1.0; 
//	   } else if ( v2*Math.log(v1) > lnmaxreal ) { 
//		   error(33);
//		   v2 = 1.0;
//	   } 
   }  /* testpow */

static void term() {
    constant(); 
    ValuRec lval = gval; 
    while (tkn == tokens.tkpow) { 
    	gettoken(); 
    	term();
    	lval.linear = gval.linear && lval.linear;
    	if ( lval.linear ) {
    		if ( (lval.coef == 0.0) && (gval.coef == 0.0) ) {    /* const ** const */
    			testpow(lval.base,gval.base);
    			if ( lval.base != 0 )
    				lval.base = Math.pow(lval.base,gval.base);
    		} else {
    		   lval.linear = false; /* nonlinear = true;*/
    		};
    	}; /* if ( ! lval.linear */
//    if ( dbgparse in debugf )
//	  Sure.println(' IN TERMLOOP: LVAL = ',lval.base:20,lval.coef:20);
    };/* while */ 
    gval = lval; 
// if ( dbgparse in debugf ) with gval do
    //f.pln(" .................%%%%%%%%%%%%%%%% TERM: gval = "+gval);
 }; /* term */

//  int CombinatorialRound(double x) {
// label 38,39;
// var ans:integer;
// {
//    if ( abs(x) > maxint )
//       {
//       if ( (x>0.0) ) ans = maxint else ans = -maxint;
//       goto 38;
//       }
//    else
//       {
//       ans = round(x);
//       if ( (abs(x-ans)>1.0E-3) ) goto 38;
//       };
//    goto 39;
//38:      error(40);
//    Sure.println('**** ARGUMENT TO ',WhichBuiltin,' ROUNDED FROM ',x:8:6,
//            ' TO ',ans,'.0000000000');
//39:      CombinatorialRound = ans;
//};

static void testmul(double v1, double v2) {
//var absv2:double;
//{ /* ---------- notice side-effect ------------ */
//  absv2 = abs(v2);
//  if ( (absv2>1.0) )
//     if ( (abs(v1)>maxreal/absv2) ) /* abs(v1*v2) > maxreal */
//         { error(33); v1 = 1.0; v2 = 1.0;
//         };
}; /* testmul */

static void testdiv(double v1, double v2) {
//var absv2:double;
//{ /* ---------- notice side-effect ------------ */
//  absv2 = abs(v2);
//  if ( (absv2 <= minreal) ) /* divide by 0.0 */
//     { error(23); v2 = 1.0; }
//  else if ( (absv2<1.0) )
//     if ( (abs(v1) > maxreal*absv2) ) /* abs(v1/v2) > maxreal */
//        { error(33); v1 = 1.0; v2 = 1.0;
//        };
} /* testdiv */

static void factor() {
	tokens stkn;
	ValuRec lval;
	boolean negflag;
	//nval,mval: integer;
	String WhichBuiltin;

	if ( tkn == tokens.tksub ) { 
		negflag = true; gettoken();
	} else negflag = false;

	term();
	//f.pln(" ******* START factor tkn = "+tkn+" gval = "+gval);
	if ( negflag ) { 
		lval = new ValuRec(-gval.coef,-gval.base, gval.linear);
	} else
		lval = gval;    
	//f.pln(" >>>>> in factor0 while LOOP: tkn = "+tkn+" lval = "+lval+" gval = "+gval);   	   
	while (tkn == tokens.tkmul || tkn == tokens.tkdiv) { //  || tkn == tokens.tkquo || tkn == tokens.tkmod || tkn == tokens.tkcyc) {
		stkn = tkn; 
		gettoken();
		term();       /*  factor <- lval op gval  */
		//    	if (tkn == tokens.tkdiv)
		//f.pln(" >........ in factor1 while LOOP: tkn = "+tkn+" lval = "+lval+" gvl = "+gval);   	   

		lval.linear = lval.linear && gval.linear;
		if ( lval.linear )  {
			if ( lval.coef == 0.0 ) {
				if ( gval.coef == 0.0 )  { /* ----- const op const ----- */
					//f.pln(" @@@@@@@@@@@@@ in factor:  stkn = "+stkn); 
					if (stkn == tokens.tkmul ) {
						testmul(lval.base,gval.base);
						lval.base = gval.base * lval.base;
					} else {
						testdiv(lval.base,gval.base);
						//    					if ( stkn == tokens.tkquo ) {
						//    						WhichBuiltin = "DIV";
						//    						lval.base =  CombinatorialRound(lval.base) div
						//    								CombinatorialRound(gval.base)
						//    					}
						//    					else if ( stkn == tokens.tkmod ) {
						//    						WhichBuiltin = "MOD";
						//    						lval.base =  CombinatorialRound(lval.base) mod
						//    								CombinatorialRound(gval.base)
						//    					}
						//    					else if ( stkn == tokens.tkcyc ){
						//    						WhichBuiltin = "CYC";
						//    						lval.base =  1 +
						//    								(CombinatorialRound(lval.base) - 1)
						//    								mod
						//    								CombinatorialRound(gval.base)
						//    					}
						//    					else
						//f.pln(" ###### factor  tokens.tkdiv(0,0): "+lval.base+" / "+gval.base);
						lval.base =  lval.base / gval.base;
					}
				} else {  /* ----- const op var ----- */
					if ( stkn == tokens.tkmul ) {
						testmul(gval.coef,lval.base);
						lval.coef = gval.coef * lval.base;
						testmul(gval.base,lval.base);
						lval.base = gval.base * lval.base;
					} else { /* if ( stkn == tokens.tkdiv or tkmod or tkquo or tkcyc ) */
						lval.linear = false;
						/* nonlinear = true;*/
					}
				}
			} else if ( gval.coef ==  0.0 )  {  /*  ----- var op const ----- */
				if ( stkn == tokens.tkmul )  {
					testmul(lval.base,gval.base);
					lval.base = lval.base * gval.base;
					testmul(lval.coef,gval.base);
					lval.coef = lval.coef * gval.base;
				} else if ( stkn == tokens.tkdiv ) {
					testdiv(lval.base,gval.base);
					testdiv(lval.coef,gval.base);
					lval.base = lval.base / gval.base;
					lval.coef = lval.coef / gval.base;
					//f.pln(" ###### factor ----- var op const ----- tokens.tkdiv: lval = "+lval);
				} else {
					lval.linear = false;
					/* nonlinear = true;*/
				}
			} else  {/* ----- var op var ----- */
				lval.linear = false; 
			}
		} /* if ( lval.linear */
	} /* while * or / */
	gval = lval; 
	//f.pln(" END FACTOR, gval = "+gval);
} /* factor */

static void testadd(double v1, double v2) {
  /* test for ((v1+v2) > maxreal)  or ((v1+v2) < -maxreal) */
  if ( (v1 > 0.0) )
     if ( (v2 > (Double.MAX_VALUE-v1)) ) error(33);
  if ( (v1 < 0.0) )
     if ( (v2 < (-Double.MAX_VALUE-v1)) ) error(33);
} /* testadd */

   
static void expression() {
	tokens ltkn;
	ValuRec lval;
	//WhichBuiltin:packed array[1..4] of char;
	factor(); 
	lval = gval;
	while (tkn == tokens.tkadd || tkn == tokens.tksub) {
		ltkn = tkn; gettoken();
		factor();
		lval.linear = lval.linear && gval.linear;
		if ( ltkn == tokens.tkadd ) {
			testadd( lval.coef, gval.coef );
			lval.coef = lval.coef + gval.coef;
			testadd( lval.base, gval.base );
			lval.base = lval.base + gval.base;
		} else {
			testadd( lval.coef, -gval.coef );
			lval.coef = lval.coef - gval.coef;
			testadd( lval.base, -gval.base );
			lval.base = lval.base - gval.base;
		};
		//        if ( dbgparse in debugf )
			//           Sure.println(' IN EXPRESSION LOOP: LVAL = ',lval.base:20,lval.coef:20);
	};/* while addition or subtraction */
	gval = lval; 
} /* expression */

//
static void skip () {
	while (tkn != tokens.tksemi) {
		if ( ch == cheoc ) tkn = tokens.tksemi;
		readch(); /* gettoken reads 1 char ahead */
	}
} /* skip */

static void testandskip () {
   if (tkn == tokens.tkrw && rw == rwtype.rwexit) return;
   if ( errflag ) skip();
   if ( tkn != tokens.tksemi ) error(7); 
} /* testandskip */



   static boolean inexprset(tokens tkn)    {    
	   return (tkn == tokens.tkreal || tkn == tokens.tkint || tkn == tokens.tkid || tkn == tokens.tklp 
			                        || tkn == tokens.tklb ||tkn == tokens.tksub);
   }

   static void parsecondef() {
	   //f.pln(" parsecondef: tkn = "+tkn);
	   String newid = ""; 
	   ValuRec lval;
	   ValuRec byval;
	   double rv1 =  0.0;
	   double	rv2 = 0.0;
	   //f.pln(" in parsecondef: id = "+id+" tkn = "+tkn+" rw = "+rw);
	   if (tkn != tokens.tkid) error(9);
	   else {
		   if (rw != rwtype.rwnull) error(49);
		   if (findfunc(id) != null) error(50);
		   newid = id; 
		   gettoken();
		   if (tkn != tokens.tkeq) error(5);
		   else {
			   gettoken();
			   //f.pln(" $$$$$ in parsecondef2: id = "+id+" tkn = "+tkn+" rw = "+rw);
			   if (!inexprset(tkn)) error(6);
			   else {
				   expression(); 
				   lval = gval;
				   if (tkn == tokens.tksemi && !errflag) {
					   //f.pln(" $$$$$$$$$$ parsecondef4: enterid!    id = "+id+" tkn = "+tkn+" rw = "+rw+" specid = "+specid);
					   enterid(newid,lval,rwtype.rwnull);
					   if (newid.equals("ECHO")) {
						   echo = (int) lval.base;
						   //f.pln(" $$$$$ in parsecondef echo = "+echo);
					   } else if (newid.equals(specid)) {
						   specid = "";
						   if (inmode != inmodes.scratch) {
							   Sure.println(" *** WARNING: VARIABLE CHANGED TO A CONSTANT! PREVIOUS TRANSITIONS MAY BE WRONG"); 
						   }
					   }
				   }  else if (rw == rwtype.rwto) {  // This will be the special variable
      				   //f.pln(" $$$$$$ in parsecondef4: id = "+id+" tkn = "+tkn+" rw = "+rw);
					   gettoken(); 
					   geometric = false;
 					   if (tkn == tokens.tkadd || tkn == tokens.tkmul) {
						   if (tkn == tokens.tkadd) geometric = false;
						   else geometric = true;
						   gettoken();
					   }
     				   //f.pln(" $$$$$$ in parsecondef5: id = "+id+" tkn = "+tkn+" rw = "+rw+" geometric = "+geometric);
					   if (!inexprset(tkn)) error(6);
					   else {
						   expression(); 
						   //f.pln(" $$$$$$ in parsecondef6: id = "+id+" tkn = "+tkn+" rw = "+rw);
						   if (lval.coef != 0.0) error(25);
						   else rv1 = lval.base;
						   if (gval.coef != 0.0) error(25);
						   else rv2 = gval.base;
						   byinc = 0.0; byfact = 0.0;
						   //f.pln(" $$$$$$ in parsecondef7: id = "+id+" tkn = "+tkn+" rw = "+rw);
						   if (rw == rwtype.rwby) {
							   gettoken(); 
							   expression(); 
							   byval = gval;
							   if (byval.coef == 0) {
								   if (geometric) {
									   byfact = byval.base;
									   if (rv1 < rv2) {
										   if (byfact < 1.0)error(22);
									   }
									   else {
										   if (byfact > 1.0) error(22);
									   }
									   if (byfact == 1.0) error(22);
								   } else {
									   byinc = byval.base;
									   if (rv1 < rv2) {
										   if (byinc < 0.0) error(22); 
									   } else {
										   if (byinc > 0.0) error(22); 
									   }
									   if (byinc == 0.0) error(22);
								   }
								   if (!errflag) numpoints = 0;    
							   }  else error(22);
						   } /* if rw = rwby */
						   if ((tkn == tokens.tksemi) && ! errflag )  {
							   if ((inmode == inmodes.scratch) ) { /* --- in scratch mode --- */
								   enterid(specid,scrval,rwtype.rwnull);
							   } else {
								   if (specid == newid ) {   // *** redefining the special variable
									   speclow = rv1; spechigh = rv2;
									   if ((echo != 0) && (inmode != inmodes.scratch) )
										   Sure.println("     "+newid+" CHANGED TO "+rv1+" TO "+rv2);
								   } else if (specid == "" ) { 
									   lval.base = 0.0; lval.coef = 1.0;
									   lval.linear = true;
									   specid = newid;
									   enterid(newid,lval,rwtype.rwnull);
									   //f.pln(" $$$$$ parsecondef: ENTER newid = "+newid+" lval = "+lval);
									   speclow = rv1; 
									   spechigh = rv2;
								   }
								   else error(14) ;
							   }
								  
						   }
					   }
				   }
			   }
		   }    
	   }
	  // f.pln(" $$$$ PARSECONDEF EXIT "+newid+" "+rv1+" tkn = "+tkn+" ch = "+ch);
   } /* parsecondef */


   static void parseclear()   {
	   gettoken();
	   if (tkn == tokens.tkmul) {
		   gettoken();
		   while (tkn == tokens.tkint) {
			   /* clrnode(int); */
			   gettoken();   
			   if (tkn == tokens.tkcomma) gettoken();
		   }  /* while */
	   }
	   testandskip();
	   Sure.clear();
	   //initrun(); 
   }; /* parseclear */


   static void parsedebug () {
	   gettoken();
	   if (tkn == tokens.tkid) {
		   Sure.println(" DEBUG OPTIONS = "+id);
		   gettoken(); /* skip past ; */
	   }
	   else error(9);
   } /* parsedebug */

   static void parsefname() {
	   String fname = getfname("run");

	   if (fname.length() > 0) {
		   Sure.println(" !!!!!!!!!!!!!!! parsefname NOT COMPLETELY IMPLEMENTED !!!!!!!!!!!!!!!!!!!!! ");
	   }
	   //f.pln("\n $$$$$$$$$$$$$$$$$$ parsefname !!!!!!!!!! ch = "+ch+" tkn = "+tkn);	   
	   skip();
   }; /* parsefname */
   

//   static void parsehelp;
///* var ch: char; */
//   {
//      gettoken; /* skip past ; */
///*    reset(helpfile);
//      while not eof(helpfile) do
//         {
//         while not eoln(helpfile) do
//            { read(helpfile,ch); Sure.print(ch);
//            };
//         readln(helpfile); Sure.println("");
//         };
//      Sure.println("");
//*/  
// }; /* parsehelp */


   static void parseif () {
	   gettoken();
	   expression();
	   ValuRec lval = gval;
	   if (tkn == tokens.tklt || tkn == tokens.tkgt || tkn == tokens.tkeq || tkn == tokens.tkge || tkn == tokens.tkle) {
		   tokens ltkn = tkn; 
		   gettoken();
		   expression();
		   //f.pln(" parseif AFTER: tkn = "+tkn+"    lval = "+lval+"     gval = "+gval);
		   if ((tkn == tokens.tkrw) && (rw == rwtype.rwthen) ) {      
			   if ((lval.coef != 0) || (gval.coef != 0)) {
				   nonlinear = true; /* check for syntax errors */
				   gettoken(); 
				   //f.pln(" parseif 0: tkn = "+tkn);
				   parsestatement();
			   } else {
				   //case ltkn of
				   boolean optest = false;
				   if (ltkn == tokens.tklt) { 
					   optest = lval.base < gval.base;
				   } else if (ltkn == tokens. tkgt) { 
					   optest = lval.base > gval.base;
				   } else if (ltkn == tokens. tkle) { 
					   optest = lval.base <= gval.base;
				   } else if (ltkn == tokens. tkge) { 
					   optest = lval.base >= gval.base;
				   } else if (ltkn == tokens. tkeq) { 
					   optest = (lval.base == gval.base);
				   } /* case ltkn of */
				   //f.pln(" $$$$$$$$$$$$ parseif: optest = "+optest);
				   if (optest) {
					   gettoken(); 
					   //f.pln(" parseif 1: tkn = "+tkn);
					   parsestatement();
				   }  else {
					   skip(); 
				   }
			   }
		   } else error(36);
	   } else error(36);
   } /* parseif (*/

   
   static void quethelist(ArrayList<Initialrec> ilp) {
	   Sure.println(" $$>> quethelist NOT YET IMPLEMENTED!!!!!!!!!!!");
   } /* quethelist */


   static void dequethelist(ArrayList<Initialrec> ilp) {
	   Sure.println(" $$>> dequethelist NOT YET IMPLEMENTED!!!!!!!!!!!");
   } /* dequethelist */


   static void addtoinitlist(int st, NumRec pb1, NumRec pb2) {
	   Initialrec iiRec = new Initialrec(st,pb1,pb2);
	   initlist.add(iiRec);
   } /* addtoinitlist */


   static void parseinit() {
      int st;
      NumRec pb,pb1,pb2;   
      gettoken(); 
      //double sum = 0.0; 
      boolean exactpb = true;
      if (tkn == tokens.tklp) { 
    	  //addtoinitlist(-1,pbnull,pbnull);               ???????????????????? WHAT IS THIS FOR ?????????????????
    	  initflag = true;
    	  do {
    		  gettoken();
    		  if (tkn == tokens.tkint) {
    			  st = intv; 
    			  gettoken();
    			  if (tkn != tokens.tkcol)error(43);
    			  else gettoken();
    			  if (tkn == tokens.tklp || tkn == tokens.tklb) { 
    				  exactpb = false; 
    				  gettoken();
    				  expression();
    				  //pb1.base = gval.base;  pb1.coef = gval.coef;
    				  pb1 = new NumRec(gval.base,gval.coef);
    				  if (! gval.linear) nonlinear = true;

    				  if (tkn == tokens.tkcomma) {
    					  gettoken(); 
    					  expression();
    					  //pb2.base = gval.base;  pb2.coef = gval.coef;
    					  pb2 = new NumRec(gval.base,gval.coef);
    					  if (! gval.linear) nonlinear = true;

    					  if (tkn == tokens.tkrp || tkn == tokens.tkrb) {
    						  gettoken();
    						  if (tkn == tokens.tkcomma || tkn == tokens.tkrp) 
    							  addtoinitlist(st,pb1,pb2);
    						  else error(21);
    					  }
    					  else error(21);
    				  }
    				  else error(4);
    			  } else {
    				  expression(); 
    				  // pb.base = gval.base;  pb.coef = gval.coef;
    				  pb = new NumRec(gval.base,gval.coef);
    				  if (! gval.linear) nonlinear = true;
    				  if (tkn == tokens.tkcomma || tkn == tokens.tkrp) 
    					  addtoinitlist(st,pb,pb);
    				  else error(21);
    			  }
    		  }
    		  else error(3);
    	  } while ( ( !(tkn == tokens.tkrp) || (tkn == tokens.tksemi)));
    	  if (tkn == tokens.tkrp) { 
    		  gettoken();
    	  }
      }
      else error(35);
   }; /* parseinit */

   
	public static String getInteractiveInput(String headerLabel, String prompt) {
	    return JOptionPane.showInputDialog(null,prompt,headerLabel,JOptionPane.QUESTION_MESSAGE);
	}

	static String consoleRead() {
		String rtn = "";
		try{
			BufferedReader bufferRead = new BufferedReader(new InputStreamReader(System.in));
			rtn = bufferRead.readLine();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		return rtn;
	}
	
	static void parseinput() {
		////      /* ------ must be in read mode ------ */
		if (inmode == inmodes.interactive) {
			error(24);
		} else {
			gettoken();
			//int i = 0;
			while (tkn == tokens.tkid) {
				String lid = id; 
				String inp = ""; 
				if (Sure.gui_active) {
					inp = getInteractiveInput("Interactive Input", id+"?");
					f.pln(" $$$$$$$$$ parseinput: val = "+val);
				} else {
		    		Sure.println(id+"?");
		    		inp = consoleRead();
				}
				double val = Double.parseDouble(inp);
				ValuRec lval = new ValuRec(0.0,val,true);
				enterid(lid,lval,rwtype.rwnull);
		    	gettoken();
				if (tkn == tokens.tkcomma) gettoken();      
			} // while
		}
	} /* parseinput */

//
   static void  parseplot() {
	   f.pln(" $$$$ PLOT not yet implemented!");
	   testandskip();
//   const
//      plotnamelower = 'LowerBound.dat';
//      plotnameupper = 'UpperBound.dat';
//   var
//      plotlowerbound,plotupperbound: text;
//      plotload: text;
//      ii: integer;
//   begin
//      gettoken; 
//      Assign(plotload,'sure.gp');
//      rewrite(plotload);
//      Sure.println(plotload,'set ylabel ''Prob Failure'' ');
//
//      while tkn in [tkid,tkint] do  
//         begin 
//         if tkn = tkid then 
//            begin 
//            if id = 'XLOG        ' then
//               Sure.println(plotload,'set logscale x');
//            if id = 'YLOG        ' then
//               Sure.println(plotload,'set logscale y');  
//            if id = 'XYLOG       ' then
//               Sure.println(plotload,'set logscale xy');  
//            gettoken;                            
//            end
//         else error(9);
//         end; 
//
//      Sure.println(plotload,'plot ''',plotnamelower, ''' title "Lower Bound" with lines,',
//                           ' ''',plotnameupper, ''' title "Upper Bound" with lines');
//      Close(plotload);
//
//      Assign(plotlowerbound,plotnamelower);
//      Assign(plotupperbound,plotnameupper);
//      reSure.print(plotlowerbound);
//      reSure.print(plotupperbound);
//      for ii := 1 to plti do
//          begin
//          Sure.println(plotlowerbound,' ',pltx[ii],' ',plty[ii]);
//          Sure.println(plotupperbound,' ',pltx[ii],' ',pltz[ii]);
//          end;
//      Close(plotlowerbound);
//      Close(plotupperbound);
//
//      testandskip;
//      if not errflag then
//         begin
//         end;
   } /* parseplot */

   static String getfname(String def) {
	   //f.pln(" $$%%>>> getfname: fname ENTER! inbuf = "+inbuf);
	   while (chlast == ' ') { 
		   readch();
	   }
	   char lch;
	   boolean periodf = false; 
	   String fname = "";
	   while (! (chlast == ' '|| chlast == '*' || chlast == cheoc || chlast == cheol || chlast == cheof) ) {
		   lch = chlast; 
		   //f.pln(" $$%% getfname: chl = "+chl+" fname = "+fname);
		   readch();
		   if (lch != cheoc) {
			   fname = fname + lch;
		   }
		   if (chlast == '.'  && lch != '.' ) periodf = true;
	   }
	   if (fname.length() > 0 && !periodf )  {
		   fname = fname + "." +def;
	   }   
	   //f.pln(" $$%%>>> getfname: EXIT fname = "+fname);
	   tkn = tokens.tksemi;
	   return fname;
   } /* getfname */


   static void  parseread() {
	   //f.pln(" $$## parseread: ENTER!");
	   if (inmode == inmodes.batch ) {
		   error(12); 
		   skip(); 
	   } else {
		   String fname = getfname("mod");
		   if (fname.length() == 0 ) { 
			   error(18);
			   //f.pln("$$## parseread: ERROR"); 
		   } else {
			   /* ---------- switch lexical input to rmd ----------- */
			   //f.pln(" $$## parseread: switch input to fname = "+fname);
			   if (inmode != inmodes.interactive) 
				   closeRmdReader();
			   if (openFile(fname)) {
				   if (! (ch  == cheol || ch ==cheoc) )
					   Sure.println(" *** WARNING: REMAINDER ON INPUT LINE IGNORED");
				   chcnt = 0; 
				   insz = 0; 
				   ch = ' '; 
				   tkn = tokens.tksemi;
				   inmode = inmodes.batch;
				   //f.pln(" $$$$^^: parseread: set inmode = inmodes.batch");
				   lastfread = fname;
				   beginreadtm = (int) System.currentTimeMillis( );
			   } else {
				   //f.pln(" $$$ parseread: GO BACK TO INTERACTIVE");
				   openFile("");  // go back to interactive
				   inmode = inmodes.interactive;
			   }
		   }
	   }
   } /* parseread */

   static void closeRmdReader() {
	   try {
		   rmdReader.close();
		   f.pln(" \n $$$$$$$$$$$$$$ rmdReader closed! inmode = "+inmode);
	   } catch (IOException e) {
		   // TODO Auto-generated catch block
		   e.printStackTrace();
			Thread.dumpStack();
			System.exit(0);

	   }
   }

   
//   static void getzid() {
//	 if (zid != blankid)
//	    { zval = 0.0e+00;
//	    if (searchid(zid,zv)) 
//	       if (zv.coef != 0)
//	 	  Sure.println(" *** THE Z DIMENSION LABEL IS  DEFINED IN TERMS OF THE VARIABLE");
//	       else zval = sngl(zv.base);
//	    };
//   }; /* getzid */
//
   static double getSpecialVar(String id, double  def) {
	   double val;
	   ValuRec v = searchid(id);
	   if (v != null)
		   if (specid.equals(id))
			   val = specval;
		   else
			   val= v.base + v.coef*specval;
	   else val = def;
	   //f.pln(" ]]]]]]]]]]]]]]] setspecvar: id = "+id+" specid = "+specid+" val = "+val);
	   return val;
   } /* setspecvar */

   // Note that user-specified start state takes precedence over calculated source state
   // If there is no user specified start state and no source state can be found then the first state entered is used
   static int determinestart() {
	   int start = (int) (getdef("START",Front.defstart));
	   if (initflag) { 
		   if (start > 0)
			   Sure.println(" ERROR: you cannot specify both 'start' and 'initial_probs'");
	   }
	   int start_state;
	   int gstart = Sure.getstart(); /* determine source state */
	   //f.pln(" $$$ determinestart: gstart = "+gstart);
	   if (start >= 0)  {/* user specified start state */
		   start_state = start;
		   if (gstart >= 0) {
			   if (gstart != start) {
				   Sure.println("     SELECTED START STATE ("+start+") IS NOT THE SOURCE STATE ("+gstart+")");
			   }
		   } else if (gstart == -2) {
			   Sure.println(" *** MORE THAN ONE SOURCE STATE IN MODEL");
		   }
	   } else if (gstart  == -1) { 
		   if (firstst < 0) firstst = 1;
		   start_state = firstst;
		   Sure.println(" *** START STATE ASSUMED TO BE "+firstst);
	   } else if (gstart == -2) {
		   Sure.println(" *** MORE THAN ONE SOURCE STATE IN MODEL - "+firstst+" USED AS THE START STATE");
		   start_state = firstst;
	   }else {
		   start_state = gstart;
	   } /* case */
 	   //f.pln(" $$$ determinestart: start_state = "+start_state);
	   return start_state;

   } /* determinestart */
   
   
   static void printScratch() {   // debug
	   f.pln(" $$ printScratch --------------------------");
	   for (int ii = 0; ii < scrFile.size(); ii++) {
		   f.pln(ii+" "+scrFile.get(ii));
	   }
       f.pln(" ---------------------------------------");	   
   }
   
	/**
	 * Returns the file name without an extension (such as, ".txt").  Also removes path information. 
	 * This also converts backward slashes to forward slashes.
	 * @param filename  filename string
	 * @return filename without extension information
	 */
	public static String no_extension(String filename) {
		if (filename == null || filename.equals("")) {
			return null;
		}
		// replace all \ (dos) with / (unix), then find last
		//String newname = no_path(filename);
		return filename.substring(0,filename.lastIndexOf("."));
	}

   
   static void runit() {
	   //f.pln(" $$$$ runit: inbuf = "+inbuf);
	   double inc;
	   double fact;
	   double xt;
	   long lclock;
	   String saveid = ""; // : idtype;
	   String savinbuf = ""; // : buftype;
	   int savchcnt = -1;
	   int savinsz = -1;
	   inmodes savinmode = inmodes.batch;
	   char savch = ' ';
	   //f.pln(" runit: $$ ===================== nonlinear = "+nonlinear);
	   lclock = System.currentTimeMillis( ); 
	   runno = runno + 1; 
	   calclast = false;
	   Sure.println("");
	   //getzid();             /* retrieve z id for contour plotting */
	   getvars();            /* get run-time variable values */
	   int startstate = determinestart();    /* find or guess start state */
	   Sure.clear_trunc_prune_stats();
	   Sure.hdr1();
	   if (listlevel >= 3) {
		   int i = 0;
		   initpfname = no_extension(lastfread) +".ini"; 
		   //f.pln(" ## runit: lastfread = "+lastfread+" initpfname = "+initpfname);
		   openInitp();
		   //rewrite(initp,initpfname);
	   }
	   if (nonlinear) {   /* save state for gettoken */
		   savinbuf = inbuf; 
		   savchcnt = chcnt; 
		   savinsz = insz;
		   savch = ch; 
		   savinmode = inmode;
	   };
	   Pair<Double,Double> pp = getinc(); 
	   inc = pp.first;
	   fact = pp.second;
	   specval = speclow; 
	   //f.pln(" $$>>>>>>> runit: speclow = "+speclow+" spechigh = "+spechigh+" nonlinear = "+nonlinear);
	   saveid = specid; 
	   pltid = saveid;
	   boolean done = false;
	   int icnt = 0;
	   int autoprune = (int) (getdef("AUTOPRUNE",defautoprune));
	   int warndig = (int) Front.defwarndig;
	   while (!done) { 
		   lbfail = 0.0; ubfail = 0.0;
		   if (nonlinear) { /* must re-parse all of the transitions */
			   /* --- treat variable as a constant --- */ 
			   //init(); /* this initializes "specid","speclow","spechigh" */
			   specid = saveid;
			   ValuRec v = new ValuRec(0.0,specval,true);
			   Sure.clear(); 
			   /* --- enterid(specid,v);  --- rather than enter it here,   will let parsecon enter it at the same time as before.  
               will set global variable scrval = v */
			   scrval = v;
			   inmode = inmodes.scratch; 
			   if (echo != 2) echo = 0;
			   ch = ' '; chcnt = 0; insz = 0;
			   scr_line = 0;
			   eofFlag = false;
			   gettoken(); /* parsestatement assumes one gettoken */
			   //f.pln(" runit: ************* tkn = "+tkn+" rw = "+rw);
			   do {
				   parsestatement();
			   }  while (rw != rwtype.rwexit);
			   rw = rwtype.rwrun;
		   } /* if (nonlinear */
		   timet = getSpecialVar("TIME",10.0);
		   autoprune = (int) (getdef("AUTOPRUNE",defautoprune));
		   prune_d = getSpecialVar("PRUNE",Sure.probNearZero);
		   if (prune_d > 1E-100) autoprune = 0; /* PRUNE has precedence over AUTOPRUNE */
		   //f.pln("  %%%%%%%%%%%%%%% runit: prune = "+prune_d);
		   int ptrunc = (int) getSpecialVar("TRUNC",25.0);
		   if (searchid("TRUNC") == null ) {        /* trunc has precedence over notrunc */
			   int noTrunc = (int) (getdef("NOTRUNC",defnotrunc));
			   if (noTrunc == 1) ptrunc = Integer.MAX_VALUE;   // effectively no TRUNC VALUE
		   }
		   int autofast = (int) (getdef("AUTOFAST",defautofast));
		   if (ptrunc < 0) { 
			   ptrunc = -ptrunc;
			   error(61);
		   }
		   warndig = (int) getSpecialVar("WARNDIG",2.0);
		   if (listlevel >= 4)
			   Sure.println(" MISSION TIME = "+timet);
		   //dequethelist(initlist);       
		   // f.pln(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> runit: specval = "+specval);
		   //f.pln(" $$$$ runit: ptrunc = "+ptrunc+"  autofast = "+autofast);
		   Sure.compute(startstate, ptrunc,autofast,autoprune,warndig);                /* ========= COMPUTE MODEL ======== */
		   icnt = icnt + 1;
//		   if (icnt <= maxpts) {
//			   pltx[icnt] =specval;
//			   plty[icnt] = lbfail;
//			   pltz[icnt] = ubfail;
//		   }
		   if (icnt < maxvpr) {			    
			    RunResults rsp = new RunResults("",runno,lbfail,ubfail);
			    runvv[icnt] = specval;
			    if (runvals.get(icnt) == null) {/* first run */
			    	ArrayList<RunResults> arsp = new ArrayList<RunResults>(10);
			    	arsp.add(rsp);
			    	runvals.set(icnt, arsp); //runvals[icnt] = arsp; 
			    	endp.set(icnt, arsp); // endp[icnt] = arsp; 
			    } else {
			    	endp.get(icnt).add(rsp); // endp[icnt].add(rsp); 
			    }
		   }
		   //f.pln(" $$$$$ runit:  geometric = "+geometric+" fact = "+fact);
		   if (geometric) specval = specval*fact;
		   else specval = specval + inc;
		   if (Math.abs(specval-spechigh) < 1.0E-6*Math.abs(0.5*(specval+spechigh)))
			   /* accumulated rounding error */
			   specval = spechigh;
		   Sure.execstats(ptrunc,autoprune,warndig);
		   if (speclow <= spechigh)
			   done = (specval > spechigh);
		   else
			   done = (specval < spechigh);
	   }; /* while */

	   //f.pln(" $$$ runit AFTER WHILE: specval = "+specval+" ch = "+ch);
	   numpoints = icnt; 
	   plti = icnt;     
	   if (runno > 1) {
		   //f.pln(" $$$$$$$$$$ runit: runno = "+runno+" nvpr = "+nvpr+" icnt = "+icnt+" runnm = "+runnm+" "+specid);
		   if (nvpr != icnt) orok = false;
		   if (!runnm.equals(specid)) orok = false;
	   };
	   nvpr = icnt; 
	   runnm = specid;
	   if (nonlinear) {    /* restore state for gettoken; */
		   inbuf = savinbuf; 
		   chcnt = savchcnt; 
		   insz = savinsz; 
		   inmode = savinmode; 
		   ch = savch;
		   eofFlag = false;  //**NEW**
		   //f.pln("\n $$$$..........^^: runit: set inmode = "+inmode+" ch = "+ch);
	   }
	   Sure.println(""); 
	   if (esyn1)
		   Sure.println(" *** WARNING: SYNTAX ERRORS PRESENT BEFORE RUN");
	   else if (erun)
		   Sure.println(" *** WARNING: RUN-TIME PROCESSING ERRORS");
	   Sure.worststats(autoprune,warndig);     
	   double rclock = (System.currentTimeMillis( )-lclock)/1000.0;
	   //Sure.println(" "+rclock+" SECS. CPU TIME UTILIZED");
	   Sure.clear();  /* clear transitions from previous run */
	   initrun(); 
	   //f.pln(" .................... runit: END $$ ===================== ch = "+ch);
   } /*runit */

   static void openInitp() {
	    //f.pln(" $$$$>>>>> openInitp: initpfname = "+initpfname);
 		try {
			initp = new PrintWriter(new FileWriter(initpfname));
		}
	    catch (IOException e) {
	    	Sure.println("Can't open file " + initpfname + "!");
	    	Sure.println("Error: " + e);
	    	return;        // End the program.
	    }
   }
  
   static void parsestatement() {
	   //f.pln(" $$$$$$$$$$$$$ BEGIN parsestatement0 ch = "+ch+" tkn = "+tkn+" rw = "+rw);
	   errlast = errflag;
	   errflag = false; 
	   if (tkn == tokens.tksemi) { /* previous command terminated */
		   do { gettoken(); } while (tkn == tokens.tksemi);   /* skip empty statements */
	   }	      
	   //f.pln(" parsestatement1 tkn = "+tkn+" rw = "+rw);
	   if (!( tkn == tokens.tkint || tkn == tokens.tkat || tkn == tokens.tkid|| tkn == tokens.tkrw || tkn == tokens.tksemi)) {
		   //f.pln(" parsestatement, error(10) tkn = "+tkn);
		   error(10); 
	   } else {
		   //f.pln(" parsestatement2 tkn = "+tkn+" rw = "+rw);
		   if (tkn == tokens.tkat) {
			   gettoken(); 
			   parsetran(true);
		   } else if (tkn == tokens.tkint) {
			   parsetran(false);      
		   } else if (tkn == tokens.tkid){
			   parsecondef();
		   } else if (tkn == tokens.tkrw) {
			   if ( inmode == inmodes.scratch ) {
				   if ( !( rw == rwtype.rwlee || rw == rwtype.rwexit || rw == rwtype.rwif  ) ) {
					   skip(); 
				   }  else  {
					   if (rw == rwtype.rwlee) { 
						   rectype = Sure.lee; 
						   gettoken();
					   } else if ( rw == rwtype.rwexit) {
						   tkn = tokens.tksemi;
					   } else if (rw == rwtype.rwif) {
						   parseif();
					   } else if (rw == rwtype.rwinit) {   // INITIAL_PROBS
						   parseinit();                    
					   }
					   /*otherwise skip;*//*ultrix-3/88-phs*/
				   } /* case */
			   } else {
				   //f.pln(" parsestatement3 tkn = "+tkn+" rw = "+rw);
				   if (rw == rwtype.rwby || rw == rwtype.rwto || rw == rwtype.rwfast || rw == rwtype.rwthen || rw == rwtype.rwnull)  {
					   error(10); 
					   skip();/*ultrix-3/88-phs*/
				   }  else {
					   //f.pln(" $$$$$ parsestatement rw = "+rw);
					   if (rw == rwtype.rwrun) {
						   parserun();
					   } else if (rw == rwtype.rwdebug) {
						   parsedebug();
					   } else if (rw == rwtype.rwshow) {
						   parseshow();
					   } else if (rw == rwtype.rwread) {  
						   parseread();
					   } else if (rw == rwtype.rwread0) {  
						   parseread();
					   } else if (rw == rwtype.rwhelp) {
//						   parsehelp();
					   } else if (rw == rwtype.rwplot) {
						   parseplot();
					   } else  if (rw == rwtype.rwinit) {  
						   parseinit();
					   } else if (rw == rwtype.rwinput) {
						   parseinput();
					   } else if (rw == rwtype.rwif) {    
						   parseif();
					   } else if (rw == rwtype.rwcalc) {
						   calc();
					   } else  if (rw == rwtype.rwlee) {
						   rectype = Sure.lee; 
						   gettoken();
					   } else if (rw == rwtype.rwclr) {
						   parseclear();
					   } else  if (rw == rwtype.rworprob) { 
						   // parsefname();                         // ?????/
						   orprob();
					   } else if (rw == rwtype.rwprune) {
						   parseprune();
					   } else if (rw == rwtype.rwlinear) {
					   	    forcelinear = true; 
					   	    tkn = tokens.tksemi; 
					   } else if (rw == rwtype.rwexit) {
						   tkn = tokens.tksemi;
					   } 
				   }
			   }
		   } else if (tkn == tokens.tksemi) { /* empty statement */;
		   //	         /* otherwise error(10); *//*ultrix-3/88-phs*/
		   } /* case */
	   }
	   testandskip();
       //f.pln(" parsestatement: END ch= "+ch+" tkn = "+tkn+" rw = "+rw);
	   } /* parsestatement */



   static void parserun() {	   
	   parsefname();	   
	   if ( ! errflag ){ if ( echo != 2 ) echo = 0; /* do not echo these */
//	   if ( lval[0].base >= 0.0 ) enterid("POINTS",lval[0],rwtype.rwnull);
//	   if ( lval[1].base >= 0.0 ) enterid("LIST  ",lval[1],rwtype.rwnull);
//	   if ( lval[2].base >= 0.0 ) enterid("QTCALC",lval[2],rwtype.rwnull);
	   runit();
	   }
   } /* parserun */


   static void parseshow () {
	   ValuRec v1;
	   gettoken();
	   while (tkn == tokens.tkid|| tkn == tokens.tkint || tkn == tokens.tkreal) {			   
		   if ( tkn == tokens.tkid ) {
			   Sure.print("      ");
			   v1 = searchid(id);
			   if (v1 == null ) 
				   Sure.println(id+" NOT FOUND");
			   else if ( id.equals(specid) ) 
				   Sure.println(id+" = VARIABLE FROM "+speclow+" TO "+ spechigh);
			   else {
				   Sure.print(id+" = "); 
				   prval(v1); 
				   Sure.println("");
			   }
			   gettoken(); /* skip past ; */
		   }  else if ( tkn == tokens.tkint ) { 
			   int i1 = intv; 
			   gettoken(); 
			   int i2 = -1;
			   if ( tkn == tokens.tksub ) {
				   gettoken(); 
				   if ( tkn == tokens.tkint ) {
					   i2 = intv; 
					   gettoken();
				   };
			   };
			   if ( tkn == tokens.tksemi || tkn == tokens.tkcomma )
				   Sure.printtran(i1,i2);
			   else error(4);
		   }
		   else if ( tkn == tokens.tkreal) {
			   Sure.println("      = "+f.Em5(val)); gettoken();
		   }
		   else error(9);
		   if ( tkn == tokens.tkcomma ) 
			   gettoken();
	   }// while
   }; /* parseshow */


   static void parsetran(boolean leeTran) {
	   int stfrom; 
	   int stto;
	   boolean fastf;
	   ValuRec vp1 = val0;
	   ValuRec vp2 = val0;
	   ValuRec vp3 = val0;
	   //f.pln(" $$$$ START in parsettran tkn = "+tkn);
	   if (tkn != tokens.tkint) error(3);
	   else {
		   stfrom = intv; gettoken();
		   if (!(tkn  == tokens.tkcomma || tkn == tokens.tkeq)) {
			   //f.pln(" parsetran, at error(4) tkn = "+tkn);
			   error(4);
		   }
		   else {
			   stto = -1;
			   if (tkn == tokens.tkcomma) {
				   gettoken(); 
				   if (tkn != tokens.tkint) error(3);
				   else {
					   stto = intv; gettoken();
				   }
			   } else {
				   if (rectype == 0) error(27);
			   }
			   if (stto == stfrom) error(11);
			   if (Front.rectype == Sure.lee) {
				   if (stto == 0 || stfrom == 0) error(60);
			   }
			   //f.pln(" $$$$ in parsettran  stfrom = "+stfrom+" stto = "+stto);
			   strstart = chcnt; gettoken();
			   if (tkn == tokens.tklt) {   /* ------ recovery transition ----- */
				   gettoken(); 
				   expression();
				   vp1 = gval; 
				   //f.pln(" $$$ parsetran1 vp1 = "+vp1);
				   if (tkn != tokens.tkcomma) {/* if lee type  then 1 parameter */
					   if (stto == -1) error(4);
					   else if (rectype == 0) error(27);
					   else if (tkn == tokens.tkgt) { /* lee transition */
						   etrans(stfrom,stto,trantype.recov,valm1,valm1,vp1);
						   gettoken(); /* skip past ; */
					   }
				   } else {
					   gettoken(); 
					   expression(); 
					   vp2 = gval;
					   //f.pln(" $$$ parsetran1 vp2 = "+vp2);
					   if (tkn == tokens.tkcomma) {   /* lee @ or white tran. */
						   gettoken(); 
						   expression();; 
						   vp3 = gval;         /* --- 3 parameters --- */
						   if ((stto > 0) && (rectype == 1)) error(28);
					   } else {  /* --- 2 parameters --- */
						   vp3.coef = 0.0; vp3.linear = true;
						   if (rectype == 1) error(30);
						   else
							   vp3.base = 1.0;       /* white transition */
					   }
					   if (tkn != tokens.tkgt) error(13);
					   else {
						   if (stto == -1 && leeTran) stto = 0;      // RWB, NEW: changing it to 0 rather than -1
						   if (!errflag) etrans(stfrom,stto,trantype.recov,vp1,vp2,vp3);
						   gettoken(); /* skip past ; */
					   }
				   }
			   } else {             /* ----- exponential transition ----- */
				   if (stto == -1) {
					   error(10);
				   }
				   if ((tkn == tokens.tkrw) && (rw == rwtype.rwfast)) {
					   fastf = true; gettoken(); 
				   } else fastf = false;
				   expression();
				   if (!errflag) {
					   if (fastf)  etrans(stfrom,stto,trantype.fastexpo,gval,valm1,valm1);
					   else 		etrans(stfrom,stto,trantype.expo,gval,valm1,valm1);
				   }
			   }
		   }
	   }
	   //f.pln(" $$ EXIT PARSETRAN,  TKN = "+tkn);
   } /* parsetran */      
   
   
   static void parseprune() {
	   gettoken();
	   //f.pln(" $$$$$$$ parseprune0: tkn = "+tkn);
	   if ( tkn == tokens.tkeq) {
		   gettoken();
		   //f.pln(" $$$$$$$ parseprune00000: tkn = "+tkn+" errflag = "+errflag);
		   if (tkn == tokens.tkint || tkn == tokens.tklp) {
			   if (tkn == tokens.tkint) {
				   if (!errflag) {
					   clearprune(); 
					   enterprune( intv );
				   }
			   } else if (tkn == tokens.tklp) { 
				   clearprune(); 
				   boolean done = false;
				   do {
					   gettoken();
					   if (tkn == tokens.tkint ) {
						   if (errflag ) {
							   error(41); 
							   clearprune(); 
							   done = true;
						   } else { /* if errflag */
							   enterprune( intv );
							   gettoken();
							   if  (tkn == tokens.tkrp) {
								   done = true;
							   } else if (tkn == tokens.tkcomma) {					 
							   } else {
								   error(41); 
								   clearprune(); 
								   done = true;
							   }
						   } /* else ! errflag */
					   } /* if tkn */
					   else {/* tkn != tkint */ 
						   error(41); 
						   clearprune(); 	
						   done = true;
					   }/*else*/
				   } while (!done);
			   } else {
				   error(41); 
				   clearprune();
			   };
		   } else {
			   error(5); /* equals expected */
		   }
		   gettoken();
		   //f.pln(" $$$$$$$ parseprune 6:  tkn = "+tkn+" intv = "+intv);
		   testandskip();
		   //Sure.println(""); 
	   }
   } /* parseprune */


   static Pair<Double,Double> getinc() {  // (var inc, fact: double);
	   double inc = 0;
	   double fact = 1;
	   int np = numpoints;
	   //f.pln(" $$>>>>0 getinc: byfact = "+byfact+" byinc = "+byinc+" specid = "+specid+" np = "+np+" geometric = "+geometric);
	   if ((np == 0) && (byfact == 0.0) && (byinc == 0.0) ) {
		   np = 10;  /* DEFAULT: POINTS = 10 */   
	   }	 
	   if (specid == "" ) {/* no variable */
		   geometric = false; 
		   inc = 1;
		   speclow = 0.0; 
		   spechigh = 0.0;
	   }  else if (np == 0) { /* BY was used */
		   if (geometric) fact = byfact;
		   else inc = byinc;
		   //f.pln(" $$ getinc: fact = "+fact);
	   } else {        /* POINTS was used */
		   inc = 100; 
		   fact = 100.0;
		   if (! (geometric )) { /* arithmetic range is desired */
			   if (np  > 1)
				   inc = (spechigh - speclow ) / (np - 1);
			   else inc = (spechigh - speclow);
			   if (inc == 0 ) inc = 1;
		   } else  {/* geometric range is desired */
			   if (np  > 1) fact = Math.exp(Math.log(spechigh/speclow)/(np - 1));
			   else fact = spechigh/speclow;
		   };
	   };
	   return new Pair<Double,Double>(inc,fact);
   } /* getinc */

   static void calc() {
	   int np = (int) getdef("POINTS      ",50.0);
	   //f.pln(" $$$$ ENTER CALC np = "+np);
	   if (specid.equals("")) np = 1;
	   int savchcnt = chcnt;
	   if (specid.equals("")) {
		   gettoken(); 
		   expression();
		   if ((tkn == tokens.tkrw) && (rw == rwtype.rwto)) parsefname();
		   else if (tkn != tokens.tksemi) error(38);
		   if (!errflag)
			   Sure.println(" "+" = "+gval.base); 
		   Sure.println("");
	   } else { 
           gval = valm1;
		   int i = savchcnt; 
		   calcfun = "";
		   while (!(inbuf.charAt(i) == cheoc || i >= inbuf.length())) {
			   calcfun = calcfun + inbuf.charAt(i); 
			   i = i + 1;
		   }
		   calcfun = calcfun + cheol; 
		   calclast = true;
		   int lv = (int) getdef("LIST        ",1.0);
		   gettoken();
		   expression(); 
		   if ((tkn == tokens.tkrw) && (rw == rwtype.rwto)) parsefname();
		   else if (tkn != tokens.tksemi) error(38);
		   Pair<Double,Double> pp = getinc(); 
		   double inc = pp.first;
		   double fact = pp.second;
		   String saveid = specid; 
		   specid = "";
		   int savecho = echo; 
		   echo = 0; 
		   int savlcnt = linecnt;
		   //v.base = speclow; v.coef = 0; v.linear = true;
		   ValuRec v = new ValuRec(0,speclow,true);
		   plti = 0; 
		   pltid = saveid;
		   boolean done = false; 
		   i = 0;
		   while(!done) {
			   i = i + 1;
			   enterid(saveid,v,rwtype.rwnull);
			   chcnt = savchcnt;  
			   ch = ' ';
			   gettoken(); 
			   expression();
			   if (! gval.linear) {
				   Sure.println("");
				   Sure.println("     IN 'CALC' MODE CONSTANTS MUST NOT BE DEFINED IN TERMS OF A NONLINEAR VARIABLE");
				   Sure.println("     TYPE 'CLEAR' AND TRY AGAIN");
				   Sure.println(""); 
				   break;
				   //goto 9;
			   };
			   if (errflag) break;
			   if (i == 1) {
				   if (lv > 0) {
					   Sure.println("");
					   Sure.println("       "+saveid+"            VALUE");
					   Sure.println("  -------------------     -------------------"); 
				   }
			   }
			   double ans = gval.base + v.base*gval.coef;
			   if (lv > 0) Sure.println(" "+v.base+"    "+ans);
//			   if (plti <= maxpts) {
//				   plti = plti + 1;
//				   pltx[plti] = v.base; 
//				   plty[plti] = ans; 
//				   pltz[plti] = ans;
//			   };
			   if (geometric) v.base = v.base*fact;
			   else v.base = v.base + inc;

			   if (speclow <= spechigh)
				   done = (v.base > spechigh);
			   else
				   done = (v.base < spechigh);

		   }; /* while */
		   // 9:  
		   specid = saveid; 
		   echo = savecho; 
		   pltsaved = false;
		   if (!zid.equals("")) {
			   zval = 0.0e+00;
			   ValuRec zv = searchid(zid);   
			   if (zv != null) /* note searchid changes rw */
				   if (zv.coef != 0)
					   Sure.println("     THE Z DIMENSION LABEL IS DEFINED IN TERMS OF THE VARIABLE");
				   else zval = zv.base;
		   }
		   if (linecnt != savlcnt)
			   Sure.println(" *** THE *CALC* EXPRESSION MUST BE ON 1 LINE ");
	   }
	   if (tkn != tokens.tksemi) skip();
	   //f.pln(" $$$$ EXIT CALC tkn = "+tkn);
   } /* calc */



     static double p2(double a, double b) {
        return a + b - a*b;
     }


     static Pair<Double,Double> padd(ArrayList<RunResults> rp, int n) {   // return double alb, double aub) {
     	 double alb = -1;
    	 double aub = -1;
    	 if (n == 1) { 
    		 alb = rp.get(0).lb; 
    		 aub= rp.get(0).ub;
    	 } else if (n == 2) {
    		 alb = p2(rp.get(0).lb,rp.get(1).lb);
    		 aub = p2(rp.get(0).ub,rp.get(1).ub);
    	 } else {
    		 int n1 = n / 2; 
    		 int n2 = n - n1;
    		 ArrayList<RunResults> lp1 = rp;
    		 ArrayList<RunResults> lp2 = new ArrayList<RunResults>();
    		 Collections.copy(rp,lp2); 
    		 for (int i = 1; i <= n1; i++) lp2.remove(0);
    		 Pair<Double,Double> p1 = padd(lp1,n1); // ,lb1,ub1); 
    		 double lb1 = p1.first;
    		 double ub1 = p1.second;
    		 Pair<Double,Double> p2 = padd(lp2,n2); //,lb2,ub2);
    		 double lb2 = p2.first;
    		 double ub2 = p2.second;
    		 alb = p2(lb1,lb2);
    		 aub = p2(ub1,ub2);
    	 }
    	 return new Pair<Double,Double>(alb,aub);
     } /* padd */

      static void orprob_fullist(double xval, ArrayList<RunResults> rp, int i, int lv) {
    	 Sure.print("    RUN #      LOWERBOUND    UPPERBOUND");
    	 if (nvpr > 1)
    		 if (lv == 2)
    			 Sure.print("          "+runnm+" = "+f.Em5(xval));
    	 Sure.println("");
    	 Sure.println(" ----------   -----------   -----------");
    	 for (int j = 0; j < rp.size(); j++) {
    		 RunResults pArrow =  rp.get(j);  
    		 Sure.println(f.padLeft(""+pArrow.runno,8)+"      "+f.Em5(pArrow.lb)+"   "+f.Em5(pArrow.ub));
    	 };
    	 Sure.println(" ----------   -----------   -----------");
    	 Pair<Double,Double> p0 = padd(rp,runno); //,alb,aub);
    	 double alb = p0.first;
    	 double aub = p0.second;
    	 Sure.println("  OR PROB =   "+f.Em5(alb)+"   "+f.Em5(aub));
    	 Sure.println("");
    	 //	    	  pltx[i] = xval;
    	 //	    	  plty[i] = alb;
    	 //	    	  pltz[i] = aub;
     } /* fullist */
     
     static void orprob_shortlist(int i, double xval, ArrayList<RunResults> rp) {
    	 Pair<Double,Double> p0 = padd(rp,runno); // ,alb,aub);
    	 double alb = p0.first;
    	 double aub = p0.second;
    	 Sure.println(f.padLeft(f.Em5(xval),9)+"  "+f.Em5(alb)+"  "+f.Em5(aub));
    	 //	    	  pltx[i] = xval;
    	 //	    	  plty[i] = alb;
    	 //	    	  pltz[i] = aub;
     }


      static void orprob() {
    	  gettoken();
    	  //f.pln("\n $$$$$$$$$$$$$>> orprob: nvpr = "+nvpr+" maxvpr = "+maxvpr+" runvals.length = "+runvals.length+" runno = ");
    	  //f.pln(" $$ orprob: orok = "+orok+" runno = "+runno);
      	  if (orok) {
    		  if (runno > 0) { 
    			  int lv = (int) (getdef("LIST",1.0));
    			  //f.pln(" $$$$ orprob: lv = "+lv);
    			  if (nvpr == 1) { 
    				  int i = 1;
    				  orprob_fullist(runvv[i],runvals.get(i),i,lv); 
    			  } else {
    				  int nv = nvpr; 
    				  if (nv >= maxvpr) { 
    					  nv = maxvpr-1; 
    					  Sure.println("     ONLY "+maxvpr+" VARIABLE RESULTS STORED"); 
    					  Sure.println("");
    				  }
    				  if (lv == 2) { 
    					  for (int i = 1; i <= nv; i++) {
    						  orprob_fullist(runvv[i],runvals.get(i),i,lv); 
    					  }
    				  } else { 
    					  //nch = idsiglen(runnm);
    					  //halfnch = (idleng-nch) div 2;
    					  //tailnch = idleng-nch-halfnch;
    					  Sure.print(" "+"("+runnm+")"+" ");
    					  Sure.println("LOWERBOUND    UPPERBOUND");
    					  Sure.println("   -----------   -----------   -----------"); 
    					  for (int i = 1; i <= nv; i++) { 
    						  orprob_shortlist(i,runvv[i],runvals.get(i));
    					  }
    				  }
    			  }
    		  } else {
    			  Sure.println("     NO RUNS MADE YET");
    		  }
    	  } else {
    		  Sure.println(" *** VARIABLES INCONSISTENT BETWEEN RUNS ");
    	  }
      } /* orprob */
      
      
      static void getvars() {	   
    	  ValuRec v = searchid("PRUNE");   
    	  if (v != null && ! v.linear) nonlinear = true;
    	  v = searchid("TIME");
    	  if (v != null && ! v.linear) nonlinear = true;
    	  v = searchid("TRUNC");
    	  if (v != null && ! v.linear) nonlinear = true;
    	  v = searchid("WARNDIG");
    	  if (v != null && ! v.linear) nonlinear = true;    
    	  listlevel  = (int) (getdef("LIST",deflistlevel));
    	  //start = (int) (getdef("START",-1.0));
    	  //qtcalc = (int) (getdef("QTCALC",defqtcalc));
    	  //autofast = (int) (getdef("AUTOFAST",defautofast));
    	  //autoprune = (int) (getdef("AUTOPRUNE",defautoprune));
    	  //notrunc =   (int) (getdef("NOTRUNC",0.0));
    	  v = searchid("TRUNC");
    	  //if (v != null) notrunc = 0;         /* trunc has precedence over notrunc */
    	  numpoints = 1;
    	  if (!specid.equals(""))
    		  numpoints = (int) (getdef("POINTS",0.0));
    	  //f.pln("$$$$ getvars: numpoints =  "+numpoints+" autoprune = "+autoprune);	      
      }; /* getvars */

	   public static void execSURE() {
		   Front.initsymtable(); 
		   //		   rerite(scr);
		   Front.initLexical();
		   Front.initrun(); 
		   Front.gettoken(); /* parsestatement assumes one gettoken() */
		   //f.pln(" $$$$$$$$ MAIN: tkn = "+Front.tkn+" rw = "+Front.rw+" ch = "+Front.ch);
		   do {
			   Front.parsestatement();
		   } while (Front.rw != rwtype.rwexit);
	   }


}
