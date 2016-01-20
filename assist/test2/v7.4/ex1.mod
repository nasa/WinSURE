NP = 6;
LAMBDA = 1E-4;
DELTA = 3.6E3;


     2(* 6,0 *),     3(* 6,1 *)        = (6-0)*LAMBDA;
     3(* 6,1 *),     4(* 5,0 *)        = <1/(1*DELTA),1/(1*DELTA)>;
     3(* 6,1 *),     5(* 6,2 *)        = (6-1)*LAMBDA;
     4(* 5,0 *),     6(* 5,1 *)        = (5-0)*LAMBDA;
     5(* 6,2 *),     6(* 5,1 *)        = <1/(2*DELTA),1/(2*DELTA)>;
     5(* 6,2 *),     1(* 6,3 DEATH  *) = (6-2)*LAMBDA;
     6(* 5,1 *),     7(* 4,0 *)        = <1/(1*DELTA),1/(1*DELTA)>;
     6(* 5,1 *),     8(* 5,2 *)        = (5-1)*LAMBDA;
     7(* 4,0 *),     9(* 4,1 *)        = (4-0)*LAMBDA;
     8(* 5,2 *),     9(* 4,1 *)        = <1/(2*DELTA),1/(2*DELTA)>;
     8(* 5,2 *),     1(* 5,3 DEATH  *) = (5-2)*LAMBDA;
     9(* 4,1 *),    10(* 3,0 *)        = <1/(1*DELTA),1/(1*DELTA)>;
     9(* 4,1 *),     1(* 4,2 DEATH  *) = (4-1)*LAMBDA;
    10(* 3,0 *),    11(* 3,1 *)        = (3-0)*LAMBDA;
    11(* 3,1 *),    12(* 2,0 *)        = <1/(1*DELTA),1/(1*DELTA)>;
    11(* 3,1 *),     1(* 3,2 DEATH  *) = (3-1)*LAMBDA;
    12(* 2,0 *),     1(* 2,1 DEATH  *) = (2-0)*LAMBDA;

(* NUMBER OF STATES IN MODEL = 12 *)
(* NUMBER OF TRANSITIONS IN MODEL = 17 *)
(* 5 DEATH STATES AGGREGATED INTO STATE 1 *)
