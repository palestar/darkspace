/*
	NounSpawnShip.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Standard/Time.h"
#include "Debug/Profile.h"
#include "Math/Helpers.h"
#include "File/FileDisk.h"
#include "NounSpawnShip.h"
#include "VerbRevolt.h"
#include "VerbPlanetEvent.h"
#include "GameContext.h"

//! Define to non-zero to enable despawning.
#define EANBLE_DESPAWNING			0

//! The CPU load must be below this amount for ships to be spawned, this help prevents overloaded server issues..
static Constant		SPAWN_AI_LOAD( "SPAWN_AI_LOAD", 0.75f );
//! what is the max spawn per player
static Constant		MAX_SPAWN_PER_PLAYER( "MAX_SPAWN_PER_PLAYER", 8.0f );

//---------------------------------------------------------------------------------------------------

struct RomanDigit
{
	const char *		m_psString;
	unsigned int		m_nValue;
};

static const RomanDigit ROMAN_DIGITS[]=
{
	{"M",  1000},
	{"CM",  900},
	{"D",   500},
	{"CD",  400},
	{"C",   100},
	{"XC",   90},
	{"L",    50},
	{"XL",   40},
	{"X",    10},
	{"IX",    9},
	{"V",     5},
	{"IV",    4},
	{"I",     1},
};

static const bool checkAllowedShips(NounShip::Type type)
{
	static const bool ALLOWED_SHIPS[] =
	{
		// list of allowed ship type spawners
		false,	// UNKNOWN,
		false,	// FIGHTER,
		false,	// ENGINEER,
		true,	// SUPPLY,
		false,	// TRANSPORT,
		false,	// SCOUT,
		false,	// FRIGATE,
		true,	// DESTROYER,
		true,	// CRUISER,
		true,	// DREAD,
		false,	// STATION,
		false,	// PLATFORM,
		false,	// PLATFORM_SUPPLY
	};

	return ALLOWED_SHIPS[type];
}

static CharString GenerateRomanNumerals( unsigned int a_nNumber )
{
	CharString digits;
	for(int i=0; a_nNumber > 0 && i < sizeof(ROMAN_DIGITS)/sizeof(ROMAN_DIGITS[0]);++i)
	{
		while( ROMAN_DIGITS[i].m_nValue <= a_nNumber )
		{
			digits += ROMAN_DIGITS[i].m_psString;
			a_nNumber -= ROMAN_DIGITS[i].m_nValue;
		}
	}

	return digits;
}

//---------------------------------------------------------------------------------------------------
const int BASE_FACTION_AI = 45;
const int MIN_SPAWN_DELAY = TICKS_PER_SECOND * 5;
const int MAX_ATTEMPTS = 10;
const int MAX_PLACEMENT_ATTEMPTS = 10;

//---------------------------------------------------------------------------------------------------

IMPLEMENT_NAMED_WIDGET_TYPES( TYPES_Spawner, NounSpawnShip::Spawner );
IMPLEMENT_NAMED_FACTORY( FACTORY_Spawner, NounSpawnShip::Spawner, Widget, true );
BEGIN_NAMED_PROPERTY_LIST( PLIST_Spawner, NounSpawnShip::Spawner, Widget );
	ADD_PROPERTY( m_pNounTemplate );
	ADD_PROPERTY( m_sBaseName );
	ADD_PROPERTY( m_bNumbered );
	ADD_PROPERTY( m_nLimit );
	ADD_PROPERTY( m_nChance );
	ADD_PROPERTY( m_nScore );
	ADD_ENUM_PROPERTY( m_Order );
		ADD_ENUM_OPTION( m_Order, NounShip::NOORDER );
		ADD_ENUM_OPTION( m_Order, NounShip::ATTACK );
		ADD_ENUM_OPTION( m_Order, NounShip::DEFEND );
		ADD_ENUM_OPTION( m_Order, NounShip::CAPTURE );
		ADD_ENUM_OPTION( m_Order, NounShip::MOVE );
		ADD_ENUM_OPTION( m_Order, NounShip::RELOAD );
		ADD_ENUM_OPTION( m_Order, NounShip::BEACON );
		ADD_ENUM_OPTION( m_Order, NounShip::HOLD );
		ADD_ENUM_OPTION( m_Order, NounShip::TRADE );
		ADD_ENUM_OPTION( m_Order, NounShip::RECON );
		ADD_ENUM_OPTION( m_Order, NounShip::BUILD );
		ADD_ENUM_OPTION( m_Order, NounShip::FALLBACK );
	ADD_PROPERTY( m_sOrderTarget );
	ADD_PROPERTY( m_Spawned );
END_PROPERTY_LIST();

void NounSpawnShip::Spawner::update()
{
	// remove any destroyed ships..
	for(int i=m_Spawned.size()-1;i>=0;--i)
	{
		if ( !m_Spawned[i].valid() || m_Spawned[i]->isDestroyed() || m_Spawned[i]->parent() == NULL )
			m_Spawned.removeSwap( i );
	}
}

NounShip::Ref NounSpawnShip::Spawner::spawn( WorldContext * a_pContext )
{
	if (! m_pNounTemplate )
		return NULL;

	// use a reference pointer, so the object will be deleted if not a NounShip object..
	Noun::Ref pUncasted = m_pNounTemplate->spawn();

	NounShip * pShip = WidgetCast<NounShip>( pUncasted );
	if (! pShip )
		return NULL;

	// set the ship to the rank required to get the ship, this prevents new players (ensigns) from giving orders
	// to AI dreadnoughts... 
	pShip->setRank( m_pNounTemplate->rank() );

	CharString sName = m_bRandom ? CharString( pickRandomName( m_nFaction, pShip->type() ) ) : m_sBaseName;
	if(m_bNumbered)
		sName += CharString(" ") + GenerateRomanNumerals( m_nNextNumber++ );

	pShip->setName( sName );
	m_Spawned.push( pShip );

	return pShip;
}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounSpawnShip, NounBody );
REGISTER_FACTORY_KEY( NounSpawnShip, 4153031812113093571LL );

BEGIN_PROPERTY_LIST( NounSpawnShip, NounBody )
	ADD_PROPERTY( m_nSpawnType );
	ADD_PROPERTY( m_nSpawnDelay );
	ADD_PROPERTY( m_fSpawnArea );
	ADD_PROPERTY( m_nMinSpawn );
	ADD_PROPERTY( m_nMaxSpawn );
	ADD_PROPERTY( m_nMinPlanets );
	ADD_PROPERTY( m_sTimeMask );
	ADD_PROPERTY( m_Spawners );
END_PROPERTY_LIST();

NounSpawnShip::NounSpawnShip() :
	m_nSpawnType( ST_NORMAL ),
	m_nSpawnTick( 0 ),
	m_nSpawnDelay( TICKS_PER_SECOND * 90 ),
	m_fSpawnArea( 0.0f ),
	m_nMinSpawn( 0 ),
	m_nMaxSpawn( 1 ),
	m_nMinPlanets( 0 ),
	m_nLastTime( Time::CPU() )
{}

//----------------------------------------------------------------------------

NounType NounSpawnShip::nounType() const
{
	return TYPE_SERVER_OBJECT;
}

bool NounSpawnShip::canBeDetected() const
{
	return false;		// object cannot be detected
}

void NounSpawnShip::initialize()
{
	NounBody::initialize();

	m_nSpawnTick = 0;
	if ( m_nSpawnDelay < MIN_SPAWN_DELAY )
		m_nSpawnDelay = MIN_SPAWN_DELAY;

	// validate that we have valid spawner objects..
	for(int i=0;i<m_Spawners.size();)
	{
		if (! m_Spawners[i].valid() )
		{
			TRACE( "Removing bad spawner!" );
			m_Spawners.removeSwap( i );
		}
		else
			++i;
	}

	//add the factions to our spawners for the ship name
	for(int i=0;i<m_Spawners.size(); i++)
		m_Spawners[i]->setFaction( factionId() );
}

void NounSpawnShip::release()
{
	NounBody::release();
}

void NounSpawnShip::simulate( dword nTick )
{
	if ( isServer() && !isSleeping() && nTick >= m_nSpawnTick && m_Spawners.size() > 0 )
	{
		// reset the count
		m_nSpawnTick = nTick + m_nSpawnDelay;

		if ( m_nMinPlanets > 0 )
		{
			int				nFactionPlanets = 0;
			int				nFactionId = factionId();
			NounPlanet *	pBestPlanet = NULL;
			float			fBestDistance = 0.0f;

			for(int j=0;j<context()->zoneCount() && nFactionPlanets < m_nMinPlanets;j++)
			{
				NodeZone * pZone = context()->zone( j );
				for(int i=0;i<pZone->childCount() && nFactionPlanets < m_nMinPlanets;i++)
				{
					NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
					if (! pPlanet )
						continue;
					if ( pPlanet->maxUnits() <= 0 )
						continue;

					if ( pPlanet->factionId() != nFactionId )
					{
						float fDistance = (pPlanet->worldPosition() - worldPosition()).magnitude();
						if ( !pBestPlanet || fDistance < fBestDistance )
						{
							pBestPlanet = pPlanet;
							fBestDistance = fDistance;
						}
						continue;
					}

					++nFactionPlanets;
				}
			}

			if( nFactionPlanets < m_nMinPlanets )
			{
				Verb::Ref( new VerbRevolt( pBestPlanet, teamId() ) );
				Verb::Ref( new VerbPlanetEvent( pBestPlanet, VerbPlanetEvent::NOT_CAPTURABLE, true ) );
			}
		}

		// if CPU load over a set amount do not spawn any new AI...
		if ( Profiler::CPUused() < SPAWN_AI_LOAD )
		{
			// update all current spawner objects and tally the total chance & spawn score ...
			int nTotalChance = 0, nTotalSpawnScore = 0;
			for (int i = 0; i < m_Spawners.size(); ++i)
			{
				Spawner * pSpawner = m_Spawners[i];
				// update the spawner first, so it will remove any dead ships that might affect it's score..
				pSpawner->update();

				// tally up some values..
				nTotalSpawnScore += pSpawner->spawnedScore();
				if (pSpawner->limit() > 0 && pSpawner->spawnedCount() >= pSpawner->limit())
					continue;		// at limit, skip this spawner..
				nTotalChance += pSpawner->chance();
			}

			int nMaxSpawn = m_nMaxSpawn;
			switch (m_nSpawnType)
			{
			case ST_NORMAL:
			case ST_ONE_SHOT:
				break;
			case ST_POPULATION:
			{
				// max spawn increases as player populations increase.. 
				int nPlayers = context()->user()->playerCount();
				int nMaxPlayers = context()->user()->maxPlayers();
				float fSpawnScale = 0.0f;
				if (nMaxPlayers > 0)
					fSpawnScale = ((float)nPlayers) / ((float)nMaxPlayers);
				nMaxSpawn = (int)ceilf(fSpawnScale * nMaxSpawn);
			}
			break;
			case ST_FACTION:
			{
				// the max spawn is the difference between the number of spawned ships on this faction
				// and the faction with the most spawned ships. (including our own faction)
				nMaxSpawn = context()->user()->maxPlayerScoreAI() + BASE_FACTION_AI;
				nTotalSpawnScore = context()->user()->spawnedScore(factionId());
			}
			break;
			}

			if (nMaxSpawn > m_nMaxSpawn)
				nMaxSpawn = m_nMaxSpawn;
			if (nMaxSpawn < m_nMinSpawn)
				nMaxSpawn = m_nMinSpawn;

			if (nTotalSpawnScore < nMaxSpawn)
			{
				// spawn ships... 
				int nAttempts = 0;
				while (nTotalChance > 0 && nTotalSpawnScore < nMaxSpawn)
				{
					// increment the number of attempts..
					if (++nAttempts > MAX_ATTEMPTS)
						break;

					// select a random spawner
					int nPick = rand() % nTotalChance;

					Spawner * pSpawner = NULL;
					for (int i = 0; i < m_Spawners.size() && nPick >= 0; ++i)
					{
						pSpawner = m_Spawners[i];
						if (pSpawner->limit() > 0 && pSpawner->spawnedCount() >= pSpawner->limit())
							continue;		// at limit, skip this spawner..
						nPick -= pSpawner->chance();
					}

					if (!pSpawner)
						continue;
					NounShip::Ref pSpawn = pSpawner->spawn(context());
					if (!pSpawn)
						continue;

					int nPlaceAttempts = 0;
					while (nPlaceAttempts < MAX_PLACEMENT_ATTEMPTS)
					{
						Vector3 vOffset(RandomFloat(-m_fSpawnArea, m_fSpawnArea), 0.0f, RandomFloat(-m_fSpawnArea, m_fSpawnArea));
						Vector3 vPosition(worldPosition() + vOffset);

						Array< GameContext::NounCollision > collisions;
						context()->proximityCheck(vPosition, pSpawn->radius() * 1.5f, collisions);

						bool bSpawnClear = true;
						for (int i = 0; i < collisions.size() && bSpawnClear; ++i)
						{
							if (WidgetCast<NounBody>(collisions[i].pNoun) != NULL)
								bSpawnClear = false;
						}

						if (bSpawnClear)
						{
							pSpawn->setPosition(vPosition);
							break;
						}

						++nPlaceAttempts;
					}

					if (nPlaceAttempts >= MAX_PLACEMENT_ATTEMPTS)
						break;

					Matrix33 vFrame(worldFrame());
					pSpawn->setFrame(vFrame);
					pSpawn->setTeamId(teamId());
					pSpawn->setHeading(atan2(vFrame.k.x, vFrame.k.z));
					pSpawn->setup();

					context()->attachNoun(pSpawn);

					nTotalSpawnScore += pSpawner->score();
					LOG_STATUS( "NounSpawnShip", "Spawning Ship, Type: %s, Name: %s, Weight: %d, Faction: %s, Spawn Score: %d/%d", 
						NounShip::typeText( pSpawn->type() ), pSpawn->name(), pSpawn->gadgetLevel(), factionText( pSpawn->factionId() ), nTotalSpawnScore, nMaxSpawn );

					// set the order after the object is attached on purpose
					if ( pSpawner->order() != NounShip::NOORDER )
					{
						Noun * pOrderTarget = context()->findNoun( pSpawner->orderTarget() );
						pSpawn->setOrder( pSpawner->order(), pOrderTarget, NounShip::wRef( pSpawn ) );
					}

					if ( pSpawner->limit() > 0 && pSpawner->spawnedCount() >= pSpawner->limit() )
						nTotalChance -= pSpawner->chance();

					// spawn only one ship at a time, this prevents floods of AI entering too quickly into the MV..
					if ( m_nSpawnType != ST_ONE_SHOT || m_nSpawnType == ST_FACTION )
						break;
				}
			}
#if EANBLE_DESPAWNING
			else if ( nTotalSpawnScore > nMaxSpawn )
			{
				int nDifference = nTotalSpawnScore - nMaxSpawn;

				// spawned ships exceed max score, try to find a ship to order back to the gate..
				Array< NounShip::Ref > ships;
				for( int i=0;i<m_Spawners.size();++i)
				{
					Spawner * pSpawner = m_Spawners[i];
					for( int j=0;j<pSpawner->spawnedCount();++j)
					{
						NounShip * pShip = pSpawner->spawned(j);
						if (! pShip || pShip->isDestroyed() || (pShip->rank() + 2) > nDifference )
							continue;

						ships.push( pShip );
					}
				}

				// pick a random ship and order it home..
				if ( ships.size() > 0 )
				{
					int nPicked = rand() % ships.size();

					NounShip * pSendHome = ships[ nPicked ];

					LOG_STATUS( "NounSpawnShip", "Despawning ship, Name: %s, Rank: %d, Faction: %s, Spawn Score: %d/%d", 
						pSendHome->name(), pSendHome->rank(), factionText( pSendHome->factionId() ), nMaxSpawn, nTotalSpawnScore );

					pSendHome->addFlags( NounShip::FLAG_NO_BEST_ORDER );		// don't allow them to pick a new order
					pSendHome->addFlags( NounShip::FLAG_NO_EXPIRE_ORDER );		
					pSendHome->setOrder( NounShip::NOORDER, NULL, pSendHome );	// set the ship to go home
				}
			}
#endif
			// detach this node from the world if this will never recur again
			if ( m_nSpawnType == ST_ONE_SHOT )
				setDetach();
		}
	}

	NounBody::simulate( nTick );
}

//---------------------------------------------------------------------------------------------------

void NounSpawnShip::addSpawner( Spawner * a_pSpawn )
{
	m_Spawners.push( a_pSpawn );
}

void NounSpawnShip::removeSpawner( int n )
{
	m_Spawners.removeSwap( n );
}

void NounSpawnShip::releaseSpawners()
{
	m_Spawners.release();
}

//---------------------------------------------------------------------------------------------------

NounSpawnShip::Ref NounSpawnShip::makeTeamSpawner( GameContext * a_pContext, int a_nTeamId, Noun * a_pParent, bool a_bGateSpawner )
{
	if (! a_pContext->isTeamValid( a_nTeamId ) )
		return NULL;

	const WorldContext::Team & team = a_pContext->team( a_nTeamId );
	if (! team.pTemplates.valid() )
		return NULL;		// no valid collection for this team

	LOG_STATUS( "NounSpawnShip", "Making team spawner for %s.", team.name.cstr() );

	// make sure all ships templates are loaded..
	team.pTemplates->load();

	Ref pShipSpawner = new NounSpawnShip();
	pShipSpawner->setTeamId( a_nTeamId );
	pShipSpawner->setSpawnType( NounSpawnShip::ST_FACTION );
	pShipSpawner->setSpawnArea( 1000.0f );
	pShipSpawner->setMaxSpawn( (int)(a_pContext->user()->maxPlayers() * MAX_SPAWN_PER_PLAYER) );

	for(int i=0;i<team.pTemplates->resourceCount();i++)
	{
		NounTemplate::Link pTemplate = team.pTemplates->resource( i );
		if (! pTemplate.valid() )
			continue;

		NounShip * pShip = WidgetCast<NounShip>( pTemplate->noun() );
		if (! pShip )
			continue;		// invalid ship
		if (!checkAllowedShips(pShip->type()))
			continue;		// ship not in array
		//if ( a_bGateSpawner && !pShip->enableGateSpawn() )
		//	continue;		// ship can't be spawned at gate

		// We used to use ship rank to determine score, but with the tier
		// changes, a single Engineer could equal one high ranked player in
		// the event it got picked to spawn
		int nRank = pShip->gadgetLevel();

		NounSpawnShip::Spawner * pSpawner = new NounSpawnShip::Spawner();
		pSpawner->setNounTemplate( pTemplate );
		pSpawner->setBaseName( pShip->name() );
		pSpawner->setNumbered( true );
		pSpawner->setRandom( true );
		pSpawner->setLimit( Max<int>( 25 / nRank, 1 ) );
		pSpawner->setChance( 100 / nRank );
		pSpawner->setScore( nRank );

		pShipSpawner->addSpawner( pSpawner );
	}

	// attach the spawner into the world!
	a_pContext->attachNoun( pShipSpawner, a_pParent );

	return pShipSpawner;
}

//---------------------------------------------------------------------------------------------------

typedef std::vector< std::string >						NameList;
typedef std::map< int, NameList >						TypeNameMap;
typedef std::map< int, TypeNameMap >					FactionTypeMap;

const char * NounSpawnShip::pickRandomName( int a_nFactionId, NounShip::Type a_eType )
{
	static FactionTypeMap		s_RandomNames;
	static bool					s_bNamesLoaded = false;

	if (! s_bNamesLoaded )
	{
		s_bNamesLoaded = true;

		char * pNameFile = FileDisk::loadTextFile( "ShipNameManfiest.txt" );
		if ( pNameFile == NULL )
		{
			LOG_ERROR( "NounSpawnShip", "Failed to load ShipNameManfiest.txt from disk." );
			return "?";
		}

		char * pParse = pNameFile;
		while( *pParse != 0 )
		{
			while( *pParse == '\n' || *pParse == '\r' )
				++pParse;

			char * pLineStart = pParse;

			// scan to the end of the line
			while( *pParse != '\r' && *pParse != '\n' && *pParse != 0 )
				++pParse;

			if ( strlen( pLineStart ) > 0 && *pLineStart != ';' )
			{
				int nFactionId = -1;
				int nType = -1;
				char sShipName[ 80 ];
				
				if ( sscanf( pLineStart, "%d,%d,%[^\r\n]s", &nFactionId, &nType, sShipName ) == 3 )
					s_RandomNames[ nFactionId ][ nType ].push_back( sShipName );
			}
		}

		FileDisk::unloadTextFile( pNameFile );
	}

	FactionTypeMap::iterator iFactionNames = s_RandomNames.find( a_nFactionId );
	if ( iFactionNames != s_RandomNames.end() )
	{
		TypeNameMap::iterator iNameList = iFactionNames->second.find( a_eType );
		if ( iNameList != iFactionNames->second.end() )
		{
			NameList & names = iNameList->second;
			return names[ rand() % names.size() ].c_str();
		}

		iNameList = iFactionNames->second.find( NounShip::UNKNOWN );
		if ( iNameList != iFactionNames->second.end() )
		{
			NameList & names = iNameList->second;
			return names[ rand() % names.size() ].c_str();
		}
	}

	iFactionNames = s_RandomNames.find( FACTION_UNKNOWN );
	if ( iFactionNames != s_RandomNames.end() )
	{
		TypeNameMap::iterator iNameList = iFactionNames->second.find( a_eType );
		if ( iNameList != iFactionNames->second.end() )
		{
			NameList & names = iNameList->second;
			return names[ rand() % names.size() ].c_str();
		}

		iNameList = iFactionNames->second.find( NounShip::UNKNOWN );
		if ( iNameList != iFactionNames->second.end() )
		{
			NameList & names = iNameList->second;
			return names[ rand() % names.size() ].c_str();
		}
	}

	return "?";
}

//----------------------------------------------------------------------------
//EOF
