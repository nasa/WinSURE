NP = 4;
LAMBDA_P = 1E-4;
LAMBDA_T = 10*LAMBDA_P;
DELTA_D = .5;
DELTA_R = 3.6E3;


     2(* 4,0,0 *),     3(* 3,1,0 *)        = 4*LAMBDA_P;
     2(* 4,0,0 *),     4(* 3,0,1 *)        = 4*LAMBDA_T;
     3(* 3,1,0 *),     5(* 3,0,0 *)        = FAST DELTA_R;
     3(* 3,1,0 *),     1(* 2,2,0 DEATH  *) = 3*LAMBDA_P;
     3(* 3,1,0 *),     1(* 2,1,1 DEATH  *) = 3*LAMBDA_T;
     4(* 3,0,1 *),     2(* 4,0,0 *)        = FAST DELTA_D;
     4(* 3,0,1 *),     5(* 3,0,0 *)        = FAST DELTA_R;
     4(* 3,0,1 *),     1(* 2,1,1 DEATH  *) = 3*LAMBDA_P;
     4(* 3,0,1 *),     1(* 2,0,2 DEATH  *) = 3*LAMBDA_T;
     5(* 3,0,0 *),     6(* 2,1,0 *)        = 3*LAMBDA_P;
     5(* 3,0,0 *),     7(* 2,0,1 *)        = 3*LAMBDA_T;
     6(* 2,1,0 *),     8(* 2,0,0 *)        = FAST DELTA_R;
     6(* 2,1,0 *),     1(* 1,2,0 DEATH  *) = 2*LAMBDA_P;
     6(* 2,1,0 *),     1(* 1,1,1 DEATH  *) = 2*LAMBDA_T;
     7(* 2,0,1 *),     5(* 3,0,0 *)        = FAST DELTA_D;
     7(* 2,0,1 *),     8(* 2,0,0 *)        = FAST DELTA_R;
     7(* 2,0,1 *),     1(* 1,1,1 DEATH  *) = 2*LAMBDA_P;
     7(* 2,0,1 *),     1(* 1,0,2 DEATH  *) = 2*LAMBDA_T;
     8(* 2,0,0 *),     1(* 1,1,0 DEATH  *) = 2*LAMBDA_P;
     8(* 2,0,0 *),     1(* 1,0,1 DEATH  *) = 2*LAMBDA_T;

(* NUMBER OF STATES IN MODEL = 8 *)
(* NUMBER OF TRANSITIONS IN MODEL = 20 *)
(* 10 DEATH STATES AGGREGATED INTO STATE 1 *)
