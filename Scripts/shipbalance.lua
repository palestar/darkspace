-----------------------------------
---- Ship Balance Tracker v0.5 ----
---- Robert Kelford, 2008 ---
-----------------------------------

enableAlerts(0);  --don't change this

ships ={};

ships[100]={name="Anti-sensor Scout", path="ships\\ugto\\scout\\st-1 anti-sensor\\st-1 anti-sensor scout.prt"};
ships[101]={name="Assault Corvette", path="ships\\ugto\\scout\\st-2 assault\\st-2 assault scout.prt"};
ships[102]={name="Long Range Scout", path="ships\\ugto\\scout\\st-4 long range\\st-4 long range scout.prt"};
ships[103]={name="Bomber Corvette", path="ships\\ugto\\scout\\st-5 bomber\\st-5 bomber scout.prt"};
ships[110]={name="ST-5 Bomber Frigate", path="ships\\ugto\\frigate\\st-5 bomber\\sc_st5.prt"};
ships[111]={name="ST-6 Interceptor Frigate", path="ships\\ugto\\frigate\\st-6 interceptor\\sc_st6.prt"};
ships[112]={name="ST-7 Minelayer Frigate", path="ships\\ugto\\frigate\\st-7 minelayer\\sc_st7.prt"};
ships[113]={name="ST-8 Harrier Frigate", path="ships\\ugto\\frigate\\st-8 harrier\\sc_st8.prt"};
ships[120]={name="ST-10 Gunboat Destroyer", path="ships\\ugto\\destroyer\\st-10 gunboat\\sc_st10.prt"};
ships[121]={name="ST-11 Missile Destroyer", path="ships\\ugto\\destroyer\\st-11 missile buster\\sc_st11.prt"};
ships[122]={name="ST-14 Assault Destroyer", path="ships\\ugto\\destroyer\\st-14 assault\\sc_st14.prt"};
ships[123]={name="ST-18 Picket Destroyer", path="ships\\ugto\\destroyer\\st-18 picket\\sc_st18.prt"};
ships[130]={name="ST-74 Torpedo Cruiser", path="ships\\ugto\\cruiser\\st-74 torpedo cruiser\\sc_st74.prt"};
ships[131]={name="ST-75 Interdictor", path="ships\\ugto\\cruiser\\st-75 interdictor\\sc_st75.prt"};
ships[132]={name="ST-76 Battle Cruiser", path="ships\\ugto\\cruiser\\st-76 battle cruiser\\sc_st76.prt"};
ships[133]={name="ST-79 Missile Cruiser", path="ships\\ugto\\cruiser\\st-79 missile cruiser\\sc_st79.prt"};
ships[134]={name="ST-80 Bomber Cruiser", path="ships\\ugto\\cruiser\\st-80 bomber\\sc_st80.prt"};
ships[135]={name="ST-79a Missile Cruiser (Beta)", path="ships\\ugto\\cruiser\\st-79a missile cruiser\\sc_st79a.prt"};
ships[140]={name="Agincourt Carrier Dreadnought", path="ships\\ugto\\dreadnought\\agincourt carrier\\sc_agincourt.prt"};
ships[141]={name="ST-101 Carrier Dreadnought", path="ships\\ugto\\dreadnought\\st-101 super carrier\\sc_st101.prt"};
ships[142]={name="ST-105 Battle Dreadnought", path="ships\\ugto\\dreadnought\\st-105 battle platform\\sc_st105.prt"};
ships[143]={name="ST-106 Command Dreadnought", path="ships\\ugto\\dreadnought\\st-106 command platform\\sc_st106.prt"};
ships[144]={name="ST-111 Elite Assault Dreadnought", path="ships\\ugto\\dreadnought\\st-111 elite assault\\sc_st111.prt"};
ships[150]={name="HT-1001a Support Station", path="ships\\ugto\\station\\ht 1001a station\\ht 1001a station.prt"};
ships[151]={name="HT-1001b Battle Station", path="ships\\ugto\\station\\ht 1001b station\\ht 1001b station.prt"};
ships[152]={name="HT-1010c Command Station", path="ships\\ugto\\station\\ht 1010c station\\sc ht1010c.prt"};
ships[160]={name="ST-7000 Engineer", path="ships\\ugto\\engineering\\st-7000\\sc_st7000.prt"};
ships[161]={name="ST-7010 Heavy Engineer", path="ships\\ugto\\engineering\\st-7010\\sc_st7010.prt"};
ships[162]={name="ST-8005 Supply Ship", path="ships\\ugto\\supply\\st-8005 supply ship\\st-8005 supply ship.prt"};
ships[163]={name="ST-8010 Heavy Supply", path="ships\\ugto\\supply\\st-8010 supply\\st-8010 supply ship.prt"};
ships[170]={name="SE-7501 Extractor", path="ships\\ugto\\miner\\se-7501\\sc_se7501.prt"};
ships[171]={name="ST-201 Light Transport", path="ships\\ugto\\transport\\st-201 transport\\st-201 light transport.prt"};
ships[172]={name="ST-211 Heavy Transport", path="ships\\ugto\\transport\\st 211 transport\\st-211 heavy transport.prt"};

