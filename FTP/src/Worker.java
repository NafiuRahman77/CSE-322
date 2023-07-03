
import java.io.*;

import java.lang.reflect.Array;
import java.net.Socket;
import java.net.SocketException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Date;

public class Worker extends Thread {
    Socket socket;
    String client_id;

    public Worker(Socket socket) {
        this.socket = socket;
    }


    public void run() {
        // buffers
        try {
            ObjectOutputStream out = new ObjectOutputStream(this.socket.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(this.socket.getInputStream());

            BufferedOutputStream bos = new BufferedOutputStream(socket.getOutputStream());
            BufferedInputStream bis = new BufferedInputStream(socket.getInputStream());

            client_id = (String) in.readObject();
            boolean flag = Server.login(client_id);
            if (flag) {
                out.writeObject("Login is successful");
                boolean hasDirectory = Server.hasDirectory(client_id);
                if (!hasDirectory) {
                    File studentPrivate = new File("src/com/company/" + client_id + "/private");
                    File studentPublic = new File("src/com/company/" + client_id + "/public");
                    studentPrivate.mkdirs();
                    studentPublic.mkdirs();
                    ArrayList<String> a = new ArrayList<>();
                    Server.inbox.put(client_id, a);
                    Server.getLoggedinClient().add(client_id);
                }
                System.out.println("login successful by client id-" + client_id);
            } else {
                out.writeObject("login failed");
                socket.close();
            }
            while (true) {
                String option = (String) in.readObject();
                System.out.println("option is" + option);
                if (option.equalsIgnoreCase("1")) {
                    ArrayList<String> activeClientList = new ArrayList<>();
                    for (int i = 0; i < Server.getActiveClient().size(); i++) {
                        if (!Server.getActiveClient().get(i).equalsIgnoreCase(client_id)) {
                            activeClientList.add(Server.getActiveClient().get(i));
                        }
                    }
                    out.writeObject(activeClientList);
                    ArrayList<String> loggedClientList = new ArrayList<>();
                    for (int i = 0; i < Server.getLoggedinClient().size(); i++) {
                        if (!Server.getLoggedinClient().get(i).equalsIgnoreCase(client_id)) {
                            loggedClientList.add(Server.getLoggedinClient().get(i));
                        }
                    }
                    out.writeObject(loggedClientList);

                } else if (option.equalsIgnoreCase("2")) {
                    //Creating a File object for directory
                    File directoryPathPublic = new File("src/com/company/" + client_id + "/public");
                    //List of all files and directories
                    File filesListPublic[] = directoryPathPublic.listFiles();
                    out.writeObject(filesListPublic);
                    File directoryPathPrivate = new File("src/com/company/" + client_id + "/private");
                    File filesListPrivate[] = directoryPathPrivate.listFiles();
                    out.writeObject(filesListPrivate);
                } else if (option.equalsIgnoreCase("3")) {
                    String choice = (String) in.readObject();

                    if (choice.equalsIgnoreCase("1")) {
                        String fileType = (String) in.readObject();
                        String fileName = (String) in.readObject();
                        int chunkSize = (int) Server.getMaxChunkSize();
                        File file = new File("src/com/company/" + client_id + "/" + fileType + "/" + fileName);
                        System.out.println("src/com/company/" + client_id + "/" + fileType + "/" + fileName);
                        long filesize = file.length();
                        out.writeObject(filesize);

                        if (file.exists() && file.isFile()) {
                            FileInputStream fileInputStream = new FileInputStream(file);
                            byte[] buffer = new byte[chunkSize];
                            int bytesRead;
                            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                                out.write(buffer, 0, bytesRead);
                                out.flush();
                            }
                            fileInputStream.close();
                            out.writeObject("File download completed");
                            System.out.println("File sent: " + fileName + " for client id-" + client_id);
                        } else {
                            String errorMessage = "File not found: " + fileName + " for client id-" + client_id;
                            out.writeObject(errorMessage);
                            System.out.println(errorMessage);
                        }


                    }
                } else if (option.equalsIgnoreCase("4")) {
                    String id = (String) in.readObject();
                    int found = 0;
                    for (int i = 0; i < Server.getLoggedinClient().size(); i++) {
                        if (Server.getLoggedinClient().get(i).equalsIgnoreCase(id)) {
                            found = 1;
                        }
                    }
                    if (found == 1) {
                        out.writeObject("A person with this id exists");
                        File directoryPath = new File("src/com/company/" + id + "/public");
                        File filesList[] = directoryPath.listFiles();
                        out.writeObject(filesList);
                        String fileName = (String) in.readObject();
                        boolean fileExist = false;
                        for (File file : filesList) {
                            if (file.isFile() && file.getName().equalsIgnoreCase(fileName)) {
                                fileExist = true;
                                break;
                            }
                        }
                        if (fileExist) {
                            out.writeObject("file exists");

                            int chunkSize = (int) Server.getMaxChunkSize();
                            File file = new File("src/com/company/" + id + "/public" + "/" + fileName);
                            long filesize = file.length();
                            out.writeObject(filesize);

                            FileInputStream fileInputStream = new FileInputStream(file);
                            byte[] buffer = new byte[chunkSize];

                            int bytesRead = 0;

                            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                                out.write(buffer, 0, bytesRead);
                                out.flush();
                                ;
                            }
                            fileInputStream.close();
                            out.writeObject("File download completed");
                            System.out.println("File-" + fileName + " of client id-" + id + "sent to client id-" + client_id);


                        } else {
                            out.writeObject("no such file");
                        }


                    } else {
                        out.writeObject("This person doesn't exist");

                    }
                } else if (option.equalsIgnoreCase("5")) {
                    String filename = (String) in.readObject();
                    int count = Server.getRequestCount() + 1;
                    Request req = new Request(client_id, count, filename);
                    String msg = "filename: " + filename + " request-id: " + count;
                    Server.getFileRequests().add(req);
                    Server.setRequestCount(count);
                    Server.getLoggedinClient().forEach(client -> {
                        System.out.print(client + " ");
                    });
                    System.out.println();

                    for (int i = 0; i < Server.getLoggedinClient().size(); i++) {
                        String c = Server.getLoggedinClient().get(i);
                        if (client_id != c) {
                            ArrayList<String> a = Server.inbox.get(c);
                            a.add(msg);
                            Server.inbox.replace(c, a);
                        }
                    }

                    out.writeObject("Request broadcasted to all clients");

                } else if (option.equalsIgnoreCase("6")) {
                    ArrayList<String> messages = Server.inbox.get(client_id);
                    //System.out.println(messages);
                    ArrayList<String> cleanInbox=new ArrayList<>();
                    Server.inbox.replace(client_id, cleanInbox);
                    out.writeObject(messages);
                    out.reset();

                } else if (option.equalsIgnoreCase(("7"))) {
                    String fileName = (String) in.readObject();
                    long filesize = (long) in.readObject();
                    String fileType = (String) in.readObject();
                    if (Server.getMaxBufferSize() < Server.getUsedBuffer() + filesize) {
                        out.writeObject("Buffer Overloaded");
                    } else {
                        Server.setUsedBuffer(filesize + Server.getUsedBuffer());
                        out.writeObject("Buffer has enough space ");
                        int chunkSize = Server.getRandomChunkSize();
                        out.writeObject(chunkSize);
                        Server.fileCount++;
                        int fileId=Server.fileCount;
                        out.writeObject(client_id);
                        out.writeObject(Server.fileCount);
                        String currentFile = "src/com/company/" + client_id + "/" + fileType + "/" + fileName;
                        System.out.println(currentFile);
                        //FileOutputStream fileOutputStream = new FileOutputStream("src/com/company/" + client_id + "/" + fileType + "/" + fileName);

                        byte[] buffer = new byte[chunkSize];
                        int fileSize = (int) filesize;
                        int bytesRead = 0;
                        int bytes_sum = 0;
                        int chunkcount = 0;
                        int offset=0;
                        // ... existing code ...
                        File downfile=new File(currentFile);

                        try {
                            //chunkSize=500;
                            long chunk_no=filesize/chunkSize +1;
                            int flagg=1;
                            byte[] fileBuffer = new byte[(int) filesize];

                            Server.files.put(fileId,fileBuffer);


                            for (int i = 0; i < chunk_no - 1; i++) {

                                Object o =  in.readObject();
                                if(o instanceof byte[] ){
                                    buffer = (byte[]) o;
                                    // Write the buffer to the file
                                    // Files.write(Paths.get(downfile.getAbsolutePath()), buffer, StandardOpenOption.APPEND);
                                    System.arraycopy(buffer, 0, fileBuffer, offset, buffer.length);
                                    offset += buffer.length;
                                    Server.files.replace(fileId,fileBuffer);

                                    // Send acknowledgment to the client
                                    out.writeObject("ok");
                                }

                                else{
                                    System.out.println("bop"+(String)o);
                                    flagg=0;
                                    break;
                                }
                                //if(i==2){Thread.sleep(7000);}

                            }
                            if(flagg==1) {
                                buffer = (byte[]) in.readObject();
                                //Files.write(Paths.get(downfile.getAbsolutePath()), buffer, StandardOpenOption.APPEND);
                                System.arraycopy(buffer, 0, fileBuffer, offset, buffer.length);
                                Server.files.replace(fileId,fileBuffer);
                                offset += buffer.length;
                            }

                            String clientMessage = (String) in.readObject();
                            System.out.println(clientMessage);

                            if (clientMessage.equalsIgnoreCase("Delete file")) {
                                // Delete the file on the server
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server");
//                                }
                            } else if(offset!=fileSize){
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server for disconnect");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server for disconnect");
//                                }
                            }else {
                                downfile.createNewFile();
                                Files.write(Paths.get(downfile.getAbsolutePath()), Server.files.get(fileId));
                                // Handle other client messages if needed
                                System.out.println(clientMessage);
                            }


                        } catch(SocketException e){
                            if(offset!=fileSize){
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server for disconnect");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server for disconnect");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server for disconnect");
//                                }
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        Server.setUsedBuffer(Server.getUsedBuffer()-filesize );
                    }
                } else if (option.equalsIgnoreCase("8")) {
                    String rid = (String) in.readObject();
                    int reqId = Integer.parseInt(rid);
                    String fileName = (String) in.readObject();
                    long filesize = (long) in.readObject();
                    String fileType = (String) in.readObject();
                    if (Server.getMaxBufferSize() < Server.getUsedBuffer() + filesize) {
                        out.writeObject("Buffer Overloaded");
                    } else {
                        Server.setUsedBuffer(filesize + Server.getUsedBuffer());
                        out.writeObject("Buffer has enough space ");
                        int chunkSize = Server.getRandomChunkSize();
                        out.writeObject(chunkSize);
                        Server.fileCount++;
                        int fileId=Server.fileCount;
                        out.writeObject(client_id);
                        out.writeObject(Server.fileCount);
                        String currentFile = "src/com/company/" + client_id + "/" + fileType + "/" + fileName;
                        System.out.println(currentFile);
                        FileOutputStream fileOutputStream = new FileOutputStream("src/com/company/" + client_id + "/" + fileType + "/" + fileName);

                        byte[] buffer = new byte[chunkSize];
                        int fileSize = (int) filesize;
                        int bytesRead = 0;
                        int bytes_sum = 0;
                        int chunkcount = 0;
                        int offset=0;
                        File downfile=new File(currentFile);

                        try {
                            chunkSize=200;
                            long chunk_no=filesize/chunkSize +1;
                            int flagg=1;
                            byte[] fileBuffer = new byte[(int) filesize];

                            Server.files.put(fileId,fileBuffer);


                            for (int i = 0; i < chunk_no - 1; i++) {

                                Object o =  in.readObject();
                                if(o instanceof byte[] ){
                                    buffer = (byte[]) o;
                                    // Write the buffer to the file
                                    // Files.write(Paths.get(downfile.getAbsolutePath()), buffer, StandardOpenOption.APPEND);
                                    System.arraycopy(buffer, 0, fileBuffer, offset, buffer.length);
                                    offset += buffer.length;
                                    Server.files.replace(fileId,fileBuffer);

                                    // Send acknowledgment to the client
                                    out.writeObject("ok");
                                }

                                else{
                                    System.out.println("bop"+(String)o);
                                    flagg=0;
                                    break;
                                }
                                //Thread.sleep(7000);

                            }
                            if(flagg==1) {
                                buffer = (byte[]) in.readObject();
                                //Files.write(Paths.get(downfile.getAbsolutePath()), buffer, StandardOpenOption.APPEND);
                                System.arraycopy(buffer, 0, fileBuffer, offset, buffer.length);
                                Server.files.replace(fileId,fileBuffer);
                                offset += buffer.length;
                            }

                            String clientMessage = (String) in.readObject();
                            System.out.println(clientMessage);

                            if (clientMessage.equalsIgnoreCase("Delete file")) {
                                // Delete the file on the server
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server");
//                                }
                            } else if(offset!=fileSize){
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server for disconnect");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server for disconnect");
//                                }
                            }else {
                                downfile.createNewFile();
                                Files.write(Paths.get(downfile.getAbsolutePath()), Server.files.get(fileId));
                                // Handle other client messages if needed
                                System.out.println(clientMessage);
                            }


                        } catch(SocketException e){
                            if(offset!=fileSize){
                                Server.files.remove(fileId);
                                System.out.println("File deleted on the server for disconnect");
//                                boolean deleted = downfile.delete();
//                                if (deleted) {
//                                    System.out.println("File deleted on the server for disconnect");
//                                } else {
//                                    System.out.println("Failed to delete the file on the server for disconnect");
//                                }
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        Server.setUsedBuffer(Server.getUsedBuffer()-filesize );

                        for (int i = 0; i < Server.getFileRequests().size(); i++) {
                            Request r = Server.getFileRequests().get(i);
                            if (r.requestCount == reqId) {
                                ArrayList<String> a = Server.inbox.get(r.client_id);
                                a.add(r.fileName + " is uploaded by " + client_id + " check his public folder");
                                Server.inbox.replace(r.client_id, a);
                            }
                        }

                    }
                } else if (option.equalsIgnoreCase("9")) {
                    String id = (String) in.readObject();
                    int found = 0;
                    for (int i = 0; i < Server.getLoggedinClient().size(); i++) {
                        if (Server.getLoggedinClient().get(i).equalsIgnoreCase(id)) {
                            found = 1;
                        }
                    }
                    if (found == 1) {
                        out.writeObject("A person with this id exists");
                        File directoryPath = new File("src/com/company/" + id + "/public");
                        File filesList[] = directoryPath.listFiles();
                        out.writeObject(filesList);

                    } else {
                        out.writeObject("This person doesn't exist");

                    }
                } else if (option.equalsIgnoreCase("10")) {
                    Server.getActiveClient().remove(client_id);
                    System.out.println(client_id + " has logged out");
                    in.close();
                    out.close();
                    socket.close();
                    break;
                }

            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SocketException e) {
            Server.getActiveClient().remove(client_id);
            System.out.println(client_id + " has logged out");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}