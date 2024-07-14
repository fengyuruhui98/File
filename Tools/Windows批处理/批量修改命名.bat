@echo off
setlocal enabledelayedexpansion

set folder=%cd%
set "old_ext=.txt"
set "new_ext=.pdf"

for /R "%folder%" %%a in (*%old_ext%) do (
  set "file=%%~na"
  ren "%%a" "!file!%new_ext%"
)
echo over

pause