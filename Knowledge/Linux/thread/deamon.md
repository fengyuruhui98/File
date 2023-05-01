# 1.定义
- 在linux系统中，我们会发现在系统启动的时候有很多的进程就已经开始跑了，也称为服务，这也是我们所说的守护进程。守护进程是脱离于终端并且在后台运行的进程，脱离终端是为了避免在执行的过程中的信息在终端上显示，并且进程也不会被任何终端所产生的终端信息所打断。守护进程一般的生命周期是系统启动到系统停止运行，当人，什么都敌不多人，也可以通过杀死进程的方式来结束进程的生命周期。linux系统中有很多的守护进程，最典型的就是我们经常看到的服务进程。当然，我们也经常会利用守护进程来完成很多的系统或者自动化任务。
- 守护进程是运行在后台的一种特殊进程，它独立于控制终端并且周期性地执行某种任务或循环等待处理某些事件的发生；它不需要用户输入就能运行而且提供某种服务，不是对整个系统就是对某个用户程序提供服务。Linux系统的大多数服务器就是通过守护进程实现的。
- 守护进程一般在系统启动时开始运行，除非强行终止，否则直到系统关机才随之一起停止运行；
- 守护进程一般都以root用户权限运行，因为要使用某些特殊的端口（1-1024）或者资源；
- 守护进程的父进程一般都是init进程，因为它真正的父进程在fork出守护进程后就直接退出了，所以守护进程都是孤儿进程，由init接管；
- 守护进程是非交互式程序，没有控制终端，所以任何输出，无论是向标准输出设备stdout还是标准出错设备stderr的输出都需要特殊处理。
- 守护进程的名称通常以d结尾，比如sshd、xinetd、crond等

# 2.作用
1. 守护进程是一个生存周期较长的进程，通常独立于控制终端并且周期性的执行某种任务或者等待处理某些待发生的事件
2. 大多数服务都是通过守护进程实现的
3. 关闭终端，相应的进程都会被关闭，而守护进程却能够突破这种限制

Linux系统的大多数服务器就是通过守护进程实现的。常见的守护进程包括：
- 系统日志进程syslogd、
- web服务器httpd、
- 邮件服务器sendmail
- 数据库服务器mysqld等。

# 3.创建守护进程的步骤：
## 背景

>进程组：一个或多个**进程**的集合  
>1. 每个进程也属于一个进程组  
>2. 每个进程主都有一个进程组号，该号等于该进程组组长的PID号**且进程组号不会随组长的退出而更改** 
>3. 一个进程只能为它自己或子进程设置进程组ID号

>会话期(session)：是一个或多个**进程组**的集合。比如用户从登陆到退出，这个期间用户运行的所有进程都属于该会话周期

>setsid()函数：建立一个新会话，并担任该会话组的组长。  
>1. 如果，调用setsid的进程不是一个进程组的组长，此函数创建一个新的会话期。  
>2. 此进程变成该对话期的首进程  
>3. 此进程变成一个新进程组的组长进程。  
>4. 此进程没有控制终端，如果在调用setsid前，该进程有控制终端，那么与该终端的联系被解除。 如果该进程是一个进程组的组长，此函数返回错误。  
>5. 为了保证这一点，我们先调用fork()然后exit()，此时只有子进程在运行
>6. 目的：其实就是起到了进程脱离原会话的控制，摆脱了原进程组的控制，摆脱了原控制终端的控制。主要是因为前面创建子进程的时候，随人父进程退出了，但是子进程拷贝了父进程的会话、进程组、控制终端等等，所以需要使用setsid来摆脱控制。如果，调用setsid的进程不是一个进程组的组长，此函数创建一个新的会话期。


## **（1）fork()创建子进程，exit()父进程退出**

因为守护进程是脱离终端控制的，所以要造成一种在终端里已经运行完的假象，把所有的工作都放在子进程中去完成。用户在Shell终端里则可以执行其他命令，从而在形式上做到了与控制终端的脱离，在后台工作。我们知道，父进程退出后，子进程其实就是变成了孤儿进程，孤儿进程一般是由1号进程收养，也就是我们所谓的init进程，也就是说原来的子进程变成了init的子进程。

## **（2）创建会话**
在调用了fork()函数后，子进程全盘拷贝了父进程的会话期、进程组、控制终端等，虽然父进程退出了，但会话期、进程组、控制终端等并没有改变，因此，这还不是真正意义上的独立开来，而 **setsid() 函数能够使进程完全独立出来。**

