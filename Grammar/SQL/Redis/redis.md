```sql
--进入客户端(127.0.0.1为主机IP，端口为6379，密码为mypass)
redis-cli
redis-cli -h 127.0.0.1 -p 6379 -a "mypass"

--更换数据库
select 3

--查询所有的key
keys *

--删除指定key
del key

--list 中删除指定元素
lrange data_list 0 10
```