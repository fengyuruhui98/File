测试平台：64位 X86 Ubuntu

 

1. 代码：

#include <stdio.h>
 
void main(void)
{
    char a[20];
 
    printf("sizeof(a) = %d\n", sizeof(a));
    printf("sizeof(&a) = %d\n",sizeof(&a));
    printf("sizeof(&a[0]) = %d\n",sizeof(&a[0]));
    printf("sizeof(a[0]) = %d\n",sizeof(a[0]));
}
 

2. 结果：

baoli@ubuntu:~/c$ ./a.out
sizeof(a) = 20
sizeof(&a) = 8
sizeof(&a[0]) = 8
sizeof(a[0]) = 1
 

3. 分析：

1）只有sizeof(a)是计算数组大小

2）sizeof(&a)和sizeof(&a[0])是计算指针大小（64位系统是8）
