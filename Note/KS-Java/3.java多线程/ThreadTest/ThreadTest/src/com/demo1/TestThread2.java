package com.demo1;

import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
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

        testThread1.start();
        testThread2.start();
        testThread3.start();
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