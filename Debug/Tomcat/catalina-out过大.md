前言
有用Tomcat的，绝对都会遇到这样一个问题：catalina.out文件过大。

它是Tomcat默认生成的日志文件，会随着时间的推移，逐渐增大，不断的增长，甚至达到几G，几十G的大小。由于文件过大，不仅占系统的存储，我们还将无法使用过常规的编辑工具进行查看，严重影响系统的维护工作。

对此，出现了以下几种解决catalina.out文件过大的方案。

# 暴力型
除非不需要日志，否则不建议使用

## 1.手动版
每次监控到tomcat的硬盘空间变小达到阈值，手动登陆服务器，切换到tomcat的logs下，手动清空

echo " "  > catalina.out

## 2.脚本版
编写脚本，放入计划任务中,定时清空

```sh
crontab -e 
0 24 * * *    sh /root/qin_catalina.out.sh

vim qin_catalina.out.sh
 #!/usr/bin/bash 
 echo " " > catalina.out
```
# 技术型
## 1.日志切割工具版0.1

使用cronolog日志切分工具切分Tomcat的catalina.out日志文件

<1>下载cronolog，并进行安装

```sh
wget http://cronolog.org/download/cronolog-1.6.2.tar.gz   (中国服务器可能无法下载或下载缓慢，可先下载到境外服务器上)

tar zxvf cronolog-1.6.2.tar.gz

./cronolog-1.6.2/configure

make

make install

(默认安装在/usr/local/sbin下)
```
<2>.配置

在tomcat/bin/catalian.sh中

```
org.apache.catalina.startup.Bootstrap "$@" start \ >> "$CATALINA_BASE"/logs/catalina.out 2&1 &

改成:
org.apache.catalina.startup.Bootstrap"$@" start \ |/usr/local/sbin/cronolog "$CATALINA_BASE"/logs/catalina.%Y-%m-%d.out >> /dev/null 2>&1 &
或
org.apache.catalina.startup.Bootstrap   "$@"  start  2>&1  \   |  /usr/local/sbin/cronolog "$CATALINA_BASE"/logs/catalina.%Y-%m-%d.out >> /dev/null & 

并注释    touch "$CATALINA_OUT"  
```

<3>重启Tomcat
Tomcat输出日志文件分割成功，输出log文件格式变为：catalina.2017-05-15.out

## 1.日志切割工具版0.2

CentOS6.5后自带logrotate程序，可以解决catalina.out的日志轮转问题

<1>在/etc/logrotate.d/目录下新建一个tomcat的文件

```sh
cat >/etc/logrotate.d/tomcat 

/usr/local/tomcat/logs/catalina.out{            要轮转的文件
    copytruncate                                创建新的catalina.out副本，截断源catalina.out文件
    daily                                       每天进行catalina.out文件的轮转
    rotate 7                                    至多保留7个副本
    missingok                                   文件丢失了，轮转不报错
    compress                                    使用压缩
    size 500M                                    当catalina.out文件大于16MB，就轮转
}
```

其他参数：

```
compress                                   通过gzip 压缩转储以后的日志
nocompress                                 不做gzip压缩处理
copytruncate                               用于还在打开中的日志文件，把当前日志备份并截断；是先拷贝再清空的方式，拷贝和清空之间有一个时间差，可能会丢失部分日志数据。
nocopytruncate                             备份日志文件不过不截断
create mode owner group                    轮转时指定创建新文件的属性，如create 0777 nobody nobody
nocreate                                   不建立新的日志文件
delaycompress                              和compress 一起使用时，转储的日志文件到下一次转储时才压缩
nodelaycompress                            覆盖 delaycompress 选项，转储同时压缩。
missingok                                  如果日志丢失，不报错继续滚动下一个日志
errors address                             专储时的错误信息发送到指定的Email 地址
ifempty                                    即使日志文件为空文件也做轮转，这个是logrotate的缺省选项。
notifempty                                 当日志文件为空时，不进行轮转
mail address                               把转储的日志文件发送到指定的E-mail 地址
nomail                                     转储时不发送日志文件
olddir directory                           转储后的日志文件放入指定的目录，必须和当前日志文件在同一个文件系统
noolddir                                   转储后的日志文件和当前日志文件放在同一个目录下
sharedscripts                              运行postrotate脚本，作用是在所有日志都轮转后统一执行一次脚本。如果没有配置这个，那么每个日志轮转后都会执行一次脚本
prerotate                                  在logrotate转储之前需要执行的指令，例如修改文件的属性等动作；必须独立成行
postrotate                                 在logrotate转储之后需要执行的指令，例如重新启动 (kill -HUP) 某个服务！必须独立成
daily                                      指定转储周期为每天
weekly                                     指定转储周期为每周
monthly                                    指定转储周期为每月
rotate count                               指定日志文件删除之前转储的次数，0 指没有备份，5 指保留5 个备份
dateext                                    使用当期日期作为命名格式
dateformat .%s                             配合dateext使用，紧跟在下一行出现，定义文件切割后的文件名，必须配合dateext使用，只支持 %Y %m %d %s 这四个参数
size(或minsize) log-size                   当日志文件到达指定的大小时才转储，log-size能指定bytes(缺省)及KB (sizek)或
```

<2>当执行以上操作时是自动执行的，也可手动切割

```sh
logrotate /etc/logrotate.conf
```

如果只轮转tomcat配置文件，要指定文件

```sh
logrotate --force /etc/logrotate.d/tomcat
```

<3>删除要清理的日志

手工查找需要清理的日志文件

```sh
cd /usr/local/tomcat/logs
rm -rf catalina.out.4.gz
```

## 2.日志切割脚本版

使用cron每天来定时备份当前的catalina.out，然后清空他的内容；
<1>crontab -e

```sh
01 0 * * *  sh /root/qie_catalina.out.sh
1
<2>cat qie_catalina.out.sh 参考脚本

 #!/bin/bash 
 DATE=`date "+%Y-%m-%d"`


cp /etc/tomcat/logs/catalina.out  /etc/tomcat/logs/`catalina.out.$DATE`
wait

echo " " > catalina.out
```
## 3.修改日志级别版

日志级别：
SEVERE (highest value) > WARNING > INFO > CONFIG > FINE > FINER > FINEST (lowest value)

<1>修改conf/logging.properties日志配置文件，将level级别设置成WARNING减少日志的输出。也可以设置成OFF，直接禁用。

catalina.org.apache.juli.FileHandler.level = WARNING

catalina.org.apache.juli.FileHandler.directory = ${catalina.base}/logs

catalina.org.apache.juli.FileHandler.prefix = catalina.
————————————————
版权声明：本文为CSDN博主「漠效」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/GX_1_11_real/article/details/80204475