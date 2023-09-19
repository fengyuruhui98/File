package com.demo1;

//方式1
public class TestThread1 extends Thread{

    @Override
    public void run() {
        //run方法线程体
        for (int i = 0; i < 20; i++) {
            System.out.println("son " + i);
        }
    }

    public static void main(String[] args) {
        TestThread1 testThread1 = new TestThread1();
        testThread1.start();
        //main线程，主线程
        for (int i = 0; i < 2000; i++) {
            System.out.println("main " + i);
        }
    }
}
