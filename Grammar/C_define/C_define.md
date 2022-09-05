# 一．C_define简单使用方法
## 1. 最最最简单的define定义
如`#define MAX 10`，编译器在处理这个代码之前会对`MAX`进行处理，替换为10，这样的定义看起来和`const`常量很相似，但是他们还是有区别的，`#define`的定义其实就是简单的文本的替换，并不是作为一个量来使用.
## 2. 用#define来对函数进行“定义”
`#define MAX(a,b) ((a) > (b) ? (a) : (b))`，这个定义就返回两个数中较大的那个，这个”函数“没有类型检查.  
可以作为一个普通的模板来使用，没函数模板那么安全      
`#define MINUS(a,b) a - b`,这个定义在一般的使用中没问题，但是在特定的情况下使用会出现问题，如果我们要这样去使用的话，展开之后会是什么样子呢？如：`2 * MINUS(a,b) / 4`，就像我前面所说的那样，宏其实就是一个简单的文本替换，所以展开时候就变为 `2 * a - b / 4`，给原定义加一个括号就OK了，也就是`#define MINUS(a,b) (a - b)`  
再说一个经常出现的一个错误，看下面的例子：
```C
#define pin (int *)
    pin a,b;
```
本意其实都想让`a`和`b`成为int型指针，但是实际上却变成了int *a，b;a是int型指针，b是int型变量，咋处理捏？这个时候typedef就出来了，它说我可以满足define满足不了的要求，所以改成typedef pin (int *) 就OK了。  
**TIP：我们在写code的时候一定要养成一个良好的习惯和一个良好的代码编写风格，建议所有的层次都加上括号.**
## 3. define的单行定义，举例说明之，属于少见用法
```C
    #define A(x) ##x
    #define B(x) #@x
    #define C(x) #x
```
如果我们假设x=1，那么A(1)就是1，B(1)就是‘1’，C(1)就是”1“
## 4. define的多行定义
#define可以进行多行定义，虽然看起来有点蛋疼，但是确实是一个灰常经典而且在设备驱动代码中经常要用到的一个方法，格式如下：  
```C
    #define MACRO(arg1,arg2) do {  \
    test1; \
    test2; \
    }while(0)
```
TIP:在每一行的末尾要加上\，切记！
## 5. 定义宏和取消宏定义的方法
定义一个宏使用#define，取消一个宏定义使用#undef
## 6. 使用宏进行条件编译
格式如下：`#ifdef ... (#else) ... #endif`  
如：
```C
    #ifdef HELLO
    #define WORLD 1
    #else
    #define WORLD 0
    #endif
```
## 7. 用define来处理头文件被头文件或者源文件包含的情况
由于头文件包含可以嵌套，那么c文件有可能包含多次同一个头文件，就会出现重复定义的问题的，那么可以就通过条件编译开关来避免重复包含，如下：  
```C
    #ifndef _HELLO_H_
    #define _HELLO_H_
    ...
    //文件内容
    ...
#endif
```
# 二．C_define详细使用
## 1. 两种宏定义
宏在C语言中是一段有名称的代码片段。无论何时使用这个宏时，宏的内容都会被这段代码替换。主要有两种宏区别主要在使用上，一种是在使用类似于数据对象称为`Object-like`，另一种在使用时类似于函数调用称为`Function-like`。在C语言使用`#define`来定义宏，可以将任意的有效的标识符定义为宏，设置C语言的关键字也可以。但是在C语言中`defined`不可以作为宏的名称。在C++中以下的关键字也不可以作为宏的名称`and,and_eq,bitand,bitor,compl,not,not_eq,or,or_eq,xor,xor_eq`。
## 2. `Object-like`宏
可以比较简单的进行代码段的替换。这种方式常用作表示常量数字。例如：  
>#define BUFFER_SIZE 1024

使用该宏的时候就可以用来替换数字。  
>foo = (char *) malloc (BUFFER_SIZE);

预处理器将会把该宏替换为对应的数字,如下所示。
>foo = (char *) malloc (1024);

按照惯例，宏一般都写作大写字母。

--------------------------------------------
**(1)多行的宏**  
宏结束于#define的行尾，如果有必要的话，可以在末尾添加反斜杠来将宏定义成多行。
```C
#define NUMBERS 1, \
	            2, \
	            3
int x[] = { NUMBERS };
//→ int x[] = { 1, 2, 3 };
```
----------------------------------------------
**(2)多次宏定义**  
如果宏定义的代码段依然是宏的话，预处理器会继续进行宏替换的操作。
```C
#define TABLESIZE BUFSIZE
#define BUFSIZE 1024
TABLESIZE
//→ BUFSIZE
//→ 1024
```
最终TABLESIZE会被替换成1024

