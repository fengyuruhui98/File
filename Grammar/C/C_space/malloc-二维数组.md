# malloc动态申请二维数组四种方法
>目录
* [方法一：利用二级指针申请一个二维数组](#方法一利用二级指针申请一个二维数组)
* [方法二：用数组指针形式申请二维数组](#方法二用数组指针形式申请二位数组)
* [方法三：用单独一维数组模拟二维数组](#方法三用单独一维数组模拟二维数组)
* [方法四：使用结构体](#方法四用结构体每个结构体定义col个元素的一维数组)

## **方法一：利用二级指针申请一个二维数组**
```C
#include<stdio.h>  
#include<stdlib.h>  
int main()  
{  
    int i, j;  
    int m, n;  
    printf("请输入行数\n");  
    scanf("%d", &m);  
    printf("请输入列数\n");  
    scanf("%d", &n);  
    int **a = (int **)malloc(sizeof(int *) * m);  
    for(i=0; i<m; i++)  
    a[i] = (int *)malloc(sizeof(int) * n);  
    for(i = 0; i < m; i++)  
    {
        for(j = 0; j < n; j++)  
        {
            printf("%p\n", &a[i][j]);     //输出每个元素地址，每行的列与列之间的地址时连续的，行与行之间的地址不连续
        }
    }
    for(i = 0; i < m; i++)  
    free(a[i]);
    free(a);  
    return 0;  
} 
```
## **方法二：用数组指针形式申请二位数组**
```C
#include<stdio.h>  
#include<stdlib.h>  

int main()  
{  
    int i , j;  
    //申请一个3行2列的整型数组  
    int (* a)[2]=(int( *)[2])malloc(sizeof(int) * 3 * 2);  
    for(i = 0; i < 3; i++)  
    {
        for(j = 0; j < 2; j++)  
        {  
            printf("%p\n", &a[i][j]);  //输出数组每个元素地址，每个元素的地址是连续的
        }
    }
    free(a);
    return 0;  
}  
```
## **方法三：用单独一维数组模拟二维数组**
```C
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    int nrows, ncolumns;
    int *Array;
    int i, j;
    printf("please input nrows&ncolumns:\n");
    scanf("%d%d", &nrows, &ncolumns);
    Array = (int *)malloc(nrows * ncolumns * sizeof(int));   //申请内存空间
    for(i = 0; i < nrows; i++)
    {
        for(j = 0; j < ncolumns; j++)
        {
            Array[i * ncolumns + j] = 1;
            printf("%d ", Array[i * ncolumns + j]);   //用Array[i*ncolumns+j] 访问第i,j个成员
        }
        printf("\n");
    }
    free(Array);
    return 0;
}
```
## **方法四：用结构体，每个结构体定义col个元素的一维数组**
```C
typedef struct {
    int a[5];
}item;
 
item *ar = (item*)malloc(sizeof(item) * 4);
```
### 第一种与第二种使用较多，**如果以指针为参数传递给一个函数一个二维数组，要用方法一，方法二会报错，但方法二简洁。要记得free。**