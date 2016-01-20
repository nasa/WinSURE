package gov.nasa.larcfm.Reliab;


class NumRec {
	double base;
	double coef;


	NumRec(double b, double c) {
		base = b;
		coef = c;
	}

	public String toString() {
		return "["+f.Em2(base)+", "+f.Em2(coef)+"]";
	}
	
   double cmp() { /* specval = 0 if (no variable is defined */
	    return base + coef*Front.specval;
   } 

}


