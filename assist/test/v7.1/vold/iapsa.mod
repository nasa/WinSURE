LAMADS = 40.0E-6;
LAMMADS = 5.0E-6;
LAMFIADS = 10.0E-6;
REC1ADS = 3.0E-4;
STD1ADS = 1.2E-4;
REC2ADS = 1.0E-4;
STD2ADS = 2.5E-5;
COVADS = 0.98;
LAMC = 10.0E-6;


    2(* 3,0,3 *),    3(* 3,1,3 *) = (3-0)*(LAMADS+0*LAMC);
    2(* 3,0,3 *),    4(* 2,0,2 *) = 3*(LAMFIADS/3.0+LAMC);
    2(* 3,0,3 *),    5(* 3,0,2 *) = 3*LAMMADS;
    3(* 3,1,3 *),    1(* 3,2,3 *) = (3-1)*(LAMADS+1*LAMC);
    3(* 3,1,3 *),    4(* 2,0,2 *) = <REC1ADS,STD1ADS,(3/3)>;
    4(* 2,0,2 *),    6(* 2,1,2 *) = (2-0)*(LAMADS+0*LAMC);
    4(* 2,0,2 *),    7(* 1,0,2 *) = 2*LAMC;
    4(* 2,0,2 *),    8(* 2,0,1 *) = 2*LAMMADS;
    5(* 3,0,2 *),    9(* 3,1,2 *) = (3-0)*(LAMADS+0*LAMC);
    5(* 3,0,2 *),    8(* 2,0,1 *) = 2*(LAMFIADS/3.0+LAMC);
    5(* 3,0,2 *),    4(* 2,0,2 *) = (3-2)*(LAMFIADS/3.0+LAMC);
    5(* 3,0,2 *),   10(* 3,0,1 *) = 2*LAMMADS;
    6(* 2,1,2 *),    1(* 2,2,2 *) = (2-1)*(LAMADS+1*LAMC);
    6(* 2,1,2 *),    7(* 1,0,2 *) = <REC2ADS,STD2ADS,COVADS>;
    6(* 2,1,2 *),    1(* 1,1,2 *) = <REC2ADS,STD2ADS,1.0-COVADS>;
    7(* 1,0,2 *),    1(* 1,1,2 *) = (1-0)*(LAMADS+0*LAMC);
    7(* 1,0,2 *),    1(* 0,0,2 *) = 1*LAMC;
    8(* 2,0,1 *),    1(* 2,1,1 *) = (2-0)*(LAMADS+0*LAMC);
    8(* 2,0,1 *),    7(* 1,0,2 *) = 2*LAMC;
    9(* 3,1,2 *),    1(* 3,2,2 *) = (3-1)*(LAMADS+1*LAMC);
    9(* 3,1,2 *),    8(* 2,0,1 *) = <REC1ADS,STD1ADS,(2/3)>;
    9(* 3,1,2 *),    4(* 2,0,2 *) = <REC1ADS,STD1ADS,(3-2)/3>;
   10(* 3,0,1 *),   11(* 3,1,1 *) = (3-0)*(LAMADS+0*LAMC);
   10(* 3,0,1 *),    8(* 2,0,1 *) = 3*(LAMFIADS/3.0+LAMC);
   11(* 3,1,1 *),    1(* 3,2,1 *) = (3-1)*(LAMADS+1*LAMC);
   11(* 3,1,1 *),    8(* 2,0,1 *) = <REC1ADS,STD1ADS>;

(* NUMBER OF STATES IN MODEL      = 11 *)
(* NUMBER OF TRANSITIONS IN MODEL = 26 *)
(* 8 DEATH STATES AGGREGATED STATES 1 - 1 *)
