package com.zdd.base;

public class tomain {
    public static void main(String[] args) {
        for(int i = 0; i < args.length; ++i){
            System.out.println("args[" + i + "]:" + args[i]);
        }
        double[] a = {2,3.4,4};
        PM(a);
    }

    public static void PM(double... number){
        double re = number[0];
        for(int i = 0; i < number.length; ++i){
            System.out.println(number[i]);
        }
    }
}
