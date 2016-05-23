enableAlerts(0); 

if isServer() then 
--/Run [scriptname.lua] server
--You need to change these variables BEFORE you run the script. 
local suspect = "Shad4c"; --This is the person whose ship you're checking. 
local investigator = "Doran"; --This is you. 

local shiptype = string.sub(getClass(suspect), string.find(getClass(suspect), "%u%a+", 2)); 

sendChat( investigator, 4, "<color;22bbff>|==Listing all nodes on "..suspect.."'s ship==|" ); 
sendChat( investigator, 4, "<color;22bbff>"..suspect.." is in a <color;3333ff>"..shiptype.."</color>" ); 

nodenum = 1; 
for nodecount = 0, nodeCount( suspect ) - 1 do 
currNode = getNode( nodecount, suspect ); 
target = getName( currNode ); 
tclass = getClass( currNode ); 
if (target ~= "Trail") then 
sendChat( investigator, 4, "Node ".. nodenum .. ": " .. target); 
nodenum = nodenum +1; 
end 
end 
sendChat( investigator, 4, "<color;22bbff>"..(nodenum-1).." nodes in total.</color>" ); 
--~ scriptAlert( investigator.." has just checked the layout of "..suspect.."'s ship."); 


end

