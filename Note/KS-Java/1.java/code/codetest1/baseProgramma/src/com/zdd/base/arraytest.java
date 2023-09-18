package com.zdd.base;
import java.util.Arrays;

public class arraytest {
    public static void main(String[] args) {
        int[] arrays = {1,2,3,4,5};
        for(int i = 0; i < arrays.length; i++){
            System.out.println(arrays[i] + "--");
        }
        //JDK1.5 省去下标
        for (int array : arrays) {
            System.out.println(array);
        }
        PR(arrays);
        PR(new String[]{"-", "-", "-"});
        PR(Turn(arrays));

        int[][] array = {{1,2},{2,3},{3,4}};

        int[] a = {1,789,46,57,7678768,4536,4526};
        PR(new String[]{"-", "-", "-"});
        System.out.println(a); //[I@16d3586
        System.out.println(Arrays.toString(a));
        //Arrays.sort(a);
        so(a);
        System.out.println(Arrays.toString(a));
        Arrays.fill(a, 2,3,0);
        int[][] test = new int[23][34];
        for (int i = 0; i < 23; i++) {
            Arrays.fill(test[i],0);
        }
        test[21][4] = 2;
        test[3][4] = 1;
        for(int i = 0; i < tr(test).length; i++){
            PR(tr(test)[i]);
        }
    }
    public static void PR(int[] array){
        System.out.print("[");
        for (int i = 0; i < array.length; i++) {
            System.out.print(array[i]);
            if(i < array.length - 1)System.out.print(", ");
        }
        System.out.print("]");
        System.out.println("");
    }
    public static void PR(String[] array){
        for (String i : array) {
            System.out.print(i);

        }
        System.out.println();
    }
    public static int[] Turn(int[] arrays){
        int[] result = new int[arrays.length];
        for (int i = 0; i < arrays.length; i++) {
            result[i] = arrays[arrays.length - i - 1];
        }
        return result;
    }

    public static void so(int[] a){
        for(int i = 0; i < a.length - 1; i++){
            boolean f = false;
            for(int j = 0; j < a.length - 1 - i; j++){
                if(a[j +1] < a[j]){
                    int te = a[j + 1];
                    a[j + 1] = a[j];
                    a[j] = te;
                    f = true;
                }
            }
            if(!f) break;
        }
    }

    //转换稀疏数组
    public static int[][] tr(int[][] a){
        int co = 0;
        for(int i = 0; i < a.length; i++){
            for (int j = 0; j < a[i].length; j++) {
                co += a[i][j] == 0 ? 0 : 1;
            }
        }
        int[][] an = new int[co + 1][3];
        an[0][0] = a.length;
        an[0][1] = a[0].length;
        an[0][2] = co;
        int row = 1;

        for(int i = 0; i < a.length; i++){
            for (int j = 0; j < a[i].length; j++) {
                if(a[i][j] != 0){
                    an[row][0] = i;
                    an[row][1] = j;
                    an[row][2] = a[i][j];
                    row++;
                }
            }
        }

        return an;
    }
}
