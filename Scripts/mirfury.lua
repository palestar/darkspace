-----------------------------------
---- Mi MV Encounters  ----
---- Written by Drafell 2007  ----
-----------------------------------

-- Start the script and leave it running.

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

local scripter = "[Admin]Mersenne Twister"; -- Insert scripter ID  here, not strictly needed but included as a failsafe for default script options.
local spawn_target = "Fury"; -- Sets the default script target. Included asafailsafe.
local diff_level = 8; -- Sets difficulty level of the targets. 8 is default
local max_ships = 15; -- Sets the maximum number of shisp the script can spawn. Recommend no more than 20 for 1.483
local message_target = scripter;

-- Initialize variables and tables used globally within the script
local faction_ID = 0;
local e = 0;
local w = 0;
local x = 0;
spawnedShips ={};

-- Set color keys, format BBGGRR.
local lightred = "<color;6666ff>";
local salmon = "<color;9999ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local lilac = "<color;ffccff>"; 
local green = "<color;00ff00>";
local lightgreen = "<color;99ff66>";
local black = "<color;000000>"; --why on earth would you use this?
local ec = "</color>";

function round2(num, idp)
	return tonumber(string.format("%." .. (idp or 0) .. "f", num))
end

-- Checks for players in server and continually loops intil an active player is found 
function checkPlayers()
	local player_check = getName(getShip(randomNumber(0, shipCount()-1)));
	if shipCount()==0 then	-- Check to see if there are any ships, restart timer if none found
		startTimer(300, "checkPlayers()");
	elseif isAIControlled(player_check)==1 and shipCount()==1 then 	-- Check to see if selected ship is AI controlled
		startTimer(300, "checkPlayers()");
	elseif isAIControlled(player_check)==1 and shipCount() > 1 then 	-- Check to see if selected ship is AI controlled
		startTimer(10, "checkPlayers()");
	else
		shipSpawn();
	end
end

-- Handles all the message broadcasts for the script and handles event triggers related to each message.
function msgTarget(msg_no)
	message_target = getName(getShip(randomNumber(0, shipCount()-1)));
	if isAIControlled(message_target) then -- Finds new message target is ship is AI controlled
		msgTarget(msg_no);
	elseif msg_no==1 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightred.."An unidentified alien force has been detected near "..ec..spawn_target..lightred..". The Quadrant Shipping News Service advises extreme caution for all civlian shipping in this area. The current threat level near this location is considered 'risky'. We also advise additional caution when using nearby jumpgates as these are known target locations."..ec);
	startTimer(20, "checkShipTable()");
	elseif msg_no==2 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."The threats near "..ec..spawn_target..lightgreen.." have been neutralised. The Quadrant Shipping News Service has downgraded the threat level near this location to 'normal'."..ec);
		e = 0;
		startTimer(randomNumber(60, shipCount() * shipCount() + randomNumber(2500, 4800)), "checkPlayers()");
	elseif msg_no==3 then
		sendChat(message_target, 1, "NEWS ALERT: "..salmon.."The Quadrant Shipping News Service continues to advise caution when travelling near "..ec..spawn_target..salmon..". We will do our utmost to keep you informed and updated until the threat near this location has been eliminated. The threat level near "..ec..spawn_target..salmon.." will continue to remain at 'risky' until further notice, and we advise extreme caution when using the nearby jumpgates as these are known to be high priority targets."..ec);
		checkShipTable();
	elseif msg_no==4 then
		sendChat(message_target, 1, "NEWS ALERT: "..salmon.."*Yawn* The Quadrant Shipping News Service has fallen asleep. We think there could be aliens running around, somewhere. You know, like in that film... called... well... Aliens... Would be nice if some hot chick with a big gun would blow them up, then maybe we could get some sleep and stop annoying you with silly news alerts. Failing that we have geeks in space ships. Oh well life sucks... Oh yes, it might be dangerous. Run away!!! *distant screams*"..ec);
		checkShipTable();
	end
end

-- Selects a spawn location from the defined list.
function selectSpawn()
	local locTable = { [1]="Fury", [2]="Lyle", [3]="Procyon 11", [4]="Griffen", [5]="Vica", [6]="Foydes", [7]="Tau Layo", [7]="Kohia", [9]="Erinyes" };
	local n = randomNumber(1, table.getn(locTable));
	return locTable[n];
