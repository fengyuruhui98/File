#!/bin/bash
 
#�ж��ļ����Ƿ�Ϊ��
is_empty_dir(){
    return `ls -A $1 | wc -w`
}
 
#�ݹ����Ŀ¼��������.gitkeep�ļ�
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