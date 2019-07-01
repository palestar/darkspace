SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBUILD"
SET LINUXBUILDER=builder@192.168.1.5

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

pushd ..\..\
SET BUILD_DIR=%CD%
popd

echo %MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\Medusa\Medusa.sln"
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\Medusa\Medusa.sln" > Logs\Medusa.log
echo "errorlevel: %errorlevel%"
if errorlevel 1 goto :Error

IF "%ACTION%"=="Clean" goto :SkipUnitTest
echo Running Medusa UnitTest...
%BUILD_DIR%\Medusa\Bin\UnitTest.exe > Logs\UnitTest.log
if NOT errorlevel 0 goto :Error
del %BUILD_DIR%\Medusa\Bin\UnitTest*.dmp
:SkipUnitTest


echo %MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\Medusa\Tools\Tools.sln"
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\Medusa\Tools\Tools.sln" > Logs\MedusaTools.log
if errorlevel 1 goto :Error

echo %MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\GameCQ\GameCQ.sln"
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\GameCQ\GameCQ.sln" > Logs\GameCQ.log
if errorlevel 1 goto :Error

echo %MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\DarkSpace\DarkSpace.sln"
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\DarkSpace\DarkSpace.sln" > Logs\DarkSpace.log
if errorlevel 1 goto :Error

echo %MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\DarkSpace\Tools\Tools.sln"
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "%BUILD_DIR%\DarkSpace\Tools\Tools.sln" > Logs\DarkSpaceTools.log
if errorlevel 1 goto :Error

:Done
echo Done...
exit /B 0

:Error
echo Error occured...
exit /B 1


