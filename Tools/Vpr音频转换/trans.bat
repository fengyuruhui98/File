for %%i in (*.ncm ^ *.vpr ^ *.kgma ^ *.kgm) do (
echo %%i
start /b um-windows-amd64.exe "%%i"
timeout /t 5 /nobreak
)
pause