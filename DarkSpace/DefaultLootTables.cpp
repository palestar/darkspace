/*
	DefaultLootTables.cpp

	List of all default loot tables used for ships that have no specific drops

	This is just a temporary addition to allow for drops before ships have been given
	full drop tables

	(c)2008 Palestar Inc, Jeff Everett
*/

#include "NounShip.h"
#include "DefaultLootTables.h"
#include "DarkSpace/BeamEnhancements.h"
#include "DarkSpace/WeaponEnhancements.h"
#include "DarkSpace/ShipEnhancements.h"
#include "DarkSpace/SpecialEnhancements.h"

//---------------------------------------------------------------------------------------------------

void DefaultLootTables::initialize()
{
	LootMap & map = getLootMap();

	// AI loot table
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(qword(0), 3500));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 25));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 100));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 100));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 100));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 100));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 100));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 100));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 100));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 100));				
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 100));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 100));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 100));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(MinorShield), 100));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 75));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 75));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 75));				
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 75));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 75));				
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 75));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 50));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 50));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 50));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 50));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 50));		
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 50));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 50));	
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 50));				
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 50));			
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 50));				
	map[ NounShip::UNKNOWN ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 50));	

	// Scout Ship Default Drops
	map[ NounShip::SCOUT ].push_back(new LootInfo(qword(0), 75));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 5));		
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 5));		
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 5));			
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 5));			
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 5));	
	map[ NounShip::SCOUT ].push_back(new LootInfo(CLASS_KEY(MinorShield), 5));	

	// Engineer Ship Default Drops
	map[ NounShip::ENGINEER ].push_back(new LootInfo(qword(0), 125));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 5));			
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 5));		
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 5));				
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 5));			
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 5));				
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(MinorShield), 5));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 2));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 3));			
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 3));		
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 3));				
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 3));			
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 3));				
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 3));	
	map[ NounShip::ENGINEER ].push_back(new LootInfo(CLASS_KEY(UpgradedBuildDrones), 3));

	// Supply Ship Default Drops
	map[ NounShip::SUPPLY ].push_back(new LootInfo(qword(0), 125));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 2));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 5));		
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 5));
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 5));		
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 5));
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 5));			
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 5));			
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(MinorShield), 5));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 4));			
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 4));		
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 4));				
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 4));			
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 4));				
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 4));	
	map[ NounShip::SUPPLY ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 4));	


	// Transport Ship Default Drops
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(qword(0), 135));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 2));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 5));		
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 5));		
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 5));			
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 5));			
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(MinorShield), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 5));			
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 5));		
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 5));				
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 5));			
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 5));				
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 5));	
	map[ NounShip::TRANSPORT ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 5));	


	// Frigate Ship Default Drops
	map[ NounShip::FRIGATE ].push_back(new LootInfo(qword(0), 3500));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 25));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 100));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 100));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 100));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 100));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 100));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 100));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 100));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 100));				
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 100));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 100));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 100));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(MinorShield), 100));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 75));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 75));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 75));				
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 75));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 75));				
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 75));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 50));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 50));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 50));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 50));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 50));		
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 50));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 50));	
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 50));				
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 50));			
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 50));				
	map[ NounShip::FRIGATE ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 50));	


	// Destroyer Ship Default Drops
	map[ NounShip::DESTROYER ].push_back(new LootInfo(qword(0), 3500));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 25));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 100));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 100));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 100));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 100));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 100));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 100));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 100));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 100));				
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 100));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 100));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 100));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(MinorShield), 100));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 75));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 75));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 75));				
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 75));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 75));				
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 75));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 50));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 50));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 50));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 50));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 50));		
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 50));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 50));	
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 50));				
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 50));			
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 50));				
	map[ NounShip::DESTROYER ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 50));	

	// Cruiser Ship Default Drops
	map[ NounShip::CRUISER ].push_back(new LootInfo(qword(0), 3500));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 100));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 100));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 100));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 100));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 100));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 100));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 100));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 100));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 100));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 100));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 100));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(MinorShield), 100));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 75));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 75));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 75));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 75));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 75));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 75));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 50));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 50));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 50));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 50));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 50));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 50));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamAccelerator), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamCooler), 25));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamMultiplexer), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponAccelerator), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCooler), 25));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponMultiplexer), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCondenser), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedThrust), 25));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedVelocity), 25));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedTurn), 25));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ImprovedArmor), 25));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamAccelerator), 15));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamCooler), 15));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamMultiplexer), 15));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAccelerator), 15));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCooler), 15));		
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponMultiplexer), 15));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCondenser), 15));	
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAmmo), 15));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedThrust), 15));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedVelocity), 15));			
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedTurn), 15));				
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(EnhancedArmor), 15));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(DeepPurpleExhaust), 3));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(ForestGreenExhaust), 3));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(RedHotExhaust), 3));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(DarkBlueExhaust), 3));
	map[ NounShip::CRUISER ].push_back(new LootInfo(CLASS_KEY(StealthModule), 3));


	// Dread Ship Default Drops
	map[ NounShip::DREAD ].push_back(new LootInfo(qword(0), 3500));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 100));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 40));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 40));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 40));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 40));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 40));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 40));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 40));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 40));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 40));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 40));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 40));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(MinorShield), 40));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 50));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 50));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 50));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 50));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 50));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 55));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 55));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 55));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 55));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 55));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 55));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamAccelerator), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamCooler), 50));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamMultiplexer), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponAccelerator), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCooler), 50));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponMultiplexer), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCondenser), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedThrust), 50));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedVelocity), 50));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedTurn), 50));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ImprovedArmor), 50));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamAccelerator), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamCooler), 38));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamMultiplexer), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAccelerator), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCooler), 38));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponMultiplexer), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCondenser), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAmmo), 38));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedThrust), 38));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedVelocity), 38));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedTurn), 38));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(EnhancedArmor), 38));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamAccelerator), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamCooler), 21));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamMultiplexer), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponAccelerator), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponCooler), 21));		
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponMultiplexer), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponCondenser), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponAmmo), 21));	
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedThrust), 21));				
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedVelocity), 21));			
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(AdvancedTurn), 21));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(DeepPurpleExhaust), 13));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(ForestGreenExhaust), 13));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(RedHotExhaust), 13));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(DarkBlueExhaust), 13));
	map[ NounShip::DREAD ].push_back(new LootInfo(CLASS_KEY(StealthModule), 3));


	// Station Ship Default Drops
	map[ NounShip::STATION ].push_back(new LootInfo(qword(0), 3400));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(JumpDriveRefill), 100));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorBeamAccelerator), 35));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorBeamCooler), 35));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorBeamMultiplexer), 35));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorWeaponAccelerator), 35));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCooler), 35));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorWeaponMultiplexer), 35));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorWeaponCondenser), 35));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorThrust), 35));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorVelocity), 35));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorTurn), 35));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorArmor), 35));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(MinorShield), 35));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedBeamAccelerator), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedBeamCooler), 37));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedBeamMultiplexer), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponAccelerator), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCooler), 37));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponMultiplexer), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedWeaponCondenser), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedThrust), 37));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedVelocity), 37));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedTurn), 37));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedArmor), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(LimitedShield), 37));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardBeamAccelerator), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardBeamCooler), 52));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardBeamMultiplexer), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardWeaponAccelerator), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCooler), 52));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardWeaponMultiplexer), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardWeaponCondenser), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardThrust), 52));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardVelocity), 52));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardTurn), 52));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StandardArmor), 52));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamAccelerator), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamCooler), 60));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedBeamMultiplexer), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponAccelerator), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCooler), 60));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponMultiplexer), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedWeaponCondenser), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedThrust), 60));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedVelocity), 60));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedTurn), 60));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ImprovedArmor), 60));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamAccelerator), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamCooler), 42));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedBeamMultiplexer), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAccelerator), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCooler), 42));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponMultiplexer), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponCondenser), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedWeaponAmmo), 42));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedThrust), 42));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedVelocity), 42));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedTurn), 42));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(EnhancedArmor), 42));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamAccelerator), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamCooler), 27));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedBeamMultiplexer), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponAccelerator), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponCooler), 27));		
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponMultiplexer), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponCondenser), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedWeaponAmmo), 27));	
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedThrust), 27));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedVelocity), 27));			
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedTurn), 27));				
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(AdvancedArmor), 27));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(DeepPurpleExhaust), 17));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(ForestGreenExhaust), 17));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(RedHotExhaust), 17));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(DarkBlueExhaust), 17));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(UpgradedBuildDrones), 17));
	map[ NounShip::STATION ].push_back(new LootInfo(CLASS_KEY(StealthModule), 3));
}

