package com;


//静态代理模式
public class StaticProxy {
    public static void main(String[] args) {
        You you = new You();

        new Thread( () -> System.out.println("I love You")).start();

        new WeddingCompany(new You()).HappyMarry();
        //下面两行精简为上面
        //WeddingCompany weddingCompany = new WeddingCompany(you);
        //weddingCompany.HappyMarry();
    }

}

interface Marry{
    void HappyMarry();
}

//真实角色, 去Marry
class You implements Marry{
    @Override
    public void HappyMarry() {
        System.out.println("Marry");
    }
}

//代理角色, 帮助Marry
class WeddingCompany implements Marry{
    //代理谁 -> 真实目标角色
    private Marry target;

    public WeddingCompany(Marry target) {
        this.target = target;
    }

    @Override
    public void HappyMarry() {
        beforeMarry();
        this.target.HappyMarry(); //真实对象
        after();
    }

    private void after() {
        System.out.println("城内");
    }

    private void beforeMarry(){
        System.out.println("城外");
    }
}