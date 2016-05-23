-----------------------------------
---- Adv Encounters v0.5 ----
-----------------------------------

enableAlerts(0);  --don't change this

ships ={};

ships[130]={name="ST-74 Torpedo Cruiser", path="ships\\ugto\\cruiser\\st-74 torpedo cruiser\\sc_st74.prt"};
ships[161]={name="ST-7010 Heavy Engineer", path="ships\\ugto\\engineering\\st-7010\\sc_st7010.prt"};

ships[234]={name="Assault Cruiser", path="ships\\icc\\cruiser\\m245 a\\sc_m245a.prt"};
ships[260]={name="MR-110 Engineer", path="ships\\icc\\engineering\\mr 110\nc_mr110.prt"};
ships[261]={name="MR-115 Engineer", path="ships\\icc\\engineering\\mr 115\nc_mr115.prt"};
ships[262]={name="MR-125 Engineer", path="ships\\icc\\engineering\\mr 125\nc_mr125.prt"};
ships[264]={name="MS-42a Heavy Supply", path="ships\\icc\\supply\\m s42 a\\sc_ms42a.prt"};

ships[331]={name="Parasite", path="ships\\kluth\\cruiser\\parasite\\parasite cruiser.prt"};
ships[360]={name="Drone Engineer", path="ships\\kluth\\engineer\\drone\\builder.prt"};
ships[363]={name="Ultimate Worker", path="ships\\kluth\\supply\\ultimate worker\\ult worker.prt"};

ships[430]={name="Legion Cruiser", path="ships\\mi\\cruiser\\legion\\sc_legion.prt"}; -- MIR Cruiser
ships[440]={name="Longhead", path="ships\\mi\\dreadnought\\longhead\\sc_dread.prt"}; -- MIR Dread
ships[460]={name="Assembler", path="ships\\mi\\engineering\\assembler\sc_assembler.prt"}; -- MIR Engy
ships[461]={name="Forager", path="ships\\mi\\supply\\forager\\sc_forager.prt"}; -- MIR Supply
ships[470]={name="Conveyor", path="ships\\mi\\transport\\conveyor\\sc_conveyor.prt"}; -- MIR Trans

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

local scripter = "[Admin]Mersenne Twister"; -- Insert scripter here
local target_1 = "Earth";
local target_2 = "Luna";
local target_3 = "Exathra";
local target_4 = "Minuete";
local target_5 = "Sag Hothha";
local target_6 = "Leaph";
local ship_name_1 = "UGTO Christmas MI Escort";
local ship_name_2 = "UGTO Christmas MI Patrol";
local ship_name_3 = "ICC Christmas MI Escort";
local ship_name_4 = "ICC Christmas MI Patrol";
local ship_name_5 = "K'Luth Christmas MI Escort";
local ship_name_6 = "K`Luth Christmas MI Patrol";


		-- local new_ship = spawnShip( ships[264].path, "Exathra", ship_name_3, getOwner("Exathra"));
		-- for f=1, 3 do
			-- spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		-- end
		-- for f=1, 12 do
			-- spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			-- spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		-- end
		-- for f=1, 2 do
			-- spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		-- end
	

function moveShip()
	local ship_true = 0;
	if not isShip(ship_name_1) then
		local new_ship = spawnShip( ships[430].path, target_1, ship_name_1, getOwner(target_1));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if not isShip(ship_name_2) then
		local new_ship = spawnShip( ships[430].path, target_2, ship_name_2, getOwner(target_2));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if not isShip(ship_name_3) then
		local new_ship = spawnShip( ships[430].path, target_3, ship_name_3, getOwner(target_3));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if not isShip(ship_name_4) then
		local new_ship = spawnShip( ships[430].path, target_4, ship_name_4, getOwner(target_4));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if not isShip(ship_name_5) then
		local new_ship = spawnShip( ships[430].path, target_5, ship_name_5, getOwner(target_5));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if not isShip(ship_name_6) then
		local new_ship = spawnShip( ships[430].path, target_6, ship_name_6, getOwner(target_6));
		for f=1, 3 do
			spawnNoun( "Gadgets\\EMP Cannon.PRT", new_ship, "Emp Cannon "..f, new_ship);
		end
		for f=1, 12 do
			spawnNoun( "Gadgets\\P Cannon.prt", new_ship, "P Cannon "..f, new_ship);
			spawnNoun( "Gadgets\\Auto Repair.prt", new_ship, "Automated Repair "..f, new_ship);
		end
		for f=1, 2 do
			spawnNoun( "Gadgets\\Drive - IE.prt", new_ship, "IE Drive "..f, new_ship);
		end
		ship_true = 1;
	end
	if ship_true == 1 then
		startTimer(600, "moveShip()");
	end
end
moveShip();

end
