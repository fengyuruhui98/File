package com.demo2;

import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.concurrent.*;

public class TestCallable1 implements Callable<Boolean> {
    private String url;
    private String fileDir;

    public TestCallable1(String url, String fileDir){
        this.url = url;
        this.fileDir = fileDir;
    }

    @Override
    public Boolean call(){
        WebDownloader webDownloader = new WebDownloader();
        webDownloader.downloader(url, fileDir);
        System.out.println("已下载：" + fileDir);
        return true;
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        TestCallable1 testThread1 = new TestCallable1("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","1.jpg");
        TestCallable1 testThread2 = new TestCallable1("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","2.jpg");
        TestCallable1 testThread3 = new TestCallable1("https://cdn82.akamai-content-network.com/IPX-817/cover.jpg?class=normal&quot","3.jpg");

        //创建执行服务
        ExecutorService ser = Executors.newFixedThreadPool(3);
        //提交执行
        Future<Boolean> result1 = ser.submit(testThread1);
        Future<Boolean> result2 = ser.submit(testThread2);
        Future<Boolean> result3 = ser.submit(testThread3);
        //获取结果
        boolean rs1 = result1.get();
        boolean rs2 = result2.get();
        boolean rs3 = result3.get();
        //关闭服务
        ser.shutdownNow();
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