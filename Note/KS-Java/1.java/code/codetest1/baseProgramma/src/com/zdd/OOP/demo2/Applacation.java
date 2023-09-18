package com.zdd.OOP.demo2;

import com.zdd.OOP.demo3.Pet;

public class Applacation {
    public static void main(String[] args) {
        //类是抽象的需要实例化
        //类实例化之后会返回一个自己的对象
        //student对象就是一个Studentleukemia的具体实例

        //使用new的时候，除了分配内存空间，还会给创建好的对象进行默认的初始化以及对类中构造器的调用


        Student stu1 = new Student();
        Student stu2 = new Student();

        stu1.name = "iedh";
        stu1.age = 23;

        //
        Pet dog = new Pet();
        dog.name = "jksd";
        dog.shout();

    }
}
