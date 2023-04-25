1. 重复文件

git add files报错does not have a commit checked out，fatal: adding files failed
原因：在files的子目录中还存在.git的隐藏文件，所以报错
解决办法：删除子目录.git即可，如果有任何 .vs 文件，也将其删除

2. 清除缓存
 
错误描述
在使用git提交的时候出现The file will have its original line endings in your working directory.错误，后来发现 自己再提交的时候忘记输入git add 上传的文件 出现了上面的错误，需要下面进行修改：
 
1 git rm -r -f --cached ./   (删除缓存)
 
2 git add . （添加该目录下所有文件）
 
3 git push -u origin master  （这时候提交就没问题了）
 
初次提交的命令
git init
git add README.md
git commit -m "first commit"
git remote add origin git@github.com:xxxx.git
git push -u origin master

