-- to run: must have a mod flag and a server flag; see an admin if you lack one or both
-- type /run mir.lua server
-- when you're done with some scripting (can do it anytime) and certainly before you leave, do:
-- /forward /kill *
-- that will toast any remnants of the script hanging out in memory.

-- some general notes (you can delete all these for readability if you want):
-- the format is spawnNoun("path to gadget", "location", "gadget name", "owner (whether to attach it to the object")
-- so to put a gadget on a ship, your location and owner will be identical: the ship name
-- if you want to just spawn the objects, say so you can mod them in to replace existing items, remove the owner
-- it'll just pop the gadgets next to your ship.  so it'd be spawnNoun("path", "location", "gadget name");
-- generally, don't deviate from this script much.. doing many more gadgets will not only be overpowered but also will not work (over the max #)
-- and don't "test" fun things if you don't know what you're doing
-- The way it works is for each ship there are 7 lines you should always uncomment, from 'for i=1, 3 do' to the WH spawn
-- after that, uncomment the 'for...' and end, then choose one or two gadgets to spawn.  DON'T do them all; the list is there
-- to choose from, not to use in its entirety.
-- there's also an inf spawning code.. can use this on planets too, if you chance the target ship to a planet name.. or on enemies, but...
-- most gadets that can/should be used are already included, but there's also a small list at the end of some other options
-- beyond those, you shouldn't need much else... though you can probably guess most (or draf/jack can check the path/name in code [darkspace\ports\gadgets])

enableAlerts(0);  --don't change this

if isServer() then 

--can revolt planets.  497 should be the MIR code, but if it doesn't work, use getOwner("Legion Base") or getFactionTeam(oneOfTheMirShips)
--setting the owner as below doesn't change inf allegiance, so it will cap.  It avoids a "X has revolted!" message though.  Works with empty or inf-less planets.
--~ revoltPlanet( "Praetoria", getFactionTeam("[N-14]Nexus 14") ); 
--~ revoltPlanet( "Procyon 3", 499 ); 
--~ revoltPlanet( "Earth", 0 ); 
--~ revoltPlanet( "Michelle", 497 ); 
--~ revoltPlanet( "Mycopia", 497 ); 
--~ revoltPlanet( "Dawn", 497 ); 
--~ revoltPlanet( "Tortuga", pirates );
--~ setOwner("Procyon 5", getOwner("Legion Base"));

--Can spoof server messages.  Note that <color;> is backwards; its GBR for the codes.  Also the target must be an actual player,
--so change the name at the start to fit someone who is actually in the server.
--~ sendChat( "[N-14]Nexus 14 Drone E", 1, "<color;ffffff>SERVER: [N-14]Nexus 14 Drone E has self-destructed!</color>" );
--~ sendChat( "[N-14]Nexus 14", 1, "<color;0000ff>ALERT:  MIR INVASION FORCE DETECTED IN Luyten.  All forces scramble to intercept immediately.  This is not a drill.");


--Simply uncomment the lines you want to use.  If you use Sc1 like I do (http://prdownloads.sourceforge.net/scintilla/Sc171.exe),
--you can just click control-Q over any line to uncomment it (hence the --~ .. the ~ means its an auto comment.. without that, it won't work)
--you can also select multiple ines and hit control-Q and un/comment them in bulk
-- ====/=/==== NEXUS 14 Equipment ====/=/====

for i=1, 5 do
--~ 	spawnNoun("Gadgets\\Drive - IE.prt", "[N-14]Nexus 14", "IE Drive", "[N-14]Nexus 14");
	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14", "IE Drive", "[N-14]Nexus 14");
end

for i=1, 3 do
	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14", "Reload", "[N-14]Nexus 14");
	spawnNoun("Gadgets\\reactor aux.prt", "[N-14]Nexus 14", "Reload", "[N-14]Nexus 14");
end

	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14", "Cloak", "[N-14]Nexus 14");
	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14", "Worm Hole Gen", "[N-14]Nexus 14");

for i=1, 5 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14", "Gadgetd", "[N-14]Nexus 14");
	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14", "Psi cannon "..i, "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14", "p cannon "..i, "[N-14]Nexus 14");
	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14", "f torp "..i, "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14", "am torp "..i, "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14", "Chem Laser "..i, "[N-14]Nexus 14");
	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14", "Disruptor Assault "..i, "[N-14]Nexus 14");
end


for i=1, 5 do
inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14", "Automaton", "[N-14]Nexus 14");
setOwner(inf, getOwner("Legion Base"));
end



-- ====/=/==== NEXUS 14 DRONE A Equipment ====/=/====

--~ for i=1, 3 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14 Drone A", "IE Drive", "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\Drive - IE.prt", "[N-14]Nexus 14 Drone A", "IE Drive", "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14 Drone A", "Reload", "[N-14]Nexus 14 Drone A");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone A", "Cloak", "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14 Drone A", "Worm Hole Gen", "[N-14]Nexus 14 Drone A");

--~ for i=1, 8 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14 Drone A", "Gadgetd", "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14 Drone A", "Psi cannon "..i, "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14 Drone A", "p cannon "..i, "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14 Drone A", "f torp "..i, "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14 Drone A", "am torp "..i, "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14 Drone A", "Chem Laser "..i, "[N-14]Nexus 14 Drone A");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14 Drone A", "Disruptor Assault "..i, "[N-14]Nexus 14 Drone A");
--~ end


