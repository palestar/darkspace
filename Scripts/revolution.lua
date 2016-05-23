enableAlerts(0); 

if isServer() then 
	
local scripter = "Doran"; -- Insert scripter name here 
local planetName = "Mycopia";
local factionID ="3"; -- 0=Neutral, 1=ICC, 2=K'Luth, 5=UGTO 4=Mir 3=Pirates
--~ local targetFleetID =""; --Use if a specific planet is supposed to be owned by a fleet (IE ICC:ICC, UGTO:GTN etc.). Put in the name of a target that has the fleet you wish to assign, leave blank if none needed. 

--~ function selectTarget() -- list of planets to revolt
--~ 	local locTable = { [1]="Dalexia", [2]="Praetoria", [3]="Procyon 5", [4]="Procyon 7", [5]="Kablikistan"};
--~ 	local n = randomNumber(1, table.getn(locTable);
--~ 	return locTable[n];
--~ end

function revoltTarget() 
local locTable = { [1]="Mycopia", [2]="Wecamp", [3]="Ihzack", [4]="Dawn", [5]="Zoca", [6]="Necroph", [6]="Michelle"};
	for index = table.getn(locTable), 1, -1 do
		local value = locTable[index];
--~ 			if isNoun(value) then
				planetName = value;
				revoltPlanet( planetName, factionID); 
--~ 			end
	end
end 

revoltTarget(); 

end 
