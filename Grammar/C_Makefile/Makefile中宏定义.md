# Makefile中宏定义

实际上是gcc命令支持-D宏定义，相当于C中的全局#define：  
>gcc -D name
gcc -D name=definition

Makefile中可以定义变量（和宏很像），但是是给make解释器用的，对所编译的文件完全没有作用。
 
MSTAR:宏定义方式：字串定义方式和值定义方式

```makefile
CC_TVOPTS += -DMS_BOARD_TYPE_SEL=$(BOARD_TYPE_SEL)
CC_TVOPTS += -DMS_SW_CUS_SEL=SW_CONFIG_TRUNK_ATVMM
CC_TVOPTS += -DENABLE_CAPE=0
CC_TVOPTS += -DMS_SW_TEST=\"VIDEOCON.BIN\"
CC_TVOPTS += -DMS_SW_TEST=\"${MEGENAME}\"
```
关键词: Make宏定义 Make传递宏定义 Makefile中添加宏定义 Makefile -D

在Makefile中我们可以通过宏定义来控制源程序的编译。只要在Makefile中的CFLAGS中通过选项-D来指定你于定义的宏即可。

如:
>CFLAGS += -D _SAYHELLO
>在编译的时候加上此选项就可以了： $(CC) $(CFLAGS) $^ -o $@

下面是我写的一个测试文件：  
例如：  

Makefile文件内容为:  
```makefile
CC = gcc
RM = rm
 
CFLAGS += -D _SAYHELLO
 
TARGETS := myapp
 
all:$(TARGETS)
 
$(TARGETS):main.c
$(CC) $(CFLAGS) $^ -o $@
 
clean:
-$(RM) -f *.o
-$(RM) -f $(TARGETS)
```
main.c文件的内容为：
```c
#include <stdio.h>
int main()
{
　　#ifdef _SAYHELLO
　　　　printf("Hello , How are you?\n");
　　#else
　　　　printf("Sorry to lost you. \n");
　　#endif
　　return 0;
}
```


在端口中输入    make clean all
然后输入           ./myapp

结果                  Hello , How are you?