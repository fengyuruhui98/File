package com.zdd.OOP.demo4;

public class Applications {
    public static void main(String[] args) {
        //一个对象的实际类型是确定的
        //可以指向的引用类型就不确定了：父类的引用指向子类

        //Stud 可以调用的方法都是自己的或者继承父亲的
        Stud s1 = new Stud();
        //父亲类型，可以指向子类型，但是不能调用子类型##独有##的方法
        Persoon s2 = new Stud();
        Object s3 = new Stud();

        s2.run();//子类重写了父类的方法，执行子类的方法
        ((Stud) s2).eat();//对象能执行哪些方法，主要看对象左边类型，和右边关系不大。

        Object object = new Stud();
        System.out.println(object instanceof Stud);
        System.out.println(object instanceof Persoon);
        System.out.println(object instanceof Object);
        System.out.println(object instanceof String);
    }
}
