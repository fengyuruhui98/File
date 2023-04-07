有两种方法可以实现LPAD的功能。

13位字符串左边补0： 
```sql
right(cast((10000000000000 + "+idx+") as char(14)), 13)
```
```sql
REPEAT('0', 13 - idx.length) || idx
```

