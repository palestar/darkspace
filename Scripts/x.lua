

-- ########################################################################
-- ###  Constants
-- ###

-- Standard Functions

-- Faction
F_NEUTRAL   = 0;
F_UGTO      = 1;
F_ICC       = 2;
F_KLUTH     = 3;
F_ICC_UGTO  = 4;
F_UNKNOWN   = 5;

-- remoteCall
RC_LOCAL        = 0;
RC_GLOBAL       = 1;
RC_TEAM         = 2;
RC_FACTION      = 3;
RC_PRIVATE      = 4;
RC_SERVER       = 5;
RC_NOT_FACTION  = 6;


-- Interface Functions

-- sendChat
SC_LOCAL        = 0;
SC_GLOBAL       = 1;
SC_TEAM         = 2;
SC_FACTION      = 3;
SC_PRIVATE      = 4;
SC_SERVER       = 5;
SC_NOT_FACTION  = 6;

-- Planet Functions

-- PlanetFlags
PF_HYDROGEN         = 1;
PF_OXYGEN           = 2;
PF_METALS           = 4;
PF_HEAVY_METALS     = 8;
PF_CYRO_METALS      = 16;
PF_HYPER_MATTER     = 32;
PF_DARK_MATTER      = 64;
PF_URDANIAM         = 128;
PF_PLAGUE           = 256;
PF_REVOLUTION       = 512;
PF_RESESSION        = 1024;
PF_STRIKE           = 2048;
PF_BOOM             = 4096;
PF_SMUGGLERS        = 8192;
PF_MINING_FIND      = 16384;
PF_FOOD_SHORT       = 32768;
PF_POWER_SHORT      = 65536;
PF_WORKER_SHORT     = 131072;

-- planetEvent
PE_PLAGUE           = 0;
PE_REVOLUTION       = 1;
PE_RESESSION        = 2;
PE_STRIKE           = 3;
PE_BOOM             = 4;
PE_SMUGGLERS        = 5;
PE_MINING_FIND      = 6;
PE_FOOD_SHORT       = 7;
PE_POWER_SHORT      = 8;
PE_WORKER_SHORT     = 9;


-- Ship Functions

-- ShipType
ST_FIGHTER      = 0;
ST_SCOUT        = 1;
ST_FRIGATE      = 2;
ST_DESTROYER    = 3;
ST_CRUISER      = 4;
ST_DREADNAUGHT  = 5;
ST_ENGINEER     = 6;
ST_SUPPLY       = 7;
ST_TRANSPORT    = 8;
ST_STATION      = 9;
ST_PLATFORM     = 10;
ST_UNKNOWN      = 11;


-- orderShip
OS_NOORDER      = 0;
OS_ATTACK       = 1;
OS_DEFEND       = 2;
OS_CAPTURE      = 3;
OS_MOVE         = 4;
OS_RELOAD       = 5;
OS_BEACON       = 6;
OS_HOLD         = 7;


-- Gadget Functions

-- GadgetType
GT_UNKNOWN              = 0;
GT_DRIVE                = 1;
GT_SPECIAL              = 2;
GT_HEAVY_SPECIAL        = 3;
GT_JUMP_DRIVE           = 4;
GT_WEAPON               = 5;
GT_WEAPON_HEAVY         = 6;
GT_WEAPON_BEAM          = 7;
GT_WEAPON_HEAVY_BEAM    = 8;
GT_BOMB                 = 9;
GT_HEAVY_BOMB           = 10;
GT_ARMOR                = 11;
GT_FIGHTER_BAY          = 12;
GT_DRONE_BAY            = 13;
GT_ORB                  = 14;
GT_CLOAK                = 15;
GT_HULL                 = 16;

-- GadgetMount
GM_FRONT    = 0;
GM_RIGHT    = 1;
GM_LEFT     = 2;
GM_BACK     = 3;
GM_FULL     = 4;


-- Mission Functions

-- Mission Types
MT_INVALID  = -1;
MT_SCOUT    = 0;	    -- move within 500gu of objective[0] / SCOUT, FRIGATE
MT_BEACON   = 1;	    -- Attach beacon to objective[0] / SCOUT
MT_DESTROY  = 2;	    -- Destroy objective[0] / DESTROYER, CRUISER, DREAD
MT_ESCORT   = 3;	    -- Escort objective[0] to objective[1] / DESTROYER, CRUISER, DREAD
MT_CAPTURE  = 4;	    -- Capture objective[0] / TRANSPORT
MT_DEFEND   = 5;	    -- Defend objective[0] / DESTROYER, CRUISER, DREAD
MT_TRANSFER = 6;	    -- Move objective[0] to objective[1] / ALL
MT_AQUIRE   = 7;	    -- Find and load objective[0] / ALL
MT_ORBIT    = 8;	    -- Orbit objective[0] / ALL
MT_REPAIR   = 9;	    -- Repair objective[0] / SUPPLY
MT_SCRIPT   = 10;	    -- SCRIPT / ALL
MT_WAIT     = 11;	    -- Wait until objective[0] is within 100gu / ALL
MT_SLEEP    = 12;	    -- Wait for duration to expire / ALL
MT_GOTO     = 13;	    -- Get within 100gu of objective[0] / ALL

