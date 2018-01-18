@echo off

IF "%DEVENV%"=="" SET DEVENV=devenv.exe
IF "%LINUXBUILDER%"=="" SET LINUXBUILDER=builder@vm1.palestar.com 
IF "%BRANCH%"=="" SET BRANCH=work/Trunk/
IF "%CONFIG%"=="" SET CONFIG=Release
IF "%ACTION%"=="" SET ACTION=Build

SET LINUX_CONFIG=%CONFIG%Linux
IF "%ACTION%"=="Build" SET MAKECMD="export BUILD_CONFIGURATION=%LINUX_CONFIG%; mkdir -p %LINUX_CONFIG%/obj; make;"
IF "%ACTION%"=="Clean" SET MAKECMD="export BUILD_CONFIGURATION=%LINUX_CONFIG%; mkdir -p %LINUX_CONFIG%/obj; make clean;"
SET SSH=ssh -p 2200

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

echo Medusa %ACTION%/%CONFIG%...
"%DEVENV%" ..\..\Medusa\Medusa.sln /%ACTION% %CONFIG% /out Logs\Medusa.log
if errorlevel 1 goto :Error

IF "%ACTION%"=="Clean" goto :SkipUnitTest
echo Running Medusa UnitTest...
..\..\Medusa\Bin\UnitTest.exe > Logs\UnitTest.log
if NOT errorlevel 0 goto :Error
del ..\..\Medusa\Bin\UnitTest*.dmp
:SkipUnitTest

echo Medusa Tools %ACTION%/%CONFIG%...
"%DEVENV%" ..\..\Medusa\Tools\Tools.sln /%ACTION% %CONFIG% /out Logs\MedusaTools.log
if errorlevel 1 goto :Error

echo GameCQ %ACTION%/%CONFIG%...
"%DEVENV%" ..\..\GameCQ\GameCQ.sln /%ACTION% %CONFIG% /out Logs\GameCQ.log
if errorlevel 1 goto :Error

echo DarkSpace %ACTION%/%CONFIG%...
"%DEVENV%" ..\DarkSpace.sln /%ACTION% %CONFIG% /out Logs\DarkSpace.log
if errorlevel 1 goto :Error

echo DarkSpace Tools %ACTION%/%CONFIG%...
"%DEVENV%" ..\Tools\Tools.sln /%ACTION% %CONFIG% /out Logs\DarkSpaceTools.log
if errorlevel 1 goto :Error

:Done
echo Done...
exit /B 0

:Error
echo Error occured...
exit /B 1


