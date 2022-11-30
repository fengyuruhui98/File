# sleep()方法

python时间sleep()方法按给定的秒数暂停执行。该参数可以是浮点数，以指示更精确的睡眠时间。  
由于任何捕获的信号将在执行该信号的捕获程序之后终止sleep()，所以实际的暂停时间可能会小于请求的时间。

## 语法
以下是sleep()方法的语法 -  
time.sleep(t)  
Python  

需要引入时间库  
import time

## 参数

t - 这是暂停执行的秒数。
## 返回值

此方法不返回任何值。  
示例以下示例显示了sleep()方法的用法 -   
```py
#!/usr/bin/python3
import time

print ("Start : %s" % time.ctime())
time.sleep( 5 )
print ("End : %s" % time.ctime())
```
Python
当运行上述程序时，它会产生以下结果 -
Start : Wed Jun 21 05:25:36 2018
End : Wed Jun 21 05:25:41 2018
