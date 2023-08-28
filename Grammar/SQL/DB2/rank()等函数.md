# DB2中rank(),dense_rank(),row_number()的用法

## 1.准备测试数据

```sql
DROP TABLE oliver_1;
CREATE TABLE oliver_1(CLASS_NO INT,CST_NAME VARCHAR(10),SUB_NO INT,SUB_NAME VARCHAR(20),SCORE int);


INSERT INTO oliver_1 VALUES(1,'张三',30,'数学',89);
INSERT INTO oliver_1 VALUES(1,'王五',20,'语文',90);
INSERT INTO oliver_1 VALUES(1,'Siza',10,'英语',99);
INSERT INTO oliver_1 VALUES(1,'李八',10,'英语',86);
INSERT INTO oliver_1 VALUES(2,'Lisa',20,'语文',50);
INSERT INTO oliver_1 VALUES(2,'Jack',30,'数学',60);
```

## 2.详解rank(),dense_rank(),row_number()的用法

### 2.1 rank()用法

#### 2.1.1不分组进行排序查询

```sql
SELECT rank() over(ORDER BY CLASS_NO,SUB_NO) num,a.* FROM oliver_1 a
``` 

#### 2.1.2对CST_NO分组后查询

SELECT rank() over(PARTITION BY CLASS_NO ORDER BY CLASS_NO,SUB_NO) num,a.* FROM oliver_1 a


 

### 2.2 dense_rank()的用法

SELECT dense_rank() over(ORDER BY CLASS_NO,SUB_NO) AS num,a.* FROM oliver_1 a

区别：dense_rank()的查询与行号没关系，但是rank()的查询与行号相关的。

### 2.3 row_number()的用法

SELECT row_number() over(ORDER BY CLASS_NO,SUB_NO),a.* FROM oliver_1 a
