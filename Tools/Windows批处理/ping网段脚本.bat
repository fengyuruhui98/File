@echo off
color F0
rem set windows back white and words black
title scanIPPing
rem set windows
echo.
echo input IP default:192.168.16:
set /p IpDuan=
rem input into IpDuan
if "%IpDuan%"=="" (set IpDuan=192.168.16)
rem If IpDuan is NUll192.168.16
echo IP start Enter default1:
set /p QiShi=
rem write into start IP
if "%QiShi%"=="" (set QiShi=1)
rem If start is NULL Default 1
echo IP End Enter default255:
set /p JieShu=
rem write into end IP
if "%JieShu%"=="" (set JieShu=255)
rem if end id null default 255
echo Start:%IpDuan%.%QiShi%  
rem Start IP
echo End Ip:%IpDuan%.%JieShu%  
rem End IP
echo ======================================================= >>Ping-%IpDuan%.txt
rem splash
echo startTime:%date%%time% >>Ping-%IpDuan%.txt
rem write Start Time
echo StartIP:%IpDuan%.%QiShi% >>Ping-%IpDuan%.txt  
rem write Start IP
echo EndIP:%IpDuan%.%JieShu% >>Ping-%IpDuan%.txt 
rem write End IP
echo Scanning...
echo Do Not close Please
@for /l %%n in (%QiShi%,1,%JieShu%) do @ping -w 600 -n 1 %IpDuan%.%%n|find  /i "ttl" >>Ping-%IpDuan%.txt
rem Start Program
echo EndTime%date% %time%  >>Ping-%IpDuan%.txt
rem Write End Time
echo ======================================================= >>Ping-%IpDuan%.txt
rem splash
echo The End...&pause>nul