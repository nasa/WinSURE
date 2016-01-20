NP = 6;
NA = 3;
LAMBDA = 1E-4;
DELTA = 3.6E3;


     2(* 6,5,4,0,0,0 *),     3(* 6,5,4,1,2,3 *)        = (6+5+4+0+0+0)*LAMBDA;
     3(* 6,5,4,1,2,3 *),     1(* 6,5,4,2,4,6 DEATH  *) = (6+5+4+1+2+3)*LAMBDA;

(* NUMBER OF STATES IN MODEL = 3 *)
(* NUMBER OF TRANSITIONS IN MODEL = 2 *)
(* 1 DEATH STATES AGGREGATED INTO STATE 1 *)
