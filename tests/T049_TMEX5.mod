LAMBDA = 1E-4;
DELTA = 508.55;
RHO =  1 TO* 1E7 BY 10;
GAMMA = 10*LAMBDA;
PHI = DELTA;
T = RHO  + PHI;

1,2= 3*LAMBDA;
2,3 = 2*LAMBDA+2*GAMMA;
2,4 = <1/DELTA,1/DELTA,1.0>;
4,5 = LAMBDA + GAMMA;
1,6 =  3*GAMMA;
6,1 = <1/T,1/T,RHO/T>;
6,4 = <1/T,1/T,PHI/T>;
6,7 = 2*LAMBDA + 2*GAMMA;