------------------------------------------
## 3.`Function-like`宏
宏还可以被定义成下面的形式，使用该宏的时候，类似于调用函数，这类宏的定义中，宏的名称后面紧跟一堆括号(与括号之间不能有空格)。  
```C
#define lang_init()  c_init()
lang_init()
//→ c_init()
```
调用该类宏的时候，也必须跟一个括号，如果不跟括号的话，会显示语法错误。  

-----------------------------------------------
**(1)宏的参数**  
`Function-like`宏可以接受参数，类似于真正的函数一样。参数必须是有效的C语言标识符，使用逗号隔开
```c  
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
    x = min(a, b);          //→  x = ((a) < (b) ? (a) : (b));
    y = min(1, 2);          //→  y = ((1) < (2) ? (1) : (2));
    z = min(a + 28, *p);    //→  z = ((a + 28) < (*p) ? (a + 28) : (*p));
```
在上面的例子中，`x = min(a, b)`调用宏的时候，将入参a,b替换到形参X, Y在宏内的位置，就变成了`x = ((a) < (b) ? (a) : (b))` 

-----------------------------------------------
**(2)字符串化**  
字符串化指的是，可以在宏的参数前面加入#，使入参变成字符串。  
例如：
```c
#include <stdio.h>
#define str(expr) printf("%s\r\n", #expr)
int main()
{
    str(abc);
    str(12345);
    return 0;
}
```
这里运行代码会打印：  
>abc
>12345
	 
str宏的入参，都变成了字符串打印了出来。  

------------------------------------------------
**(3)连接符号**  
在宏中，可以使用两个#将两个符号连接成一个符号。  
```c
#include <stdio.h>
#define A1 printf("print A1\r\n")
#define A2 printf("print A2\r\n")
#define A(NAME) A##NAME
int main()
{
    A(1);
    return 0;
}
```
这里会打印
>print A1

在该例子中，调用宏A(1)时，NAME为1。A##NAME这个符号连接，即将A和1连接成了一个符号A1，然后执行宏A1的内容。最终打印出来了`print A1`

------------------------------------------------
**(4)可变参数**  
定义宏可以接受可变数量的参数，类似于定义函数一样。如下就是一个例子  
```c
#include <stdio.h>
#define myprintf(...) fprintf (stderr, __VA_ARGS__)
int main()
{
    myprintf("1234\r\n");
    return 0;
}
这里会输出
>1234
```
这种形式的宏，会把…的代表的参数扩展到后面的`__VA_ARGS__`中。在该例子中，就会扩展为`fprintf(stderr, "1234\r\n")`。  
如果你的参数比较复杂，上面的`myprintf`还可以定义为如下的形式，用自定义的名称`args`来表示参数的含义：
>#define myprintf(args...) fprintf (stderr, args)

-------------------------------------------------------
**(5)预定义宏**  
*标准预定义宏*  

标准的预定义宏都是用双下划线开头和结尾，例如`__FILE__`和`__LINE__`，表示文件的名称和该行代码的行号。
```c
#include <stdio.h>
int main()
{
    printf("FILE:%s,LINE:%d\r\n",__FILE__, __LINE__);
    printf("DATA:%s\r\n",__DATE__);
    printf("TIME:%s\r\n",__TIME__);
    printf("STDC:%d\r\n",__STDC__);
    printf("STDC_VERSION:%d\r\n",__STDC_VERSION__);
    printf("STDC_HOSTED:%d\r\n",__STDC_HOSTED__);
#ifdef __cplusplus
    printf("cplusplus:%d\r\n", __cplusplus);    
#else
    printf("complied by c\r\n");    
#endif
    return 0;
}
```
输出如下
>FILE:macro.c,LINE:5  
>DATA:Jan 13 2019  
>TIME:21:41:14  
>STDC:1  
>STDC_VERSION:201112  
>STDC_HOSTED:1  
>complied by c

本文件名为 `macro.c`，并且该行代码为第5行。  
`__DATA__`表示当前的日期  
`__TIME__`表示当前的时间  
`__STDC__`在正常的操作中，此宏为1，表示编译器符合ISO C标准  
`__STDC_VERSION__`表示ISO C的版本  
`__STDC_HOSTED__`如果值为1的话，表示目标环境有完成的标准C库  
`__cplusplus`如果该宏被定义了，表示是被C++编译器编译的  

