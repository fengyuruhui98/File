package Note.Tomcat.手写中间件;

import java.io.File;
import java.io.FileInputStream;
import java.io.OutputStream;

public class MyHttpResponse {
  private OutputStream outputStream;

  public MyHttpResponse(OutputStream outputStream){
    this.outputStream = outputStream;
  }

  public void sendRedirect(String uri){
    //判断uri存在，不存在返回404，否则返回目标资源数据
    System.out.println(System.getProperty("user.dir") + "/WebContent/" + uri);
    File file = new File(System.getProperty("user.dir") + "/WebContent/" + uri);
    System.out.println(file.exists());
    if(file.exists()){
      try{
        //返回目标资源数据
        FileInputStream fileInputStream = new FileInputStream(file);
        byte[] bytes = new byte[(int)file.length()];
        fileInputStream.read(bytes);
        String result = new String(bytes);
        System.out.println(result);
        String response = getResponseMessage("200", result);
        System.out.println(response);
        this.outputStream.write(response.getBytes());
      
      }catch(Exception e){
        e.printStackTrace();
      }
    }else{
      //返回404
      try{
        String error = getResponseMessage("404", "404 Not Found");
        this.outputStream.write(error.getBytes());
      }catch(Exception e){
        e.printStackTrace();
      }
      
    }
  }

  public String getResponseMessage(String code, String message){
    return "HTTP/1.1 " + code + "\r\n"
      + "Content-type: text/html\r\n"
      + "Content-Length: " + message.length()
      + "\r\n"
      + "\r\n"
      + message;
  }
}
