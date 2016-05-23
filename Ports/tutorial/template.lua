----------------------------------------------------------------------------

function onInitialize()
	-- Perform any initialization here, this is called right after the script is loaded
end

----------------------------------------------------------------------------

function onRelease()		
	-- Called before the script is release
end

----------------------------------------------------------------------------

-- The following callbacks are only called when running on the client

function onTeamSelect()
	-- Called before the player selects a team
end

function onTeamSelected( team )
	-- Called after the player selects a team, the team ID is passed 
end

function onShipSelect()
	-- Called before the player selects a ship
end

function onShipSelected( shipName )
	-- Called after the player selects a ship
end

function onTactical()
	-- Called when the player enters the game
end

function onDeath()
	-- Called when the players ship is destroyed
end

function onCaptured()
	-- Called when the players ship is captured
end

function onDisconnected()
	-- Called when connection to the server is lost
end

function onEndGame()
	-- Called when the scenerio or mission is completed
end


----------------------------------------------------------------------------
