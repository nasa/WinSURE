/**
  
Notices:


Copyright 2001 United States Government as represented by the Administrator of the National 
Aeronautics and Space Administration. 
All Rights Reserved.    

Disclaimers


No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANYx WARRANTY OF ANY KIND, 
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


//import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

import javax.swing.ImageIcon;
import javax.swing.JEditorPane;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;


public class vSURE extends JFrame{

	public static JEditorPane editorPane;
	
	private static final Color lightYellow = new Color(255,255,210);
	private static File lastDir = null;
	private static JPanel xrt;
	static public vParams Pframe;

	public void vSURE() {
		setTitle("Simple example");
		setSize(300, 200);
		setLocationRelativeTo(null);
		setDefaultCloseOperation(EXIT_ON_CLOSE); 
	}

	public static void main(String[] args) {
		//f.pln("SURE V8.0");
		Sure.gui_active = true;
		Front.startup();
		JFrame frame = new JFrame("jSURE");

		frame.addWindowListener(new WindowAdapter() {  
			public void windowClosing(WindowEvent e){  
				System.exit(0);
			}
//			public void windowActivated(WindowEvent e){
//				lastTimeActivated = System.currentTimeMillis();
//			}
		} );
		
        frame.setSize(1300, 900);
        
        // Creates a menubar for a JFrame
        JMenuBar menuBar = new JMenuBar();
        
        // Add the menubar to the frame
        frame.setJMenuBar(menuBar);
        
//		pframe = new SureParameterFrame();
		Pframe = new vParams();

        
        /** MENUS */
        
        //File Menu
        JMenu fileMenu = new JMenu("File");
        menuBar.add(fileMenu);
        JMenuItem openAction = new JMenuItem("Open");
        fileMenu.add(openAction);
        openAction.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
            	String fileName = getFilenameDialog("Input File For Sure", false);
            	if (fileName.isEmpty()) {
            		System.out.println("No file selected!");
            	} else {
            		Sure.runSure(fileName);
					vSURE.editorPane.setText(Sure.editorPaneContent);
            	}   		
            }
        });  
        

        //Edit Menu
        JMenu editMenu = new JMenu("Edit");
        menuBar.add(editMenu);
        
        //Plot Menu
        JMenu plotMenu = new JMenu("Plot");
        menuBar.add(plotMenu);
        
        //Defaults Menu
        JMenu defaultsMenu = new JMenu("Defaults");
        menuBar.add(defaultsMenu);
      
		JPanel panel = new JPanel();
		panel.setLayout(new FlowLayout());

		ImageIcon imageIcon = new ImageIcon("meatball_no_back.GIF");
		Image image = imageIcon.getImage(); // transform it 
		Image newimg = image.getScaledInstance(60, 50,  java.awt.Image.SCALE_SMOOTH); // scale it the smooth way  		
		imageIcon = new ImageIcon(newimg);  // transform it back
		JLabel pic_label = new JLabel("", imageIcon, JLabel.CENTER);
		panel.add(pic_label);
		
		JLabel winSURE = new JLabel("WinSure 1.4                                        ");
	    winSURE.setFont(new Font("Courier New", Font.ITALIC, 30));
	    
	    winSURE.setForeground(Color.GRAY);

	    panel.add(winSURE);
		
		JButton ModelButton = new JButton();
		ModelButton.setText("Model");		
		panel.add(ModelButton);	
		ModelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String fileName = getFilenameDialog("Input File For Sure", false);
				if (fileName.isEmpty()) {
					System.out.println("No file selected!");
				} else {
					if (Front.openFile(fileName)) {
						//f.pln(" $$$$$$$$$ readSureFile fileName = "+fileName);
						Front.execSURE();
						vParams.update();
						vSURE.editorPane.setText(Sure.editorPaneContent);
					}  else {
						  f.pln(" $$ ERROR: openFile failed!");  
				  }
				}   		
			}
		});  

		
		JButton ParametersButton = new JButton();
		ParametersButton.setText("Parameters");	
		ParametersButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				//pframe.setVisible(true);
				Pframe.setVisible(true);
				//pframe.update(getParameters());
				//Pframe.update();
			}
		});
		panel.add(ParametersButton);		


		JButton SolveButton = new JButton();
		SolveButton.setText("Solve");		
		panel.add(SolveButton);
		SolveButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Front.runit();  
				vSURE.editorPane.setText(Sure.editorPaneContent);
			}
		});  

		
		JButton ClearButton = new JButton();
		ClearButton.setText("Clear");		
		ClearButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Sure.clear();
				Front.clear();
				Sure.editorPaneContent = "";
				vSURE.editorPane.setText(Sure.editorPaneContent);
			}
		});  
		panel.add(ClearButton);		
		
//		JButton OrProbButton = new JButton();
//		OrProbButton.setText("OrProb");
//		OrProbButton.addActionListener(new ActionListener() {
//			public void actionPerformed(ActionEvent e) {				
//				Front.orprob();				
//				JOptionPane.showMessageDialog(null, "Plot capability not yet implemented");				
//			}
//		});  						
//		panel.add(OrProbButton);		
		
		JButton PlotButton = new JButton();
		PlotButton.setText("Plot");	
		PlotButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(null, "Plot capability not yet implemented");				
			}
		});  						
		panel.add(PlotButton);		

		
		
		JButton DisplayButton = new JButton();
		DisplayButton.setText("Display");	
		DisplayButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(null, "Display capability not yet implemented");				
			}
		});  						

		panel.add(DisplayButton);
				
		editorPane = new JEditorPane();
		editorPane.setEditable(false);
		editorPane.setFont(new Font("monospaced", Font.PLAIN, 14));
		
		//Put the editor pane in a scroll pane.
		JScrollPane editorScrollPane = new JScrollPane(editorPane);
		editorScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		editorScrollPane.setPreferredSize(new Dimension(1200, 800));
		editorScrollPane.setMinimumSize(new Dimension(400, 400));
		panel.add(editorScrollPane);

		
		