DefaultLootTables::LootMap & DefaultLootTables::getLootMap()
{
	static LootMap map;
	return map;
}

const ClassKey & DefaultLootTables::selectLoot(NounShip * a_pShip)
{
	LootMap & map = getLootMap();

	NounShip::Type eType = a_pShip->type();
	LootInfo * pLoot = selectItem( map[ eType ] );
	if ( pLoot )
		return pLoot->m_classKey;

	static ClassKey NULL_CLASS( (qword)0 );
	return NULL_CLASS;
}

DefaultLootTables::LootInfo * DefaultLootTables::selectItem(LootVector & a_list)
{
	// add up the chances for each loot item
	int nTotalChance = 0;
	for(int i=0;i<(int)a_list.size();i++)
		nTotalChance += a_list[i]->m_nChance;

	// pick a random number between 0 and total chance
	int randomDrop = rand() % nTotalChance;

	int nTotal =0;
	// grab that drop
	for(int i=0;i<(int)a_list.size();i++)
	{
		int nThisValue = nTotal+a_list[i]->m_nChance;
		if(randomDrop >= nTotal && randomDrop <= nThisValue)
			return a_list[i];
		nTotal = nThisValue;
	}

	return NULL;
}

//---------------------------------------------------------------------------------------------------
// EOF
