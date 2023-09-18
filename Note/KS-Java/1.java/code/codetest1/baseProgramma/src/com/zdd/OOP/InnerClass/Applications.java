package com.zdd.OOP.InnerClass;

public class Applications {
    public static void main(String[] args) {
        Outer outer = new Outer();
        //通过外部类来实例化内部类
        Outer.Inner inner = outer.new Inner();
        inner.in();
        inner.getId();

        //匿名内部类：没有名字去初始化类,不用将实例保存到变量中
        new Apple().eat();
        new UserService(){
            @Override
            public void hello() {
                System.out.println("jnkj");
            }
        };
    }
}

class Apple{
    public void eat(){
        System.out.println("kh");
    }
}

interface UserService{
    void hello();
}
