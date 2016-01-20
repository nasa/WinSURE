 a = 1e-5;
b = 1e-4; 

test = (b-a)*0.5;

rf = 1-(a/(2*b));
uf = a*[3*b-2*a]/[3*(2*b-a)];
sf = a*a*[4*b-3*a]/[6*(2*b-a)] - uf*uf;
sf = sf**0.5;
rg = a/(2*b);
ug = a/3;
sg = (a*a)/6 - ug*ug;
sg = sg**0.5;

1,2 = <uf,sf,rf>;
1,3 = <ug,sg,rg>;
show uf; show sf; show rf;
show ug; show sg; show rg;

p1 = rf;
p2 = rg;
show p1; show p2;
list=2;
