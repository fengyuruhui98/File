# 前言
## Crontab介绍：
Linux crontab是用来crontab命令常见于Unix和类Unix的操作系统之中，用于设置周期性被执行的指令。该命令从标准输入设备读取指令，并将其存放于“crontab”文件中，以供之后读取和执行。该词来源于希腊语 chronos(χρ?νο?)，原意是时间。通常，crontab储存的指令被守护进程激活， crond常常在后台运行，每一分钟检查是否有预定的作业需要执行。这类作业一般称为cron jobs。当安装完成操作系统之后，默认便会启动此任务调度命令。crond 命令每分锺会定期检查是否有要执行的工作，如果有要执行的工作便会自动执行该工作。

注意：新创建的 cron 任务，不会马上执行，至少要过 2 分钟后才可以，当然你可以重启 cron 来马上执行。

## 1.使用yum命令安装Crontab：
yum install vixie-cron 
yum install crontabs
注：vixie-cron软件包是cron的主程序；
crontabs软件包是用来安装、卸装、或列举用来驱动 cron 守护进程的表格的程序。
cron是linux的内置服务，但它不自动起来，可以用以下的方法启动、关闭这个服务：
/sbin/service crond start #启动服务
systemctl start crond.service
/sbin/service crond stop #关闭服务
systemctl stop crond.service
/sbin/service crond restart #重启服务
systemctl restart crond.service
/sbin/service crond reload #重新载入配置
systemctl reload crond.service

## 2.查看Crontab状态：
service crond status
systemctl status crond.service
ntsysv #查看crontab服务是否已设置为开机启动
chkconfig –level 35 crond on #加入开机自动启动

## 3.添加定时任务：
crontab -e #编辑cron任务模式
i #默认文字编辑器为vim，按i字母键即可添加cron任务
30 3 * * * /usr/local/etc/rc.d/lighttpd restart #将命令代码放入，此命令意义为每天的03:30 重启apache
ESC #按ESC键退出编辑模式
:wq #键入:wq保存
service crond restart #重启crontab服务

## 4.查看任务列表：
crontab -l

## 5.Crontab相关命令：
### （1）语法：
crontab [-u <用户名称>][配置文件] 或 crontab { -l | -r | -e }
-u   #<用户名称> 是指设定指定<用户名称>的定时任务，这个前提是你必须要有其权限(比如说是 root)才能够指定他人的时程表。如果不使用 -u user 的话，就是表示设定自己的定时任务。
-l 　#列出该用户的定时任务设置。
-r 　#删除该用户的定时任务设置。
-e 　#编辑该用户的定时任务设置。
 
### （2）命令时间格式 :
```
.---------------- 分 (0 - 59)
|  .------------- 时 (0 - 23)
|  |  .---------- 日 (1 - 31) 
|  |  |  .------- 月 (1 - 12) 
|  |  |  |  .---- 星期 (0 - 6) (星期日可为0或7) 
|  |  |  |  | 
*  *  *  *  * 执行的命令
```
/*     * 　  *　  *　  *　　command
分　  时　  日　  月　 周　  命令
第1列表示分钟1～59 每分钟用*或者 */1表示
第2列表示小时1～23（0表示0点）
第3列表示日期1～31
第4列表示月份1～12
第5列标识号星期0～6（0表示星期天）
第6列要运行的命令
 
### （3）一些Crontab定时任务例子：
30 21 * * * /usr/local/etc/rc.d/lighttpd restart  #每晚的21:30 重启apache
45 4 1,10,22 * * /usr/local/etc/rc.d/lighttpd restart  #每月1、10、22日的4 : 45重启apache
10 1 * * 6,0 /usr/local/etc/rc.d/lighttpd restart  #每周六、周日的1 : 10重启apache
0,30 18-23 * * * /usr/local/etc/rc.d/lighttpd restart  #每天18 : 00至23 : 00之间每隔30分钟重启apache
0 23 * * 6 /usr/local/etc/rc.d/lighttpd restart  #每星期六的11 : 00 pm重启apache
* 23-7/1 * * * /usr/local/etc/rc.d/lighttpd restart  #晚上11点到早上7点之间，每隔一小时重启apache
* */1 * * * /usr/local/etc/rc.d/lighttpd restart  #每一小时重启apache
0 11 4 * mon-wed /usr/local/etc/rc.d/lighttpd restart  #每月的4号与每周一到周三的11点重启apache
0 4 1 jan * /usr/local/etc/rc.d/lighttpd restart  #一月一号的4点重启apache
 
*/30 * * * * /usr/sbin/ntpdate cn.pool.ntp.org  #每半小时同步一下时间
0 */2 * * * /sbin/service httpd restart  #每两个小时重启一次apache 
50 7 * * * /sbin/service sshd start  #每天7：50开启ssh服务 
50 22 * * * /sbin/service sshd stop  #每天22：50关闭ssh服务 
0 0 1,15 * * fsck /home  #每月1号和15号检查/home 磁盘 
1 * * * * /home/bruce/backup  #每小时的第一分执行 /home/bruce/backup这个文件 
00 03 * * 1-5 find /home "*.xxx" -mtime +4 -exec rm {} \;  #每周一至周五3点钟，在目录/home中，查找文件名为*.xxx的文件，并删除4天前的文件。
30 6 */10 * * ls  #每月的1、11、21、31日是的6：30执行一次ls命令

## 6.部分脚本无法执行问题：
### 1. 如果我们使用 crontab 来定时执行脚本，无法执行，但是如果直接通过命令（如：./test.sh)又可以正常执行，这主要是因为无法读取环境变量的原因。
解决方法：
（1）所有命令需要写成绝对路径形式，如: /usr/local/bin/docker, /bin/bash。
（2）在 shell 脚本开头使用以下代码：

#!/bin/sh
 
. /etc/profile
. ~/.bash_profile

## 2.更改系统时区后需要重启cron。

7.Crontab默认调度任务：
cron默认配置了调度任务，分别为：hourly、daily、weekly、mouthly，默认配置文件为/etc/anacrontab
将需要执行的脚本放到相应的目录下即可，目录分别为：

/etc/cron.hourly
/etc/cron.daily
/etc/cron.weekly
/ect/cron.mouthly
1
2
3
4
8.注意清理系统用户的邮件日志：
每条任务调度执行完毕，系统都会将任务输出信息通过电子邮件的形式发送给当前系统用户，这样日积月累，日志信息会非常大，可能会影响系统的正常运行，因此，将每条任务进行重定向处理非常重要。
例如，可以在crontab文件中设置如下形式，忽略日志输出：

0 */3 * * * /usr/local/apache2/apachectl restart >/dev/null 2>&1
1
“/dev/null 2>&1”表示先将标准输出重定向到/dev/null，然后将标准错误重定向到标准输出，由于标准输出已经重定向到了/dev/null，因此标准错误也会重定向到/dev/null，这样日志输出问题就解决了。

9.Crontab日志路径：
ll /var/log/cron* #/var/log/cron只会记录是否执行了某些计划的脚本，但是具体执行是否正确以及脚本执行过程中的一些信息则linux会每次都发邮件到该用户下。

10.Crontab命令在线验证工具：
crontab执行时间计算 - 在线工具

————————————————
版权声明：本文为CSDN博主「逆流°只是风景-bjhxcc」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u011397981/article/details/134021831