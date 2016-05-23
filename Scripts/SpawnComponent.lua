enableAlerts(0);

if not isServer() then
	return;
end

-- ================================ OPTIONS ================================
-- =======================================================================

-- =============================== MISC VARIABLES ============================
local scripter = "Lyedtau"; -- This is you

-- ============================== FACTION ID and text =============================
local ugtofactionid = 1;	-- selects ugto faction ID for faction check
local iccfactionid = 2;	-- selects icc faction ID for faction check
local kluthfactionid = 3;	-- selects kluth faction ID for faction check

-- ============================== SPAWN OPTIONS =============================
local nPrim = 5;			-- number of components to spawn
local component = "Gadgets\\ELF.prt";
local gadgetname = "Razzle-Dazzle Power!";
local tSpawnForFactions = { ugtofactionid, iccfactionid, kluthfactionid };	-- Factions to include


function spawnit()
	for _, factionid in pairs( tSpawnForFactions ) do
		
		-- Get all ships for the faction
		for i = 1, shipCount() do
		
			local ship = getShip( i - 1 );
			if ship and isFaction( ship, factionid ) then
				local targetname = getName( ship );
				sendChat( scripter, 4, string.format( "<color;ffcc00> %s is being modified..", targetname )  );
				sendChat( target, 3, string.format( "<color;ffcc00> You have been modified by %s.", scripter ) );
				
				for i = 1, nPrim do
				
					local gadgetid = spawnNoun( component, targetname, gadgetname, targetname ); 
					setGadgetMount( gadgetid, 983040 ); 
				end
			end
		end
	end
end

spawnit();