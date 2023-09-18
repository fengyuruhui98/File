package com.zdd.OOP.interf;


//抽象的思维
public interface UserService {
    //接口中的所有定义其实都是抽象的public abstract
    public abstract void run();

    //接口中的属性是常量
    public static final int age = 9;

    void add(String name);
    void del(String name);
    void update(String name);
    void sear(String name);
}
