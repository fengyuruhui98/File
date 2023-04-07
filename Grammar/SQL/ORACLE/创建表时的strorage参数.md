通过图表和实例的阐述在Oracle数据库创建新表时Storage的参数具体含义。

可用于：表空间、回滚段、表、索引、分区、快照、快照日志

参数名称|缺省值|最小值|最大值|说明
-|-|-|-|-
INITIAL|5（数据块）|2（数据块）|操作系统限定|分配给Segment的第一个Extent的大小，以字节为单位，这个参数不能在alter语句中改变，如果指定的值小于最小值，则按最小值创建。
NEXT|5（数据块）|1（数据块）|操作系统限定|第二个Extent的大小等于NEXT的初值，以后的NEXT值=前一NEXT大小乘以（1+PCTINCREASE/100），如果指定的值小于最小值，则按最小值创建。如果在alter语句中改变NEXT的值，则下一个分配的Extent将具有指定的大小，而不管上一次分配的Extent大小和PCTINCREASE参数值。
MINEXTENTS|1（Extent）回滚段为2个Extent|1（Extent）回滚段为2个Extent|操作系统限定|Segment第一次创建时分配的Extent数量
MAXEXTENTS根据数据块大小而定|1（Extent）|回滚段为2个Extent|无限制|随着Segment中数据量的增长，最多可分配的Extent数量
PCTINCREASE|50%（Oracle816中为0%）|0%|操作系统限定|指定第三个及其后的Extent相对于上一个Extent所增加的百分比，如果PCTINCREASE为0，则Segment中所有新增加的Extent的大小都相同，等于NEXT的值，如果PCTINCREASE大于0，则每次计算NEXT的值（用上面的公式），PCTINCREASE不能为负数。创建回滚段时，不可指定此参数，回滚段中此参数固定为0。
OPTIMAL|----|不能小于回滚段初始分配空间|操作系统限定|仅与回滚段有关，当回滚段因为增长、扩展而超过此参数的设定范围时，Oracle系统会根据情况动态地重新分配Extents，试图收回多分配的Extent。
FREELISTS|1|1|数据块大小限制|只能在CREATE TABLE、CLUSTER、INDEX中指定FREELISTS和FREELIST GROUPS参数。模式对象中每一个自由列表组中自由列表的数量
FREELIST GROUPS|1|1|取决于Oracle并行实例的数量|用户创建的数据库对象的自由列表组的数量，只有用OPS并行服务器选项时才使用这一参数，一个实例对应一个组。
BUFFER_POOL|----|----|----|给模式对象定义缺省缓冲池（高速缓存），该对象的所有块都存储在指定的高速缓存中，对于表空间或回滚段无效。

　　建议PCTINCREASE参数设置为0，可使碎片最小化，使每一个Extent都相同（等于NEXT值） 
　　一旦建立了某个对象，它的INITIAL和MINEXTENTS参数不能修改（Oracle 816中可修改MINEXTENTS参数） 
　　对于NEXT和PCTINCREASE的任何修改都只影响后来分配的那些Extent 
　　在分配一个新Extent时，系统直接按NEXT的值分配一个Extent， 
　　然后用公式：前一NEXT值*(1+PCTINCREASE/100) 计算出下一个应该分配的Extent的大小， 
　　并把计算结果保存到相关数据字典的NEXT_EXTENT列上，做为下一个应该分配的Extent的大小。 
　　
```sql
　　CREATE TABLE test(a number)
　　STORAGE(
　　INITIAL 100K
　　NEXT 100K
　　MINEXTENTS 2
　　MAXEXTENTS 100
　　PCTINCREASE 100);
　　解释： 
　　初始给test表分配两个Extent， 
　　第一个Extent是100K，因INITIAL=100K； 
　　第二个Extent是100K，因NEXT=100K； 
　　假如因表内数据增长，需要分配第三个Extent，因PCTINCREASE是100，则 
　　第三个Extent是200K=100K+100K； 
　　第四个Extent是400K=200K+200K。 
　　可通过数据字典表DBA_TABLES、ALL_TABLES、USER_TABLES查看参数设置情况，如： 
　　
　　select table_name,initial_extent,next_extent,
　　min_extents,max_extents,pct_increase from user_tables;
　　TABLE_NAME INITIAL_EXTENT NEXT_EXTENT MIN_EXTENTS MAX_EXTENTS PCT_INCREASE
　　---------- -------------- ----------- ----------- ----------- ------------
　　TEST 106496 212992 2 100 100
```