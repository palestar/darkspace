@echo off

cd C:\Work\Trunk\DarkSpace\Build\

rem Stop MirrorServers for Release builds during rsync below
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9100:MirrorServer - GCQ"
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9101:MirrorServer - DarkSpace"
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9103:MirrorServer - Tools"
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini STOP "9200:MirrorServer - GCQS"

..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteBetaRelease 1
if errorlevel 1 goto :Error
..\..\GameCQ\Bin\ProcessClientCLI vm1.palestar.com.ini START PromoteReleaseExternal 1
if errorlevel 1 goto :Error

:Done
echo Promoted Beta to Relase...
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9100:MirrorServer - GCQ" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9101:MirrorServer - DarkSpace" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9103:MirrorServer - Tools" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9200:MirrorServer - GCQS" 1
exit /B 0

:Error
echo Error occured while promoting Beta to Release...
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9100:MirrorServer - GCQ" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9101:MirrorServer - DarkSpace" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9103:MirrorServer - Tools" 1
..\..\GameCQ\Bin\ProcessClientCLI vm3.palestar.com.ini START "9200:MirrorServer - GCQS" 1
exit /B 1

rem pause

