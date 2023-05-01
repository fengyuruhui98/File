# 线程参数
Posix线程中的线程属性pthread_attr_t主要包括scope属性、detach属性、堆栈地址、堆栈大小、优先级。在pthread_create中，把第二个参数设置为NULL的话，将采用默认的属性配置。

线程属性结构如下：
```c
typedef struct
{
        int                       detachstate;     线程的分离状态
        int                       schedpolicy;     线程调度策略
        struct sched_param        schedparam;      线程的调度参数
        int                       inheritsched;    线程的继承性
        int                       scope;           线程的作用域
        size_t                    guardsize;       线程栈末尾的警戒缓冲区大小
        int                       stackaddr_set;
        void *                    stackaddr;       线程栈的位置
        size_t                    stacksize;       线程栈的大小
}pthread_attr_t;
```
## __detachstate
表示新线程是否与进程中其他线程脱离同步， 如果设置为PTHREAD_CREATE_DETACHED 则新线程不能用pthread_join()来同步，且在退出时自行释放所占用的资源。
缺省为PTHREAD_CREATE_JOINABLE状态。这个属性也可以在线程创建并运行以后用pthread_detach()来设置，而一旦设置为PTHREAD_CREATE_DETACH状态（不论是创建时设置还是运行时设置）则不能再恢复到PTHREAD_CREATE_JOINABLE状态。

线程的分离状态决定一个线程以什么样的方式来终止自己。**在默认情况下线程是非分离状态的，这种情况下，原有的线程等待创建的线程结束。**只有当pthread_join() 函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。分离线程没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。
通俗的说也就是：**我们知道一般我们要等待(pthread_join)一个线程的结束，主要是想知道它的结束状态，否则等待一般是没有什么意义的！**但是if有一些线程的终止态我们压根就不想知道，那么就可以使用“分离”属性，那么我 们就无须等待管理，只要线程自己结束了，自己释放src就可以咯
```c
 #include <pthread.h>
int pthread_attr_getdetachstate(const pthread_attr_t * attr, int * detachstate);
int pthread_attr_setdetachstate(pthread_attr_t * attr, int detachstate);
```
参数：attr:线程属性变量，detachstate:分离状态属性
若成功返回0，若失败返回-1。
设置的时候可以有两种选择：
<1>.detachstate参数为：PTHREAD_CREATE_DETACHED 分离状态启动
<2>.detachstate参数为：PTHREAD_CREATE_JOINABLE 正常启动线程

## __schedpolicy
表示新线程的调度策略，主要包括：
SCHED_OTHER（正常、非实时）
SCHED_RR（实时、轮转法）
SCHED_FIFO（实时、先入先出）
缺省为SCHED_OTHER，后两种调度策略仅对超级用户有效。
运行时可以用过pthread_setschedparam()来改变。

函数pthread_attr_setschedpolicy和pthread_attr_getschedpolicy分别用来设置和得到线程的调度策略。
```c
int pthread_attr_getschedpolicy(const pthread_attr_t *, int * policy)
int pthread_attr_setschedpolicy(pthread_attr_*, int policy)
```

参数：
attr 线程属性变量
policy 调度策略
若成功返回0，若失败返回-1。

所谓调度策略也就是我们之前在OS中所学过的那些调度算法：
SCHED_FIFO ：先进先出
SCHED_RR ：轮转法
SCHED_OTHER ：其他方法
SCHED_OTHER是不支持优先级使用的,而SCHED_FIFO和SCHED_RR
支持优先级的使用,他们分别为1和99,数值越大优先级越高.
注意：
> 此处的SCHED_FIFO是允许被高优先级抢占的！
> 也就是有高优先级的必须先运行
> SCHED_RR是设置一个时间片
> 当有SCHED_FIFO或SCHED_RR策赂的线程在一个条件变量上等持或等持加锁同一个互斥量时，它们将以优先级顺序被唤醒。即，如果一个低优先级的SCHED_FIFO线程和一个高优先织的SCHED_FIFO线程都在等待锁相同的互斥且，则当互斥量
被解锁时，高优先级线程将总是被首先解除阻塞。