## **（3）再次 fork() 一个孙进程并让子进程退出**

为什么要再次fork呢，假定有这样一种情况，之前的父进程fork出子进程以后还有别的事情要做，在做事情的过程中因为某种原因阻塞了，而此时的子进程因为某些非正常原因要退出的话，就会形成僵尸进程，所以由子进程fork出一个孙进程以后立即退出，孙进程作为守护进程会被init接管，此时无论父进程想做什么都随它了。

## **（4）孙进程调用chdir()函数,让当前目录改变为根目录 ”/” 做为孙进程的工作目录**

这一步也是必要的步骤，使用fork创建的子进程继承了父进程的当前工作目录。由于在进程运行中，当前目录所在的文件系统（如“/mnt/usb”）是不能卸载的，这对以后的使用会造成诸多的麻烦（比如系统由于某种原因要进入单用户模式）。因此，通常的做法是让"/"作为守护进程的当前工作目录，这样就可以避免上述的问题，当然，如有特殊需要，也可以把当前工作目录换成其他的路径，如/tmp，改变工作目录的常见函数是chdir。

在fork子进程的时候，子进程也继承了父进程的工作目录。通常是让‘/’目录作为守护进程的当前目录，可以避免很多麻烦。

## **（5）孙进程中调用umask()函数，设置进程的文件权限掩码为0**

同一个文件的权限掩码是一样的，我们使用fork创建的子进程会从父进程那里继承一些已经打开的文件，打开的文件可能永远不会被守护进程读写，但是他们一样消耗资源，而且可能导致所在的文件系统无法结束。

文件权限掩码是指屏蔽掉文件权限中的对应位。比如，有个文件权限掩码是050，它就屏蔽了文件组拥有者的可读与可执行权限。由于使用fork函数新建的子进程继承了父进程的文件权限掩码，这就给该子进程使用文件带来了诸多的麻烦。因此，把文件权限掩码设置为0，可以大大增强该守护进程的灵活性。设置文件权限掩码的函数是umask。在这里，通常的使用方法为umask(0)。

## **（6）在孙进程中关闭任何不需要的文件描述符**
同文件权限码一样，用fork函数新建的子进程会从父进程那里继承一些已经打开了的文件。这些被打开的文件可能永远不会被守护进程读写，但它们一样消耗系统资源，而且可能导致所在的文件系统无法卸下。

（可先通过函数sysconf（）来获取系统最大的文件打开数）

在上面的第（2）步之后，守护进程已经与所属的控制终端失去了联系。因此从终端输入的字符不可能达到守护进程，守护进程中用常规方法（如printf）输出的字符也不可能在终端上显示出来。所以，文件描述符为0、1和2 的3个文件（常说的输入、输出和报错）已经失去了存在的价值，也应被关闭。或者将0,1,2定位到/dev/null（将标准输入，标准输出，标准错误弄到回收站）

## **（7）守护进程退出处理**
当用户需要外部停止守护进程运行时，往往会使用 kill 命令停止该守护进程。所以，**守护进程中需要编码来实现 kill 发出的signal信号处理，达到进程的正常退出。**

# 4. 守护进程实例：
## 示例1
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXFILE 65535

int main(){
    int i,fd;
    pid_t pid;
    char *buf = "hello,dameon";
    int len = strlen(buf);

    //第一步，创建子进程
    pid = fork();
    if(pid < 0){
        printf("error\n");
        exit(1);//异常退出
    }
    else if(pid > 0){
        exit(0);
    }

    //第二步，创建会话
    setsid();

    //第三步，修改根目录
    chdir("/");

    //第四步，设置掩码
    umask(0);
    for(i=0;i<MAXFILE;i++){
        close(i);
    }

    while(1){
        fd = open("/mnt/hgfs/H/C/dameon.log",O_CREAT|O_WRONLY,0600);
        if(fd < 0){
            perror("open");
            exit(1);
        }
        write(fd,buf,len+1);
        close(fd);
    }
    return 0;
}
```
我们执行ps -ef | grep dameon
![image1][image1]

## 示例2
```c
//守护进程代码实现
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

static bool flag = true;
void create_daemon();
void handler(int);

