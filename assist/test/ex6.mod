LAM_S = 1E-6;
LAM_C = 1E-5;


     2(* 0,0,0,1 *),     3(* 1,0,0,1 *)        = (3-0)*LAM_C;
     2(* 0,0,0,1 *),     4(* 0,1,0,1 *)        = (3-0)*LAM_C;
     2(* 0,0,0,1 *),     5(* 1,1,1,1 *)        = 3*LAM_S;
     3(* 1,0,0,1 *),     1(* 2,0,0,1 DEATH  *) = (3-1)*LAM_C;
     3(* 1,0,0,1 *),     6(* 1,1,0,1 *)        = LAM_C;
     3(* 1,0,0,1 *),     7(* 1,1,0,0 *)        = (2-0)*LAM_C;
     3(* 1,0,0,1 *),     1(* 2,2,2,0 DEATH  *) = (3-1)*LAM_S;
     3(* 1,0,0,1 *),     5(* 1,1,1,1 *)        = LAM_S;
     4(* 0,1,0,1 *),     6(* 1,1,0,1 *)        = LAM_C;
     4(* 0,1,0,1 *),     7(* 1,1,0,0 *)        = (2-0)*LAM_C;
     4(* 0,1,0,1 *),     1(* 0,2,0,1 DEATH  *) = (3-1)*LAM_C;
     4(* 0,1,0,1 *),     1(* 2,2,2,0 DEATH  *) = (3-1)*LAM_S;
     4(* 0,1,0,1 *),     5(* 1,1,1,1 *)        = LAM_S;
     5(* 1,1,1,1 *),     1(* 2,1,1,1 DEATH  *) = LAM_C;
     5(* 1,1,1,1 *),     1(* 2,1,1,0 DEATH  *) = (2-1)*LAM_C;
     5(* 1,1,1,1 *),     1(* 1,2,1,1 DEATH  *) = LAM_C;
     5(* 1,1,1,1 *),     1(* 1,2,1,0 DEATH  *) = (2-1)*LAM_C;
     5(* 1,1,1,1 *),     1(* 2,2,2,0 DEATH  *) = (3-1)*LAM_S;
     5(* 1,1,1,1 *),     5(* 1,1,1,1 *)        = LAM_S;
     6(* 1,1,0,1 *),     1(* 2,1,0,1 DEATH  *) = LAM_C;
     6(* 1,1,0,1 *),     1(* 2,1,0,0 DEATH  *) = (2-1)*LAM_C;
     6(* 1,1,0,1 *),     1(* 1,2,0,1 DEATH  *) = LAM_C;
     6(* 1,1,0,1 *),     1(* 1,2,0,0 DEATH  *) = (2-1)*LAM_C;
     6(* 1,1,0,1 *),     1(* 2,2,2,0 DEATH  *) = (3-1)*LAM_S;
     6(* 1,1,0,1 *),     5(* 1,1,1,1 *)        = LAM_S;
     7(* 1,1,0,0 *),     1(* 2,1,0,0 DEATH  *) = LAM_C;
     7(* 1,1,0,0 *),     1(* 2,1,0,0 DEATH  *) = (2-1)*LAM_C;
     7(* 1,1,0,0 *),     1(* 1,2,0,0 DEATH  *) = LAM_C;
     7(* 1,1,0,0 *),     1(* 1,2,0,0 DEATH  *) = (2-1)*LAM_C;
     7(* 1,1,0,0 *),     1(* 2,2,2,0 DEATH  *) = (3-0)*LAM_S;

(* NUMBER OF STATES IN MODEL = 7 *)
(* NUMBER OF TRANSITIONS IN MODEL = 30 *)
(* 19 DEATH STATES AGGREGATED INTO STATE 1 *)
