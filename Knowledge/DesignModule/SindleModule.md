# 单例模式:
单例模式，顾名思义就是只有一个实例，并且她自己负责创建自己的对象，这个类提供了一种访问其唯一的对象的方式，可以直接访问，不需要实例化该类的对象

单例模式，属于创建类型

实际代码我放在了Github: https://github.com/liangtengyu/DesignPatterns-for-Java

## 应用场景:
学习一个设计模式之前 我们先了解它的使用场景能够帮我们更快的理解它,

单例模式只允许创建一个对象，因此更节省内存，加快对象访问速度，因此对象需要被公用的场合适合使用，如多个模块使用同一个数据源连接对象等等.如：

- 需要频繁实例化然后销毁的对象。
- 创建对象时耗时过多或者耗资源过多，但又经常用到的对象。
- 有状态的工具类对象。
- 频繁访问数据库或文件的对象。

## 实现方式:

### 饿汉式
```java
public class Singleton_3 {
    //使用饿汉式   线程安全
    private static Singleton_3 instance = new Singleton_3() ;

    private Singleton_3() {
    }

    public  static Singleton_3 getInstance() {

        return instance;
    }
}
```
### 懒汉式 - 线程安全
```java
public class Singleton_2 {
    //使用懒汉式  线程安全 不建议使用
    private static Singleton_2 instance =null ;

    private Singleton_2() {
    }

    public synchronized static Singleton_2 getInstance() {
        if (instance !=null) return instance;
        return new Singleton_2();
    }
}
```
### 懒汉式 - 非线程安全
```java
public class Singleton_1 {
    //使用懒汉式  非线程安全
    private static Singleton_1 instance =null ;

    private Singleton_1() {
    }

    public static Singleton_1 getInstance() {
        if (instance !=null) return instance;
        return new Singleton_1();
    }
}
```
### 静态类方式
```java
public class sigleton0 {       //使用静态类方式实现单例
    private static ConcurrentHashMap cache = new ConcurrentHashMap();

}
```
### 内部类方式
```java
public class Singleton_4 {
    //使用内部类方式构造单例, 线程安全并且懒加载
    private AtomicInteger id = new AtomicInteger(0);


    private Singleton_4() {
    }

     public static  Singleton_4 getInstance(){
        return SingletonCreator.singleton_4;
    }

    private static class SingletonCreator{
        static  Singleton_4 singleton_4 = new Singleton_4();

    }
    public Integer getIncrementId(){
        return this.id.getAndIncrement();
    }
```
### 双重校验锁方式
```java
public class Singleton_5 {
    //使用双重锁校验 线程安全
    private static Singleton_5 instance =null ;
    //满足懒加载
    private Singleton_5() {
    }

    public  static Singleton_5 getInstance() {
        if (instance !=null) return instance;
        synchronized (Singleton_5.class) {
            if (instance == null) {
                return new Singleton_5();
            }
        }
        return new Singleton_5();
    }
}
```
### 原子类方式
```java
public class Singleton_6 {
    //使用atomicrefence  使用CAS方式  支持懒加载
    private static AtomicReference<Singleton_6> INSTANCE = new AtomicReference<Singleton_6>();

    private Singleton_6() {
    }

    public static Singleton_6 getInstance(){
        for (; ; ) {
            Singleton_6 singleton_6 = INSTANCE.get();
            if (null != singleton_6)return singleton_6;
            INSTANCE.compareAndSet(null, new Singleton_6());
            return INSTANCE.get();
        }

    }
```

## 实验
每次获取单例对象的结果实际都是同一个对象
```java
public static void main(String[] args) {
    for (int i = 0; i < 100; i++) {
        //获取实例
        Singleton_4 instance = Singleton_4.getInstance();
        //输出地址
        System.out.println("实例的地址:" + instance);
        //获取id
        System.out.println(instance.getIncrementId());
        System.out.println("-------------------------------------------------");
    }
}
```
结果 :
```
实例的地址:Singleton_4@63947c6b
0
-------------------------------------------------
实例的地址:Singleton_4@63947c6b
1
-------------------------------------------------
实例的地址:Singleton_4@63947c6b
2
-------------------------------------------------
实例的地址:Singleton_4@63947c6b
3
-------------------------------------------------
  .....
```