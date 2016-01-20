NP = 4;
LAMBDA = 1E-4;
GAMMA = 10*LAMBDA;
W = .5;
DELTA = 3.6E3;


     2(* 4,0,0 *),     3(* 3,1,0 *)        = 4*LAMBDA;
     2(* 4,0,0 *),     4(* 3,0,1 *)        = 4*GAMMA;
     3(* 3,1,0 *),     5(* 3,0,0 *)        = FAST DELTA;
     3(* 3,1,0 *),     1(* 2,2,0 DEATH  *) = 3*LAMBDA;
     3(* 3,1,0 *),     1(* 2,1,1 DEATH  *) = 3*GAMMA;
     4(* 3,0,1 *),     2(* 4,0,0 *)        = FAST W;
     4(* 3,0,1 *),     5(* 3,0,0 *)        = FAST DELTA;
     4(* 3,0,1 *),     1(* 2,1,1 DEATH  *) = 3*LAMBDA;
     4(* 3,0,1 *),     1(* 2,0,2 DEATH  *) = 3*GAMMA;
     5(* 3,0,0 *),     6(* 2,1,0 *)        = 3*LAMBDA;
     5(* 3,0,0 *),     7(* 2,0,1 *)        = 3*GAMMA;
     6(* 2,1,0 *),     8(* 2,0,0 *)        = FAST DELTA;
     6(* 2,1,0 *),     1(* 1,2,0 DEATH  *) = 2*LAMBDA;
     6(* 2,1,0 *),     1(* 1,1,1 DEATH  *) = 2*GAMMA;
     7(* 2,0,1 *),     5(* 3,0,0 *)        = FAST W;
     7(* 2,0,1 *),     8(* 2,0,0 *)        = FAST DELTA;
     7(* 2,0,1 *),     1(* 1,1,1 DEATH  *) = 2*LAMBDA;
     7(* 2,0,1 *),     1(* 1,0,2 DEATH  *) = 2*GAMMA;
     8(* 2,0,0 *),     1(* 1,1,0 DEATH  *) = 2*LAMBDA;
     8(* 2,0,0 *),     1(* 1,0,1 DEATH  *) = 2*GAMMA;

(* NUMBER OF STATES IN MODEL = 8 *)
(* NUMBER OF TRANSITIONS IN MODEL = 20 *)
(* 10 DEATH STATES AGGREGATED INTO STATE 1 *)
