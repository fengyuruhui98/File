package com;

public class Testlambda {
    //3.静态内部类
    static class Like2 implements I{
        @Override
        public void lambda() {
            System.out.println("I like lambda2");
        }
    }

    public static void main(String[] args) {
        I i = new Like();
        i.lambda();

        i = new Like2();
        i.lambda();

        //4.局部内部类
        class Like3 implements I{
            @Override
            public void lambda() {
                System.out.println("I like lambda3");
            }
        }

        i = new Like3();
        i.lambda();

        //5.匿名内部类
        i = new I(){
            @Override
            public void lambda() {
                System.out.println("I like lambda4");
            }
        };
        i.lambda();

        //6.lambda简化
        i = () -> {
            System.out.println("I like lambda5");
        };
        i.lambda();
    }
}

//1.定义一个函数式接口
interface I{
    void lambda();
}

//2.实现类
class Like implements I{
    @Override
    public void lambda() {
        System.out.println("I like lambda");
    }
}