package com.zdd.OOP.extend;

public class Applacations {
    public static void main(String[] args) {
        students stu = new students();//无参构造执行先执行父类再执行子类
        stu.say();
        stu.test("ve");
        stu.test1();



        //静态方法与非静态方法区别很大

        //方法的调用只和左边有关
        A a = new A();
        a.test();

        //父类的引用指向了子类
        B b = new A();//子类重写了父类的方法
        b.test();
    }
}
