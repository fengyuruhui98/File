主要就是增加约束的

以下几种约束 、并 一一列举：

1.主键约束：
要对一个列加主键约束的话，这列就必须要满足的条件就是分空
因为主键约束：就是对一个列进行了约束，约束为（非空、不重复）
以下是代码  要对一个列加主键，列名为id,表名为emp

格式为：
alter table 表格名称 add constraint 约束名称 增加的约束类型 （列名）

例子：
alter table emp add constraint ppp primary key (id)

————————————————————————————————

2.check约束：
就是给一列的数据进行了限制
比方说，年龄列的数据都要大于20的
表名（emp） 列名（age）

格式：
alter table 表名称 add constraint 约束名称 增加的约束类型 （列名）

例子：
alter table emp add constraint xxx check(age>20)

______________________________________________________________________

3.unique约束：
这样的约束就是给列的数据追加的不重复的约束类型

格式：
alter table 表名 add constraint 约束名称 约束类型（列名）
比方说可以给ename列加个unique，让ename列的数据不重复
例子：
alter table emp add constraint qwe unique(ename)

————————————————————————————————

4.默认约束：
意思很简单就是让此列的数据默认为一定的数据

格式：
alter table 表名称 add constraint 约束名称 约束类型 默认值） for 列名

比方说：emp表中的gongzi列默认为10000

alter table emp add constraint jfsd default 10000 for gongzi


————————————————————————————————

5.外键约束：
这个有点难理解了，外键其实就是引用
因为主键实现了实体的完整性，
外键实现了引用的完整性，
应用完整性规定，所引用的数据必须存在！

其实就是个引用，
比方说一个表名称叫dept 里面有2列数据 一列是ID一列是ENAME
id：表示产品的编号
ename：表示产品的名称

另外一个表格名称是emp 里面有2列数据，一列是ID 一列是DID
id:表示用户号
did：表示购买的产品号

要让emp表中的did列去引用dept表中的id

可以用下面的方法

格式：
alter table 表名 add constraint 约束名称 约束类型 (列名) references 被引用的表名称（列名）

例子：
alter table emp add constraint emp_dept_fk foreign key (did) references dept (id)