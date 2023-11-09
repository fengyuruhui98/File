#!/bin/bash

backup_date=$(date +"%Y-%m-%d")

echo $backup_date

da=$(date +"%d")

echo $da

backup_dir="/home/metro/metro_center/Autobackup"

if [ ! -d "$backup_dir" ]; then
 mkdir $backup_dir
 mkdir $backup_dir/data_$backup_date
fi

cp -R /home/metro/metro_center/datafile/end/ACC/send/$da $backup_dir/data_$backup_date

tar -czvf $backup_dir/data_$backup_date.tar.gz $backup_dir/data_$backup_date

#echo (du -h $backup_dir/data_$backup_date.tar.gz | awk '{print $1}')

backup_size=$(du -h $backup_dir/data_$backup_date.tar.gz | awk '{print $1}')

echo "$backup_date Backup Completed: data_$backup_date.tar.gz {$backup_size}" >> $backup_dir/backup_$da.log

