# 1. release会开启许多优化，debug模式会关闭这些优化，因此debug模式执行速度更快。
# 2. 编译器会为每个源文件生成一个.obj文件
# 3. 函数的声明需要在一个源文件里，但是编译器会全局搜索定义，在一个原文件中编译器默认声明的函数都能够找到。
# 4. 文件对于编译器来说没有意义，文件只是给编译器提供源码等。
# 5. 预处理的直接替换

```c++
/*
 *main.c
 */

#include <iostream>

int main(){
    std::out << "hhh" << std::endl;
#include "a.h"

/*
 *a.c
 */
}
```

# 6.
```c++
#if 1

#endif
```

# 7. 链接会找到每个符号与函数的位置，并将它们连接起来，编译会对，每个源文件进行编译。(此处的源文件及 翻译单元指的是C源程序并不是H头文件)**头文件不参与编译**
# 8. 在visual studio中ctrl + F7只编译不进行链接，build或者运行则会。
# 9. 错误以C开头，发生在编译阶段，编译错，以LNK开头，发生在链接阶段，链接错。

```c++
/*
 *main.c
 */

#include <iostream>

void Log(const char * message);

static int Mutiply(int a, int b){
    Log("Mutiply");
    return a * b;
}

int main(){
    //std::out << Mutiply(5, 8) << std::endl;
    std::cin.get();
}

/*
 *a.c
 */
#include <iostream>

void Logr(const char * message){
    std::out << messgae << endl;
}
```
在进行链接时，虽然mutiply函数从来没有被调用，因此Log以后也不会被调用，但是仍然会报出链接错误。
**原因：**虽然本文件内实际上并没有进行调用，但是链接器猜测会在其他文件中使用，因此链接器确实需要链接它。

**解决：**去掉这种链接的必要性，告诉链接器只会在本源文件内使用。在Multify定义前添加static关键字。Multify函数只被声明在这个翻译单元中。即main.c中，而在main.c中该函数并没有被调用，因此不会得到编译错误。

如果取消注释，之后build，肯定会得到链接错误。

除函数名称必须一致，返回类型与参数列表都必须对应。

# 10. 
```c++
/*
 *main.c
 */

#include <iostream>

void Log(const char * message);

static int Mutiply(int a, int b){
    Log("Mutiply");
    return a * b;
}

int main(){
    //std::out << Mutiply(5, 8) << std::endl;
    std::cin.get();
}

/*
 *a.c
 */
#include <iostream>

void Logr(const char * message){
    std::out << messgae << endl;
}

void Logr(const char * message){
    std::out << messgae << endl;
}
```
上述情况会发生在一个文件中，错误发生时，还没有开始链接，

```c++
/*
 *main.c
 */

#include <iostream>

void Log(const char * message);

static int Mutiply(int a, int b){
    Log("Mutiply");
    return a * b;
}

void Logr(const char * message){
    std::out << messgae << endl;
}

int main(){
    //std::out << Mutiply(5, 8) << std::endl;
    std::cin.get();
}

/*
 *a.c
 */
#include <iostream>

void Logr(const char * message){
    std::out << messgae << endl;
}
```

当如上述情况时，不会报出编译错误，会报出链接错误。

# 11. include文件在不同文件中被引用会在预处理时被多次替换，因此会造成重复定义
**解决方法**
1. #ifdef
2. 将重复定义的函数/常量声明为static 这样在不同文件中有对其他文件不可见的函数/常量。本obj对其他obj不可见
3. 使用inline，获取实际函数体，并将函数调用替换我函数体

```c
/*
**Log.h
*/
#pragma once

inline void Log(const char * message){
    std::cout << message << std::endl;
}

/*
**Log.cpp
*/
#include <iostream>
#include "Log.h"

void InitLog(){
    Log("Init");
}

/*
**Main.c
*/

#include <iostream>
#include "Log.h"

static int Multiply(int a, int b){
    Log("Multiply");
    return a * b;
}

int main(){
    std::cout << Multiply(5, 8) << std::endl;
    std:cin.get();
}

```

# 12. 原始数据类型之间唯一的区别是占用内存的大小 

# 13. int类型为32位，一位做符号位，范围在-2 ^ 31 - 2 ^ 31 之间，unsigned翻倍

# 14. char与int short 等的区别就在于对其处理上，如char 和short 都存储65 cout会将char类型输出为A，short会输出65

# 15. 原则：函数只能够被定义一次，声明可以多次。声明是告诉编译器函数在其他地方有定义，该函数确实存在。
如果没有头文件（即使已经有了头文件）在进行编译前，还是要把函数声明拷贝到到处都是。（头文件引用即将声明考的到处都是）

# 16. 所有以\#开头的命令都是预处理命令
```c
#pragma once
//本文件只会被包含一次，常用在头文件中
//只要针对常量变量被重复包含
#ifndef _DL
#define _DL

#endif
```

# 17. 尖括号只用于编译器包含路径，引号可以做一切，但通常只用在相对路径。