package Note.Tomcat.手写中间件;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MyHttpServer{
  //端口
  private int port = 8080;
  //接受请求的方法
  public void receiving(){
    //创建Socket服务
    try{
      ServerSocket serverSocket = new ServerSocket(port);
      //循环接收请求
      while(true){
        //获取连接对象
        Socket socket = serverSocket.accept();
        //获取连接对象的输入流
        InputStream inputStream = socket.getInputStream();
        System.out.println(inputStream);
        //创建request对象
        MyHttpRequest request = new MyHttpRequest(inputStream);
        //解析请求
        request.parse();
        //创建Response
        OutputStream outputStream = socket.getOutputStream();
        MyHttpResponse response = new MyHttpResponse(outputStream);
        //进行响应
        response.sendRedirect(request.getUri());
      }
    }catch(Exception e){
      e.printStackTrace();
    }
  }
}