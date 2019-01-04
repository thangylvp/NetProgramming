/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package btlclient;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;
/**
 *
 * @author thanhthu
 */
public class BtlClient {
    	private static Socket socket;
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner input = new Scanner(System.in);
//		while(true) {
			try {
				String host = "localhost";
				int port = 1235;
				InetAddress address = InetAddress.getByName(host);
				socket = new Socket(address, port);
				OutputStream os = socket.getOutputStream();
				OutputStreamWriter osw = new OutputStreamWriter(os);
				BufferedWriter bw = new BufferedWriter(osw);
				
				FileList fileList = new FileList();
				fileList.setFileList();
				String list = fileList.getFileList().concat("\0");
				bw.write(list);
				bw.flush();
			}
			catch (IOException exception) {
				System.out.println("Error");
			}
	}
    
}
