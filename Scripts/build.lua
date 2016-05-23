enableAlerts(0)

if isServer()  then

-- ====!!==== BUILD PATTERN FOR PIRATE (SUPER) PLANET ====!!====

--~ local buildtarget = "Tortuga";

--~ for i=1,4  do
--~ spawnNoun("Structures\\ugto\\colony hub.prt", buildtarget, "Pirate Bay", buildtarget);
--~ end

--~ for i=1,5  do
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Grog Brewery", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Wench Bar", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Wench Bar", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Wood Stove", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Industrial Espionage Station", buildtarget);
--~ end
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Wood Stove", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Industrial Espionage Station", buildtarget);
--~ spawnNoun("Structures\\common\\shipyard.prt", buildtarget, "Lagoon", buildtarget);
--~ spawnNoun("Structures\\ugto\\starport.prt", buildtarget, "Black Market", buildtarget);

--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Kluth Cutlass Shoppe", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Cutlass Shoppe", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "UGTO Cutlass Shoppe", buildtarget);

--~ local shield = spawnNoun("Structures\\icc\\shield generator.prt", buildtarget, "Iron Plating", buildtarget);
--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Massage Parlour", buildtarget);
--~ local sens = spawnNoun("Structures\\common\\sensor base.prt", buildtarget, "Crow's Nest", buildtarget);
--~ 	for n=1, 8 do
--~ 		spawnNoun("Gadgets\\eccm.prt", sens, "eccm", sens);
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 	end
--~ 	spawnNoun("Gadgets\\planetary shield.prt", shield, "Shield", shield);
--~ 	spawnNoun("Gadgets\\scanner.prt", sens, "scanner", sens);

--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "Buccaneer Recruiter", buildtarget);

--~ for i=1, 10 do
--~ local doom = spawnNoun("Structures\\common\\defense base.prt", buildtarget, "Rusty Cannon", buildtarget);
--~ 	for n=1, 5 do
--~ 		spawnNoun("Gadgets\\pulse beam.prt", doom, "beam", doom);
--~ 		spawnNoun("Gadgets\\p cannon.prt", doom, "p cannon", doom);
--~ 		spawnNoun("Gadgets\\ar missile.prt", doom, "missile", doom);
--~ 		spawnNoun("Gadgets\\fusion torpedo.prt", doom, "torp", doom);
--~ 		spawnNoun("Gadgets\\fusion reactor 1500.prt", doom, "nrg", doom);
--~ 		spawnNoun("Gadgets\\fusion reactor 1500.prt", doom, "nrg", doom);
--~ 	end
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", doom, "ass ruptor", doom);
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", doom, "ass ruptor", doom);
--~ 	spawnNoun("Gadgets\\qstorpedo.prt", doom, "QST", doom);
--~ 	spawnNoun("Gadgets\\ioncannon.prt", doom, "IC", doom);
--~ 	spawnNoun("Gadgets\\stellarincinerator.prt", doom, "SI", doom);
--~ end


--~ for n=1,124 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Buccaneer", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end



-- ====!!==== ``BUILD PATTERN FOR (SUPER) MIR`` ====!!====

--~ local buildtarget = "Legion Base";

--~ spawnNoun("Structures\\ugto\\colony hub.prt", buildtarget, "Legion Nexus Hub", buildtarget);

--~ for i=1,5  do
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Storage Facility", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Storage Facility", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Cortex Nexus", buildtarget);
--~ end

--~ spawnNoun("Structures\\common\\shipyard.prt", buildtarget, "MIR Assembly Facility", buildtarget);
--~ spawnNoun("Structures\\ugto\\starport.prt", buildtarget, "MIR Junkyard", buildtarget);

--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Kluth Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "UGTO Factory", buildtarget);

--~ local shield = spawnNoun("Structures\\icc\\shield generator.prt", buildtarget, "MIR Planetary Defense Grid", buildtarget);
--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "MIR Regeneration Bay", buildtarget);
--~ local sens = spawnNoun("Structures\\common\\sensor base.prt", buildtarget, "MIR Sensory Outpost", buildtarget);
--~ 	for n=1, 6 do
--~ 		spawnNoun("Gadgets\\eccm.prt", sens, "eccm", sens);
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 	end
--~ 	spawnNoun("Gadgets\\planetary shield.prt", shield, "Shield", shield);
--~ 	spawnNoun("Gadgets\\scanner.prt", sens, "scanner", sens);

--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "MIR Automaton Assembly Plant", buildtarget);

--~ for i=1, 6 do
--~ local doom = spawnNoun("Structures\\common\\defense base.prt", buildtarget, "Doomslingers", buildtarget);
--~ 	for n=1, 4 do
--~ 		spawnNoun("Gadgets\\pulse beam.prt", doom, "beam", doom);
--~ 		spawnNoun("Gadgets\\psi cannon.prt", doom, "p cannon", doom);
--~ 		spawnNoun("Gadgets\\ar missile.prt", doom, "missile", doom);
--~ 		spawnNoun("Gadgets\\fusion torpedo.prt", doom, "torp", doom);
--~ 		spawnNoun("Gadgets\\fusion reactor 1500.prt", doom, "nrg", doom);
--~ 		spawnNoun("Gadgets\\fusion reactor 1500.prt", doom, "nrg", doom);
--~ 		spawnNoun("Gadgets\\fusion reactor 1500.prt", doom, "nrg", doom);
--~ 	end
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", doom, "ass ruptor", doom);
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", doom, "ass ruptor", doom);
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", doom, "ass ruptor", doom);
--~ 	if i==4 then
--~ 		spawnNoun("Gadgets\\qstorpedo.prt", doom, "QST", doom);
--~ 	spawnNoun("Gadgets\\ioncannon.prt", doom, "IC", doom);
--~ 	spawnNoun("Gadgets\\stellarincinerator.prt", doom, "SI", doom);
--~ 	elseif i==9 then
--~ 		spawnNoun("Gadgets\\qstorpedo.prt", doom, "QST", doom);
--~ 	spawnNoun("Gadgets\\ioncannon.prt", doom, "IC", doom);
--~ 	spawnNoun("Gadgets\\stellarincinerator.prt", doom, "SI", doom);
--~ 	end
--~ end
--~ 	spawnNoun("Gadgets\\qstorpedo.prt", doom, "QST", doom);
--~ 	spawnNoun("Gadgets\\ioncannon.prt", doom, "IC", doom);
--~ 	spawnNoun("Gadgets\\stellarincinerator.prt", doom, "SI", doom);

--~ for n=1,84 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Automaton", buildtarget);
--~ 	setOwner(inf, getOwner("Legion Base"));
--~ end





--====!!==== WIPE A PLANET, if for some reason that's necessary ====!!====
--~ local buildtarget = "Exathra";

--~ for structnum = 0, structureCount( buildtarget ) - 1 do
--~ 	currNode = getStructure( buildtarget, structnum );
--~ 	detachNoun(currNode);
--~ end


end