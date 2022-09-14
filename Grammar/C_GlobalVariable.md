# GlobalVariable
## 全局变量
在所有函数外部定义的变量称为全局变量（Global Variable），它的作用域默认是整个程序，也就是所有的源文件，包括 .c 和 .h 文件。  
## 缺点
1. 全局变量存在周周期长，将会占用更多的内存。  
2. 全局变量多处被调用，使全局变量的值变得不确定。  
3. 用全局变量会增加程序的耦合性，会有一定移植麻烦，代码重用率低。  
4. 全局变量，使用和更改他的地方多了以后，各个地方逻辑关系难以确定。  
5. 过多的全局变量，大大降低程序的可读性，可维护性。  
6. 容易造成命名冲突  
## 合理使用全局变量
1. 如果只有某.c文件用，就static到该文件中；如果只有一个函数用，那就static到函数里面去；  
2. 尽量把变量的作用范围控制在使用它的模块里面，如果其他模块要访问，就开个读或写函数接口出来，严格控制访问范围。（就是利用.h文件#include）  
3. 当一个模块里面的全局变量超过3个(含)时，就用结构体包起来吧。要归0便一起归0，省得丢三落四的。  
4. 小程序结构简单，即使使用全局变量也方便管理。  
## 代码案例
```c
typedef struct rz_device_t rz_device_t;
CLASS(rz_device_t)
{
	/*成员参数变量 */
    rz_device_param_t device_param;   
};

/* 定义一个全局设备对象 */
static rz_device_t *pmain_dev;

/* 设置主设备 */
void main_device_set(rz_device_t *pdev)
{
    pmain_dev = pdev;
}

/* 获取主设备 */
rz_device_t *main_device_get(void)
{
    return pmain_dev;
}

```

## 两种错误例程
**1.unresolved external symbol**

例子包含两个C文件(test.c)和（first.c）和一个头文件（test.h）。下边具体展示下它们的代码。  
test.h内容
```c
#ifndef _TEST_H
#define _TEST_H
 
extern int count;   
 
#endif
```
test.c内容
```c
#include <stdio.h>
#include "test.h"
extern void Fis_Cal(void);
void main(void)
{
 Fis_Cal();
  printf("the present value of count is %d\n",count);
}
```
first.c内容
```c
#include <stdio.h>
#include "test.h"
void Fis_Cal(void)
{
 printf("the last value of count is %d\n",count);
 count = 1;
}
```
错误分析：test.h头文件中声明了全局变量count,但是在两个C文件中都没有对count进行定义，所以才会出现unresolved external symbol。  
一种解决方法：随便在两个C文件中加入一句“int count;”就OK了。例如我们加到test.c中，代码如下。 
```c
#include <stdio.h>
#include "test.h"
extern void Fis_Cal(void);
int count;
void main(void)
{
 Fis_Cal();
 printf("the present value of count is %d\n",count);
}
```
说明：加入的“int count;”就是对count的定义，默认的将其初始化为0。

结论：这种错误原因是“只声明未定义”。

**2.multiply defined symbols found**

还是如此，三个文件。但是，两个C文件与例程一中的文件一样，改动的只是头文件。  
test.h内容
```c
#ifndef _TEST_H
#define _TEST_H
 
int count; 
 
#endif
```
可以看到，与例程一仅仅差了一个“`extern`”关键词。

错误分析：test.h头文件中定义了全局变量count,但是在两个C文件都通过“#include "test.h"”这句话对“int count;”进行了引用，所以造成了重复定义的错误。

一种解决方法：添加一个“first.h”的头文件，并且更改first.c的内容，具体更改如下。

first.h内容
```c
#ifndef _FIRST_H
#define _FIRST_H
 
extern int count;
 
#endif
```
first.c内容
```c
#include <stdio.h>
#include "first.h"
void Fis_Cal(void)
{
 printf("the last value of count is %d\n",count);
 count = 1;
}
```
说明：经过这样的修改，原来的test.c中就包含了count的定义，而first.c中就包含了对count的声明，重复定义错误就得到解决。

