LEE;
1,2 = 3;
SHOW 1;

CLEAR;

RUN;

1,2 = <0,5>;
2,3 = 1E-4;
3,4 = 1E-6;
RUN;

1,2 = <1E-4,3E-4,.6>;
1,3 = <1E-4,3E-4,.400002>;
RUN;

1,2 = 1E-7;
2,5 = 1E-4;
1,99999999 = 1E-5;
5,6 = E;

1,2 = 1.4E-4;;
1,2 = 4E-5;
RUN;

XXX YYY;
AAA = 12;
BBB = AAA + - 12;
CLEAR;

1,3 = <1E-4, 1E-5;
CLEAR;


ABCDE = 123E-18;
1,2 = 1E-12;
2,3 = (ABCDE*SIN(12E-4);
CLEAR;

2,3 = 1E-6;
3,4   1E-7;
CLEAR;


