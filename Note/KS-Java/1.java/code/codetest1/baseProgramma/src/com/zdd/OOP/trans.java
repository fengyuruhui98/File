package com.zdd.OOP;

//引用传递：对象，本质还是值传递
public class trans {
    public static void main(String[] args) {
        Stu stud = new Stu();
        System.out.println(stud.name);
        trans.change(stud);
        System.out.println(stud.name);
    }

    public static void change(Stu a){
        a.name = "ijed";
    }
}

//定义了一个类
class Stu{
    String name;
}