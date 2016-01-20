A = 1;
B = 2;
C = 3;
D = 4;
E = 5;
X = 1 TO 10 BY 1;
1,2 = 1/(X-5);
RUN;

A = 1;
B = 2;
C = 3;
D = 4;
E = 5;
X = 1 TO 10 BY 1;
1,2 = SIN(X-5);
RUN;

ORPROB;

A = 1;
B = 2;
C = 3;
D = 4;
E = 5;
X = 1 TO 11 BY 1;
1,2 = SIN(X-5);
RUN;

ORPROB;


READ ERR1;

1,2 = 1E-4;
2,1 = 1E-4;
TIME = 1E6;
ETCALC= 0;
RUN;

A_A_A_A_A_A_A_A_A = 1;
1,2 = A_A_A_A_A_A_A_A_A*1E-6;
2,3 = A_A_A_A_A_A_A_A_A*1E-6;

