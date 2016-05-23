enableAlerts(0);  -- Don't change this.

if isServer() then
	
local scripter = "[-GTN-]BackSlash";  -- Your name here (including fleet tag, if any).
local spawnTarget = "Lock's Core"; -- Needs to be a planet (if a cluster, select the center planet).
local factionName = "Pirates"; -- Enter the name of the faction you want to run this script for.

-- Generic ChatLine Colors --
local lightred = "<color;6666ff>";
local salmon = "<color;9999ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local lilac = "<color;ffccff>"; 
local green = "<color;00ff00>";
local lightgreen = "<color;99ff66>";
local ec = "</color>";

-- Ship array --
ships ={}; 
ships[100]={name="Zeyrcic Defender", path="ships\\ugto\\destroyer\\st-10 gunboat\\sc_st10.prt"}; 
ships[101]={name="Nazton's Elite", path="ships\\ugto\\cruiser\\st-76 battle cruiser\\sc_st76.prt"}; 
ships[102]={name="Rogen's Privateer", path="ships\\ugto\\dreadnought\\st-105 battle platform\\sc_st105.prt"}; 

ships[200]={name="Nazton's Privateer", path="ships\\icc\\cruiser\\m230 a\\sc_m230a.prt"}; 
ships[201]={name="Anarion Patrol", path="ships\\icc\\cruiser\\m231 b\\sc_m231b.prt"}; 
ships[202]={name="Zeyrcic Jammer", path="ships\\icc\\cruiser\\m235 i\\sc_m235i.prt"}; 
ships[203]={name="Cartel Dreadnought", path="ships\\icc\\dreadnought\\m400-a\\sc_m400a.prt"}; 
ships[204]={name="Anarion Command", path="ships\\icc\\dreadnought\\old\\m310\\sc_m310.prt"};  

ships[300]={name="Ray A", path="ships\\pirate\\scout\\ray a\\sc_ray_a.prt"};
ships[301]={name="Ray AS", path="ships\\pirate\\scout\\ray as\\sc_ray_as.prt"};
ships[302]={name="Ray E", path="ships\\pirate\\scout\\ray e\\sc_ray_e.prt"};
ships[303]={name="Rogen's Patrol", path="ships\\pirate\\destroyer\\sloop\\sc_sloop.prt"};
ships[304]={name="Lieutenant Flynn [Elite]", path="ships\\pirate\\frigate\\elite01\\sc_pirate_elite01.prt"};
ships[305]={name="Rogen's Assaulter", path="ships\\pirate\\frigate\\raider t\\sc_raider.prt"};
ships[306]={name="Corvette", path="ships\\pirate\\frigate\\corvette\\sc_corvette.prt"};
ships[307]={name="Rogen's Raider", path="ships\\pirate\\frigate\\raider\\sc_raider.prt"};
ships[308]={name="Rogen's Carrier", path="ships\\pirate\\destroyer\\c208cvm\\sc_c208cvm.prt"};
ships[309]={name="Tender", path="ships\\pirate\\supply\\tender\\sc_tender.prt"};
ships[310]={name="Hauler", path="ships\\pirate\\transport\\hauler\\sc_hauler.prt"};

-- Function to simplify spawning of ships --
function spawn( _shipId, _target, _indent)
	spawnShip(ships[_shipId].path, _target, ships[_shipId].name.." ".._indent, shipFaction);
end

-- Function to simplify sending of messages --
function sendNotice( _chatLine )
	sendChat( scripter, 1, _chatLine);
end

-- Function to find the correct team ID, and set the planet and shipFaction correctly --
function findTeam()
	local teamNo = teamCount();
	for i=1,teamNo do
		teamName = getTeamName( i );
		if teamName.find(teamName, factionName) then
			faction = i;
			break;
		end
	end
	shipFaction = teamId( faction );
end

-- Function to add the spawn target to table, as well as any planets oribiting that --
function getPlanets()
	planets ={};
	table.insert(planets,spawnTarget);
	local planetCount = planetCount();
	for i=1,planetCount do
		local planetKey = getPlanet( i );
		local planetName = getName( planetKey );
		if isOrbiting( planetKey, spawnTarget) then
			table.insert(planets,planetName);
		end
	end
	spawnCount = # planets;
end

-- Function to spawn ships in an escalating manner - the more planets in the table, the tougher the ship spawn --
function spawnTime()
	for i=1,spawnCount do
		revoltPlanet( planets[i], faction );
		
		for unitnum = 0, unitCount( planets[i] ) - 1 do
			currNode = getUnit( planets[i], unitnum );
			detachNoun(currNode);
		end
		
		if i = 1 then
			for y=1,4 do
				spawn( 303, planets[i], y );
			end
		end
		
		if i = 2 then
			for y=1,2 do
				spawn( 101, planets[i], y );
			end
		end
		
		if i = 3 then
			for y=1,2 do
				spawn( 200, planets[i], y );
			end
		end
		
		if i > 4 then
			for y=1,2 do
				spawn( 102, planets[i], y );
				spawn( 203, planets[i], y );
			end
		end
	end
end

findTeam();
getPlanets();
spawnTime();

if spawnCount > 1 then
	sendNotice("COMMS ALERT: "..teal.."A sudden rebellion in the "..spawnTarget.." cluster has caused a cluster-wide revolt! Pirates have ceased control of the entire "..spawnTarget.." planet cluster, overwhelming the security militia and overtaking their ships. All civilians are advised to stay clear of the Star System for now."..ec);
else
	sendNotice("COMMS ALERT: "..teal.."A sudden rebellion on planet "..spawnTarget.." has caused a revolt! Pirates have ceased control of the entire planet, overwhelming the security militia and overtaking their ships. All civilians are advised to stay clear of the Star System for now."..ec);
end

sendChat( scripter, 4 , teal.."Script finished..."..ec );

end
