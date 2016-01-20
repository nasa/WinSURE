LAMBDA = 1E-4;
K = 10.0;
GAMMA = K*LAMBDA;
ALPHA = 2; (* Weibull shape parameter, 2, 1, .5 *)
IF ALPHA = 0.5 THEN GFACT = 2;
IF ALPHA = 1 THEN GFACT = 1;
IF ALPHA = 2 THEN GFACT = 0.5*SQRT(3.14159265);
IF ALPHA = 5 THEN GFACT = 5*4*3*2;
MU_W = 1E-9 TO* 1E-1 BY 10;
PHI = [GFACT/MU_W]**ALPHA;
ECHO =0;

W0 = 0.0;
W1 = 1E-6;
W2 = 3.16E-6;
W3 = 1E-5;
W4 = 31.62E-6;
W5 = 100.0E-6;
W6 = 316.22E-6;
W7 = 1.0E-3;
W8 = 3.162E-3;
W9 = 10E-3;
W10 = 31.62E-3;
W11 = 100E-3;
W12 = 316.22E-3;
W13 = 1000.0E-3;


ERW0 = 239.0;
ERW1 = 239.0;  (* millisecs *)
ERW2 = 350.889;
ERW3 = 239.445;
ERW4 = 255.875;
ERW5 = 247.150;
ERW6 = 238.333;
ERW7 = 284.320;
ERW8 = 256.916; 
ERW9 = 249.111; 
ERW10 = 248.444;
ERW11 = 255.778;
ERW12 = 251.500; 
ERW13 = 236.700;

ER2W0 = 57282.602;
ER2W1 = 57282.602;
ER2W2 = 181682.000;
ER2W3 = 58599.273;
ER2W4 = 68679.125;
ER2W5 = 62566.750;
ER2W6 = 57448.199;
ER2W7 = 108444.447;
ER2W8 = 69975.867; 
ER2W9 = 62921.777; 
ER2W10 = 62708.109;
ER2W11 = 66016.664;
ER2W12 = 64170.500; 
ER2W13 = 56681.301;


EZ2W0 = .160;
EZ2W1 = .160;
EZ2W2 = 92402.336;
EZ2W3 = 0.0;
EZ2W4 = 15875.00;
EZ2W5 = 0.0;
EZ2W6 = 40.00;
EZ2W7 = 0.0;
EZ2W8 = 0.0;
EZ2W9 = 0.0;
EZ2W10 = 79524.00;
EZ2W11 = 9801.00;
EZ2W12 = 0.0;
EZ2W13 = 0.0;

EZW0 = 0.0;
EZW1 = 0.1;
EZW2 = 66.333;
EZW3 = 0.0;
EZW4 = 47.286;
EZW5 = 0.0;
EZW6 = 2.462;
EZW7 = 0.0;
EZW8 = 0.0; 
EZW9 = 0.0; 
EZW10 = 282.000;
EZW11 = 99.000;
EZW12 = 0.0; 
EZW13 = 0.0;


PRW0 = 0.0;
PRW1 = .17;
PRW2 = .30;
PRW3= .37;
PRW4= .53;
PRW5 = .69;
PRW6= .54;
PRW7 = .86;
PRW8 = 1.00;
PRW9 = 1.00;
PRW10 = .95;
PRW11 = .90;
PRW12 = 1.00;
PRW13 = 1.00;

FW0 = 1-EXP(-PHI*W0**ALPHA);
FW1 = 1-EXP(-PHI*W1**ALPHA);
FW2 = 1-EXP(-PHI*W2**ALPHA);
FW3 = 1-EXP(-PHI*W3**ALPHA);
FW4 = 1-EXP(-PHI*W4**ALPHA);
FW5 = 1-EXP(-PHI*W5**ALPHA);
FW6 = 1-EXP(-PHI*W6**ALPHA);
FW7 = 1-EXP(-PHI*W7**ALPHA);
FW8 = 1-EXP(-PHI*W8**ALPHA);
FW9 = 1-EXP(-PHI*W9**ALPHA);
FW10 = 1-EXP(-PHI*W10**ALPHA);
FW11 = 1-EXP(-PHI*W11**ALPHA);
FW12 = 1-EXP(-PHI*W12**ALPHA);
FW13 = 1-EXP(-PHI*W13**ALPHA);

SHOW FW1,FW2,FW12,FW13;

XXX =   ( FW1  - FW0 ) +
        ( FW2  - FW1 ) +
        ( FW3  - FW2 ) +
        ( FW4  - FW3 ) +
        ( FW5  - FW4 ) +
        ( FW6  - FW5 ) +
        ( FW7  - FW6 ) +
        ( FW8  - FW7 ) +
        ( FW9  - FW8 ) +
        ( FW10 - FW9 ) +
        ( FW11 - FW10 ) +
        ( FW12 - FW11 ) +
        ( FW13 - FW12 ) ;

CALC XXX;

MU_R = 
        ( FW1  - FW0 ) * ERW1 +
        ( FW2  - FW1 ) * ERW2 +
        ( FW3  - FW2 ) * ERW3 +
        ( FW4  - FW3 ) * ERW4 +
        ( FW5  - FW4 ) * ERW5  +
        ( FW6  - FW5 ) * ERW6  +
        ( FW7  - FW6 ) * ERW7  +
        ( FW8  - FW7 ) * ERW8  +
        ( FW9  - FW8 ) * ERW9  +
        ( FW10 - FW9 ) * ERW10  +
        ( FW11 - FW10 ) * ERW11  +
        ( FW12 - FW11 ) * ERW12  +
        ( FW13 - FW12 ) * ERW13 ;


