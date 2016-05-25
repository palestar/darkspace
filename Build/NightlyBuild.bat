@echo off

IF "%DEVENV%"=="" SET DEVENV=devenv.exe

cd C:\Work\Trunk\DarkSpace\Build\
del /Q Logs\*.log

call GitLatest.bat
if errorlevel 1 goto :Error

call DarkSpaceCleanRelease.bat
if errorlevel 1 goto :Error

call DarkSpaceBuildRelease.bat
if errorlevel 1 goto :Error

echo Alpha Distribution...
"%DEVENV%" ..\Distribution\Distribution.sln /build Release
if errorlevel 1 goto :Error

echo Promoting Alpha to Beta...
call  PromoteAlphaBeta.bat
if errorlevel 1 goto :Error

:BuildSuccess
sendmail -ini -sub "Build Success!" -body "No errors occurred during the build." -attdir Logs

echo Commiting binary updates
svn --username builder --password god2899 commit ..\..\. -m "Nightly Build" | tee Logs/svn.log
if errorlevel 1 goto :Error

:Done
echo Done...
exit /B 0

:Error
sendmail -ini -sub "Build Error!" -body "A error occurred during the build, please see the attached logs..." -attdir Logs
echo Error occured...
exit /B 1

rem pause

