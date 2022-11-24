# Ubuntu中如何管理python环境(无ananconda)

## 一、前言
在以前使用Ubuntu的时候，都是直接安装python的各种库，也没有注意过这些库以及python被具体安装到了哪里。可能是由于之前系统中的python版本较少，或者其他原因，反正没有遇到过环境问题。

但是这两天，我需要尝试安装不同版本的python3，比如3.6和3.8，那么这个时候我就开始有疑问了：

不同版本的python3在系统中是如何区分的？  
我应该如何指定库只安装到python3.6或者python3.8之中？  
我在使用pip安装库的时候这些库是被安装到了哪里？  
之所以会有这些疑问，是因为其实在windows系统中这些问题其实都很明确，而且也是一开始学python必备的配置环境的知识，但是到了linux中我就有点懵了emmm！！！那么下面我就来简单的解释一下上面的三个问题。

## 二、问题解释
1、不同版本的python3在系统中是如何区分的？

我们在使用sudo apt-get install安装好python3.x之后，此时会在/usr/bin路径下，产生一个python3.x的可执行文件，其实就可以理解为windows下面的.exe。

/usr/bin路径下存放的就是我们平时使用的一些可执行文件，例如你可以直接在命令行里面敲得各种命令(它会自动的在该路径下寻找命令对应的可执行文件，相当于被添加了环境变量)。当然，这些命令也会有一些是在/bin路径下。

需要注意的是，这里因为Ubuntu 18.04 LTS默认安装的就是python3.6，所以还会有一个python3的链接指向python3.6，也就是此时你在命令行敲下以下命令都会执行python3.6的可执行文件。

python3或者python3.6

如果此时你又安装了其他版本的python，如python3.8，那么在此路径下还会出现一个python3.8。如果你直接使用python3.6和python3.8来打开python，那么其实都无所谓，因为它们都各自对应着自己版本的python可执行文件。

但是如果你想要在命令行只输入python或者python3或者其他命令就可以打开某个版本的python，那么你只需要为每个执行文件创建一个软链接或者硬链接即可，链接文件的名字你随便起。创建的链接你也可以删除，比如上面说到的python3就可以删除，但是最好别删除掉原始的python3.x的可执行文件。

2、我应该如何指定库只安装到python3.6或者python3.8之中？

首先，你需要先安装pip，我们只需要在命令行输入以下命令即可：

sudo apt-get install python3-pip

安装好python3-pip之后，其实所有的python3.x都是可以使用的，好像没有比如python3.6-pip，所以只能安装上面说的那个。

安装好之后pip之后，我们在安装库的时候使用如下命令来安装：

python3.x -m pip install 包名

这样在前面加上python3.x的限制之后，安装的包就只属于某个版本的python。这样的话，如果没有特殊需求，其实不用ananconda就可以不错的管理python环境了。

3、我在使用pip安装库的时候这些库是被安装到了哪里？

对于这个问题，我们可以先使用如下命令(我的python3链接到的是python3.6)：

python3 -m pip list

这样我们会得到我们已经安装的python库的列表，然后选择其中一个库，使用如下命令来查看具体某个包的信息：

python3 -m pip show 包名

通过查看以上几个包的信息，发现它们都被安装到了~$/.local/lib/python3.6/site-packages路径下，所以我们大致可以总结出pip安装的包，都被装在了~$/.local/lib/python3.x/site-packages路径下。


## 三、镜像源

