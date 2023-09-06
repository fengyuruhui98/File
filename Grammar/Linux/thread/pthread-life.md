# Linux线程
# 1、简单了解一下线程
 线程也被称为轻量级进程，启动一个线程所花费的空间远远小于启动一个进程所花费的空间，因为进程切换时需要**更新cache和tlb**，而线程就不用。由于多个线程访问的都是同一地址空间，所以**线程间通信可以通过全局变量**，但最好需要需要加入同步或互斥等机制来确保数据访问的安全性，本篇文章探讨线程的创建、退出、回收、取消、分离等函数，此外，**pthread并非Linux系统的默认库，而是POSIX线程库，所以编译时需要加上-pthread。**


# 2、线程创建：pthread_create
```c
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
				   void *(*start_routine) (void *), void *arg);
```
参数解析

参数|说明
-|-
thread|传出参数，用于获得线程的ID
attr|设置线程的属性，通常为默认属性NULL
start_routine|函数指针，线程开始执行的函数
arg|传递给线程的参数

看下例子就明白了：
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct test{
	int i;
};

void *mypthreadFunction1(void *pvoid)
{
	int i = 0;
	
	while(1)
	{
		printf("thread function 1, i: %d\n", i++);
		sleep(1);
	}
}

void *mypthreadFunction2(void *pvoid)
{
	int i = *((int*)pvoid); // 只是把这个地址的值读出来，后面操作并没有改变该地址上的值
	
	while(1)
	{
		printf("thread function 2, i: %d\n", i++);
		sleep(1);
	}
}

void *mypthreadFunction3(void *pvoid)
{
	int j = (int)pvoid;
	
	while(1)
	{
		printf("thread function 3, j: %d\n", j++);
		sleep(1);
	}
}

void *mypthreadFunction4(void *pvoid)
{
	struct test *pt = (struct test *)pvoid;
	
	while(1)
	{
		printf("thread function 4, i: %d\n", (pt->i)++);
		sleep(1);
	}
}

int main()
{
	int i = 10;
	int j = 100;
	struct test *pt = malloc(sizeof(struct test));
	pt->i = 1000;
	pthread_t tid[4];

	pthread_create(&tid[0], NULL, mypthreadFunction1, NULL);
	pthread_create(&tid[1], NULL, mypthreadFunction2, (void *)&i);// 传变量的地址（危险！！！见下方说明）
	pthread_create(&tid[2], NULL, mypthreadFunction3, (void *)j); // 传变量的值
    pthread_create(&tid[3], NULL, mypthreadFunction4, (void *)pt);// 传结构体地址，还可以传函数地址等

	while(1)
	{
		sleep(1);
	}

    free(pt);

	return 0;
}
```
gcc main.c -pthread -m32编译之后达到a.out，程序运行输出结果：
```c
thread function 2, i: 10
thread function 1, i: 0
thread function 3, j: 100
thread function 4, i: 1000
thread function 1, i: 1
thread function 4, i: 1001
thread function 3, j: 101
thread function 2, i: 11
thread function 2, i: 12
thread function 4, i: 1002
thread function 1, i: 2
thread function 3, j: 102
thread function 1, i: 3
thread function 3, j: 103
thread function 4, i: 1003
thread function 2, i: 13
thread function 2, i: 14
thread function 3, j: 104
thread function 4, i: 1004
thread function 1, i: 4
thread function 4, i: 1005
thread function 1, i: 5
thread function 3, j: 105
thread function 2, i: 15
^C
```
可以看到，线程不是严格按顺序打印的，并且每次运行的打印顺序都不一定相同，是因为线程之间存在资源竞争调度等，不过可以使用线程同步实现顺序输出。


# 3、线程传参注意事项
  《UNIX环境高级编程》第三版中说到线程：**一个进程的所有信息对该进程的所有线程都是共享的，包括可执行程序的代码、程序的全局内存和堆内存、栈以及文件描述符。**现在我们回到上面创建第2个线程的这里，由于前面说**栈是共享的，但栈帧是独立的**呀，也就是说函数之间不能直接访问内部的变量，所以我们需要把main函数里面的局部自动变量 i 的地址作为线程2的参数传递进去，线程2再读出该地址上的值。上面示例中只是读取它的值就没有使用其他操作了，读取它的值倒不会造成严重的影响，问题就在于如果我们在线程2使用它的地址进行一些操作又会是怎样的呢？看以下例子：
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void *mypthreadFunction(void *pvoid)
{
        int *i = (int*)pvoid;  // 危险！！！线程里用指针指向main函数里变量的地址，后续改写操作会影响到main函数该地址上的值
        int j = 0;

        // 此时i指向的main函数的栈帧
        printf("thread i addr: %p, val: %d\n", i, *i);

        while(j < 10)
        {
                printf("thread function, i: %d\n", (*i)++);
                j++;
                sleep(1);
        }
}

int main()
{
        int i = 10;
        int j = 0;
        pthread_t tid;

        printf("-main- i addr: %p, val: %d\n", &i, i);
        pthread_create(&tid, NULL, mypthreadFunction, (void *)&i); // 传局部自动变量地址

        while(j < 6)
        {
                printf("-main- function, i: %d\n", i);
                j++;
                sleep(1);
        }
        printf("-main- function exit!\n");
        pthread_exit(NULL);
}

```
输出结果：
```c
-main- i addr: 0x7fff81066ab8, val: 10
-main- function, i: 10
thread i addr: 0x7fff81066ab8, val: 10
thread function, i: 10
-main- function, i: 11
thread function, i: 11
-main- function, i: 12
thread function, i: 12
-main- function, i: 13
thread function, i: 13
-main- function, i: 14
thread function, i: 14
-main- function, i: 15
thread function, i: 15
-main- function exit!
thread function, i: 16
thread function, i: 0
thread function, i: 1
thread function, i: 2
```
  可以看到，线程函数里面可以操作到main函数的栈帧喔。需要知道线程函数和main函数不是调用关系，他们是同级别的线程函数。把栈地址传递是很危险的，可以看到上面的例子，线程里面改变了 i 的值，main函数里面也跟着改变，而main函数结束之后 i 被释放了（值变为0），而线程还在操作刚才的地址，而且如果下一次其他函数分配的变量又是在这个地址呢？是不是很危险、很难把握这个地址的值什么时候被改变了？这种情况可以概括为：**在C/C++中，使用指针能够做到把线程A的栈地址传递给线程B去操作。这种做法其实是很危险的，给线程传参的时候需要考虑到栈内存的特殊性。**


