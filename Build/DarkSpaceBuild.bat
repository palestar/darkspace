@echo off

IF "%DEVENV%"=="" SET DEVENV=devenv.exe
IF "%LINUXBUILDER%"=="" SET LINUXBUILDER=builder@192.168.1.5
IF "%BRANCH%"=="" SET BRANCH=work/Trunk/
IF "%CONFIG%"=="" SET CONFIG=Release
IF "%ACTION%"=="" SET ACTION=Build

SET LINUX_CONFIG=%CONFIG%Linux
IF "%ACTION%"=="Build" SET MAKECMD="export BUILD_CONFIGURATION=%LINUX_CONFIG%; mkdir -p %LINUX_CONFIG%/obj; make;"
IF "%ACTION%"=="Clean" SET MAKECMD="export BUILD_CONFIGURATION=%LINUX_CONFIG%; mkdir -p %LINUX_CONFIG%/obj; make clean;"
SET SSH=ssh -oHostKeyAlgorithms=+ssh-dss -p 2200

echo Linux %ACTION%/%CONFIG%Linux...
%SSH% %LINUXBUILDER%  "cd %BRANCH%Medusa/Medusa; %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%Medusa/Network; %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%Medusa/GCQ;  %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%Medusa/Render3D; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%Medusa/World; %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%DarkSpace/DarkSpace; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%DarkSpace/Gadgets; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%DarkSpace/DarkSpaceServer; %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/ChronDemon; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/GCQDB; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/GCQS; %MAKECMD%" 
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/LogServer; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/MetaServer; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/MirrorServer; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/ProcessServer; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/RotateLogs; %MAKECMD%"
if errorlevel 1 goto :Error
%SSH% %LINUXBUILDER%  "cd %BRANCH%GameCQ/ProcessClientCLI; %MAKECMD%"
if errorlevel 1 goto :Error

pushd ..\..\
SET BUILD_DIR=%CD%
popd

echo "%DEVENV%" "%BUILD_DIR%\Medusa\Medusa.sln" /%ACTION% %CONFIG% /out Logs\Medusa.log
"%DEVENV%" "%BUILD_DIR%\Medusa\Medusa.sln" /%ACTION% %CONFIG% /out Logs\Medusa.log
if errorlevel 1 goto :Error

IF "%ACTION%"=="Clean" goto :SkipUnitTest
echo Running Medusa UnitTest...
%BUILD_DIR%\Medusa\Bin\UnitTest.exe > Logs\UnitTest.log
if NOT errorlevel 0 goto :Error
del %BUILD_DIR%\Medusa\Bin\UnitTest*.dmp
:SkipUnitTest


echo "%DEVENV%" "%BUILD_DIR%\Medusa\Tools\Tools.sln" /%ACTION% %CONFIG% /out Logs\MedusaTools.log
"%DEVENV%" "%BUILD_DIR%\Medusa\Tools\Tools.sln" /%ACTION% %CONFIG% /out Logs\MedusaTools.log
if errorlevel 1 goto :Error

echo "%DEVENV%" "%BUILD_DIR%\GameCQ\GameCQ.sln" /%ACTION% %CONFIG% /out Logs\GameCQ.log
"%DEVENV%" "%BUILD_DIR%\GameCQ\GameCQ.sln" /%ACTION% %CONFIG% /out Logs\GameCQ.log
if errorlevel 1 goto :Error

echo "%DEVENV%" "%BUILD_DIR%\DarkSpace\DarkSpace.sln" /%ACTION% %CONFIG% /out Logs\DarkSpace.log
"%DEVENV%" "%BUILD_DIR%\DarkSpace\DarkSpace.sln" /%ACTION% %CONFIG% /out Logs\DarkSpace.log
if errorlevel 1 goto :Error

echo "%DEVENV%" "%BUILD_DIR%\DarkSpace\Tools\Tools.sln" /%ACTION% %CONFIG% /out Logs\DarkSpaceTools.log
"%DEVENV%" "%BUILD_DIR%\DarkSpace\Tools\Tools.sln" /%ACTION% %CONFIG% /out Logs\DarkSpaceTools.log
if errorlevel 1 goto :Error

:Done
echo Done...
exit /B 0

:Error
echo Error occured...
exit /B 1


