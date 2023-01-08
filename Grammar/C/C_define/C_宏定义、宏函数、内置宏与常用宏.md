# C语言宏定义、宏函数、内置宏与常量宏
前言：  
在C语言中，变量类型、循环控制、基础语法等与其他高级语言基本无异；而C语言（C++）特有的两把双刃剑指针和宏定义/宏函数使得C语言在底层开发中披荆斩棘、无所不能。这两个概念涉及范围比较广，其分支点也比较多，可谓星罗棋布，但这每颗星都足以照亮C语言因其开发周期、可维护性、可移植性等问题而显的黯淡的天空，使得这门语言灵活多变、操作犀利，令人难以揣摩却也深深着迷。

首先，C的第一把双刃剑：指针，这也是所有学C的人，最先会接触、最多接触也最无法避免、也是最为之魂牵梦萦茶饭不思的概念；包括一级指针、二级指针、数组指针、指针数组、函数指针、甚至函数指针数组、函数指针数组指针等，怪异称呼说不尽道不清，指东指西、指其所想指，所能指、所不能指。尤其是函数指针、以及函数指针数组等在C的高级特性中（一般是用来进行适配层函数挂载，驱动分发）时常会被用到。

而宏则是更为锋利的一把双刃剑，由于疏于习练，至今尚未参透其中奥秘。今天且稍作总结，记录学历过程之烦恼万千与趣味无穷。

