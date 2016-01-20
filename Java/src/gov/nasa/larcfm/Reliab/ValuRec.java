package gov.nasa.larcfm.Reliab;


//   value = base + coef * SPECVAR
public class ValuRec {
 	double base; 
   	double coef;            
	boolean linear;

	ValuRec(double c, double b, boolean l) {
		coef = c;
		base = b;
		linear = l;
	}
	
	ValuRec(ValuRec v) {
   		coef = v.coef;
		base = v.base;
		linear = v.linear;
	}

	public String toString() {
		return "<"+f.Em5(base)+", "+f.Em5(coef)+", "+linear+">";
	}    	    	    	
}
