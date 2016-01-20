package gov.nasa.larcfm.Reliab;

class HistoryTrans {
	int st;
	Sure.Tranclass cl;
	double lam_alpha_fract;
	double gam_muh;
	
	public HistoryTrans(int st, Sure.Tranclass cl, double lam_alpha_fract, double gam_muh) {
		super();
		this.st = st;
		this.cl = cl;
		this.lam_alpha_fract = lam_alpha_fract;
		this.gam_muh = gam_muh;
	}
	
	public String toString()  {
		return "<"+st+" "+cl+" "+f.Em5(lam_alpha_fract)+" "+f.Em5(gam_muh)+">";
	}
}
