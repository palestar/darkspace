@echo off

cd C:\Work\Trunk\DarkSpace\Build\

echo Getting latest from SVN...
svn --username builder --password god2899 --non-interactive update ..\..\.
if errorlevel 1 goto :Error

echo Building DarkSpace Installer...
..\Tools\NSIS\makensis.exe ..\DarkSpaceInstall.nsi
if errorlevel 1 goto :Error

echo Commiting binary updates
svn --username builder --password god2899 commit ..\..\. -m "Installer Build"

goto :Done

:Error
echo Error occured...
exit 1

:Done
echo Done...
exit 0

