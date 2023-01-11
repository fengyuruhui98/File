执行报错信息

Line 207: Char 3: runtime error: load of null pointer of type 'int' (__Serializer__.c)


原因分析

因为函数返回的是指针地址指向函数内的局部变量数组，在函数退出时，数组的存储空间会被销毁，此时去访问该地址就会出现这个错误。

修改方法一般有3种：

1）返回的指针提前分配空间

2）用static修饰变量

3）使用全局变量