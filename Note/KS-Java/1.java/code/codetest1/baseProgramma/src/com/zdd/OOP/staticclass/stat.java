package com.zdd.OOP.staticclass;

import static java.lang.Math.random;

public class stat {

    //可以用来赋初始值
    {
        System.out.println("匿名代码块");
    }

    static{
        System.out.println("静态代码块");
    }

    public stat(){
        System.out.println("构造方法");
    }

    public static void main(String[] args) {
        stat stat1 = new stat();
        System.out.println("==============");
        stat stat2 = new stat();
        System.out.println(random());
    }
}
