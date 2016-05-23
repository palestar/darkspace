--~ Faction Reporter - Reports the faction ID to your screen for the main factions 
--~ as we all know they tend to change sometimes 

enableAlerts(0); 

if isServer() then 

local scripter = "[Admin]Mersenne Twister" -- Change to your name 
local faction = getOwner(scripter) -- local faction = getOwner(target) 

function showfaction() 
local u = getOwner("Bifrost Fleet Foundry"); 
sendChat(scripter, 4, "The faction ID for UGTO is "..u); 
local i = getOwner("Shi Jie Shipyards"); 
sendChat(scripter, 4, "The faction ID for ICC is "..i); 
local k = getOwner("Sag'Keja Fleet Armory"); 
sendChat(scripter, 4, "The faction ID for K'Luth is "..k); 
local m = getOwner("Spatial Fissure"); 
sendChat(scripter, 4, "The faction ID for Mir is "..m); 
local p = getOwner("Caribbean Whirlpool"); 
sendChat(scripter, 4, "The faction ID for Pirates is "..p); 
sendChat(scripter, 4, "Your faction ID is "..faction); 
end 

showfaction(); 

end