*常见的预定义宏*  

该节中的宏是GNU C编译器的扩展实现。
```c
#include <stdio.h>
int main()
{
    printf("__COUNTER_%d\r\n", __COUNTER__);
    printf("__COUNTER_%d\r\n", __COUNTER__);    
    printf("__GNUC__:%d\r\n",__GNUC__);
    printf("__GNUC_MINOR__:%d\r\n",__GNUC_MINOR__);
    printf("__GNUC_PATCHLEVEL__:%d\r\n",__GNUC_PATCHLEVEL__);
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        printf("little endian\r\n");
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        printf("big endian\r\n");
    #elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
        printf('pdp endian\r\n')
    #endif
    #if __LP64__ == 1
        printf("64bit env\r\n");
    #else
        printf("other bit env\r\n");
    #endif
    return 0;
}
``` 
输出
>__COUNTER_0  
>__COUNTER_1  
>__GNUC__:7  
>__GNUC_MINOR__:3  
>__GNUC_PATCHLEVEL__:0  
>little endian  
>64bit env

`__COUNTER_`：是生成一个唯一的数字。
`__GNUC__`，`__GNUC_MINOR__`，`__GNUC_PATCHLEVEL__`确定了你的GCC版本号。例如我的环境就是7.3.0  
`__BYTE_ORDER__`表示当前环境的字节序  
`__LP64__`表示当前环境是不是64位，如果该值为1，则环境为64位环境  
 
*系统特定的预定义宏*  

系统特定的预定义宏，在不同的操作系统和CPU上面，呈现的结果可能会有所不同。例如我的环境是Linux X86_64平台。执行下面的代码  
```C
#include <stdio.h>
int main()
{
    printf("__unix_:%d\r\n", __unix__);
    printf("__x86_64__:%d\r\n", __x86_64__);
    return 0;
}
```
输出结果是：  
>__unix_:1
>__x86_64__:1

如果是其他操作系统的CPU平台的话，执行的结果会有所不同。  

*C++的命名操作符*
在第一节就说过C++ 中有`and,and_eq,bitand,bitor,compl,not,not_eq,or,or_eq,xor,xor_eq`这些命名不可以用作宏的名称。是因为在C++ 中系统将这些关键字预定义成了操作符。  
>命名操作符    符号  
and    &&  
and_eq    &=  
bitand    &  
bitor    |  
compl    ~  
not    !  
not_eq    !=  
or    ||  
or_eq    |=  
xor    ^  
xor_eq    ^=  

所以在C++ 中，你可以使用命名操作符来代替这些符号。例如：  	 
```c
#include <iostream>
using namespace std;
int main()
{
    int a = 10;
    int b = 20;
    int c = a bitor b; // a | b
    int d = a bitand b; //a & b
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;
    if ( true and (a > b))
        cout << "true" << endl;
    else
        cout << "false" << endl;
    return 0;
}
```
输出：
>c = 30  
d = 0  
false
------------------------------------------------

**(6)取消宏定义与重复宏定义**  

*取消宏定义*  
使用#undef可以将已经定义的宏取消掉
```c
#define BUFSIZE 1020
#undef BUFSIZE
```
如果在`#undef`之后再使用`BUFSIZE`就会报错，没有定义`BUFSIZE`  

*重复宏定义*  
如果两个宏定义之间，仅有空格和注释不同的话，两个宏定义还是同一个宏定义。  
例如：
```c
#define FOUR (2 + 2)
#define FOUR         (2    +    2)
#define FOUR (2 /* two */ + 2)
```
这三个宏定义实际上是相同的，不算是重复定义。  
而下面的宏定义则是不同的，编译器会给出宏重复定义的警告。也只有最后一个宏才会生效  
```c
#define FOUR (2 + 2)
#define FOUR ( 2+2 )
#define FOUR (2 * 2)
#define FOUR(score,and,seven,years,ago) (2 + 2)
```
-------------------------------------
**(7)几个常见的使用场景**  

*替代魔法数字*  
这个可能是在C语言中非常常见的一种用法了，就是使用宏来替代一个魔法的数字，增加代码可读性。
```c
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024
int main()
{
    char *buf = (char *)malloc(BUFSIZE);
    free(buf);
    return 0;
}
```

