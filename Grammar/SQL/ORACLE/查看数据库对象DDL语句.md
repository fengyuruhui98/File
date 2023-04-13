当我们想要查看某个表或者是表空间的DDL的时候，可以利用dbms_metadata.get_ddl这个包来查看。 dbms_metadata包中的get_ddl函数详细参数 GET_DDL函数返回创建对象的原数据的DDL语句，详细参数如下 -- object_type ---需要返回原数据的DDL语句的对象类型 -- n

当我们想要查看某个表或者是表空间的DDL的时候，可以利用dbms_metadata.get_ddl这个包来查看。

### dbms_metadata包中的get_ddl函数详细参数
GET_DDL函数返回创建对象的原数据的DDL语句，详细参数如下
-- object_type ---需要返回原数据的DDL语句的对象类型
-- name --- 对象名称
-- schema ---对象所在的Schema，默认为当前用户所在所Schema
-- version ---对象原数据的版本
-- model ---原数据的类型默认为ORACLE
-- transform. - XSL-T transform. to be applied.
-- RETURNS: 对象的原数据默认以CLOB类型返回

### dbms_metadata包中的get_ddl函数定义
FUNCTION get_ddl ( object_type IN VARCHAR2,
name IN VARCHAR2,
schema IN VARCHAR2 DEFAULT NULL,
version IN VARCHAR2 DEFAULT 'COMPATIBLE',
model IN VARCHAR2 DEFAULT 'ORACLE',
transform. IN VARCHAR2 DEFAULT 'DDL') RETURN CLOB;

注意如果使用sqlplus需要进行下列格式化，特别需要对long进行设置，否则无法显示完整的SQL

set linesize 180
set pages 999
set long 90000

### 查看创建用户表的SQL

查看当前用户表的SQL
select dbms_metadata.get_ddl('TABLE','EMPLOYEES') from dual;
查看其他用表或索引的SQL

SELECT DBMS_METADATA.GET_DDL('TABLE','DEPT','SCOTT') FROM DUAL;

### 查看创建用户索引的SQL
查看所需表的索引
SQL> select INDEX_NAME, INDEX_TYPE, TABLE_NAME from user_indexes WHERE table_name='EMP';
查看当前用户索引的SQL
select dbms_metadata.get_ddl('INDEX','PK_DEPT') from dual;

查看其他用户索引的SQL
select dbms_metadata.get_ddl('INDEX','PK_DEPT','SCOTT‘) from dual;
查看创建主键的SQL
查看所需表的约束
SQL> select owner, table_name, constraint_name, constraint_type from user_constraints where table_name='EMP';
查看创建主键的SQL
SELECT DBMS_METADATA.GET_DDL('CONSTRAINT','EMP_PK') FROM DUAL;
查看创建外键的SQL
SQL> SELECT DBMS_METADATA.GET_DDL('REF_CONSTRAINT','EMP_FK_DEPT') FROM DUAL;
### 查看创建VIEW的语句
查看当前用户视图的SQL
SQL> SELECT dbms_metadata.get_ddl('VIEW', 'MY_TABLES')

查看其他用户视图的SQL
SQL> SELECT dbms_metadata.get_ddl('VIEW', 'MY_TABLES','SCOTT‘) FROM DUAL;
查看创建视图的SQL也可以
SQL> select text from user_views where view_name=upper('&view_name');
### DBMS_METADATA.GET_DDL的一些使用技巧
1、得到一个用户下的所有表，索引，存储过程，函数的ddl
SELECT DBMS_METADATA.GET_DDL(U.OBJECT_TYPE, u.object_name)
FROM USER_OBJECTS u
where U.OBJECT_TYPE IN ('TABLE','INDEX','PROCEDURE','FUNCTION‘);
2、得到所有表空间的ddl语句
SELECT DBMS_METADATA.GET_DDL('TABLESPACE', TS.tablespace_name)
FROM DBA_TABLESPACES TS;
3、得到所有创建用户的ddl
SELECT DBMS_METADATA.GET_DDL('USER',U.username)
FROM DBA_USERS U;
4、去除storage等多余参数
EXECUTE DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM,'STORAGE',false);

### 常见错误

SQL> select dbms_metadata.get_ddl('TABLE','PC','SCOTT') from dual;
ERROR:
ORA-19206: Invalid value for query or REF CURSOR parameter
ORA-06512: at "SYS.DBMS_XMLGEN", line 83
ORA-06512: at "SYS.DBMS_METADATA", line 345
ORA-06512: at "SYS.DBMS_METADATA", line 410
ORA-06512: at "SYS.DBMS_METADATA", line 449
ORA-06512: at "SYS.DBMS_METADATA", line 615
ORA-06512: at "SYS.DBMS_METADATA", line 1221
ORA-06512: at line 1

no rows selected

解决办法：运行$ORACLE_HOME/rdbms/admin/catmeta.sql