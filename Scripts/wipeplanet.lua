enableAlerts(0)

if isServer()  then

--====!!==== WIPE A PLANET, if for some reason that's necessary ====!!====
local buildtarget = "Wesly";

for structnum = 0, structureCount( buildtarget ) - 1 do
	currNode = getStructure( buildtarget, structnum );
	detachNoun(currNode);
end


end