# GCC ，gcc 和g++：

一直没搞清这几个东西的概念，搜了半天看到了一个不错的解释，所以大致记录一下，以免以后再忘记，链接。（原谅没找到原文出处）

GCC:GNU Compiler Collection(GUN 编译器集合)，它可以编译C、C++、JAV、Fortran、Pascal、Object-C等语言。

gcc是GCC中的GUN C Compiler（C 编译器）

g++是GCC中的GUN C++ Compiler（C++编译器）



由于编译器是可以更换的，所以gcc不仅仅可以编译C文件

所以，更准确的说法是：gcc调用了C compiler，而g++调用了C++ compiler

gcc和g++的主要区别

1. 对于 *.c和*.cpp文件，gcc分别当做c和cpp文件编译（c和cpp的语法强度是不一样的）

2. 对于 *.c和*.cpp文件，g++则统一当做cpp文件编译

3. 使用g++编译文件时，g++会自动链接标准库STL，而gcc不会自动链接STL

4. gcc在编译C文件时，可使用的预定义宏是比较少的

5. gcc在编译cpp文件时/g++在编译c文件和cpp文件时（这时候gcc和g++调用的都是cpp文件的编译器），会加入一些额外的宏。

6. 在用gcc编译c++文件时，为了能够使用STL，需要加参数 –lstdc++ ，但这并不代表 gcc –lstdc++ 和 g++等价，它们的区别不仅仅是这个。