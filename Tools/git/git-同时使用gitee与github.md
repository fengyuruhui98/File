# 对github于gitee进行提交同步
Git同步提交多个仓库，以Mac操作系统为例，其他操作系统搭建对应找一下相关的命令和操作。

首先，将GitHub的仓库clone到本地，比如执行以下命令：
```bash
git clone git@github.com:secbr/shiro.git
```
然后进入本地项目的根目录，在根目录下会有一个.git的隐藏目录。
```bash
192:shiro zzs$ ls .git
COMMIT_EDITMSG	ORIG_HEAD	description	info		packed-refs
FETCH_HEAD	branches	hooks		logs		refs
HEAD		config		index		objects
```
找到.git下面的config文件，通过vi命令进行修改，笔者起初文件内容如下：
```bash
[core]
        repositoryformatversion = 0
        filemode = true
        bare = false
        logallrefupdates = true
        ignorecase = true
        precomposeunicode = true
[remote "origin"]
        url = git@github.com:secbr/shiro.git
        fetch = +refs/heads/*:refs/remotes/origin/*
[branch "main"]
        remote = origin
        merge = refs/heads/main
修改之后变为：

[core]
        repositoryformatversion = 0
        filemode = true
        bare = false
        logallrefupdates = true
        ignorecase = true
        precomposeunicode = true
[remote "origin"]
        // github的仓库地址
        url = git@github.com:secbr/shiro.git
        // gitee的仓库地址
        url = git@gitee.com:secbro/shiro.git
        fetch = +refs/heads/*:refs/remotes/origin/*
[branch "main"]
        remote = origin
        merge = refs/heads/main
```
也就是说，在原来的github仓库地址下面再添加一个url配置，指向gitee的地址。

当然，这里有一个前提条件，Gitee和GitHub的账号的公私钥为同一套。

此时再修改本地代码，进行提交，你会发现GitHub和Gitee上的代码同时被修改了。是不是很cool？