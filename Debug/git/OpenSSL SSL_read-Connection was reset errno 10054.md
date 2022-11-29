问题场景：

有时候在使用 git 命令的时候可能会报这样的错误：

OpenSSL SSL_read: Connection was reset, errno 10054

报错原因：

字面意思：服务器的SSL证书灭有经过第三方机构的签署。

网上信息也有的说可能是网络不稳定，连接超时导致。

解决办法：

1.修改设置，解除SSL验证。打开 命令行工具， 输入：  
```bash
git config --global http.sslVerify "false"
git config --global https.sslVerify "false"
```
2.从 stack overflow 上查到的：

在项目目录下打开命令行工具，输入   git  init ，问题得到有效解决。 