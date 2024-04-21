@echo off
color F0
rem set windows back white and words black
title strcatJAV
rem set windows
echo.
:lab1
echo input AV Number:
set /p AV=
rem input into AV
if "%AV%"=="" (goto lab1)
echo AV:%AV%.
echo ======================================================= >>AV.txt
rem splash
for /f "tokens=1,2 delims=-" %%a in ("%AV%") do (
   set first_part=%%a
   set second_part=%%b
)
echo # [%AV%](./../Fire--/%first_part%/%AV%.mp4)  >>AV.txt
echo ![%AV%][%AV%]  >>AV.txt
echo. >>AV.txt
echo [%AV%]:./Pics/%first_part%/%AV%.PNG  >>AV.txt
echo =======================================================
echo.
echo # [%AV%](./../Fire--/%first_part%/%AV%.mp4)
echo ![%AV%][%AV%] 
echo.
echo [%AV%]:./Pics/%first_part%/%AV%.PNG
echo.
echo =======================================================
echo ======================================================= >>AV.txt
goto lab1
echo The End...&pause>nul