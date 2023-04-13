# 一、oracle中Blob和Clob类型的区别
## 1.BLOB   
    BLOB全称为二进制大型对象（Binary   Large   Object)。它用于存储数据库中的大型二进制对象。可存储的最大大小为4G字节   
## 2.CLOB   
    CLOB全称为字符大型对象（Character   Large   Object)。它与LONG数据类型类似，只不过CLOB用于存储数据库中的大型单字节字符数据块，不支持宽度不等的字符集。可存储的最大大小为4G字节
    通常像图片、文件、音乐等信息就用BLOB字段来存储，先将文件转为二进制再存储进去。而像文章或者是较长的文字，就用CLOB存储，这样对以后的查询更新存储等操作都提供很大的方便。

EMPTY_BLOB()和EMPTY_CLOB()函数是用来对大数据类型字段进行初始化操作的。
INSERT INTO 表(file_name,file_blob) VALUES ("文件名",EMPTY_BLOB());
或
INSERT INTO 表(file_name,file_blob) VALUES ("文件名",EMPTY_BLOB())RETURNING file_blob INTO dst_file; ----dst_file是与file_blob对应的变量。
select file_name,dbms_lob.getlength(file_blob) si_ze from 表;
EMPTY_CLOB()与EMPTY_BLOB()类同。
```sql
	string sql = "insert into PT_PLAYBACK_CBIINFO values(";
	sql += ":INFO_TYPE<unsigned>, ";						//信息类型
	sql += ":INFO_FUNC<unsigned>, ";						//信息功能
	sql += ":BUREAU_CODE<unsigned>, ";          //局码
	sql += ":STATION_CODE<char[256]>, ";        //站码
	sql += ":INFO_SAVE_TIME<timestamp>, ";      //信息保存时的时间
	sql += "INFO_SAVE_SEQ.nextval, ";     			//信息保存的序列	
	sql += ":INFO_TIME<timestamp>, ";           //信息中的时间
	sql += "empty_blob()) returning INFO_CONTENT into :INFO_CONTENT<blob> ";    // 表示信息内容
```
# 二、Oracle 的Blob使用小结
Oracle的Lobs的流处理方式与Long等对象的Stream方式不一样，没有Long的诸多限制；只要保持连接，就能通过blob对象正确读取对象。
有两种方式可以读取Blob：
1. 直接使用ps.getBinaryStream()的方法得到流对象
2. 使用getBlob得到blob，然后通过blob的方法提供的getBinaryStream(),getBytes() 访问blob的数据。
这两种方法都可以在rs.close之后正确获取数据。(在spring 的JdbcTemplet环境下，该rs理论上被JdbcTemplet自动关闭；从数据库连接来看，连接也正确关闭了)。

使用Blob的好处是，按需获取Blob对象。而且可以多次通过blob.getBinaryStream得到对象。且Blob返回的对象可以使用mark/reset方法反复访问。且连接状态正常。
使用blob得到InputStream，可以调用close()接口，也可以不调用该接口，tb在连接关闭时将自动关闭该连接。最好调用close()释放资源。

c3p0的setBlob(pos,InputStream)接口不能正常工作。

写入或更新Blob时，可以使用ps.setBinaryStream();调用此接口后，in对象到文件尾（在把stream写入blob后，不能要再调用in.close()关闭文件，否则报错）。
也可以使用setBlob(pos,Blob)方法来写入或更新Blob字段；但是注意的是，无论是以blob还是blob.getBinaryStream的方式，都不能自己更新自己，否则死锁。

使用spring读取blob的示例程序：
```java
String sql = "select photo from my_photoes where id='test2' and photo is not null and rownum<2 ";
BLOB blob= (BLOB) simpleDao.queryForObject(sql,Blob.class);       
InputStream in = blob.getBinaryStream();
String filename = "./test/dao/pic" + 1+ ".gif";
BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(filename));
/* 需oracle的BLOB支持。效率可能会高些，但是空间上会有些浪费
byte[] b = new byte[blob.getBufferSize()];                //blob必须为oracle.sql.BLOB时才可调getBufferSize方法； 与java.sql.Blob区别。
System.out.println("bufferSize="+b.length);            //32k左右，用这种方式读取文件会有一点空间的浪费。
int len=-1;
while ((len = in.read(b)) != -1) {
out.write(b);
}
            */

            /*   纯jdbc方法：
                nt b;
     while ((b = in.read()) != -1) {
    out.write(b);
     }
            */

in.close();
out.close();
```
http://hi.baidu.com/hexiong/blog/item/34d7b2b7f9b3e8f431add103.html  其他参考
# 三、BLOB处理遇到的问题：
1. 用spring的模板类来处理blob时，遇到大文件时，流会异常关闭。解决办法，使用oracle的本地连接来获取blob流，如下：
```sql
    public boolean queryForBlobStream(String sql,OutputStream fout)
    {
        boolean flag=true;
        try {
            Connection conn = DataSourceUtils.getConnection(getJdbcTemplate().getDataSource());
            conn.setAutoCommit(false);                        //此部分ms能提高性能
            Statement st = conn.createStatement();
            ResultSet rs = st.executeQuery(sql);
            if (rs.next()) {
                java.sql.Blob blob = rs.getBlob(1);
                InputStream ins = blob.getBinaryStream();
                //输出到文件
                //下面将BLOB数据写入文件
                byte[] b = new byte[1024];
                int len = 0;
                while ((len = ins.read(b)) != -1) {
                    fout.write(b, 0, len);
                }
                //依次关闭
                fout.close();
                ins.close();
            }
            conn.commit();
            rs.close();             //maybe not nessesary
            st.close();             //maybe not nessesary
            conn.close();
        } catch (IOException ex) {
            flag=false;
        } catch (SQLException ex) {
            flag=false;
        }
        return flag;
    }
```
2. 如果把blob对象放到记录的字段中，在web开发中，通过blob.getBinaryStream()只能获得一次blob流，第二次调用同一对象的blob流会得到null流。
且在这种方式下，不能使用in.close()关闭流。
