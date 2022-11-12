# 双宿双飞的 malloc 和 free
>目录
* [简介](#简介)
* [void *](#void)
* [malloc](#malloc)
* [free](#free)
* [malloc、free小结](#mallocfree-小结)
* [calloc、realloc、reallocf、valloc](#callocreallocreallocfvalloc)
* [glib与libc](#glibc-与-libc)

## 简介
函数 malloc 和 free 是一对好朋友，几乎是形影不离。有 malloc 的地方就应该有 free 的存在。这两个函数都是对堆内存进行管理的函数，另外还有 calloc、realloc、reallocf、valloc 等堆内存管理函数。

## void *
在进行下面话题之前，我们先回忆一下 void * 是什么？  

void * 表示未确定类型的指针。C/C++规定，void * 类型可以强制转换为任何其它类型的指针。

void * 也被称之为无类型指针，void * 可以指向任意类型的数据，就是说可以用任意类型的指针对 void * 赋值，如下示例：
```c
void *p1;
int *p2;
p1 = p2;
```
但一般不会反过来使用，如下示例在有些编译器上面可以编译通过，有些就不行：
```c
void *p1;
int *p2;
p2 = p1;
```
可以修改一下代码，将 void * 转换为对应的指针类型再进行赋值，如下示例：
```c
void *p1;
int *p2;
p2 = (char *)p1;
```
由于 GNU 和 ANSI 对 void * 类型指针参与运算的规定不一样，所以为了兼容二者并且让程序有更好的兼容性，最好还是将 void * 转换为有明确类型的指针再参与运算，如下示例。
```c
void *pd;
char *pc = (char *)pd;
pc ++;
pc += 1;
```
## malloc
函数原型：

>void * malloc(size_t size);  

malloc 向系统申请分配指定 size 个字节的内存空间，即 malloc 函数用来从***堆空间***中申请指定的 size 个字节的内存大小，返回类型是 void * 类型，如果成功，就会返回指向申请分配的内存，否则返回空指针，所以 malloc 不保证一定成功。

查看函数手册或者直接在Linux、macOS等Unix系统上面直接 man malloc 会显示对应的函数信息：
```
The malloc() function allocates size bytes of memory and returns a pointer to the allocated memory.

If successful, malloc() function return a pointer to allocated memory.  
If there is an error, they return a NULL pointer and set errno to ENOMEM.
```
另外需要注意一个问题，***使用 malloc 函数分配内存空间成功后，malloc 不会对数据进行初始化，里边数据是随机的垃圾数据，所以一般结合 memset 函数和 malloc 函数 一起使用。***
```c
int *arr;
arr = (int *)malloc(10 * sizeof(int));
if (NULL != arr) {
    memset(arr, 0, 10 * sizeof(int));
    printf("arr: %p\n", arr);
}
```
```c
char *arr;
arr = (char *)malloc(10 * sizeof(char));
if (NULL != arr) {
    memset(arr, '\0', 10 * sizeof(char));
    printf("arr string: %s\n", arr);
}
```
为了安全起见，建议可以考虑使用 calloc() 函数，后面会提到它。

函数 free、malloc、calloc() 都被包含在 stdlib.h 头文件中，当然了 malloc.h 头文件也有对应的函数声明，实现在 malloc.c 文件中。

关于 malloc 函数中的 size 参数务必要大于指针本身的数值，在32位操作系统上，指针占用4字节的大小；在64位操作系统上，指针占用8字节的大小。

## free
函数原型：

>void free(void *ptr);

我们知道在 C 语言中， 堆上的内存空间不会自动释放（Java 有自动回收机制，而 C 语言没有），直到调用 free 函数，才会释放堆上的存储空间，即 free 函数会释放指针指向的内存分配空间。

下面是函数手册查到关于 free 函数的资料：
```
The free() function deallocates the memory allocation pointed to by ptr.
If ptr is a NULL pointer, no operation is performed.
```
对于 free 函数我们要走出一个误区，不要以为调用了 free 函数，变量就变为 NULL 值了。本质是 free 函数***只是割断了指针所指的申请的那块内存之间的关系，并没有改变所指的地址（本身保存的地址并没有改变）***。如下示例：
```c
char *pchar = (char *)malloc(10 * sizeof(char));
        
if (NULL != pchar) {
    strcpy(pchar, "blog");
    /* pchar所指的内存被释放，但是pchar所指的地址仍然不变 */
    free(pchar);
    
    /* 该判断没有起到防错作用，此时 pchar 并不为 NULL */
    if (NULL != pchar) {
        strcpy(pchar, "it");
        printf("pchar: %s", pchar);
    }
}
```
正确且安全的做法是对指针变量先进行 free 然后再将其值置为 NULL，如下下面示例：
```c
char *pchar = (char *)malloc(10 * sizeof(char));
        
if (NULL != pchar) {
    strcpy(pchar, "blog");
    /* pchar所指的内存被释放，但是pchar所指的地址仍然不变 */
    free(pchar);
    /* 将其置为 NULL 值 */
    pchar = NULL;
    
    /* 该判断没有起到防错作用，此时 pchar 并不为 NULL */
    if (NULL != pchar) {
        strcpy(pchar, "it");
        printf("pchar: %s", pchar);
    }
}
```
函数 free 只能释放alloc的内存空间，对于其他非alloc的指针变量使用free会出现错误，如下面的例子：
```c
int a = 0x2010;
int *p;
p = &a;
free(p);
```
运行后直接跪了，报类似下面的错误信息：

pointer being freed was not allocated.
另外，告诉大家一个秘密，free 一个空指针没问题。如下例子：
```c
int *p;
p = NULL;
free(p);
```
代码可以正常运行，在 free 函数源码为证：
```
/* free(0) has no effect */
if (mem == 0){
	return;   
}
```
而 NULL 在源码（在 morecore.c 文件中）中定义就是0，如下：
```c
#ifndef NULL
# define NULL 0
#endif
```
## malloc、free 小结
### 1、连续内存块

malloc 函数申请的是连续的一块内存，如果所申请的内存块大于目前堆上剩余内存块，则内存分配会失败，函数返回 NULL 值。

***注意：上面说的 堆上剩余内存块 不是所有剩余内存块之和，而是连续的内存。***

### 2、双宿双飞才好

调用 malloc 函数多余 free 函数会发生内存泄漏，这个很好理解，因为申请过的内存没有被释放完。调用 malloc 函数少于free 函数，肯定会出错。换句话说，在程序中 malloc 的使用次数务必要和 free 相等，否则必有隐患或者发生错误。

如下面的例子 free 两次指针变量就会在运行时报错： malloc: *** error for object 0x10071be90: pointer being freed was not allocated，也称之为 double free。
```c
char *pchar = (char *)malloc(10 * sizeof(char));
free(pchar);
free(pchar);
```
对指针变量进行 free 之后，一定要记得对其赋值为 NULL，否则该指针就是一个野指针，这个在上面已经说明。

### 3、0字节的内存有毒

使用 malloc 函数也可以申请0字节的内存，该函数的返回值并不是 NULL，而是返回一个正常的内存地址，所以如果使用这种方式申请的内存很危险，如下面的例子，指针 pchar 是一个使用 malloc 函数创建的占用0字节的内存空间的一个指针变量，if (NULL == pchar) 并没有生效，而是执行了 else 语句中的代码。
```c
char *pchar = (char *)malloc(0);
if (NULL == pchar) {
    printf("malloc 0 byte memory failed.\n");
} else {
    printf("malloc 0 byte successfully and pchar: %s.\n", pchar);
    printf("pchar: %s.\n", pchar);
}
```
这中方式的指针类似于一个野指针，可以使用但有风险，不知道这个指针后面的内存空间被谁使用着，要是被核心进程使用，会造成相应程序的崩溃。

### 4、调皮的free

使用非alloc方式创建的内存，使用 free 函数会报错，这个要注意。另外，free 空指针没有影响。

## calloc、realloc、reallocf、valloc
### 1、calloc 函数

>void * calloc(size_t count, size_t size);

在堆上，分配 n*size 个字节，并初始化为0，返回 void *类型，返回值情况跟 malloc 一致。

函数 malloc() 和函数 calloc() 的主要区别是前者不能初始化所分配的内存空间，而后者能。如果由 malloc() 函数分配的内存空间原来没有被使用过，则其中的每一位可能都是0；反之，如果这部分内存曾经被分配过，则其中可能遗留有各种各样的数据。也就是说，使用 malloc() 函数的程序开始时(内存空间还没有被重新分配)能正常进行，但经过一段时间(内存空间还已经被重新分配)可能会出现问题。

函数 calloc() 会将所分配的内存空间中的每一位都初始化为零，也就是说，如果你是为字符类型或整数类型的元素分配内存，那么这些元素将保证会被初始化为0；如果你是为指针类型的元素分配内存，那么这些元素通常会被初始化为空指针；如果你为实型数据分配内存，则这些元素会被初始化为浮点型的零。

>The calloc() function contiguously allocates enough space for count objects that are size bytes of memory each and returns a pointer to the allocated memory.  
The allocated memory is filled with bytes of value zero.
### 2、realloc() 函数

>void * realloc(void *ptr, size_t size);

realloc() 会将 ptr 所指向的内存块的大小修改为 size，并将新的内存指针返回。假设之前内存块的大小为 n，如果 size <= n，那么截取的内容不会发生变化，如果 size > n，那么新分配的内存不会被初始化。

对于上面说的新的内存指针地址可能变也可能不变，假如原来alloc的内存后面还有足够多剩余内存的话，realloc后的内存=原来的内存+剩余内存，realloc还是返回原来内存的地址即不会创建新的内存。假如原来alloc的内存后面没有足够多剩余内存的话，realloc将申请新的内存，然后把原来的内存数据拷贝到新内存里，原来的内存将被free掉，realloc返回新内存的地址。

另外要注意，如果 ptr = NULL，那么相当于调用 malloc(size)；如果 ptr != NULL且size = 0，那么相当于调用 free(ptr)。

当调用 realloc 失败的时候，返回NULL，并且原来的内存不改变，不会释放也不会移动。

>The realloc() function tries to change the size of the allocation pointed to by ptr to size, and returns ptr.   
If there is not enough room to enlarge the memory allocation pointed to by ptr, realloc() creates a new allocation, copies as much of the old data pointed to by ptr as will fit to the new allocation, frees the old allocation, and returns a pointer to the allocated memory.    
If ptr is NULL, realloc() is identical to a call to malloc() for size bytes.    
If size is zero and ptr is not NULL, a new, minimum sized object is allocated and the original object is freed.    
When extending a region allocated with calloc(3), realloc(3) does not guarantee that the additional memory is also zero-filled.

看一下源码的关键部分，如下：
```c
void * __libc_realloc (void *oldmem, size_t bytes)
{
	// 省略... 
	
#if REALLOC_ZERO_BYTES_FREES
  	if (bytes == 0 && oldmem != NULL) {
    	__libc_free (oldmem); return 0;
    }
#endif

	// 省略...
}
```
这里可以看出，如果 oldmem 不为NULL且 bytes 为0的情况下，会自动进行 free 操作。如下面的例子运行后会报错。
```c
#include <stdlib.h>
#include <stdio.h> 

int main()
{
    char *pre_ptr = (char *)malloc(20);
    int bytes = 0;
    if(NULL != pre_ptr) {
        void *ptr = realloc(pre_ptr, bytes);
        // 对pre_ptr进行了double free，这种条件下源码中会进行一次free
        free(pre_ptr);
    }
	
    return 0;
}
```
所以为了保险起见，要对 bytes 进行判断再做处理。

传递给 realloc 的指针变量必须是先前通过 malloc()、 calloc() 或 realloc() 等alloc分配的，否则会报 pointer being realloc'd was not allocated. 错误。如下例子（运行直接报错）：
```c
char *p = "";
void *newP = realloc(p, 10);
```
### 3、reallocf() 函数

>void * reallocf(void *ptr, size_t size);

reallocf() 函数是由 FreeBSD 实现的，它会在任何情况下释放输入的指针（即使是再分配失败之后）。reallocf() 一样会调用 realloc 函数，但是只有我们在获得空的指针之后才会调用 free 函数。

下面是 reallocf 函数具体的实现部分：
```c
void * reallocf(void *p, size_t size) 
{
    void *ptr = realloc(p, size);
    if (!p) {
        free(p);
    }
    return ptr;
}
```

>The reallocf() function is identical to the realloc() function, except that it will free the passed pointer when the requested memory cannot be allocated.  
This is a FreeBSD specific API designed to ease the problems with traditional coding styles for realloc causing memory leaks in libraries.  

### 4、valloc() 函数

>void * valloc(size_t size);

这个函数是最少见也是最少用的一个函数。

malloc 或 realloc 返回的是以8字节对齐的内存地址，在64bits上是16字节对齐。然而 memalign 或 valloc 可以更大的粒度进行字节对齐。

valloc 是一个废弃的函数，分配 size 大小的字节，返回已分配的内存地址指针，其内存地址将是页大小(page size)的倍数，如果分配失败返回 NULL。
>The valloc() function allocates size bytes of memory and returns a pointer to the allocated memory. 
The allocated memory is aligned on a page boundary.  

说了这么多，接下来我们看一下例子，如下代码示例：
```c
// 对其分配内存，这个时候pchar值是随机的垃圾值
char *pchar = (char *)malloc(16);
// 手动初始化pchar的值，下面的方法则不需要
memset(pchar, 0, 16);

// calloc分配内存，会自动设置为0，不需要memset
char *pchar_orig = (char *)calloc(12, sizeof(char));

// 在原内存基础上，在堆内存空间中连续增加内存
// 如果原内存没有连续空间可拓展，realloc会新分配一个空间，将原有内存copy到新空间，然后释放原内存  
// 注意：realloc和malloc，只分配内存不进行赋值操作
char *pchar_dest = (char *)realloc(pchar_orig, 10);
  
// 相当于 malloc(60)
char *pchar_ini = (char *)realloc(NULL, 60);

free(pchar);
pchar = NULL;

free(pchar_orig);
pchar_orig = NULL;

free(pchar_dest);
pchar_dest = NULL;

free(pchar_ini);
pchar_ini = NULL;
```

## glibc 与 libc
上面说过这些内存管理函数都在头文件 stdlib.h 中，函数实现都在 malloc.c 文件中，这些都可以在 glibc 中找到对应的代码，我下载的 glibc 是 2.29 这个版本。

libc 是 Stantard C Library 的简称，它是符合ANSI C标准的一个标准函数库。libc库提供C语言中所使用的宏，类型的定义，字符串操作符，数学计算函数以及输入输出函数等。正如ANSI C是C语言的标准一样，libc只是一个函数库标准，每个操作系统都会按照该标准对标准库进行具体实现。通常我们所说的libc是特指某个操作系统的标准库，比如：在Linux操作系统下所说的libc即glibc。glibc是类Unix操作系统中使用最广泛的libc库，它的全称是GNU C Library.

glibc是 Linux 下的 C 函数库。libc 库是Linux下的ANSI C的函数库也泛指一切符合C规范的库，glibc 是Linux下的GNU C函数库，是一种 libc 的实现。glibc 本身是GNU旗下的C标准库，后来逐渐成为了Linux的标准C库。Linux下面的标准C库不仅有这一个，如 uclibc、klibc，但是 glibc 无疑是用得最多的，用 glibc 作为系统的C函式库，是GNU/Linux演进的一个重要里程碑，可以查看 维基百科 对 glibc 的介绍。

还有比如Android的Bionic库也是一种libc的实现，只是在Android上面使用罢了。Bionic是Google为Android操作系统提供的BSD标准C库的衍生库，专门为移动计算而精心设计的，针对移动设备上有限的CPU周期和可用内存进行了裁剪以提高工作效率。

这里也说一下基本的C语言函数库，这个库可以根据头文件划分为 15 个部分，其中包括：

- 文件 assert.h：包含宏和信息，用于进行诊断，帮助程序调试；

- 文件 ctype.h：包含用来测试某个特征字符的函数的函数原型，以及用来转换大小写字母的函数原型；

- 文件 errno.h：定义用来报告错误条件的宏；

- 文件 float.h：包含系统的浮点数大小限制；

- 文件 math.h：包含数学库函数的函数原型；

- 文件 stddef.h：包含执行某些计算 C 所用的常见的函数定义；

- 文件 stdio.h：包含标准输入输出库函数的函数原型，以及他们所用的信息；

- 文件 stdlib.h：包含数字转换到文本，以及文本转换到数字的函数原型，还有内存分配、随机数字以及其他实用函数的函数原型；

- 文件 string.h：包含字符串处理函数的函数原型；

- 文件 time.h：包含时间和日期操作的函数原型和类型；

- 文件 stdarg.h：包含函数原型和宏，用于处理未知数值和类型的函数的参数列表；

- 文件 signal.h：包含函数原型和宏，用于处理程序执行期间可能出现的各种条件；

- 文件 setjmp.h：包含可以绕过一般函数调用并返回序列的函数的原型，即非局部跳转；

- 文件 locale.h：包含函数原型和其他信息，使程序可以针对所运行的地区进行修改。地区的表示方法可以使计算机系统处理不同的数据表达约定，如全世界的日期、时间、美元数和大数字；

- 文件 limits.h：决定了各种变量类型的各种属性。定义在该头文件中的宏限制了各种变量类型（比如 char、int 和 long）的值；

在 glibc 库中可以找到 malloc.c 文件(路径是 glibc-2.29/malloc/malloc.c )，从中可以看到如下别名的定义：
```c
strong_alias (__libc_calloc, __calloc) weak_alias (__libc_calloc, calloc)
strong_alias (__libc_free, __free) strong_alias (__libc_free, free)
strong_alias (__libc_malloc, __malloc) strong_alias (__libc_malloc, malloc)
strong_alias (__libc_memalign, __memalign)
weak_alias (__libc_memalign, memalign)
strong_alias (__libc_realloc, __realloc) strong_alias (__libc_realloc, realloc)
strong_alias (__libc_valloc, __valloc) weak_alias (__libc_valloc, valloc)
strong_alias (__libc_pvalloc, __pvalloc) weak_alias (__libc_pvalloc, pvalloc)
strong_alias (__libc_mallinfo, __mallinfo)
weak_alias (__libc_mallinfo, mallinfo)
strong_alias (__libc_mallopt, __mallopt) weak_alias (__libc_mallopt, mallopt)
__malloc 是 __libc_malloc 的别名，malloc 也是 __libc_malloc 的别名。其他以此类推。
```
所以看 malloc 函数的源码，直接看 __libc_realloc 函数实现即可，看 free 的源码可以看 __libc_free 的实现。
```c
void *
__libc_realloc (void *oldmem, size_t bytes)
```
也可以在线查看 glibc 的源码，点击 [这里](https://codebrowser.dev/glibc/glibc/) 前往欣赏！

以后要是有人问你在哪里可以下载libc的源码，你就可以告诉他libc只是一个标准，不同的平台上面有不同的libc实现，比如glibc，bionic等。