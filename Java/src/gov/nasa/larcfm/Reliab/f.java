/* 
 * String Formatting functions
 *
 * Contact: Rick Butler (r.w.butler@nasa.gov)
 *
 * Copyright (c) 2011-2014 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 * 
 */

package gov.nasa.larcfm.Reliab;

import java.io.File;
import java.io.FilenameFilter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.List;

import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.filechooser.FileFilter;

/** Various printout operations, used in testing and debugging. */   
public class f {


	/** Given a list of names that may include files or directories,
	 * return a list of files that contains (1) all of the files in
	 * the original list and (2) all files ending with ".txt" in
	 * directories from the original list. */
	public static List<String> getFileNames(String[] names) {
		ArrayList<String> txtFiles = new ArrayList<String>();
		for (int i=0; i < names.length; i++) {
			File file = new File(names[i]);
			if (file.canRead()) {
				if (file.isDirectory()) {
					File[] fs=file.listFiles(new FilenameFilter() {
						public boolean accept(File f, String name) {
							return name.endsWith(".txt");
							//return true;	
						}                       
					}); 
					for (File txtfile:fs) {
						txtFiles.add(txtfile.getPath());
					}
				} else {
					txtFiles.add(file.getPath());
				}
			}
		}
		return txtFiles;
	}


	/** send string to the console with a "carriage return" Also flushes both stdout and stderr */
	public static void pln(String string) {
		System.err.flush();
		System.out.println(string);
		System.out.flush();
	}

	public static void pln() {
		pln("");
	}
	
	/** send a string to the console without a "carriage return" */
	public static void p(String string) {
		System.out.print(string);
	}

	public static double fm_nz(double v, int precision) {
		if (v < 0.0 && Math.ceil(v*Math.pow(10,precision)-0.5) == 0.0) return 0.0;
		return v;
	}

	private static DecimalFormat Frm0 = new DecimalFormat("0");
	private static DecimalFormat Frm1 = new DecimalFormat("0.0");
	private static DecimalFormat Frm2 = new DecimalFormat("0.00");
	private static DecimalFormat Frm3 = new DecimalFormat("0.000");
	private static DecimalFormat Frm4 = new DecimalFormat("0.0000");
	private static DecimalFormat Frm5 = new DecimalFormat("0.00000");
	private static DecimalFormat Frm6 = new DecimalFormat("0.000000");
	private static DecimalFormat Frm7 = new DecimalFormat("0.0000000");
	private static DecimalFormat Frm8 = new DecimalFormat("0.00000000");
	private static DecimalFormat Frm9 = new DecimalFormat("0.000000000");
	private static DecimalFormat Frm10 = new DecimalFormat("0.0000000000");
	private static DecimalFormat Frm11 = new DecimalFormat("0.00000000000");
	private static DecimalFormat Frm12 = new DecimalFormat("0.000000000000");
	private static DecimalFormat Frm13 = new DecimalFormat("0.0000000000000");
	private static DecimalFormat Frm14 = new DecimalFormat("0.00000000000000");
	private static DecimalFormat Frm15 = new DecimalFormat("0.000000000000000");
	private static DecimalFormat Frm16 = new DecimalFormat("0.0000000000000000");

	private static DecimalFormat Erm2 = new DecimalFormat("0.00E00");
	private static DecimalFormat Erm4 = new DecimalFormat("0.0000E00");
	private static DecimalFormat Erm5 = new DecimalFormat("0.00000E00");
	private static DecimalFormat Erm8 = new DecimalFormat("0.00000000E00");

	
	/** Format a double with 1 decimal places */
	public static String Fm0(double v) {
		return Frm0.format(v);
	}

	/** Format a double with 1 decimal places */
	public static String Fm1(double v) {
		return Frm1.format(v);
	}

	/** Format a double with 2 decimal places */
	public static String Fm(double v) {
		return Frm2.format(v);
	}

	/** Format a double with 2 decimal places */
	public static String Fm2(double v) {
		return Frm2.format(v);
	}

	/** Format a double with 2 decimal places */
	public static String Fm3(double v) {
		return Frm3.format(v);
	}

	/** Format a double with 4 decimal places */
	public static String Fm4(double v) {
		return Frm4.format(v);
	}

	/** Format a double with 6 decimal places */
	public static String Fm6(double v) {
		return Frm6.format(v);
	}

	public static String FmPrecision(double v, int prec) {
		switch(prec) {
		case 1 : return Frm1.format(v);
		case 2 : return Frm2.format(v);
		case 3 : return Frm3.format(v);
		case 4 : return Frm4.format(v);
		case 5 : return Frm5.format(v);
		case 6 : return Frm6.format(v);
		case 7 : return Frm7.format(v);
		case 8 : return Frm8.format(v);
		case 9 : return Frm9.format(v);
		case 10: return Frm10.format(v);
		case 11: return Frm11.format(v);
		case 12: return Frm12.format(v);
		case 13: return Frm13.format(v);
		case 14: return Frm14.format(v);
		case 15: return Frm15.format(v);
		case 16: return Frm16.format(v);
		default : return Frm0.format(v);
		}
	}

