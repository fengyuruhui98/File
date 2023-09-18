package com.zdd.OOP.extend;

public class students extends Person{
    private String name = "lo";

    public students(){
        //此处相当于调用了super()

        super();
        System.out.println("子类的无参构造执行");//若显式super并将顺序调换，会报错
    }

    public void pr(){
        System.out.println("Student");
    }

    public void test1(){
        pr();
        this.pr();
        super.pr();
    }

    public void test(String name){
        System.out.println(name);
        System.out.println(this.name);
        System.out.println(super.name);
    }

}
