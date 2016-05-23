----------------------------------- 
---- Display Users + Factions ---- 
----------------------------------- 
---- Script written by Drafell ---- 
----------------------------------- 

-- Use only on script enabled accounts and only in the MV server. 
-- Replace the name in the "scripter" variable with the name of the account being used 
-- Save this file as getusers.lua 
-- Log into the MV game, and type /run getusers.lua server 
-- After the list shows, type /forward /kill * 
-- to kill the script and avoid potential memory leaks. 

enableAlerts(0); --don't change this 

if isServer() then 

------------------------------------ 
-- Main Script Variables ---- 
------------------------------------ 

local scripter = "Doran"; -- Insert scripter name here 
local target = scripter; -- leave as scripter if targetting yourself 

-- Initialize variables and tables used globally within the script 

local e = 0; 

-- Set color keys, format BBGGRR. 

local lightred = "<color;6666ff>"; 
local red = "<color;3333ff>"; 
local teal = "<color;ffff66>"; 
local blue = "<color;ff0000>"; 
local ctan = "<color;99ccff>"; 
local white = "<color;ffffff>"; 
local green = "<color;00ff00>"; 
local black = "<color;000000>"; --why on earth would you use this? 
local ec = "</color>"; 

function displayShips() 
local ships = shipCount(); 
sendChat(scripter, 4, white.."There are "..ec..teal..ships..ec..white.." ships currently in the server."..ec); 
for i=1, shipCount() do 
local name = getName(getShip(i-1)); 
local userId = getUserId(name); 
local userFaction = returnFaction(name); 
local class = getClass(name); 
sendChat(scripter, 4, white..i.."= "..ec..green..name..", "..ec..teal.."@"..userId..", "..ec..lightred..userFaction..", "..ec..ctan..class..ec); 
end 
end 

function returnFaction(e) 
local factionID = getFaction(e); 
local retFaction = "Unknown"; 
if factionID==0 then 
retFaction="Neutral"; 
elseif factionID==1 then 
retFaction="UGTO"; 
elseif factionID==2 then 
retFaction="ICC"; 
elseif factionID==3 then 
retFaction="K`Luth"; 
else 
retFaction="Unknown"; 
end 
return retFaction; 
end 

displayShips(); 

end