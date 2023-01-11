# 背景信息
在Git客户端中，提交文件时，如果文件名是中文，会出现乱码。

## 操作步骤
避免中文文件名出现乱码的几种提交场景操作方法如下表所示。

## 1. 在Git客户端中，使用git add添加要提交的文件时，如果文件名是中文，会显示形如“256\346\200\273\347\273\223.png”的乱码。

在bash提示符下输入：
```sh
git config --global core.quotepath false
```
## 2. 使用git log提交显示中文Log乱码。

1. 设置git gui的界面编码：
```SH
git config --global gui.encoding utf-8
```
2. 设置commit log提交时使用“utf-8”编码，可避免服务器上乱码，同时与Linux上的提交保持一致：
```sh
git config --global i18n.commitencoding utf-8
```
3. 使得在$ git log时将“utf-8”编码转换成“gbk”编码，解决Msys bash中git log乱码：
```sh
git config --global i18n.logoutputencoding gbk
```
4. 使得git log可以正常显示中文（配合“i18n.logoutputencoding” = “gbk”)，在“/etc/profile”中添加：
```sh
export LESSCHARSET=utf-8
```
## 3. 在MsysGit自带的bash中，使用ls命令查看中文文件名乱码，cygwin没有该问题。

1. 使用ls --show-control-chars命令来强制使用控制台字符编码显示文件名，即可查看中文文件名。
为了方便使用，可以编辑“/etc/git-completion.bash”，新增一行：
```sh
alias ls="ls --show-control-chars"
```
2. 重启Git客户端即可生效。