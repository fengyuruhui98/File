package com.zdd.base;

import java.util.Scanner;

public class Scanner1 {
    public static void main(String[] args){
        //Scanner sc = new Scanner(System.in);
        System.out.println("使用next方式接收：");

        //判断用户是否有输入
//        if(sc.hasNext()){
//            String str = sc.next();
//            System.out.println(str);
//        }
        //if(sc.hasNextLine()){
            //String str = sc.nextLine();
            //System.out.println(str);
        //}
        //IO流用完都要关掉
       // sc.close();
        int o = get();
    }
    //键盘接收数据
    public static int get(){

        int i = 9;
        float g = 2F;


        Scanner sc1 = new Scanner(System.in);
        System.out.println("请输入整数:");
        if(sc1.hasNextInt()){
            i = sc1.nextInt();
            System.out.println("整数" + i);
        }else if(sc1.hasNextFloat()){
            g = sc1.nextFloat();
            System.out.println("浮点数：" + g);
        }

        sc1.close();
        return 0;
    }
}
