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

# 7. 链接会找到每个符号与函数的位置，并将它们连接起来，编译会对，每个源文件进行编译。(此处的源文件及 翻译单元指的是C源程序并不是H头文件)
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