*`LOG`日志与`do{}while(0)`*  
 ```c
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024
#define LOG(str) \
do \
{\
    fprintf(stderr, "[%s:%d %s %s]:%s\r\n",  __FILE__, __LINE__, __DATE__, __TIME__, str); \
}while(0)
int main()
{
    char *buf = (char *)malloc(BUFSIZE);
    LOG("malloc for buf");
    free(buf);
    return 0;
}
 ```
输出内容：  
>[macro.c:12 Jan 13 2019 22:38:33]:malloc for buf

这里定义了`LOG`宏，可以打印日志，输出当前的代码文件和行数，以及时间和用户定义的内容。自行扩展可以增加更丰富的内容。  
这里使用了一个`do{} while(0)`来包含宏的内容。看似这个`do() while(0)`没有什么意义。但是这是一个编写宏内多行代码段的好习惯。  
使用`do{}while(0)`包含的话，可以作为一个独立的block，进行变量定义等一些复杂的操作  
该用法主要是防止在使用宏的过程中出现错误。  
例如
```c
#define foo() \
    fun1(); \
    fun2()
if (a > 10)
    foo()
```
在这种情况下，`if`后面没有跟大括号，我们`foo`宏里面定义的是两个语句，其中`fun2`是在`if`条件判断之外的。这样就不符合我们的预期了。  
如果使用大括号来避免上面的错误，还会出现下面的错误：  
```c
#include <stdio.h>
#include <stdlib.h>
#define add(x, y) {x += 1; y += 2;}
int main()
{
    int x = 10;
    int y = 20;
    if (x > y)
        add(x, y);
    else
        ;
    return 0;
}
 ```
这里在`add(x, y)`之后有个分号。会造成`else`匹配不到if编译错误。所以为了防止发生这些错误，可以使用`do{}while(0)`将函数体包含。

*`Linux`内核中`offsetof`*  
在Linux的内核代码中，大量的使用到了offsetof这个宏，该宏的作用就是计算出一个结构体中的变量的偏移值是多少。
```c
#include <stdio.h>
#include <stdlib.h>
#define offsetof(TYPE, MEMBER) ((int) &((TYPE *)0)->MEMBER)
typedef struct myStructTag
{
    int a;
    double b;
    float c;
    char szStr[20];
    long int l;
}myStruct;
int main()
{
    printf("%d\r\n", offsetof(myStruct, a));
    printf("%d\r\n", offsetof(myStruct, b));
    printf("%d\r\n", offsetof(myStruct, c));
    printf("%d\r\n", offsetof(myStruct, szStr));
    printf("%d\r\n", offsetof(myStruct, l));
}
```
输出结果：
>0  
8  
16  
20  
40

该宏的入参第一项`TYPE`为结构体的类型，第二项`MEMBER`为结构体中的变量名称。该宏将`0`强转为`TYPE *`类型的指针，然后获取该结构体指针指向具体成员的地址。因为结构体指针的地址为`0`，所以取地址得到的成员地址就是以`0`为基址的偏移值。  
有了该宏，我们就可以通过任意一个结构体成员的地址来得到结构体指针的地址了。

*`Linux`内核中`container_of`宏*  
该宏的作用就是通过结构体任意成员的地址来获取结构体指针。该宏需要借助上一节的`offsetof`。
下面是使用该宏的代码：
```c
#include <stdio.h>
#include <stdlib.h>
#define offsetof(TYPE, MEMBER) ((int) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ \
    const typeof(((type *)0)->member) * __mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); })
typedef struct myStructTag
{
    int a;
    double b;
    float c;
    char szStr[20];
    long int l;
}myStruct;
int main()
{
    myStruct *p = (myStruct *)malloc(sizeof(myStruct));
    printf("base ptr=%p\r\n", p);
    printf("base ptr by l=%p\r\n", container_of(&p->l, myStruct, l));
}
``` 
输出内容：
>base ptr=0x55cc10d66260  
base ptr by l=0x55cc10d66260  
 
