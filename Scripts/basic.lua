-- enable alerts, turn off for release version
enableAlerts(0);
-- disable all safe zones
setSafeZones( 0 );

----------------------------------------------------------------------------

function onInitialize()
	-- Perform any initialization here, this is called right after the script is loaded

end

----------------------------------------------------------------------------

function onRelease()
	-- Called before the script is release
end

-------------------------------------------------------------------------------
-- Server Side functions

function onSpawnScout1( cadetId )
	if isNoun( "Weasel" ) then
      	detachNoun( "Weasel" );
    	end

	local fleetId = getFleetId( cadetId );
	local shipId = spawnShip( "Ships\\ICC\\Scout\\M21 A\\SC_M21A.PRT", "Zoca", "Weasel", fleetId );
	orderShip( shipId, cadetId, 2 );    -- follow cadet

    	startTimer( 4, "activateECCMScanner( \"" .. shipId .. "\");" );
end

function activateECCMScanner( shipId )
    local count = useGadget( shipId, "P", nil, 0 );     -- activate ECCM
    if count <= 0 then scriptAlert( "Could not activate ECCM" ); end;
    count = useGadget( shipId, "Y", nil, 0 );           -- activate Scanner
    if count <= 0 then scriptAlert( "Could not activate Scanner" ); end;
end

function getEnemyFleetId( notFleetId )
	local maxFleet = fleetCount();
	for i=0, maxFleet do
		local checkFleetId = fleetId( i );
		if not checkFleetid == notFleetId then
			return checkFleetId;
		end
	end
end

function onSpawnCombat1( cadetId )
	local fleetId = getFleetId( cadetId );
	local enemyFleetId = fleetId + 1; -- getEnemyFleetId( fleetId );

	local shipICC = spawnShip( "Ships\\ICC\\Destroyer\\M190 A\\SC_M190A.PRT", "Mycopia", "Schak'JaJ", fleetId );
	local shipUGTO1 = spawnShip( "Ships\\UGTO\\Frigate\\ST-6 Interceptor\\SC_ST6.prt", "Mycopia", "Mephiblo", enemyFleetId );
	local shipUGTO2 = spawnShip( "Ships\\UGTO\\Frigate\\ST-6 Interceptor\\SC_ST6.prt", "Mycopia", "Damion", enemyFleetId ); 

    	orderShip( shipICC, "Mycopia", 2 );
    	orderShip( shipUGTO1, "Mycopia", 2 );  
   	orderShip( shipUGTO2, "Mycopia", 2 );  


    	enablePointDefence( shipICC );
    	enablePointDefence( shipUGTO1 );
    	enablePointDefence( shipUGTO2 );
end

function onSpawnCombat2( cadetId )
	local fleetId = getFleetId( cadetId );
	local shipICC1 = spawnShip( "Ships\\ICC\\Destroyer\\M190 A\\SC_M190A.PRT", "Zoca", "Faustus", fleetId );
	local shipICC2 = spawnShip( "Ships\\ICC\\Destroyer\\M190 A\\SC_M190A.PRT", "Zoca", "Lemming", fleetId );

	-- order both ICC ships to defend Mycopia
    	orderShip( shipICC1, "Mycopia", 2 );
    	orderShip( shipICC2, "Mycopia", 2 );

    	enablePointDefence( shipICC1 );
    	enablePointDefence( shipICC2 );
end

function enablePointDefence( shipId )
    local maxGadget = gadgetCount( shipId ) - 1;
    for i = 0, maxGadget do
        local gadgetId = getGadget( shipId, i );
        if getGadgetType( gadgetId ) == 6 then  -- normal beam weapon
            setBeamPointDefence( gadgetId, 1 );
        end
    end
end

function doEJump( shipId )
    if not isShipDestroyed( shipId ) then
        useGadget( shipId, "J", nil, 1 );
        startTimer( 5, "removeShip( \"" .. shipId .. "\" );" );
    end
end

function removeShip( shipId )
    if not isShipDestroyed( shipId ) then
        detachNoun( shipId );
    end
end

----------------------------------------------------------------------------

-- The following callbacks are only called when running on the client

function onFleetSelect()
	-- Called before the player selects a team
end

function onFleetSelected( fleetId )
	-- Called after the player selects a fleet, the fleet ID is passed
end

function onSetFleet( fleetId )
	-- called when the server assigns the players fleetId
end

function onSpawnSelect()
	-- called when the player enters the spawn selection screen
end

function onSpawnSelected( spawnId )
	-- called when the player selects a spawn point
end

function onShipSelect()
	-- Called before the player selects a ship
end

function onShipSelected( shipName )
	-- Called after the player selects a ship
end

function onTactical()
	startTutorial();
end

function onShipAttached( shipId )
end

function onShipDetached( shipId )
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

function startTutorial()

	messageBox( "<b>Tutorial: Basic Trainingl</b>\n\nWelcome to the DarkSpace basic tutorial. This tutorial will teach you how to navigate your ship, engage in basic combat, and capture planets. You may press <b>F1</b> at any time to see additional help for that screen.\n\nClick on the <b>OK</b> button to continue, click on the <b>Cancel</b> button to abort this tutorial.", "onTacticalView()", "cancelTutorial()" );

