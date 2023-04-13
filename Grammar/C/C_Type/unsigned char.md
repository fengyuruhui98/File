1. 区别与联系：

1）在定义字符类型时一般使用char来定义

2）char被当成有符号或是无符号视不同编译器决定，由于ASCII码范围从0-0x7F（127），所以char无论被当成有符号还是无符号类型均可

3）在定义数值类型时使用 signed char 和 unsigned char，signed char表示-128—127，unsigned char表示0—255

4）三者在内存里都表示8位一个字节大小，所以当只是被用来存储二进制数据时不关心是有符号还是无符号

 

 

测试平台：64位 X86 Ubuntu

 

2. 代码：

#include <stdio.h>
 
void main(void)
{
    char a = 0xff;
    signed char sa = 0xff;
    unsigned char ua = 0xff;
 
    printf("a = %d, a = %c\n", a, a);
    printf("sa = %d, ua = %d\n", sa, ua);
 
}
 

3. 结果：

baoli@ubuntu:~/c$ ./a.out
a = -1, a = �
sa = -1, ua = 255
 

 

4. 分析：

1）在Ubuntu gcc下char被定义为 signed char
