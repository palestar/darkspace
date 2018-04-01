@echo off
:start
cls
set /p DEBUG=Start debug executable [y/n]?: 
SET EXENAME=DarkSpaceServer
IF NOT %DEBUG%==y IF NOT %DEBUG%==n	GOTO start
IF %DEBUG%==n	SET EXENAME=%EXENAME%.exe
IF %DEBUG%==y	SET EXENAME=%EXENAME%D.exe


SET CONFIGLOC=.\multiverse\configs

cd ..

:: Deleting old logs first...
del /F /Q .\multiverse\logs\*


:: Loop through config files and start them
FOR %%i in (%CONFIGLOC%\*) DO (
	echo Starting .\Bin\%EXENAME% "%%i"
	start /min .\Bin\%EXENAME% "%%i"
)

:: Kill any servers after we're done
pause > nul| echo Press any button to terminate all servers...
TASKKILL /IM %EXENAME%