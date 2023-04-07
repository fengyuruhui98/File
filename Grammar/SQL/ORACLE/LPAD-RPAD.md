# lpad函数
lpad函数是Oracle数据库函数，lpad函数从左边对字符串使用指定的字符进行填充。从其字面意思也可以理解，l是left的简写，pad是填充的意思，所以lpad就是从左边填充的意思。

## 语法
语法格式如下：

```sql
lpad( string, padded_length, [ pad_string ] )
```
string: 准备被填充的字符串；  
padded_length: 填充之后的字符串长度，也就是该函数返回的字符串长度，如果这个数量比原字符串的长度要短，lpad函数将会把字符串截取成从左到右的n个字符;  
pad_string: 填充字符串，是个可选参数，这个字符串是要粘贴到string的左边，如果这个参数未写，lpad函数将会在string的左边粘贴空格。  

## 举例说明
示例1：  
```sql
SQL> select lpad('abcde',10,'x') from dual;
LPAD('ABCDE',10,'X')
--------------------
xxxxxabcde
```
示例2：
```sql
SQL> select lpad('abcde',10,'oq') from dual;
LPAD('ABCDE',10,'OQ')
---------------------
oqoqoabcde
```
示例3：
```sql
SQL> select lpad('abcde',2) from dual;
LPAD('ABCDE',2)
---------------
ab
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```
# 与lpad函数对应的是rpad函数：
 
rpad函数从右边对字符串使用指定的字符进行填充，语法格式与lpad格式相同：
```sql
rpad(string,padded_length,[pad_string])
```
string: 被填充的字符串;  
padded_length: 字符的长度，是返回的字符串的数量，如果这个数量比原字符串的长度要短，rpad函数将会把字符串截取成从左到右的n个字符;  
pad_string: 是个可选参数，这个字符串是要粘贴到string的右边，如果这个参数未写，rpad函数将会在string的右边粘贴空格。  
 
例如：  
复制代码  
```sql
LPAD('tech', 7); -- 将返回'tech'
RPAD('tech', 7); -- 将返回'tech'

RPAD('tech1237', 7); -- 将返回'tech123'
LPAD('tech1237', 7); -- 将返回'tech123'

LPAD('tech', 2); -- 将返回'te'
LPAD('tech', 2); -- 将返回'te'

LPAD('tech', 8, '0'); -- 将返回'0000tech'
RPAD('tech', 8, '0'); -- 将返回'tech0000'

LPAD('tech on the net', 15, 'z'); -- 将返回 'tech on the net'

LPAD('tech on the net', 16, 'z'); -- 将返回 'ztech on the net'
RPAD('tech on the net', 16, 'z'); -- 将返回 'tech on the netz'
```
