java -jar命令用于执行jar程序，常用的写法有5种：

## 第1种

```sh
java -jar demo.jar
```

说明：用这种方法启动后，不能继续执行其它命令了，如果想要继续执行其它命令，需要退出当前命令运行窗口，会打断jar的运行，打断一般用ctrl+c。

## 第2种

```sh
java -jar demo.jar &
```

第2种在第1种方式的基础上在命令的结尾增加了&，&表达的意思是在后台运行。

这种方式可以避免打断后程序停止运行的问题，但是如果关闭当前窗口后程序会停止运行。

## 第3种

```sh
nohup java -jar demo.jar &
```

第3种在第2种方式的基础上，在命令的最前面增加了nohup。

nohup 英文全称 no hang up（不挂起），用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。

nohup 命令，在默认情况下（非重定向时），会输出一个名叫 nohup.out 的文件到当前目录下，如果当前目录的 nohup.out 文件不可写，输出重定向到 $HOME/nohup.out 文件中。

nohup是不挂断运行命令,当账户退出或终端关闭时,程序仍然运行。

加了nohup后，即使关掉命令窗口，后台程序demo.jar也会一直执行。

## 第4种

```sh
nohup java -jar demo.jar >1.txt &
```

第4种在第3种的基础上，在后面增加了>1.txt，意思是将nohup java -jar demo.jar的运行内容重定向输出到1.txt文件中，即输出内容不打印到当前窗口上，而是输出到1.txt文件中。

第3种没有加>1.txt，它的输出重定向到nohup.out文件中，nohup.out也就是nohup命令的默认输出文件， 文件位于\$HOME/nohup.out 文件中,比如用root执行，就会输出到/root/nohup.out。

## 第5种

```sh
nohup java -jar demo.jar >/dev/null 2>&1 &
```

这里说下jar后面这串符号的意义 >/dev/null 2>&1 &

- > 代表重定向到哪里，例如：echo "123" > /home/123.txt
- /dev/null 代表空设备文件
- 2> 表示stderr标准错误
- & 表示等同于的意思，2>&1，表示2的输出重定向等同于1
- 1 表示stdout标准输出，系统默认值是1，所以">/dev/null"等同于 "1>/dev/null"
- 最后一个&表示在后台运行。

这里再补充说下这几个数字代表的含义：

- 0 标准输入（一般是键盘）
- 1 标准输出（一般是显示屏，是用户终端控制台）
- 2 标准错误（错误信息输出）

- /dev/null ：首先表示标准输出重定向到空设备文件，也就是不输出任何信息到终端，说白了就是不显示任何信息。一般项目中定义中输出运行日志到指定地址，这样的话，就不需要再单独输出nohup.out文件，这种情况可以考虑使用这种。

我们可以把它写成一个脚本，不用每次都写一遍。新建start.sh，根据我上传的demo.jar,输出到1.txt中，具体脚本如下：

```sh
nohup  java  -Xms515m -Xmx1024m -jar -Dfile.encoding=UTF-8 demo.jar --spring.profiles.active=prod >/dev/null 2>&1 &
```

可以看到，上面的命令中我使用了Xms、Xmx、Dfile.encoding、spring.profiles.active等参数，那么java -jar可以添加什么参数，各自又能实现什么样的效果呢，且看下文：

  -Xms 指定jvm运行最小运行堆内存，默认为物理内存1/64，用法 ：-Xmx512m 注意：Xmx和512m中间不用添加空格

  -Xmx 指定jvm运行最大运行堆内存，认物理内存1/4，用法： -Xmx1024m 注意：Xmx和1024m中间不用添加空格

  --server.port 指定jar运行的port端口，用法：--server.port=8085

  --spring.profiles.active=pro 指定运行的配置文件、环境，用法：--spring.profiles.active=prod