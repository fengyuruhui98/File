@echo off

setlocal enabledelayedexpansion
color F0
title GenSQL
echo.
set dir=%cd%
set filename=insert.txt

echo input tablename (default:dual):
set /p Table=
if "%Table%"=="" (set Table=dual)
echo input colum count (default 1):
set /p colum=
if "%colum%"=="" (set colum=1)
set /a count=%colum%

set "sql=INSERT INTO %Table%("

:lable1
if %count%==0 (
  goto label2
)else (
  set /a count-=1
  echo %count%
  echo the %count%th colum:
  set /p temp=
  set "sql=%sql%%temp%,"
  echo %sql%
  goto lable1
)
:label2

for %%i in (%%i < %colum%) do (
  %%i=%%i+1
  echo input %%i th colum(default systemdate):
  set 
)
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
for /r "%dir%" %%f in (*.sql) do (
	echo CREATE DATABASE %%~nf CHARACTER SET utf8 COLLATE utf8_general_ci; >> create_db.sql
)
 
endlocal