end

function onTacticalView()

	messageBox( "You are now in the <b>TACTICAL</b> view. Your ship is displayed in the center of the screen.<b>Right Click and Drag</b> in this view to change the view of your ship. The <b>Mouse Wheel</b> and the <b>X/Z</b> keys control zoom.\n\nTry rotating/zooming your view before clicking on the <b>OK</b> button...", "onShipStatus()", "cancelTutorial()" );

end

function onShipStatus()

	messageBox( "<con;HullBar>This is your <b>Ship Status</b> display.\n\nThe <b>Hull</b> bar shows the status of your ships hull, at 0% your hull is breached and your ship is destroyed.\n\nThe <b>Energy</b> bar shows the amount of energy you have available for weapons and other ship systems.\n\nThe <b>Signature</b> bar shows how much radiation your ship is emitting, the more higher your signature the easier your ship is detected. Your signature is affected by your ships velocity, energy usage, and ECM/ECCM in the area.", "onRepairQueue()", "cancelTutorial()" );

end

function onRepairQueue()

	messageBox( "<con;ButtonRepairQueue>This is your <b>Repair Queue</b>, as systems get damaged, ship system icons will appear listed here in order of repair, <b>Left click</b> on a button to move it to the top of the queue, <b>Right click</b> to move a system to the end of the repair queue. ", "onSystemsDisplay()", "cancelTutorial()" );

end

function onSystemsDisplay()

	messageBox( "<con;ButtonSystemDisplay>This is your <b>Systems Display</b>, all your ships systems/weapons/drives are displayed in this area. <b>Right Click</b> on a system to target that system. <b>Left Click</b> on a system button to activate that system or weapon if usable.\n\nIf the icon for a system is <b>white</b> then the system is currently active, <b>Green</b> icons indicate the system is ready to use/fire, and <b>grey</b> icons indicate they are currently unavailable (i.e. charging, out of range/arc).\n\nIf the device uses ammo or fuel, the amount of ammo/fuel available will be displayed in the upper right.\n\nThe <b>Hotkeys</b> for a device will be displayed in the lower right, pressing the hotkey will use the device immediately.", "onCargoDisplay()", "cancelTutorial()" );

end

function onCargoDisplay()

	messageBox( "<con;ButtonCargoDisplay>This is your <b>Cargo Display</b>. Any cargo your ship has in it's hold is displayed as buttons in this display. <b>Left click</b> on a button to unload it onto your current target. <b>Right click</b> on a button to target that cargo item.", "onHelmControls()", "cancelTutorial()" );

end

function onHelmControls()

	messageBox( "<con;ButtonHelmControls>This is your <b>Helm Display</b>. Your ships current velocity and heading are displayed in this area. You may left click on the velocity/heading bars to control your ship. However, usually you would use the <b>A/S/D</b> keys to control your heading and the <b>Q/W</b> keys to control your velocity. The <b>Arrow</b> keys may be used as well to control your heading and velocity.\n\nTry using the <b>A/S/D and Q/W</b> keys to control your heading and velocity, click on <b>OK</b> when you are ready to continue...", "onTargetDisplay()", "cancelTutorial()" );

end

function onTargetDisplay()

	-- go ahead and spawn the scout near zoca, it will jump to the players location
	remoteCall( "Zoca", 5, "onSpawnScout1(\"" .. playersShip() .. "\");" );    -- get the ECCM cover here

	messageBox( "<con;TargetFrame>Your current target is displayed here. Depending on the type of target additional buttons will appear beside the target window. You set your current target by <b>Left Clicking</b> on a <b>target indicator</b>. (The target indicators are those colored arrows along the edge of your screen, red for enemy, green for friendly, and yellow for neutral.)\n\nA friendly scount is jumping to your location, you will see a green target indicator appear shortly, try left clicking on that indicator to set the scout as your current target...", "onNavigation()", "cancelTutorial()" );

end

function onNavigation()
	
	messageBox( "<con;ButtonNavigation>Press <b>F2</b> or click on this button to enter the navigation view." );
	watchKey( 113, "onNavigationView()" );

end

function onNavigationView()
	watchKey( 113 );

	messageBox( "This is the <b>Navigation view</b> which provides you with an overview of the current system. You rotate and zoom the camera in this view the same as in the tactical view. Hold down the <b>Right mouse button and drag</b> to rotate, use the <b>mouse wheel</b> or the <b>X/Z</b> keys to zoom.\n\nTIP: <b>SHIFT+Z</B> will zoom in the maximum distance, while <b>SHIFT+X</b> will zoom out the maximum distance.", "onNavigationFocus()", "cancelTutorial()" );

end

