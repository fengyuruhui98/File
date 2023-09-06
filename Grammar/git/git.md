# 一、快速设置
Git是Linus花了两周时间自己用C写了一个分布式版本控制系统  
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
git pull --rebase origin master
```
已有仓库
```
cd existing_git_repo
git remote add origin git@gitee.com:blsm14/file.git
git push -u origin "master"
```
# 二、命令  
文件方式配置，打开文件`.gitconfig`
```
[user]
    email = zdwzd888@163.com
    name = zdd
```
命令方式配置：  
```
git config --global user.name "Fomalhaut"
git config --global user.email "zdwzd888@163.com"
```
这两种配置方式都是一样的，命令的方式配置最终也是修改了配置文件，这里的配置信息只是一个用户标识，不是后面连接远程仓库身份认证用的。  
查看配置:  
>git config --system --list

查看用户配置：  
>git config --global --list
系统配置文件路径：Git\etc\gitconfig (Git 安装目录下)  
用户配置文件路径：C:\Users\哥们并肩走过\.gitconfig（用户目录下.gitconfig）  
## Git版本控制原理：
**Git管理共分为四个区：**  
>Workspace：工作区  
Stage：暂存区，用于存放临时修改的文件  
Repository：本地仓库，所有版本信息都在这里  
Remote：远程仓库，托管代码的服务器（如GitHub，Gitee ）

我们平时写代码后需要先把文件提交到暂存区，然后再从暂存区提交到本地仓库（仓库里存放的就是不同版本的代码）  
**文件总共有四种状态**，分别是
>Untracked （文件未被追踪状态）  
Staged （暂存状态）  
Modified（ 文件已被修改）  
Unmodify（文件未被修改）  

我们先在本地学会使用Git的常用命令，然后再和码云远程仓库连接，把代码文件推送到远程仓库。  

**本地仓库搭建**  

我们新建一个文件夹gitstudy，用于存放我们的代码，现在它就是一个普通的文件夹，鼠标右键点击Git Bash。  
初始化仓库命令：  
```
git init
```
我们接下来新建一个`test.txt`文件，并写入内容：“第一次测试”，这个时候`text.txt`文件是未被追踪的。  
我们可以使用
```
git status
```
命令查看文件状态现在我们把`test.txt`文件添加到暂存区，文件状态就会被修改为暂存状态。  
添加到暂存区命令:  
```
git add test.txt
```  
添加到暂存区之后，文件就是已经可以被追踪的了，如果我们这时候不小心删除`text.txt`文件（永久删除），不要慌，我们可以使用下面的命令恢复文件:
```
git checkout test.txt
```
接下来，我们将暂存区的文件添加到本地仓库，执行命令
```
git commit test.txt -m "first commit"
```
-m 参数代表的意思是提示信息，比如我们对文件做了哪些改动。   
我们已经在提交了一个版本到仓库中了，接下来我们修改test.txt的内容为"第二次测试"，提交到暂存区，再添加到本地仓库。接着修改test.txt的内容为"第三次测试"，提交到暂存区，再添加到本地仓库。  
现在我们的本地仓库有三个版本的text.txt了，内容分别是“第一次测试”，“第二次测试”，“第三次测试”

查看版本信息
```
git log
```
现在可以很清楚地看到我们一开始配置的用户信息，如果是有多个人开发一个项目，我们就可以很清楚地知道这个版本是由谁提交的。  
git log后面还可以跟一些参数。
```
git log master --all --graph
```
>master 代表主分支  
–all 代表所有版本  
–graph 代表图形化展示  

关于分支，后面我们会讲到，别急。我们现在只有一条分支就是master。
版本回滚，我们最后提交到仓库是，text.txt文件的内容为"第三次测试"，我们回滚到第一个版本。  
可以先查看一下text.txt内容，再执行下面的命令。  
```
git reset --hard 4df36347dbb6d6e8c2af38272592e5635781c7f8
```
`4df36347dbb6d6e8c2af38272592e5635781c7f8` 是第一次提交的版本号，相当于一个身份证号，是唯一的。  
查看版本信息也可以通过下面的命令:  
```
git reflog
```
现在我们test.txt文件的内容为"第一次测试"  
现在我们对test.txt文件进行修改，修改为：“第一次测试,正在学习git…”（仅仅修改，并没有提交到暂存区）  
git diff test.txt 命令可以查看我们修改的内容  
```
git diff test.txt
```
如果这时候，我们想回到修改前，可以执行下面这条命令
```
git checkout test.txt
```
还记得？git checkout test.txt命令还能够恢复能被删除的文件

下面，我们先来总结一下，我们学了哪些git命令
>git init #初始化一个仓库  
git add test.txt #把文件添加到暂存区  
git commit test.txt -m "first commit" #把暂存区的文件提交到本地仓库  
git log master --all --graph #查看版本信息  
git reflog #另一种查看版本信息的方法  
git reset --hard 4df36347dbb6d6e8c2af38272592e5635781c7f8 #版本回滚  
git diff test.txt # 查看文件做了哪些修改  
git checkout test.txt #恢复文件  

下面我们重新初始化一个仓库介绍Git的分支管理  
删除原来的文件，gitstudy又变成了一个普通的文件夹了，空空如也

## Git版本控制
初始化一个仓库，新建test.txt文件，并写入"master第一次提交"，提交到本地仓库之后，修改test.txt文件内容为"master第二次提交"。再次提交到本地仓库，再次修改test.txt文件内容为"master第三次提交"并提交到本地仓库。  
现在我们在master分支上提交了三个版本啦，如下  
接下来，我们创建一条新的分支，名字就叫dev  
命令如下：
```
git branch dev
```
查看所有分支
```
git banch
```
这样我们就创建了一个新的分支dev，master前面的*代表当前分支  
切换到dev分支  
```
git switch dev
```
我们现在在dev分支上了，接下来，我们修改test.txt的内容为：“dev分支第一次提交”，并提交到本地仓库，不同分支上工作是互不影响的。  
目前，我们在master分支提交了三次，在dev分支提交了一次    
查看提交版本日志  
现在，我们切换到master分支上来，切换到master分支
```
git switch master
```
把dev分支上的修改合并到master分支上
```
git merge dev
```
删除dev分支
```
git branch -d dev
```
可以查看一下test.txt文档的内容，内容变为了"dev分支第一次提交"  
接下来，介绍第二种合并分支的方式。  
我们现在只有一个分支master，我们还是创建一个新的分支dev    
这里介绍一种快速创建分支并切换到新分支的命令
```
git switch -c dev
```
修改test.txt的内容为"dev分支第二次提交"，重新提交到仓库，并切换回master分支  
合并dev分支的另一种方式，加上--no-ff
```
git merge --no-ff -m "merge with no-ff" dev
```
加上--no-ff 参数可以很清晰地看到合并的信息

我们现在有master 和 dev两个分支，我们切换回dev分支
```
git switch dev
```
新建一个feature分支
```
git switch -c feature
```
现在我们三个分支分别是 master - dev - feature  
我们现在feature分支上工作，修改test.txt的内容为"feature分支第一次提交"，添加到暂存区并提交到本地仓库。切换回dev分支  
我们可以把feature分支往dev分支上合并，但是我们现在不想合并且想删除feature分支，如果直接删除。会删除失败，因为feature分支上有我们的提交的版本，没有合并到dev分支上来。  
直接删除feature分支  
如果我们想强制删除，根据提示，我们采用git branch -D feature可以强制删除feature分支  
现在feature分支已经被我们删除了，如果我们删除之后，又反悔了，怎么办。不要慌，git总是有后悔药，我们可以恢复刚刚被删除的feature分支  
我们需要知道，刚刚在feature分支提交的版本号，这个可以用git reflog命令查看，这个命令我们之前学过的。  
恢复删除的分支，执行下面的命令
```
git branch feature <hash_val>
```
现在我们又有三个分支了，分别是master - dev - feature  
现在我们处于dev分支上，我们现在来修改text.txt为“dev第三次提交”，但是并没有提交到暂存区，我们现在需要去处理master分支上版本的出现一个bug，如果我们现在直接切换到master分支上，我们修改的内容就没有保存了。我们可以先把我们的作业冻结起来，修复完bug之后再来唤醒它。  
冻结当前的作业，保护现场
```
git stash
```
切换到master分支
```
git switch master
```
创建一个新的分支bug，并切换到bug分支
```
git switch -c bug
```
修改test.txt的内容为：“bug修复”，提交到暂存区之后提交到本地仓库  
切换回master分支，合并bug分支，还记得如何合并？
```
git merge --no-ff -m "merge with no-ff" bug
```
删除bug分支
```
git branch -d bug
```
现在我们需要回到dev分支上开展我们的工作了，切换到dev分支。  
查看冻结的内容
```
git stash list
```
唤醒并删除stash内容
```
git stash pop
```
我们冻结的作业就是保存在stash区，所以我们可以在stash区恢复没有提交到暂存区的文件。  
我们刚才修复了一个bug，那么这个bug在dev分支上的版本也是存在的，我们需要修复它，怎么办呢。还记得我们刚才在bug分支上提交的版本？内容为“bug修复”  
在dev分支上快速修复bug  
```
git cherry-pick 
```
版本号为在bug分支上提交的版本号，可以使用git reflog命令查看    
可以看到出错了，为什么呢？我们现在都只是基于test.txt一个文件做的修改，修复bug就意味着改变test.txt的内容，所以这次修复会冲突。根据提示，我们需要手动解决合并的冲突。  
test.txt文件的内容现在如下：  
打开test.txt文件的内容内修改为" dev第三次提交 + bug修复"
添加到暂存区
```
git add .
```
执行提交命令
```
git commit -m "dev three commit and fix bug"
```
我们先来做个总结，然后学习本篇文章介绍的最后一个git命令-rebase，然后就和我们的远程仓库进行交互了。
## 总结
```shell
git init #初始化一个仓库
git add test.txt #把文件添加到暂存区
git commit test.txt -m "first commit" #把暂存区的文件提交到本地仓库
git log master --all --graph #查看版本信息
git reflog #另一种查看版本信息的方法
git reset --hard 4df36347dbb6d6e8c2af38272592e5635781c7f8 #版本回滚
git diff test.txt # 查看文件做了哪些修改
git checkout test.txt #恢复文件
git branch dev #创建分支
git banch #查看所有分支
git switch dev # 切换到dev分支
git merge dev #合并dev分支
git merge --no-ff -m "merge with no-ff" dev #合并dev分支,可查看合并的历史
git branch -d dev #删除分支
git switch -c dev #创建并删除分支
git branch feature <hash_val> # 恢复删除的分支
git stash #冻结作业，保存在stash区
git stash list #查看stash内容
git stash pop #恢复并删除stash内容
git cherry-pick <hash_val> #合并分支,如果有冲突，需要手动解决冲突，再提交
git push -u origin master
git push -f origin master #强制更新
git pull --rebase origin master
```
