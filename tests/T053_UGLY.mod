
      omega = 195.0;
      w = 320.0;
      rho = 15.0e-6;
      S = 2.4e3;
      b=18.2;
      vcomm = 38.4;
      PSI = 170 TO 225.0;
      delta = psi-b;
      R = .104E6;

      C=0.1212E22;
      ALPHA = 0.189E2;


      Nn = 6; m = 0;
      epsip = .5*(delta - rho*(2*s + Nn*R/(Nn-m))) - m*omega/(Nn-m);
      epsiw = [(W-2*rho*S-2*m*omega/(Nn-m)-Nn*rho*R/(Nn-m)-vcomm)]/3.0;
      epsio = [((Nn-3*m)*omega/(Nn-m)-rho*(2*S+Nn*R/(Nn-m)))]/3.0;

      epsi = epsip;
      if epsiw < epsi then epsi=epsiw;
      if epsio < epsi then epsi=epsio;
      if epsi < 0.0    then SHOW EPSI;

      PE = C*(EPSI**(-ALPHA));

      LC60 =  (Nn-1)*PE/(R*1.0E-6/3600.0);
      LC50 =  (Nn-1)*PE/(R*1.0E-6/3600.0);
      LC40 =  (Nn-1)*PE/(R*1.0E-6/3600.0);
      LC30 =  (Nn-1)*PE/(R*1.0E-6/3600.0);
     
      echo=0;

      Nn = 6; m = 1;
      epsip = .5*(delta - rho*(2*s + Nn*R/(Nn-m))) - m*omega/(Nn-m);
      epsiw = [(W-2*rho*S-2*m*omega/(Nn-m)-Nn*rho*R/(Nn-m)-vcomm)]/3.0;
      epsio = [((Nn-3*m)*omega/(Nn-m)-rho*(2*S+Nn*R/(Nn-m)))]/3.0;
      epsi = epsip;
      if epsiw < epsi then epsi=epsiw;
      if epsio < epsi then epsi=epsio;
      if epsi < 0.0    then SHOW EPSI;

      PE = C*(EPSI**(-ALPHA));
      LC61 =  (Nn-1)*PE/(R*1.0E-6/3600.0);

      Nn = 5; m = 1;
      epsip = .5*(delta - rho*(2*s + Nn*R/(Nn-m))) - m*omega/(Nn-m);
      epsiw = [(W-2*rho*S-2*m*omega/(Nn-m)-Nn*rho*R/(Nn-m)-vcomm)]/3.0;
      epsio = [((Nn-3*m)*omega/(Nn-m)-rho*(2*S+Nn*R/(Nn-m)))]/3.0;
      epsi = epsip;
      if epsiw < epsi then epsi=epsiw;
      if epsio < epsi then epsi=epsio;
      if epsi < 0.0    then SHOW EPSI;

      PE = C*(EPSI**(-ALPHA));
      LC51 =  (Nn-1)*PE/(R*1.0E-6/3600.0);

      Nn = 4; m = 1;
      epsip = .5*(delta - rho*(2*s + Nn*R/(Nn-m))) - m*omega/(Nn-m);
      epsiw = [(W-2*rho*S-2*m*omega/(Nn-m)-Nn*rho*R/(Nn-m)-vcomm)]/3.0;
      epsio = [((Nn-3*m)*omega/(Nn-m)-rho*(2*S+Nn*R/(Nn-m)))]/3.0;
      epsi = epsip;
      if epsiw < epsi then epsi=epsiw;
      if epsio < epsi then epsi=epsio;
      if epsi < 0.0    then SHOW EPSI;

      PE = C*(EPSI**(-ALPHA));
LC41 = 0.99505e01;
      LC41 =  (Nn-1)*PE/(R*1.0E-6/3600.0);


      ECHO=1;
      LP = 1.0E-4;         (* processor failure rate *)

      RC = 18.0E3;       (* 3600 rec/hour  or  1 rec takes 1 sec *)
      RP = 18.0E3;

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

POINTS = 10;