-- Mission Trigger
TR_INITIALIZE   = 0;
TR_ASSIGNED     = 1;
TR_ABORTED      = 2;
TR_COMPLETED    = 3;
TR_FAILED       = 4;

-- Mission State
MS_INACTIVE     = 0;
MS_ACTIVE       = 1;
MS_FAIL         = 2;
MS_COMPLETE     = 3;




Util = {};

-- just for testing, delete in final version
--function onClientSide()
--    Util:testShowMenu()
--end

-- ########################################################################
-- ###  Testcases
-- ###

--================ RandomText + Formatspecial =============================
function Util:testGetRandomText()
    local testcase = { "This $s1 a $s2..."
                ,"A $s2 of patience this $s1, ...so says Yoda."
                ,"$s2ing 1,2,3..."
                ,"$s1 this working ?"
                ,"This $s1 a triple $s2 $s2 $s2..."
                ,"This one has nothing to replace at all"
               };

    for i = 1, 5 do
        scriptAlert( self:getRandomText( testcase, "is", "test" ));
    end
end

--================ selectNumber ===========================================
function Util:testSelectNumber()
    pushChat("Press a numberkey from 0 to 9...");
    self:selectNumber( "Util:testSelectNumberOutput" );
end

function Util:testSelectNumberOutput( aNumber )
    assert( type( aNumber ) == "number", "choice was not a number" );
    pushChat( "You pressed " .. aNumber ..".");
end

--================ getFactionName =========================================
function Util:testGetFactionName()
    pushChat("Listing all known factions:");
    for i = 0, 5 do
        pushChat( i .. ": " .. self:getFactionName( i ) );
    end

    pushChat("\n3 random planets:");
    for i = 1, 3 do
        local pId = getRandomPlanet();
        pushChat( getName( pId ) .. " is a " .. self:getFactionName( getFaction( pId ) ) .. " planet." );
    end
end

--================ getShipTypeName =========================================
function Util:testGetShipTypeName()
    pushChat("Listing all known ship-types:");
    for i = 0, 11 do
        pushChat( i .. ": " .. self:getShipTypeName( i ) );
    end

    pushChat("\n3 random ships:");
    for i = 1, 3 do
        local sId = getShip( randomNumber( 0, shipCount() - 1 ) );
        pushChat( getName( sId ) .. " is a " .. self:getShipTypeName( getShipType( sId ) ) );
    end
end

--================ showMenu ===============================================
function Util:testShowMenu()
    self.tempNum = 0;
    self:testShowMenuRedraw();
end

function Util:testShowMenuRedraw()
    local tblMenu = { "Current planet " .. getName( getPlanet( Util.tempNum ) ) .. " is " .. self:getFactionName( getFaction( getPlanet( Util.tempNum ) ) ) ,
                      "Next planet"     , "Util:testShowMenuNP"    ,
                      "Previous planet" , "Util:testShowMenuPP"    ,
                      "Get position"    , "Util:testShowMenuGPP"   ,
                      "Get planetValue" , "Util:testShowMenuGPV"   ,
                                          "Util:testShowMenuExit"
                    };

    self:showMenu( tblMenu );
end

function Util:testShowMenuNP()
    self.tempNum = self.tempNum + 1
    if self.tempNum == planetCount() then
        self.tempNum = 0;   -- no modulo function available.
    end
    self:testShowMenuRedraw();
end

function Util:testShowMenuPP()
    self.tempNum = self.tempNum -1;
    if self.tempNum < 0 then
        self.tempNum = planetCount() - 1;
    end
    self:testShowMenuRedraw();
end

function Util:testShowMenuGPP()
    self:testShowMenuRedraw();
    local x,y,z =  getPosition(getPlanet( self.tempNum ));
    pushChat( "X:"..x..", Y:"..y..", Z:"..z );
end

function Util:testShowMenuGPV()
    self:testShowMenuRedraw();
    pushChat( "Value: " .. getPlanetValue(getPlanet( self.tempNum )) );
end

function Util:testShowMenuExit()
    pushChat("Bye bye...");
    self.tempNum = nil;         -- "remove" the var
end




-- ########################################################################
-- ###  Utility functions
-- ###


