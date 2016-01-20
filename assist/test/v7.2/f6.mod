NP = 6;
FOOBAR_1 = 5;
FOOBAR_2 = 1;
FOOBAR_3 = 6;
ONE = 1;
LAMBDA = 1E-4;
DELTA = 3.6E3;


     2(* 6,5,0,6 *),     3(* 6,1,1,1 *)        = (6-0)*5*LAMBDA;
     3(* 6,1,1,1 *),     4(* 6,1,2,1 *)        = (6-1)*5*LAMBDA;
     4(* 6,1,2,1 *),     1(* 6,1,3,1 DEATH  *) = (6-2)*5*LAMBDA;

(* NUMBER OF STATES IN MODEL = 4 *)
(* NUMBER OF TRANSITIONS IN MODEL = 3 *)
(* 1 DEATH STATES AGGREGATED INTO STATE 1 *)
