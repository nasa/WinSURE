
L1 = 1E-4; L2 = 5E-4; A = 3E-4;
UF1 = 1E-4; SF1 = UF1;
UF2 = 1E-2; SF2 = UF2;

1,2 = L1+L2;
2,3 = A;
UG = (L1*UF1+L2*UF2)/(L1+L2);
SG = SQRT( (L1*SF1*SF1+L2*SF2*SF2)/(L1+L2) );
2,5 = <UG,SG>;