int main()
{
    time_t t;
    int fd;
    create_daemon();
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sigaction(SIGQUIT, &act, NULL))
    {
        printf("sigaction error.\n");
        exit(0);
    }
    while(flag)
    {
        fd = open("/home/mick/daemon.log", 
        O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd == -1)
        {
            printf("open error\n");
        }
        t = time(0);
        char *buf = asctime(localtime(&t));
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(60);
    }
    return 0;
}

void handler(int sig)
{
    printf("I got a signal %d\nI'm quitting.\n", sig);
    flag = false;
}

void create_daemon()
{
    pid_t pid;
    pid = fork();

    if(pid == -1)
    {
        printf("fork error\n");
        exit(1);
    }
    else if(pid)
    {
        exit(0);
    }

    if(-1 == setsid())
    {
        printf("setsid error\n");
        exit(1);
    }

    pid = fork();
    if(pid == -1)
    {
        printf("fork error\n");
        exit(1);
    }
    else if(pid)
    {
        exit(0);
    }

    chdir("/");
    int i;
    for(i = 0; i < 3; ++i)
    {
        close(i);
    }
    umask(0);
    return;
}
```
```c
//用系统函数实现deamon：守护进程的代码
#include <unistd.h>

int daemon(int nochdir, int noclose);
/*-------------------------------------------------------------------
功能：创建一个守护进程

参数：
nochdir：=0将当前目录更改至“/”
noclose：=0将标准输入、标准输出、标准错误重定向至“/dev/null”

返回值：
成功：0
失败：-1
--------------------------------------------------------------------*/
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

static bool flag = true;
void handler(int);

int main()
{
    time_t t;
    int fd;
    if(-1 == daemon(0, 0))
    {
        printf("daemon error\n");
        exit(1);
    }
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sigaction(SIGQUIT, &act, NULL))
    {
        printf("sigaction error.\n");
        exit(0);
    }
    while(flag)
    {
        fd = open("/home/mick/daemon.log", 
        O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd == -1)
        {
            printf("open error\n");
        }
        t = time(0);
        char *buf = asctime(localtime(&t));
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(60);
    }
    return 0;
}
void handler(int sig)
{
    printf("I got a signal %d\nI'm quitting.\n", sig);
    flag = false;
}
```
```c
//守护进程每100s向文件写一句话
#include <iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
using namespace std;

#define max_file 65535

int main()
{
    pid_t pc;
    int fd,len;

    char buf[]="this is a demo\n";
    len=strlen(buf);

    pc=fork();//第一步，创建子进程

    if(pc<0)
    {
        cout<<"fork error"<<endl;
        exit(1);
    }
    else if(pc>0)
    {
        exit(0);//第二步，父进程退出
    }
    else
    {
        setsid();//第三步，创建新会话

        chdir("/");//第四步，将当前目录改为根目录

        umask(0);//第五步，重新设置文件权限掩码

        for(int i=0; i<max_file; i++)
        {
            close(i);//第六步，关闭不需要的文件描述符
        }

        while(1)
        {
            if((fd=open("/tmp/dameo.log",O_CREAT|O_WRONLY|O_APPEND,0600))<0)
            {

                cout<<"open erro"<<endl;
                exit(1);
            }
            write(fd,buf,len+1);

            close(fd);

            sleep(100);//每隔100s输出一句话到文件
        }
    }
}
```
## 示例3
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void create_daemon(void)//将一般进程变为守护进程
{
 pid_t pid = 0;
 pid = fork();//创建子进程
 if(pid<0)
 {
  perror("fork");
  exit(-1);
 }
 else if (pid > 0)
 {
  printf("this is parent progress");
  exit(-1);
 }
 else if (pid == 0)
 {
 pid = setsid();//子进程脱离控制台创建新的会话期
 if(pid < 0)
  {
   perror("setsid");
  }
 chdir("/");//脱离文件系统
 umask(0);//用umas函数取消任何文件的权限屏蔽
 int i = sysconf(_SC_OPEN_MAX);//得到系统最大的文件描述符个数
 for (;i>0;i--)//关闭所有文件描述符
 {
  close(i);
 }
 open("/dev/null",O_RDWR);//将0,1,2定位到/dev/null
 open("/dev/null",O_RDWR);
 open("/dev/null",O_RDWR);
}
}
int main(void)/*change a progress to a daemon*/
{
 create_daemon();
 while(1)
  printf("ok\n");
  sleep(1);
 }
 return 0;
}
```




































[image1]:data:image/png;base64,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