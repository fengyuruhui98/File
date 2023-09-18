package com.zdd.OOP.InnerClass;

public class Outer {
    private int id = 10;
    public void out(){
        System.out.println("外部类方法");
    }

    public class Inner{
        public void in(){
            System.out.println("内部类方法");
        }

        //内部类可以访问外部类的一些私有属性
        public void getId(){
            out();
            System.out.println(id);
        }
    }

    //静态内部类，拿不到private属性
    public static class Inner2{
        public void in(){
            System.out.println("内部类方法");
        }
    }

    //局部内部类
    public void method(){
        class Inner{

        }
    }
}

//一个java文件中可以有多个class类，但是只能有一个public class
class A{
    public static void main(String[] args) {

    }
}