ships[200]={name="Sensor Scout", path="ships\\icc\\scout\\m21 a\\sc_m21a.prt"};
ships[201]={name="Recon Scout", path="ships\\icc\\scout\\m21 b\\sc_m21b.prt"};
ships[202]={name="Stealth Scout", path="ships\\icc\\scout\\m21 c\\sc_m21c.prt"};
ships[203]={name="Bomber Scout", path="ships\\icc\\scout\\m22 b\\sc_m22b.prt"};
ships[210]={name="Sensor Frigate", path="ships\\icc\\frigate\\m40s\\sc_m40s.prt"};
ships[211]={name="Bomber Frigate", path="ships\\icc\\frigate\\m42b\\sc_m42b.prt"};
ships[212]={name="Assault Frigate", path="ships\\icc\\frigate\\m44a\\sc_m44a.prt"};
ships[213]={name="Minelayer Frigate", path="ships\\icc\\frigate\\m45b\\sc_m45b.prt"};
ships[220]={name="Combat Destroyer", path="ships\\icc\\destroyer\\m190 a\\sc_m190a.prt"};
ships[221]={name="Bomber Destroyer", path="ships\\icc\\destroyer\\m190 b\\sc_m190b.prt"};
ships[222]={name="Missile Destroyer", path="ships\\icc\\destroyer\\m192 f\\sc_m192f.prt"};
ships[223]={name="Minelayer Destroyer", path="ships\\icc\\destroyer\\old\\m194a\\sc_m194a.prt"};
ships[224]={name="Escort Destroyer", path="ships\\icc\\destroyer\\old\\m194 e\\sc_m194e.prt"};
ships[230]={name="Heavy Cruiser", path="ships\\icc\\cruiser\\m230 a\\sc_m230a.prt"};
ships[231]={name="Border Cruiser", path="ships\\icc\\cruiser\\m231 b\\sc_m231b.prt"};
ships[232]={name="Interdictor", path="ships\\icc\\cruiser\\m235 i\\sc_m235i.prt"};
ships[233]={name="Jump Cruiser", path="ships\\icc\\cruiser\\m239 j\\sc_m239j.prt"};
ships[234]={name="Assault Cruiser", path="ships\\icc\\cruiser\\m245 a\\sc_m245a.prt"};
ships[235]={name="Missile Cruiser", path="ships\\icc\\cruiser\\m247 m\\sc_m247m.prt"};
ships[236]={name="Carrier Cruiser", path="ships\\icc\\cruiser\\m248c\\sc_m248c.prt"};
ships[237]={name="Wormhole Cruiser", path="ships\\icc\\cruiser\\m251 w\\sc_m251w.prt"};
ships[240]={name="Combat Dreadnought", path="ships\\icc\\dreadnought\\m400-a\\sc_m400a.prt"};
ships[241]={name="Bomber Dreadnought", path="ships\\icc\\dreadnought\\m400-b\\sc_m400b.prt"};
ships[242]={name="Assault Dreadnought", path="ships\\icc\\dreadnought\\m410-a\\sc_m410a.prt"};
ships[243]={name="Missile Dreadnought", path="ships\\icc\\dreadnought\\old\\m300\\sc_m300.prt"};
ships[244]={name="Command Carrier", path="ships\\icc\\dreadnought\\old\\m310\\sc_m310.prt"};
ships[245]={name="M300a Missile Dreadnought (Beta)", path="ships\\icc\\dreadnought\\old\\m300-a\\sc_m300-a.prt"};
ships[250]={name="Support Station", path="ships\\icc\\station\\m2200 a\\sc_m2200a.prt"};
ships[251]={name="", path="ships\\icc\\station\\m2200 d\\sc_m2200d.prt"}; -- ICC
ships[252]={name="", path="ships\\icc\\station\\m2250\\sc_m2250.prt"}; -- ICC
ships[253]={name="", path="ships\\icc\\station\\m2300\\sc_m2300.prt"}; -- ICC
ships[260]={name="MR-110 Engineer", path="ships\\icc\\engineering\\mr 110\sc_mr110.prt"};
ships[261]={name="MR-115 Engineer", path="ships\\icc\\engineering\\mr 115\sc_mr115.prt"};
ships[262]={name="MR-125 Engineer", path="ships\\icc\\engineering\\mr 125\sc_mr125.prt"};
ships[263]={name="MS-40a Light Supply", path="ships\\icc\\supply\\m s40 a\\sc_ms40a.prt"};
ships[264]={name="MS-42a Heavy Supply", path="ships\\icc\\supply\\m s42 a\\sc_ms42a.prt"};
ships[270]={name="ME-1522 Extractor", path="ships\\icc\\miner\\me 1522\\sc_me1522.prt"};
ships[271]={name="M2111 Light Transport", path="ships\\icc\\transport\\m2111 transport\\sc_m2111.prt"};
ships[272]={name="M2122 Heavy Transport", path="ships\\icc\\transport\\m2111 transport\\sc_m2122.prt"};


