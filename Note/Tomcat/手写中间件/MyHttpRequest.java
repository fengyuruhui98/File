package Note.Tomcat.手写中间件;
import java.io.IOException;
import java.io.InputStream;

public class MyHttpRequest{
  private InputStream inputStream;
  private String uri;

  public MyHttpRequest(InputStream inputStream){
    this.inputStream = inputStream;
  }
  public void parse(){
    try{
      byte[] bytes = new byte[1024];
      inputStream.read(bytes);
      String request = new String(bytes);
      System.out.println(request);
      parseUri(request);
    }catch(IOException e){
      e.printStackTrace();
    }
  }
  public void parseUri(String request){
    int index1, index2;
    index1 = request.indexOf(' ');
    index2 = request.indexOf(' ', index1 + 1);
    System.out.println(index1);
    System.out.println(index2);
    uri = request.substring(index1 + 1, index2);
    System.out.println(uri);
  }
  public String getUri(){
    return this.uri;
  }
}