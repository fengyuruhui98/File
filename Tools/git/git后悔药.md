# 1. 强制覆盖本地代码（与远程仓库保持一致）
git强制覆盖：
```sh
git fetch --all
git reset --hard origin/master
git pull
```

git强制覆盖本地命令（单条执行）：
```sh
git fetch --all &&  git reset --hard origin/master && git pull
```
- 第一个是：拉取所有更新，不同步；
- 第二个是：本地代码同步线上最新版本(会覆盖本地所有与远程仓库上同名的文件)；
- 第三个是：再更新一次（其实也可以不用，第二步命令做过了其实）
 
备注： 命令连接符 && 的意思是 前一条命令执行成功才执行后一条命令。  
扩展：命令连接符 ;; 的意思是 不论前一条是否执行成功都继续执行后一条命令。
# 2. 强制更新
当在gitee平台进行更新后，由于设置GitHub先于gitee更新，pull操作时本地版本新于GitHub版本而低于gitee版本，更新失败，需要使用强制更新
```sh
git push -f origin master #强制更新
```
# 3. 本地仓库回退
本地仓库的回退（撤销本地的commit）： git  reset：

reset命令有3种方式：

- git reset --mixed：此为默认方式，不带任何参数的git reset，即时这种方式，它回退到某个版本，只保留源码，回退commit和index信息git 
- reset --soft  HEAD^：回退到某个版本，只回退了commit的信息，不会恢复到index file一级。如果还要提交，直接commit即可
- git reset --hard：彻底回退到某个版本，本地的源码也会变为上一个版本的内容

 

## 1. 回退所有内容到上一个版本  
git reset HEAD^  
## 2. 回退a.py这个文件的版本到上一个版本  
git reset HEAD^ a.py  
## 3. 向前回退到第3个版本  
git reset –-soft HEAD~3  
## 4. 将本地的状态回退到和远程的一样 （[参见1.](#1-强制覆盖本地代码与远程仓库保持一致)） 
git reset –-hard origin/master  

## 5. 回退到某个版本  
git reset 057d  
## 6. 回退到上一次提交的状态，按照某一次的commit完全反向的进行一次commit  
git revert HEAD  
 远程仓库回退操作参见http://www.tech126.com/git-reset/



当更新代码出现问题的时候（可能由于别人提交的代码和我们提交的代码有冲突，或者管理人员修改了我们的代码之后再提交），我们最为常用的办法是。

1.确保分支正确，git branch

2.用git reset 命令恢复到更老的版本

3.git checkout 和 git rm，确保恢复到了想要的老的版本。rm 的内容可以在合代码的时候得到，有冲突的话会提示。

4.git fetch   git merge   ；如果是android的话，repo sync ，疑惑repo sync xxx（某一个git库）

# 4. 修改提交信息
amend翻译成中文：修改

## 1. 修改还未push的提交
```sh
//修改最近一次提交
git commit --amend

//进入注释页面，进行修改

//修改后保存退出
```
## 2. 修改已经push的提交
```sh
//修改最近一次提交
git commit --amend

//进入注释页面，进行修改

//修改后保存退出

//强制推送
git push --force-with-lease origin master
```

# 5. 撤销已经push推送到远端仓库的commit提交信息
有时，在git push之后，才发现还有一些代码需要进行很小的改动，这些改动在原则上不应该作为一次新的提交。
这时，我们需要撤销这次推送(git push)与提交(git commit)，然后进行代码修改，再重新进行提交和推送。

为了实现这个目的，需要进行三步操作。

## 1. 撤销提交信息
首先，通过git log查看提交信息，以便获取需要回退至的版本号：
```sh
$ git log
commit a44822002522f2ab1e3a5e2a1cec00a7d3d02185 (HEAD -> master, origin/master, origin/HEAD)
Author: hanchao5272 <89298141@qq.com>
Date:   Sun Mar 4 11:14:55 2018 +0800

    CSDN-java反射06-成员变量Field-代码优化

commit aa909cff2239536df14820fe086d96305b24e9f1
Author: hanchao5272 <89298141@qq.com>
Date:   Sat Mar 3 23:43:03 2018 +0800

    CSDN-java反射-修饰符-代码优化
```
我们需要撤销CSDN-java反射06-成员变量Field-代码优化这次提交，所以需要回退至的版本是CSDN-java反射-修饰符-代码优化，即需要回退至的版本号是：aa909cff2239536df14820fe086d96305b24e9f1。

然后，通过git reset –soft <版本号>重置至指定版本的提交，达到撤销提交的目的：
```sh
$ git reset --soft aa909cff2239536df14820fe086d96305b24e9f1
```
参数soft指的是：保留当前工作区，以便重新提交
还可以选择参数hard，会撤销相应工作区的修改，一定要谨慎使用

然后，通过git log确认是否成功撤销：
```sh
$ git log
commit aa909cff2239536df14820fe086d96305b24e9f1 (HEAD -> master)
Author: hanchao5272 <89298141@qq.com>
Date:   Sat Mar 3 23:43:03 2018 +0800

    CSDN-java反射-修饰符-代码优化
```
已经成功撤销。

## 2. 撤销
通过git push origin master –force强制提交当前版本号，以达到撤销版本号的目的：
```sh
$ git push origin  master --force
Total 0 (delta 0), reused 0 (delta 0)
To github.com:hanchao5272/myreflect.git
 + a448220...aa909cf master -> master (forced update)
```
必须添加参数force进行强制提交，否则会提交失败，并报错：
```sh
$ git push origin master
To github.com:hanchao5272/myreflect.git
 ! [rejected]        master -> master (non-fast-forward)
error: failed to push some refs to 'git@github.com:hanchao5272/myreflect.git'
hint: Updates were rejected because the tip of your current branch is behind
hint: its remote counterpart. Integrate the remote changes (e.g.
hint: 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```
报错原因：本地项目版本号低于远端仓库版本号。

## 3. 修改代码，重新提交和推送
```sh
//修改代码，添加修改
git add .
//重新提交
git commit -m "CSDN-java反射06-成员变量Field-代码优化"
//重新推送
git push origin master
```
