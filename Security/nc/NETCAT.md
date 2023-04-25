# NC简介
NC是NETCAT的简写，有着网络界的瑞士军刀美誉。因为他短小精悍，功能实用，被设计为一个简单，可靠的网络工具。

# NC的作用
（1）传输文本信息

（2）传输文件/目录

（3）加密传输文件

（4）远程控制/木马

（5）加密传输文件

（6）流媒体服务器

（7）端口扫描

（8）远程克隆硬盘

 
# NC常用的参数
  -h 帮助信息

   -l 监听模式

  -n 指定数字的IP地址，不会进行DNS解析

  -p 指定监听的本地端口

  -u UDP模式

  -v 显示详细的信息

  -i secs延迟时间 

 -w secs 超时时间 

 -z  表示扫描时不发送任何数据包

-q 限制会话的次数 （到达次数后自动关闭会话，用于证明数据传输完成）

# 前期准备
准备两台机器，用于测试nc命令方法

主机A ：10.1.1.1

主机B：10.1.1.2

# NC传输文本信息
 利用NC可以实现聊天，原理也十分简单，首先A先侦听一个端口(4444)，等待B连接.。

然后主机指定主机A的IP地址以及侦听的端口(4444)。

A： nv   -lp  4444           

 B：nc -nv  10.1.1.1 4444

# NC传输文件
## 传输文件演示（侦听的端口接受数据）

使用NC传输文件还是比较方便的，因为不用scp和rsync那种输入密码的操作了

把A机器上的一个txt文件发送到B机器上

需要注意操作次序，receiver (A)先侦听端口，sender (B)向recevier (A)所在的机器的该端口发送数据。

## 步骤1，先在A机器上启动一个侦听端口4444等待B连接A，然后把数据存在1.txt中

A： nc  -lp  4444    >  1.txt    

## 步骤2 在B机器上往A机器的4444端口发送数据，把1.txt文件发送过去

B： nc   -nv  10.1.1.2 4444  <   1.txt

A机器接收完毕，文件大小和B机器一样，MD5值也一样。

## 传输文件演示（侦听的端口发送数据）

## 步骤1，A机器通过侦听4444端口等待B来连接A，并发送1.txt文件给B

A：nc -lp 4444 < 1.txt

## 步骤2，B机器连接上A机器，接受数据存在1.txt中

下面命令表示连接A机器的4444端口接受文件，并把文件保存在本目录下，文件名为1.txt

B：nc -nv 10.1.1.1 4444 > 1.txt

 

# NC传输目录
## 传输目录演示(侦听的端口接受数据)

## 步骤1，A机器先启动监听，等待B机器连接A，接受数据然后通过tar解压

传输目录需要结合其他的命令，比如tar

经过测试管道后面最后必须是 - ，不能是其他的自定义的文件名

A： nc -lp 4444  |  tar -xvf - 

## 步骤2，B机器打包文件并连接A机器的端口，将数据发送给A

管道前面表示把music目录下的文件打包为 - 

B： tar -cvf  - music/ | nc -nv  10.1.1.1 4444 

# 传输目录演示(侦听的端口发送数据)

## 步骤1，A机器先将文件打包，然后通过管道符将数据发给侦听的4444端口

A：   tar -cvf  - music/ | nc -lp 4444

## 步骤2，B连接A机器的4444端口接受数据

B： nc -nv  10.1.1.1 4444   |  tar -xvf -

 

# NC流媒体服务
流媒体服务演示(侦听的端口发送数据)

## 步骤1，A机器先查看1.mp4，然后通过管道符把数据发给侦听4444端口

A：cat 1.mp4 | nc -lp 4444

## 步骤2，B机器连接A机器侦听的端口，把接受的数据通过管道符发给mplayer播放

B： nc -nv 10.1.1.1 4444 | mplayer -vo x11 -cache 3000 -

# 流媒体服务演示(侦听的端口接受数据)

## 步骤1，A机器先侦听4444端口，然后通过管道符把接收的数据发给mplayer播放

A： nc -lp 4444 | mplayer -vo x11 -cache 3000 -

## 步骤2，B机器先查看1.mp4 然后通过管道符把数据通过连接A机器4444端口发送给A数据

B：cat 1.mp4 | nc -nv 10.1.1.1 4444 

# NC端口扫描
UDP的扫描过程比较慢，可能1秒一个端口，下面表示扫描A机器1-024端口。

nc -nvz 10.1.1.1 1-65535 （默认扫描TCP端口）

nc -nvzu  10.1.1.1 1-1024 (扫描UDP端口)

# NC远程克隆硬盘
远程克隆硬盘演示(侦听的端口发送数据)

## 步骤1，A机器将硬盘的数据通过管道符发给侦听的4444端口

A：dd if=/dev/sda | nc -lp 4444  

## 步骤2，B机器连接A机器的4444端口获取数据

B：nc -nv 10.1.1.1 4444 | dd of=/dev/sda

# 远程克隆硬盘演示(侦听的端口接受数据)

## 步骤1，A机器先侦听的4444端口，将接受的数据放在sda硬盘上

A： nc -lp 4444 | dd of=/dev/sda

## 步骤2，B机器将硬盘的数据通过连接A机器的4444端口发送给A数据

B：dd if=/dev/sda | nc -nv 10.1.1.1 4444

适用于远程电子取证，可以将目标服务器硬盘远程复制，或者内存。