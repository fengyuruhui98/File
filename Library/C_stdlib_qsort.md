# C_stdlib_qsort

## 1. 描述
C 库函数 `void qsort(void *base, size_t nitems, size_t size, int (*compare)(const void *, const void *))`, 对数组进行排序。二分法，时间复杂度为：`O(n*logn)`.不稳定排序。
## 2. 声明
`Void qsort(void *base, size_t nitems, size_t size, int (*compare)(const void *, const void *))`
## 3. 参数
base -- 指向要排序的数组的第一个元素的指针，类型强制转换为 void* 指针。  
nitems – 由base指向的数组中元素的个数。  
size – 数组中每个元素的大小，以字节为单位。  
compare – 用来比较两个元素的函数。如果compare返回值小于0，第一个元素会排在第二个元素之前，如果compare返回值大于0，第一个元素会排在第二个元素后面，如果两个值相等，则不能确定。（其中有两个元素做参数，参数列表是两个空指针，在使用过程中，要指向不同类型的元素，需要转换为相应的类型。  
## 4. 返回值
无返回值，base数组并未声明为const 故直接改变base值。
## 5. 实例
```c
/*对int型数组排序*/
int num[100];
int cmp_int(const void *_a, const void *_b){
	int *a = (int *)_a;
	int *b = (int *)_b;
	return *a - *b; 
}
qsort(num, 100, sizeof(num[0]), cmp_int);
/*对char型数组排序*/
int word[100];
int cmp_word(const void *_a, const void *_b){
	char *a = (char *)_a;
	char *b = (char *)_b;
	return *a - *b; 
}
qsort(word, 100, sizeof(word[0]), cmp_char);
/*对double型数组排序*/
double in[100];
int cmp_double(const void* _a , const void* _b)　　//参数格式固定
{
    double* a = (double*)_a;    //强制类型转换
    double* b = (double*)_b;
    return *a > *b ? 1 : -1;　  //特别注意
    //在对浮点或者double型的一定要用三目运算符
	//因为要是使用像整型那样相减的话，如果是两个很接近的数则可能返回一个很小的小数（大于-1，小于1）
	//而cmp的返回值是int型，因此会将这个小数返回0，系统认为是相等，失去了本来存在的大小关系
}
qsort(in,100,sizeof(in[0]),cmp_double);
/*对字符串进行排序*/
char word[100][10];
int cmp_string(const void* _a , const void* _b)　　//参数格式固定
{
    char* a = (char*)_a;　　//强制类型转换
    char* b = (char*)_b;
    return strcmp(a,b);
}
qsort(word,100,sizeof(word[0]),cmp_string);
```

## 6. 二维qsort
qsort的cmp的写法：

入参实际是数组元素的指针，这里元素是int*，所以入参应该是int**，而要比较的数组是这个指针指向的内容。

将void* a强制转型为本来的样子int **，然后ap指针指向a的第一个元素（这个元素也是指针，指向一个int型一维数组）；
```c
int cmp(const void *a,const void *b)
{
    int *ap = *(int **)a;       
    int *bp = *(int **)b;

    if(ap[0] == bp[0])
        return ap[1] - bp[1];
    else
        return ap[0] - bp[0];
} 
```
对于非malloc申请的多维数组

比如：二维数组 int a[5][2];  
要求：按照二维数组每一维的第一个元素排  
比较参数函数：
```c
int cmp(const void *a, const void *b)
{
    return ((int *)a)[0] - ((int *)b)[0];
}
```
结构体排序相对简单。
结构体的结构：
```c
typedef struct node{
    int x;              
    int y;              
    int z;              
}Node;
```
比较算法cmp，切记强制转型不如->优先级高：
```c
int cmp(const void *a, const void *b)
{
//    return (Node *)a->x - (Node *)b->x;   //错误写法！
//    return ((Node *)a)->x - ((Node *)b)->x;   //正确写法1
    return (*(Node *)a).x - (*(Node *)b).x;     //正确写法2
}
```
排序：

qsort(a, n, sizeof(a[0]), cmp);
