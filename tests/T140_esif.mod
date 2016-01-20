
psi = 180 TO 260 BY 5;
omega = 210.0;
w = 320.0;


echo = 0;
rho = 15.0e-6;
S = 2.4e3;
b=18.2;
delta = psi-b;
vcomm = 38.4;

R = .104E6;
C=0.1212E22;
ALPHA = 0.189E2;

savep1 = .5*delta - rho*S;
savep2 = .5*rho*R;

savew1 = W - 2*rho*S - vcomm;
savew2 = rho*R;

saveo1 = 2.0*rho*s;
saveo2 = rho*R;

Nn = 6; m = 0;
epsip = savep1 - (Nn*savep2 + m*omega)/(Nn-m);
epsiw = [savew1 - (2.0*m*omega + Nn*savew2)/(Nn-m)]/3.0;
epsio = [(Nn-3.0*m)*omega/(Nn-m)-saveo1-Nn*saveo2/(Nn-m)]/3.0;

epsi = epsip;
if epsiw < epsi then epsi=epsiw;
if epsio < epsi then epsi=epsio;
if epsi < 0.0    then SHOW EPSI;

PE = 1;
IF EPSI > 0.0 THEN PE = C*(EPSI**(-ALPHA));
IF PE > 1.0 THEN PE = 1;

LC60 =  (6-1)*PE/(R*1.0E-6/3600.0);
LC50 =  (5-1)*PE/(R*1.0E-6/3600.0);
LC40 =  (4-1)*PE/(R*1.0E-6/3600.0);
LC30 =  (3-1)*PE/(R*1.0E-6/3600.0);
     
Nn = 6; m = 1;
epsip = savep1 - (Nn*savep2 + m*omega)/(Nn-m);
epsiw = [savew1 - (2.0*m*omega + Nn*savew2)/(Nn-m)]/3.0;
epsio = [(Nn-3*m)*omega/(Nn-m)-saveo1-Nn*saveo2/(Nn-m)]/3.0;
epsi = epsip;
if epsiw < epsi then epsi=epsiw;
if epsio < epsi then epsi=epsio;
if epsi < 0.0    then SHOW EPSI;

PE = 1;
IF EPSI > 0.0 THEN PE = C*(EPSI**(-ALPHA));
IF PE > 1.0 THEN PE = 1;

LC61 =  (Nn-1)*PE/(R*1.0E-6/3600.0);

Nn = 5; m = 1;
epsip = savep1 - (Nn*savep2 + m*omega)/(Nn-m);
epsiw = [savew1 - (2.0*m*omega + Nn*savew2)/(Nn-m)]/3.0;
epsio = [(Nn-3*m)*omega/(Nn-m)-saveo1-Nn*saveo2/(Nn-m)]/3.0;
epsi = epsip;
if epsiw < epsi then epsi=epsiw;
if epsio < epsi then epsi=epsio;
if epsi < 0.0    then SHOW EPSI;

PE = 1;
IF EPSI > 0.0 THEN PE = C*(EPSI**(-ALPHA));
IF PE > 1.0 THEN PE = 1;

LC51 =  (Nn-1)*PE/(R*1.0E-6/3600.0);

Nn = 4; m = 1;
epsip = savep1 - (Nn*savep2 + m*omega)/(Nn-m);
epsiw = [savew1 - (2.0*m*omega + Nn*savew2)/(Nn-m)]/3.0;
epsio = [(Nn-3*m)*omega/(Nn-m)-saveo1-Nn*saveo2/(Nn-m)]/3.0;
epsi = epsip;
if epsiw < epsi then epsi=epsiw;
if epsio < epsi then epsi=epsio;
if epsi < 0.0    then SHOW EPSI;

PE = 1;
IF EPSI > 0.0 THEN PE = C*(EPSI**(-ALPHA));
IF PE > 1.0 THEN PE = 1;

LC41 =  (Nn-1)*PE/(R*1.0E-6/3600.0);

(* ECHO=0; *)
LP = 1.0E-4;         (* processor failure rate *)


RC = 18.0E3;       (* 3600 rec/hour  or  1 rec takes 1 sec *)
RP = 18.0E3;

(*F = 1E-5 TO 1; *)
F=1;

600,610 = 6*(1-F)*LP;
600,601 = 6*(LC60+F*LP);

601,611 = 5*(1-F)*LP;
601,500 = <1/RC,1/RC>;
601,602 = 5*(LC61+F*LP);

610,620 = 5*(1-F)*LP;
610,500 = <1/RP,1/RP>;
610,611 = 5*(LC60+F*LP);

611,621 = 4*(1-F)*LP;
611,612 = 4*(LC61+F*LP);
611,501 = <1/(RP+RC),1/RP,RP/(RP+RC)>;
611,510 = <1/(RP+RC),1/RC,RC/(RP+RC)>;

620,630 = 4*(1-F)*LP;
620,621 = 4*(LC60+F*LP);
620,510 = <1/(2*RP),1/(2*RP)>;

500,510 = 5*(1-F)*LP;
500,501 = 5*(LC50+F*LP);

501,511 = 4*(1-F)*LP;
501,502 = 4*(LC51+F*LP);
501,400 = <1/RC,1/RC>;

510,520 = 4*(1-F)*LP;
510,511 = 4*(LC50+F*LP);
510,400 = <1/RP,1/RP>;

511,521 = 3*(1-F)*LP;
511,512 = 3*(LC51+F*LP);
511,410 = <1/(RP+RC),1/RC,RC/(RP+RC)>;
511,401 = <1/(RP+RC),1/RP,RP/(RP+RC)>;

520,521 = 3*(LC50+F*LP);
520,530 = 3*(1-F)*LP;
520,410 = <1/(2*RP),1/(2*RP)>;

400,410 = 4*(1-F)*LP;
400,401 = 4*(LC40+F*LP);

401,411 = 3*(1-F)*LP;
401,402 = 3*(LC41+F*LP);
401,300 = <1/RC,1/RC>;

410,411 = 3*(LC40+F*LP);
410,420 = 3*(1-F)*LP;
410,300 = <1/RP,1/RP>;

300,310 = 3*(1-F)*LP;
300,301 = 3*(LC30+F*LP);

310,320 = 2*(1-F)*LP;
310,311 = 2*(LC30+F*LP);
310,100 = <1/RP,1/RP>;

100,101 = LP;

ECHO = 1;

(*
 OMEGA =     196.00; PSI =     202.60; W =     320.00;
 LC60 =     0.12143E-07;
 LC61 =     0.22154E-03;
 LC50 =     0.97144E-08;
 LC51 =     0.60170E-02;
 LC40 =     0.72858E-08;
 LC41 =     0.12200E+02;
 LC30 =     0.48572E-08;
 *)

 SHOW LC60,LC61,LC50,LC51,LC40,LC41,LC30;


