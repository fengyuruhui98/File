首先搞清楚get和set有什么用，在面向对象编程里面，有的类的数据是私有的，是封装起来的，所以为了读取和写入对应的私有数据，c#采用了关键字get和set，其中get负责读取私有数据，set负责写入私有数据，但这两种用法应该并不绝对，我见过在get里面进行写入操作的。

我们在学习C#语法的属性时，都要首先和GET,SET访问器打交道，从英文的字面意思上理解，GET应该就是获得什么什么，而SET应该是设置什么什么，那我们看一下，官方是怎么定义这对访问器的：get是读取属性时进行的操作，set是设置属性时进行的操作。定义一个属性如果只有get，这个属性就是只读的。同样，只有set，属性就是只写的，当然只写的属性我觉的意义不是很大。

接下来我们通过一些代码实例来接触一下GET和SET访问器
```c#
class Bank
{
    private int money;//私有字段

    public int Money  //属性
    {
        //GET访问器，可以理解成另类的方法，返回已经被赋了值的私有变量money
        get { return money;  }
        //SET访问器，将我们打入的值赋给私有变量money
        set { money = value; }
    }  
}
```
我们可以打个生动的比方：

Money属性就像银行里的自动存取款机，你看不见里面的money，但你能用set(存钱），用get(取钱）。money是一个私有字段，是分装在类中的，类以外的程序不能直接访问的，类的set和get成员是外部程序访问类内部属性的唯一方法，就像你去银行取钱，你不能直接从银行的保险柜里拿到钱，而是银行营业人员把钱取出来给你的。
那我们设置了Money属性，怎么使用呢？？
```c#
class Program
{
     static void Main(string[] args)
     {
        //实例化一个Bank银行
        Bank bank = new Bank();

        //对Money属性做赋值操作，这时我们访问的是SET访问器
        bank.Money = 15;

        //对Money属性做取值操作，这时我们访问的是GET访问器
        int a = bank.Money;
     }
}
```
细心的朋友可以发现当我们设置断点在SET访问的中，执行代码 bank.Money = 15;时，就会进入我们属性中的SET访问器了。

以上就是我们的SET和GET访问器的最基本的应用了。

但是我们为什么要用GET和SET访问器呢？？
通过GET和SET对公有变量Money进行读写操作，实际就是间接更改money私有变量的值，那既然如此。为何不设money为public,让实例直接对money进行读写操作呢？我们是不是在多此一举呢？

其实，我当初刚接触GET和SET就很理解，并不是说他们的含义难理解，而是为什么要用？为何不直接用个public的字段变量来代替呢？

理解一：用了GET和SET能让赋值和取值增加限制
```c#
class Bank
{
    private int money;//私有字段

    public int Money  //属性
    {
        //GET访问器，可以理解成另类的方法，返回已经被赋了值的私有变量money
        get { return money;  }
        //SET访问器，将我们打入的值赋给私有变量money,并且加了限制，不能存负的
        set
        {
            if (value >= 0)
            {
                money = value;
            }
            else
            {
                money = 0; 
            }
        }
    }        
}
```
这样的话，我们是永远无法给Money属性赋值负的金钱了！

其实这种应用才我们的实际的项目中，属性设置中非常常见！例：
```c#
 public string ApplyStaff
 {
      //延迟加载ApplyStaff对象
      get
      {
         if (this.IsGhost)
         {
            LazyLoaderFactory.GetLoader().LoadApplicationConfirm(this);
         }
         return applyStaff;
      }
      set
      {
          //判断输入的字符的长度
          if (value.Length > 40)
          {
              throw new Exception("领用人不能超过40个字符。");
          }
          this.applyStaff = value;
      }
 }

 public string BillName
 {
     get
     {
         return billName;
     }
     set
     {
         if (value==string.Empty)
         {
             throw new Exception("单据名称不能为空");
         }
         else if (value.Length > 40)
         {
             throw new Exception("单据名称长度不能超过40个字符");
         }
         else
         {
             this.billName = value;
         }
     }
 }
```
如果在SET访问器中设置这个属性的一些特性，比如说不能为空，长度大小，必须为数字，各类正则表达式等等，然后抛出异常，表示层再来接受，这样就可以少写很多验证程序了。