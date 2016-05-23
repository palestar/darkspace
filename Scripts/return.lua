enableAlerts(0);  -- Don't change this.

if isServer() then 

local scripter = "[-GTN-]BackSlash"; -- Your name here (including fleet tag, if any).
local player = "Shigernafy"; -- Player to be removed (include fleet tag, if any).
local sc = "<color;6666ff>"; -- Change the 6 digit hex code if you want a different colour.
local ec = "</color>";

sendChat(scripter, 4 , sc.."Script running..."..ec);
startTimer( 0, "returnPlayer()" ); 

function returnPlayer()
	local shipNo = shipCount();
	for i=1, shipNo do
		local targetShip = getKey(getShip(i-1));
		if getName(targetShip)==player then
			sendChat(scripter, 4 , sc.."Returning "..getName(targetShip).." to ship selection..."..ec);
			detachNoun(targetShip);
		end
	end
	startTimer( 30, "returnPlayer()" ); 
end

end

