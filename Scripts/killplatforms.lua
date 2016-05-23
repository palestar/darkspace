enableAlerts(0);  --don't change this

if isServer() then 

local scripter = "BackSlash *Jack*"; -- Insert scripter here

-- Set color keys, format BBGGRR.

local lightred = "<color;6666ff>";
local ec = "</color>";

function killPlatforms()
	sendChat(scripter, 4 , lightred.."Script starting...");
	local shipNo = shipCount();
		for i=1, shipNo do
		local targetShip = getKey(getShip(i-1));
		if getShipType(targetShip) >= 11 then
			sendChat(scripter, 4 , lightred.."[SCRIPT] Destroying "..getName(targetShip).."..."..ec);
			detachNoun(targetShip);	
		end
	end
	sendChat(scripter, 4 , lightred.."Script ending...");
end

killPlatforms();

end

