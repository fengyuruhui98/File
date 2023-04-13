# 前言
SQL程序语言有四种类型，对数据库的基本操作都属于这四类，它们分别为；数据定义语言(DDL)、数据查询语言（DQL）、数据操纵语言（DML）、数据控制语言（DCL）

## 数据定义语言（DDL）
DDL全称是Data Definition Language，即数据定义语言，定义语言就是定义关系模式、删除关系、修改关系模式以及创建数据库中的各种对象，比如表、聚簇、索引、视图、函数、存储过程和触发器等等。

数据定义语言是由SQL语言集中负责数据结构定义与数据库对象定义的语言，并且由CREATE、ALTER、DROP和TRUNCATE四个语法组成。比如：
```sql
 --创建一个student表
    create table student(
     id int identity(1,1) not null,
     name varchar(20) null,
     course varchar(20) null,
     grade numeric null
    )


--student表增加一个年龄字段
alter table student add age int NULL


 --student表删除年龄字段，删除的字段前面需要加column，不然会报错，而添加字段不需要加column
alter table student drop Column age


--删除student表
drop table student --删除表的数据和表的结构
truncate table student -- 只是清空表的数据，，但并不删除表的结构，student表还在只是数据为空
```

## 数据操纵语言(DML)


数据操纵语言全程是Data Manipulation Language，主要是进行插入元组、删除元组、修改元组的操作。主要有insert、update、delete语法组成。

```sql
 --向student表中插入数据
    --数据库插入数据 一次性插入多行多列 格式为INSERT INTO table (字段1, 字段2,字段3) VALUES (值1,值2,值3),(值1,值2,值3),...;
    INSERT INTO student (name, course,grade) VALUES ('张飞','语文',90),('刘备','数学',70),('关羽','历史',25),('张云','英语',13);


  --更新关羽的成绩
    update student set grade='18' where name='关羽'


    --关羽因为历史成绩太低，要退学，所以删除关羽这个学生
    delete from student where name='关羽'

```
## 数据查询语言(DQL)


数据查询语言全称是Data Query Language，所以是用来进行数据库中数据的查询的，即最常用的select语句

```sql
  --从student表中查询所有的数据
    select * from student


    --从student表中查询姓名为张飞的学生
    select * from student where name='张飞'
```
## 数据控制语言(DCL)

数据控制语言：Data Control Language。用来授权或回收访问数据库的某种特权，并控制数据库操纵事务发生的时间及效果，能够对数据库进行监视。

比如常见的授权、取消授权、回滚、提交等等操作。

### 1、创建用户

语法结构：

```sql
CREATE USER 用户名@地址 IDENTIFIED BY '密码';
    --创建一个testuser用户，密码111111
    create user testuser@localhost identified by '111111';
```

### 2、给用户授权

语法结构：
```sql
 GRANT 权限1, … , 权限n ON 数据库.对象  TO 用户名;
    --将test数据库中所有对象(表、视图、存储过程，触发器等。*表示所有对象)的create,alter,drop,insert,update,delete,select赋给testuser用户
    grant create,alter,drop,insert,update,delete,select on test.* to testuser@localhost;
```

### 3、撤销授权
```sql
语法结构：

REVOKE权限1, … , 权限n ON 数据库.对象 FORM 用户名;
--将test数据库中所有对象的create,alter,drop权限撤销
revoke create,alter,drop on test.* to testuser@localhost;
```


### 4、查看用户权限

语法结构：
```sql
 SHOW GRANTS FOR 用户名;
 --查看testuser的用户权限
 show grants for testuser@localhost;
```


### 5、删除用户

语法结构：
```sql
DROP USER 用户名;
--删除testuser用户
drop user testuser@localhost;
```

### 6、修改用户密码

语法结构：
```sql
USE mysql;
UPDATE USER SET PASSWORD=PASSWORD(‘密码’) WHERE User=’用户名’ and Host=’IP’;
FLUSH PRIVILEGES;
--将testuser的密码改为123456
update user set password=password('123456') where user='testuser' and host=’localhost’;
FLUSH PRIVILEGES;
```

## 结尾
本文对SQL程序语言有四种操作语言做了一个简单的介绍和概括，对数据库的基本操作都属于这四类，它们分别为；数据定义语言(DDL)、数据查询语言（DQL）、数据操纵语言（DML）、数据控制语言（DCL） 。