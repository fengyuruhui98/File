package com.zdd.exception;

public class TestMyException {
    public static void main(String[] args) {
        try {
            test(22);
        } catch (myexception e) {
            System.out.println(e);
        }
    }
    static void test(int a) throws myexception {
        if(a > 10){
            throw new myexception(a);
        }
    }
}