--~ for i=1, 5 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14 Drone A", "Automaton", "[N-14]Nexus 14 Drone A");
--~ setOwner(inf, getOwner("Legion Base"));
--~ end



-- ====/=/==== NEXUS 14 DRONE B Equipment ====/=/====

--~ for i=1, 3 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14 Drone B", "IE Drive", "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\Drive - IE.prt", "[N-14]Nexus 14 Drone B", "IE Drive", "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14 Drone B", "Reload", "[N-14]Nexus 14 Drone B");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone B", "Cloak", "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14 Drone B", "Worm Hole Gen", "[N-14]Nexus 14 Drone B");

--~ for i=1, 8 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14 Drone B", "Gadgetd", "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14 Drone B", "Psi cannon "..i, "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14 Drone B", "p cannon "..i, "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14 Drone B", "f torp "..i, "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14 Drone B", "am torp "..i, "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14 Drone B", "Chem Laser "..i, "[N-14]Nexus 14 Drone B");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14 Drone B", "Disruptor Assault "..i, "[N-14]Nexus 14 Drone B");
--~ end


--~ for i=1, 5 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14 Drone B", "Automaton", "[N-14]Nexus 14 Drone B");
--~ setOwner(inf, getOwner("Legion Base"));
--~ end



-- ====/=/==== NEXUS 14 DRONE C Equipment ====/=/====

--~ for i=1, 3 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14 Drone C", "IE Drive", "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\Drive - IE.prt", "[N-14]Nexus 14 Drone C", "IE Drive", "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14 Drone C", "Reload", "[N-14]Nexus 14 Drone C");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone C", "Cloak", "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14 Drone C", "Worm Hole Gen", "[N-14]Nexus 14 Drone C");

--~ for i=1, 8 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14 Drone C", "Gadgetd", "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14 Drone C", "Psi cannon "..i, "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14 Drone C", "p cannon "..i, "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14 Drone C", "f torp "..i, "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14 Drone C", "am torp "..i, "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14 Drone C", "Chem Laser "..i, "[N-14]Nexus 14 Drone C");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14 Drone C", "Disruptor Assault "..i, "[N-14]Nexus 14 Drone C");
--~ end


--~ for i=1, 5 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14 Drone C", "Automaton", "[N-14]Nexus 14 Drone C");
--~ setOwner(inf, getOwner("Legion Base"));
--~ end



-- ====/=/==== NEXUS 14 DRONE D Equipment ====/=/====

--~ for i=1, 3 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14 Drone D", "IE Drive", "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\Drive - IE.prt", "[N-14]Nexus 14 Drone D", "IE Drive", "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14 Drone D", "Reload", "[N-14]Nexus 14 Drone D");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone D", "Cloak", "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14 Drone D", "Worm Hole Gen", "[N-14]Nexus 14 Drone D");

--~ for i=1, 8 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14 Drone D", "Gadgetd", "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14 Drone D", "Psi cannon "..i, "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14 Drone D", "P cannon "..i, "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14 Drone D", "f torp "..i, "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14 Drone D", "am torp "..i, "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14 Drone D", "Chem Laser "..i, "[N-14]Nexus 14 Drone D");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14 Drone D", "Disruptor Assault "..i, "[N-14]Nexus 14 Drone D");
--~ end


--~ for i=1, 5 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14 Drone D", "Automaton", "[N-14]Nexus 14 Drone D");
--~ setOwner(inf, getOwner("Legion Base"));
--~ end



-- ====/=/==== Nexus 14 DRONE E Equipment ====/=/====

--~ for i=1, 2 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[N-14]Nexus 14 Drone E", "IE Drive", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Drive - ame.prt", "[N-14]Nexus 14 Drone E", "IE Drive", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[N-14]Nexus 14 Drone E", "Reload", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\reactor aux.prt", "[N-14]Nexus 14 Drone E", "Reactor 1500", "[N-14]Nexus 14 Drone E");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone E", "Cloak", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[N-14]Nexus 14 Drone E", "Worm Hole Gen", "[N-14]Nexus 14 Drone E");

