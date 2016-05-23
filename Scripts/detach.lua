-----------------------------------
---- Ship Balance Tracker v0.5 ----
---- Robert Kelford, 2008 ---
-----------------------------------

enableAlerts(0);  --don't change this

if isServer() then 

------------------------------------
-- Main Script Variables ----
------------------------------------

local scripter = "Drafell"; -- Insert scripter here
local target = "ICC Homegate";
local lightred = "<color;6666ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local green = "<color;00ff00>";
local black = "<color;000000>"; --why on earth would you use this?
local ec = "</color>";

function deleteTarget(value)
	local kill_target = getKey(value);
	if isNoun(kill_target) then
	detachNoun(kill_target);
	sendChat(scripter, 4, lightred..value.." destroyed."..ec);
	return true;
	else
	sendChat(scripter, 4, lightred..value.." is not a valid noun."..ec);
	return false;
	end
end

deleteTarget(target);

end
