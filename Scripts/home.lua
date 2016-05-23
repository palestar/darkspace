enableAlerts(0)

if isServer()  then

-- ====!!==== BUILD PATTERN FOR UGTO HOME PLANETS ====!!====

--~ local buildtarget = "Earth";


--~ spawnNoun("Structures\\ugto\\colony hub.prt", buildtarget, "UN Headquarters", buildtarget);

--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "US Robotics Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Lockheed Martin Central Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "EADS Weapons Development", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Boeing Weapons Systems", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Northrop Grumman Development", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Raytheon Weaponry", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "BAE Systems", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Los Alamos Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Max Planck Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Dyson Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Oxford Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Sydney Applied Sciences Institute", buildtarget);

--~ spawnNoun("Structures\\common\\shipyard.prt", buildtarget, "Earth Spacedocks", buildtarget);
--~ spawnNoun("Structures\\ugto\\starport.prt", buildtarget, "Earth Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Terran Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 16 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 	end

--~ spawnNoun("Structures\\ugto\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);
--~ spawnNoun("Structures\\ugto\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,64 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end


--~ local buildtarget = "Luna";


--~ spawnNoun("Structures\\ugto\\colony hub.prt", buildtarget, "Luna Central Command", buildtarget);

--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Boeing Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Thales Nederland Lunar Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Rosoboronexport Weaponry Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "General Dynamics Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Honeywell Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Halliburton Factory", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Lockheed Martin Plant", buildtarget);
--~ spawnNoun("Structures\\ugto\\factory.prt", buildtarget, "Levdan Procurement", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Almaden Research Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Fraunhofer Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Kavli Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "TRIUMF Center", buildtarget);

--~ spawnNoun("Structures\\ugto\\starport.prt", buildtarget, "Luna Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Lunar Advanced Reconstuction Services", buildtarget);
--~ local depot2 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Lunar Advanced Reconstuction Services", buildtarget);
--~ local depot3 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Lunar Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 6 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 		spawnNoun("Gadgets\\reload.prt", depot2, "reload", depot2);
--~ 		spawnNoun("Gadgets\\reload.prt", depot3, "reload", depot3);
--~ 	end

--~ spawnNoun("Structures\\ugto\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);
--~ spawnNoun("Structures\\ugto\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,64 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end



-- ====!!==== ``BUILD PATTERN FOR ICC HOME PLANETS`` ====!!====

--~ local buildtarget = "Exathra";


--~ spawnNoun("Structures\\icc\\colony hub.prt", buildtarget, "ICC Headquarters", buildtarget);

--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "High-rise Apartment", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weaponry Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weaponry Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weaponry Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Shi Jea Weapons Systems", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Carrolls Defense Systems", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Exathran Weaponry", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Righteous Development Systems", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Triton Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Horatio Bulvar Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Freedman Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Shi Jea Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Antigone Sciences Research", buildtarget);

--~ spawnNoun("Structures\\common\\shipyard.prt", buildtarget, "Exathra Spacedocks", buildtarget);
--~ spawnNoun("Structures\\icc\\starport.prt", buildtarget, "Exathra Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Exathran Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 16 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 	end

--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);
--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,64 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end



--~ local buildtarget = "Minuete";


--~ spawnNoun("Structures\\icc\\colony hub.prt", buildtarget, "Minuete Central Command", buildtarget);

--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Biosphere", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Biosphere Condenser", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weapons Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weapons Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "ICC Weapons Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "General Defense Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Magnae Factory", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Eslante Weaponry", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Ingmann Plant", buildtarget);
--~ spawnNoun("Structures\\icc\\factory.prt", buildtarget, "Waldreich Procurement", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Almduddler Research Laboratories", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Bifidus Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Perisrup Institute", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "PYMPN Center", buildtarget);

--~ spawnNoun("Structures\\icc\\starport.prt", buildtarget, "Minuete Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Minuete Advanced Reconstuction Services", buildtarget);
--~ local depot2 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Minuete Advanced Reconstuction Services", buildtarget);
--~ local depot3 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Minuete Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 8 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 		spawnNoun("Gadgets\\reload.prt", depot2, "reload", depot2);
--~ 		spawnNoun("Gadgets\\reload.prt", depot3, "reload", depot3);
--~ 	end

--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);
--~ spawnNoun("Structures\\icc\\barracks.prt", buildtarget, "Armed Forces Recruiter", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,64 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end



--~ -- ====!!==== ``BUILD PATTERN FOR KLUTH HOME PLANETS`` ====!!====

--~ local buildtarget = "Sag Hothha";


--~ spawnNoun("Structures\\kluth\\colony hub.prt", buildtarget, "K'Luth Queen Lair", buildtarget);

--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Nourishment Assembler", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Nourishment Assembler", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);

--~ spawnNoun("Structures\\common\\shipyard.prt", buildtarget, "Sag Hothhan Spacedocks", buildtarget);
--~ spawnNoun("Structures\\kluth\\starport.prt", buildtarget, "Sag Hothhan Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Hothhan Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 6 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 	end

--~ spawnNoun("Structures\\kluth\\barracks.prt", buildtarget, "Warrior Training Grounds", buildtarget);
--~ spawnNoun("Structures\\kluth\\barracks.prt", buildtarget, "Warrior Training Grounds", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,92 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end



--~ local buildtarget = "Leaph";


--~ spawnNoun("Structures\\kluth\\colony hub.prt", buildtarget, "Leaph Central Command", buildtarget);

--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Nourishment Assembler", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Nourishment Assembler", buildtarget);
--~ spawnNoun("Structures\\common\\biosphere condenser.prt", buildtarget, "Nourishment Assembler", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\Dome.prt", buildtarget, "Drone Habitat", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\common\\variance generator.prt", buildtarget, "Variance Generator", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);
--~ spawnNoun("Structures\\kluth\\factory.prt", buildtarget, "Psionic Shaping Facility", buildtarget);

--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);
--~ spawnNoun("Structures\\common\\cortex nexus.prt", buildtarget, "Collective Knowledge Storage", buildtarget);

--~ spawnNoun("Structures\\kluth\\starport.prt", buildtarget, "Leaph Central Market", buildtarget);

--~ local depot = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Leaph Advanced Reconstuction Services", buildtarget);
--~ local depot2 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Leaph Advanced Reconstuction Services", buildtarget);
--~ local depot3 = spawnNoun("Structures\\common\\depot.prt", buildtarget, "Leaph Advanced Reconstuction Services", buildtarget);
--~ 	for n=1, 6 do
--~ 		spawnNoun("Gadgets\\reload.prt", depot, "reload", depot);
--~ 		spawnNoun("Gadgets\\reload.prt", depot2, "reload", depot2);
--~ 		spawnNoun("Gadgets\\reload.prt", depot3, "reload", depot3);
--~ 	end

--~ spawnNoun("Structures\\kluth\\barracks.prt", buildtarget, "Warrior Training Grounds", buildtarget);
--~ spawnNoun("Structures\\kluth\\barracks.prt", buildtarget, "Warrior Training Grounds", buildtarget);

--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);
--~ spawnNoun("Structures\\common\\mantle extractor.prt", buildtarget, "Mantle Extractor", buildtarget);


--~ for n=1,64 do
--~ 	local inf = spawnNoun("Units\\Heavy Infantry.prt", buildtarget, "Heavy Infantry", buildtarget);
--~ 	setOwner(inf, getOwner(buildtarget));
--~ end

end