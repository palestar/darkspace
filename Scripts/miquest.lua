-----------------------------------
---- Adv Encounters v0.5 ----
-----------------------------------

enableAlerts(0);  --don't change this

ships ={};

ships[430]={name="Legion Cruiser", path="ships\\mi\\cruiser\\legion\\sc_legion.prt"}; -- MIR Cruiser
ships[440]={name="Longhead", path="ships\\mi\\dreadnought\\longhead\\sc_dread.prt"}; -- MIR Dread
ships[460]={name="Assembler", path="ships\\mi\\engineering\\assembler\sc_assembler.prt"}; -- MIR Engy
ships[461]={name="Forager", path="ships\\mi\\supply\\forager\\sc_forager.prt"}; -- MIR Supply
ships[470]={name="Conveyor", path="ships\\mi\\transport\\conveyor\\sc_conveyor.prt"}; -- MIR Trans

if isServer() then 

------------------------------------
-- Main Script Variables ----
------------------------------------

local scripter = "[Myth]Drafell"; -- Insert scripter here
local target = scripter; -- leave as scripter if targetting yourself, otherwise use "targetname" with the ""'s

-- Initialize variables and tables used globally within the script
local shipClass = 0;
local factionID = 0;
local e = 0;
spawnedShips ={};

-- Set color keys, format BBGGRR.

local lightred = "<color;6666ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local green = "<color;00ff00>";
local black = "<color;000000>"; --why on earth would you use this?
local ec = "</color>";

function findTarget()
	local newTarget = getName(getShip(randomNumber(0, (shipCount() - 1))));
	if shipCount()==0 then	-- Check to see if there are any ships, restart timer if none found
		setTimer(300, "findTarget()");
	elseif isAIControlled(newTarget)==1 and shipCount()==1 then 	-- Check to see if selected ship is AI controlled
		sendChat(scripter, 4, red.."Non-valid target."..ec);	--  and sets a 5 minute timer if it is the only ship
		setTimer(300, "findTarget()"); 
	elseif isAIControlled(newTarget)==1 and shipCount()>1 then 	-- Check to see if selected ship is AI controlled
		sendChat(scripter, 4, red.."Non-valid target."..ec);	--  and sets a short delay before finding a new target
		setTimer(10, "findTarget()");
	else
		target = newTarget;	-- Assigns the found target to the global variable
		sendChat(scripter, 4, green.."New target "..target.." found."..ec);
		setQuestTimer(); -- Initializes the "quest" timer.
	end
end

function setQuestTimer()
	-- local timerMod = randomNumber( 1, 300-( getHull(target) + getEnergy(target)/2) ); --Ahjust's the coundown timer according to the targets hull and energy.
	local timerMod = 20;
	sendChat(scripter, 4, teal.."timerMod="..timerMod..ec);
	startTimer(timerMod,"verifyShipClass()");
end

function verifyShipClass()
	if isShip(target) then
		shipClass = getShipType(target);
		sendChat(scripter, 4, green.."shipClass="..shipClass..ec);
		if shipClass < 5 then
			sendChat(scripter, 4, lightred.."Invalid shipclass, finding new target..."..ec);
			findTarget();
		else
			shipSpawn();
		end
	else
		findTarget();
	end
end

function shipSpawn()
	local shipNum = spawnSeed();
	for e=1, shipNum do
		local g = randomNumber(1,3);
		if g==1 then
			spawnFrigate(e);
		elseif g==2 and shipClass < 7 then
			spawnFrigate(e);
		elseif g==3 and shipClass < 7 then
			spawnFrigate(e);
		elseif g==2 and shipClass > 6 then
			spawnCruiser(e);
		elseif g==3 and shipClass < 9 then
			spawnCruiser(e);
		elseif g==3 and  shipClass > 8 then
			spawnDread(e);
		else
			sendChat(target, 1, red.."Fatal script error"..ec);
		end
	end
	startTimer(8, "checkShipTable()");
end

function spawnSeed()
	return randomNumber( (shipClass - 6) , (shipClass - 2));
end

function spawnFrigate(e)
	local ship1 = "Nexus 14 Conveyor "..e;
	local newShip = spawnShip(ships[470].path, target, ship1, factionID );
	for f=1,2 do
		spawnNoun( "Gadgets\\Auto Repair.prt", newShip, "Automated Repair "..f, newShip);
		spawnNoun( "Gadgets\\p cannon.prt", newShip, "Particle cannon "..f, newShip);
		spawnNoun( "Gadgets\\Drive - IE.prt", newShip, "IE Drive "..f, newShip);
	end
	table.insert(spawnedShips, e, ship1);
	sendChat(scripter, 4, green..spawnedShips[e]..ec);
end

function spawnCruiser(e)
	local ship2 = "Nexus 14 Cruiser "..e;
	local newShip = spawnShip(ships[430].path, target, ship2, factionID );
	for f=1,4 do
		spawnNoun( "Gadgets\\Auto Repair.prt", newShip, "Automated Repair "..f, newShip);
		spawnNoun( "Gadgets\\p cannon.prt", newShip, "Particle cannon "..f, newShip);
		spawnNoun( "Gadgets\\Drive - IE.prt", newShip, "IE Drive "..f, newShip);
	end
	table.insert(spawnedShips, e, ship2);
	sendChat(scripter, 4, green..spawnedShips[e]..ec);
end

function spawnDread(e)
	local ship3 = "Nexus 14 Longhead "..e;
	local newShip = spawnShip(ships[440].path, target, ship3, factionID );
	for f=1,6 do
		spawnNoun( "Gadgets\\Auto Repair.prt", newShip, "Automated Repair "..f, newShip);
		spawnNoun( "Gadgets\\p cannon.prt", newShip, "Particle cannon "..f, newShip);
		spawnNoun( "Gadgets\\Drive - IE.prt", newShip, "IE Drive "..f, newShip);
	end
	table.insert(spawnedShips, e, ship3);
	sendChat(scripter, 4, green..spawnedShips[e]..ec);
end

function checkShipTable()
	sendChat(scripter, 4, green.."table entries="..tableCount(spawnedShips)..ec);
	if tableCount(spawnedShips)==0 then
		findTarget();
	elseif tableCount(spawnedShips)>=1 then
		for index,value in ipairs(spawnedShips) do
			if isShip(value) then
				-- Do nothing
			else
				table.remove(spawnedShips, index);
			end
		end
		startTimer(8, "checkShipTable()");
	else
		sendChat(scripter, 4, red.."Unknown script error"..ec);
		findTarget();
    end
	-- startTimer(2, "checkShipTable()");
	-- Eequentially check through the table to see if the stored ships are destroyed
	-- If an entry is no longer in the game server, remove that entry
	-- Once all entries are removed, re-initialize the script (global) variables and find a new target. 
end

function tableCount(t)
	local entries = 0;
	for index,value in ipairs(t) do
		entries=entries+1; 
	end
	return entries;
end

findTarget();

end 