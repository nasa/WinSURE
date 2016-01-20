/**
  
Notices:


Copyright 2001 United States Government as represented by the Administrator of the National 
Aeronautics and Space Administration. 
All Rights Reserved.    

Disclaimers


No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, 
EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT
 THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT 
SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO 
THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY 
GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE 
PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, 
GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF 
PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES 
GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  
IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES
OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, 
RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED 
STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE 
EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, 
UNILATERAL TERMINATION OF THIS AGREEMENT.

*/


package gov.nasa.larcfm.Reliab;

import java.io.*;
import java.util.*;

public class bSURE {
	
	static boolean showFileNames = false;

	
    /** Given a list of names that may include files or directories,
     * return a list of files that contains (1) all of the files in
     * the original list and (2) all files ending with ".txt" in
     * directories from the original list. */
    static List<String> getFileNames(String[] names) {
        ArrayList<String> txtFiles = new ArrayList<String>();
        for (int i=0; i < names.length; i++) {
            File file = new File(names[i]);
            if (file.canRead()) {
                if (file.isDirectory()) {
                    File[] fs=file.listFiles(new FilenameFilter() {
                            public boolean accept(File f, String name) {
                                return name.endsWith(".txt");
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


	public static void main(String[] args) {
		//			try {
		//			   fin = new File(dir.getCanonicalPath() + File.separator + "in.txt");
		//			   fis = new FileInputStream(fin);
		//			} catch (IOException x) {
		//			    System.err.format("IOException: %s%n", x);
		//			}
		//Sure.progname();
		Sure.println("");
		if (args.length == 0) {
			//printHelpMsg();
			//System.exit(1);
		}
		int i = 0;
		while (i < args.length && (args[i].indexOf("--") == 0 || args[i].indexOf("-") == 0)) {
			if (args[i].equals("--verbose")) {
				//verbose = true;
			} else if (args[i].equals("--filenames")) {
				   showFileNames = true;
			} else if (args[i].equals("--run")) {
				   Sure.autorun = true;
			} else if (args[i].equals("-r")) {
				   Sure.autorun = true;
			}
			args[i] = "";
			i++;
		}	
		//f.pln(" $$ showFileNames = "+showFileNames);
		List<String> txtFiles = getFileNames(args);
		//Front.command_line_files = true;
		if (txtFiles.size() == 0) {                   // for debugging in Eclipse
			Front.startup();
			Sure.runSure("");
		} else {
			java.util.Date date = new java.util.Date();
			for (String filename:txtFiles) {
				//double startTimeMs = System.currentTimeMillis( );
				if (showFileNames)
					System.out.println(filename + " ----------------------"+date.getTime());
				else
					System.out.println(filename + " ----------------------");
				Front.startup();
				Sure.runSure(filename);
				Front.eofFlag = false;
				//double execTime = (System.currentTimeMillis() - startTimeMs);
				//f.pln(" $$>> "+filename+" execTime = "+execTime);
			}
		}
		if (Front.listlevel >= 3) {
			//f.pln(" %%%%%%%%% CLOSE initp");
			Front.initp.close();
		}

	  }// main

	
}
