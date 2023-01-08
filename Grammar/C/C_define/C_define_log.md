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