可以看出，通过`container_of`算出来的基址和直接打印的`p`的地址是相同的。`Linux`内核中很多基础的抽象数据结构，例如双线链表等，都大量使用到了`container_of`这个宏。有了这个宏，我们就可以写出来数据无关的抽象数据结构，例如我们可以写一个没有数据域的双向链表。
```c
struct list_head {
    struct list_head *next, *prev;
};
```
实现的时候，我们只需要关系链表的操作即可，完全没有任何数据域的干扰。而在使用时，我们只需要把链表节点定义为具体数据结构中的一个节点即可。  
```c
struct person 
{ 
    int age; 
    char name[20];
    struct list_head list; 
};
```
插入和删除操作仅需要操作链表的节点，而通过`container_of`这个宏，我们完全可以通过链表的指针去获取到整个数据结构的首地址。这样就把数据结构抽象了，和具体的数据完全剥离。

*VPP中节点注册的例子*
>\VLIB_REGISTER_NODE宏的定义

首先看一段VPP中节点注册的宏的定义：
```c
#define VLIB_REGISTER_NODE(x,...)                                       \
    __VA_ARGS__ vlib_node_registration_t x;                             \
static void __vlib_add_node_registration_##x (void)                     \
    __attribute__((__constructor__)) ;                                  \
static void __vlib_add_node_registration_##x (void)                     \
{                                                                       \
    vlib_main_t * vm = vlib_get_main();                                 \
    x.next_registration = vm->node_main.node_registrations;             \
    vm->node_main.node_registrations = &x;                              \
}                                                                       \
__VA_ARGS__ vlib_node_registration_t x
```
在该代码段中，`VLIB_REGISTER_NODE`宏有一个参数`x`，和可变参数。
`__VA_ARGS__ vlib_node_registration_t x`声明了一个`vlib_node_registration_t`结构体变量 `x`，这里作用是仅声明。     
`static void __vlib_add_node_registration_##x (void) \ __attribute__((__constructor__))`这段代码是声明了一个函数，使用`##`连接符根据参数来生成函数名称。`__constructor__`是`GNU`编译器的一个扩展，把该函数作为构造函数，指明该函数会在模块初始化时调用。
接下来就是`__vlib_add_node_registration_##x `函数的定义了。具体的内容我们可以先无视掉。  
而最后一行，又定义了一遍`x`。这个需要结合宏调用的地方来看了。总之，这个宏声明了一个变量x，然后定义了一个
`VLIB_REGISTER_NODE`宏的使用
看完了宏的定义，我们看一下该宏是怎样调用的。
```c
VLIB_REGISTER_NODE (ip4_icmp_echo_request_node,static) = {
  .function = ip4_icmp_echo_request,
  .name = "ip4-icmp-echo-request",
  .vector_size = sizeof (u32),
  .format_trace = format_icmp_input_trace,
  .n_next_nodes = 1,
  .next_nodes = {
    [0] = "ip4-load-balance",
  },
};
```
首先宏的参数`x`传入了`ip4_icmp_echo_request_node`，在宏的扩展时，x都会被替换成传入的参数。  
而第二个参数是`static`，所以定义变量x时，都会`static`修饰。
最后在定义之后，有等号和大括号。这里是对宏的代码中最后一行`__VA_ARGS__ vlib_node_registration_t x`进行结构体赋值操作。这里就可以理解为什么`__VA_ARGS__ vlib_node_registration_t x`定义在宏里面进行了两次了。第一次是仅声明，后面定义的函数仅需要该值的地址去进行注册。而在宏的代码段的最后，是真正的结构体定义。
最后这段代码展开变成了下面的样子：
```c
static vlib_node_registration_t ip4_icmp_echo_request_node;                             
static void __vlib_add_node_registration_ip4_icmp_echo_request_node (void)                     
    __attribute__((__constructor__)) ;                                  
static void __vlib_add_node_registration_ip4_icmp_echo_request_node (void)                     
{                                                                       
    vlib_main_t * vm = vlib_get_main();                                 
    ip4_icmp_echo_request_node.next_registration = vm->node_main.node_registrations;             
    vm->node_main.node_registrations = &ip4_icmp_echo_request_node;                              
}                                                                       
static vlib_node_registration_t ip4_icmp_echo_request_node = {
  .function = ip4_icmp_echo_request,
  .name = "ip4-icmp-echo-request",

  .vector_size = sizeof (u32),
 
  .format_trace = format_icmp_input_trace,
 
  .n_next_nodes = 1,
  .next_nodes = {
    [0] = "ip4-load-balance",
  },
};
``` 