SIGMA_R = SQRT( 
        ( FW1  - FW0 ) * ER2W1 +
        ( FW2  - FW1 ) * ER2W2 +
        ( FW3  - FW2 ) * ER2W3 +
        ( FW4  - FW3 ) * ER2W4 +
        ( FW5  - FW4 ) * ER2W5  +
        ( FW6  - FW5 ) * ER2W6  +
        ( FW7  - FW6 ) * ER2W7  +
        ( FW8  - FW7 ) * ER2W8  +
        ( FW9  - FW8 ) * ER2W9  +
        ( FW10 - FW9 ) * ER2W10  +
        ( FW11 - FW10 ) * ER2W11  +
        ( FW12 - FW11 ) * ER2W12  +
        ( FW13 - FW12 ) * ER2W13  - MU_R*MU_R);

MU_Z = 
        ( FW1  - FW0 ) * EZW1 +
        ( FW2  - FW1 ) * EZW2 +
        ( FW3  - FW2 ) * EZW3 +
        ( FW4  - FW3 ) * EZW4 +
        ( FW5  - FW4 ) * EZW5  +
        ( FW6  - FW5 ) * EZW6  +
        ( FW7  - FW6 ) * EZW7  +
        ( FW8  - FW7 ) * EZW8  +
        ( FW9  - FW8 ) * EZW9  +
        ( FW10 - FW9 ) * EZW10  +
        ( FW11 - FW10 ) * EZW11  +
        ( FW12 - FW11 ) * EZW12  +
        ( FW13 - FW12 ) * EZW13 ;

       		
SIGMA_Z = SQRT( 
        ( FW1  - FW0 ) * EZ2W1 +
        ( FW2  - FW1 ) * EZ2W2 +
        ( FW3  - FW2 ) * EZ2W3 +
        ( FW4  - FW3 ) * EZ2W4 +
        ( FW5  - FW4 ) * EZ2W5  +
        ( FW6  - FW5 ) * EZ2W6  +
        ( FW7  - FW6 ) * EZ2W7  +
        ( FW8  - FW7 ) * EZ2W8  +
        ( FW9  - FW8 ) * EZ2W9  +
        ( FW10 - FW9 ) * EZ2W10  +
        ( FW11 - FW10 ) * EZ2W11  +
        ( FW12 - FW11 ) * EZ2W12  +
        ( FW13 - FW12 ) * EZ2W13  - MU_Z*MU_Z);


P_R = 
        ( FW1  - FW0 ) * PRW1 +
        ( FW2  - FW1 ) * PRW2 +
        ( FW3  - FW2 ) * PRW3 +
        ( FW4  - FW3 ) * PRW4 +
        ( FW5  - FW4 ) * PRW5  +
        ( FW6  - FW5 ) * PRW6  +
        ( FW7  - FW6 ) * PRW7  +
        ( FW8  - FW7 ) * PRW8  +
        ( FW9  - FW8 ) * PRW9  +
        ( FW10 - FW9 ) * PRW10  +
        ( FW11 - FW10 ) * PRW11  +
        ( FW12 - FW11 ) * PRW12  +
        ( FW13 - FW12 ) * PRW13 ;

SHOW P_R, MU_R, SIGMA_R, MU_Z, SIGMA_Z;

MU_RP = 272.6;
SIGMA_RP = 121.5;

(* convert to hours *)

MS_PER_HOUR = 1E3*60*60;
MU_R = MU_R/MS_PER_HOUR ;
SIGMA_R = SIGMA_R /MS_PER_HOUR;
MU_Z = MU_Z/MS_PER_HOUR;
SIGMA_Z = SIGMA_Z/MS_PER_HOUR;
MU_RP= MU_RP/MS_PER_HOUR;
SIGMA_RP = SIGMA_RP/MS_PER_HOUR;

SHOW MU_R,SIGMA_R,MU_Z,SIGMA_Z,MU_RP,SIGMA_RP;

1,2 = 4*GAMMA;
2,3 = 3*GAMMA + 3*LAMBDA;
1,4 = 4*LAMBDA;
4,5 = 3*GAMMA;
2,5 = 3*LAMBDA;
4,6 = 3*LAMBDA + 3*GAMMA;
2,7 = <MU_R, SIGMA_R, P_R>;
2,1 = <MU_Z, SIGMA_Z, 1-P_R>;
4,7 = <MU_RP, SIGMA_RP>;
7,8 = 3*GAMMA;
8,9 = 2*GAMMA + 2*LAMBDA;
7,10  = 3*LAMBDA; 
10,12 = 2*LAMBDA + 2*GAMMA;
10,11 = 2*GAMMA;
8,12  = 2*LAMBDA;
8,13 = <MU_R, SIGMA_R, P_R>;
10,13 = <MU_RP, SIGMA_RP>;
8,7 =  <MU_Z, SIGMA_Z, 1-P_R>;
13,14 = GAMMA + LAMBDA;