end

-- Randomly calls the ship spawn functions
function shipSpawn()
	spawn_target = selectSpawn();
	local ship_num = spawnSeed();
	for e=1, ship_num do
		local g = randomNumber(1,2);
		if g==1 then
			spawnCruiser(e);
		else
			spawnDread(e);		
		end
	end
	msgTarget(1);
end

-- Adjusts the number of spawned ships in accordance with the number of players in game.
function spawnSeed()
	local ship_no = randomNumber(shipCount()*0.75, shipCount()*1.25);
	if ship_no > max_ships then
		return max_ships;
	else
		return round2(ship_no, 0);
	end
end

function spawnCruiser(e)
	local ship_name = "[N-14]Nexus 14 Drone "..e;
	local new_ship = spawnShip(ships[430].path, spawn_target, ship_name, faction_ID );
	for f=1, 2 do
		spawnNoun( "Gadgets\\Elf Beam.prt", new_ship, "Elf Beam "..f, new_ship);
		spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		spawnNoun( "Gadgets\\armorstandard.prt", new_ship, "Standard armor "..f, new_ship);
		spawnNoun( "Gadgets\\P Cruise Missile.prt", new_ship, "PCM "..f, new_ship);
		local infantry = spawnNoun("Units\\Heavy Infantry.prt", new_ship, "Automaton"..f, new_ship);
		setOwner(infantry, getOwner(new_ship));
	end
	for f=1, diff_level + 5 do
		spawnNoun( "Gadgets\\Psi Cannon.prt", new_ship, "Psi Cannon "..f, new_ship);
	end
	for f=1, diff_level + 6 do
		spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
	end
	table.insert(spawnedShips, e, ship_name);
end

function spawnDread(e)
	local ship_name = "[N-14]Nexus 14 Drone "..e;
	local new_ship = spawnShip(ships[440].path, spawn_target, ship_name, faction_ID );
	for f=1, 1 do
		if f==randomNumber(1,9) then
			spawnNoun( "Gadgets\\stellarincinerator.prt", new_ship, "Stellar Incinerator "..f, new_ship);
		end
		if (f+1)==randomNumber(1,10) then
			spawnNoun( "Gadgets\\ioncannon.prt", new_ship, "Ion Cannon "..f, new_ship);
		end
	end
	for f=1, 2 do
		spawnNoun( "Gadgets\\Elf Beam.prt", new_ship, "Elf Beam "..f, new_ship);
		spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		spawnNoun( "Gadgets\\armorstandard.prt", new_ship, "Standard armor "..f, new_ship);
		local infantry = spawnNoun("Units\\Heavy Infantry.prt", new_ship, "Automaton"..f, new_ship);
		setOwner(infantry, getOwner(new_ship));
	end
	for f=1, diff_level + 5 do
		spawnNoun( "Gadgets\\Psi Cannon.prt", new_ship, "Psi Cannon "..f, new_ship);
	end
	for f=1, diff_level + 6 do
		spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
	end
	table.insert(spawnedShips, e, ship_name);
end

-- continually checks to see if the AI ships are still alive. Ends and restarts the script once all dead.
function checkShipTable()
	if table.getn(spawnedShips)==0 then
		x = 0;
		w = 0;
		msgTarget(2);
	elseif table.getn(spawnedShips)>=1 and w >= 10 and x < 50 then
		w = 0;
		msgTarget(3);
	elseif table.getn(spawnedShips)>=1 and w >= 10 and x >= 50 then
		w = 0;
		x = 0;
		msgTarget(4);
	elseif table.getn(spawnedShips)>=1 then
		for index = table.getn(spawnedShips), 1, -1 do
			local value = spawnedShips[index]; 
			if not isShip(value) then
				table.remove(spawnedShips, index);
			end
		end
		w = w + 1;
		x = x + 1;
		startTimer(60, "checkShipTable()");
	else
		sendChat(message_target, 1, red.."Unknown script error"..ec);
		w = 0;
		e = 0;
		startTimer(randomNumber(600, 4000), "checkPlayers()");
    end
end

startTimer(randomNumber(600, 8000), "checkPlayers()");

end
