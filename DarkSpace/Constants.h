/*
	Constants.h

	Universe constants such as the tick rate
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "File/Stream.h"
#include "World/Constants.h"

//---------------------------------------------------------------------------------------------------

const Color UNIVERSE_AMBIENT( 63, 36, 72 );
const float SHADOW_MAP_FOCUS = 1000.0f;
const int	MAX_PLATFORMS_PER_FACTION = 30.0f;

//----------------------------------------------------------------------------

enum Faction
{
	FACTION_UNKNOWN			=	0,
	FACTION_NEUTRAL			=	1,
	FACTION_UGTO			=	2,			
	FACTION_ICC				=	3,			
	FACTION_KLUTH			=	4,	
	FACTION_HUMAN_ALLIANCE	=	5,
	FACTION_PIRATE			=	6,
	FACTION_MI				=	7,
	FACTION_TEROS			=	8,
	FACTION_COUNT
};
IMPLEMENT_RAW_STREAMING( Faction );

inline const char * factionText( int a_nFactionId )
{
	static const char * FACTION_NAMES[] = 
	{
		"UNKNOWN",		// FACTION_UNKNOWN
		"NEUTRAL",		// FACTION_NEUTRAL
		"UGTO",			// FACTION_UGTO
		"ICC",			// FACTION_ICC
		"KLUTH",		// FACTION_KLUTH
		"HUMAN_ALLIANCE",	// FACTION_HUMAN_ALLIANCE
		"PIRATE",		// FACTION_PIRATE
		"MI",			// FACTION_MI
		"TEROS",		// FACTION_TEROS
	};

	if ( a_nFactionId < 0 || a_nFactionId >= sizeof(FACTION_NAMES)/sizeof(FACTION_NAMES[0]) )
		return "?";

	return FACTION_NAMES[ a_nFactionId ];
}

// types of damage
enum DamageType
{
	DAMAGE_KINETIC		= 0x1,				// causes damage to hull
	DAMAGE_ENERGY		= 0x2,				// causes damage to shields
	DAMAGE_PSI			= 0x4,				// causes damage to crew
	DAMAGE_EMP			= 0x8,				// causes damage to gadgets
	DAMAGE_BIO			= 0x10,				// causes damage to life
	DAMAGE_ELF			= 0x20,				// drains energy
	DAMAGE_PLANET		= 0x40,				// can cause damage to planet and structure / units on planet

	// Currently, this is limited to 0x80, since a byte is used to transmit this damage type..
};
IMPLEMENT_RAW_STREAMING( DamageType );

// types of resources
enum ResourceType				// types of resoures
{
	CYRO_METALS,
	DARK_MATTER,
	FOOD,
	HEAVY_METALS,
	HYDROGEN,
	HYPER_MATTER,
	METALS,
	OXYGEN,
	URDANIAM,

	RESOURCE_TYPE_LAST
};
IMPLEMENT_RAW_STREAMING( ResourceType );

// player ranks
enum Rank
{
	CADET = -1,
	MIDSHIPMAN,					// 0
	ENSIGN,						// 1
	LIEUTENANT2,				// 2
	LIEUTENANT1,				// 3
	LIEUTENANT_COMMANDER,		// 4
	COMMANDER,					// 5
	CAPTAIN,					// 6
	REAR_ADMIRAL2,				// 7
	REAR_ADMIRAL1,				// 8
	VICE_ADMIRAL,				// 9
	ADMIRAL,					// 10
	FLEET_ADMIRAL,				// 11
	GRAND_ADMIRAL,				// 12
	MARSHAL,					// 13
	CHIEF_MARSHAL,				// 14
	
	FIRST_RANK = CADET,
	LAST_RANK = CHIEF_MARSHAL
};
IMPLEMENT_RAW_STREAMING( Rank );

enum Race
{
	RACE_NONE,
	RACE_HUMAN,
	RACE_KLUTH,
	RACE_MI,

	RACE_COUNT
};
IMPLEMENT_RAW_STREAMING( Race );

enum Tech
{
	TECH_NEUTRAL,
	TECH_UGTO,
	TECH_ICC,
	TECH_KLUTH,
	TECH_PIRATE,
	TECH_MI,
	TECH_TEROS,

	TECH_UNKNOWN = 255,
};
IMPLEMENT_RAW_STREAMING( Tech );

// Facing enum is used to determine hit directions for ships etc... this breaks
// down a direction into 1 of 4 possible directions. This is much smaller to send over
// the network than the 3 floats required for a real 3D direction
enum Facing
{
	FACING_FRONT,
	FACING_RIGHT,
	FACING_LEFT,
	FACING_BACK
};
IMPLEMENT_RAW_STREAMING( Facing );

enum Badges
{
	BRONZE_NAVIGATOR		= 0,
	SILVER_NAVIGATOR		= 1,
	GOLD_NAVIGATOR			= 2,
	BRONZE_ENGINEER			= 3,
	SILVER_ENGINEER			= 4,
	GOLD_ENGINEER			= 5,
	BRONZE_COMBAT			= 6,
	SILVER_COMBAT			= 7,
	GOLD_COMBAT				= 8,
	BRONZE_TRANSPORT		= 9,	
	SILVER_TRANSPORT		= 10,
	GOLD_TRANSPORT			= 11,
	BRONZE_BOMBER			= 12,
	SILVER_BOMBER			= 13,
	GOLD_BOMBER				= 14,
	BRONZE_PRIVATEER		= 15,
	SILVER_PRIVATEER		= 16,
	GOLD_PRIVATEER			= 17,
	BRONZE_SUPPORT			= 18,
	SILVER_SUPPORT			= 19,
	GOLD_SUPPORT			= 20,
	
	PLATINUM_NAVIGATOR		= 21,
	PLATINUM_ENGINEER		= 22,
	PLATINUM_COMBAT			= 23,
	PLATINUM_TRANSPORT		= 24,
	PLATINUM_BOMBER			= 25,
	PLATINUM_PRIVATEER		= 26,
	PLATINUM_SUPPORT		= 27,

	BRONZE_SCOUT			= 28,
	SILVER_SCOUT			= 29,
	GOLD_SCOUT				= 30,
	PLATINUM_SCOUT			= 31,

	FIRST_BADGE = BRONZE_NAVIGATOR,
	LAST_BADGE = PLATINUM_SCOUT,
};
IMPLEMENT_RAW_STREAMING( Badges );


enum Medals								
{
	SIRIUS_COMBAT,
	BRONZE_STAR,
	SILVER_STAR,
	GOLD_STAR,
	BRAVERY,
	VALOR,
	DEFENSE,
	STAR_FURY,
	CONQUEROR,
	PLATINUM_STAR,

	FIRST_MEDAL = SIRIUS_COMBAT,
	LAST_MEDAL = PLATINUM_STAR,
};
IMPLEMENT_RAW_STREAMING( Medals );


enum Statistics
{
	GAMES_PLAYED,
	TIME_PLAYED,
	KILLS,
	KILLED,
	BONUS_PRESTIGE,
	CREDITS,
	RESOURCES,
	SHIPS_DAMAGED,
	SHIPS_CAPTURED,
	PLANETS_DAMAGED,
	PLANETS_CAPTURED,
	FRIENDLY_FIRE,
	REPAIR,
	CONSTRUCTION,
	KAMIKAZE,
	SELF_DESTRUCTS,
	JUMPS,
	PLANET_COLLISIONS,
	RESOURCES_LOST,
	SCOUT,

	FIRST_STATISTIC = GAMES_PLAYED,
	LAST_STATISTIC = SCOUT,
};
IMPLEMENT_RAW_STREAMING( Statistics );

// Ship/Enhancement modifier types
enum ModifierType {
	MT_UNKOWN = 0,
	MT_DRIVE_VELOCITY,			// affects max velocity
	MT_DRIVE_THRUST,			// affects accerleration
	MT_BEAM_RANGE,				// affects beam range
	MT_BEAM_ENERGY,				// affects beam recharge rate
	MT_BEAM_DAMAGE,				// affects beam damage
	MT_WEAPON_RANGE,			// affects weapon range
	MT_WEAPON_ENERGY,			// affects weapon recharge rate
	MT_WEAPON_DAMAGE,			// affects weapon damage
	MT_WEAPON_COOLDOWN,			// affects weapon cooldown 
	MT_WEAPON_AMMO,				// affects weapon ammo
	MT_DEFENSE,					// affects shield/armor strength
	MT_DEFENSE_RECHARGE,		// affects shield/armor charge rate
	MT_AUTOREPAIR,				// affects auto repair rate
	MT_BUILD_SPEED,				// affects build speed
	MT_CLOAK,					// affects cloaking rate
	MT_EWAR_RANGE,				// affects ECM/ECCM range
	MT_EWAR_STRENGTH,			// affects ECM/ECCM strength
	MT_EWAR_ENERGY,				// affects ECM/ECCM energy consumption
	MT_JUMPDRIVE_RANGE,			// affects Jumpdrive range
	MT_JUMPGATE_RANGE,			// affects jumpgate range
	MT_MINING,					// affects mining rate
	MT_PULSESHIELD,				// affects pulse shield range
	MT_REACTOR,					// affects energy generate rate
	MT_RELOAD,					// affects reload rate
	MT_SCANNER,					// affects scanner sensitivity
	MT_TRACTOR,					// affects tractor beam range
	MT_TURNRATE,				// affects turn rate
	MT_SHIPENERGY,				// affects max ship energy
	MT_DAMAGE_REDUCTION,		// affects all incoming damage to ship 
	MT_CAPTURE_DEFENSE,			// affects change to be captured
	MT_SIGNATURE,				// affects signature
	MT_JUMPCOOLDOWN,			// affects jump drive cooldown
	MT_JUMPSPEED,				// affects jump drive velocity
	MT_MASS,					// affects ships mass
	MT_CAPTURE_WEIGHT,			// affects planetary capture rate

	MT_COUNT
};
IMPLEMENT_RAW_STREAMING( ModifierType );

enum CargoType {
	CARGO_TYPE_ITEM = 0,
	CARGO_TYPE_SHIP = 1,
	CARGO_TYPE_ALLY = 2,
};
IMPLEMENT_RAW_STREAMING( CargoType );

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
