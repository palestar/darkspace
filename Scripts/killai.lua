enableAlerts(0);  --don't change this

if isServer() then 

local scripter = "Doran"; -- Insert scripter name here

-- Set color keys, format BBGGRR.

local lightred = "<color;6666ff>";
local ec = "</color>";

function killAI()
	local shipNo = shipCount();
	for i=1, shipNo do
		local targetShip = getKey(getShip(i-1));
		if isAIControlled(targetShip) then
			sendChat(scripter, 4 , lightred.."Destroying "..getName(targetShip).."..."..ec);
			detachNoun(targetShip);
		end
	end
end

killAI();

end

