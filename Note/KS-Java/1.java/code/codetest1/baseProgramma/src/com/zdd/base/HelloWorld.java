package com.zdd.base;

public class HelloWorld {
    public static void main(String[] args){
        System.out.println("Hello, World");
        //float 有限 离散 舍入误差 大约 接近但不等于
        //double
        //java中最好完全使用浮点数进行比较
        /**
         * 银行业务：
         * BigDecimal 数学工具类
         */

        float f = 0.1f;
        double f1 = 1.0/10;
        System.out.println(f == f1);

        float f2 = 23131312312312313f;
        float f3 = f2 + 1;
        System.out.println(f2 == f3);

        char c3 = '\u0061';
        System.out.println(c3);

        String sa = new String("Hello");
        String sb = new String("Hello");
        System.out.println(sa == sb);
        System.out.println(sa.equals(sb));

        String sc = "Hello";
        String sd = "Hello";
        System.out.println(sc == sd);

        int i = 128;
        byte t = (byte)i;//内存溢出
        System.out.println(t);

        System.out.println((int)23.7);
        System.out.println((int)-45.89f);

        char c = 'a';
        int d = c + 1;
        System.out.println((char)d);

        //操作比较大的数，注意溢出问题
        //JDK7新特性：数字之间可以用下划线进行分割
        int money = 10_0000_0000;
        int years = 20;
        int total = money * years; //-1474836480 计算溢出
        int total1 = money * years; //-1474836480 计算溢出
        long total3 = money * (long)years;
        System.out.println(money);//默认是int，转换之前已经存在问题了，先用int类型缓存存结果，再转移到long里
        System.out.println(total);
        System.out.println(total1);
        System.out.println(total3);
    }
}
