/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package btlclient;
import java.io.File;
/**
 *
 * @author thanhthu
 */
public class FileList {
    	private static int numFile;
//	private static String source = "./src/Client";
	String list = new String("");
	public void setFileList() {
		String workingDir = System.getProperty("user.dir");
		File fileList = new File(workingDir+"/src/Client1");
		File[] file = fileList.listFiles();
		numFile = file.length;
		for (File index : file) {
			list = list.concat(index.getName().concat("\n"));
		}
		System.out.print(list);
	}
	
	public String getFileList() {
		return list;
	}
	
	public int getNumFile() {
		return numFile;
	}
}
