C编译器允许main()没有参数或者有多个参数。main（）有两个参数时，第一个参数是命令行中的字符串数量（int argc)。系统用空格的表示一个字符串的结束和下一个字符串的开始。而char * argv[]则用来存储命令行的参数。因此看我的允许结果

```c
#include <stdio.h>

int main(int argc, char * argv[]){
    int count;
    printf("Program has %d arguments\n", argc - 1);
    for(int i = 1; i < argc; ++i)printf("%d:%s\n", i, argv[i]);
    return 0;
}
```
.\material\ex_3.exe yu cheng jing
Argv[0] = .\material\ex_3.exe(运行程序名称)
Argv[1] = yu
Argv[2] = cheng
Argv[3] = jing
int main(int argc,char * argv[])。argc,argv名字不固定，可自行定义


