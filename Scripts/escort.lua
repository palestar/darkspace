-- TITLE: ICC Diplomatic Escort
-- REQ: ICC controlled planet, ICC jumpgate

enableAlerts( 1 )

if isServer() then
	-- find jumpgate for this to all start
	local gate = getFriendlyJumpgate( 2 );
	assert( gate ~= nil, "gate not found!" );

	-- find a planet for the basis of this mission
	local planet = getFriendlyPlanet( 2 );
	assert( planet ~= nil, "planet not found!" );
	-- start strike on planet
	planetEvent( planet, 3, 1 );

	-- spawn a transport, by default they are defending their spawn position
	local ship = spawnShip( "Ships\\ICC\\Transport\\M2111 Transport\\SC_M2111.PRT",
		gate, "Diplomatic Transport 1A", getFactionTeam( 2 ) );
	-- order the transport to hold it's current position for 150 seconds
    -- if failed, the transport will just go back into the jumpgate
    assignMission( createMission( 12, 2, "Sleep", "", 150, 0, 0 ), ship );

	-- create an escort mission to the target planet
	local mission = createMission( 3, 2, "Escort Diplomatic Transport",
		"Worker relations are in shambles on "..getName( planet ).."."..
		"ICC fleet command has sent a notible and respected diplomat to resolve"..
		" the issues and advert a worker strike.", 300, 5, 5000, ship, planet );
	assert( mission ~= nil, "Failed to create mission" );

	-- when the mission is accepted, create a counter mission to destroy the transport
	local trigger = "onMissionAccepted( \""..ship.."\",\""..planet.."\");";
	addMissionTrigger( mission, 1, trigger );

	assert( pushMission( mission ) == 1, "Failed to push mission!" );
	closeMission( mission );
end

function onMissionAccepted( transport, planet )

    -- open the calling mission
	local mission = openThisMission();
	assert( mission ~= nil, "Failed to open this mission" );

	-- create UGTO counter mission to destroy transport
	local counter = createMission( 2, 1, "Destroy Diplomatic Escort",
		"Intelligence reports that the ICC are sending an diplomatic transport"..
		" to "..getName( planet )..". The mission of the transport is unknown,"..
		"however it should be destroyed before it can unload it's cargo.",
		300, 5, 10000, transport );
	pushMission( counter );

	-- get the mission owner
	local owner = getMissionOwner( mission );
	assert( owner ~= nil, "Failed to get mission owner" );
	-- create the mission for the transport
	local wait = createMission( 11, 2, "Wait", "Wait", 150, 0, 0, owner );
	addMissionTrigger( wait, 3, "onEscortArrive( \""..owner.."\" );" );
    addMissionTrigger( wait, 4, "onDiplomatDead( \""..planet.."\" );" );
    -- sleep for 30 seconds after escort arrives
    local sleep = createMission( 12, 2, "Sleep", "", 30, 0, 0 );
    addMissionTrigger( sleep, 4, "onDiplomatDead( \""..planet.."\" );" );
    setNextMission( wait, sleep );
	-- after sleep is complete jump to target planet
	local orbit = createMission( 8, 2, "Orbit", "Orbit", 300, 0, 0, planet );
    addMissionTrigger( orbit, 4, "onDiplomatDead( \""..planet.."\" );" );
    addMissionTrigger( orbit, 3, "onDiplomatArrive( \""..owner.."\", \""..transport.."\" ); " );
	setNextMission( sleep, orbit );
    -- after arriving at planet, sleep for 120 seconds
    local sleep2 = createMission( 12, 2, "Sleep2", "", 120, 0, 0 );
    addMissionTrigger( sleep2, 4, "onDiplomatDead( \""..planet.."\" );" );
    addMissionTrigger( sleep2, 3, "onDiplomatAlive( \""..planet.."\" );" );
    setNextMission( orbit, sleep2 );
	-- return to the jumpgate
	local gate = createMission( 0, 2, "Gate", "Gate", 300, 0, 0, getFriendlyJumpgate( 2 ) );
	setNextMission( sleep2, gate );

    -- remove the previous sleep mission from the transport
    setMissionState( openShipMission( transport ), 0 );
	-- assign mission series to transport
	assignMission( wait, transport );
	-- close all open missions
	closeAllMissions();
end

function onEscortArrive( escort )
	sendChat( escort, 4, "DIPLOMAT: Thanks for the escort, now aligning for jump in 30 seconds..." );
end

function onDiplomatArrive( escort, transport )
    sendChat( escort, 4, "DIPLOMAT: Orbit established, starting negotiations..." );

    local next = createMission( 5, 2, "Defend Diplomatic Transport",
        "Defend the diplomatic transport during negotiations", 120, 5, 5000, transport );
    assignMission( next, escort );
end

function onDiplomatAlive( planet )
	-- turn off strike if any
	planetEvent( planet, 3, 0 );
	-- start economic boom
	planetEvent( planet, 4, 1 );
end

function onDiplomatDead( planet )
	-- revolt the planet to the UGTO
	revoltPlanet( planet, getFactionTeam( 1 ) );
end
