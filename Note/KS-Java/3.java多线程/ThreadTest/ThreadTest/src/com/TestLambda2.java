package com;

public class TestLambda2 {
    static class Love implements ILove{
        @Override
        public void love(int a) {
            System.out.println("I love you - 1");
        }
    }

    public static void main(String[] args) {

        class Love implements ILove{
            @Override
            public void love(int a) {
                System.out.println("I love you - 1");
            }
        }

        ILove love = new Love();
        love.love(2);

        ILove love1 = new ILove(){
            @Override
            public void love(int a) {
                System.out.println("I love you - 1");
            }
        };

        ILove love2 = (int a) -> {
            System.out.println("I love you - 1");
        };

        //1. 简化1：去掉参数类型
        love = (a) -> {
            System.out.println("dojio");
        };
        //2. 简化2：去掉括号
        love = a -> {
            System.out.println("dojio");
        };
        //3. 简化3：去掉花括号
        love = a -> System.out.println("dojio");
    }
}

interface ILove{
    void love(int a);
}

class Love implements ILove{
    @Override
    public void love(int a) {
        System.out.println("I love you - 1");
    }
}