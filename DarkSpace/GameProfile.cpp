/**
	@file GameProfile.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:35:33 PM
*/

#include "GameProfile.h"
#include "Debug/Assert.h"
#include "Standard/Bits.h"

//---------------------------------------------------------------------------------------------------

struct BadgeRequirement 
{
	Statistics		m_eStatistic;
	int				m_nLevel;
	Badges			m_eBadge;
};

//---------------------------------------------------------------------------------------------------

BadgeRequirement BADGE_REQUIREMENTS[] =
{
	{ JUMPS, 50, BRONZE_NAVIGATOR },
	{ JUMPS, 500, SILVER_NAVIGATOR },
	{ JUMPS, 1000, GOLD_NAVIGATOR },
	{ JUMPS, 5000, PLATINUM_NAVIGATOR },
	{ CONSTRUCTION, 50, BRONZE_ENGINEER },
	{ CONSTRUCTION, 500, SILVER_ENGINEER },
	{ CONSTRUCTION, 1000, GOLD_ENGINEER },
	{ CONSTRUCTION, 5000, PLATINUM_ENGINEER },
	{ SHIPS_DAMAGED, 50, BRONZE_COMBAT },
	{ SHIPS_DAMAGED, 500, SILVER_COMBAT },
	{ SHIPS_DAMAGED, 1000, GOLD_COMBAT },
	{ SHIPS_DAMAGED, 5000, PLATINUM_COMBAT },
	{ PLANETS_CAPTURED, 25, BRONZE_TRANSPORT },
	{ PLANETS_CAPTURED, 50, SILVER_TRANSPORT },
	{ PLANETS_CAPTURED, 100, GOLD_TRANSPORT },
	{ PLANETS_CAPTURED, 500, PLATINUM_TRANSPORT },
	{ PLANETS_DAMAGED, 50, BRONZE_BOMBER },
	{ PLANETS_DAMAGED, 500, SILVER_BOMBER },
	{ PLANETS_DAMAGED, 1000, GOLD_BOMBER },
	{ PLANETS_DAMAGED, 5000, PLATINUM_BOMBER },
	{ SHIPS_CAPTURED, 25, BRONZE_PRIVATEER },
	{ SHIPS_CAPTURED, 50, SILVER_PRIVATEER },
	{ SHIPS_CAPTURED, 100, GOLD_PRIVATEER },
	{ SHIPS_CAPTURED, 500, PLATINUM_PRIVATEER },
	{ REPAIR, 50, BRONZE_SUPPORT },
	{ REPAIR, 500, SILVER_SUPPORT  },
	{ REPAIR, 1000, GOLD_SUPPORT  },
	{ REPAIR, 5000, PLATINUM_SUPPORT  }
};

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameProfile, Profile );

GameProfile::GameProfile()
{}

//---------------------------------------------------------------------------------------------------

float GameProfile::calculatePrestige() const
{
	float fPrestige = ( (operator[]( SHIPS_DAMAGED ) * 3.0f) + 
		(operator[]( SHIPS_CAPTURED ) * 15.0f) +
		(operator[]( PLANETS_DAMAGED ) * 1.0f) +
		(operator[]( PLANETS_CAPTURED ) * 5.0f) +
		(operator[]( FRIENDLY_FIRE ) * -3.0f) +
		(operator[]( REPAIR ) * 2.5f) +
		(operator[]( CONSTRUCTION ) * 1.0f) +
		(operator[]( KAMIKAZE ) * 5.0f) +
		(operator[]( SELF_DESTRUCTS ) * -10.0f) +
		(operator[]( JUMPS ) * 0.1f) +
		(operator[]( PLANET_COLLISIONS ) * -1.0f) +
		(operator[]( RESOURCES_LOST ) * -0.005f ) +
		(operator[]( SCOUT ) * 1.0f) +
		operator[]( BONUS_PRESTIGE ) );
	if ( fPrestige < 1.0f )
		fPrestige = 1.0f;
	
	return fPrestige;
}

