package com.demo1;

public class TestThread3 implements Runnable{
    @Override
    public void run() {
        //run方法线程体
        for (int i = 0; i < 20; i++) {
            System.out.println("son " + i);
        }
    }

    public static void main(String[] args) {
        TestThread3 testThread3 = new TestThread3();
        //创建线程对象，通过线程对象来开启我们的线程---代理
        //Thread thread = new Thread(testThread3);
        //thread.start();
        //上面两行可以简写为：
        new Thread(testThread3).start();
        //main线程，主线程
        for (int i = 0; i < 2000; i++) {
            System.out.println("main " + i);
        }
    }
}
