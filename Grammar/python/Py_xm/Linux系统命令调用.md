# Python调用执行Linux系统命令
Python作为一门脚本语言，胶水语言，自然是有它的胶水特性的，就比如Python和Linux系统的结合，比如，在使用Python编写自动化运维脚本的时候，总免不了需要和Linux系统交互，如，查询Linux系统内的某个文件，然后修改这个文件并运行它（假设有可运行权限）。

Python3中常用的执行操作系统命令有os.system()、os.popen()、subprocess.popen()、subprocess.call()、subprocess.run()、subprocess.getstatusoutput()六种方法。

Python让人称道的其中一个地方就是它的类库非常齐全，这是真的不吹牛，只有你想不到，基本没有它没有的库，比如科学计算库numpy，pandas这些，或者与系统交互的库，os库，或者自动化测试的selenium库等等，到底Python有多少库我想可能Python创始人也可能不知道。（Python的社区太强大了！！！！！）那么，如果Python需要调用Linux系统命令，比如，ls，df，free，ps，这样的命令自然是需要与系统交互的专用库os结合了，还一种方法就是直接操纵进程的啦，那么，自然就是subprocess库啦。也就是说，我们想要Python能够调用Linux系统命令，也就是Python与Linux系统交互，要么需要使用os库，要么需要使用subprocess库啦还有一个不太常用的commands库。

Python的os模块，见名之意，opera system的意思是毫无疑问了。该模块是Python的内建模块，也可以说是内建库了，该模块内的函数自然也是内建函数了，我们要和系统命令交互，需要使用它的system（），popen（）这两个函数，或者使用commands模块，或者使用subprocess模块，前面两个函数和commands模块是比较陈旧的，现在官方比较推荐也可以说强烈推荐的是subprocess模块。

