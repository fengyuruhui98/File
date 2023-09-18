package com.zdd.exception;

public class test {
    public static void main(String[] args) {
        int a = 1;
        int b = 0;

        //捕获异常要从小到大
        try{
            //主动抛出异常： throw throws
            if(b == 0){
                throw new ArithmeticException();
            }

            System.out.println(a/b);
            //new test().a();
        }catch(Error e) {
            System.out.println(e);
        }catch(ArithmeticException e){ //Throwable捕获最大最上层的异常
            System.out.println(e);
        }catch(Exception e){
            System.out.println(e);
        }catch(Throwable e){//最大的写在最后面
            System.out.println(e);
        } finally{//善后工作，关闭IO流
            System.out.println("finally");
        }

        try {
            new test().test1(1, 0 );
        } catch (ArithmeticException e) {
            e.printStackTrace();
        }
    }

    public void a(){
        try {
            b();
        } catch (Exception e) {
            e.printStackTrace();//打印错误栈信息
        }
    };
    public void b(){a();};

    //假设这个方法处理不了该异常。方法上抛出异常
    public void test1(int a, int b) throws ArithmeticException{
        //主动抛出异常： throw throws
        //一般在方法中
        if(b == 0){
            throw new ArithmeticException();
        }
    }
}