ships[300]={name="Extruder", path="ships\\kluth\\scout\\extruder\\extruder scout.prt"}; -- Kluth Bomber Scout
ships[301]={name="Talon", path="ships\\kluth\\scout\\talon\\talon scout.prt"};
ships[302]={name="Fang", path="ships\\kluth\\scout\\fang\\fang scout.prt"};
ships[310]={name="Beak", path="ships\\kluth\\frigate\\beak\\beak_sc.prt"};
ships[311]={name="Instars", path="ships\\kluth\\frigate\\instars\\instars_sc.prt"}; -- Kluth Minelayer Frig
ships[312]={name="Nymph", path="ships\\kluth\\frigate\\nymph\\nymph.prt"};
ships[313]={name="Proboscis", path="ships\\kluth\\frigate\\proboscis\\prob ship.prt"}; -- Kluth Bomber Frig
ships[320]={name="Drainer", path="ships\\kluth\\destroyer\\drainer\\drainer destroyer.prt"};
ships[321]={name="Shell", path="ships\\kluth\\destroyer\\shell\\shell.prt"};
ships[322]={name="Stinger", path="ships\\kluth\\destroyer\\stinger\\stinger.prt"};
ships[323]={name="Claw", path="ships\\kluth\\destroyer\\claw\\claw.prt"};
ships[330]={name="Clavate", path="ships\\kluth\\cruiser\\clavate\\clavate cruiser.prt"}; -- Kluth Bomber Cruiser
ships[331]={name="Parasite", path="ships\\kluth\\cruiser\\parasite\\parasite cruiser.prt"};
ships[332]={name="Peircer", path="ships\\kluth\\cruiser\\peircer\\peircer cruiser.prt"}; -- Kluth Interdictor
ships[333]={name="Scarab", path="ships\\kluth\\cruiser\\scarab\\scarab cruiser.prt"};
ships[334]={name="Scale", path="ships\\kluth\\cruiser\\scale\\scale cruiser.prt"};
ships[340]={name="Brood", path="ships\\kluth\\dreadnought\\brood\\brood_sc.prt"};
ships[341]={name="Ganglia", path="ships\\kluth\\dreadnought\\ganglia\\ganglia.prt"};
ships[342]={name="Krill", path="ships\\kluth\\dreadnought\\krill\\krill_sc.prt"};
ships[343]={name="Mandible", path="ships\\kluth\\dreadnought\\mandible\\mandible_sc.prt"};
ships[344]={name="Siphon", path="ships\\kluth\\dreadnought\\siphon\\siphon_sc.prt"};
ships[350]={name="Hive", path="ships\\kluth\\station\\hive\\k’luth hive.prt"};
ships[351]={name="Colony", path="ships\\kluth\\station\\colony\\colony_sc.prt"};
ships[352]={name="Nest", path="ships\\kluth\\station\\nest\\nest_sc.prt"};
ships[360]={name="Drone Engineer", path="ships\\kluth\\engineer\\drone\\builder.prt"}; -- Kluth Engineer
ships[361]={name="Worker", path="ships\\kluth\\supply\\worker\\worker.prt"}; -- Kluth Light Suppy
ships[362]={name="Advanced Worker", path="ships\\kluth\\supply\\advanced worker\\advanced worker.prt"}; -- Kluth Medium Suppy
ships[363]={name="Ultimate Worker", path="ships\\kluth\\supply\\ultimate worker\\ult worker.prt"}; -- Kluth Heavy Suppy
ships[370]={name="Extractor", path="ships\\kluth\\miner\\extractor\\extractor.prt"};
ships[371]={name="Carrier", path="ships\\kluth\\transport\\carrier\\carrier.prt"};
ships[372]={name="Advanced Carrier", path="ships\\kluth\\transport\\advanced carrier\\adv carrier.prt"};

