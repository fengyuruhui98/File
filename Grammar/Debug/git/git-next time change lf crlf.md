# Git: ‘LF will be replaced by CRLF the next time Git touches it‘ 问题解决与思考

## 一、问题
windows平台进行 git add 时，控制台打印警告warning: in the working copy of ‘XXX.py’, LF will be replaced by CRLF the next time Git touches it
 

## 二、问题分析
Dos/Windows平台默认换行符：回车（CR）+换行（LF），即’\r\n’  
Mac/Linux平台默认换行符：换行（LF），即’\n’  
企业服务器一般都是Linux系统进行管理，所以会有替换换行符的需求  
 

## 三、解决方法
设置方法一：
```sh
#提交时转换为LF，检出时转换为CRLF
git config --global core.autocrlf true
```
*适用于Windows系统，且一般为Windows默认设置，会在提交时对换行符进行CRLF - LF的转换，检出时又会进行LF - CRLF的转换。
 

设置方法二：
```sh
#提交时转换为LF，检出时不转换
git config --global core.autocrlf input
```
*适用于Linux系统，所有换行符都会进行CRLF - LF转换，但操作时不会转换回CRLF。
 

设置方法三：
```sh
#提交检出均不转换
git config --global core.autocrlf false
```
*适用于Windows系统，且只在Windows上开发的情况。在提交、检出时不会对CRLF/LF换行符进行转换
 

文件提交时进行safecrlf检查：
```sh
#拒绝提交包含混合换行符的文件
git config --global core.safecrlf true

#允许提交包含混合换行符的文件
git config --global core.safecrlf false

#提交包含混合换行符的文件时给出警告
git config --global core.safecrlf warn
```

## 四、问题思考
跨平台文件都有兼容性的问题，为什么只有core.autocrlf参数设置true检出时，会有LF-CRLF的转换？  
有看到跨平台文件的问题：  
· Linux文件在Windows下会显示成一行。  
· Windows文件在Linux下结尾可能多出^M符号  
 
那么就有以下可能性：  
① Windows因为可视化界面较好，操作简易，且文件格式对日常操作没有较大影响，所以不做该功能。  
② Git的pull等功能将文件拉取到本地时，都会基于检出配置进行操作，所以只要把core.autocrlf设置成true就好了。
