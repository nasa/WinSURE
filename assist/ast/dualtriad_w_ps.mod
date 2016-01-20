LAM_PS = 1E-6;
LAM_C = 1E-5;


     3(* F,F,F,F,F,F,F,F,F *),     4(* T,F,F,F,F,F,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),     5(* F,F,F,T,F,F,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),     6(* T,F,F,T,F,F,T,F,F *)
     = LAM_PS;
     3(* F,F,F,F,F,F,F,F,F *),     7(* F,T,F,F,F,F,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),     8(* F,F,F,F,T,F,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),     9(* F,T,F,F,T,F,F,T,F *)
     = LAM_PS;
     3(* F,F,F,F,F,F,F,F,F *),    10(* F,F,T,F,F,F,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),    11(* F,F,F,F,F,T,F,F,F *)
     = LAM_C;
     3(* F,F,F,F,F,F,F,F,F *),    12(* F,F,T,F,F,T,F,F,T *)
     = LAM_PS;
     4(* T,F,F,F,F,F,F,F,F *),    13(* T,F,F,T,F,F,F,F,F *)
     = LAM_C;
     4(* T,F,F,F,F,F,F,F,F *),     6(* T,F,F,T,F,F,T,F,F *)
     = LAM_PS;
     4(* T,F,F,F,F,F,F,F,F *),     1(* T,T,F,F,F,F,F,F,F DEATH  *)
     = LAM_C;
     4(* T,F,F,F,F,F,F,F,F *),    14(* T,F,F,F,T,F,F,F,F *)
     = LAM_C;
     4(* T,F,F,F,F,F,F,F,F *),     1(* T,T,F,F,T,F,F,T,F DEATH  *)
     = LAM_PS;
     4(* T,F,F,F,F,F,F,F,F *),     1(* T,F,T,F,F,F,F,F,F DEATH  *)
     = LAM_C;
     4(* T,F,F,F,F,F,F,F,F *),    15(* T,F,F,F,F,T,F,F,F *)
     = LAM_C;
     4(* T,F,F,F,F,F,F,F,F *),     1(* T,F,T,F,F,T,F,F,T DEATH  *)
     = LAM_PS;
     5(* F,F,F,T,F,F,F,F,F *),    13(* T,F,F,T,F,F,F,F,F *)
     = LAM_C;
     5(* F,F,F,T,F,F,F,F,F *),     6(* T,F,F,T,F,F,T,F,F *)
     = LAM_PS;
     5(* F,F,F,T,F,F,F,F,F *),    16(* F,T,F,T,F,F,F,F,F *)
     = LAM_C;
     5(* F,F,F,T,F,F,F,F,F *),     2(* F,F,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
     5(* F,F,F,T,F,F,F,F,F *),     2(* F,T,F,T,T,F,F,T,F DEATH  *)
     = LAM_PS;
     5(* F,F,F,T,F,F,F,F,F *),    17(* F,F,T,T,F,F,F,F,F *)
     = LAM_C;
     5(* F,F,F,T,F,F,F,F,F *),     2(* F,F,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
     5(* F,F,F,T,F,F,F,F,F *),     2(* F,F,T,T,F,T,F,F,T DEATH  *)
     = LAM_PS;
     6(* T,F,F,T,F,F,T,F,F *),     1(* T,T,F,T,F,F,T,F,F DEATH  *)
     = LAM_C;
     6(* T,F,F,T,F,F,T,F,F *),     2(* T,F,F,T,T,F,T,F,F DEATH  *)
     = LAM_C;
     6(* T,F,F,T,F,F,T,F,F *),     1(* T,T,F,T,T,F,T,T,F DEATH  *)
     = LAM_PS;
     6(* T,F,F,T,F,F,T,F,F *),     1(* T,F,T,T,F,F,T,F,F DEATH  *)
     = LAM_C;
     6(* T,F,F,T,F,F,T,F,F *),     2(* T,F,F,T,F,T,T,F,F DEATH  *)
     = LAM_C;
     6(* T,F,F,T,F,F,T,F,F *),     1(* T,F,T,T,F,T,T,F,T DEATH  *)
     = LAM_PS;
     7(* F,T,F,F,F,F,F,F,F *),     1(* T,T,F,F,F,F,F,F,F DEATH  *)
     = LAM_C;
     7(* F,T,F,F,F,F,F,F,F *),    16(* F,T,F,T,F,F,F,F,F *)
     = LAM_C;
     7(* F,T,F,F,F,F,F,F,F *),     1(* T,T,F,T,F,F,T,F,F DEATH  *)
     = LAM_PS;
     7(* F,T,F,F,F,F,F,F,F *),    18(* F,T,F,F,T,F,F,F,F *)
     = LAM_C;
     7(* F,T,F,F,F,F,F,F,F *),     9(* F,T,F,F,T,F,F,T,F *)
     = LAM_PS;
     7(* F,T,F,F,F,F,F,F,F *),     1(* F,T,T,F,F,F,F,F,F DEATH  *)
     = LAM_C;
     7(* F,T,F,F,F,F,F,F,F *),    19(* F,T,F,F,F,T,F,F,F *)
     = LAM_C;
     7(* F,T,F,F,F,F,F,F,F *),     1(* F,T,T,F,F,T,F,F,T DEATH  *)
     = LAM_PS;
     8(* F,F,F,F,T,F,F,F,F *),    14(* T,F,F,F,T,F,F,F,F *)
     = LAM_C;
     8(* F,F,F,F,T,F,F,F,F *),     2(* F,F,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
     8(* F,F,F,F,T,F,F,F,F *),     2(* T,F,F,T,T,F,T,F,F DEATH  *)
     = LAM_PS;
     8(* F,F,F,F,T,F,F,F,F *),    18(* F,T,F,F,T,F,F,F,F *)
     = LAM_C;
     8(* F,F,F,F,T,F,F,F,F *),     9(* F,T,F,F,T,F,F,T,F *)
     = LAM_PS;
     8(* F,F,F,F,T,F,F,F,F *),    20(* F,F,T,F,T,F,F,F,F *)
     = LAM_C;
     8(* F,F,F,F,T,F,F,F,F *),     2(* F,F,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
     8(* F,F,F,F,T,F,F,F,F *),     2(* F,F,T,F,T,T,F,F,T DEATH  *)
     = LAM_PS;
     9(* F,T,F,F,T,F,F,T,F *),     1(* T,T,F,F,T,F,F,T,F DEATH  *)
     = LAM_C;
     9(* F,T,F,F,T,F,F,T,F *),     2(* F,T,F,T,T,F,F,T,F DEATH  *)
     = LAM_C;
     9(* F,T,F,F,T,F,F,T,F *),     1(* T,T,F,T,T,F,T,T,F DEATH  *)
     = LAM_PS;
     9(* F,T,F,F,T,F,F,T,F *),     1(* F,T,T,F,T,F,F,T,F DEATH  *)
     = LAM_C;
     9(* F,T,F,F,T,F,F,T,F *),     2(* F,T,F,F,T,T,F,T,F DEATH  *)
     = LAM_C;
     9(* F,T,F,F,T,F,F,T,F *),     1(* F,T,T,F,T,T,F,T,T DEATH  *)
     = LAM_PS;
    10(* F,F,T,F,F,F,F,F,F *),     1(* T,F,T,F,F,F,F,F,F DEATH  *)
     = LAM_C;
    10(* F,F,T,F,F,F,F,F,F *),    17(* F,F,T,T,F,F,F,F,F *)
     = LAM_C;
    10(* F,F,T,F,F,F,F,F,F *),     1(* T,F,T,T,F,F,T,F,F DEATH  *)
     = LAM_PS;
    10(* F,F,T,F,F,F,F,F,F *),     1(* F,T,T,F,F,F,F,F,F DEATH  *)
     = LAM_C;
    10(* F,F,T,F,F,F,F,F,F *),    20(* F,F,T,F,T,F,F,F,F *)
     = LAM_C;
    10(* F,F,T,F,F,F,F,F,F *),     1(* F,T,T,F,T,F,F,T,F DEATH  *)
     = LAM_PS;
    10(* F,F,T,F,F,F,F,F,F *),    21(* F,F,T,F,F,T,F,F,F *)
     = LAM_C;
    10(* F,F,T,F,F,F,F,F,F *),    12(* F,F,T,F,F,T,F,F,T *)
     = LAM_PS;
    11(* F,F,F,F,F,T,F,F,F *),    15(* T,F,F,F,F,T,F,F,F *)
     = LAM_C;
    11(* F,F,F,F,F,T,F,F,F *),     2(* F,F,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    11(* F,F,F,F,F,T,F,F,F *),     2(* T,F,F,T,F,T,T,F,F DEATH  *)
     = LAM_PS;
    11(* F,F,F,F,F,T,F,F,F *),    19(* F,T,F,F,F,T,F,F,F *)
     = LAM_C;
    11(* F,F,F,F,F,T,F,F,F *),     2(* F,F,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    11(* F,F,F,F,F,T,F,F,F *),     2(* F,T,F,F,T,T,F,T,F DEATH  *)
     = LAM_PS;
    11(* F,F,F,F,F,T,F,F,F *),    21(* F,F,T,F,F,T,F,F,F *)
     = LAM_C;
    11(* F,F,F,F,F,T,F,F,F *),    12(* F,F,T,F,F,T,F,F,T *)
     = LAM_PS;
    12(* F,F,T,F,F,T,F,F,T *),     1(* T,F,T,F,F,T,F,F,T DEATH  *)
     = LAM_C;
    12(* F,F,T,F,F,T,F,F,T *),     2(* F,F,T,T,F,T,F,F,T DEATH  *)
     = LAM_C;
    12(* F,F,T,F,F,T,F,F,T *),     1(* T,F,T,T,F,T,T,F,T DEATH  *)
     = LAM_PS;
    12(* F,F,T,F,F,T,F,F,T *),     1(* F,T,T,F,F,T,F,F,T DEATH  *)
     = LAM_C;
    12(* F,F,T,F,F,T,F,F,T *),     2(* F,F,T,F,T,T,F,F,T DEATH  *)
     = LAM_C;
    12(* F,F,T,F,F,T,F,F,T *),     1(* F,T,T,F,T,T,F,T,T DEATH  *)
     = LAM_PS;
    13(* T,F,F,T,F,F,F,F,F *),     6(* T,F,F,T,F,F,T,F,F *)
     = LAM_PS;
    13(* T,F,F,T,F,F,F,F,F *),     1(* T,T,F,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    13(* T,F,F,T,F,F,F,F,F *),     2(* T,F,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    13(* T,F,F,T,F,F,F,F,F *),     1(* T,T,F,T,T,F,F,T,F DEATH  *)
     = LAM_PS;
    13(* T,F,F,T,F,F,F,F,F *),     1(* T,F,T,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    13(* T,F,F,T,F,F,F,F,F *),     2(* T,F,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    13(* T,F,F,T,F,F,F,F,F *),     1(* T,F,T,T,F,T,F,F,T DEATH  *)
     = LAM_PS;
    14(* T,F,F,F,T,F,F,F,F *),     2(* T,F,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    14(* T,F,F,F,T,F,F,F,F *),     2(* T,F,F,T,T,F,T,F,F DEATH  *)
     = LAM_PS;
    14(* T,F,F,F,T,F,F,F,F *),     1(* T,T,F,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    14(* T,F,F,F,T,F,F,F,F *),     1(* T,T,F,F,T,F,F,T,F DEATH  *)
     = LAM_PS;
    14(* T,F,F,F,T,F,F,F,F *),     1(* T,F,T,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    14(* T,F,F,F,T,F,F,F,F *),     2(* T,F,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    14(* T,F,F,F,T,F,F,F,F *),     1(* T,F,T,F,T,T,F,F,T DEATH  *)
     = LAM_PS;
    15(* T,F,F,F,F,T,F,F,F *),     2(* T,F,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    15(* T,F,F,F,F,T,F,F,F *),     2(* T,F,F,T,F,T,T,F,F DEATH  *)
     = LAM_PS;
    15(* T,F,F,F,F,T,F,F,F *),     1(* T,T,F,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    15(* T,F,F,F,F,T,F,F,F *),     2(* T,F,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    15(* T,F,F,F,F,T,F,F,F *),     1(* T,T,F,F,T,T,F,T,F DEATH  *)
     = LAM_PS;
    15(* T,F,F,F,F,T,F,F,F *),     1(* T,F,T,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    15(* T,F,F,F,F,T,F,F,F *),     1(* T,F,T,F,F,T,F,F,T DEATH  *)
     = LAM_PS;
    16(* F,T,F,T,F,F,F,F,F *),     1(* T,T,F,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    16(* F,T,F,T,F,F,F,F,F *),     1(* T,T,F,T,F,F,T,F,F DEATH  *)
     = LAM_PS;
    16(* F,T,F,T,F,F,F,F,F *),     2(* F,T,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    16(* F,T,F,T,F,F,F,F,F *),     2(* F,T,F,T,T,F,F,T,F DEATH  *)
     = LAM_PS;
    16(* F,T,F,T,F,F,F,F,F *),     1(* F,T,T,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    16(* F,T,F,T,F,F,F,F,F *),     2(* F,T,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    16(* F,T,F,T,F,F,F,F,F *),     1(* F,T,T,T,F,T,F,F,T DEATH  *)
     = LAM_PS;
    17(* F,F,T,T,F,F,F,F,F *),     1(* T,F,T,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    17(* F,F,T,T,F,F,F,F,F *),     1(* T,F,T,T,F,F,T,F,F DEATH  *)
     = LAM_PS;
    17(* F,F,T,T,F,F,F,F,F *),     1(* F,T,T,T,F,F,F,F,F DEATH  *)
     = LAM_C;
    17(* F,F,T,T,F,F,F,F,F *),     2(* F,F,T,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    17(* F,F,T,T,F,F,F,F,F *),     1(* F,T,T,T,T,F,F,T,F DEATH  *)
     = LAM_PS;
    17(* F,F,T,T,F,F,F,F,F *),     2(* F,F,T,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    17(* F,F,T,T,F,F,F,F,F *),     2(* F,F,T,T,F,T,F,F,T DEATH  *)
     = LAM_PS;
    18(* F,T,F,F,T,F,F,F,F *),     1(* T,T,F,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    18(* F,T,F,F,T,F,F,F,F *),     2(* F,T,F,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    18(* F,T,F,F,T,F,F,F,F *),     1(* T,T,F,T,T,F,T,F,F DEATH  *)
     = LAM_PS;
    18(* F,T,F,F,T,F,F,F,F *),     9(* F,T,F,F,T,F,F,T,F *)
     = LAM_PS;
    18(* F,T,F,F,T,F,F,F,F *),     1(* F,T,T,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    18(* F,T,F,F,T,F,F,F,F *),     2(* F,T,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    18(* F,T,F,F,T,F,F,F,F *),     1(* F,T,T,F,T,T,F,F,T DEATH  *)
     = LAM_PS;
    19(* F,T,F,F,F,T,F,F,F *),     1(* T,T,F,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    19(* F,T,F,F,F,T,F,F,F *),     2(* F,T,F,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    19(* F,T,F,F,F,T,F,F,F *),     1(* T,T,F,T,F,T,T,F,F DEATH  *)
     = LAM_PS;
    19(* F,T,F,F,F,T,F,F,F *),     2(* F,T,F,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    19(* F,T,F,F,F,T,F,F,F *),     2(* F,T,F,F,T,T,F,T,F DEATH  *)
     = LAM_PS;
    19(* F,T,F,F,F,T,F,F,F *),     1(* F,T,T,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    19(* F,T,F,F,F,T,F,F,F *),     1(* F,T,T,F,F,T,F,F,T DEATH  *)
     = LAM_PS;
    20(* F,F,T,F,T,F,F,F,F *),     1(* T,F,T,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    20(* F,F,T,F,T,F,F,F,F *),     2(* F,F,T,T,T,F,F,F,F DEATH  *)
     = LAM_C;
    20(* F,F,T,F,T,F,F,F,F *),     1(* T,F,T,T,T,F,T,F,F DEATH  *)
     = LAM_PS;
    20(* F,F,T,F,T,F,F,F,F *),     1(* F,T,T,F,T,F,F,F,F DEATH  *)
     = LAM_C;
    20(* F,F,T,F,T,F,F,F,F *),     1(* F,T,T,F,T,F,F,T,F DEATH  *)
     = LAM_PS;
    20(* F,F,T,F,T,F,F,F,F *),     2(* F,F,T,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    20(* F,F,T,F,T,F,F,F,F *),     2(* F,F,T,F,T,T,F,F,T DEATH  *)
     = LAM_PS;
    21(* F,F,T,F,F,T,F,F,F *),     1(* T,F,T,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    21(* F,F,T,F,F,T,F,F,F *),     2(* F,F,T,T,F,T,F,F,F DEATH  *)
     = LAM_C;
    21(* F,F,T,F,F,T,F,F,F *),     1(* T,F,T,T,F,T,T,F,F DEATH  *)
     = LAM_PS;
    21(* F,F,T,F,F,T,F,F,F *),     1(* F,T,T,F,F,T,F,F,F DEATH  *)
     = LAM_C;
    21(* F,F,T,F,F,T,F,F,F *),     2(* F,F,T,F,T,T,F,F,F DEATH  *)
     = LAM_C;
    21(* F,F,T,F,F,T,F,F,F *),     1(* F,T,T,F,T,T,F,T,F DEATH  *)
     = LAM_PS;
    21(* F,F,T,F,F,T,F,F,F *),    12(* F,F,T,F,F,T,F,F,T *)
     = LAM_PS;

(* NUMBER OF STATES IN MODEL = 21 *)
(* NUMBER OF TRANSITIONS IN MODEL = 138 *)
(* 102 DEATH STATES AGGREGATED INTO STATES 1 - 2 *)
