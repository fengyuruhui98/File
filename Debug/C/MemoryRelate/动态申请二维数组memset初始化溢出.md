# 动态申请二维数组memset初始化溢出

在编写一个动态二维数组并且需要将其初始化时，出现了问题，相关部分如下：
```c
	int ** sum = (int **)malloc(sizeof(int *) * numsSize);
    for(int i = 0; i < numsSize; i++){
        sum[i] = (int *)malloc(sizeof(int) * 4);
    }
    memset(sun, 0x00, sizeof(int) * numsSize * 4);
```
编译可以通过，debug时发现错误在于数组越界（在后续调用这个数组时出现了错误，提示访问地址出错）
```
=================================================================
==42==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603000000030 at pc 0x7fb028ee0f3d bp 0x7ffe3cedd2b0 sp 0x7ffe3cedca58
WRITE of size 64 at 0x603000000030 thread T0
    #0 0x7fb028ee0f3c in __interceptor_memset ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:762
    #3 0x7fb02833e082 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24082)
0x603000000030 is located 0 bytes to the right of 32-byte region [0x603000000010,0x603000000030)
allocated by thread T0 here:
    #0 0x7fb028f86808 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cc:144
    #3 0x7fb02833e082 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24082)
SUMMARY: AddressSanitizer: heap-buffer-overflow ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:762 in __interceptor_memset
Shadow bytes around the buggy address:
  0x0c067fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c067fff8000: fa fa 00 00 00 00[fa]fa fa fa fa fa fa fa fa fa
  0x0c067fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==42==ABORTING
```

先看微软官方对memset的解释
[Microsoft Docs memset](https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/memset-wmemset?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DZH-CN%26k%3Dk(memset)%3Bk(DevLang-C%2B%2B)%3Bk(TargetOS-Windows)%26rd%3Dtrue&view=msvc-170&viewFallbackFrom=vs-2017)

具体原因如下：

- memset函数只能够对连续的内存空间进行初始化。如果使用new()函数或是malloc()函数分配的内存可能会不连续，因此初始化时会出现错误。
- 而如果是申请一个静态的数组如maze[25][25]，其内存肯定是连续的，此时对maze()使用memset是不会出现错误的。

正确的使用方法：

因为是maze[i]相当于一个指向一个一维数组的指针，对每一个数组指针maze[i]开辟一个一维数组紧接着对这个一维数组进行初始化。

举例：

```c
	int ** sum = (int **)malloc(sizeof(int *) * numsSize);
    for(int i = 0; i < numsSize; i++){
        sum[i] = (int *)malloc(sizeof(int) * 4);
        memset(sun[i], 0x00, sizeof(int) * 4);
    }
```

在申请大内存空间时：
```c
int a[314][1024][1024];
//---
```
进行操作时可能会有问题，连续内存空间可能无法分配（为初始化仅声明，并不分配内存空间）