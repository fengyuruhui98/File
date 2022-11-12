# c函数中返回字符串数组、char[]和char * 的区别与转换
>目录
* [一、原因](#一字符指针可以返回而作为局部变量的字符数组不能直接返回的原因)
* [二、字符数组与字符指针](#二字符数组与字符指针的区别)
* [一切皆有可能](#一切皆有可能)
* [小结](#小结)

## 一、字符指针可以返回，而作为局部变量的字符数组不能直接返回的原因
1、为什么作为局部变量的字符数组不能直接返回，而字符指针却可以?  
2、当字符数组是局部变量的时候，函数如何返回它？  
3、字符数组(char [])和字符指针(char *)如何互转？

## 局部变量的字符数组  
在C中如果我们直接返回字符数组，编译会直接报警告。如下示例：
```c
char * fork_user_name()
{
    char name[] = "veryitman";
    return name;
}
```
在Xcode中编译警告信息是这样的：
> Address of stack memory associated with local variable 'name' returned

在Linux上面GCC编译显示警告是这样的：
> warning: function returns address of local variable [-Wreturn-local-addr]

无论哪种警告信息，基本意思都是告诉我们不应该返回一个局部变量 name 的地址（函数内部的变量在栈内存上）。

如果我们修改一下代码，将 char 改为指针变量 char * ，示例如下：
```c
char * fork_user_name2()
{
    char *name = "veryitman";
    return name;
}
```

无论是Linux的GCC还是Xcode的Clang编译器都不会报出警告。

首先我们要知道，常量是放在数据段里面的。

这里比较特殊，局部变量 name 保存在栈中，但是字符串 veryitman 的值是一个常量，保存在常量区。即便函数返回了，数据段里面的常量数据也还不会消亡，它会直到程序结束才会消失，其内存空间直到程序运行结束才会被释放。 所以，返回的地址是一个实际存在的有效地址。
```c
char * fork_user_name()
{
    char name[] = "veryitman";
    return name;
}

char * fork_user_name2()
{
    char *name = "veryitman";
    return name;
}

int main()
{
    printf("fork_user_name: %s\n", fork_user_name());
    printf("fork_user_name2: %s\n", fork_user_name2());
    
    return 0;
}
```

用GCC编译、运行后的打印结果，如下：
>fork_user_name: (null)  
fork_user_name2: veryitman

总之，在函数中的局部变量只要是返回类似 int[]、char[]、long[] 地址的，都是不正确的做法。

## 一切皆有可能
下面例子是不正确的，如下：
```c
char * v_string()
{
    char rest[10] = {'\0'};
    return rest;
}
```

### 1、使用 static

在C语言中，用 static 限定外部变量与函数，该外部变量或者函数除了对该所在的文件可见外，其他文件都无法访问。 而用 static 声明内部变量，则该变量是某个特定函数的局部变量，只能在该函数中使用。但它与自动变量不同的是，不管其所在函数是否被调用，它一直存在，而不像自动变量那样，随着所在函数的被调用和退出而存在和消失。换句话说，static 类型的内部变量是一种只能在某个特定函数中使用但一直占据存储空间的变量。

所以使用static修饰一下，就没有问题了。示例如下：
```c
char * v_string()
{
    static char rest[10] = {'\0'};
    return rest;
}
```

### 2、使用 malloc

这种方式可以解决这个问题，是因为使用 malloc 分配的内存是在堆上而不是在栈内存上面。但是要记得将其在调用方使用 free 释放申请的内存空间，否则容易造成内存泄漏问题。

```c
char * v_string()
{
    char *p = (char *)malloc(10 * sizeof(char));
    p = "\0";
    return p;
}
```

### 3、全局变量

这个很好理解。全局变量在程序真个生命周期中都是有效的，所以使用全局变量也可以解决类似问题。

但是这种方案就会让这个封装的方法不够内聚，因为它依赖了全局变量。
```c
char g_rest[100];
char * v_string()
{
    strcpy(g_rest, "verytiamn");
    return g_rest;
}
```

### 4、返回形参指针变量

在Linux Kernel(内核源码版本5.0.7)中，函数 strcpy 的实现如下：
```c
#ifndef __HAVE_ARCH_STRCPY
/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
#undef strcpy
char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
EXPORT_SYMBOL(strcpy);
#endif
```

参考内核实现，我们可以修改一下自己的代码，示例如下：
```c
char * v_string(char *s1, char *s2)
{
    char *tmp = s1;
    // 省略...
    return tmp;
}
```
这里补充另外一个知识点，函数 strcpy 在glibc和Linux Kernel中实现不一样。
```c
在glibc的新版中(2.29版本)，本质是调用了函数 memcpy， 实现如下：

#include <stddef.h>
#include <string.h>

#undef strcpy

#ifndef STRCPY
# define STRCPY strcpy
#endif

/* Copy SRC to DEST.  */
char * STRCPY (char *dest, const char *src)
{
  return memcpy (dest, src, strlen (src) + 1);
}
libc_hidden_builtin_def (strcpy)
```

包括 strncpy 在glibc和Linux Kernel中实现也不一样，有兴趣的可以去看看源码。

字符数组和字符指针的互转
char [] 转 char *
这种情况下，可以直接进行赋值，示例如下：
```c
int main()
{
	char c_str_array[] = "veryitman.com";
    char *p_str;
    p_str = c_str_array;
    printf("p_str: %s\n", p_str);
	return 0;
}
```
char * 转 char []
是不是也可以直接进行赋值呢？撸段代码看看，如下：
```c
int main()
{
	char c_str_array[] = "veryitman.com";
    char *p_str = "veryitman.com";
    c_str_array = p_str;
    printf("c_str_array: %s\n", c_str_array);
	return 0;
}
```
很遗憾，编译报错，GCC编译错误截图如下：
![image1][image1]

Clang编译错误如下：

>Array type 'char [14]' is not assignable

可以考虑使用 strncpy 来实现，示例代码如下：
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char c_str_array[] = "veryitman.com";
    char *p_str = "veryitman.com";
    strncpy(c_str_array, p_str, strlen(p_str));
    printf("c_str_array: %s\n", c_str_array);
    return 0
}
```
## 二、字符数组与字符指针的区别
此段转自：https://blog.csdn.net/u012611878/article/details/78291036

***相同点*** 

首先 这两种类型都可以对应一个字符串，比如：
```c
char * a=”string1”;
char b[]=”string2”;
printf(“a=%s, b=%s”, a, b);
```
其中a是一个***指向char变量的指针***，b则是一个***char数组***（字符数组），

其次 ，很多时候***二者可以混用***，像函数传参数的时候，实参可以是char*,形参可以是 char[]，比如：
```c
void fun1(char b[]){
printf("%s",b);
}
int main(){
char *a="HellowWorld";
fun1(a);
}
```
反过来，实参可以是char[],形参可以是 char *也是可以的。
存在即合理，char *和char[]肯定是有本质的不同。

***不同点***  

1. ***char*是变量，值可以改变， char[]是常量，值不能改变。***
比如：
```c
char * a="string1";
char b[]="string2";
a=b； //OK
a="string3"; //OK
b=a; //报错！左边操作数只读
b="string3"; //报错！左边操作数只读
```
>解释： a是一个char型指针变量，其值（指向）可以改变； b是一个char型数组的名字，也是该数组首元素的地址，是常量，其值不可以改变 。

2. char[]对应的内存区域总是可写，char*指向的区域有时可写，有时只读
比如：
```c
char * a="string1";
char b[]="string2";
gets(a); //试图将读入的字符串保存到a指向的区域，运行崩溃！
gets(b); //OK
```
解释： a指向的是一个字符串常量，即指向的内存区域只读； b始终指向他所代表的数组在内存中的位置，始终可写！

注意，若改成这样gets(a)就合法了：
```c
char * a="string1";
char b[]="string2";
a=b; //a,b指向同一个区域，注意这里改变了a的指向
gets(a) //OK
printf("%s",b) //会出现gets(a)时输入的结果
```
解释： a的值变成了是字符数组首地址，即&b[0]，该地址指向的区域是char *或者说
char[8]，习惯上称该类型为字符数组，其实也可以称之为“字符串变量”，区域可读可写。

注意：char *本身是一个字符指针变量，但是它既可以指向字符串常量，又可以指向字符串变量，指向的类型决定了对应的字符串能不能改变。

3. char * 和char[]的初始化操作有着根本区别：
```c
char *a="Hello World";
char b[]="Hello World";
printf("%s, %d\n","Hello World", "Hello World");
printf("%s, %d %d\n", a, a, &a);
printf("%s, %d %d\n", b, b, &b);
```
![image2][image2]

结果可见：尽管都对应了相同的字符串，但”Hellow World”的地址 和 a对应的地址相同，与b指向的地址有较大差异；&a 、&b都是在同一内存区域，且&b{b
根据c内存区域划分知识，我们知道，局部变量都创建在栈区，而常量都创建在文字常量区，显然，a、b都是栈区的变量，但是a指向了常量（字符串常量），b则指向了变量（字符数组），指向了自己(&b}b==&b[0])。

说明以下问题：

- char * a = "string1"; 是实现了3个操作：
>1. 声明一个char*变量(也就是声明了一个指向char的指针变量)。  
>2. 在内存中的文字常量区中开辟了一个空间存储字符串常量"string1"。 
>3. 返回这个区域的地址，作为值，赋给这个字符指针变量a  

最终的结果：指针变量a指向了这一个字符串常量“string1” （注意，如果这时候我们再执行：char * c = ”string1”；则，c == a，实际上，只会执行上述步骤的1和3，因为这个常量已经在内存中创建）

- char b[] = "string2";则是实现了2个操作：  
>1. 声明一个char 的数组
>2. 为该数组“赋值”，即将”string2”的每一个字符分别赋值给数组的每一个元素，存储在栈上。  

最终的结果：“数组的值”（注意不是b的值）等于”string2”，而不是b指向一个字符串常量

PS：
实际上， char * a = "string1"; 的写法是不规范的！  
因为a指向了即字符常量，一旦strcpy(a,”string2”)就糟糕了，试图向只读的内存区域写入，程序会崩溃的！尽管VS下的编译器不会警告，但如果你使用了语法严谨的Linux下的C编译器GCC，或者在windows下使用MinGW编译器就会得到警告。

所以，我们还是应当按照”类型相同赋值”的原则来写代码：

>const char * a = "string1";

保证意外赋值语句不会通过编译。

## 小结
对于
```c
const char * a="string1"
char b[]="string2";
```
>1. a是const char 类型， b是char const类型 （ 或者理解为 (const char)xx 和 char (const xx) ）
>2. a是一个指针变量，a的值（指向）是可以改变的，但a只能指向（字符串）常量，指向的区域的内容不可改变；
>3. b是一个指针常量，b的值（指向）不能变；但b指向的目标（数组b在内存中的区域）的内容是可变的
>4. 作为函数的声明的参数的时候，char []是被当做char *来处理的！两种形参声明写法完全等效！












[image2]:data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEAYABgAAD/2wBDAAMCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMUFRUVDA8XGBYUGBIUFRT/2wBDAQMEBAUEBQkFBQkUDQsNFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBT/wAARCAA2AQ4DASIAAhEBAxEB/8QAHQAAAgMBAQEBAQAAAAAAAAAAAAcFBggECQMCAf/EADAQAAEEAQQBBAICAgIBBQAAAAIBAwQFBgAHERITCBQhIhUxFiMyQUJRYSUzOHXE/8QAFgEBAQEAAAAAAAAAAAAAAAAAAAUB/8QAGxEBAAIDAQEAAAAAAAAAAAAAAAERBTNxAyH/2gAMAwEAAhEDEQA/AGfvPisvNtls2xyA4y1Nt4bFew5IJUaFx2bHbFTVEVUFFJOVRFXjn4XXlTmWw+QYPiF7kk+bWuwafK38OkNx3XFdKY02ThOAitoitKgrwqqhc8cin7163ZtfQMVwW7urV/2tbWlBmS3+hH42W7GKZl1FFVeBFV4RFVf9JzrBOcepDBcmx95ixch3DEfd9ci/Fw6QGPfUYgSeRxPE2DzhoqivmXylyndeE5SJiNM9Q8TpnrHOmLuNs1L27xLEMn/P0+Q0mUe8/HyqlZKc+2cFt3uD7DRD9i4T4XnhV/XHLh9WW+FJudjzkGvyKnyLnJpdlAWJX2hSYcIg6iLkqc7/AFeTkOYsdrworSEhDwgrAbvWuH2vp32txioz2mubzD/yvvIsaJYt+593MBxvwE7FAV6iiqXdQ/XA9l4Rba4Xu42zUvbvEsQyf8/T5DSZR7z8fKqVkpz7ZwW3e4PsNEP2LhPheeFX9ccrrWp7zLtuoO2+w9Y/mFPlf8Bs5cm7qI1fOX8jHkWLLytsJIittn/UJ9hdIEX9J251+fU1vLi+dYBZVUfK0zi7k5zPvKuajEpfxVO411CH3lNtk3yXRfE0hN/18qvKDyGd8LxeXnGYUeOQHGGZ9xPYro7kklFoXHXBbFTVEVUFFJOeEVeOfhdXKXsLfwavdOcU2sJjbuczX2wi452fcclHGFY/9f2Hu2qr36L1VPhV+NU3C8ol4Rl9FkcBtl2dTzmLCO3JFVaJxpwXBQ0RUVRVRTnhUXj/AGn71sHOPUFtFSFlhUDrWRwcgzSrkWsJyqcQ7CoakHZvvKjgNgbqypD0ZEdX5YbaHr8K6oYh0a1rvxvTj+Y4/wCwr9wEvMic3ClX9NkKtz0/j1SYoLbPldZF1rqfRzxxxMU8XKfZARfh6uPU3Bzm6zLH8SeoMgwnInYFmEwq+UEyJNZaBo3RJ9A8bpAyLS9BUPCgInBk8qgltxtmpe3eJYhk/wCfp8hpMo95+PlVKyU59s4LbvcH2GiH7FwnwvPCr+uOfhI2lsGNmYm5bVnWSaZ+4WiciNE8kuPK8Ru8GJNoCirYoXIGX+Yp++yI6bzLtuoO2+w9Y/mFPlf8Bs5cm7qI1fOX8jHkWLLytsJIittn/UJ9hdIEX9J2513+ofevHdw9qcwx4Nwf5PPXOTyeoR5ueoJWug8yEJpXmR8Zs9kMm16tIJ/1mRdh0GSddtdXSriwjQYEV2ZOlOiwxGjtq4664SoIgApypEqqiIifKqurxutt3jO38XHmanOoeW3cqKjtxBr46KxVPK00aNDKBw2pPyZipNrwitrzwq8JR4EVqdYRo70xiAy66DZy5CGrTAqqIpmgCRqIp8r1Ei4T4RV+NBcc42Zyjba4xinySIxVW1/AZsGIcx8Y5Rm3XnGm0kE4ogyXLSkvZeAEk7qKoQj355sZYYTh/wDKY2QUOV4+Fw7QvWNA+8bTM0G0c8fLrTfkEg5IXGu4Kgr9vkebz6jbzB8zTa52rzeFaMUWM1eM2jVdXzfcgTPlV6Q0L7LLbjaISdUVwCJVRFRE5JJDNdzcRxfZ/F8Wqrunz60o8wW3ijFpnoNa/XNR22wCwjE2yj77pAnJKjpoHkFXvt9gp9d6Vs5l4RhOUymYdRAy+9i0NS1YOOA+4T6ErUkhQF6sF1XgueyoiEIKJCS8FvsQVbuBAwmNm2NW2TP3oY85BhjYD7aQTqsqThuxABQE0QVUFJflFQSTldPC99TdJmOyLU6wt6ak3FcziNlJs10SzlvxUbQGkkNJJcJg3xEE6sE6kfwogogOIiIvd+MywrI8BRpL6nzbcFzJpU3+TUuPrU+SsJpPrLDwMob7j39n6d6/dPL8/YIm19LVtAqs1lQ8xxW2dw+dHrbmKw9LjlGfelLGAVckxmmuqOIXY/J1ERVVXjjmNuvTdkESvrJ+P3NDmkGwyL+KNyqGW54hs1QSBns+20hCaFyLodm+EXk05TmSzfO8VqsCw3AsLtno+N2UCFY5rOiwzKdKs1MlcZPyK0jzUdEFWmhUW1IlJSI/ul+c3dwfb/ENu6VrJWcxexrPmL4JGOVDlc0VUw2yKe4YMGBdmGrfKGXkJE7or/BfYKPZ+kPMAg3T9HY0+VHS3jOOWDNYspnwTnXkYFtHJTDLbnDpNgXjMuvkAlRAXtqA3M9PV3trQSrt24pr2sh3sjGp0ipN9PZ2LQdyYMX2WlLkUJUNtCD6rySfHOoc19Ve37cTMo7d8lljtleU8iqpsZOzCSwy1bPWNhLdckgwLT7xOLx4D5FfCCL1b8ml76sN+sX3P2/cq4d9DyG6PMJlrGlU0SVDa/GK0rcdJwOttC7KEfE0JoB9W2eO/JF3DJWjRo0Bo0aNAaNGjQGjRo0Bo0aNAaNGjQe0+9OLys22VzbHIDjLU23hsV7DkglFoXHZsdsVNURVQUUk54RV45+F15TZlsPkGD4he5JPm1rsGnyt/DpDcd1xXSmNNk4TgIraIrSoK8KqoXPHIp+9eumaX9ZiWEXN1dPHGqa9YcuTIACPxNtz4xqSiKKqoiDzwiKv/Sc6wdkm/wDhWewErZbsG2bDeFch/HR6UI4S6IQLs89/U2DpEKkhK8vlVP8ANeE5SJidE9RMTonrF2mLuNs1L27xLEMn/P0+Q0mUe8/HyqlZKc+2cFt3uD7DRD9i4T4XnhV/XHOiPUpurQb4YBkTNfc1mT2EO/OxqPxkCyWREhuE2yIOPTC5FXUIeY8dtGkJpC554RaLvUWMt7CbV4i3mFfOv8PSzOdXBBsopyglzAcb9uT8QB+ARVJT6/rge68IttbLHcbZqXt3iWIZP+fp8hpMo95+PlVKyU59s4LbvcH2GiH7FwnwvPCr+uOV1rX24d5tvQ4L6f6/+R1t7FwuS5Y2lGMSY8VlFkzWHTFhXozbZoog8qi8rfxwnC864/U1uTUZ1tjZMR7uTnE6TnE++q7VIUxW6qmcb6BE8spptW+T6f1Ndm+W/leUHkM1YXi8vOMwo8cgOMMz7iexXR3JJKLQuOuC2KmqIqoKKSc8Iq8c/C6uUvYW/g1e6c4ptYTG3c5mvthFxzs+45KOMKx/6/sPdtVXv0XqqfCr8arW3k68x7LqXJaKsOxm0s+PPZAmDdaV1txHAE0HheqqHzwqLwi/KfvWvdyN8NrsPlZY1WtLbsZBmVVKta2ZTOtu2FO2+do8+oug2JuFJkOxkR1flhtoevwrqhhjRrWu/G9GP5lj6V9duAl7kTu4cq+psgVuen8eqTFBbZ8rrIutdT6OeOOJini5T7ICL9vWN6gTyW3y+ix+NVWuDZI9BsWp0mpmMzIs5hoGnHAKQIeN0gaBpegqHh6InBk8qgidxtmpe3eJYhk/5+nyGkyj3n4+VUrJTn2zgtu9wfYaIfsXCfC88Kv645+EjaWwY2ZibltWdZJpn7haJyI0TyS48rxG7wYk2gKKtihcgZf5in77Ij8sXsSXazYuOFzFzSNt7Om2F9XQKaxcCfDesGnjRlX4jbRCjQOd0dIB+FROeddu+G6NXuxtXmVHDy+Zm8hrOlyeuRqPYGIVjoPNBBaN2Pwy40i91AkRpBL+sjLsOgxxrtrq6VcWEaDAiuzJ0p0WGI0dtXHXXCVBEAFOVIlVURET5VV03N9dkqHAbhmHh2SP5lJFetrCh15kFO4QtK00UoFJt8iUyFVDjggVFRFXhFYtE/HuI0C0X8Irptib1gy6AMASonkMREjUET5XqJFwnwir8aC0ZxszlG21xjFPkkRiqtr+AzYMQ5j4xyjNuvONNpIJxRBkuWlJey8AJJ3UVQhHvzzYywwnD/5TGyChyvHwuHaF6xoH3jaZmg2jnj5dab8gkHJC413BUFft8jzevUXdYTmy7WnU5tDtmaPGarGbNmtrpqyhNny+Z9oX2WW3G0Qk6orgESqiKiJySWXPsgrcL2bx/GoRR83sMazBbgPDjcmvrCrm4zQAFlHNpkXX3VQFUiR00BTFXvv9gXNd6Vs5l4RhOUymYdRAy+9i0NS1YOOA+4T6ErUkhQF6sF1XgueyoiEIKJCS8FvsQVbuBAwmNm2NW2TP3oY85BhjYD7aQTqsqThuxABQE0QVUFJflFQSTldP2X6gIu5WySTfPAhbjfziLlLzNLXW09+GgILQyG0fM2CfEW/qwTqR/CiCiA4iIlK3OcxzdPHI9PX29VnO4z+VSZZ5TTY8tSKVZNJyk0PAyhvm8iuccOqnBojnz9gp9r6WraBVZrKh5jits7h86PW3MVh6XHKM+9KWMAq5JjNNdUcQux+TqIiqqvHHMbdem7IIlfWT8fuaHNINhkX8UblUMtzxDZqgkDPZ9tpCE0LkXQ7N8IvJpynMzuHluPY1hGG7e4rOmVtBZ18KzzSW3AcSfKslMlcZNHVaR5qOiIrTQqLakSkpEf3Rjjurh+2eB7d1IXkTNTqs6ZuY0nH6c68Sqo/t07OxzBhHJhkyqI4XkLhXR8/CryC3s/SHmAQbp+jsafKjpbxnHLBmsWUz4JzryMC2jkphltzh0mwLxmXXyASogL21Abmenq721oJV27cU17WQ72RjU6RUm+ns7FoO5MGL7LSlyKEqG2hB9V5JPjnWWbeobFmnc6qW5U2yq5VxUzYGO4y1ahOiRmbR2zsZr5SW2EYdd8iqisn9f6RRerfk1SPVLvBiuebbwQsLIMhO0y6fa107HG3oKFWCPhaKa280AvSQHxsiXReAZ4Rz5PuGLtGrbMwZun3GPF7Kc422koI4y4zCOdxNR6GgqQ/CiSF+/jnX4jYdGl7kP4378IUQJz0b3coxHqAEXyqqqDyqDx8qicqn61liJx3Hp+V31ZSVUf3VnZSWocRnyCHkecNAAexKgpySonKqiJz8rpo5Z6XMrxz2qV9hT5T5snPDTWpfeb8FsnHEY/ctM89uV4cHs39V5JPjmsY7iWPZNWlYPWErHIESzhRbCZKRJgtR3yJCeEGwEiJtAUlBOVJP0qcfOp8vlbfbanhjlLc3D+KYPkcad/HqCpkGrxNojhWM6S800DsoyAW1BCQWhdEW+EE+ywiMj9KeTUlFkltCu6G9iY5cN0Vw5FdkxW4confFwT0thhohFxQE1Ay6I4BFwC9tR116bsgiV9ZPx+5oc0g2GRfxRuVQy3PENmqCQM9n22kITQuRdDs3wi8mnKc6ez7enAdx8L3ex2NlrR0FsjEmrrqGPPfeitNTpE+ZMdGYDLauvOvcq22aJx42+3AeRF/VbmYLi1DtpjDWVM5ZLotwI9zGlY5TOV7B1zQMACSmTFgXJRkHPkVHTFO6K8qF9tCyyP0p5NSUWSW0K7ob2Jjlw3RXDkV2TFbhyid8XBPS2GGiEXFATUDLojgEXAL21DZZ6f7XHaCLe1+R41lNI9eHjZ2lTONuMxPQENAN2S2yKgQqpI8Kk1wKqponHOp9yt28Rz6Hubt7HyWP7TIrKJGqamjanyHYqt2EqfPkvJLBltXnjNUQGjVOyst8qIeTSu3NtcDlbBRcPrsqG0aocsSSUyoqnIvWvJkY4k9HJqK2/MXopop+Q0QHEV/gtZYVWV+nq7oMei3dNcU2cVki8PG0kYwch/8A9RQEMWBFxlsne4ryBtIYFx/l8jzXty9t3drbZumsL2nsrtrkLCuqnnXjrXkEVVh5xW0aI0UlEkaNxBIDFVRU+dRs7nbS4hiuEVMO7nZazh+TNZGUyhhpXN+3YbYASkMlHYF59xwREefI5wp8yUQlRav6k8i2+yTDyvQCDktvc5hZTq65xyt/GeGrVPiNLMo7avP91EuTAy+CXycF91jJ+jUxl9COL5LYVYP+5CM51FxR6kqcIqIScrwSc8Kn+lRU1D60e1m9uLys12TzfG4DrLU22hsQGHXyUWhcdmx2xU1RFVB5JOeEVeP9Lry2yPZS/wBsdvcpySe/CNImTO4URMG4pK82DhvOt8gnLZeLohcovCmiimvV7O7+Bi2BXV3ayPa11aUCZLf6EfjZbsYpmXUUVV4EVXhEVf8ApNYIz31MYVlmOrGtZMa8iwt3Vv26qNSiykuhETXuSeJsHTPuSKjy+UlL7rx8pFxX3xnqLipvxnrN+AJLcxHOGIKksp+FHbFtr/3HEWQCkIp+1Xqhconzwhf651dd59qZ+CbY7XPPWEO8S1OzCG/XtSQUm23Wvorb7LTicGZ8Io/PZVT4X5u/qy3wpNzsecg1+RU+Rc5NLsoCxK+0KTDhEHURclTnf6vJyHMWO14UVpCQh4QVgN3rXD7X077W4xUZ7TXN5h/5X3kWNEsW/c+7mA434CdigK9RRVLuofrgey8ItmlpDeoDaCxwPGdvMhdsYFtUW9YsWNIgDIBe0dUQ1MHmWyBC8idUVOV6kvCIic8vIJvWl8raphvbuknqvt/x3i6+Hn9c+P8Ar6fvt9eOfjTDvMu26g7b7D1j+YU+V/wGzlybuojV85fyMeRYsvK2wkiK22f9Qn2F0gRf0nbnX59TW8uL51gFlVR8rTOLuTnM+8q5qMSl/FU7jXUIfeU22TfJdF8TSE3/AF8qvKDyoULa7BZO4NZhlXFlxoUwsgNqMjspuOriuLHDsvckIuFFE7NC4SJynVV4Rfxk+x2V2k7eC5ferGlwewELWOjrg+RXpDjYrF7Np2bTopJ36KoKPCKq8apuH7lWGIW+M2bMCBPl47LbmVxTRc4bIHkeQV6GPYO/JfPz8r8omtY7ieo/amG/lL9RKbyiLfZhUFaRHqpxDsKaO6Vg66qOA2Bu+4eciojq/LDTQ9fhXVDI+1MqHC3CpXpzjTLIur0dfVEbbdUCRoyVfhERzoqqvwnGvhYwMjpqWfDsmn4EJ6S2+43Mb6lIfFCEVBVTklRDNVVF44X5+VHnSG/G9OP5jj/sK/cBLzInNwpV/TZCrc9P49UmKC2z5XWRda6n0c8ccTFPFyn2QEX4erj1Nwc5usyx/EnqDIMJyJ2BZhMKvlBMiTWWgaN0SfQPG6QMi0vQVDwoCJwZPKoQ2e7LysOxrEb6zsIJOXdXICvyHHHpBRyYjMA3xLZkMNqCG2QghCvz8qorz81WZtla3fp0xPLaJ+NMgRriTVzocbypJZmmiuI4aKCB08LbXJoaoncE/wBkiXy8y7bqDtvsPWP5hT5X/AbOXJu6iNXzl/Ix5Fiy8rbCSIrbZ/1CfYXSBF/Sdudd/qH3rx3cPanMMeDcH+Tz1zk8nqEebnqCVroPMhCaV5kfGbPZDJterSCf9ZkXYdKC3yyJZxrzeV5lqSnIB3eaEuFApTSqvZP9KPZf/Kc/61ASMHusxrNsKesgG7ZWcd2JER4kaFwymPKIoZqg8fbn9/8AL/zrl3W27xnb+LjzNTnUPLbuVFR24g18dFYqnlaaNGhlA4bUn5MxUm14RW154VeEp2OwoFlkFZFtbFKerkSmmpdl4Ce9oyRohu+Mfk+oqpdU+V44TSg0Mr9LmV477X8fYU+U+fJzw01qX3m/BbJxxGP3LTPPbleHB7N/VeST45c1x6X8zHOri6j2NddwKOMNBPaaKSwMZw68mURHJLLTRt+RR7eMyUEeAjQUVVSDyncXb7DJ23r2I5oxKw/EsrhzmcSqK2X7mYy2qG7azJMhtgXZhq30RpE6NiYi2SChqtx3n9ReB5Ng25+PRcoZeosinV7dPXUUefIdiNpZSJ0+a+EwGG/K6Tqr0bPjnxN89Q8mlBYXOwlm7gOLRcYdiZ023kcmhlTMYnEDAWhq2raKT7AdhUOvDw8toiLyadk5npOwl+1uLcZu1Z0E+vo8gZp7z2DspsIcw18JdnpUdhtz+5RQ1bIlDzCRIgr21/HN3cH2/wAQ27pWslZzF7Gs+YvgkY5UOVzRVTDbIp7hgwYF2Yat8oZeQkTuiv8ABfa5bz+ovA8mwbc/HouUMvUWRTq9unrqKPPkOxG0spE6fNfCYDDfldJ1V6Nnxz4m+eoeTSgi8s9KmWY1FrZMKfVZRFnXpY55qRZJ+Ocg90bUHWWzNCHkhNtDBUFV7fI8wu7e1GU4ZleNUtjTvsT7iuYfqaUGyKYDBuuMstuNInKPuK2pq2PJITvC8H2FGHnubYk1slR7eVWc19xPZyh15m3qqqTWQmasogRvJNZRgFdkF+1NAfcQfInkXtwUnuTuThEbP9hssqsth5FHwisoau0gV0OY3JMobxuvOtedhpsgVEQR5MSVSTkUTlU0cGXbTW2Ibt+R+TFmvlTymJMeGjxLDfCsJpWDdJtGnHO/YerJucKBovVUXSusK6Xmu32GxqmOcyZUlKr5cdtOSZ7vq624Sf8AEFRwk7L8IoLyqaZ3qYz7bzNKCxfqplPfZdMzCfZRLSkpzgeCmcDkGJakwwr76uKhd1F1U6ny59uCzVrKDQyGWOX78tO0wnYsBNhsNuRgU0cFkWmlcTj/AIr055/6XUTkmGW97u7b0rEF8J8uxkm2040SEoIZkpoPHKogiS/CKq8fHK6oujSg2JW12aWLEGgg45YVlcc5iNDasW/bHYTXnBabUlNUHsvK8Jz1AefnlVInxmexGWxt94WUtWFZkNBPyB3G3hgSiFIz/j6lDcR0Gx+6qYIoqQqXKEqEqIuQ8dhQLLIKyLa2KU9XIlNNS7LwE97RkjRDd8Y/J9RVS6p8rxwmtNZTuLt9hk7b17Ec0YlYfiWVw5zOJVFbL9zMZbVDdtZkmQ2wLsw1b6I0idGxMRbJBQ1XKHFh/pWyani7gSq+5pMjSoblY5YDBKUwMeaXIKKuyo7LRIJCiGoGXRDAi4Eu2qnD9NeR09vj1pTWlNmVceVhjbsmgeeIGJ4KB+Mleaa5FRVVR0OzfCLyacpy6t5/UXgeTYNufj0XKGXqLIp1e3T11FHnyHYjaWUidPmvhMBhvyuk6q9Gz458TfPUPJqmubu4Pt/iG3dK1krOYvY1nzF8EjHKhyuaKqYbZFPcMGDAuzDVvlDLyEid0V/gvto5rf0uXcLLMuywrWos6mlun40xBOVCFiYZOe3AnpLDDZB5/C2ZNOF1R0CVUBe+vluntXlMjbV6dc2tVmc4cgSmrp+OtSfM/YLx2jEBsNe47gvZt1sTRfGqIaoqJpgbz+ovA8mwbc/HouUMvUWRTq9unrqKPPkOxG0spE6fNfCYDDfldJ1V6Nnxz4m+eoeTSv3FyzDXtj6HbqBmtbeTY+VnKYtqyqk1sFit9sDAuzWUYBXZKqiEpoD7iCjieRe3BKFfz3ZDKdsNucRrLaodZyXL5qSmqhsCOWLYp447JtonKOEThl405JFIEXg0UUk7zH8l2Ij01LSza2+s0M1yOqru09Ik0VTiO+qB4xUAIR5bJVRwXk7J1FdWXcncnCI2f7DZZVZbDyKPhFZQ1dpArocxuSZQ3jdeda87DTZAqIgjyYkqknIonKpG+pjPtvM0oLF+qmU99l0zMJ9lEtKSnOB4KZwOQYlqTDCvvq4qF3UXVTqfLn24JQRm4keti5rbN1BoUDy9gRHfKgEqIpgh/wDNBJSFC+eUTnledV3Ro1o9k/U5/wDGTdP/AOl//Qzrxu/1o0ai4jRPUTE6J6/OjRo1aWxo0aNAaNGjQGjRo0Bo0aNAaNGjQGjRo0Bo0aNAaNGjQGjRo0Bo0aNAaNGjQGjRo0Bo0aNAaNGjQGjRo0H/2Q==



[image1]:data:image/png;base64,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