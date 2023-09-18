package com.zdd.OOP;

public class demo01 {
    public static void main(String[] args) {
        //静态方法
        stu.say();
        //stu.s();
        //非静态方法要先进行实例化这个类 new
        stu s = new stu();
        s.s();

    }

    //当a是静态方法而b不是静态方法时，a不可以直接调用b
    //当a与b不是静态方法，可以互相调用
    //当a与b都是静态方法，也可以互相进行调用

    //和类一起加载
    public void a(){
        b();
    }
    //类实例化之后，对象创建之后
    public void b(){

    }


}
