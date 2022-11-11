# gcc编译链接原理及使用
## 1、gcc 和 arm-linux-gcc的常用选项
gcc 的使用方法：  
>gcc [选项] 文件名  

gcc常用选项：  
　　-v *(查看gcc 编译器的版本，显示gcc执行时的详细过程)*  
　　-o <file> *(指定输出文件名为file，这个名称不能跟源文件名同名)*  
　　-E *(只预处理，不会编译、汇编、链接)*  
　　-S *(只编译，不会汇编、链接)*  
　　-c *(编译，汇编，不链接)*  

## 方式1：  
gcc hello.c *(输出一个 a.out，然后 ./a.out 来执行该应用程序)  
gcc -o hello hello.c *(输出hello ，然后 ./hello 来执行该应用程序。)*  
## 方式2：  
gcc -E -o hello.i hello.c  
gcc -S -o hello.s hello.i  
gcc -c -o hello.o hello.s  
gcc -o hello hello.o  
>.o *(object file (OBJ文件))  

小结：  
1）输入文件的后缀名和选项共同决定gcc到底执行哪些操作。  
一个C/C++程序要经过预处理(preprocessing)、编译(compilation)、汇编(assembly)、链接(linking)四步成为可执行文件。
1. 预处理：将include文件插入源文件中、宏定义展开、根据条件编译命令选择要使用的代码，将这些代码输出到一个`.i`文件中等待进一步处理。预处理用到arm-linux-cpp工具。
2. 编译：将C/C++代码（如之上的`.i`文件）“翻译”成汇编代码，所用到的工具为`ccl`不是arm-linux-ccl
3. 汇编：将第二步输出的汇编代码翻译成机器代码，在Linux上一般表现为ELF文件（OBJ文件），用到的工具为arm-linux-as。
4. 链接：将上步形成的OBJ文件和系统库的OBJ文件、库文件链接起来，最终生成可执行文件，用到arm-linux-ld。  
2）在编译过程中，除非使用了-E、-S、-C选项（或者编译出错阻止了完整的编译过程）否则最后的步骤都是链接。
## 方式3：
gcc -c -o hello.o hello.s  
gcc -o hello hello.o  
gcc会对.c文件默认进行预处理操作，-c再来指明了编译、汇编，从而得到.o文件  
再通过gcc -o hello hello.o将.o文件进行链接，得到可执行应用程序。  
链接就是将汇编生成的OBJ文件、系统库的OBJ文件、库文件链接起来，最终生成可以在特定平台运行的可执行程序。  
crt1.o、crti.o、crtbegin.o、crtend.o、crtn.o是gcc加入的系统标准启动文件，对于一般应用程序，这些启动是必需的。  
-lc：链接libc库文件，其中libc库文件中就实现了printf等函数。  
gcc -v -nostdlib -o hello  hello.o会提示因为没有链接系统标准启动文件和标准库文件，而链接失败。  
这个-nostdlib选项常用于裸机 /bootloader 、linux 内核等程序，因为它们不需要启动文件、标准库文件。  
一般应用程序才需要系统标准启动文件和标准库文件。  
裸机 /BootLoader、linux内核灯程序不需要启动文件、标准库文件。  
动态链接使用动态链接库进行链接，生成的程序在执行的时候需要加载所需的动态库才能巡行。  
动态链接生成的程序体积较小，但是必须依赖所需的动态库，否则无法执行。  
静态链接使用静态库进行链接，生成的程序包含程序运行所需要的全部库，可以直接运行，不过静态链接生成的程序比较大。  
gcc -c -o hello.o hello.c  
gcc -o hello_shared  hello.o  
gcc -static -o hello_static hello.o  
## 2、举例说明编译链接过程
一个C/C++文件要经过预处理(preprocessing)、编译(compilation)、汇编(assembly)、和连接(linking)才能变成可执行文件。
以下列程序为例，追层来分析编译过程。 
hello.c：
```c
#include <stdio.h>

#define   MAX  20 
#define   MIN  10 

#define  _DEBUG
#define   SetBit(x)  (1<<x) 

int main(int argc, char* argv[])
{
    printf("Hello World \n");
    printf("MAX = %d,MIN = %d,MAX + MIN = %d\n",MAX,MIN,MAX + MIN); 

#ifdef _DEBUG
    printf("SetBit(5) = %d,SetBit(6) = %d\n",SetBit(5),SetBit(6));
    printf("SetBit( SetBit(2) ) = %d\n",SetBit( SetBit(2) ));       
#endif    
    return 0;
}
```
① 预处理:
gcc -E -o hello.i hello.c
预处理就是将要包含(include)的文件插入原文件中、将宏定义展开、根据条件编译命令选择要使用的代码，最后将这些代码输出到一个“.i”文件中等待进一步处理。   
② 编译:  
gcc -S -o hello.s hello.i  
编译就是把C/C++代码(比如上面的”.i”文件)“翻译”成汇编代码。  
③ 汇编:  
gcc -c -o hello.o hello.s  
.o:object file(OBJ文件) 这里表现为二进制目标文件：   
汇编就是将第二步输出的汇编代码翻译成符合一定格式的机器代码，在Linux系统上一般表现位ELF目标文件(OBJ文件)。  
④ 链接:  
gcc -o hello hello.o  
链接就是将汇编生成的OBJ文件、系统库的OBJ文件、库文件链接起来，最终生成可以在特定平台运行的可执行程序。   
总结：在编译过程中。除非使用了”-c”，“-S”,或”-E”选项(或者编译错误阻止了完整的过程)，否则统一完整链接步骤。  
譬如：gcc hello.c 和gcc -o hello hello.c都已经完成链接操作。   
又如：gcc -c -o hello.o hello.c   
链接原理：  
gcc -c -o hello.o hello.c  
不作最后一步链接，得到hello.o二进制OBJ文件
gcc -v -o hello hello.o 我们来看一样链接过程是怎样的：  
crt1.o、crti.o、crtbegin.o、crtend.o、crtn.o是gcc加入的系统标准启动文件，对于一般应用程序，这些启动是必需的。
-lc：链接libc库文件，其中libc库文件中就实现了printf等函数。  
① 动态链接：动态链接使用动态链接库进行链接，生成的程序在执行的时候需要加载所需的动态库才能运行。  动态链接生成的程序体积较小，但是必须依赖所需的动态库，否则无法执行。
默认使用动态链接：gcc -o hello_shared hello.o  
② 静态链接：静态链接使用静态库进行链接，生成的程序包含程序运行所需要的全部库，可以直接运行，不过静态链接生成的程序体积较大。  
gcc -static -o hello_static hello.o  
③ -nostartfiles
不链接系统标准启动文件，而标准库文件仍然正常使用：  
gcc -v -nostartfiles -o hello hello.o  
④ -nostdlib（最常用）
不链接系统标准启动文件和标准库文件：   
gcc -v -nostdlib -o hello hello.o
- 会提示因为没有链接系统标准启动文件和标准库文件，而链接失败。 
- 这个-nostdlib选项常用于裸机/bootloader、linux内核等程序，因为它们不需要启动文件、标准库文件。