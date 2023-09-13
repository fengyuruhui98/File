package com.zdd.base;

import java.util.Scanner;

public class suav {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        double sum = 0;
        int count =  0;

        while(sc.hasNextDouble()){
            sum += sc.nextDouble();
            count++;
        }
        sc.close();
    }
}
