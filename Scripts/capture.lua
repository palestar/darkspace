
enableAlerts( 1 )

if isServer() then

	-- spawn a transport, by default they are defending their spawn position
	local ship = spawnShip( "Ships\\ICC\\Transport\\M2111 Transport\\SC_M2111.PRT",
		getFriendlyJumpgate( 2 ), "Transport", getFactionTeam( 2 ) );
	-- order the transport to capture an enemy planet
	orderShip( ship, getEnemyPlanet( 2 ), 3 );

end

