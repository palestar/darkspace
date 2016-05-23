enableAlerts( 1 )

if isServer() then
	
	local mission = createMission( 0, 2, "Scout1", "Scout1", 300, 1, 1, "Mycopia" );
	pushMission( mission );
	
	mission = createMission( 0, 2, "Scout2", "Scout2", 300, 1, 1 );
	addMissionObjective( mission, "Mycopia" );
	pushMission( mission );

end