-- uses formatSpecial to return a generated random string
-- from the passed table which includes several textlines in $s1 $s2 style
-- example: getRandomText( {"1$s2 $s1", "3$s2 $s1 $s1 $s1" }, "test", "x" )
-- returns "1x test" or "3x test test test"
function Util:getRandomText( tblText, ... )
    return self:formatSpecial( tblText[randomNumber(1, getn( tblText ) )], arg );
end


-- Replaces each occurance of $s1, $s2 etc with the corresponding parameter.
-- An argument placeholder for an parameter may occur zero, one or multiple times.
-- example: formatSpecial( "This $s1 a triple $s2 $s2 $s2...", { "is", "test", "nothing" } );
-- returns: "This is a triple test test test..."
function Util:formatSpecial( sText, params )
    local tblTemp = {};
    --local inline function (o:
    function tblTemp:funcReplace( sPlaceholder )
        sPlaceholder = self;    -- bad design, but required to keep the function local
        local num = tonumber( sPlaceholder );
        if num <= getn( %params ) then
            return %params[num];
        else
            assert( nil, "Not enough parameters for text replacement" );
        end
    end
    return gsub( sText, "%$s(%d+)", tblTemp.funcReplace );
end



-- Traps all number keys ( not keypad ) and places onSelectNumber as
-- event handler.
function Util:selectNumber( aCallbackFunc )
    for i = 0, 9 do
        pushChat( i );
        trapKey( i+48, "Util:onSelectNumber(\"" .. aCallbackFunc .. "\", " .. i .. ")" );
    end
end


-- Untraps all trapped numberkeys and calls the given callback routine
-- with the pressed number key as parameter.
function Util:onSelectNumber( aCallbackFunc, aNumber )
    for i = 48, 57 do
        trapKey( i );
    end
    dostring(aCallbackFunc .. "(" .. aNumber .. ")");
end


-- Returns the name for the factionnumber entered, valid is 0 to 5.
function Util:getFactionName( iFactionNumber )
    local tblFac = { "UGTO", "ICC", "K'Luth", "ICC/UGTO Alliance", "unknown faction" };
    tblFac[0] = "neutral";  -- table index starts at 1, so add this here.
    return tblFac[iFactionNumber];
end


-- Returns the name for the shipType entered, valid is 0 to 11.
function Util:getShipTypeName( iShipType )
    local tblType = { "Scout", "Frigate", "Destroyer", "Cruiser", "Dreadnaught", "Engineer", "Supply", "Transport", "Station", "Platform", "unknown ship" };
    tblType[0] = "Fighter";  -- table index starts at 1, so add this here.
    return tblType[iShipType];
end


-- displays a menu and calls the according callbacks, exit is always "0"
-- tblMenu format is: "Title", "menuItem1", "Callback1", "menuItem2", "Callback2", ... "callBackExit"
function Util:showMenu( tblMenu )
    --flushChat();                        -- clear chatscreen

    local sMenu = tblMenu[1] .. "\n" ;             -- print title
    for i = 2, getn(tblMenu)-1, 2 do
        if tblMenu[i] then
            sMenu = sMenu .. "<color;00ff00>" .. (i/2) .. ")</color> " .. tblMenu[i] .. "\n"; -- print each menu item
            trapKey( 48 + (i/2), "Util:showMenuOnSelect(\"" .. tblMenu[i+1] .. "\")" ); -- and trap itªs key
        end;
    end;
    sMenu = sMenu .. "\n<color;00ff00>0)</color> Exit";
    trapKey( 48, "Util:showMenuOnSelect(\"" .. tblMenu[getn(tblMenu)] .. "\")" ); -- and trap itªs key
    pushChat( sMenu ); -- print the menu
end

function Util:showMenuOnSelect( aCallbackFunc )
    for i = 48, 57 do
        trapKey( i );  -- untrap all keys
    end
    dostring(aCallbackFunc .. "()"); -- call the appropriate function
end


-- just for testing, delete in final version
--enableAlerts(1);

--if not isServer() then
--    onClientSide();
--end



---------------------------- EOF -------------------------------------------


enableAlerts(1);
_DEBUG = 1;
_DEBUGDEPTH = 0;

WT_SMALLBEAM    = 0;     -- always leave this lowest number
WT_BIGBEAM      = 1;
WT_RAILGUN      = 2;
WT_TORPEDO      = 3;
WT_SMALLMISSLE  = 4;
WT_BIGMISSLE    = 5;
WT_FIGHTER      = 6;
WT_MINE         = 7;
WT_BOMB         = 8;
WT_BEACON       = 9;     -- always leave this highest number



-- ########################################################################
-- ###
-- ###  General ship & gadget functions
-- ###

-- returns a tablestructure with general info about the ship + its weapons
function analyzeShip( shipId )
    debugBegin( "analyzeShip( " .. shipId .. " )" );
    assert( isShip( shipId ) == 1, "ID is no ship !" );
    debugId( shipId );

    -- get standard info
    local st_temp = {};
    st_temp.shipType = getShipType( shipId );
    st_temp.shipId = shipId;
    st_temp.shipName = getName( shipId );
    st_temp.shipFaction = getFaction( shipId );
    st_temp.weaponCount = 0;
    st_temp.weapon = {};
    st_temp.lastweapon = {};
    for i = WT_SMALLBEAM, WT_BEACON do
        st_temp.weapon[i] = {};
        st_temp.lastweapon[i] = 0;
    end

    -- analyze gadgets
    local maxGadget = gadgetCount( shipId ) - 1;
    for i = 0, maxGadget do
        local gTemp = getGadget( shipId, i );
        local tTemp = getGadgetType( gTemp );
        local gNameTemp = getName( gTemp );
        local gUnknown = nil;

        if tTemp > 16 then
            debug( "ERROR: Gadgettype unknown to script: name: \"" .. gNameTemp ..  "\", type: " .. tTemp .. "\", ship: \"" .. st_temp.shipName .. "\", type: " .. st_temp.shipType );
        end;

        if tTemp == GT_UNKNOWN then
            debug( "ERROR: Gadget unknown to game: name: \"" .. gNameTemp ..  "\", type: " .. tTemp .. "\", ship: \"" .. st_temp.shipName .. "\", type: " .. st_temp.shipType );

        elseif tTemp == GT_WEAPON then
                if gNameTemp == "AR Missile" or gNameTemp == "Psi Missile" or gNameTemp == "Sabot Rocket" then
                    tappend( st_temp.weapon[WT_SMALLMISSLE], gTemp );
                elseif gNameTemp == "Gauss Gun" or gNameTemp == "Railgun" or gNameTemp == "Particle Cannon" or gNameTemp == "EMP Cannon" then
                    tappend( st_temp.weapon[WT_RAILGUN], gTemp );
                elseif gNameTemp == "Beacon" then
                    tappend( st_temp.weapon[WT_BEACON], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_WEAPON_HEAVY then
                if gNameTemp == "IT Missile" then
                    tappend( st_temp.weapon[WT_BIGMISSLE], gTemp );
                elseif gNameTemp == "AM Torpedo" or gNameTemp == "P Torpedo" or gNameTemp == "Fusion Torpedo" then
                    tappend( st_temp.weapon[WT_TORPEDO], gTemp );
                elseif gNameTemp == "AntiMatter Mine" or gNameTemp == "Nuke Mine" or gNameTemp == "EMP Mine" then
                    tappend( st_temp.weapon[WT_MINE], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_WEAPON_BEAM then
                if gNameTemp == "IP Beam" or gNameTemp == "IPS Beam" or gNameTemp == "CL 300" or gNameTemp == "CL 500" or gNameTemp == "CL 550" then
                    tappend( st_temp.weapon[WT_SMALLBEAM], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_WEAPON_HEAVY_BEAM then
                if gNameTemp == "Disruptor" or gNameTemp == "CL 1000" or gNameTemp == "CL 2000" then
                    tappend( st_temp.weapon[WT_BIGBEAM], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_FIGHTER_BAY then
                if gNameTemp == "M7 Fighter" or gNameTemp == "Fighter Bay - UGTO" or gNameTemp == "Larvae Formation" then
                    tappend( st_temp.weapon[WT_FIGHTER], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_BOMB then
                if gNameTemp == "MiRV Bomb" or gNameTemp == "Bio Bomb" then
                    tappend( st_temp.weapon[WT_BOMB], gTemp );
                else
                    gUnknown = 1;
                end;

        elseif tTemp == GT_HEAVY_BOMB then
                if gNameTemp == "Proton Cruise Missile" then
                    tappend( st_temp.weapon[WT_BIGMISSLE], gTemp );
                else
                    gUnknown = 1;
                end;

        else
            st_temp.weaponCount = st_temp.weaponCount - 1;    -- Don´t count other Gadgets

        end;

        if gUnknown then
            debug( "ERROR: Gadget unknown to script: name: \"" .. gNameTemp ..  "\", type: " .. tTemp .. ", ship: \"" .. st_temp.shipName .. "\", type: " .. st_temp.shipType );
        else
            st_temp.weaponCount = st_temp.weaponCount + 1;
        end

    end

    debugEnd( "analyzeShip( " .. shipId .. " )" );
    debugId( shipId );

    return st_temp;
end

function dumpShipTable( tb )
    debugBegin( "dumpShipTable()" );

    debug( "Name:\"" .. tb.shipName .. "\" , Type: \"" .. Util:getShipTypeName( tb.shipType ) .. "\" , Faction: \"" .. Util:getFactionName( tb.shipFaction ) .. "\" , ID: " .. tb.shipId );
    debug( "Weapons: " .. tb.weaponCount .. " , SmallBeams: " .. getn( tb.weapon[WT_SMALLBEAM] )  .. " , BigBeams: " ..  getn( tb.weapon[WT_BIGBEAM] )
            .. " , Railguns: " ..  getn( tb.weapon[WT_RAILGUN] ) .. " , Torpedos: " .. getn( tb.weapon[WT_TORPEDO] ) .. " , SmallMissles: " .. getn( tb.weapon[WT_SMALLMISSLE] )
            .. " , BigMissles: " .. getn( tb.weapon[WT_BIGMISSLE] ) .. " , Fighters: " ..  getn( tb.weapon[WT_FIGHTER] ).. " , Mines: " ..  getn( tb.weapon[WT_MINE] )
            .. " , Beacons: " ..  getn( tb.weapon[WT_BEACON] ) ..  ", Bombs: " ..  getn( tb.weapon[WT_BOMB] ) );

    debugEnd( "dumpShipTable()" );
end

-- Fires the specified weapon at the target
function fireWeapon( shipTable, wType, targetId )
    debugBegin( "fireWeapon( ... )" );
    debug( "fireWeapon( \"" .. shipTable.shipName .. "\" , " .. wType .. " , \"" .. getName( targetId ) .. "\")" );

    assert( wType >= WT_SMALLBEAM and wType <= WT_BEACON, "Invalid weapon type: " .. wType );

    -- cycle to next weapon
    --shipTable.lastweapon[wType] = mod( ( shipTable.lastweapon[wType] + 1 ) , getn( shipTable.weapon[wType] ) ) + 1;
    shipTable.lastweapon[wType] = shipTable.lastweapon[wType] + 1;
    if shipTable.lastweapon[wType] >  getn( shipTable.weapon[wType] ) then
        shipTable.lastweapon[wType] = 1;
    end;

    local wTemp = shipTable.weapon[wType][shipTable.lastweapon[wType]];
    assert( wTemp, "Used weapon does not exist in table" );

    useGadget2( wTemp, targetId, 0 );

    debugEnd( "fireWeapon( \"" .. shipTable.shipName .. "\" , " .. wType .. " , \"" .. getName( targetId ) .. "\")" );
end

-- Alphastrike at target
function fireAlphaStrike( shipTable, targetId )
    debugBegin( "fireAlphaStrike( ... )" );
    debug( "fireAlphaStrike( \"" .. shipTable.shipName .. "\" , \"" .. getName( targetId ) .. "\")" );

    for i = WT_SMALLBEAM, WT_BEACON do
        for j = 1, getn( shipTable.weapon[i] ) do
            useGadget2( shipTable.weapon[i][j] , targetId, 0 );
        end
    end

    debugEnd( "fireAlphaStrike( \"" .. shipTable.shipName .. "\" , \"" .. getName( targetId ) .. "\")" );
end

-- ship moves around the planet and drops mines, each layer is 50gu more away
function minefieldPlanet( shipTable, nLayers )
end




-- ########################################################################
-- ###
-- ###  Class CommandStack
-- ###

-- Task interface:
-- Each task has to check taskSuspend() and taskTerminate() every cycle
-- After suspending it has to call taskSuspended(), after terminating or
-- completion: taskTerminated()
-- The Task has to implement getCmd()
-- The routine returned by getCmd gets an additional first parameter: the commandstack

-- Command interface:
-- Normal commands can be added to the queue by addCommand()
-- Prioritycommands can be issued using pushCommand(), they will interrupt
-- any current order and be executed within two polling intervals of the current task



-- settag() returns a table "{}" which has been tagged with a new tag value
CommandStack = settag( {}, newtag() )

-- commandtypes:
CommandStack.CMD_DUMMY = 0;         -- the dummy command, executes the dummy task, value must not be changed
CommandStack.CMD_ENDOFLIST = 0;     -- has to have the same value as the last command


-- use the index tag method to redirect a request for a function to the
-- class function table. Rawget is used to prevent a crash for calling a nonexistant function
settagmethod(tag(CommandStack),"index", function(t,f) return rawget(%CommandStack,f) end)

-- instance function take preference over the class functions, if a
-- function is not found in the instance, the class function is used.
settagmethod(tag(CommandStack), "gettable",
          function(t,k)
              if rawget(t,k) then
                return rawget(t,k);
              else
                if rawget(%CommandStack,k) then
                  return rawget(%CommandStack,k);
                else
                  assert(false, "Unable to find function \"" .. k .. "\" in CommandStack" );
                end
              end
          end )

function CommandStack:new( shipId )    -- create an instance of class CommandStack
    debugBegin( "CommandStack:new( " .. shipId .. " )" );
    debugId( shipId );

    local t = {};
    t.intAbstrStack = {};      -- stores abstract orders i.e. "lay minefield"
    t.intExecStack = {};       -- holds orders to execute (lua functions)
    t.shipId = shipId;
    t.shipName = getName( shipId );

    settag( t, tag( CommandStack ) )  -- tag the new table to tell it what its class type is

    debugEnd( "CommandStack:new( " .. shipId .. " )" );
    debugId( shipId );
    return t;
end

-- returns true if the task shall suspend (write itself to the ExecStack)
function CommandStack:taskSuspend()
    debugBegin( self.shipName .. ":taskSuspend()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task checked suspend status, but there is no active task" );

    debugEnd( self.shipName .. ":taskSuspend()" );
    return tTemp.suspend;
end

-- returns true if the task shall terminate (call taskCompleted afterwards)
function CommandStack:taskTerminate()
    debugBegin( self.shipName .. ":taskTerminate()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task checked termination status, but there is no active task" );

    debugEnd( self.shipName .. ":taskTerminate()" );
    return tTemp.terminate;
end

-- Called by the task after it obeyed the suspend command
function CommandStack:taskSuspended( reactivationCode, ... )
    debugBegin( self.shipName .. ":taskSuspended( " .. type(reactivationCode) .. " , " .. getn(arg) .. " )" );
    assert( ( type( reactivationCode ) == "string" ) or ( type( reactivationCode ) == "function" and getn(arg) == 1 ), "Unsupported reactivationCode format" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task suspended, but there is no active task" );
    tTemp.active = 0;
    tTemp.exec = reactivationCode;
    if type( reactivationCode ) == "function" then
        tTemp.execParams = arg;
    end
    self:checkCommands()

    debugEnd( self.shipName .. ":taskSuspended( " .. type(reactivationCode) .. " )" );
end

-- Task completed or got terminated ( called by the task )
function CommandStack:taskCompleted()
    debugBegin( self.shipName .. ":taskCompleted()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task completed, but there was no active task" );
    tremove( self.intExecStack );       -- remove the task

    self:checkCommands();

    debugEnd( self.shipName .. ":taskCompleted()" );
end

-- Inserts a command into the stack, so every other command will be executed first
function CommandStack:addCommand( command, target, params )
    debugBegin( self.shipName .. ":addCommand( \"" .. command .. "\" , \"" .. target .. "\" , " .. type(params) .. " )" );
    assert( command >= CommandStack.CMD_DUMMY and command <= CommandStack.CMD_ENDOFLIST, "Illegal command" );

    aTemp = {};
    aTemp.command = command;
    aTemp.target = target;
    aTemp.params = params;
    aTemp.priority = nil;

    tinsert( self.intAbstrStack, 1, aTemp );

    self:checkCommands();

    debugEnd( self.shipName .. ":addCommand( \"" .. command .. "\" , \"" .. target .. "\" , " .. type(params) .. " )" );
end

-- Places a command on top of the stack, so it will be executed after the
-- ExecStack is empty or the upmost task suspended. If priority is set the
-- current task will suspend and the new one will be executed immediatly
function CommandStack:pushCommand( command, target, params, priority )
    debugBegin( self.shipName .. ":addCommand( \"" .. command .. "\" , \"" .. target .. "\" , Prio: " .. priority or "-" .. " , " .. type(params) .. " )" );
    assert( command >= CommandStack.CMD_DUMMY and command <= CommandStack.CMD_ENDOFLIST, "Illegal command" );

    aTemp = {};
    aTemp.command = command;
    aTemp.target = target;
    aTemp.params = params;
    aTemp.priority = priority;

    tappend( self.intAbstrStack, aTemp );

    self:checkCommands();

    debugEnd( self.shipName .. ":addCommand( \"" .. command .. "\" , \"" .. target .. "\" , Prio: " .. priority or "-" .. " , " .. type(params) .. " )" );
end

-- Suspends the current task
function CommandStack:setTaskSuspend()
    debugBegin( self.shipName .. ":setTaskSuspend()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task shall be suspended, but there is no active task" );
    tTemp.suspend = 1;

    debugEnd( self.shipName .. ":setTaskSuspend()" );
end

-- Terminates the current task
function CommandStack:setTaskTerminate()
    debugBegin( self.shipName .. ":setTaskTerminate()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task shall be terminated, but there is no active task" );
    tTemp.terminate = 1;

    debugEnd( self.shipName .. ":setTaskTerminate()" );
end

-- INTERNAL USE ONLY. Moves the next abstract task onto the exec stack, and executes it
-- also adds a reference to the CommandStack as first parameter, so the callback function can be used
function CommandStack:abstrToExec()
    debugBegin( self.shipName .. ":abstrToExec()" );

    aTemp = self.intAbstrStack[getn(self.intAbstrStack)];
    assert( aTemp, "No command on abstract Stack" );

    tTemp = {};
    tTemp.suspend = nil;    -- shall suspend
    tTemp.terminate = nil;  -- shall terminate
    tTemp.active = nil;     -- if not active, then it is suspended, every task starts inactive
    tTemp.command = aTemp.command; -- save original command for later checks
    tTemp.target = aTemp.target;   -- same with the target
    tTemp.execParams = {};  -- optional arguments in case exec is not a string but a function pointer ( this line is just for consistent declaration )
    tTemp.exec, tTemp.execParams = self:orderToFunction( aTemp.command, aTemp.target, aTemp.params, aTemp.priority );  -- function/code to start the task
    tinsert( tTemp.execParams, 1, self );   -- give reference to commandstack as first parameter

    tappend( self.intExecStack, tTemp );
    tremove( self.intAbstrStack );

    self:checkCommands();

    debugEnd( self.shipName .. ":abstrToExec()" );
end

-- INTERNAL USE ONLY. Translates a command into a functioncall
function CommandStack:orderToFunction( cmd, target, params, priority )
    debugBegin( self.shipName .. ":orderToFunction( \"" .. cmd .. "\" , \"" .. target .. "\" , Prio: " .. (priority or "-") .. " , " .. type(params) .. " )" );

    assert( cmd, "No command given" );
    assert( cmd >= CommandStack.CMD_DUMMY and cmd <= CommandStack.CMD_ENDOFLIST, "Critical error. Attempt to convert illegal command" );

    local res1, res2;

    if cmd == CommandStack.CMD_DUMMY then res1, res2 = Task_Dummy:getCmd( target, params, priority );
    else
        assert( false, "Critical error. Commandsearch led to no result" );
    end

    assert( res2, "getCmd must at least return an empty table as res2" );
    assert( type(res2) == "table", "getCmd must at least return an empty table as res2" );

    debugEnd( self.shipName .. ":orderToFunction( \"" .. cmd .. "\" , \"" .. target .. "\" , Prio: " .. (priority or "-") .. " , " .. type(params) .. " )" );
    return res1, res2;
end

-- INTERNAL USE ONLY. Is called when ship got nothing left to do
function CommandStack:idle()
    debugBegin( self.shipName .. ":idle()" );

    debug( "Not supported yet" );       -- TODO

    debugEnd( self.shipName .. ":idle()" );
end

-- INTERNAL USE ONLY. Checks if there is a command to execute
function CommandStack:checkCommands()
    debugBegin( self.shipName .. ":checkCommands()" );

    local nAbstr = getn( self.intAbstrStack );
    local nExec = getn( self.intExecStack );

    if nAbstr > 0 then                          -- is there an abstract command on queue ?
        local aTemp = self.intAbstrStack[getn(self.intAbstrStack)];
        assert( aTemp, "Topmost abstract command is nil, although there has to be one" );

        if nExec > 0 then                       -- is there a command on the exec stack ?
            local eTemp = self.intExecStack[getn(self.intExecStack)];
            assert( aTemp, "Topmost exec command is nil, although there has to be one" );

            if  eTemp.active then               -- current task is and there are other tasks on queue
                if aTemp.priority then
                    self:setTaskSuspend();      -- task on queue has priority, suspend current task
                end

            else                                -- task is inactive

                if aTemp.priority then
                    self:abstrToExec();         -- task on queue has priority, and current task is inactive -> set as next task
                else
                    eTemp.suspend = nil;        -- another task is waiting but has no prio, current task is suspended, so resume it
                    eTemp.active = 1;
                    self:execTask();  -- start/resume task
                end

            end

        else                                    -- no command exec stack, but abstrstack still has one
            self:abstrToExec();
        end

    else                                        -- no abstract command available
        if nExec > 0 then                       -- is there a command on the exec stack ?
            local eTemp = self.intExecStack[getn(self.intExecStack)];
            assert( aTemp, "Topmost exec command is nil, although there has to be one" );

            if not eTemp.active then            -- current task is inactive, and nothing else to do
                eTemp.suspend = nil;
                eTemp.active = 1;
                self:execTask();  -- start/resume task
            end

        else                                    -- no command on abstract & exec stack
            self:idle();
        end;

    end;

    debugEnd( self.shipName .. ":checkCommands()" );
end

-- INTERNAL USE ONLY. Executes the topmost task from the execStack
function CommandStack:execTask()
    debugBegin( self.shipName .. ":execTask()" );

    local tTemp = self.intExecStack[getn(self.intExecStack)];
    assert( tTemp, "Task to be executed, but there is task" );

    assert( type( tTemp.exec ) == "string" or type( tTemp.exec ) == "function", "Unsupported exec format" );

    if type( tTemp.exec ) == "function" then
        debug( "executing function, " .. getn( tTemp.execParams ) .. " params..." );
        call( tTemp.exec, tTemp.execParams );
    else
        debug( "executing plaintext code: \"" .. tTemp.exec .. "\"..." );
        doString( tTemp.exec );
    end

    debugEnd( self.shipName .. ":execTask()" );
end

-- Dumps command and abstr stack
function dumpCommandStack( cs )
    debugBegin( "dumpCommandStack( \"" .. cs.shipName .. "\" )" );

    debug( getn( cs.intAbstrStack ) .. " commands on abstract command stack." );
    for i = 1, getn( cs.intAbstrStack ) do
        debug( i .. ") Command: " .. cs.intAbstrStack[i].command .. " , Target: \"" .. cs.intAbstrStack[i].target
                 .. "\" , Prio: " .. cs.intAbstrStack[i].priority or 0 );
    end
    debug( getn( cs.intExecStack ) .. " commands on exec stack." );

    for i = 1, getn( cs.intExecStack ) do
        debug( i .. ") Exec: \"" .. cs.intExecStack[i].exec .. "\" , Active: " .. cs.intExecStack[i].active or "-"
                 .. " , Suspend: " .. cs.intExecStack[i].suspend or "-" .. " , Terminate: " .. cs.intExecStack[i].terminate or "-"
                 .. " Command: " .. cs.intExecStack[i].command .. " , Target: \"" .. cs.intExecStack[i].target );
    end

    debugEnd( "dumpCommandStack( \"" .. cs.shipName .. "\" )" );
end



-- ########################################################################
-- ###
-- ###  Dummy task
-- ###

-- This is a general example of the structure of a Task for usage with CommandStack
-- It implements typical behavior. The actual "Task" is to count from 1 to 7 or 11.

Task_Dummy = {};

-- how to start this task
function Task_Dummy:getCmd( target, params, priority )
    -- target & parameters are ignored, this is a dummy task :)
    local count = 7;
    if priority then    -- priority tasks count to 11
        count = 11;
    end

    tParams = { self, 0, count };  -- give the task a reference to itself , current count, max count

    return self.onTimer, tParams;
end

-- the "workhorse" of the task
function Task_Dummy:onTimer( cStack, currCount, maxCount )
    debug( cStack.shipName":Task_Dummy: current count: " .. currCount .. " max count: " .. maxCount );

    currCount = currCount + 1;
    if currCount >= maxCount then           -- check for completion
        cStack:taskCompleted();
    else
        --startTimer( 1, "Ta
    end

end


-- ########################################################################
-- ###
-- ###  General utility functions
-- ###

function tappend( table, value )
    tinsert(table, value);
end

-- general debug info
function debug( s )
    if _DEBUG then
        assert( _DEBUGDEPTH >= 0, "_DEBUGDEPTH not >= 0 in debug()" );
        local sShift = "";
        for i = 1, _DEBUGDEPTH do
            sShift = sShift .. "    ";
        end;

        sendChat( "Chromix", SC_PRIVATE, "Trace: " .. sShift .. ( s or "nil" ) );
    end;
end

function debugId( id )
    debug( id .. " is \"" .. getName( id ) .. "\"" );
end

-- to be called when a function starts
function debugBegin( s )
    debug( "BEGIN - " .. s );
    assert( _DEBUGDEPTH >= 0, "_DEBUGDEPTH not >= 0 in debugBegin()" );
    _DEBUGDEPTH = _DEBUGDEPTH + 1;
end

-- to be called when a function ends
function debugEnd( s )
    assert( _DEBUGDEPTH, "_DEBUGDEPTH not set" );
    _DEBUGDEPTH = _DEBUGDEPTH - 1;
    assert( _DEBUGDEPTH >= 0, "_DEBUGDEPTH not >= 0 in debugEnd()" );
    debug( "END - " .. s );
end



if isServer() == 1 then

    --local temp = analyzeShip( getKey("Chromix") );
    --dumpShipTable( temp );

    --local cStack = CommandStack:new( getKey("Chromix") );
    --cStack:addCommand( CommandStack.CMD_DUMMY, "No target needed", "no params needed also" );
    --dumpCommandStack( cStack );
end