# 4、线程退出：pthread_exit
```c
void pthread_exit(void *value_ptr);
```
  线程内部调用该函数用于退出当前线程，参数value_ptr是用于存储线程的退出值传递出去，也可以设为NULL，调用接下来的pthread_join回收线程就可以传出到参数void **value_ptr中。

## 区分

函数/关键字|作用
continue|退出本次循环进入下一轮循环
break|退出该层循环
exit|退出当前进程
return|结束函数，返回到调用者
pthread_exit|退出当前线程

# 5、线程回收：pthread_join
```c
int pthread_join(pthread_t thread, void **value_ptr);
```
  阻塞等待id为thread的线程退出，并且可以通过传出参数value_ptr获取线程退出状态，如果不需要查看线程退出状态也可以设为NULL，可以类比于进程中的wait/waitpid()函数，这里用于回收线程资源。

结合线程退出与回收查看例子：
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *mypthreadFunction1(void *pvoid)
{
	pthread_exit("mypthreadFunction1 exit!");
}

void *mypthreadFunction2(void *pvoid)
{
	static int i = 0;

	while(1)
	{
		printf("thread function 2, i: %d\n", i++);
		sleep(1);
		if(i == 2)
		{
			pthread_exit((void*)&i);
		}
	}
}

int main()
{
	int *reVal;
	char *str;
	pthread_t tid1;
	pthread_t tid2;

	pthread_create(&tid1, NULL, mypthreadFunction1, NULL);
	pthread_create(&tid2, NULL, mypthreadFunction2, NULL);

	pthread_join(tid1, (void **)&str);
	printf("thread1 return a str: %s\n", str);

	pthread_join(tid2, (void **)&reVal);
	printf("thread2 return a value: %d\n",  *reVal);

	return 0;
}
```
输出结果：
```c
thread function 2, i: 0
thread1 return a str: mypthreadFunction1 exit!
thread function 2, i: 1
thread2 return a value: 2
```

# 6、线程分离：pthread_detach
```c
int pthread_detach(pthread_t thread);
```

  POSIX线程的一个特点是：除非线程是被分离了的，否则在线程退出时，它的资源是不会被释放的，需要调用pthread_join回收（注意：进程结束后，占用的资源全部会被释放的）。

  调用该函数分离线程，子线程主动与主线程断开关系，线程结束后，其退出状态不能由其他线程获取，而自己自动释放资源，多用于网络、多线程服务器。

  返回值为0成功或非零错误码。


# 7、线程取消：pthread_cancel
```c
int pthread_cancel(pthread_t thread);
```
  参数 thread 是要取消的目标线程的线程 ID。该函数并不阻塞调用线程，它发出取消请求后就返回了。类比于进程的kill函数，但是不同的是，调用函数时，并非一定能取消掉该线程，因为这个函数需要线程进到内核时才会被杀掉，所以线程如果一直运行于用户空间，就没有契机进入内核也就无法取消（例while(1){}空循环），此时可以使用pthread_testcancel();进入内核。

  如果成功，pthread_cancel 返回 0，如果不成功，pthread_cancel 返回一个非零的错误码。

  如果线程处于PTHREAD_CANCEL_ENABLE 状态，它就接受取消请求，如果线程处于 PTHREAD_CANCEL_DISABLE状态，取消请求就会被保持在挂起状态。默认情况下，线程处于 PTHREAD_CANCEL_ENABLE状态。

结合线程分离和取消查看例子：
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


void *mypthreadFunction(void *pvoid)
{
	int i = 0;

	while(1)
	{
		printf("thread function, i: %d\n", i++);
		sleep(1);
	}
}


int main()
{
	pthread_t tid;

	pthread_create(&tid, NULL, mypthreadFunction, NULL);
	pthread_detach(tid);	// 将线程分离

	// 2秒钟之后取消线程
	sleep(2);
	pthread_cancel(tid);
	printf("had cancel the thread!\n");

	return 0;
}
```
运行结果：
```c
thread function, i: 0
thread function, i: 1
had cancel the thread!
```

