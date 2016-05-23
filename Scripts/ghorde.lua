
enableAlerts( 1 )

if isServer() then

    local n = 0;
    while n < 10 do

        local location = getRandomJumpgate();

	-- spawn a transport, by default they are defending their spawn position
	    local monster = spawnShip( "Monsters\\Gaifen Opaque\\Gaifen Monster.PRT", location, "Gaifen", -1 );

        n = n + 1;

    end

end
