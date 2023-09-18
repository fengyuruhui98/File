package com.zdd.OOP.demo2;

public class Student {
    String name;
    int age;

    //一个类，即使什么都不写，也会存在一个构造类
    //1. 必须和类的名字相同
    //2. 必须没有返回值类型，也不能写void

    //显式定义构造器
    //1. 使用new关键字必须要有构造器，本质是调用构造器
    //2. 用来初始化值
    public Student(){
        //实例化初始值
        this.name = "jh";
    }

    //一但定义了有参构造，无参构造就必须显式定义
    public Student(int a){
        this.age = a;
    }

    public void study(){
        System.out.println(this.name + "study");
    }
}