# 8、线程其他函数
## 8.1 获得线程自身ID
```c
pthread_t pthread_self(void);
```
## 8.2 线程属性初始化与关闭

```c
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
```
  函数 pthread_attr_init 用默认值对一个线程属性对象进行初始化。
  函数pthread_attr_destroy函数将属性对象的值设为无效的。被设为无效的属性对象可以再次被初始化为一个新的属性对象。
  pthread_attr_init 和 pthread_attr_destroy 都只有一个参数，即一个指向属性对象的指针。

## 8.3 线程属性状态
```c
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```
  线程状态的可能取值为PTHREAD_CREATE_JOINABLE和PTHREAD_CREATE_DETACHED 。pthread_attr_getdetachstate函数用来查看一个属性对象中的线程状态。而pthread_attr_setdetachstate函数用来设置一个属性对象中的线程状态。

## 8.4 线程id比较
```c
int pthread_equal(pthread_t t1, pthread_t t2);
```
  两个参数 t1 和 t2 是两个线程 ID，如果它们相等，pthread_equal 就返回一个非零值，如果不相等，则返回 0。

## 8.5 改变线程取消状态
```c
int pthread_setcancelstate(int state, int *oldstate);
```
  参数 state 表示要设置的新状态，参数 oldstate 为一个指向整形的指针，用于保存线程以前的状态。如果成功，该函数返回 0，如果不成功，它返回一个非 0 的错误码。

## 8.6 改变线程取消类型
```c
int pthread_setcanceltype(int type, int *oldtype);
```
  当线程将退出作为对取消请求的响应时，取消类型允许线程控制它在什么地方退出。当它的取消类型为 PTHREAD_CANCEL_ASYNCHRONOUS 时，线程在任何时候都可以响应取消请求。当它的取消类型为 PTHREAD_CANCEL_DEFERRED 时，线程只能在特定的几个取消点上响应取消请求。在默认情况下，线程的类型为 PTHREAD_CANCEL_DEFERRED。

  参数 type 指定线程的取消类型，参数 oldtype 用来指定保存原来的取消类型的地址。如果成功，该函数返回 0，如果不成功，它返回一个非 0 的错误码。

  线程还有很多其他函数，这里不一一列举。


# 9、线程注意事项
其他线程保留，主线程退出用pthread_exit；
避免僵尸线程考虑使用：
  pthread_join
  pthread_detach
  pthread_create指定分离属性；
malloc和mmap的内存其他线程可以使用；
应多避免多线程调用fork，除非马上exec，子进程中只有调用fork的线程存在，其他线程都已退出；
信号的复杂语义很难和多线程共存，应避免多线程引入信号机制，因为信号来了哪个线程先抢到就是它处理，各线程有独立的mask屏蔽字，处理方式是共享的，很难指定哪个线程处理。
