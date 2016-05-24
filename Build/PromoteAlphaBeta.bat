@echo off

cd C:\Work\Trunk\DarkSpace\Build\
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteAlphaBeta 1
if errorlevel 1 goto :Error
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteBetaExternal 1
if errorlevel 1 goto :Error

:Done
echo Promoted Alpha to Beta...
exit /B 0

:Error
echo Error occured while promoting Alpha to Beta...
exit /B 1

rem pause

