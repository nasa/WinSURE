a = 1.7e-5;
b = 1.3e-4; 

rf = 1-(a/b);
uf = a;
sf = 0.0;

rg = a/b;
ug = a/2;
sg = a*a/12;
sg = sg**0.5;

1,2 = <uf,sf,rf>;
1,3 = <ug,sg,rg>;

p1 = rf; 
p2 = rg;
show p1; show p2;

list=2;
