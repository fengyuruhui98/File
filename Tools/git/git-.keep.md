# 简介

本文主要介绍在git中, .gitkeep的作用。

# 作用

git无法追踪一个空的文件夹，当用户需要追踪(track)一个空的文件夹的时候，按照惯例，大家会把一个称为.gitkeep的文件放在这些文件夹里。

# 例程

就个人而言，一般需要.gitkeep地方，是希望完成以下功能:

使git忽略一个文件夹下的所有文件，并保留该文件夹
```sh
# .gitignore 

# ignore all files in lib/
lib/*
# except for .gitkeep
!.gitkeep
# ignore TODO file in root directory,not subdir/TODO
/TODO
```
当.gitignore采用上面的写法时，git会忽略lib文件夹下除了.gitkeep外的所有文件。

# Windows系统下空目录批量添加文件夹

## 1.当前目录下打开gitbash

## 2.执行：
```
find.exe "C:\Users\zhengdong\Desktop\File" -empty -type d -print0 | xargs.exe -0 -I {} touch "{}\.gitkeep"
```

## 3.或者执行脚本
```sh
#!/bin/bash
 
#判断文件夹是否为空
is_empty_dir(){
    return `ls -A $1 | wc -w`
}
 
#递归检查空目录，并创建.gitkeep文件
check_empty_dir()
{
    #echo "Enter --> $1"
    for file in `ls $1`
    do
        if [ -d "$1/$file" ];then
            if is_empty_dir $1"/"$file
            then
                echo "$1/$file is empty,touch $1/$file/.gitkeep"
                touch "$1/$file/.gitkeep"
            else
                #echo "$1/$file is not empty"
                check_empty_dir "$1/$file"
            fi
        fi
    done
}
 
check_empty_dir $1
```