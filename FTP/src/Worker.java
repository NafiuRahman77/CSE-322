
import java.io.*;

import java.lang.reflect.Array;
import java.net.Socket;
import java.net.SocketException;
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
                    ArrayList<String> a=new ArrayList<>();
                    Server.inbox.put(client_id,a);
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
                            Server.inbox.replace(c,a);
                        }
                    }

                    out.writeObject("Request broadcasted to all clients");

                } else if (option.equalsIgnoreCase("6")) {
                    ArrayList<String> messages = Server.inbox.get(client_id);
                    System.out.println(messages);
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
                        try {
                            long totalBytesReceived = 0;
                            boolean filesuccess = true;
                            while (fileSize > 0) {
                                bytesRead = bis.read(buffer, 0, (int) Math.min(buffer.length, fileSize));
                                if (bytesRead == -1) {
                                    break;
                                }
                                fileOutputStream.write(buffer, 0, bytesRead);
                                fileSize -= bytesRead;
                                bytes_sum += bytesRead;

                            }
                            fileOutputStream.close();


                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                } else if (option.equalsIgnoreCase("8")) {
                    String rid= (String) in.readObject();
                    int reqId= Integer.parseInt(rid);
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
                        try {
                            long totalBytesReceived = 0;
                            boolean filesuccess = true;
                            while (fileSize > 0) {
                                bytesRead = bis.read(buffer, 0, (int) Math.min(buffer.length, fileSize));
                                if (bytesRead == -1) {
                                    break;
                                }
                                fileOutputStream.write(buffer, 0, bytesRead);
                                fileSize -= bytesRead;
                                bytes_sum += bytesRead;

                            }
                            fileOutputStream.close();

                            for(int i=0;i<Server.getFileRequests().size();i++){
                                Request r=Server.getFileRequests().get(i);
                                if(r.requestCount==reqId){
                                    ArrayList<String> a = Server.inbox.get(r.client_id);
                                    a.add(r.fileName+" is uploaded by "+client_id+" check his public folder");
                                    Server.inbox.replace(r.client_id,a);
                                }
                            }


                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                } else if (option.equalsIgnoreCase("9")) {
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