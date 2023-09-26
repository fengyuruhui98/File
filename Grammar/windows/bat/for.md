# Windows 批处理(bat) for循环语句使用教程

## for指令基本格式
基本格式： 
```for %%i in (command1) do command2```
```for [参数] %%变量名 in （匹配符） do （执行的命令）```

注意事项：

- for、in 和 do是for语句的关键字，它们三个缺一不可；
- 在 in 之后，do 之前的括号"()"不能省略；
- %%i 是for语句中对形式变量的引用；
- for语句的形式变量I，可以换成26个字母中的任意一个，这些字母会区分大小写；
- in和do之间的command1表示的字符串或变量可以是一个，也可以是多个，每一个字符串或变量，我们称之为一个元素，每个元素之间，用空格键、跳格键、逗号、分号或等号分隔；
- 每个指令之间必须以空格隔开，in 与 ( 之间有空格，do 与 （ 间也有空格，否则命令会无法成功执行
```bat
@echo off

for %%a in (ele1 ele2 ele3) do (
	echo %%a
)
for %%a in (1,2,3) do (
	echo %%a
)
pause
```

## 指令参数
### 1. 参数 /d(D) 遍历文件夹
格式：
```for /d %%i  in (路径) do command```

括号里的 * 代表bat文件所在的目录
```bat
@echo off
for /d %%i in (*) do (echo %%i)
pause
```
### 2. 参数 /l(L) 数字序列
格式：
```for /l %%i  in (start,step,end) do command```

表示以step为增量从开始到结束的一个数字序列

```bat
@echo off
for /l %%i in (1,3,20) do (echo %%i)
pause
```

### 3. 参数 /f(F) 打印文件内容
不加参数，默认以空格分割此行文本，即只显示第一个空格前或此行结尾前文本

格式：
```
for /f ["options"] %%i IN (file) DO command
for /f ["options"] %%i IN ("string") DO command
for /f ["options"] %%i IN ('command') DO command
```
1. file为文件名，for会依次将file中的文件打开，而且在进行到下一个文件以前将每一个文件读取到内存，按照每一行分为一个一个的元素，忽略空白的行
```bat
@echo off
for /f %%i in (.\test.txt) do (echo %%i)
pause
```

如果不加 /f 参数，只会显示 test.txt 文件名字

#### 3.1 /f "delims= "
此外，还可以将每一行分解为更小的内容
```
for /f "delims= " %%i in (.\test.txt) do echo %%i
```
delims这个参数，=后面有一个空格，意思是再将每一个元素以空格分割，默认是只取分割以后的第一个元素。
/f "delims=" **注意：这种 = 后不指定分割符的，会打印整行文本**

#### 3.2 /f "tokens=2 delims= "
tokens的作用就是当你经过delims将每一行分为更小的元素时，由它来控制要取哪个或哪几个。
```bat
:: 取 dilims 分成的第2个元素
for /f "tokens=2 delims= " %%i in (a.txt) do echo %%i

:: 取 dilims 分成的第2到4个元素
for /f "tokens=2,3,4 delims= " %%i in (a.txt) do echo %%i %%j %%k
for /f "tokens=2-4 delims= " %%i in (a.txt) do echo %%i %%j %%k
:: i j k 分别代表取的三个元素

:: 取 dilims 分成的第2之后的元素
for /f "tokens=2,* delims= " %%i in (a.txt) do echo %%i %%j %%k
``` 
#### 3.3 /f “skip=1”
用 skip 来告诉for跳过前两行。
```bat
@echo off
for /f "skip=1" %%i in (.\test.txt) do (echo %%i)
pause

```
#### 3.4 /f “eol=1”
用 eol 来告诉for忽略以“.”开头的行。
```bat
@echo off
for /f "eol=1" %%i in (.\test.txt) do (echo %%i)
pause
```
### 4. 参数 /r(R） 是否在子目录中查找文件
格式：
```
for /r "目录路径" %%a in (匹配规则，例如*.txt) do 执行操作
```
 

## 高级用法
搜索目录下有哪些文件
```bat
@echo off
::搜索当前目录下的文件
for %%i in (*.*) do echo "%%i"
:: 或者
for /r "目录路径" %%a in (匹配规则，例如*.txt) do 执行操作
pause
```