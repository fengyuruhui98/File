DB2获取指定行数据不像oracle方便，oracle可以使用rownum获取指定行范围内数据。

DB2要实现类似功能，需借助over开窗函数实现。

```sql
-- 获取前n条数据：使用fetch first n rows only方式
SELECT * FROM SYSIBM.SYSTABLES 
WHERE TYPE = 'T'  
FETCH FIRST 10 ROWS only
 
 
-- 获取行范围内数据：使用row_number() over()开窗函数实现
SELECT * FROM
(
    SELECT ROW_NUMBER() OVER() AS id,name,remarks 
    FROM SYSIBM.SYSTABLES 
    WHERE TYPE = 'T'
)
WHERE id BETWEEN 10 AND 20
```