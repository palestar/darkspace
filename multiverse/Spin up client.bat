@echo off
:debug
cls
SET /p DEBUG=Start debug executable [y/n]?: 
SET EXENAME=DarkSpaceClient
IF NOT "%DEBUG%"=="y" IF NOT "%DEBUG%"=="n" GOTO debug
IF "%DEBUG%"=="n" SET EXENAME=%EXENAME%.exe
IF "%DEBUG%"=="y" SET EXENAME=%EXENAME%D.exe

SET /p ADDRESS=What's the server ip [press enter to default to localhost]?:
IF "%ADDRESS%"=="" SET ADDRESS=localhost

SET /p PORT=What's the server port [press enter to default to 9020]?: 
IF "%PORT%"=="" SET PORT=9020
cls
ECHO Connecting to %ADDRESS%:%PORT%...

SET /p USERNAME=Enter your DarkSpace username: 
SET /p PASSWORD=Enter your DarkSpace password: 
cls
cd ..

:: Start client...
start .\Bin\%EXENAME% -connect %ADDRESS% %PORT% -login %USERNAME% %PASSWORD%