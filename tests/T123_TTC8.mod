A = 1E-3;
MU = 1E-4;
P = 0.1 TO .9 BY .1;
TRUNC = 25;
Q = 1E-6;

0,1 = <MU,MU,1-Q>;
0,5 = <MU,MU,Q>;
1,2 = 1E-4;
2,5 = <MU,MU,P>;
2,4 = <MU,MU,1-P>;
5,2 = <MU,MU>;
2,3 = A;
