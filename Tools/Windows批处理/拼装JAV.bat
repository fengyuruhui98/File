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

for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do call set first_part=%%first_part:%%i=%%i%%
set url=https://spic.hotoss.com/dmm/mono/movie/adult/%first_part%%second_part%bod/%first_part%%second_part%bodpl.jpg
echo %url%
curl %url% >I:\%first_part%-%second_part%.PNG -s
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