--~ for i=1, 2 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[N-14]Nexus 14 Drone E", "Gadgetd", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[N-14]Nexus 14 Drone E", "Psi cannon "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[N-14]Nexus 14 Drone E", "p cannon "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[N-14]Nexus 14 Drone E", "f torp "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[N-14]Nexus 14 Drone E", "am torp "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[N-14]Nexus 14 Drone E", "Chem Laser "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[N-14]Nexus 14 Drone E", "Disruptor Assault "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Railgun.prt", "[N-14]Nexus 14 Drone E", "railgun "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\EMP Cannon.PRT", "[N-14]Nexus 14 Drone E", "emp "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Gauss Gun.prt", "[N-14]Nexus 14 Drone E", "gauss "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\P Torpedo.prt", "[N-14]Nexus 14 Drone E", "ptorp "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\cl standard.prt", "[N-14]Nexus 14 Drone E", "SCL "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Elf Beam.prt", "[N-14]Nexus 14 Drone E", "elf "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Flux Cannon.prt", "[N-14]Nexus 14 Drone E", "flux "..i, "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Pulse Beam.PRT", "[N-14]Nexus 14 Drone E", "pulse "..i, "[N-14]Nexus 14 Drone E");
--~ end


--~ for i=1, 5 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "[N-14]Nexus 14 Drone E", "Automaton", "[N-14]Nexus 14 Drone E");
--~ setOwner(inf, getOwner("Legion Base"));
--~ end

-- ====/=/==== Pirate Equipment ====/=/====

--~ for i=1, 3 do
--~ 	spawnNoun("Gadgets\\Drive - AME.prt", "[RR!]Edward Rogen", "IE Drive", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\Drive - ame.prt", "[RR!]Edward Rogen", "IE Drive", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\Reload.prt", "[RR!]Edward Rogen", "Reload", "[RR!]Edward Rogen");
--~ end
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[RR!]Edward Rogen", "Cloak", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\worm hole device ii.prt", "[RR!]Edward Rogen", "Worm Hole Gen", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\jump disruptor.prt", "[RR!]Edward Rogen", "dictor", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\auto repair - mir.prt", "[RR!]Edward Rogen", "ahr", "[RR!]Edward Rogen");

	
--~ for i=1, 2 do
--~ 	spawnNoun("Gadgets\\mirv bomb.prt", "[RR!]Edward Rogen", "Gadgetd", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\psi cannon.prt", "[RR!]Edward Rogen", "Psi cannon "..i, "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\p cannon.prt", "[RR!]Edward Rogen", "p cannon "..i, "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\fusion torpedo.prt", "[RR!]Edward Rogen", "f torp "..i, "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\ecm.prt", "[RR!]Edward Rogen", "ecm", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\ecm.prt", "[RR!]Edward Rogen", "ecm", "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\am torpedo.prt", "[RR!]Edward Rogen", "am torp "..i, "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\cl heavy.prt", "[RR!]Edward Rogen", "Chem Laser "..i, "[RR!]Edward Rogen");
--~ 	spawnNoun("Gadgets\\disruptor assualt.prt", "[RR!]Edward Rogen", "Disruptor Assault "..i, "[RR!]Edward Rogen");
--~ end


--~ for i=1, 8 do
--~ inf = spawnNoun("Units\\Heavy Infantry.prt", "Doran", "Heavy Infantry", "Doran");
--~ setOwner(inf, getOwner("Earth"));
--~ end

-- ====/=/==== Miscellanea ====/=/====
--Can paste these into an above for loop and change the target name, if you want other gadets
--~ 	spawnNoun("Gadgets\\jump disruptor.prt", "[N-14]Nexus 14", "dictor", "[N-14]Nexus 14");  --use sparingly
--~ 	spawnNoun("Gadgets\\flux wave.prt", "[N-14]Nexus 14", "Flux Wave"..i, "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\armorstandard.prt", "[N-14]Nexus 14", "std armor", "[N-14]Nexus 14");  --remember: all spawns are fore mount
--~ 	spawnNoun("Gadgets\\neutron bomb.prt", "[N-14]Nexus 14", "neut", "[N-14]Nexus 14");

--~ 	spawnNoun("Gadgets\\auto repair - mir.prt", "[N-14]Nexus 14", "ahr", "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\Tractor Beam.prt", "[N-14]Nexus 14", "tractor"..i, "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\eccm.prt", "[N-14]Nexus 14", "eecm", "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\ecm.prt", "[N-14]Nexus 14", "ecm", "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\AntiMatter Drive.prt", "[N-14]Nexus 14 Drone E", "AMJD", "[N-14]Nexus 14 Drone E");
--~ 	spawnNoun("Gadgets\\Singularity Jump Drive.prt", "[N-14]Nexus 14", "SEJD", "[N-14]Nexus 14");
--~ 	spawnNoun("Gadgets\\cloaking device.prt", "[N-14]Nexus 14 Drone E", "Cloak", "[N-14]Nexus 14 Drone E");
	

-- 0 ugto
-- 1 icc
-- 2 kluth
-- 498 pirates
-- 497 Mir 
-- 499 Unknown  (monsters or neutral)

end 