## __schedparam
一个struct sched_param结构，目前仅有一个sched_priority整型变量表示线程的运行优先级。这个参数**仅当调度策略为实时（即SCHED_RR或SCHED_FIFO）时才有效**，并可以在运行时通过pthread_setschedparam()函数来改变，缺省为0。
函数pthread_attr_getschedparam 和pthread_attr_setschedparam分别用来设置和得到线程的调度参数。
```c
int pthread_attr_getschedparam(const pthread_attr_t *,struct sched_param *);
int pthread_attr_setschedparam(pthread_attr_t *,const struct sched_param *);
```
参数：
attr 线程变量属性
param sched_parm 结构体
若成功返回0，若失败返回-1。
/usr/include /bits/sched.h
struct sched_param
{
int sched_priority; // 参数的本质就是优先级
};
注意：大的权值对应高的优先级!
系统支持的最大和最小的优先级值可以用函数：
sched_get_priority_max和sched_get_priority_min得到！
```c
#include <pthread.h>
int sched_get_priority_max( int policy );
int sched_get_priority_min( int policy );
```
参数：max_： 系统支持的优先级的最小值
min_ ： 系统支持的优先级的最大值
```c
max_ = sched_get_priority_max( policy );
min_ = sched_get_priority_min( policy );
```
注意参数是policy调用策略，也就是说对于不同的策略的值是不
一样的！
policy = SCHED_OTHER
max_priority = 0
min_priority = 0
```c
Show SCHED_FIFO of priority
                    max_priority = 99
                    min_priority = 1
                    
Show SCHED_RR of priority
                    max_priority = 99
                    min_priority = 1
   
Show priority of current thread
                    priority = 0
```
## __scope
表示线程间竞争CPU的范围，也就是说线程优先级的有效范围。POSIX的标准中定义了两个值：PTHREAD_SCOPE_SYSTEM和PTHREAD_SCOPE_PROCESS，前者表示与系统中所有线程一起竞争CPU时间，后者表示仅与同进程中的线程竞争CPU。目前LinuxThreads仅实现了PTHREAD_SCOPE_SYSTEM一值。
函数pthread_attr_setscope和pthread_attr_getscope分别用来设置和得到线程的作用域。
```c
#include <pthread.h>   
int    pthread_attr_getscope( const pthread_attr_t * attr, int * scope );
int pthread_attr_setscope( pthread_attr_t*, int scope );
```
参数：
attr 线程属性变量
scope 线程的作用域
若成功返回0，若失败返回-1。

作用域控制线程是否在进程内或在系统级上竞争资源，可能的值是
PTHREAD_SCOPE_PROCESS（进程内竞争资源）
PTHREAD_SCOPE_SYSTEM （系统级竞争资源）。

为了设置这些属性，POSIX定义了一系列属性设置函数，包括pthread_attr_init()、pthread_attr_destroy()和与各个属性相关的pthread_attr_getXXX/pthread_attr_setXXX函数。

