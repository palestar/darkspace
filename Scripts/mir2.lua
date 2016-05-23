 enableAlerts(0); 

if isServer() then 

local scripter = "[Admin]Mersenne Twister" -- change to your name 
local target = scripter
local full_mount = 983040 
local fore_mount = 65536 
local left_mount = 262144 
local right_mount = 131072 
local aft_mount = 524288 

function Upgrade() 

for f = 1,3 do 
spawnNoun("Gadgets\\Drive - AME.prt", target, "AME"..f, target); 
end 

spawnNoun("Gadgets\\worm hole device ii.prt", target, "Wormhole Device II", target); 

spawnNoun("Gadgets\\cloaking device.prt", target, "Cloaking Device", target); 

spawnNoun("Gadgets\\Build - ICC.PRT", target, "Build", target); 

for f = 1,3 do 
spawnNoun("Gadgets\\reactor aux.prt", target, "Auxilliary Generator"..f, target); 
end 

for f = 1,3 do 
local gadgetid = spawnNoun("Gadgets\\mouth beam.prt", target, "Mouth Beam"..f, target); 
setGadgetMount(gadgetid, full_mount); 
end 

for f = 1,3 do 
local gadgetid = spawnNoun("Gadgets\\p cruise missile.prt", target, "Proton Cruise Missile"..f, target); 
setGadgetMount(gadgetid, full_mount); 
end 

--~ for f = 1,16 do 
--~ local gadgetid = spawnNoun("Gadgets\\Fighter Bay Squid- Pirate.prt", target, "Squibb"..f, target); 
--~ setGadgetMount(gadgetid, full_mount); 
--~ end 
	

--~ for f = 1,2 do 
--~ local gadgetid = spawnNoun("Gadgets\\qstorpedo.prt", target, "Quantum Singularity Torpedo"..f, target); 
--~ setGadgetMount(gadgetid, fore_mount); 
--~ end 

for f = 1,4 do 
spawnNoun("Gadgets\\reload.prt", target, "Reload"..f, target); 
end 

for f = 1,4 do 
spawnNoun("Gadgets\\auto repair.prt", target, "Auto Hull Repair"..f, target); 
end 

for f = 1,2 do 
local gadgetid = spawnNoun("Gadgets\\armorstandard.prt", target, "Standard Armor"..f, target); 
setGadgetMount(gadgetid, fore_mount); 
end 

for f = 1,2 do 
local gadgetid = spawnNoun("Gadgets\\armorstandard.prt", target, "Standard Armor"..f, target); 
setGadgetMount(gadgetid, left_mount); 
end 

for f = 1,2 do 
local gadgetid = spawnNoun("Gadgets\\armorstandard.prt", target, "Standard Armor"..f, target); 
setGadgetMount(gadgetid, right_mount); 
end 

for f = 1,2 do 
local gadgetid = spawnNoun("Gadgets\armorstandard.prt", target, "Standard Armor"..f, target); 
setGadgetMount(gadgetid, aft_mount); 
end 

end 

Upgrade(); 

end
