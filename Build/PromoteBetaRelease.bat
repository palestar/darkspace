@echo off

cd C:\Work\Trunk\DarkSpace\Build\
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteBetaRelease 1
if errorlevel 1 goto :Error
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteReleaseExternal 1
if errorlevel 1 goto :Error

:Done
echo Promoted Beta to Relase...
exit /B 0

:Error
echo Error occured while promoting Beta to Release...
exit /B 1

rem pause