-- ship[420]={name="", path="ships\\mi\\destroyer\\"}; -- MIR Dessy
ships[430]={name="Legion Cruiser", path="ships\\mi\\cruiser\\legion\\sc_legion.prt"}; -- MIR Cruiser
ships[440]={name="Longhead", path="ships\\mi\\dreadnought\\longhead\\sc_dread.prt"}; -- MIR Dread
ships[460]={name="Assembler", path="ships\\mi\\engineering\\assembler\sc_assembler.prt"}; -- MIR Engy
ships[461]={name="Forager", path="ships\\mi\\supply\\forager\\sc_forager.prt"}; -- MIR Supply
ships[470]={name="Conveyor", path="ships\\mi\\transport\\conveyor\\sc_conveyor.prt"}; -- MIR Trans


ships[500]={name="Ray A", path="ships\\pirate\\scout\\ray a\\sc_ray_a.prt"};
ships[501]={name="Ray AS", path="ships\\pirate\\scout\\ray as\\sc_ray_as.prt"};
ships[502]={name="Ray E", path="ships\\pirate\\scout\\ray e\\sc_ray_e.prt"};
ships[510]={name="Corvette", path="ships\\pirate\\frigate\\corvette\\sc_corvette.prt"};
ships[511]={name="Raider", path="ships\\pirate\\frigate\\raider\\sc_raider.prt"};
ships[520]={name="C208cvm Destroyer", path="ships\\pirate\\destroyer\\c208cvm\\sc_c208cvm.prt"};
ships[560]={name="Tender", path="ships\\pirate\\supply\\tender\\sc_tender.prt"};
ships[570]={name="Hauler", path="ships\\pirate\\transport\\hauler\\sc_hauler.prt"};


ships[600]={name="Crystalline Entity", path="monsters\\crystal entity\\sc_crystal.prt"};
ships[601]={name="Energy Entity", path="monsters\\energy\\sc_energy.prt"};
ships[610]={name="Shadow Gaifen", path="monsters\\gaifen energy\\sc_gaifen.prt"};
ships[611]={name="Gaifen", path="monsters\\gaifen opaque\\gaifen monster.prt"};

ships[700]={name="ICC Weapon Platform", path="ships\\icc\\platform\iccweapon\\s_iccweapon.prt"};

-- ships[430]={name="Legion Cruiser", path="ships\\mi\\cruiser\\legion\\sc_legion.prt"}; -- MIR Cruiser
-- ships[440]={name="Longhead", path="ships\\mi\\dreadnought\\longhead\\sc_dread.prt"}; -- MIR Dread
-- ships[460]={name="Assembler", path="ships\\mi\\engineering\\assembler\sc_assembler.prt"}; -- MIR Engy
-- ships[461]={name="Forager", path="ships\\mi\\supply\\forager\\sc_forager.prt"}; -- MIR Supply
-- ships[470]={name="Conveyor", path="ships\\mi\\transport\\conveyor\\sc_conveyor.prt"}; -- MIR Trans

-- ships[502]={name="Ray E", path="ships\\pirate\\scout\\ray e\\sc_ray_e.prt"};
-- ships[510]={name="Corvette", path="ships\\pirate\\frigate\\corvette\\sc_corvette.prt"};
-- ships[511]={name="Raider", path="ships\\pirate\\frigate\\raider\\sc_raider.prt"};
-- ships[520]={name="C208cvm Destroyer", path="ships\\pirate\\destroyer\\c208cvm\\sc_c208cvm.prt"};


if isServer() then 

------------------------------------
-- Main Script Variables ----
------------------------------------

local scripter = "Doran"; -- Insert scripter here
local target = scripter;
local x,y,z = getPosition(target);
local target_faction_1 = 1;
local target_faction_2 = 2;
local target_faction_3 = 3;
local ship_1 = ships[135]; -- Cruiser
local ship_2 = ships[135]; -- Cruiser
local ship_3 = ships[135]; -- Dread
local ship_4 = ships[135]; -- Dread
local ship_5 = ships[245]; -- Cruiser
local ship_6 = ships[245]; -- Cruiser
local ship_7 = ships[240]; -- Dread
local ship_8 = ships[242]; -- Dread
local ship_9 = ships[121]; -- Destroyer
local ship_10 = ships[120]; -- Destroyer
local ship_11 = ships[222]; -- Destroyer
local ship_12 = ships[220]; -- Destroyer
local ship_qty = 2; -- Must be an even number

-- local timer = 0;