*VPP中错误码的定义*  
在实际C语言编程中，会有很多错误码的和对应的错误提示的定义。在VPP的代码中使用下面的方式来进行错误码和错误字符串的定义。  
```c
#include <stdio.h>
#define foreach_ethernet_arp_error                    \
  _ (replies_sent, "ARP replies sent")                    \
  _ (l2_type_not_ethernet, "L2 type not ethernet")            \
  _ (l3_type_not_ip4, "L3 type not IP4")                \
  _ (l3_src_address_not_local, "IP4 source address not local to subnet") \
  _ (l3_dst_address_not_local, "IP4 destination address not local to subnet") \
  _ (l3_src_address_is_local, "IP4 source address matches local interface") \
  _ (l3_src_address_learned, "ARP request IP4 source address learned")  \
  _ (replies_received, "ARP replies received")                \
  _ (opcode_not_request, "ARP opcode not request")                      \
  _ (proxy_arp_replies_sent, "Proxy ARP replies sent")            \
  _ (l2_address_mismatch, "ARP hw addr does not match L2 frame src addr") \
  _ (gratuitous_arp, "ARP probe or announcement dropped") \
  _ (interface_no_table, "Interface is not mapped to an IP table") \
  _ (interface_not_ip_enabled, "Interface is not IP enabled") \
static char *ethernet_arp_error_strings[] = {
#define _(sym,string) string,
  foreach_ethernet_arp_error
#undef _
};
typedef enum
{
#define _(sym,string) ETHERNET_ARP_ERROR_##sym,
  foreach_ethernet_arp_error
#undef _
    ETHERNET_ARP_N_ERROR,
} ethernet_arp_input_error_t;
 
int main()
{
    printf("%s\r\n", ethernet_arp_error_strings[ETHERNET_ARP_ERROR_interface_no_table]);
}
 ```

输出：
>Interface is not mapped to an IP table

`foreach_ethernet_arp_error`中定义了该模块所有错误类型和错误码的对应关系。 `ethernet_arp_error_strings`定义了错误字符串的集合。  
`ethernet_arp_input_error_t`定义了错误码的集合。  
我们可以通过错误码作为索引去`ethernet_arp_error_strings`中查找对应的错误字符串。  
这样我们就可以很方便的扩展和修改错误类型和错误码了。只需要修改`foreach_ethernet_arp_error`中的定义即可。  
这里错误码和错误字符串都是用过宏来自动生成的。  

----------------------------------------------------------------------------------------------------------------------------------------
# 一、用来把参数转换成字符串
例子一：  
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
例子二：
```c
#define SQUARE(x) printf("The square of "#x" is %d.\n", ((x)*(x)));
SQUARE(8)
```
输出的是：`The square of 8 is 64`  
# 二、运算符可以用于宏函数的替换部分。这个运算符把两个语言符号组合成单个语言符号，为宏扩展提供了一种连接实际变元的手段
例子一：  
>#define XNAME(n) x ## n

如果这样使用宏：`XNAME(8) `   
则会被展开成这样：`x8 `   
`##`就是个粘合剂，将前后两部分粘合起来，也就是有“字符化”的意思。但是“`##`”不能随意粘合任意字符，必须是合法的C语言标示符。  
在单一的宏定义中，最多可以出现一次“`#`”或“`##`”预处理操作符。如果没有指定与“`#`”或“`##`”预处理操作符相关的计算次序，则会产生问题。为避免该问题，在单一的宏定义中只能使用其中一种操作符(即，一份“`#`”或一个“`##`”，或都不用)。除非非常有必要，否则尽量不要使用“`#`”和“`##`”。  
# 三、`__VA_ARGS__` 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。
实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。  
 
# 四、`__VA_ARGS__` 宏前面加上##的作用在于，当可变参数的个数为0时，这里的##起到把前面多余的","去掉的作用,否则会编译出错
一般这个用在调试信息上多一点  
例如：  
```c
#define my_print1(...)  printf(__VA_ARGS__)   my_print1("i=%d,j=%d\n",i,j)  //正确打印
#define my_print2(fmt,...)  printf(fmt,__VA_ARGS__)  
my_print1("i=%d,j=%d\n",i,j) //正确打印
my_print1("iiiiiii\n")       //编译失败打印，因为扩展出来只有一个参数，至少要两个及以上参数
```
如果是`#define my_print2(fmt,...)  printf(fmt,##__VA_ARGS__)`  
那么
`my_print1`里面不管是几个参数都能正确打印
```c
#define MODULE_NAME   "MY_LIB"
#define error_print(fmt, ...)  printf("[ERROR]["MODULE_NAME"](%s|%d)" fmt, __func__, __LINE__, ##__VA_ARGS__)
```
