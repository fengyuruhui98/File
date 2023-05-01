第一，没有把所有的非聚合函数字段加入到group by 中

例如我们要查询一个班级学生的总成绩

select
b.姓名，
sum(c.成绩)，
b.性别，
b.年龄
from
班级表 b,
成绩表 c
where
b.姓名=c.姓名
group by
b.姓名，
b.性别，
b.年龄

在这个sql中的group by中 姓名，性别，年龄都必须在 group by分组里
否则会出现 不是group by 表达式的错误。

第二，在group by 中不能用别名

比如

select
b.姓名 as name，
sum(c.成绩)，
b.性别，
b.年龄
from
班级表 b,
成绩表 c
where
b.姓名=c.姓名
group by
name，
b.性别，
b.年龄

把姓名换成name 也是会出现 不是group by 表达式的错误
把name 改成b.姓名就可以了。