local lightred = "<color;6666ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local green = "<color;00ff00>";
local black = "<color;000000>"; --why on earth would you use this?
local ec = "</color>";

function spawnShips(d)
	for i=1, d do
		local new_ship = spawnShip(ship_1.path, target, ship_1.name, target_faction_1);
		setPosition(new_ship, x - 3100 + randomNumber(0,250), y, z + 400 + randomNumber(0,250));
	end
	for i=1, d do
		local new_ship = spawnShip(ship_2.path, target, ship_2.name, target_faction_1);
		setPosition(new_ship, x - 3100 - randomNumber(0,250), y, z + 700 - randomNumber(0,250));
	end
	for i=1, d / 2 do
		local new_ship = spawnShip(ship_3.path, target, ship_3.name, target_faction_1);
		setPosition(new_ship, x - 3100 + randomNumber(0,250), y, z + 400 + randomNumber(0,250));
	end
	for i=1, d / 2 do
		local new_ship = spawnShip(ship_4.path, target, ship_4.name, target_faction_1);
		setPosition(new_ship, x - 3100 - randomNumber(0,250), y, z + 700 - randomNumber(0,250));
	end
	for i=1, d do
		local new_ship = spawnShip(ship_5.path, target, ship_5.name, target_faction_2);
		setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 400 - randomNumber(0,250));
	end
	for i=1, d do
		local new_ship = spawnShip(ship_6.path, target, ship_6.name, target_faction_2);
		setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 700 - randomNumber(0,250));
	end
	-- for i=1, d / 2 do
		-- local new_ship = spawnShip(ship_7.path, target, ship_7.name, target_faction_2);
		-- setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 400 - randomNumber(0,250));
	-- end
	-- for i=1, d / 2 do
		-- local new_ship = spawnShip(ship_8.path, target, ship_8.name, target_faction_2);
		-- setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 700 - randomNumber(0,250));
	-- end
	-- for i=1, d * 2 do
		-- local new_ship = spawnShip(ship_9.path, target, ship_9.name, target_faction_1);
		-- setPosition(new_ship, x - 3100 - randomNumber(0,250), y, z + 700 - randomNumber(0,250));
	-- end
	-- for i=1, d * 2 do
		-- local new_ship = spawnShip(ship_10.path, target, ship_10.name, target_faction_1);
		-- setPosition(new_ship, x - 3100 - randomNumber(0,250), y, z + 700 - randomNumber(0,250));
	-- end	
	-- for i=1, d * 2 do
		-- local new_ship = spawnShip(ship_11.path, target, ship_11.name, target_faction_2);
		-- setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 700 - randomNumber(0,250));
	-- end
	-- for i=1, d * 2 do
		-- local new_ship = spawnShip(ship_12.path, target, ship_12.name, target_faction_2);
		-- setPosition(new_ship, x - 4400 + randomNumber(0,250), y, z - 700 - randomNumber(0,250));
	-- end
end

-- setPosition(scripter, x, y, z);
spawnShips(ship_qty);
-- function msgTarget()
	-- local msg_target = getName(getShip(randomNumber(0, (shipCount() - 1))));
	-- return msg_target;
-- end

		-- local new_ship = spawnShip(ship_1.path, target, ship_1.name, target_faction_1);
		-- setPosition(new_ship, x - 80000, y, z + 500);
-- spawnShips();
-- spawnNoun("gadgets\\horn.prt", scripter, "Horn", scripter);

-- function deleteTarget(value)
	-- local kill_target = getKey(value);
	-- if isNoun(kill_target) then
	-- detachNoun(kill_target);
	-- sendChat(msgTarget(), 1, lightred..value.." destroyed."..ec);
	-- return true;
	-- else
	-- sendChat(msgTarget(), 1, lightred..value.." is not a valid noun."..ec);
	-- return false;
	-- end
-- end

-- function killSpawns()
	-- deleteTarget(ship_spawn);
-- end

-- local target_test = "Zoca";
-- local x,y,z = getPosition(target_test);
-- sendChat(msgTarget(), 1, lightred..target_test.."'s location: X = "..x..", "..ec..teal.."Y = "..y..", "..ec..green.."Z = "..z..ec);

-- local ship_spawn = spawnShip(ships[323].path, "Zheph", "KLUTH SHIP", 3);
-- setPosition(ship_spawn, x - 5000, y, z);
-- sendChat(msgTarget(), 1, teal.."KLUTH SHIP spawned near "..target_test..ec);
-- starTimer(30, "killSpawns()");

-- deleteTarget("KLUTH SHIP");
 
-- deleteTarget(ship_1.name);

end
