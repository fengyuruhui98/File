本文，就如题的指针和引用的用法做一个介绍。

首先我们来普及一下概念，说明一下什么是指针和引用，这里借用一下变量名作为过渡。


■指针 ---- 指向某一块内存的变量，其本身存储的信息就是内存地址的首地址，其类型决定指向内存的大小。
        比喻，超市的储物柜。我记住我放包的柜子在第一排右起第三个，这里的1-3就是一个地址，通过地址，我可以找到柜子。这里的地址就是一个指针。

■变量 ---- 某一块内存地址数据值的名字
        比喻，超市的储物柜。我记住我放包的柜子上有一张贴纸，上面写着18，通过18，我可以找到柜子。这里的18，就是变量名了。

■引用 ---- 变量的别名
        比喻，超市的储物柜。柜子的贴纸上面写着18，但是18不好记，我用谐音记忆法，把18想成“要发”，通过“要发”，也可以找到柜子。这里“要发”就是一个引用。

通过上面的比喻可以看出来，要找到柜子，有三种方法：

1. 第一排右起第三个                      地址（指针）
2. 柜子贴纸上号为18的                    变量名
3. “要发”                              引用
 

理解了指针、引用的概念，我们来看看它们的使用吧，这里以经典的值交换为例，来说明int、int*、int&、int*&、int&*、int**。

# 1. int

```c
#include <iostream>
using namespace std;

void swap1(int a, int b){
    int tmp;
    tmp = a;
    a = b;
    b = tmp;
}
int main(){
    int a = 1;
    int b = 2;
    swap1(a, b);
    cout<<"a = "<<a<<endl;
    cout<<"b = "<<b<<endl;
    system("pause");
    return 0;
}
```
交换失败。基础牢靠的同学一眼就看出来，这样的交换只是交换了形参的值，是无法达到交换值的效果的，这段程序很简单，不做细说。

 

# 2. int*


```c
#include <iostream>
using namespace std;

void swap2(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(){
    int a = 1;
    int b = 2;
    swap2(&a, &b);
    cout<<"a = "<<a<<endl;
    cout<<"b = "<<b<<endl;
    system("pause");
    return 0;
}
```
交换成功。上面是的swap2接受的参数是地址，我们传入地址，就可以直接操作实参的值了，这个也很简单，不做细说。

 

# 3. int&

```c
#include <iostream>
using namespace std;

void swap3(int& a, int& b){
    int tmp;
    tmp = a;
    a = b;
    b = tmp;
}

int main(){
    int a = 1;
    int b = 2;
    swap3(a, b);    
        cout<<"a = "<<a<<endl;
    cout<<"b = "<<b<<endl;
    system("pause");
    return 0;
}
```
交换成功。引用即别名，通过引用也是可以直接访问到实参和控制实参的，这里不做细说。

# 4. int*&

```c
#include <iostream>
using namespace std;

void swap5(int *&a, int *&b){
    int tem = *a;
    *a = *b;
    *b = tem;
}

int main(){
    int a = 1;
    int b = 2;

    int *aPtr = &a;
    int *bPtr = &b;
    int *&arPtr = aPtr;
    int *&brPtr = bPtr;

    swap5(arPtr, brPtr);

    cout<<"a = "<<a<<endl;
    cout<<"b = "<<b<<endl;
    system("pause");
    return 0;
}
```

交换成功。这个稍微复杂一点，int*&  value这样一个声明，我们从最接近value的符号看起，是个&，说明value是一个引用，它是一个什么引用呢？再看*，它是一个指针的引用，即指针的别名，我们用*value就可以访问到实参的值了。所以，其交换函数的内部逻辑跟int *是一样的。

 

# 5. int &*

交换成功。这样定义就不合法，如果从字面上来看，我们分析一下，如int &* value，最接近value的是*，说明value是一个指针，再看前面的是&，说明是一个指针的引用，而指针是无法被引用的，所以，这样的定义是不合法的，不要记这个。

 

# 6. int**

```c
#include <iostream>
using namespace std;

void swap6(int **a, int **b){
    int tmp;
    tmp = **a;
    **a = **b;
    **b = tmp;
}

int main(){
    int a = 1;
    int b = 2;
    int *aPtr = &a;//指向数据的指针
    int *bPtr = &b;//指向数据的指针
    int **aaPtr = &aPtr;//指向指针的地址的指针
    int **bbPtr = &bPtr;//指向指针的地址的指针
    swap4(aaPtr, bbPtr);
    cout<<"a = "<<a<<endl;
    cout<<"b = "<<b<<endl;
    system("pause");
    return 0;
}
```
交换成功。同样的，比如int \*\*value， 最接近value的是\*，说明value是一个指针，在前一个是\*，说明是一个指向指针的指针，这样是合法的，那么如何访问value代表的实际参数的值呢？很简单，用\*\*value就可以了，记住\*是一个操作符，如同&一样，不过&是取地址操作符，而\*是取值操作符。