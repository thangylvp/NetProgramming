/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client4;
import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;
/**
 *
 * @author banhb
 */
public class Client4 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        int port = 10002;
        Thread connThread = new ConnectThread(port);
        connThread.start();

        try {
            ServerSocket servSocket = new ServerSocket(port, 1);
            System.out.println("Listening for connections on port "
                    + servSocket.getLocalPort());
            while (true) {
                Socket conn = servSocket.accept();
                System.out.println("New client has connected with " + conn);
                Thread listenThread = new ListenThread(conn);
                listenThread.start();
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

}

class ConnectThread extends Thread {

    static final int BUFSIZE = 8192;
    private Socket conn;
    private DataInputStream in;
    private DataOutputStream out;
    private int port;
    private int type;
    private String path = "C:\\Users\\banhb\\Desktop\\LTM\\NetProgramming\\Client\\Toàn\\Client4\\listFiles";
    
    public ConnectThread(int port) {
        this.port = port;
    }

    public void run() {
        byte[] socketBuffer = new byte[BUFSIZE];
        try {
            Socket socket = new Socket("localhost", 1998);
            in = new DataInputStream(socket.getInputStream());
            out = new DataOutputStream(socket.getOutputStream());
            while (true) {
                type = SubFunction.readType(in);
                if (type == 1) {
                    do {
                        SubFunction.sendListFileMessage(out, path, port);
                        type = SubFunction.readType(in);
                    } while (type != 1);
                    String fileName;
                    do {
                        System.out.println("Enter file name to download or 'quit' to close the process:");
                        Scanner sc = new Scanner(System.in);
                        fileName = sc.nextLine();
                        if (!fileName.equalsIgnoreCase("quit")) {
                            ArrayList<Long> ports = new ArrayList<Long>();
                            ArrayList<InetAddress> ipAddrs = new ArrayList<InetAddress>();
                            SubFunction.sendRequestDownloadMess(out, fileName);
                            type = SubFunction.readType(in);
                            if (type == 4) {
                                byte[] longBuff = new byte[8];
                                in.read(longBuff, 0, 8);
                                long file_size = ConvertIntoBytes.bytesToLong(longBuff);
                                byte[] intBuff = new byte[4];
                                in.read(intBuff, 0, 4);
                                int numberClients = ConvertIntoBytes.bytesToInt(intBuff);
                                for (int i = 0; i < numberClients; i++) {
                                    byte[] byteIpAddr = new byte[4];
                                    byte[] byte_port = new byte[8];
                                    in.read(byteIpAddr, 0, 4);
                                    in.read(byte_port, 0, 8);
                                    long port = ConvertIntoBytes.bytesToLong(byte_port);
                                    InetAddress ip_addr = InetAddress.getByAddress(byteIpAddr);
                                    ports.add(port);
                                    ipAddrs.add(ip_addr);

                                }
                                for (int i = 0; i < ipAddrs.size(); i++) {
                                    long port = ports.get(i);
                                    Socket downloadSocket = new Socket(ipAddrs.get(i), (int) port);
                                    DataInputStream inDownload = new DataInputStream(downloadSocket.getInputStream());
                                    DataOutputStream outDownload = new DataOutputStream(downloadSocket.getOutputStream());
                                    int type2;
                                    type2 = inDownload.readInt();
                                    if (type2 == 1) {
                                        SubFunction.sendRequestToClientMess(outDownload, fileName);
                                        type2 = inDownload.readInt();
                                        if (type2 == 3) {
                                            boolean flag = SubFunction.recvFile(inDownload,path + "/" + fileName, file_size, BUFSIZE);
                                            if(flag){
                                                type2 = 1;
                                                outDownload.writeInt(type2);

                                                outDownload.close();
                                                inDownload.close();
                                                downloadSocket.close();
                                                break;
                                            }
                                        }
                                    }
                                }
                                SubFunction.sendListFileMessage(out, path, port);
                            }else if(type==7){
                                byte[] error_byte = new byte[4];
                                in.read(error_byte,0,4);
                                int errorCode = ConvertIntoBytes.bytesToInt(error_byte);
                                if(errorCode==1){
                                    System.out.println("File not exist!");
                                }
                            }
                           
                        } else {
                            break;
                        }
                    } while (!fileName.equalsIgnoreCase("quit"));
                    break;
                } else {

                    System.out.println("Wrong type! " + type);
                }
                break;
            }
            in.close();
            out.close();
            socket.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

}

class ListenThread extends Thread {

    static final int BUFSIZE = 8192;
    private Socket connSock;
    private DataInputStream in;
    private DataOutputStream out;

    public ListenThread(Socket socket) {
        this.connSock = socket;
    }

    public void run() {
        byte[] buffer = new byte[BUFSIZE];
        try {
            in = new DataInputStream(connSock.getInputStream());
            out = new DataOutputStream(connSock.getOutputStream());
            int type;
            while (true) {
                out.writeInt(1);
                type = in.readInt();
                if (type == 5) {
                    String fileName = in.readUTF();
                    File dir = new File("/home/tranquangtoan/NetBeansProjects/Client4/listFiles");
                    File[] listFile = dir.listFiles();
                    File file = new File("/home/tranquangtoan/NetBeansProjects/Client4/listFiles/"+fileName);
                    
                    if (file.exists()) {
                        SubFunction.sendFile(out, file, BUFSIZE);
                        type = in.readInt();
                        if (type == 1) {
                            break;
                        }
                    }else{
                        type=7;
                        out.writeInt(type);
                        out.flush();
                        int errorCode = 1;
                        out.writeInt(errorCode);
                        out.flush();
                    }

                }
            }
            in.close();
            out.close();
            connSock.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    
}
