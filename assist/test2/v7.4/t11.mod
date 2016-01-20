LAMBDA = 1E-4;
DELTA = 3.6E3;


     2(* 6,6,6,6,6 *),     3(* 5,6,6,6,200 *)        = FOONESS;
     2(* 6,6,6,6,6 *),     4(* 5,6,6,6,6 *)        = FOO1;
     2(* 6,6,6,6,6 *),     5(* 5,5,6,6,6 *)        = FOO2;
     3(* 5,6,6,6,200 *),     1(* 2,6,6,5,200 DEATH  *) = FOO4;
     4(* 5,6,6,6,6 *),     1(* 2,6,6,5,6 DEATH  *) = FOO4;
     5(* 5,5,6,6,6 *),     1(* 3,5,5,6,6 DEATH  *) = FOO3;
     5(* 5,5,6,6,6 *),     1(* 2,5,6,5,6 DEATH  *) = FOO4;

(* NUMBER OF STATES IN MODEL = 5 *)
(* NUMBER OF TRANSITIONS IN MODEL = 7 *)
(* 4 DEATH STATES AGGREGATED INTO STATE 1 *)
(* 0001 WARNING *)
