SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBUILD"

cd C:\Work\Trunk\DarkSpace\Build\
del /Q Logs\*.log
echo Nightly Build Started > Logs\build.log

call GitLatest.bat
if errorlevel 1 goto :Error

call DarkSpaceCleanRelease.bat
if errorlevel 1 goto :Error

call DarkSpaceBuildRelease.bat
if errorlevel 1 goto :Error

echo Alpha Distribution...
%MSBUILD% /p:Configuration=%CONFIG% /t:%ACTION% "..\Distribution\Distribution.sln" > Logs\Distribution.log
if errorlevel 1 goto :Error

echo Promoting Alpha to Beta...
call  PromoteAlphaBeta.bat
if errorlevel 1 goto :Error

:BuildSuccess
sendmail -ini -sub "Build Success!" -body "No errors occurred during the build." -attdir Logs

:Done
echo Done...
exit /B 0

:Error
sendmail -ini -sub "Build Error!" -body "A error occurred during the build, please see the attached logs..." -attdir Logs
echo Error occured...
exit /B 1

rem pause

