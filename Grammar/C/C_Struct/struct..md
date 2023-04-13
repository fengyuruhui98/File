C语言中结构体成员变量前.的作用

结构体成员指定初始化

1. 该结构体需要先定义
2. 一个成员变量赋值完后用逗号而不是分号
3. 初始化语句的元素以固定的顺序出现，和被初始化的数组或结构体中的元素顺序一样，这样可以不用按顺序初始化
4. C99才开始支持

验证代码：
```c
#include “stdio.h”

Struct student{

Int year,

Int ID;

};

Int main(){
struct student s1 = {2017, 1111};

struct student s2 = {2022, 1122};

struct student s3 = {

.year = 2019,

.ID = 3333

};

printf("s1.ID = %d \n",s1.ID);

printf("s1.year = %d \n\n",s1.year)

printf("s2.ID = %d \n",s2.ID);

printf("s2.year = %d \n\n",s2.year);

printf("s3.ID = %d \n",s3.ID);

printf("s3.year = %d \n\n",s3.year);

}
```