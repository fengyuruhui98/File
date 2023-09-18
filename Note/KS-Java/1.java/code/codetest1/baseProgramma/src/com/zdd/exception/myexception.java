package com.zdd.exception;

public class myexception extends Exception {
    //传递数字 >10
    private int detail;

    public myexception(int a) {
        this.detail = a;
    }

    //异常打印信息
    @Override
    public String toString() {
        return "myexception{" +
                "detail=" + detail +
                '}';
    }
}