- [Python调用执行Linux系统命令](#python调用执行linux系统命令)
  - [1.os.system（系统层面的直接调用）](#1ossystem系统层面的直接调用)
  - [2.os.popen（command mode 新开线程方式）](#2ospopencommand-mode-新开线程方式)
  - [3. 使用模块 subprocess（Python文档中目前全力推荐）](#3-使用模块-subprocesspython文档中目前全力推荐)
    - [3.1 subprocess.Popen()](#31-subprocesspopen)
    - [3.2 subprocess.call()](#32-subprocesscall)
    - [3.3 subprocess.run()](#33-subprocessrun)
    - [3.4 subprocess.getstatusoutput()](#34-subprocessgetstatusoutput)


## 1.os.system（系统层面的直接调用）
这个调用相当直接，且是同步进行的，程序需要阻塞并等待返回。返回值是依赖于系统的，直接返回系统的调用返回值，所以windows和linux是不一样的。

仅仅在一个子终端运行系统命令，而不能获取命令执行后的返回信息

system函数可以将字符串转化成命令在服务器上运行；其原理是每一条system函数执行时，其会创建一个子进程在系统上执行命令行，子进程的执行结果无法影响主进程。

其返回值是指令运行后返回的状态码，0表示指令成功执行，1表示失败，256表示没有返回结果，该方法适用于系统命令不需要输出内容的场景。

用法：
```py
os.system("command")
```
示例：
```py
import os
a=os.system("ping 192.168.1.101")  #使用a接收返回值
print(a)
```
输出：
```
正在 Ping 192.168.1.101 具有 32 字节的数据:
来自 192.168.1.101 的回复: 字节=32 时间=8ms TTL=127
来自 192.168.1.101 的回复: 字节=32 时间=2ms TTL=127
来自 192.168.1.101 的回复: 字节=32 时间=2ms TTL=127
来自 192.168.1.101 的回复: 字节=32 时间=2ms TTL=127

192.168.1.101 的 Ping 统计信息:
    数据包: 已发送 = 4，已接收 = 4，丢失 = 0 (0% 丢失)，
往返行程的估计时间(以毫秒为单位):
    最短 = 2ms，最长 = 8ms，平均 = 3ms
0
Process finished with exit code 0
```
也可以在linux系统下运行
```py
import os
os.system('cd /home && mkdir test') 执行多条命令可以使用&&连接
```
## 2.os.popen（command mode 新开线程方式）
可以看出，popen方法通过p.read()获取终端输出，而且popen需要关闭close().当执行成功时，close()不返回任何值，失败时，close()返回系统返回值. 可见它获取返回值的方式和os.system不同。

该方法不但执行命令还返回执行后的信息对象，好处在于：将返回的结果赋于一变量，便于程序的处理。
这种调用方式是通过管道的方式来实现，这个函数的返回值是一个文件对象，可以读或者写(由mode决定，mode默认是’r’)。如果mode为’r’，调用该对象的read()或readlines()方法可以读取输出内容。

用法：os.popen(command[, mode[, bufsize]])

os.system(cmd)或os.popen(cmd)，前者返回值是脚本的退出状态码，后者的返回值是脚本执行过程中的输出内容。
```py
import os
a=os.popen("ipconfig")
print(a.read())
输出：
以太网适配器 以太网:

   连接特定的 DNS 后缀 . . . . . . . : 
   本地链接 IPv6 地址. . . . . . . . : fe81::b0ed:2b1b:7385:97d1%8
   IPv4 地址 . . . . . . . . . . . . : 192.168.1.100
   子网掩码  . . . . . . . . . . . . : 255.255.255.0
   默认网关. . . . . . . . . . . . . : 192.168.1.1
```

## 3. 使用模块 subprocess（Python文档中目前全力推荐）

直接调用命令，返回值即是系统返回。shell=True表示命令最终在shell中运行。Python文档中出于安全考虑，不建议使用shell=True。

subprocess模块是python从2.4版本开始引入的模块，主要用来取代 一些旧的模块方法，如os.system、os.spawn、os.popen、commands.*等。官方推荐使用该模块执行系统命令，subprocess模块通过子进程来执行外部指令，并通过input/output/error管道，获取子进程的执行的返回信息。

好处在于:运用对线程的控制和监控，将返回的结果赋于一变量，便于程序的处理。

```py
import os
import commands
print('=============================ls')
os.system('ls -al /')
print('===========df')
df = os.popen('df -ah').read()
print(df)
print('=========================free')
a = commands.getoutput("free -mh")
print(a)
```
输出结果为：
```
=============================ls
total 28
dr-xr-xr-x.  17 root root  244 Jan 24 12:11 .
dr-xr-xr-x.  17 root root  244 Jan 24 12:11 ..
-rw-r--r--    1 root root    0 Jan 24 12:11 .autorelabel
lrwxrwxrwx.   1 root root    7 Jan 24 11:12 bin -> usr/bin
dr-xr-xr-x.   5 root root 4096 Jan 24 12:11 boot
drwxr-xr-x   19 root root 3320 Feb  1 09:29 dev
drwxr-xr-x. 138 root root 8192 Feb  1 09:29 etc
drwxr-xr-x.   2 root root    6 Nov  5  2016 home
lrwxrwxrwx.   1 root root    7 Jan 24 11:12 lib -> usr/lib
lrwxrwxrwx.   1 root root    9 Jan 24 11:12 lib64 -> usr/lib64
drwxr-xr-x.   2 root root    6 Nov  5  2016 media
drwxr-xr-x.   3 root root   19 Jan 24 11:34 mnt
drwxr-xr-x.   4 root root   34 Jan 24 11:58 opt
dr-xr-xr-x  229 root root    0 Feb  1 09:29 proc
dr-xr-x---.  11 root root 4096 Feb  1 09:30 root
drwxr-xr-x   39 root root 1200 Feb  1 09:29 run
lrwxrwxrwx.   1 root root    8 Jan 24 11:12 sbin -> usr/sbin
drwxr-xr-x.   2 root root    6 Nov  5  2016 srv
dr-xr-xr-x   13 root root    0 Feb  1 09:29 sys
drwxrwxrwt.  21 root root 4096 Feb  1 09:46 tmp
drwxr-xr-x.  13 root root  155 Jan 24 11:12 usr
drwxr-xr-x.  21 root root 4096 Jan 24 11:44 var
===========df
Filesystem      Size  Used Avail Use% Mounted on
rootfs             -     -     -    - /
sysfs              0     0     0    - /sys
proc               0     0     0    - /proc
devtmpfs        1.9G     0  1.9G   0% /dev
securityfs         0     0     0    - /sys/kernel/security
tmpfs           1.9G     0  1.9G   0% /dev/shm
devpts             0     0     0    - /dev/pts
tmpfs           1.9G  9.0M  1.9G   1% /run
tmpfs           1.9G     0  1.9G   0% /sys/fs/cgroup
cgroup             0     0     0    - /sys/fs/cgroup/systemd
pstore             0     0     0    - /sys/fs/pstore
cgroup             0     0     0    - /sys/fs/cgroup/pids
cgroup             0     0     0    - /sys/fs/cgroup/cpu,cpuacct
cgroup             0     0     0    - /sys/fs/cgroup/net_cls,net_prio
cgroup             0     0     0    - /sys/fs/cgroup/perf_event
cgroup             0     0     0    - /sys/fs/cgroup/freezer
cgroup             0     0     0    - /sys/fs/cgroup/memory
cgroup             0     0     0    - /sys/fs/cgroup/cpuset
cgroup             0     0     0    - /sys/fs/cgroup/hugetlb
cgroup             0     0     0    - /sys/fs/cgroup/blkio
cgroup             0     0     0    - /sys/fs/cgroup/devices
configfs           0     0     0    - /sys/kernel/config
/dev/sda6        70G  4.7G   66G   7% /
systemd-1          -     -     -    - /proc/sys/fs/binfmt_misc
debugfs            0     0     0    - /sys/kernel/debug
mqueue             0     0     0    - /dev/mqueue
hugetlbfs          0     0     0    - /dev/hugepages
nfsd               0     0     0    - /proc/fs/nfsd
/dev/sr0        4.3G  4.3G     0 100% /mnt/cdrom
/dev/sda3       5.0G   33M  5.0G   1% /home
/dev/sda2        20G  164M   20G   1% /var
/dev/sda1       497M  128M  370M  26% /boot
sunrpc             0     0     0    - /var/lib/nfs/rpc_pipefs
tmpfs           378M  4.0K  378M   1% /run/user/0
binfmt_misc        0     0     0    - /proc/sys/fs/binfmt_misc
 
=========================free
              total        used        free      shared  buff/cache   available
Mem:           3.7G        939M        1.8G        8.9M        1.0G        2.5G
Swap:          4.0G          0B        4.0G
```
以上是os模块的system方法，popen方法和commands库调用Linux库的方法。后续补充subprocess的调用Linux系统命令的方法。（人在外面没Python环境）


### 3.1 subprocess.Popen()
使用Popen可以创建进程，并与进程进行复杂的交互。

用法：child = subprocess.Popen(["cmd","arg1"...])

常用参数说明：

![image-20201123115519457](C:\Users\Administrator\Desktop\pycharm调用系统命令的方法\python 调用系统命令.assets\image-20201123115519457.png)

示例
```py
from subprocess import PIPE, Popen

# 返回的是 Popen 实例对象
proc = Popen(
    'ipconfig',  # cmd特定的查询空间的命令
    stdin=None,  # 标准输入 键盘
    stdout=PIPE,  # -1 标准输出（演示器、终端) 保存到管道中以便进行操作
    stderr=PIPE,  # 标准错误，保存到管道
    shell=True)

# print(proc.communicate()) # 标准输出的字符串+标准错误的字符串
outinfo, errinfo = proc.communicate()
print(outinfo.decode('gbk'))  # 外部程序(windows系统)决定编码格式
print(errinfo.decode('gbk'))
```
输出：
```
以太网适配器 以太网:

   连接特定的 DNS 后缀 . . . . . . . : 
   本地链接 IPv6 地址. . . . . . . . : fe81::b0ed:2b1b:7385:97d1%8
   IPv4 地址 . . . . . . . . . . . . : 192.168.1.100
   子网掩码  . . . . . . . . . . . . : 255.255.255.0
   默认网关. . . . . . . . . . . . . : 192.168.1.1


Process finished with exit code 0
```
### 3.2 subprocess.call()
执行指定的命令， 返回命令执行状态， 功能类似os.system（cmd），参数shell默认为False

用法：subprocess.call("command")
```py
# linux获取磁盘空间
import subprocess
subprocess.call(['df', '-h']) # 数组作为参数运行命令
```
输出：
```
Filesystem      Size  Used Avail Use% Mounted on
devtmpfs        909M     0  909M   0% /dev
tmpfs           920M   32K  920M   1% /dev/shm
tmpfs           920M  472K  919M   1% /run
tmpfs           920M     0  920M   0% /sys/fs/cgroup
/dev/vda1        50G  6.2G   41G  14% /
tmpfs           184M     0  184M   0% /run/user/0
```
### 3.3 subprocess.run()
python3.5中新增的函数， 执行指定的命令， 等待命令执行完成后返回一个包含执行结果的CompletedProcess类的实例。run默认不会返回输出，只返回命令和执行状态。

用法：

subprocess.run(args, *, stdin=None, input=None, stdout=None, stderr=None, shell=False, timeout=None, check=False, universal_newlines=False)
```py
import subprocess
completed = subprocess.run(['ls', '-1'])
print('returncode:', completed.returncode)
```
### 3.4 subprocess.getstatusoutput()

执行cmd命令，返回一个元组（命令执行状态，命令执行结果输出）。

返回状态码和结果，0表示成功
```py
import subprocess
ret, val = subprocess.getstatusoutput("ping www.baidu.com")
print(ret)
print(val)
0

正在 Ping www.a.shifen.com [14.215.177.39] 具有 32 字节的数据:
来自 14.215.177.39 的回复: 字节=32 时间=29ms TTL=53
来自 14.215.177.39 的回复: 字节=32 时间=29ms TTL=53
来自 14.215.177.39 的回复: 字节=32 时间=29ms TTL=53
来自 14.215.177.39 的回复: 字节=32 时间=30ms TTL=53

14.215.177.39 的 Ping 统计信息:
    数据包: 已发送 = 4，已接收 = 4，丢失 = 0 (0% 丢失)，
往返行程的估计时间(以毫秒为单位):
    最短 = 29ms，最长 = 30ms，平均 = 29ms
```
