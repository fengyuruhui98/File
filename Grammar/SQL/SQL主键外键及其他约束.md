# 约束
概念

约束是对表中的数据进行限定，保证数据的正确性、有效性和完整性。
# 约束分类
主键约束：primary key
非空约束：not null
唯一约束：unique
外键约束：foreign key

# 非空约束：not null
- 创建表时添加约束
```sql
CREATE TABLE stu(
id int,
name VARCHAR(20) NOT NULL
);
```
- 创建完表后，添加非空约束
```sql
#创建完表后，添加非空约束
ALTER TABLE stu MODIFY name VARCHAR(20) NOT NULL;
```
- 删除表的非空约束
```sql
#删除name的非空约束
ALTER TABLE stu MODIFY name VARCHAR(20);
```
# 唯一约束：unique
- 创建表时添加约束
```sql
CREATE TABLE stu(
id int,
phone_number VARCHAR(20) UNIQUE  #手机号
);
```
- 创建表完成后添加唯一约束
```sql
#创建表完成后添加唯一约束
ALTER TABLE stu MODIFY phone_number VARCHAR(20) UNIQUE;
```
- 删除表的唯一约束
```sql
#删除表的唯一约束
ALTER TABLE stu DROP INDEX phone_number;
```
注意
- 唯一约束可以有null值，但是一列只能有一条记录为null
# 主键约束：primary key
主键

- 主键字段非空而且唯一
- 一张表只能有一个字段为主键
- 主键是数据库表中记录的唯一标识

操作

- 创建表时添加主键约束
```sql
CREATE TABLE stu(
id int PRIMARY KEY,
name VARCHAR(20)
);
```
- 表创建完成后，添加主键
```sql
#表创建完成后添加主键
ALTER TABLE stu MODIFY id INT PRIMARY KEY;
```
删除表的主键约束
```sql
#删除表的主键约束
ALTER TABLE stu DROP PRIMARY KEY;
```
# 自动增长
如果某一列是数值类型的，使用 auto_increment 可以来完成值的自动增长，自动增长操作通常是和主键在一起使用

操作

- 创建表时添加自动增长
```sql
CREATE TABLE stu(
id int PRIMARY KEY AUTO_INCREMENT,
name VARCHAR(20)
);
```
- 创建表完成后添加自动增长
```sql
#添加自动增长
ALTER TABLE stu MODIFY id INT AUTO_INCREMENT;
```
- 删除自动增长
```sql
#删除自动增长
ALTER TABLE stu MODIFY id INT;
```
注意

- 自动增长通常是和主键一起使用
- 自动增长是根据表的上一条记录的值，进行增长
# 外键约束：foreign key
让表与表产生关闭，从而保证数据的正确性

操作

- 创建表时，添加外键

- 语法：
```sql
create table 表名(
	...
	外键列,
	constraint 外键名称自定义 foreign key (连接外键的列名称) references 主表(主表列)
);
```
- 表创建完成后添加外键
```sql
#添加外键
ALTER TABLE 表名 ADD CONSTRAINT 外键名称自定义 FOREIGN KEY (连接外键的列名称) REFERENCES 主表(主表列);
```
- 删除外键
```sql
#删除外键
ALTER TABLE 表名 DROP FOREIGN KEY 外键名称;
```