结论：这种错误原因是“多个C程序都包含了定义全局变量的头文件”。 

原理分析

    我认为“int count;”是对全局变量的定义，而“extern int count”是对全局变量的声明，目的是让其他文件也使用这个全局变量。下边我们来挖掘全局变量的定义与声明的内涵。

    全局变量要么初始化（非零），要么没有初始化（为零）。非零时存储在程序中的data段，零时存储在程序的bss段。这谈了程序（.bin或者.hex）的结构。我再讲一下程序的启动，程序在启动（boot）过程中，通常都会运行一个叫bootloader的引导程序，这个引导程序干了很多事情，其中有一最重要的任务就是把程序（test段和rodata段）拷贝到内存，还包括data段的拷贝和bss段初始化。我们着重讲一下data段的拷贝和bss段初始化。

    我们的编译器会为我们定义的全局变量分配内存（地址），而且给我们的全局变量赋初值（写内存或清零），以后我们的程序就会根据需要来读这个全局变量（地址）或者修改这个全局变量（写内存）。初值为零时就在bss段，这个段初始化代码会将这部分清零。初值非零时，初始化代码会将全局变量的初值拷贝到data段。

    那么，显然全局变量的初值只有一个。我们程序中的全局变量的定义就是对全局变量分配内存并赋初值。而全局变量的声明是为了跨文件使用全局变量的需要，通过"extern"关键词来将全局变量引出。 

顺便说一下C语言的存储类说明符，这能帮助我们加深理解。

C语言的存储类说明符

     Auto 只在块内变量声明中被允许, 表示变量具有本地生存期。

     Extern 出现在顶层或块的外部变量函数与变量声明中，表示声明的对象具有静态生存期, 连接程序知道其名字。

    Static 可以放在函数与变量声明中,在函数定义时,只用于指定函数名,而不将函数导出到链接程序,在函数声明中,表示其后边会有定义声明的函数,存储类型static.在数据声明中,总是表示定义的声明不导出到连接程序关键字。

一种更好的声明与定义方式

test.h内容 
```c
#ifndef _TEST_H
#define _TEST_H
 
#ifdef GLOBALS
int count;
#else
extern int count;
#endif 

#endif
```
test.c内容
```c
#define GLOBALS
#include <stdio.h>
#include "test.h"
extern void Fis_Cal(void);
void main(void)
{
 Fis_Cal();
 printf("the present value of count is %d\n",count);
}
```
first.c内容
```c
#include <stdio.h>
#include "test.h"
void Fis_Cal(void)
{
 printf("the last value of count is %d\n",count);
 count = 1;
}
```
说明：这种方法可以只定义一个头文件实现在不同C文件中分别实现定义与声明。“`#define GLOBALS`”只在当前定义的test.c文件中有效，所以在test.c中`#include "test.h"`预处理后，加入的是`int count`，而first.c中加入的"`extern int count;`"。其实还有一种书写方法，也能实现这个效果。

test.h内容 
```c
#ifndef _TEST_H
#define _TEST_H
 
#ifdef GLOBALS
#define EXT
#else
#define EXT extern
#endif
 
EXT int count;
 
#endif
 ```

# 声明与定义
## 1.编译单元(模块)
在`VC`或`VS`上编写完代码，点击编译按钮准备生成`exe`文件时，编译器做了两步工作：  
- 将每个`.cpp(.c)`和相应的`.h`文件编译成`obj`文件；  
- 将工程中所有的`obj`文件进行`LINK`，生成最终`.exe`文件。  

那么，错误可能在两个地方产生：  
- 编译时的错误，这个主要是语法错误；  
- 链接时的错误，主要是重复定义变量等。  

编译单元指在编译阶段生成的每个`obj`文件。  
>一个`obj`文件就是一个编译单元。  
一个`.cpp(.c)`和它相应的`.h`文件共同组成了一个编译单元。  
一个工程由很多编译单元组成，每个`obj`文件里包含了变量存储的相对地址等。

