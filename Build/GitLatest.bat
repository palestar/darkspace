@echo off

echo Getting latest from GIT...
cd C:\Work\Trunk\DarkSpace\
git fetch origin
if errorlevel 1 goto :Error
git reset --hard 
if errorlevel 1 goto :Error
git checkout develop
if errorlevel 1 goto :Error

cd C:\Work\Trunk\Medusa\
git fetch origin
if errorlevel 1 goto :Error
git reset --hard
if errorlevel 1 goto :Error
git checkout develop
if errorlevel 1 goto :Error

cd C:\Work\Trunk\GameCQ\
git fetch origin
if errorlevel 1 goto :Error
git reset --hard
if errorlevel 1 goto :Error
git checkout develop
if errorlevel 1 goto :Error

:Done
cd C:\Work\Trunk\DarkSpace\Build\
echo Done...
exit /B 0

:Error
sendmail -ini -sub "Build Error!" -body "A error occurred during the build, please see the attached logs..." -attdir Logs
echo Error occured...
exit /B 1

rem pause

