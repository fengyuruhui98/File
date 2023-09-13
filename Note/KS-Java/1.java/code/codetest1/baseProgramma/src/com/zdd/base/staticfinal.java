package com.zdd.base;

public class staticfinal {
    //修饰符不存在先后顺序
    static final double PI = 3.14;
    final static double P = 3.56;

    public static void main(String[] args[]){
        System.out.println(PI);
        int a = 10;
        int b = 20;
        System.out.println("" + a + b);
        System.out.println(a + b + "");
    }
}
