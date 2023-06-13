public class Request {
    public String client_id; //kon client request pathaise
    public int requestCount;
    public String fileName;

    public Request(String client_id, int requestCount, String fileName) {
        this.client_id = client_id;
        this.requestCount = requestCount;
        this.fileName=fileName;
    }
}
