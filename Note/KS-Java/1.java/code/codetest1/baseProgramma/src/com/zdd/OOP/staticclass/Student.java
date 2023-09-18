package com.zdd.OOP.staticclass;

public class Student {
    private static String name;//静态变量只有一个被类的所有实例共享。
    private double score;

    public static void run(){

    }

    public void go(){

    }

    public static void main(String[] args) {
        Student student = new Student();

        System.out.println(Student.name);
        //System.out.println(Student.score);
        System.out.println(student.name);
        System.out.println(student.score);

        test te = new test();
        test te2 = new test();
        te.a = 3;
        System.out.println(te2.a);//静态属性只有一个被类的所有实例共享。

        Student.run();
        //go();static方法跟类一块加载，加载之前都没有go方法
    }
}
