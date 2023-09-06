# 1. Failure to find com.oracle:ojdbc6:jar:11.2.0

原因：
Oracle的ojdbc.jar是收费的，所以maven的中央仓库中没有这个资源，只能通过配置本地库才能加载到项目中去。

解决方案：
打开cmd输入

mvn install:install-file -Dfile=D:/ojdbc6-11.2.0.3.jar -DgroupId=com.oracle -DartifactId=ojdbc6 -Dversion=11.2.0.3 -Dpackaging=jar
1
目录是该jar的所在目录，安装时候自动会下载到setting配置下的目录里

需要进行下载ojdbc jar包 下载地址 http://mvnrepository.com/artifact/oracle/ojdbc6/11.2.0.3
