# C语言printf中%s、%*s、%*.*s的作用以及实现一个进度条
在c语言printf的解析格式里，如果遇到`%`符号，则认为是一个格式化标识符，它会自动读取下一个字符作为占位符，如果`%`后面是`*`则取参数对应位作为宽度，如果是`%.`或`%*.`，这个`.`只要出现在`%`上下文中，那么后面一个字符就要求为宽度占位数  
下面是在字符串上的用法：  
最多输出5个字符，超出5个不会输出后续字符  
>printf("%.*s\n", 5,"hello word");  

至少输出15个字符，如果少于15个则在左边用空格来补齐  
>printf("%*s\n", 15, "hello word");  

这个比较特殊，首先前面的*代表至少输出多少个字符，如果少于这几个字符则在左边空格补齐  
第二个*代表最多输出多少个字符  
第一个*的值为10，意味着至少输出10个字符  
第二个*的值为5，意味着最多只能输出5个字节  
所以这段参数的含义是至少输出10个字符，但最多只能输出5个字符
打印结果是（“-”代表空格）：-----hello  
因为有第二个*的原因，所以我们只能输出hello，但又由于第一个*的限制  
printf发现最终只输出了5个字符，所以又在左边用空格进行了补齐  
>printf("%*.*s\n", 10, 5, "hello word");

下面的代码与上面格式，printf允许使用%常数与%*来作为控制输出宽度的限制：  
```c
printf("%.5s\n", "hello word");
printf("%15s\n", "hello word");
printf("%10.15s\n", "hello word");
```
最终的输出：
```c
hello
     hello word
     hello
hello
     hello word
hello word
```
结合`\r`实现一个linux上常见的进度条：
```c
#include <stdio.h>
#include <unistd.h>
#define PBSTR "##################################################"
#define PBWIDTH 50
 
void printProgress(int offset, int total) {
 
        int val = offset * 100 / total;
 
        int lpad = offset * PBWIDTH / total;
 
        int rpad = PBWIDTH - lpad;
 
        printf("\r%3d%% [%.*s%*s] [%d/%d]", val, lpad, PBSTR, rpad, "", offset, total);
 
        fflush(stdout);
 
}
 
void main(){
        for(int i = 0;i < 50;++i){
                
                printProgress(i,100);
                sleep(1);
        }
}
```
最终输出效果：
```c
 16% [########                                          ] [16/100]
 ```