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