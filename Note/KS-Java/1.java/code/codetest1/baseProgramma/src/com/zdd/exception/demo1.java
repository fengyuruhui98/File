package com.zdd.exception;

public class demo1 {
    public static void main(String[] args) {
        System.out.println(1/0);
        new demo1().a();
    }

    public void a(){
        b();
    }
    public void b(){
        a();
    }
}