## 2.声明和定义
函数或变量在声明时，并没有给它实际的物理内存空间，它有时候可保证你的程序编译通过；函数或变量在定义时，它就在内存中有了实际的物理空间。  
如果你在编译单元中引用的外部变量没有在整个工程中任何一个地方定义的话，那么即使它在编译时可以通过，在链接时也会报错，因为程序在内存中找不到这个变量。  
函数或变量可以***声明多次***(如果是变量多次声明需要加`extern`关键字)，但定义只能有一次。  
>对于变量而言，在同一个文件里面，很少使用声明这个说法，一般没有人去说我要声明一个变量，然后定义这个变量。或者说，***同一文件下声明与定义没有明显的区别***。就好比`int a`；我们可以说这是一个声明，也可以说这是一个定义，因为当程序执行到这句话的时候就完成了内存分配。数据类型，变量名，对应的内存单元就已经明确。  
而`int a = 1`; 就完成了初始化，因为它明确了内存单元里到底存放什么样的数据。

>在同一个工程，在多个文件中变量的声明和定义才有区别。比如说在`first.c`文件中先定义了一个全局变量`int a;`在`second.c`中要访问这个`a`,这时我们需要在`second.c`这个文件中用`extern`声明一下。这是常规写法，我们有更好的写法，在下面介绍。

## 3.extern 作用
- 当它与 `“C”` 一起连用时，如`extern “C” void fun(int a, int b);`，则编译器在编译`fun`这个函数名时按`C`的规则去翻译相应的函数名而不是`C++`的。  
- 当它不与`“C”`在一起修饰变量或函数时，如在头文件中，`extern int g_nNum;`，它的作用就是声明函数或变量的作用范围的关键字，其声明的函数和变量可以在本编译单元或其他编译单元中使用。  
>即`B`编译单元要引用`A`编译单元中定义的全局变量或函数时,`B`编译单元只要包含`A`编译单元的头文件即可，在编译阶段，`B`编译单元虽然找不到该函数或变量，但它不会报错，它会在链接时从`A`编译单元生成的目标代码中找到此函数。  
## 4.全局变量(extern)
有两个以上文件都需要使用共同的变量，我们将这些变量定义为全局变量。比如，`res.h`和`res.cpp`分别来***声明***和***定义***全局变量，其他文件需要使用全局变量时，只需要包含`res.h`即可，同时这也是 推荐的写法。  
下面是示例代码，只为演示全局变量的用法，不保证算法完备性：  
```C
/**********res.h声明全局变量************/
#pragma once
 
#include <QSemaphore>
 
const int g_nDataSize = 1000; // 生产者生产的总数据量
const int g_nBufferSize = 500; // 环形缓冲区的大小
 
extern char g_szBuffer[]; // 环形缓冲区
extern QSemaphore g_qsemFreeBytes; // 控制环形缓冲区的空闲区（指生产者还没填充数据的区域，或者消费者已经读取过的区域）
extern QSemaphore g_qsemUsedBytes; // 控制环形缓冲区中的使用区（指生产者已填充数据，但消费者没有读取的区域）
/**************************/
```
上述代码中`g_nDataSize`、`g_nBufferSize`为全局常量，其他为全局变量。
```C
/**********res.cpp定义全局变量************/
#pragma once
#include "res.h"
 
// 定义全局变量
char g_szBuffer[g_nBufferSize];
QSemaphore g_qsemFreeBytes(g_nBufferSize);
QSemaphore g_qsemUsedBytes;
/**************************/
```
在其他编译单元中使用全局变量时只要包含其所在头文件即可。
```C
/**********在consumerthread.cpp使用全局变量************/
#include "consumerthread.h"
#include "res.h"
#include <QDebug>

void ConsumerThread::run() {
     for (int i = 0; i < g_nDataSize; i++) {
          g_qsemUsedBytes.acquire();              
          qDebug()<<"Consumer "<<g_szBuffer[i % g_nBufferSize];
          g_szBuffer[i % g_nBufferSize] = ' ';
          g_qsemFreeBytes.release();
         
     }
     qDebug()<<"&&Consumer Over";
}
/**************************/
```
为了程序的简洁和清爽，我们通常将变量声明放在头文件中，需要使用的时候只需要包含头文件即可，如果不这样做，当涉及到的全局变量变多时，我们要做一大堆繁琐的声明工作。

