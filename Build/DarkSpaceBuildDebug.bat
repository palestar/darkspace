@echo off

SET CONFIG=Debug
SET ACTION=Build
CALL DarkSpaceBuild.bat
if errorlevel 1 goto :Error

:Done
echo Done...
exit 0

:Error
echo Error occured...
exit 1