float GameProfile::calculateModifier( Statistics nStat )
{
	switch( nStat )
	{
		case SHIPS_DAMAGED:
			return 3.0f;
		case SHIPS_CAPTURED:
			return 15.0f;
		case PLANETS_DAMAGED:
			return 1.0f;
		case PLANETS_CAPTURED:
			return 5.0f;
		case FRIENDLY_FIRE:
			return -3.0f;
		case REPAIR:
			return 2.5f;
		case CONSTRUCTION:
			return 1.0f;
		case KAMIKAZE:
			return 5.0f;
		case SELF_DESTRUCTS:
			return -10.0f;
		case JUMPS:
			return 0.1f;
		case PLANET_COLLISIONS:
			return -1.0f;
		case RESOURCES_LOST:
			return -0.005f;
		case SCOUT:
			return 1.0f;
		case BONUS_PRESTIGE:
			return 1.0f;
		default:
			return 1.0f;
	}

	return 1.0f;
}

float GameProfile::calculateRankUpPercent() const
{
	float pres = calculatePrestige();
	Rank therank = calculateRank();
	if( therank == CADET )		
		return pres / 1.0f; //cadet doesn't follow the pattern of 10 * 2^rank

	//the current prestige gained from the last rank divided by the prestige difference between the next rank and the current rank
	return (pres - (10 * (2 << (therank)))) / ((10 * (2 << (therank+1))) - (10 * (2 << (therank))));
}


Rank GameProfile::calculateRank() const
{
	return getRank( calculatePrestige() );
}

Rank GameProfile::getRank( float fPrestige ) const
{
	if ( fPrestige < 1.0f )
		return CADET;
	if ( fPrestige < 40.0f )
		return MIDSHIPMAN;
	if ( fPrestige < 80.0f )
		return ENSIGN;
	if ( fPrestige < 160.0f )
		return LIEUTENANT2;
	if ( fPrestige < 320.0f )
		return LIEUTENANT1;
	if ( fPrestige < 640.0f )
		return LIEUTENANT_COMMANDER;
	if ( fPrestige < 1280.0f )
		return COMMANDER;
	if ( fPrestige < 2560.0f )
		return CAPTAIN;
	if ( fPrestige < 5120.0f )
		return REAR_ADMIRAL2;
	if ( fPrestige < 10240.0f )
		return REAR_ADMIRAL1;
	if ( fPrestige < 20480.0f )
		return VICE_ADMIRAL;
	if ( fPrestige < 40960.0f )
		return ADMIRAL;
	if ( fPrestige < 81920.0f )
		return FLEET_ADMIRAL;
	if ( fPrestige < 163840.0f )
		return GRAND_ADMIRAL;
	if ( fPrestige < 327680.0f )
		return MARSHAL;
	return CHIEF_MARSHAL;
}

dword GameProfile::calculateBadges() const
{
	dword badges = 0;
	for(int i=0;i<sizeof(BADGE_REQUIREMENTS)/sizeof(BADGE_REQUIREMENTS[0]);++i)
	{
		if ( i >= 32 )
			break;

		BadgeRequirement & req = BADGE_REQUIREMENTS[ i ];
		if ( operator[]( req.m_eStatistic ) >= req.m_nLevel ) 
			badges |= 1 << req.m_eBadge;
	}

	return badges;
}

