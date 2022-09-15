# 解决Faild to connect github.com port 443:connection timed out.
解决方案：先把子切换到全局，然后再取消，接着取消全局代理，最后git pull或者git push。
取消全局代理：
```git
git config --global http.proxy
git config --global https.proxy
```
