# git提交忽略文件或文件夹

在项目根目录下面 添加 .gitignore文件  
文件中每一行表示需要忽略的文件的正则表达式。

.gitignore文件过滤有两种模式，开放模式和保守模式

1. 开放模式负责设置过滤哪些文件和文件夹

例如： /target/ 表示项目根目录下的target文件夹里面所有的内容都会被过滤，不被GIT 跟踪

.classpath 表示项目根目录下的.classpath文件会被过滤，不被GIT跟踪

2. 保守模式负责设置哪些文件不被过滤，也就是哪些文件要被跟踪。

例如：

!/target/*.h 表示target文件夹目录下所有的.h文件将被跟踪

 

如果你不清楚GIT会跟踪哪些文件，你可以先用git status命令查看一下

C代码  
```
$git status  
# On branch master  
# Untracked files:  
#   (use "git add <file>..." to include in what will be committed)  
#  
#    .metadata/  
#    gitignore  
```
 像这样的话，就是.metadata文件夹和gitignore被跟踪。

**另外别忘记了在.gitignore文件中加入一行.gitignore，否则的话.gitignore将会被传到GIT服务器上**

# 规则写法
## 1. 在已忽略文件夹中不忽略指定文件夹
 

/node_modules/*

!/node_modules/layer/

## 2. 在已忽略文件夹中不忽略指定文件
 

/node_modules/*

!/node_modules/layer/layer.js

 

【注意项】注意写法 要忽略的文件夹一定要结尾 /* ，否则不忽略规则将无法生效

## 3. 其他规则写法 (附)
　 以斜杠“/”开头表示目录；


　　以星号“*”通配多个字符；


　　以问号“?”通配单个字符


　　以方括号“[]”包含单个字符的匹配列表；


　　以叹号“!”表示不忽略(跟踪)匹配到的文件或目录；

1、忽略文件
```
*.bak               # 忽略所有扩展名为.bak的文件

!keep.bak      # 但keep.bak文件除外（不会被忽略）

temp/test.txt  # 忽略temp目录下的test.txt文件

temp/*.txt       # 忽略temp目录下所有扩展名为.txt的文件
```
 

2、忽略目录
```
temp/       # 忽略temp目录下的所有目录和文件

temp/*/    # 忽略temp目录下的所有目录，但不会忽略该目录下的文件

```

# 一般配置方法

```
target/
.classpath
.project
.settings/
.springBeans
.metadata/
*.ipr
*.iws
*.iml
.idea/
.idea
../.idea/
*.swp
*.swo
*.swn
*.DS_Store
logs
```
也就是说这些后缀或者是目录下的文件即使修改了也不会被git检查到，不会被提交。

但是，我今天突然想把原先被管理的文件夹变成不被管理，怎么做呢？

1.第一步想到的让然是在这个文件里加上我要屏蔽的文件夹，譬如：xxx/

但是我这么加上之后，执行 git status 并不生效，修改了的文件还是会显示，很奇怪。

2.这个小问题花了我很长时间，可能有一个小时，我一度以为是我写错语法了

后来才发现git已经tracking了某些想忽略的文件，这时候只在文件里加上想忽略的文件夹是不能生效的。可以理解成 有缓存，需要我们自己手动删除已经tracking的文件，执行如下命令：

 git rm -r --cached ignoreFile（ignoreFile就是你想忽略的文件），让git不再tracking这些文件。

然后
git add -A
git commit -m "msg"

就可以啦～