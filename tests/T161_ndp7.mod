LAMBDA = 1E-4;
MU_REC = 2E-5;

1,2 = 3*LAMBDA;
2,3 = 2*LAMBDA;
2,4 = FAST 1/MU_REC;
4,5 = LAMBDA;
LIST=3;
INITIAL_PROBS(1:       9.99400E-01,
              2:       1.14180E-06,
              3:       2.40192E-12,
              4:       6.00391E-04,
              5:       6.00511E-08);

TIME=2;