dword GameProfile::calculateMedals( dword badges ) const
{
	dword medals = 0;
	
	if ( BitTestAll<dword>( badges, (1<<BRONZE_COMBAT)|(1<<BRONZE_BOMBER)|(1<<BRONZE_TRANSPORT) ) )
		medals |= SIRIUS_COMBAT;
	if ( BitTestAll<dword>( badges, (1<<BRONZE_NAVIGATOR)|(1<<BRONZE_ENGINEER)|(1<<BRONZE_COMBAT)|
		(1<<BRONZE_TRANSPORT)|(1<<BRONZE_BOMBER)|(1<<BRONZE_SUPPORT) ) )
		medals |= BRONZE_STAR;
	if ( BitTestAll<dword>( badges, (1<<SILVER_NAVIGATOR)|(1<<SILVER_ENGINEER)|(1<<SILVER_COMBAT)|
		(1<<SILVER_TRANSPORT)|(1<<SILVER_BOMBER)|(1<<SILVER_SUPPORT) ) )
		medals |= SILVER_STAR;
	if ( BitTestAll<dword>( badges, (1<<GOLD_NAVIGATOR)|(1<<GOLD_ENGINEER)|(1<<GOLD_COMBAT)|
		(1<<GOLD_TRANSPORT)|(1<<GOLD_BOMBER)|(1<<GOLD_SUPPORT) ) )
		medals |= GOLD_STAR;
	if ( BitTestAll<dword>( badges, (1<<SILVER_COMBAT)|(1<<SILVER_BOMBER)|(1<<SILVER_TRANSPORT) ) )
		medals |= BRAVERY;
	if ( BitTestAll<dword>( badges, (1<<GOLD_COMBAT)|(1<<GOLD_BOMBER)|(1<<GOLD_TRANSPORT) ) )
		medals |= VALOR;
	if ( BitTestAll<dword>( badges, (1<<GOLD_ENGINEER)|(1<<SILVER_COMBAT) ) )
		medals |= DEFENSE;
	if ( BitTestAll<dword>( badges, (1<<SILVER_TRANSPORT)|(1<<SILVER_BOMBER) ) )
		medals |= STAR_FURY;
	if ( BitTestAll<dword>( badges, (1<<GOLD_TRANSPORT)|(1<<GOLD_BOMBER) ) )
		medals |= CONQUEROR;

	return medals;
}

//---------------------------------------------------------------------------------------------------

void GameProfile::load( const MetaClient::Profile & profile )
{
	Profile::load( profile );
}

void GameProfile::save( MetaClient::Profile & profile )
{
	profile.score = calculatePrestige();
	profile.rank = rankText( getRank( profile.score ) );
	
	Profile::save( profile );
}

int GameProfile::spawnScore()
{
	return calculateRank() + 1;				// CADETS (-1) are worth 0 on purpose
}

//---------------------------------------------------------------------------------------------------

const char * GameProfile::rankText( int rank )
{
	if ( rank < FIRST_RANK || rank > LAST_RANK )
		return "";
	static const char * RANK_TEXT[] = 
	{
		"Cadet",					// CADET
		"Midshipman",				// MIDSHIPMAN
		"Ensign",					// ENSIGN
		"2nd Lieutenant",			// LIEUTENANT2
		"1st Lieutenant",			// LIEUTENANT1
		"Lieutenant Commander",		// LIEUTENANT_COMMANDER,
		"Commander",				// COMMANDER,
		"Captain",					// CAPTAIN,
		"2nd Rear Admiral",			// REAR_ADMIRAL2,
		"1st Rear Admiral",			// REAR_ADMIRAL1,
		"Vice Admiral",				// VICE_ADMIRAL,
		"Admiral",					// ADMIRAL
		"Fleet Admiral",			// FLEET_ADMIRAL,
		"Grand Admiral",			// GRAND_ADMIRAL
		"Marshal",					// MARSHAL
		"Chief Marshal"				// CHIEF_MARSHAL
	};
	return RANK_TEXT[ rank + 1];
}

const char * GameProfile::rankShortText( int rank )
{
	if ( rank < FIRST_RANK || rank > LAST_RANK )
		return "";
	static const char * RANK_TEXT[] = 
	{
		"CD",					// CADET
		"MS",					// MIDSHIPMAN
		"EN",					// ENSIGN
		"2L",					// LIEUTENANT2
		"1L",					// LIEUTENANT1
		"LC",					// LIEUTENANT_COMMANDER,
		"CO",					// COMMANDER,
		"CP",					// CAPTAIN,
		"2A",					// REAR_ADMIRAL2,
		"1A",					// REAR_ADMIRAL1,
		"VA",					// VICE_ADMIRAL,
		"AD",					// ADMIRAL
		"FA",					// FLEET_ADMIRAL,
		"GA",					// GRAND_ADMIRAL
		"MA",					// MARSHAL
		"CM",					// CHIEF_MARSHAL
	};
	return RANK_TEXT[ rank + 1];
}

