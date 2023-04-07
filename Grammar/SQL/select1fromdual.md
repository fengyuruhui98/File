# 使用场景DUAL的作用

## 说明

>DUAL是一张虚表，作用就是输出一条记录。  
>SELECT 1 FROM DUAL，这句话的意思是：从DUAL这张表中输出一个常量1。

## 数据库的区别

对于ORACLE  
SELECT必须有表名，所以不能省略FROM xxx。
对于MySQL  
可以省略FROM DUAL（不省略也可以），即：SELECT 1。

使用示例
- 检查数据库是否在线
```sql
SELECT 1 FROM DUAL
```
- 数学计算（本处是取模）
```sql
SELECT MOD(12, 5) FROM DUAL;
```
- 查看当前用户
```sql
SELECT USER() FROM DUAL;
```
- 查看当前时间
```sql
SELECT SYSDATE() FROM DUAL;
```
SpringBoot中的作用
application.yml可以看到如下配置：

spring.datasource.validation-query=SELECT 1 FROM DUAL
这个配置的意思是：使用SELECT 1 FROM DUAL来检查数据库是否有效。