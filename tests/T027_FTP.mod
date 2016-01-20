LAMBDA = 1E-4;                    (* Failure rate of processor *)
MU = 1E-5 TO* 1E-1;               (* Mean reconfiguration time *)
SIGMA = 1E-4;                     (* Standard deviation of " " *)

1,2 = 4*LAMBDA;
2,3 = 3*LAMBDA;
2,4 = <MU,SIGMA>;
4,5 = 3*LAMBDA;
5,6 = 2*LAMBDA;
5,7 = <MU,SIGMA>;
7,8 = LAMBDA;
POINTS=25;
