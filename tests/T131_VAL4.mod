a = 1.003;
b = 2.9803; 
exp_ab = exp(-a*b);
rho = -exp_ab*[b/a + 1/(a**2)] + 1/a**2;
mu = -exp_ab*[b**2/a+2*b/a**2+1/a**3] + 1/a**3;
sig = -exp_ab*[b**3/a + 3*b**2/a**2 + 6*b/a**3 + 6/a**4] + 6/a**4;
show rho,mu,sig;
1,2 = fast a;
1,3 = <mu,sig,rho>;
