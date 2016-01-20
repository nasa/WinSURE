package gov.nasa.larcfm.Reliab;


import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

//import static gov.nasa.larcfm.Util.UnitSymbols.*;

class vParams extends JFrame {    

	private static final long serialVersionUID = 1L;

	static JTextField Jqtcalc = new JTextField(6);
	static JTextField Jlistlevel = new JTextField(6);
	static JTextField Jptrunc = new JTextField(6);
	static JTextField Jtime = new JTextField(6); 
	static JTextField Jautofast = new JTextField(6);
	static JTextField Jautoprune = new JTextField(6);
	static JTextField Jnotrunc = new JTextField(6);
	static JTextField Jwarndig = new JTextField(6); 
	static JTextField Jstart = new JTextField(6); 
	static JTextField Jprune = new JTextField(6);
	static JScrollPane scroll;

	public vParams() {  
		setTitle("Parameters");
		setSize(800,500);
		
		Font bigFont = new Font("SansSerif", Font.BOLD, 15);
		
		
		addWindowListener(new WindowAdapter() {  
			public void windowClosing(WindowEvent e) {  
				//System.exit(0);
			}
		} );
		JPanel contentPane = new JPanel();
		scroll = new JScrollPane(contentPane);
		add(scroll);

		contentPane.setLayout(new GridBagLayout() );
		GridBagConstraints c = new GridBagConstraints();


		// ROW 0

		Jqtcalc = new JTextField(6);
		Jqtcalc.setFont(bigFont);
		Jqtcalc.requestFocus(); 
		Jqtcalc.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jqtcalc.getText());	
				//f.pln("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ JFDd.addActionListener val = "+val);
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("QTCALC",lval);
				f.pln(" $$$$$ vParams: ENTER newid = QTCALC,  lval = "+lval);								
			}// actionPerformed
		}
				);
		c.gridx = 1;    
		c.gridy = 1;
		JLabel Lqtcalc = new JLabel("QTCALC    ",JLabel.RIGHT);
		Lqtcalc.setFont(bigFont);
		contentPane.add(Lqtcalc,c);
		c.gridx = 2;
		c.gridy = 1;
		contentPane.add(Jqtcalc,c);
		Jqtcalc.setText(""+(int) (Front.getdef("QTCALC",Front.defqtcalc)));

		
		Jlistlevel = new JTextField(6);
		Jlistlevel.setFont(bigFont);
		Jlistlevel.requestFocus(); 
		Jlistlevel.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jlistlevel.getText());
				Front.listlevel = val;
				f.pln(" $$$$ Front.listlevel = "+Front.listlevel);
			}// actionPerformed
		}
				);
		c.gridx = 3;
		c.gridy = 1;
		JLabel Llistlevel = new JLabel("LISTLEVEL    ",JLabel.RIGHT);
		Llistlevel.setFont(bigFont);
		contentPane.add(Llistlevel,c);
		c.gridx = 4;
		c.gridy = 1;
		contentPane.add(Jlistlevel,c);
		Jlistlevel.setText(""+Front.listlevel);


		// ROW 2

		Jptrunc = new JTextField(6);
		Jptrunc.setFont(bigFont);
		Jptrunc.requestFocus(); 
		Jptrunc.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jptrunc.getText());
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("TRUNC",lval);
			}// actionPerformed
		}
				);
		c.gridx = 1;    
		c.gridy = 2;
		JLabel Ltrunc = new JLabel("TRUNC    ",JLabel.RIGHT);
		Ltrunc.setFont(bigFont);
		contentPane.add(Ltrunc,c);
		c.gridx = 2;
		c.gridy = 2;
		contentPane.add(Jptrunc,c);
		//f.pln(" $$$$$$$$$$$$$ vChorus.chorus.getResDistance()) = "+vChorus.chorus.getResDistance());
		Jptrunc.setText(""+(int) Front.getSpecialVar("TRUNC",25.0));

		Jtime = new JTextField(6);
		Jtime.setFont(bigFont);
		Jtime.requestFocus(); 
		Jtime.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				double val = Double.parseDouble(Jtime.getText());
				//Front.timet = val;
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("TIME",lval);
				f.pln(" $$$$$ vParams: ENTER newid = TIME,  lval = "+lval);
			}// actionPerformed
		}
				);
		c.gridx = 3;
		c.gridy = 2;
		JLabel Ltime = new JLabel("TIME    ",JLabel.RIGHT);
		Ltime.setFont(bigFont);
		contentPane.add(Ltime,c);
		c.gridx = 4;
		c.gridy = 2;
		contentPane.add(Jtime,c);
		Jtime.setText(""+Front.timet);



		//    // ROW 3
		Jstart = new JTextField(6);
		Jstart.setFont(bigFont);
		Jstart.requestFocus(); 
		Jstart.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jstart.getText());	
				//f.pln("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ JFDd.addActionListener val = "+val);
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("START",lval);
				f.pln(" $$$$$ vParams: ENTER newid = START,  lval = "+lval);								
			}// actionPerformed
		}
				);
		c.gridx = 1;    
		c.gridy = 3;
		JLabel Lstart = new JLabel("START    ",JLabel.RIGHT);
		Lstart.setFont(bigFont);
		contentPane.add(Lstart,c);
		c.gridx = 2;
		c.gridy = 3;
		contentPane.add(Jstart,c);
		Jstart.setText(""+(int) (Front.getdef("START",Front.defstart)));



		Jautofast = new JTextField(6);
		Jautofast.setFont(bigFont);
		Jautofast.requestFocus(); 
		Jautofast.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jautofast.getText());
				//Front.autofast = val;
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("AUTOFAST",lval);
				f.pln(" $$$$$ vParams: ENTER newid = AUTOFAST,  lval = "+lval);
			}// actionPerformed
		}
				);
		c.gridx = 3;    
		c.gridy = 3;
		JLabel Lautofast = new JLabel("AUTOFAST    ",JLabel.RIGHT);
		Lautofast.setFont(bigFont);
		contentPane.add(Lautofast,c);
		c.gridx = 4;
		c.gridy = 3;
		contentPane.add(Jautofast,c);
		//f.pln(" $$$$$$$$$$$$$ vChorus.chorus.getResDistance()) = "+vChorus.chorus.getResDistance());
		Jautofast.setText(""+(int) (Front.getdef("AUTOFAST",Front.defstart)));


		//   ROW 4

		Jautoprune = new JTextField(6);
		Jautoprune.setFont(bigFont);
		Jautoprune.requestFocus(); 
		Jautoprune.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jautoprune.getText());
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("AUTOPRUNE",lval);
				f.pln(" $$$$$ vParams: ENTER newid = AUTOPRUNE,  lval = "+lval);
			}// actionPerformed
		}
				);
		c.gridx = 1;    
		c.gridy = 4;
		JLabel Lautoprune = new JLabel("AUTOPRUNE    ",JLabel.RIGHT);
		Lautoprune.setFont(bigFont);
		contentPane.add(Lautoprune,c);
		c.gridx = 2;
		c.gridy = 4;
		contentPane.add(Jautoprune,c);
		//f.pln(" $$$$$$$$$$$$$ vChorus.chorus.getResDistance()) = "+vChorus.chorus.getResDistance());
		Jautoprune.setText(""+(int) (Front.getdef("AUTOPRUNE",Front.defautoprune)));

		
		
		Jnotrunc = new JTextField(6);
		Jnotrunc.setFont(bigFont);
		Jnotrunc.requestFocus(); 
		Jnotrunc.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jnotrunc.getText());
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("NOTRUNC",lval);
				f.pln(" $$$$$ vParams: ENTER newid = NOTRUNC,  lval = "+lval);								
			}// actionPerformed
		}
				);
		c.gridx = 3;    
		c.gridy = 4;
		JLabel Lnotrunc = new JLabel("NOTRUNC    ",JLabel.RIGHT);
		Lnotrunc.setFont(bigFont);
		contentPane.add(Lnotrunc,c);
		c.gridx = 4;
		c.gridy = 4;
		contentPane.add(Jnotrunc,c);
		//f.pln(" $$$$$$$$$$$$$ vChorus.chorus.getResDistance()) = "+vChorus.chorus.getResDistance());
		Jnotrunc.setText(""+(int) (Front.getdef("NOTRUNC",Front.defnotrunc))
				);

		
		//   ROW 5

		
		Jwarndig = new JTextField(6);
		Jwarndig.setFont(bigFont);
		Jwarndig.requestFocus(); 
		Jwarndig.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jwarndig.getText());
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("WARNDIG",lval);
				f.pln(" $$$$$ vParams: ENTER newid = WARNDIG,  lval = "+lval);								

			}// actionPerformed
		}
				);
		c.gridx = 1;
		c.gridy = 5;
		JLabel Lwarndig = new JLabel("WARNDIG    ",JLabel.RIGHT);
		Lwarndig.setFont(bigFont);
		contentPane.add(Lwarndig,c);
		c.gridx = 2;
		c.gridy = 5;
		contentPane.add(Jwarndig,c);
		Jwarndig.setText(""+(int) (Front.getdef("WARNDIG",Front.defwarndig)));

		
		Jprune = new JTextField(6);
		Jprune.setFont(bigFont);
		Jprune.requestFocus(); 
		Jprune.addActionListener( new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int val = Integer.parseInt(Jprune.getText());
				ValuRec lval = new ValuRec(0.0,val,true);
				Front.enterid("prune",lval);
				f.pln(" $$$$$ vParams: ENTER newid = prune,  lval = "+lval);								
			}// actionPerformed
		}
				);
		c.gridx = 3;    
		c.gridy = 5;
		JLabel Lprune = new JLabel("prune    ",JLabel.RIGHT);
		Lprune.setFont(bigFont);
		contentPane.add(Lprune,c);
		c.gridx = 4;
		c.gridy = 5;
		contentPane.add(Jprune,c);
		//f.pln(" $$$$$$$$$$$$$ vChorus.chorus.getResDistance()) = "+vChorus.chorus.getResDistance());
		Jprune.setText(""+(int) (Front.getdef("prune",Front.defprune))
				);

		
		
		// ----------------------------------------------
		pack();
		scroll.revalidate();
	}


	// not used 
	static public void update() {
	    int listlevel  = (int) (Front.getdef("LIST",Front.deflistlevel));
	    //int start = (int) (Front.getdef("START",-1.0));
	    int qtcalc = (int) (Front.getdef("QTCALC",Front.defqtcalc));
	    int autofast = (int) (Front.getdef("AUTOFAST",Front.defautofast));
	    int autoprune = (int) (Front.getdef("AUTOPRUNE",Front.defautoprune));
	    int notrunc =   (int) (Front.getdef("NOTRUNC",0.0));
		int ptrunc = (int) Front.getSpecialVar("TRUNC",25.0);
		int warndig = (int) Front.getSpecialVar("WARNDIG",2.0);	
		int prune = (int) Front.getSpecialVar("PRUNE",25.0);
		Jqtcalc.setText(""+qtcalc);
		Jlistlevel.setText(""+listlevel);
		Jptrunc.setText(""+ptrunc);
		Jwarndig.setText(""+warndig);
		Jautofast.setText(""+autofast);    
		Jautoprune.setText(""+autoprune);
		Jnotrunc.setText(""+notrunc);
		Jprune.setText(""+prune);
		scroll.revalidate();
	}



}// Params 