# 主要的函数如下：
## 1、pthread_attr_init
功能： 对线程属性变量的初始化。
头文件： <pthread.h>
函数原型： int pthread_attr_init (pthread_attr_t* attr);
函数传入值：attr:线程属性。
函数返回值：成功： 0
失败： -1
## 2、pthread_attr_setscope
功能： 设置线程 __scope 属性。scope属性表示线程间竞争CPU的范围，也就是说线程优先级的有效范围。POSIX的标准中定义了两个值：PTHREAD_SCOPE_SYSTEM和PTHREAD_SCOPE_PROCESS，前者表示与系统中所有线程一起竞争CPU时间，后者表示仅与同进程中的线程竞争CPU。默认为PTHREAD_SCOPE_PROCESS。目前LinuxThreads仅实现了PTHREAD_SCOPE_SYSTEM一值。
头文件： <pthread.h>
函数原型： int pthread_attr_setscope (pthread_attr_t* attr, int scope);
函数传入值：attr: 线程属性。
scope:PTHREAD_SCOPE_SYSTEM，表示与系统中所有线程一起竞争CPU时间，
PTHREAD_SCOPE_PROCESS，表示仅与同进程中的线程竞争CPU
函数返回值得：同1。
## 3、pthread_attr_setdetachstate
功能： 设置线程detachstate属性。该表示新线程是否与进程中其他线程脱离同步，如果设置为PTHREAD_CREATE_DETACHED则新线程不能用pthread_join()来同步，且在退出时自行释放所占用的资源。缺省为PTHREAD_CREATE_JOINABLE状态。这个属性也可以在线程创建并运行以后用pthread_detach()来设置，而一旦设置为PTHREAD_CREATE_DETACH状态（不论是创建时设置还是运行时设置）则不能再恢复到PTHREAD_CREATE_JOINABLE状态。
头文件： <phread.h>
函数原型： int pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate);
函数传入值：attr:线程属性。
detachstate:PTHREAD_CREATE_DETACHED，不能用pthread_join()来同步，且在退出时自行释放所占用的资源
PTHREAD_CREATE_JOINABLE，能用pthread_join()来同步
函数返回值得：同1。
## 4、pthread_attr_setschedparam
功能： 设置线程schedparam属性，即调用的优先级。
头文件： <pthread.h>
函数原型： int pthread_attr_setschedparam (pthread_attr_t* attr, struct sched_param* param);
函数传入值：attr：线程属性。
param：线程优先级。一个struct sched_param结构，目前仅有一个sched_priority整型变量表示线程的运行优先级。这个参数仅当调度策略为实时（即SCHED_RR或SCHED_FIFO）时才有效，并可以在运行时通过pthread_setschedparam()函数来改变，缺省为0
函数返回值：同1。
## 5、pthread_attr_getschedparam
功能： 得到线程优先级。
头文件： <pthread.h>
函数原型： int pthread_attr_getschedparam (pthread_attr_t* attr, struct sched_param* param);
函数传入值：attr：线程属性；
param：线程优先级；
函数返回值：同1。
## 6、pthread_attr_setstacksize
功能： 设置堆栈大小。
头文件： <pthread.h>
函数原型：int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
函数传入值：attr：线程属性；
stacksize：设置的堆栈大小；
函数返回值：同1。
备注：pthread_create 创建线程时，若不指定分配堆栈大小，系统会分配默认值，查看默认值方法如下：
```
# ulimit -s
8192
#
```
上述表示为8M；单位为KB。

也可以通过# ulimit -a 其中 stack size 项也表示堆栈大小。ulimit -s  value 用来重新设置stack 大小。

一般来说 默认堆栈大小为 8388608; 堆栈最小为 16384 。 单位为字节。

堆栈最小值定义为 PTHREAD_STACK_MIN ，包含#include <limits.h>后可以通过打印其值查看。对于默认值可以通过pthread_attr_getstacksize (&attr, &stack_size); 打印stack_size来查看。

尤其在嵌入式中内存不是很大，若采用默认值的话，会导致出现问题，若内存不足，则 pthread_create 会返回 12，定义如下：
```c
#define EAGAIN 11 
#define ENOMEM 12 /* Out of memory */
```

# 线程属性程序示例：
```c
pthread_t thd;
void *thread(void *arg);

int main()
{
	pthread_attr_t attr={0};
	pthread_attr_init(&attr);
	struct sched_param sched={99};
	pthread_attr_setschedpolicy(&attr,SCHED_RR);
	pthread_attr_setschedparam(&attr,&sched);
	if(pthread_create(&thd,&attr,thread,NULL)!=0)
	{
		pthread_attr_destroy(&attr);
		printf("create thread error\n");
		return;
	}
}
```