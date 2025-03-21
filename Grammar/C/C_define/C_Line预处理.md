# C语言#line预处理

#line 的作用是改变当前行数和文件名称，它们是在编译程序中预先定义的标识符命令的基本形式如下：

#line number["filename"]

其中[]内的文件名可以省略。例如：

#line 30 a.h(其中，文件名a.h 可以省略不写)

#line指令改变__LINE__和__FILE__的内容。__LINE__和__FILE__都是编译程序中预定义的标识符。标识符__LINE__的内容是当前被编译代码行的行号，__FILE__的内容是当前被编译源文件的文件名。

其中，number 是正整数并变成__LINE__的新值；可选的“filename”是合法文件标识符并变成__FILE__的新值。#line主要用于调试和特殊应用。

所以针对上面_LINE_=30,_FILE_=a.h

这条指令可以改变当前的行号和文件名，例如上面的这条预处理指令就可以改变当前的行号为30，文件名是a.h。初看起来似乎没有什么用，不过，他还是有点用的，那就是用在编译器的编写中，我们知道编译器对C 源码编译过程中会产生一些中间文件，通过这条指令，可以保证文件名是固定的，不会被这些中间文件代替，有利于进行分析。

看个例子再说：


#include "stdio.h"  
void Test();  
#line 10 "Hello.c"  
int main(int argc, char* argv[])  
{  
#define CONST_NAME1 "CONST_NAME1"  
    printf("%s/n",CONST_NAME1);  
#undef CONST_NAME1  
    printf("%s/n",CONST_NAME1);  
    {  
    #define CONST_NAME2 "CONST_NAME2"  
        printf("%s/n",CONST_NAME2);  
    }  
    printf("%s/n",CONST_NAME2);  
    return 0;  
}  
void Test()  
{  
    printf("%s/n",CONST_NAME2);  
}  


提示如下的编译信息：
Hello.c(15) : error C2065: 'CONST_NAME1' : undeclared identifier
表示当前文件的名称被认为是Hello.c， #line 10 "Hello.c"所在的下一行被认为是第10行，因此提示第15行出错。

形式2
语法格式如下:
# line constant
其作用在于编译的时候，准确输出出错代码所在的位置（行号），而在源程序中并不出现行号，从而方便程序员准确定位。一般方式很少用