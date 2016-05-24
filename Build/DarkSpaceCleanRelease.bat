@echo off

SET CONFIG=Release
SET ACTION=Clean
CALL DarkSpaceBuild.bat
if errorlevel 1 goto :Error

:Done
echo Done...
exit /B 0

:Error
echo Error occured...
exit /B 1



