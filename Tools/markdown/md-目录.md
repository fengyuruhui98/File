# 目录设置

手写目录
手动添加目录的方法就是将「列表」和「页内超链接」相结合。（与HTML的锚点链接类似）

用法：
[目录名](#标题链接)

规则：

标题链接格式为： 一个 # + 被链接标题
标题链接中不能出现大写字母，大写字母用小写字母代替
标题链接中不能出现空格  ，空格  用 - 代替。
note:

目录排布由 有序列表 或 无序列表 控制
跳转与 目录名 无关，标题 和 标题链接 符合规则即可
栗子：

// 目录

* [跳到1. 这是一级标题](#1-这是一级标题)
  * [跳到1.1. 这是二级标题 1-1](#11-这是二级标题-1-1)
  * [跳到1.2. 这是大写字母标题 QWER](#12-这是大写字母标题-qwer)
    * [跳到1.2.1. 这是 空格测试标题](#121-这是-空格测试标题)

// 标题

# 1. 这是一级标题

## 1.1. 这是二级标题 1-1

## 1.2. 这是大写字母标题 QWER

### 1.2.1. 这是 空格测试标题

// 预览效果如下：
跳到1. 这是一级标题
跳到1.1. 这是二级标题 1-1
跳到1.2. 这是大写字母标题 QWER
跳到1.2.1. 这是 空格测试标题
1. 这是一级标题
1.1. 这是二级标题 1-1#
1.2. 这是大写字母标题 QWER#
1.2.1. 这是 空格测试标题#