const char * GameProfile::badgeText( int badge )
{
	if ( badge < FIRST_BADGE || badge > LAST_BADGE )
		return "";

	static const char * BADGE_TEXT[] = 
	{
		"Bronze Navigator",		// BRONZE_NAVIGATOR,
		"Silver Navigator",		// SILVER_NAVIGATOR,
		"Gold Navigator",		// GOLD_NAVIGATOR,
		"Bronze Engineer",		// BRONZE_ENGINEER,
		"Silver Engineer",		// SILVER_ENGINEER,
		"Gold Engineer",		// GOLD_ENGINEER,
		"Bronze Combat",		// BRONZE_COMBAT,
		"Silver Combat",		// SILVER_COMBAT,
		"Gold Combat",			// GOLD_COMBAT,
		"Bronze Transport",		// BRONZE_TRANSPORT,
		"Silver Transport",		// SILVER_TRANSPORT,
		"Gold Transport",		// GOLD_TRANSPORT,
		"Bronze Bomber",		// BRONZE_BOMBER,
		"Silver Bomber",		// SILVER_BOMBER,
		"Gold Bomber",			// GOLD_BOMBER,
		"Bronze Privateer",		// BRONZE_PRIVATEER,
		"Silver Privateer",		// SILVER_PRIVATEER,
		"Gold Privateer",		// GOLD_PRIVATEER,
		"Bronze Support",		// BRONZE_SUPPORT,
		"Silver Support",		// SILVER_SUPPORT,
		"Gold Support",			// GOLD_SUPPORT,
		"Platinum Navigator",	// PLATINUM_NAVIGATOR,
		"Platinum Engineer",	// PLATINUM_ENGINEER,
		"Platinum Combat",		// PLATINUM_COMBAT,
		"Platinum Transport",	// PLATINUM_TRANSPORT,
		"Platinum Bomber",		// PLATINUM_BOMBER,
		"Platinum Privateer",	// PLATINUM_PRIVATEER,
		"Platinum Support",		// PLATINUM_SUPPORT,
	};
	return BADGE_TEXT[ badge ];
}

const char * GameProfile::medalText( int medal )
{
	if ( medal < FIRST_MEDAL || medal > LAST_MEDAL )
		return "";
	static const char * MEDAL_TEXT[] = 
	{
		"Sirus Combat Medal",	// SIRIUS_COMBAT,
		"Bronze Star",			// BRONZE_STAR,
		"Silver Star",			// SILVER_STAR,
		"Gold Star",			// GOLD_STAR,
		"Medal of Bravery",		// BRAVERY,
		"Medal of Valor",		// VALOR,
		"Defense Medal",		// DEFENSE,
		"Star Fury Medal",		// STAR_FURY,
		"Medal of Conquest",	// CONQUEROR,
	};
	return MEDAL_TEXT[ medal ];
}

const char * GameProfile::statisticName( Statistics nStat )
{
	if ( nStat < FIRST_STATISTIC || nStat > LAST_STATISTIC )
		return "";

	static const char * STAT_NAME[] = 
	{
		"Games Played",			// GAMES_PLAYED
		"Time Played",			// TIME_PLAYED,
		"Kills",				// KILLS,
		"Killed",				// KILLED,
		"Bonus Prestige",		// BONUS_PRESTIGE,
		"Credits",				// CREDITS
		"Resources",			// RESOURCES
		"Ships Damaged",		// SHIPS_DAMAGED,
		"Ships Captured",		// SHIPS_CAPTURED,
		"Planets Damaged",		// PLANETS_DAMAGED,
		"Planets Captured",		// PLANETS_CAPTURED,
		"Friendly Fire",		// FRIENDLY_FIRE,
		"Repair",				// REPAIR,
		"Construction",			// CONSTRUCTION,
		"Kamikaze",				// KAMIKAZE,
		"Self Destructs",		// SELF_DESTRUCTS,
		"Jumps",				// JUMPS,
		"Planet Collisions",	// PLANET_COLLISIONS,
		"Resources Lost",		// RESOURCES_LOST,
		"Scout",				// SCOUT,
	};
	return STAT_NAME[ nStat ];
}

//---------------------------------------------------------------------------------------------------
//EOF
