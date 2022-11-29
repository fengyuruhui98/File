# 基于SSH协议的Git服务

- [基于SSH协议的Git服务](#基于ssh协议的git服务)
- [一、查看git config配置](#一查看git-config配置)
- [二、配置多个git的用户名和邮箱](#二配置多个git的用户名和邮箱)
  - [a、单个配置](#a单个配置)
  - [b、多个配置](#b多个配置)
  - [c、删除某个配置](#c删除某个配置)
- [三、生成多个密钥](#三生成多个密钥)
  - [1、生成gitte仓库的SSH](#1生成gitte仓库的ssh)
  - [2、生成github仓库的SSH](#2生成github仓库的ssh)
- [四、配置多个仓库](#四配置多个仓库)
- [五、增加新的私钥](#五增加新的私钥)
- [六、补充](#六补充)


# 一、查看git config配置
config配置有3个层级：

- system（系统级别）
- global（用户级别）
- local（仓库级别）

一般都会配置用户级别

```
$ git config --global --list 
```
# 二、配置多个git的用户名和邮箱
## a、单个配置
```
$ git config --global user.name "yourusername"
$ git config --global user.email "youremail@email.com"
```
## b、多个配置

账户1生成
```
$ git config user.name "1"
$ git config user.email "1@hotmail.com"
```
账户2
```
$ git config user.name "1"
$ git config user.email "1@hotmail.com"
```
注意： 这里git config命令没有带—global，表示这是一个局部的设置，也就是这个用户是当前项目的，而不是全局的。

## c、删除某个配置
```
$ git config --unset user.name
$ git config --unset user.email
```
# 三、生成多个密钥
配置github和gitee以及公司的git

## 1、生成gitte仓库的SSH
```
ssh-keygen -t rsa -C "\*\*\*@***.com"   
# 生成rsa加密的公钥私钥对,ed25519加密也可
```
>注意：这里的 xxxxx@xxxxx.com 只是生成的 sshkey 的名称，并不约束或要求具体命名为某个邮箱。  
现网的大部分教程均讲解的使用邮箱生成，其一开始的初衷仅仅是为了便于辨识所以使用了邮箱。

按照提示完成三次回车，第一次输入文件名称，可选直接跳过，剩下两项不需要输入即可生成 ssh key。通过查看 ~/.ssh/[文件名].pub 文件内容，获取到你的 public key
```
cat ~/.ssh/id_ed25519.pub
# ssh-ed25519 AAAAB3NzaC1yc2EAAAADAQABAAABAQC6eNtGpNGwstc....
```

## 2、生成github仓库的SSH
```
$ ssh-keygen -t rsa -C "B@hotmail.com"
```
# 四、配置多个仓库
在生成密钥的.ssh 目录下，新建一个config文件，然后配置不同的仓库，如下：
```
# 新建文件
vim config 

# 把下面的内容粘贴到config文件中

# gitee的配置
host gitee.com  # 别名,最好别改
Hostname gitee.com #要连接的服务器
User yuchun.yan #用户名
#密钥文件的地址，注意是私钥
IdentityFile ~/.ssh/gitee_id_rsa  

#github 配置
host github.com #别名
Hostname github.com
User yuchun.yan
IdentityFile ~/.ssh/github_id_rsa
```
如果还有其它仓库，按照这个去配置就可以

# 五、增加新的私钥
使用ssh的ssh-add命令将密钥添加到 ssh-agent 的高速缓存中，方便使用。
```
$ ssh-agent bash
//仓库1的私钥
$ ssh-add ~/.ssh/gitee_id_rsa

//仓库2的私钥
$ ssh-add ~/.ssh/github_id_rsa
```
# 六、补充
 查看项目目录下的.git/config文件，可以找到当前项目的git配置,如下配置：
```
[core]
    repositoryformatversion = 0
    filemode = true
    bare = false
    logallrefupdates = true
    ignorecase = true
    precomposeunicode = true
[remote "origin"]
    url = https://gitee.com/eatingAndCoding/leetcode.git
    fetch = +refs/heads/*:refs/remotes/origin/*
[branch "master"]
    remote = origin
    merge = refs/heads/master
[branch "20190814_twosum_js"]
    remote = origin
    merge = refs/heads/20190814_twosum_js
[branch "20190814-twoSum-js"]
    remote = origin
    merge = refs/heads/20190814-twoSum-js
[user]
    name = mars_yuchun
    email = yuchunextremeli@icloud.com
```
如果你在上面手欠设置了别名的话，就把对应的hostname用别名替代就可以了。

命令：
```
ssh -v 
#v参数：调试模式