//		JButton DirectoryButton = new JButton();
//		DirectoryButton.setText("Directory");		
//		panel.add(DirectoryButton);		

//		JButton PrintButton = new JButton();
//		PrintButton.setText("Print");		
//		panel.add(PrintButton);


//		JButton CreateButton = new JButton();
//		CreateButton.setText("Create");		
//		panel.add(CreateButton);				

		// ************* DEBUG ***********
		
		
		//editorPane.setText(" ########################### SAMPLE0 ################################");
		//editorPane.setText(" ########################### SAMPLE1 ################################");
		
		
		
		frame.add(panel);
		

		panel.setBackground(lightYellow);
		
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);	
		
		frame.addWindowListener(new WindowAdapter() {  
			public void windowClosing(WindowEvent e){  
				System.exit(0);
			}
		} );

		
	}// main

	
	
	private static final FileFilter main_filter = new FileNameExtensionFilter("SURE input files (.mod, .MOD)", "mod", "MOD");

	
	/**
	 * Open a dialog box to choose a file.  The title of the box will be "open" or "save," depending on 
	 * the value of the "save" parameter.
	 * @param save if true open a "save" dialog box, if false open a "open" dialog box
	 * @param ext the list of extensions to filter results 
	 * @param description a description of the extensions using for filtering
	 * @return the chosen filename or "" if no file chosen
	 */
	public static String getFilenameDialog(String title, boolean save) {
		FileFilter[] ff = new FileFilter[1];
		ff[0] = main_filter;
		return getFilenameDialog(title,save,ff);
	}

	
	/**
	 * Open a dialog box to choose a file.  
	 * @param title the title of this dialog box
	 * @param save if true open a "save" dialog box, if false open a "open" dialog box
	 * @param ff	an array of file filters, the 0 index filter will be set as the default
	 * @return the chosen filename or "" if no file chosen
	 */
	public static String getFilenameDialog(String title, boolean save, FileFilter[] ff) {	
		JFileChooser fc = new JFileChooser(System.getProperty("user.dir"));
		if ( ! title.equals("")) {
			fc.setDialogTitle(title);
		}
		
		// load up last directory used
		if (lastDir != null) {
			fc.setCurrentDirectory(lastDir);
		}
		fc.setAcceptAllFileFilterUsed(true);  // if false, then the "all files" option is turned off
		for (FileFilter filter: ff) {
			fc.addChoosableFileFilter(filter); 			
		}
		int returnVal;
		
		if (ff.length > 0) {
			fc.setFileFilter(ff[0]);
		}

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
	
//	public static void setParameters(ParameterData params) {
//		// Execution control parameters
//		if (params.contains("listlevel")) {
//			Front.listlevel = (int) params.getValue("listlevel");
//			f.pln(" $$$$$$ Front.listlevel = "+Front.listlevel);
//		}		
//		if (params.contains("qtcalc")) {
//			Front.qtcalc = (int) params.getValue("qtcalc");
//			f.pln(" $$$$$$ Front.qtcalc = "+Front.qtcalc);
//		}
//		if (params.contains("ptrunc")) {
//			Front.ptrunc = (int) params.getValue("ptrunc");
//			f.pln(" $$$$$$ Front.ptrunc = "+Front.ptrunc);
//		}
//		if (params.contains("warndig")) {
//			Front.warndig = (int) params.getValue("warndig");
//			f.pln(" $$$$$$ Front.warndig = "+Front.warndig);
//		}
//		if (params.contains("rectype")) {
//			Front.rectype = (int) params.getValue("rectype");
//			f.pln(" $$$$$$ Front.rectype = "+Front.rectype);
//		}		
//		if (params.contains("autofast")) {
//			Front.autofast = (int) params.getValue("autofast");
//			f.pln(" $$$$$$ Front.autofast = "+Front.autofast);
//		}
//		if (params.contains("autoprune")) {
//			Front.autoprune = (int) params.getValue("autoprune");
//			f.pln(" $$$$$$ Front.autoprune = "+Front.autoprune);
//		}
//		if (params.contains("notrunc")) {
//			Front.notrunc = (int) params.getValue("notrunc");
//			f.pln(" $$$$$$ Front.notrunc = "+Front.notrunc);
//		}		
//	}

	
	
//	public static ParameterData getParameters() {
//		ParameterData params = new ParameterData();
//
//		// Execution control parameters
//		params.setInternal("listlevel", Front.listlevel, "unitless");    /* list level */
//		params.setInternal("qtcalc", Front.qtcalc, "unitless");          /* q(t) technique */
//		//params.setInternal("start", Front.start, "unitless");            /* start state */
//		params.setInternal("ptrunc", Front.ptrunc, "unitless");          /* trunc level */
//		params.setInternal("warndig", Front.warndig, "unitless");        /* prune warn dig. */
//
//		params.setInternal("rectype", Front.rectype, "unitless");        /* (white = 0,lee =1) */ 
//		//params.setInternal("startstate", Front.startstate, "unitless");  /* start state */
//		params.setInternal("autofast", Front.autofast, "unitless");      /* AUTOFAST constant value */
//		params.setInternal("autoprune", Front.autoprune, "unitless");    /* AUTOPRUNE constant value */
//		params.setInternal("notrunc", Front.notrunc, "unitless");       /* NOTRUNC constant value */
//
//		f.pln(" $$$$$ getParametersFront.qtcalc = "+Front.qtcalc);
//
//		return params;
//	}


}
