R_D_TRIAD = 1.414E3;
L_I_TRIAD = 0.707E4;
ALP_TRIAD = 2.718281E3;
BET_TRIAD = 3.141592E3;
NI_TRIAD = 3;
NTOT = NI_TRIAD;


     2(* 3,0,0 *),     3(* 2,1,0 *)        = 3*L_I_TRIAD;
     3(* 2,1,0 *),     4(* 2,0,1 *)        = FAST BET_TRIAD;
     3(* 2,1,0 *),     5(* 1,0,0 *)        = FAST R_D_TRIAD;
     3(* 2,1,0 *),     1(* 1,2,0 DEATH  *) = 2*L_I_TRIAD;
     4(* 2,0,1 *),     3(* 2,1,0 *)        = FAST ALP_TRIAD;
     4(* 2,0,1 *),     6(* 1,1,1 *)        = 2*L_I_TRIAD;
     5(* 1,0,0 *),     1(* 0,1,0 DEATH  *) = 1*L_I_TRIAD;
     6(* 1,1,1 *),     1(* 1,2,0 DEATH  *) = FAST ALP_TRIAD;
     6(* 1,1,1 *),     7(* 1,0,2 *)        = FAST BET_TRIAD;
     6(* 1,1,1 *),     8(* 0,0,1 *)        = FAST 0.5*R_D_TRIAD;
     6(* 1,1,1 *),     5(* 1,0,0 *)        = FAST 0.5*R_D_TRIAD;
     6(* 1,1,1 *),     1(* 0,2,1 DEATH  *) = 1*L_I_TRIAD;
     7(* 1,0,2 *),     6(* 1,1,1 *)        = FAST ALP_TRIAD;
     7(* 1,0,2 *),     9(* 0,1,2 *)        = 1*L_I_TRIAD;
     8(* 0,0,1 *),     1(* 0,1,0 DEATH  *) = FAST ALP_TRIAD;
     9(* 0,1,2 *),     1(* 0,2,1 DEATH  *) = FAST ALP_TRIAD;
     9(* 0,1,2 *),    10(* 0,0,3 *)        = FAST BET_TRIAD;
     9(* 0,1,2 *),     8(* 0,0,1 *)        = FAST R_D_TRIAD;
    10(* 0,0,3 *),     9(* 0,1,2 *)        = FAST ALP_TRIAD;

(* NUMBER OF STATES IN MODEL = 10 *)
(* NUMBER OF TRANSITIONS IN MODEL = 19 *)
(* 6 DEATH STATES AGGREGATED INTO STATE 1 *)
(* 0001 WARNING *)
