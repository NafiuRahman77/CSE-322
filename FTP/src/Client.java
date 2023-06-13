
import java.awt.*;
import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        Socket socket = new Socket("localhost", 6666);
        System.out.println("Connection established");
        System.out.println("Remote port: " + socket.getPort());
        System.out.println("Local port: " + socket.getLocalPort());


        Scanner sc = new Scanner(System.in);
        // buffers
        ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
        ObjectInputStream in = new ObjectInputStream(socket.getInputStream());

        BufferedOutputStream bos = new BufferedOutputStream(socket.getOutputStream());
        BufferedInputStream bis = new BufferedInputStream(socket.getInputStream());


        System.out.println("Enter your client id : ");
        String client_id = sc.nextLine();
        out.writeObject(client_id);

        String loginConfirmation = (String) in.readObject();
        if (loginConfirmation.equalsIgnoreCase("Login is successful")) {
            System.out.println("Login is successful");
        } else {
            System.out.println("Login failed, this user is already logged in");
            socket.close();
        }

        //


        while (true) {

            System.out.println("1. Lookup Clients\t 2. Your Files\t 3.Download Your file\t 4. Lookup a Client's file\t 5. Request File\t 6. View\t 7. Upload\t 8.Upload to a request\t 9. Logout");
            String option = sc.nextLine();
            out.writeObject(option);
            if (option.equalsIgnoreCase("1")) {
                ArrayList<String> activeclientList = (ArrayList<String>) in.readObject();
                ArrayList<String> loggedclientList = (ArrayList<String>) in.readObject();
                loggedclientList.forEach(client -> {
                    if (activeclientList.contains(client)) {
                        System.out.println(client + " is online");
                    } else {
                        System.out.println(client);
                    }
                });
            } else if (option.equalsIgnoreCase("2")) {
                File filesListPublic[] = (File[]) in.readObject();
                File filesListPrivate[] = (File[]) in.readObject();
                System.out.println("Public files:");
                for (File file : filesListPublic) {
                    if (file.isFile()) {
                        System.out.println(file.getName());
                    }
                }
                System.out.println("Private Files:");
                for (File file : filesListPrivate) {
                    if (file.isFile()) {
                        System.out.println(file.getName());
                    }

                }


            } else if (option.equalsIgnoreCase("3")) {
                System.out.println("Do you want to download any file?1. Yes 2. No ");
                String choice = sc.nextLine();
                out.writeObject(choice);
                if (choice.equalsIgnoreCase("1")) {
                    System.out.println("Enter filetype - 1.Public 2.Private");
                    String fileType = sc.nextLine();
                    if (fileType.equalsIgnoreCase("1")) {
                        out.writeObject("public");
                    } else {
                        out.writeObject("private");
                    }

                    System.out.println("Enter filename -");
                    String fileName = sc.nextLine();
                    out.writeObject(fileName);

                    long fileSize = (long) in.readObject();

                    FileOutputStream fileOutputStream = new FileOutputStream("src/files/" + fileName);

                    byte[] buffer = new byte[Server.getMaxChunkSize()];
                    int bytesRead = 0;
                    while (fileSize > 0 && (bytesRead = in.read(buffer, 0, (int) Math.min(buffer.length, fileSize))) != -1) {
                        fileOutputStream.write(buffer, 0, bytesRead);
                        fileSize -= bytesRead;
                    }
                    String confirmMsg = (String) in.readObject();
                    System.out.println(confirmMsg);
                    fileOutputStream.close();
                }


            } else if (option.equalsIgnoreCase("4")) {
                System.out.println("Mention the client ID :");
                String id = sc.nextLine();
                out.writeObject(id);
                String personExistCheck = (String) in.readObject();
                if (personExistCheck.equals("A person with this id exists")) {
                    File filesList[] = (File[]) in.readObject();
                    System.out.println("Public files of client id-" + id + ":");
                    for (File file : filesList) {
                        if (file.isFile()) {
                            System.out.println(file.getName());
                        }
                    }
                    System.out.println();
                    System.out.println("Enter the filename that you want to download");
                    String fileName = sc.nextLine();
                    out.writeObject(fileName);
                    String fileExistConfirmation = (String) in.readObject();
                    if (fileExistConfirmation.equalsIgnoreCase("file exists")) {
                        long fileSize = (long) in.readObject();

                        FileOutputStream fileOutputStream = new FileOutputStream("src/files/" + fileName);

                        byte[] buffer = new byte[Server.getMaxChunkSize()];
                        int bytesRead = 0;
                        while (fileSize > 0 && (bytesRead = in.read(buffer, 0, (int) Math.min(buffer.length, fileSize))) != -1) {
                            fileOutputStream.write(buffer, 0, bytesRead);
                            fileSize -= bytesRead;
                        }
                        String confirmMsg = (String) in.readObject();
                        System.out.println(confirmMsg);
                        fileOutputStream.close();
                    } else {
                        System.out.println("No such file exists");
                        continue;
                    }
                } else {
                    System.out.println("No person with this id exists");
                }
            } else if (option.equalsIgnoreCase("5")) {
                System.out.println("Enter file name");
                String filename = sc.nextLine();
                out.writeObject(filename);
                String confirmMsg = (String) in.readObject();
                System.out.println(confirmMsg);
            } else if (option.equalsIgnoreCase("6")) {
                ArrayList<String> messages = (ArrayList<String>) in.readObject();
                if (messages.size() == 0) {
                    System.out.println("You have no unread messages");
                } else {
                    System.out.println(messages.size());
                    for (int i = 0; i < messages.size(); i++) {
                        System.out.println(messages.get(i));
                    }
                }

            } else if (option.equalsIgnoreCase("7")) {
                System.out.println("Input file name from the files folder");
                String tempfile = sc.nextLine();
                File upload = new File("src/files/" + tempfile);
                // send file name
                System.out.println("Choose a filename : ");
                String fileName = sc.nextLine();
                out.writeObject(fileName);
                // send file size
                long length = upload.length();
                out.writeObject(length);
                System.out.println("Choose a filetype 1.Public 2.Private ");
                String fileType = sc.nextLine();
                if (fileType.equalsIgnoreCase("1")) {
                    out.writeObject("Public");
                } else {
                    out.writeObject("Private");
                }
                String uploadmsg = (String) in.readObject();


                if (uploadmsg.equalsIgnoreCase("Buffer Overloaded")) {
                    System.out.println(uploadmsg);
                } else {
                    System.out.println(uploadmsg);
                    int chunkSize = (int) in.readObject();
                    String clientId = (String) in.readObject();
                    int fileCount = (int) in.readObject();
                    int bytesRead = 0;
                    File file = new File("src/files/" + tempfile);
                    FileInputStream fileInputStream = new FileInputStream(file);

                    byte[] buffer = new byte[chunkSize];
                    socket.setSoTimeout(30000);

                    try {

                        long totalBytesSent = 0;

                        while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                            bos.write(buffer, 0, bytesRead);
                            bos.flush();

                        }

                        fileInputStream.close();
                        // bos.close();

                    } catch (SocketTimeoutException e) {

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            else if(option.equalsIgnoreCase("8")){
                System.out.println("Enter request id");
                String reqId=sc.nextLine();
                out.writeObject(reqId);
                System.out.println("Input file name from the files folder");
                String tempfile = sc.nextLine();
                File upload = new File("src/files/" + tempfile);
                // send file name
                System.out.println("Choose a filename : ");
                String fileName = sc.nextLine();
                out.writeObject(fileName);
                // send file size
                long length = upload.length();
                out.writeObject(length);
                System.out.println("Choose a filetype 1.Public 2.Private ");
                String fileType = sc.nextLine();
                if (fileType.equalsIgnoreCase("1")) {
                    out.writeObject("Public");
                } else {
                    out.writeObject("Private");
                }
                String uploadmsg = (String) in.readObject();


                if (uploadmsg.equalsIgnoreCase("Buffer Overloaded")) {
                    System.out.println(uploadmsg);
                } else {
                    System.out.println(uploadmsg);
                    int chunkSize = (int) in.readObject();
                    String clientId = (String) in.readObject();
                    int fileCount = (int) in.readObject();
                    int bytesRead = 0;
                    File file = new File("src/files/" + tempfile);
                    FileInputStream fileInputStream = new FileInputStream(file);

                    byte[] buffer = new byte[chunkSize];
                    socket.setSoTimeout(4000);

                    try {

                        long totalBytesSent = 0;

                        while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                            bos.write(buffer, 0, bytesRead);
                            bos.flush();

                        }

                        fileInputStream.close();
                        // bos.close();

                    } catch (SocketTimeoutException e) {

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            else if(option.equalsIgnoreCase("9")){

                in.close();
                out.close();
                socket.close();
                break;
            }
        }


    }
}

