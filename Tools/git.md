# 一、快速设置
HTTPS：`https://gitee.com/blsm14/file.git`  
SSH:`git@gitee.com:blsm14/file.git`  
强烈建议所有git仓库都有一个`README`，`LICENSE`， `.gitignore`等文件  
简单命令：  
```
git config --global user.name "Fomalhaut"
git config --global user.email "zdwzd888@163.com"
```
创建Git仓库
```
mkdir file
cd file
git init 
touch README.md
git add README.md
git commit -m "first commit"
git remote add origin git@gitee.com:blsm14/file.git
git push -u origin "master"
```
已有仓库
```
cd existing_git_repo
git remote add origin git@gitee.com:blsm14/file.git
git push -u origin "master"
```
