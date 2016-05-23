enableAlerts(1);

----------------------------------------------------------------------------

function onInitialize()
	-- Perform any initialization here, this is called right after the script is loaded

end

----------------------------------------------------------------------------

function onRelease()
	-- Called before the script is release
end

---------------------------------------------------------------------------

-- The following callbacks are only called when running on the client

function onFleetelect()
	-- Called before the player selects a team
	pushChat( "onFleetSelect" );
end

function onFleetSelected( fleetId )
	-- Called after the player selects a fleet, the fleet ID is passed
	-- server may not allow the player to join this fleet, so use onSetFleet()
	pushChat( "onFleetSelected" );
end

function onSetFleet( fleetId )
	-- called when the players fleet is actually assigned
	pushChat( "onSetFleet" );
end

function onSpawnSelect()
	pushChat( "onSpawnSelect" );
end

function onSpawnSelected( spawn )
	pushChat( "onSpawnSelected" );
end

function onShipSelect()
	-- Called before the player selects a ship
	pushChat( "onShipSelect" );
end

function onShipSelected( shipName )
	-- Called after the player selects a ship
	pushChat( "onShipSelected" );
end

function onTactical()
	pushChat( "onTactical" );
end

function onShipAttached( shipId )
	pushChat( "onShipAttached" );
end

function onShipDetached()
	pushChat( "onShipDetached" );
end

function testOK()
	pushChat( "testOK" );
end

function testCancel()
	pushChat( "testCancel" );
end

--messageBox( "This is a test of the message box code!", "testOK()", "testCancel()" );
messageBox( "This is a test of the message box code!", "testOK()" );
--messageBox( "This is a test of the message box code!" );



