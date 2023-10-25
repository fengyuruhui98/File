package zdd;

import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.net.URL;

public class TestThread2 extends  Thread{
    private String url;
    private String fileDir;

    public TestThread2(String url, String fileDir){
        this.url = url;
        this.fileDir = fileDir;
    }

    @Override
    public void run() {
        WebDownloader webDownloader = new WebDownloader();
        webDownloader.downloader(url, fileDir);
        System.out.println("已下载：" + fileDir);
    }

    public static void main(String[] args) {
        TestThread2 testThread1 = new TestThread2("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","1.jpg");
        TestThread2 testThread2 = new TestThread2("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","2.jpg");
        TestThread2 testThread3 = new TestThread2("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","3.jpg");

//        testThread1.start();
//        testThread2.start();
//        testThread3.start();

        String dir = "J:\\source\\";
        findFile fi = new findFile();
        fi.finds(dir);
    }
}

class WebDownloader{
    public void downloader(String url, String name){
        try {
            FileUtils.copyURLToFile(new URL(url), new File(name));
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("downloader出现问题");
        }
    }
}

class findFile{
    public void finds(String dir){
        File file = new File(dir);
        File[] fileName = file.listFiles();
        for(File f : fileName){
            String [] name = f.toString().split("\\\\");
            String [] id = name[2].split("\\.");
            if(id.length >= 2){
                if(id[1].equals("mp4") || id[1].equals("wmv") || id[1].equals("mkv")) {
                    String u = "https://cdn82.akamai-content-network.com/" + id[0] + "/cover.jpg?class=normal&quot";
                    TestThread2 threadGet = new TestThread2(u,"J:\\javGet\\" + id[0] + ".png");
                    threadGet.start();
                    //writeMd wr = new writeMd();
                    //wr.wrmd(id[0]);
                }
            }
        }
    }
}

class writeMd{
    public void wrmd(String code){
        //System.out.println(code);
        String [] name = code.split("-");
        StringBuilder sb = new StringBuilder();
        sb.append("#[");
        sb.append(code);
        sb.append("](./../Fire--/");
        sb.append(name[0]);
        sb.append("/");
        sb.append(code);
        sb.append(".mp4)");
        sb.append("\n");
        sb.append("![");
        sb.append(code);
        sb.append("][");
        sb.append(code);
        sb.append("]");
        sb.append("\n");
        sb.append("\n");
        sb.append("[");
        sb.append(code);
        sb.append("]:./Pics/");
        sb.append(name[0]);
        sb.append("/");
        sb.append(code);
        sb.append(".PNG");
        sb.append("\n");
        System.out.println(sb);

        File file = new File("J:\\out.txt");
        Writer out=null;

        try {
            out=new FileWriter(file, true);
            out.write(sb.toString());
            out.write("\n");
            out.flush();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
