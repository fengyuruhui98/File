#!/bin/bash
program='metro_center'
#rm -rf watchdog.log
watchdoglog='./watchdog.log'
while(true)
do 
  now=`date '+%Y-%m-%d %H:%M:%S'`
  count=`ps -ef|grep $program |grep -v grep` 
  if [ "$?" != "0" ];then
    echo "$now : $program process not exists, will call" > "$watchdoglog"
    cd /home/metro/metro_center
    ./ccd start
    echo "$now : $program is running" > "$watchdoglog"
    exit
  else 
    echo "$now : $program process is running" > "$watchdoglog"
  fi 
  sleep 5
done
