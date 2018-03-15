@echo off

cd C:\Work\Trunk\DarkSpace\Build\

rem Stop MirrorServers for Beta builds during rsync below
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9102:MirrorServer - DarkSpace (Beta)"
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9201:MirrorServer - GCQS (Beta)"

..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteAlphaBeta 1
if errorlevel 1 goto :Error
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteBetaExternal 1
if errorlevel 1 goto :Error

:Done
echo Promoted Alpha to Beta...
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9102:MirrorServer - DarkSpace (Beta)" 0
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9201:MirrorServer - GCQS (Beta)" 0
exit /B 0

:Error
echo Error occured while promoting Alpha to Beta...
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9102:MirrorServer - DarkSpace (Beta)" 0
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9201:MirrorServer - GCQS (Beta)" 0
exit /B 1

rem pause