### 4.1 如果直接将声明和定义都放在头文件中会如何？
效果看起来是这样：
```C
/**********res.h声明全局变量************/
#pragma once
 
#include <QSemaphore>
 
const int g_nDataSize = 1000; 
const int g_nBufferSize = 500; 
 
char g_szBuffer[]; 
QSemaphore g_qsemFreeBytes; 
QSemaphore g_qsemUsedBytes; 
/**************************/
```
这样做非常不妙，如果你的工程简单到只有一个头文件和一个源文件，那么没问题。但是当工程变得复杂时，很容易出现重定义错误。  
看到其他的说法是，即使在`res.h`中加`#pragma once`，或`#ifndef`也会出现重复定义，因为每个编译单元是单独的，都会对它各自进行定义），那么`res.h`声明的其他函数或变量，你也就无法使用了，除非也都`extern`修饰，这样太麻烦，所以还是推荐使用`.h`中声明，`.cpp`中定义的做法。  
但是我个人的验证是`#pragma once`，或`#ifndef`可以有效防止重定义的问题，遇到使用`#ifndef`也会提示重定义的情况是`#ifndef`仅对代码片段有效，在`#ifndef`范围外的代码自然会提示重定义。  
这里先保留意见，总之，`.h`中声明，`.cpp`中定义的做法是最稳妥的。

## 5. 静态全局变量(static)
注意使用`static`修饰变量，就不能使用`extern`来修饰，即`static`和`extern`不可同时出现。  
`static`修饰的全局变量的声明与定义同时进行，即当你在使用`static`声明了全局变量，同时它也被定义了。  
`static`修饰的全局变量的作用域只能是本身的编译单元。在其他编译单元使用它时，只是简单的把其值复制给了其他编译单元，其他编译单元会另外开个内存保存它，在其他编译单元对它的修改并不影响本身在定义时的值。即在其他编译单元`A`使用它时，它所在的物理地址，和其他编译单元`B`使用它时，它所在的物理地址不一样，`A`和`B`对它所做的修改都不能传递给对方。  
多个地方引用静态全局变量所在的头文件，不会出现重定义错误，因为在每个编译单元都对它开辟了额外的空间进行存储。  
下面是示例程序，同样只做演示，不保证算法完备性：
```C
/***********res.h**********/
static char g_szBuffer[6] = "12345";
/************************/
```
```C
/***********test1.cpp**********/
#include "res.h"

//fun1对g_szBuffer中的内容进行修改
void fun1() { 
     for (int i = 0; i < 6; i++) {
          g_szBuffer[i] = 'a' + i;
     }
     cout<<g_szBuffer<<endl;
}
/************************/
```
```C
/***********test2.cpp**********/
#include "res.h"

//fun2对g_szBuffer原样打印 
void fun2() {
     cout<<g_szBuffer<<endl;
}
/************************/
```
```C
/***********main.cpp**********/
#include "test1.h"
#include "test2.h"
 
int main() {
     fun1();
     fun2();
 
     system("PAUSE");
     return 0;
}
/************************/
```
按我们的直观印象，认为`fun1()`和`fun2()`输出的结果都为相同，可实际上`fun2()`输出的确是初始值。然后我们再跟踪调试，发现`test1`、`test2`中`g_szBuffer`的地址都不一样，这就解释了为什么不一样。  
注：一般定义`static`全局变量时，都把它放在`.cpp`文件中而不是`.h`文件中，这样就不会给其他编译单元造成不必要的信息污染。

## 6.全局常量(const)
`const`单独使用时，其特性与`static`一样（每个编译单元中地址都不一样，不过因为是常量，也不能修改，所以就没有多大关系）。  
`const`与`extern`一起使用时，其特性与`extern`一样。
```C
extern const char g_szBuffer[];      //写入 .h中
const char g_szBuffer[] = "123456"; // 写入.cpp中
```