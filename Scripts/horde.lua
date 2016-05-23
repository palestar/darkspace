
enableAlerts( 1 )

if isServer() then

    local n = 0;
    while n < 10 do

        local location = getRandomJumpgate();

	-- spawn a transport, by default they are defending their spawn position
	    local monster = spawnShip( "Monsters\\Crystal Entity\\SC_Crystal.PRT", location, "Crystalline Entity", -1 );

        n = n + 1;

    end

end
