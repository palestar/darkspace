-- You must have a mod flag and a server flag to use this script.  See an admin if you lack one or both. 
-- To run type '/run <scriptname>.lua server'.
-- When you're done with your scripting and before you leave the server, purge the script: 
-- To purge type '/forward /kill *'.
-- This will toast any remnants of the script hanging out in memory, and can be done at any time.

enableAlerts(1);  -- Don't change this.

if isServer() then 
	local scripter = "[-GTN-]BackSlash"; -- Your name here (including fleet tag, if any).
	local planet = "Ursovica"; -- Planet to be revolted.
	local factionName = "ICC"; -- Enter the name of the faction you want to run this script for (punctuation and spelling must be correct).
	local sc = "<color;6666ff>"; -- Change the 6 digit hex code if you want a different colour for the debug messages
	local ec = "</color>";

	for i=0 ,teamCount() do
		teamName = getTeamName( i );
		if teamName.find(teamName, factionName) then
			sendChat(scripter, 4 , sc.."Faction for "..factionName.." is "..i.."..."..ec);
			faction = i;
			break;
		end
	end
	
	sendChat(scripter, 4 , sc.."Revolting "..planet.."..."..ec);
	revoltPlanet( planet, faction );

	sendChat(scripter, 4 , sc.."Setting infantry on "..planet.." to "..factionName.."..."..ec);
	for unitnum = 0, unitCount( planet ) - 1 do
		currNode = getUnit( planet, unitnum );
		setTeamId( currNode, faction+1 );
	end

	setPlanetPopulation( planet, 60 );
	setPlanetResources( planet, 1000000 );
	setPlanetTech( planet, 70 );
	


	sendChat(scripter, 4 , sc.."Script ended.  Please remove from server memory."..ec);
end

