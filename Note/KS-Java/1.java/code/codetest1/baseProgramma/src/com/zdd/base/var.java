package com.zdd.base;

public class var {
    //类变量
    static int salary = 2500;

    //实例变量：从属于对象, 如果不自行初始化，这个类型的默认值 0 0.0 null false；
    //布尔值默认为false
    //除了基本类型，其余的默认值都是null
    String name;
    int age;

    public static void main(String[] args){
        String name = "李莲花";

        var va = new var();
        System.out.println(va.name);
        System.out.println(va.age);

        System.out.println(salary);
        //静态变量随类一起出现一起消失，不用new。
    }

    public void add(){
        //局部变量: 必须声明和初始化值。
    }
}
