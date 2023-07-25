# 查询

## 1.查询重复项
1. 查找表中多余的重复记录，重复记录是根据单个字段（peopleId）来判断
```sql
select * from people
where peopleId in (select peopleId from people group by peopleId having count(peopleId) > 1)

Select Name,Count(*) From A Group By Name Having Count(*) > 1

Select Name,sex,Count(*) From A Group By Name,sex Having Count(*) > 1
```
2. 删除表中多余的重复记录，重复记录是根据单个字段（peopleId）来判断，只留有rowid最小的记录
```sql
delete from people 
where peopleId  in (select peopleId  from people group by peopleId having  count(peopleId) > 1)
and rowid not in (select min(rowid) from   people group by peopleId  having count(peopleId )>1)
```

3. 查找表中多余的重复记录（多个字段） 
```sql
select * from vitae a
where (a.peopleId,a.seq) in (select peopleId,seq from vitae group by peopleId,seq having count(*) > 1)
```

4. 删除表中多余的重复记录（多个字段），只留有rowid最小的记录

```sql
delete from vitae a
where (a.peopleId,a.seq) in   (select peopleId,seq from vitae group by peopleId,seqhaving count(*) > 1)
and rowid not in (select min(rowid) from vitae group by peopleId,seq havingcount(*)>1)
```

5. 查找表中多余的重复记录（多个字段），不包含rowid最小的记录

```sql
select * from vitae a
where (a.peopleId,a.seq) in   (select peopleId,seq from vitae group by peopleId,seqhaving count(*) > 1)
and rowid not in (select min(rowid) from vitae group by peopleId,seq havingcount(*)>1)
```
```sql
declare @max integer,@id integer
declare cur_rows cursor local for select 主字段,count(*) from 表名 group by 主字段 having count(*) >； 1
open cur_rows
fetch cur_rows into @id,@max
while @@fetch_status=0
begin
select @max = @max -1
set rowcount @max
delete from 表名 where 主字段 = @id
fetch cur_rows into @id,@max
end
close cur_rows
set rowcount 0
```