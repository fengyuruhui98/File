package com.demo1;

//多个线程同时操作同一个对象
//买火车票
//多个线程操作同一个资源情况下线程不安全，线程紊乱
public class TestThread4 implements Runnable{
    //票数
    private int ticketCount = 10;
    @Override
    public void run() {
        while(true){
            if(ticketCount <= 0){
                break;
            }
            //模拟延时
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName() + "拿到了第" + ticketCount-- + "张票");
        }
    }

    public static void main(String[] args) {
        TestThread4 ticket = new TestThread4();

        new Thread(ticket,"笔记本").start();
        new Thread(ticket,"洛克").start();
        new Thread(ticket,"颗粒剂").start();
    }
}
