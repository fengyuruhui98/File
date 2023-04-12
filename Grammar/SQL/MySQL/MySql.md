```sql
--进入终端
mysql -u root -p
--退出终端
exit

--创建数据库
create database zdd;
--删除数据库
drop database zdd;
--选择数据库
use zdd;
--查看数据库
show databases like '%test%';

--查看数据库IP
select substring_index(host,':',1) as ip, count(*) from information_schema.processlist group by ip;

--执行脚本
source zdd.sql;

--设置允许外部链接
use mysql;
select user, host from user;
update user set host = '%' where user = 'root';
flush privileges;
grant all privileges on *.* to 'root'@'%' identified by '密码' with grant option;
flush privileges;


```