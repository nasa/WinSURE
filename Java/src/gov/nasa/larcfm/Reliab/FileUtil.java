/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.Reliab;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.List;

/** A collection of utility functions useful for files */
public final class FileUtil {

	  /**
	   * Given a file name return the path of this name.
	   * This also converts backward slashes to forward slashes.
	   * @filename the name of the file
	   * @return the path of the file, if no path, then an empty string is returned.
	   */
	  public static String get_path(String filename) {
		  return filename.substring(0,filename.replaceAll("\\\\", "/").lastIndexOf("/") + 1);
	  }
	  	  
	/**
	 * Returns the base (no path) file name. 
	 * <ul> 
	 * <li>a/b/c.txt = c.txt
	 * <li>a.txt = a.txt
	 * <li>a/b/ = ""
	 * </ul>
	 * This also converts backward slashes to forward slashes.
	 * @param filename  filename string
	 * @return filename without path information
	 */
	public static String no_path(String filename) {
		if (filename == null) {
			return null;
		}
		// replace all \ (dos) with / (unix), then find last
		return filename.substring(filename.replaceAll("\\\\", "/").lastIndexOf("/") + 1);
	}

	/**
	 * Returns the file name without an extension (such as, ".txt").  Also removes path information. 
	 * This also converts backward slashes to forward slashes.
	 * @param filename  filename string
	 * @return filename without extension information
	 */
	public static String no_extension(String filename) {
		if (filename == null) {
			return null;
		}
		// replace all \ (dos) with / (unix), then find last
		String newname = no_path(filename);
		return newname.substring(0,newname.lastIndexOf("."));
	}

	  /** Given an array of names that may include files or directories,
	   * return a list of files that contains (1) all of the files in
	   * the original list and (2) all files ending with ".txt" in
	   * directories from the original list. */
	  public static List<String> getFileNames(String[] names) {
		  ArrayList<String> txtFiles = new ArrayList<String>();
		  for (String name: names) {
			  txtFiles.addAll(getFileName(name));
		  }
		  return txtFiles;	  
	  }
	  
	  /** Given a list of names that may include files or directories,
	   * return a list of files that contains (1) all of the files in
	   * the original list and (2) all files ending with ".txt" in
	   * directories from the original list. */
	  public static List<String> getFileNames(List<String> names) {
		  ArrayList<String> txtFiles = new ArrayList<String>();
		  for (String name: names) {
			  txtFiles.addAll(getFileName(name));
		  }
		  return txtFiles;	  
	  }
	  
	  /** Given a name that may be a file or directory,
	   * return a list of files that contains (1) all of the files in
	   * the original list and (2) all files ending with ".txt" in
	   * directories from the original list. */
	  public static List<String> getFileName(String name) {
		  ArrayList<String> txtFiles = new ArrayList<String>();
		  File file = new File(name);
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
		  return txtFiles;
	  }
}