关于宏的内容大部分摘自：[C语言宏定义,内置宏,FILE,LINE,## 用法](https://blog.csdn.net/tomtc123/article/details/8875468)

# 一、下面列举一些成熟软件中常用的宏定义：
## 1，防止一个头文件被重复包含
```c
#ifndef COMDEF_H
#define COMDEF_H

//头文件内容 …
#endif
```
## 2，重新定义一些类型，防止由于各种平台和编译器的不同，而产生的类型字节数差异，方便移植。
```c
typedef  unsigned long int  uint32;      	/* Unsigned 32 bit value */
```
## 3，得到指定地址上的一个字节或字
```c
#define  MEM_B( x )  ( *( (byte *) (x) ) )
#define  MEM_W( x )  ( *( (word *) (x) ) )
```
## 4，求最大值和最小值
```c
#define  MAX( x, y )  ( ((x) > (y)) ? (x) : (y) )
#define  MIN( x, y )  ( ((x) < (y)) ? (x) : (y) )
```
## 5，得到一个field在结构体(struct)中的偏移量
```c
#define FPOS( type, field )   ( (dword) &(( type *) 0)-> field )
```
## 6,得到一个结构体中field所占用的字节数
```c
#define FSIZ( type, field ) sizeof( ((type *) 0)->field )
```
## 7，按照LSB格式把两个字节转化为一个word
```c
#define  FLIPW( ray ) ( (((word) (ray)[0]) * 256) + (ray)[1] )
```
## 8，按照LSB格式把一个word转化为两个字节
```c
#define  FLOPW( ray, val ) \
(ray)[0] = ((val) / 256); \
(ray)[1] = ((val) & 0xFF)
```
## 9，得到一个变量的地址（word宽度）
```c
#define  B_PTR( var )  ( (byte *) (void *) &(var) )
#define  W_PTR( var )  ( (word *) (void *) &(var) )
```
## 10，得到一个字的高位和低位字节
```c
#define  WORD_LO(xxx)  ((byte) ((word)(var) & 255))
#define  WORD_HI(xxx)  ((byte) ((word)(var) >> 8))
```
## 11，返回一个比X大的最接近的8的倍数
```c
#define RND8( x )       ((((x) + 7) / 8 ) * 8 )
```
## 12，将一个字母转换为大写
```c
#define  UPCASE( ch ) ( ((ch) >= ’a' && (ch) <= ’z') ? ((ch) - 0×20) : (ch) )
```
## 13，判断字符是不是10进值的数字
```c
#define  DECCHK( ch ) ((ch) >= ’0′ && (ch) <= ’9′)
```
## 14，判断字符是不是16进值的数字
```c
#define  HEXCHK( ch ) \
(((ch) >= ’0′ && (ch) <= ’9′) || \
((ch) >= ’A' && (ch) <= ’F') || \
((ch) >= ’a' && (ch) <= ’f') )
```
## 15，防止溢出的一个方法
```c
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val))
```
## 16，返回数组元素的个数
```c
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) )
```
## 17，对于IO空间映射在存储空间的结构，输入输出处理
```c
#define inp(port)         (*((volatile byte *) (port)))
#define inpw(port)        (*((volatile word *) (port)))
#define inpdw(port)       (*((volatile dword *)(port)))

#define outp(port, val)   (*((volatile byte *) (port)) = ((byte) (val)))
#define outpw(port, val)  (*((volatile word *) (port)) = ((word) (val)))
#define outpdw(port, val) (*((volatile dword *) (port)) = ((dword) (val)))
```
# 二、使用一些内置宏跟踪调试：
ANSI标准定义了几个个预定义的宏名。它们包括但不止于：
```c
__LINE__
__FILE__
__DATE__
__TIME__
__STDC__
```
注： 常用的还有__FUNCTION__等【非标准】，详细信息可查看：[Predefined Macros](https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?redirectedfrom=MSDN&view=msvc-170)，如果编译不是标准的，则可能仅支持以上宏名中的几个，或根本不支持。记住编译程序 也许还提供其它预定义的宏名。

\\是行连接符，会将下一行和前一行连接成为一行，即将物理上的两行连接成逻辑上的一行  
__FILE__ 是内置宏 代表源文件的文件名  
__LINE__ 是内置宏，代表该行代码的所在行号  
__DATE__宏指令含有形式为月/日/年的串，表示源文件被翻译到代码时的日期。  
源代码翻译到目标代码的时间作为串包含在__TIME__ 中。串形式为时：分：秒。  
如果实现是标准的，则宏__STDC__含有十进制常量1。如果它含有任何其它数，则实现是非标准的。

可以定义宏，例如:当定义了_DEBUG，输出数据信息和所在文件所在行
```c
#ifdef _DEBUG
#define DEBUGMSG(msg,date) printf(msg);printf("%d%d%d", date, __LINE__, __FILE__)
#else
#define DEBUGMSG(msg,date)
#endif
```
# 三、宏定义防止使用时错误：
## ①用小括号包含。
```c
//例如：
#define ADD(a,b) （a+b）
```
## ②用do{}while(0)语句包含多语句防止错误（注意while(0)后无分号）.
```c
//例如：
#difne DO(a,b) a+b; a++;
//应写成：
#difne DO(a,b) do{a+b; a++;}while(0)
```
为什么需要do{…}while(0)形式?大致有以下几个原因：

- 1.空的宏定义避免warning:  
#define foo() do{}while(0)

- 2.存在一个独立的block，可以用来进行重复性变量定义，进行比较复杂的实现。

- 3.如果出现在判断语句过后的宏，这样可以保证作为一个整体来是实现：
    ```c
    #define foo(x)
    action1();
    action2();
    //在以下情况下：
    if(NULL == pPointer)
        foo();
    //就会出现action2必然被执行的情况，而这显然不是程序设计的目的。
    ```
- 4.以上的第3种情况用单独的{}也可以实现，但是为什么一定要一个do{}while(0)呢，看以下代码：
    ```c
    #define switch(x,y) {int tmp; tmp=x;x=y;y=tmp;}
    if(x>y)
    switch(x,y);
    else        //error, parse error before else
    otheraction();
    ```
    在把宏引入代码中，会多出一个分号，从而会报错。使用do{….}while(0) 把它包裹起来，成为一个独立的语法单元，从而不会与上下文发生混淆。同时因为绝大多数的编译器都能够识别do{…}while(0)这种无用的循环并进行优化，所以使用这种方法也不会导致程序的性能降低，**【但是并非所有情况都用这种形式，有些情况不需要，有些情况则不能够够】**。

# 四、宏中#和##的用法
## 1、一般用法
我们使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起.例如：
```c
＃include<cstdio>
＃include<climits>
using namespace std;

#define STR(s)     #s
#define CONS(a,b)  int(a##e##b)

int main()
{
	/* 输出字符串vck */ 
	printf(STR(vck));						
	
	/* 2e3 输出:2000 */ 
	printf("%d\n", CONS(2,3));					
	return 0;
}
```
```c
#define P(A) printf("%s:%d\n",#A,A);
int main(int argc, char **argv)
{
    int a = 1, b = 2;
    P(a);
    P(b);
    P(a+b);
    system("pause");
}
```
>a:1  
b:2  
a+b:3
```c
#define SQUARE(x) printf("The square of "#x" is %d.\n", ((x)*(x)));
SQUARE(8)
```
>The square of 8 is 64

## 2、当宏参数是另一个宏的时候

需要注意的是凡宏定义里有用#或##的地方宏参数是不会再展开.

### （1）没有’#'和’##’的情况
```c
#define TOW      (2)
#define MUL(a,b) (a*b)

printf("%d*%d=%d\n", TOW, TOW, MUL(TOW,TOW));
```
这行的宏会被展开为：
```c
printf("%d*%d=%d\n", (2), (2), ((2)*(2)));
```
MUL里的参数TOW会被展开为(2).

### （2）当有’#'或’##’的时候
```c
#define A          (2)
#define STR(s)     #s
#define CONS(a,b)  int(a##e##b)

printf("int max: %sn",  STR(INT_MAX));    // INT_MAX ＃include<climits>
printf("%s\n", CONS(A, A));               // compile error
```
第一个printf()这行会被展开为：
```c
printf(“int max: %s\n”, #INT_MAX);
```
第二个printf()则是：
```c
printf("%s\n", int(AeA)); //编译错误
```

INT_MAX和A都不会再被展开, 然而解决这个问题的方法很简单. 加多一层中间转换宏；加这层宏的用意是把所有宏的参数在这层里全部展开, 那么在转换宏里的那一个宏(_STR)就能得到正确的宏参数.
```c
#define A           (2)
#define _STR(s)     #s
#define STR(s)      _STR(s)                 // 转换宏
#define _CONS(a,b)  int(a##e##b)
#define CONS(a,b)   _CONS(a,b)       		// 转换宏
```
```c
printf(“int max: %s\n”, STR(INT_MAX));
```
输出为: int max: 0x7fffffff

`STR(INT_MAX) –> _STR(0x7fffffff) 然后再转换成字符串；`

```c
printf("%d\n", CONS(A, A));
```
输出为：200

`CONS(A, A) –> _CONS((2), (2)) –> int((2)e(2))`

### （3）’#'和’##’的一些应用特例：

#### **a、合并匿名变量名，例：**
```c
#define  __ANONYMOUS1(type, var, line)  type  var##line
#define  _ANONYMOUS0(type, line)  __ANONYMOUS1(type, _anonymous, line)
#define  ANONYMOUS(type)  _ANONYMOUS0(type, __LINE__)

ANONYMOUS(static int);
//即: 
static int _anonymous70;  	//70表示该行行号；
```
    ①第一层：ANONYMOUS(static int); –> __ANONYMOUS0(static int, LINE);  
    ②第二层：–> ___ANONYMOUS1(static int, _anonymous, 70);  
    ③第三层：–> static int _anonymous70;  
    即每次只能解开当前层的宏，所以__LINE__在第二层才能被解开；

#### **b、填充结构**
```c
#define  FILL(a)   {a, #a}

enum IDD{OPEN, CLOSE};
typedef struct MSG{
	IDD id;
	const char * msg;
}MSG;
```
```c
MSG _msg[] = {
	FILL(OPEN), 
	FILL(CLOSE)
};
//相当于：
MSG _msg[] = {
	{OPEN, “OPEN”},
	{CLOSE, ”CLOSE“}
};
```
#### **c、记录文件名**
```c
#define  _GET_FILE_NAME(f)   #f
#define  GET_FILE_NAME(f)    _GET_FILE_NAME(f)
static char  FILE_NAME[] = GET_FILE_NAME(__FILE__);
```
#### **d、得到一个数值类型所对应的字符串缓冲大小**
```c
#define  _TYPE_BUF_SIZE(type)  sizeof #type
#define  TYPE_BUF_SIZE(type)   _TYPE_BUF_SIZE(type)
char  buf[TYPE_BUF_SIZE(INT_MAX)];
```
```c
char buf[_TYPE_BUF_SIZE(“0x7fffffff”)];
char buf[sizeof “0x7fffffff”];
```
这里相当于：
```c
char buf[11];
```
# 五、__VA_ARGS__与##__VA_ARGS__

- __VA_ARGS__ 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。
- ##__VA_ARGS__ 宏，在__VA_ARGS__前面加上##的作用在于，当可变参数的个数为0时，这里的##起到把前面多余的逗号去掉的作用,否则会编译出错。（摘自：#、##、__VA_ARGS__和##__VA_ARGS__的作用）

```c
#define MODULE_NAME "MY_LIBS"
 
#define error_printf(fmt,...) printf("[ERROR]["MODULE_NAME"](%s|%d)"fmt,__func__,__LINE__,##__VA_ARGS__)
```

使用例如：[Linux C实现文件拷贝可变色进度条显示](https://blog.csdn.net/Apollon_krj/article/details/82908432)
