enableAlerts(0);  -- Don't change this.

if isServer() then 

local scripter = "[N-14]Nexus 14 Drone E";  -- Your name here (including fleet tag, if any).
local person = "[PB]Darth Jaybud";  -- Player or object's name here (including fleet tag, if any).
local target = "[PB]Darth Jaybud"; -- Place or player to move to (include fleet tag, if any).
local sc = "<color;6666ff>"; -- Change the 6 digit hex code if you want a different colour.
local ec = "</color>";

function moveNoun()
	breakOrbit( person );
	x,y,z = getPosition( target );
	sendChat(scripter, 4 , sc.."Got position ("..x..", "..y..", "..z..") of  "..person.."..."..ec);
	sendChat(scripter, 4 , sc.."Attempting to move to target..."..ec);
	y = y + 2500;
	setPosition( person, x, y, z );
end

moveNoun();

sendChat(scripter, 4 , sc.."Script finished..."..ec);

end