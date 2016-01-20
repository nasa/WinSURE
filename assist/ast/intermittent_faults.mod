L_I_TRIAD = 0.707E-4;
ALP_TRIAD = 2.718281E3;
BET_TRIAD = 3.141592E3;
NI_TRIAD = 3;
NTOT = NI_TRIAD;


     2(* 3,0,0 *),     3(* 2,1,0 *)        = 3*L_I_TRIAD;
     3(* 2,1,0 *),     4(* 2,0,1 *)        = FAST BET_TRIAD;
     3(* 2,1,0 *),     1(* 1,2,0 DEATH  *) = 2*L_I_TRIAD;
     4(* 2,0,1 *),     3(* 2,1,0 *)        = FAST ALP_TRIAD;
     4(* 2,0,1 *),     5(* 1,1,1 *)        = 2*L_I_TRIAD;
     5(* 1,1,1 *),     1(* 1,2,0 DEATH  *) = FAST ALP_TRIAD;
     5(* 1,1,1 *),     6(* 1,0,2 *)        = FAST BET_TRIAD;
     5(* 1,1,1 *),     1(* 0,2,1 DEATH  *) = 1*L_I_TRIAD;
     6(* 1,0,2 *),     5(* 1,1,1 *)        = FAST ALP_TRIAD;
     6(* 1,0,2 *),     7(* 0,1,2 *)        = 1*L_I_TRIAD;
     7(* 0,1,2 *),     1(* 0,2,1 DEATH  *) = FAST ALP_TRIAD;
     7(* 0,1,2 *),     8(* 0,0,3 *)        = FAST BET_TRIAD;
     8(* 0,0,3 *),     7(* 0,1,2 *)        = FAST ALP_TRIAD;

(* NUMBER OF STATES IN MODEL = 8 *)
(* NUMBER OF TRANSITIONS IN MODEL = 13 *)
(* 4 DEATH STATES AGGREGATED INTO STATE 1 *)
