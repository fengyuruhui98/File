# 修改db2数据库表中列的属性--将列的非空属性改为允许空的属性
修改数据表的某一列属性其实很简单但是里面有需要细节需要dba注意，毕竟数据的安全才是最重要的啊！  
db2数据库支持直接使用ddl修改原表列属性，但是在修改之前需要确认要修改的列是否存在唯一性约束，否则你是无法修改属性的。  
注：该操作会导致表处于pending状态，在操作之前需要确认该表是否24小时表，是否为大表（因为需要reorg重置表状态，数据量太大将导致业务中断时间变长），谨慎操作~  
以下是具体操作步骤,敬请参考：  
- 1、首先检查需要修改的列是否含有唯一性检查约束（注：主键不可设置为null）
    - 1）使用db2系统表查询将要修改的表是否含有唯一约束
    #db2 "select CONSTNAME, type  from SYSCAT.TABCONST  where TABNAME='T01'"
    #主要看type，一般type的值有P（主键约束）、U（唯一性约束）、K（列值检查）、F（外键）
    #如果返回的type值中有没有U类型的行则可以直接将原列设置为null然后reorg即可，反之需要继续第二步
    - 2）使用db2look工具确认
    #db2look -d dbname -e -t tabname
    #查看将要修改的表的ddl语句，检查是否有unique子句，如果有这证明有唯一性约束列存在
- 2、如果有唯一性约束且恰好定义在需要修改的列上，我们需要先将该列的唯一性约束删除,如果没有则跳过该步
   #db2 "alter table tabname drop unique CONSTNAME "
   #回退步骤：db2 "alter table tabname add unique(colname)"
- 3、修改列的属性为null
db2 "alter table tabname ALTER colname drop not null"
\#回退步骤：db2 "alter table t01 ALTER colname set not null"
- 4、对该表进行重组
因为修改列的属性后，该表处于reorg pending状态所以我们必须进行reorg才能使该表恢复到正常状态（这一步很重要）
db2 "reorg table tabname use tempsys"
db2 "runstats on table tabname with distribution and detailed indexes all"
- 5、验证
db2 load query table tabname
如果返回表状态为normal则此次操作完成。