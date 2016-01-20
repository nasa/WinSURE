package gov.nasa.larcfm.Reliab;


class  Initialrec {
	int st; //  statetypes;
	NumRec pbl;
	NumRec pbu;

	public Initialrec(int st, NumRec pbl, NumRec pbu) {
		super();
		this.st = st;
		this.pbl = pbl;
		this.pbu = pbu;
	}

	public String toString() {
		return "initlrec [st=" + st + ", pbl=" + pbl + ", pbu=" + pbu + "]";
	}
}
