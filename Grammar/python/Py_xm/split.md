# Python split()方法

## 描述
Python split() 通过指定分隔符对字符串进行切片，如果参数 num 有指定值，则分隔 num+1 个子字符串

## 语法
split() 方法语法：

str.split(str="", num=string.count(str)).
## 参数
str -- 分隔符，默认为所有的空字符，包括空格、换行(\n)、制表符(\t)等。
num -- 分割次数。默认为 -1, 即分隔所有。
## 返回值
返回分割后的字符串列表。

## 实例
以下实例展示了 split() 函数的使用方法：

实例(Python 2.0+)
```py
#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
str = "Line1-abcdef \nLine2-abc \nLine4-abcd";
print str.split( );       # 以空格为分隔符，包含 \n
print str.split(' ', 1 ); # 以空格为分隔符，分隔成两个
```
以上实例输出结果如下：
```
['Line1-abcdef', 'Line2-abc', 'Line4-abcd']
['Line1-abcdef', '\nLine2-abc \nLine4-abcd']
```
以下实例以 # 号为分割符，指定第二个参数为 1，返回两个参数列表。

实例(Python 2.0+)
```py
#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
txt = "Google#Runoob#Taobao#Facebook"
 
# 第二个参数为 1，返回两个参数列表
x = txt.split("#", 1)
 
print x
```
以上实例输出结果如下：
```
['Google', 'Runoob#Taobao#Facebook']
```