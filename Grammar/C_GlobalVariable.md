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