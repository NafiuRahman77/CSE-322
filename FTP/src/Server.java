import java.io.File;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class Server {

    private static long MAX_BUFFER_SIZE = 1000000000;
    private static int MIN_CHUNK_SIZE = 1000;
    private static int MAX_CHUNK_SIZE = 10000;
    public static long usedBuffer = 0;
    private static ArrayList<String> LoggedinClient = new ArrayList<>();
    private static ArrayList<String> ActiveClient = new ArrayList<>();
    private static ArrayList<Request> fileRequests = new ArrayList<>();
    private static int requestCount=0;
    public static HashMap<Integer, byte[]> files=new HashMap<>();
    public static Map<String,ArrayList<String>> inbox=new TreeMap<>();
    public static int fileCount=0;

    public static ArrayList<Request> getFileRequests() {
        return fileRequests;
    }

    public static int getRequestCount() {
        return requestCount;
    }

    public static void setRequestCount(int requestCount) {
        Server.requestCount = requestCount;
    }

    public static int getMinChunkSize() {
        return MIN_CHUNK_SIZE;
    }

    public static int getRandomChunkSize(){
        return (int) (Math.random()*(Server.getMaxChunkSize()- Server.getMinChunkSize()+1)+ Server.getMinChunkSize());
    }
    public static int getMaxChunkSize() {
        return MAX_CHUNK_SIZE;
    }


    public static long getMaxBufferSize() {
        return MAX_BUFFER_SIZE;
    }

    public static long getUsedBuffer() {
        return usedBuffer;
    }

    public static void setUsedBuffer(long usedBuffer) {
        Server.usedBuffer = usedBuffer;
    }

    public static ArrayList<String> getActiveClient() {
        return ActiveClient;
    }

    public static ArrayList<String> getLoggedinClient() {
        return LoggedinClient;
    }
    public static boolean login(String student_id){
        int flag = 1;
        for (int i = 0; i < Server.getActiveClient().size(); i++){
            if (Server.getActiveClient().get(i).equals(student_id)){   // login failed
                return false;
            }
        }

            return true;

    }

    public static boolean hasDirectory(String student_id){
        Server.getActiveClient().add(student_id);
        for (int i = 0; i < Server.getLoggedinClient().size(); i++){
            if(Server.getLoggedinClient().get(i).equals(student_id)){
                return true;
            }
        }

       return false;
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException {
        ServerSocket welcomeSocket = new ServerSocket(6666);
        File uploadable_file = new File("src/files/");
        uploadable_file.mkdirs();

        while(true) {
            System.out.println("Waiting for connection...");
            Socket socket = welcomeSocket.accept();
            System.out.println("Connection established");

            // open thread
            Thread worker = new Worker(socket);
            worker.start();


        }

    }
}