	/** Format a double with 8 decimal places */
	public static String Fm8(double v) {
		return Frm8.format(v);
	}


	/** Format a double with 8 decimal places */
	public static String Fm12(double v) {
		return Frm12.format(v);
	}

	/** Format a double with 8 decimal places */
	public static String Fm15(double v) {
		return Frm15.format(v);
	}

	
	/** Format a double with 8 decimal places */
	public static String Em2(double v) {
		String sresult = Erm2.format(v);
		if (!sresult.contains("E-")) { //don't blast a negative sign
		    sresult = sresult.replace("E", "E+");
		}		
		return sresult;
	}
	

	/** Format a double with 8 decimal places */
	public static String Em5(double v) {		
		String sresult = Erm5.format(v);
		if (!sresult.contains("E-")) { //don't blast a negative sign
		    sresult = sresult.replace("E", "E+");
		}		
		return sresult;
	}


	/** Format a double with 8 decimal places */
	public static String Im4(int v) {
        return String.format("%4d",v);
	}
	
	/** Format a double with 8 decimal places */
	public static String Im6(int v) {
        return String.format("%6d",v);
	}

	/** Format a double with 8 decimal places */
	public static String Im8(int v) {
        return String.format("%8d",v);
	}

	/** Format a double with 8 decimal places */
	public static String Im12(int v) {
        return String.format("%12d",v);
	}

	
	public static String padLeft(String s, int n) {
		String str = "";
//		try {
	    	str = String.format("%1$" + n + "s", s);  
//		} catch (IllegalArgumentException e) {
//		    str = "      "+s;	
//		}
		return str;
	}

	public static String padRight(String s, int n) {
		String str = "";
//		try {
		    str = String.format("%1$-" + n + "s", s);  
//		} catch (IllegalArgumentException e) {
//		    str = s+"      ";	
//		}
		return str;
	}


	public static String bool2char(boolean b) {
		if (b) return "T"; 
		else return "F";
	}


	public static String bool2str(boolean b) {
		if (b) return "True"; 
		else return "False";
	}

	/** Indent all lines of source string the given number of spaces */
	public static String indent(String s, int i) {
		String tab = "";
		for (int j=0; j < i; j++) tab += " ";
		String s2 = tab+s;
		int j = 0;
		int k = s2.indexOf("\n");
		while (k > j && k < s2.length()-2) {
			s2 = s2.substring(0,k+1)+tab+s2.substring(k+1);
			j = k+i+1;
			k = s2.indexOf("\n",j);
		}
		return s2;
	}
	
	
	static File lastDir = null;
	static JPanel xrt;
	
	// returns filename or "" if no file chosen
	public static String getInputDataFilename() {
		String descr = "Traffic files (.txt, .csv, .pln, .swy)";
		String [] ext = new String[4];
		ext[0] = "txt";
		ext[1] = "csv";
		ext[2] = "swy";
		ext[3] = "pln";
		return getFilenameDialog(false,ext,descr);
	}


	// returns filename or "" if no file chosen
	public static String getFilenameDialog(boolean save, String[] e, String d) {
		final String descr = d;
		final String[] extensions = e;
		JFileChooser fc = new JFileChooser(System.getProperty("user.dir"));
		// load up last directory used
		if (lastDir != null) {
			fc.setCurrentDirectory(lastDir);
		}
		FileFilter filter = new FileFilter(){
			public boolean accept(java.io.File f) {
				if (f.isDirectory()) {
					return true;
				}
				int i = f.getName().lastIndexOf('.');

				String ext = null;
				if (i > 0 &&  i < f.getName().length() - 1) {
					ext = f.getName().substring(i+1).toLowerCase();
				}
				if (ext != null) {
					for (int j = 0; j < extensions.length; j++) {
						if (ext.equals(extensions[j])) return true;
					}
				}
				return false;
			} // accept

			public String getDescription() {
				return descr;
			}
		};
		fc.setAcceptAllFileFilterUsed(true);
		fc.addChoosableFileFilter(filter); // this will be default
		int returnVal;

		if (save) returnVal = fc.showSaveDialog(xrt);
		else returnVal = fc.showOpenDialog(xrt);

		if (returnVal == JFileChooser.APPROVE_OPTION) {
			File f = fc.getSelectedFile();
			lastDir = f.getParentFile();
			return f.getPath();
		} else {
			return "";
		}
	}
	
	public static void halt() {
		Thread.dumpStack();
		System.exit(0);
	}

} 