function onNavigationFocus()

	messageBox( "The navigation screen allows you to move around the focus of your view. Try holding down the <B>Left mouse button</b> over an empty part of the map and <b>DRAG</b>. If you target an object and press <b>Spacebar</b> that will center the map on that object. Also, <b>Double Left Click</b> will center the map on an object as well.\n\nTry looking around the system some, click the <b>OK</b> button to continue...", "onNavigationContacts()", "cancelTutorial()" );

end

function onNavigationContacts()

	messageBox( "<con;ButtonDisplayContacts> Displayed here are your navigation contacts. You can <b>LEFT CLICK</b> on an navigation contact to target that object, and press <b>SPACEBAR</b> to center the map on that object.", "onNavigationJump()", "cancelTutorial()" );

end

function onNavigationJump()

	messageBox( "If you have not already, select <color;00ffff>MYCOPIA</color> as your current target. Once selected you will notice a line appear between your current position and your target. The line will be either <b>WHITE</b> or <color;0000ff>RED</color> which indicates if any known objects are between you and your target.\n\nIf you press the <b>O</b> key, that will command your ship to orbit Mycopia. Your ship will automatically set course and jump to the target avoiding any collisions\n\nNOTE: You can press the <b>J</b> key to force a jump to your current target, however this will not avoid collisions or other hazards, so make sure the line is white.", "onNavigationOrbit()", "cancelTutorial()" );

end

function onNavigationOrbit()

	-- short combat scene to watch
	remoteCall( "Mycopia", 5, "onSpawnCombat1(\"" .. playersShip() .. "\");" );  

	messageBox( "Press the <b>O</b> key to command your ship to orbit </b>Mycopia</b> then press <b>F2</b> to return to the tactical view.", "onWaitOrbit()", "cancelTutorial()" );
	
end

function onWaitOrbit()
	messageBox( "You'll notice ships are already engaged in combat around Mycopia, do not engage or fire at any enemy ships yet, our mission is to land infantry on the planet for capture...\n\nEnter orbit around Mycopia to continue...", "", "cancelTutorial()" );

	if isOrbiting( playersShip(), "Mycopia" ) then
		onOrbit();
	else
		startTimer( 1, "onWaitOrbit()" );
	end
end

function onOrbit()

	messageBox( "You are now in orbit around the primary planet of this system. Target <b>Mycopia</b> again if needed, the <color;00ffff>yellow</color> markers on the planets surface are drop zones.\n\nSelect a drop zone by <b>LEFT CLICKING</b>, then press the <b>U</b> key to unload your infantry on the planet.", "onInfantryUnloaded()", "cancelTutorial()" );

end

function onInfantryUnloaded()

	messageBox( "Once the infantry has landed on the surface it will take some time to actually capture the planet. While we are waiting, lets help out the ICC destroyer, but first some basics on combat!", "onCombat()", "cancelTutorial()" );

end

function onCombat()

	messageBox( "Look for a <b>RED</b> target indicator, this is the enemy ship. (NOTE: Pressing the <b>E</b> key will target the closest enemy ship, pressing the <b>R</b> key will target the closest friendly ship).\n\nOnce the ship is targeted, you may press the <B>F</b> key to automatically follow, or the <b>G</b> key to shadow the ship (Shadow is like follow, except you just match heading/velocity and doesn't close range as much).\n\nPress <b>SPACEBAR</b> to alpha strike, this fires all available weapons at your current target. NOTE: All weapons on your ship are limited by range and firing arc, when a weapon can fire will be indicated by it's icon turning green in the <b>Systems Display</b>. <con;GadgetLayout>", "onCombat2()", "cancelTutorial()" );

end

function onCombat2()
	messageBox( "Target the enemy by pressing the <b>E</b> key, and help the destroy the enemy..." );

	-- spawn 2 more destroyers to make sure the ship is destroyed
	remoteCall( "Mycopia", 5, "onSpawnCombat2(\"" .. playersShip() .. "\");" );  
	-- start waiting for Mycopia to be captured
	onWaitCapture();
end

function onWaitCapture()
	local cadetId = playersShip();
	if getFaction( cadetId ) == getFaction( "Mycopia" ) then
		onPlanetCaptured();
	else
		startTimer( 15, "onWaitCapture()" );
	end
end

function onPlanetCaptured()
	
	messageBox( "YAY! you've just captured your first planet. Your faction can now build structures on the planet, and use it's resources to take control of this star system.", "onLoadInfantry()", "cancelTutorial()" );

end

function onLoadInfantry()
	
	messageBox( "You may now load your infantry back onto your ship to capture the other key planets in this system, <b>Dawn</b> and <b>Lhzack</b>.\n\nTarget <b>Mycopia</b>, then you will your infantry appear on the surface at the location you unloaded them, target the infantry and press the <b>L</b> key to load the infantry back onto your ship.", "onInfantryLoaded()", "cancelTutorial()" );

end

function onInfantryLoaded()

	messageBox( "You may now capture <b>Dawn</b> and <b>Lhzack</b> to end this tutorial.", "endTutorial()", "cancelTutorial()" );

end

function endTutorial()
	pushChat( "Tutorial completed..." );
end

function cancelTutorial()
	pushChat( "Tutorial Cancelled..." );
end

