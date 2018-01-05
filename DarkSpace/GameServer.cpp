/*
	GameServer.cpp

	This object is used to route verbs for one or more clients
	(c)1999 Palestar Development, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Debug/ExceptionHandler.h"
#include "Standard/Settings.h"

#include "Standard/Time.h"
#include "GameServer.h"
#include "GameClient.h"
#include "GameProfile.h"
#include "NounStar.h"
#include "NounJumpGate.h"
#include "NounSpawnShip.h"
#include "NounUnit.h"
#include "NounTrail.h"
#include "TraitOrbit.h"
#include "VerbJumpAbort.h"
#include "VerbSpawn.h"
#include "CargoEnhancement.h"
#include "StructureShipyard.h"
#include "ProxyShip.h"
#include "TraitDisplayName.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_LIGHT_FACTORY( ProxyShip, NounShip);

dword GameServer::sm_nAlwaysSpawnFlags		= MetaClient::ADMINISTRATOR|MetaClient::DEVELOPER;
bool GameServer::sm_bEnableTeamSpawners		= true;
bool GameServer::sm_bHiddenServer			= false;

//---------------------------------------------------------------------------------------------------

//! What percent of damage is repaired per second in storage..
DLL_EXPORT Constant STORAGE_REPAIR_PER_SECOND( "STORAGE_REPAIR_PER_SECOND", 1.0f / (12.0f * 3600.0f) );
//! What percentage of resources does it cost to spawn a ship from a location other than it's last location
DLL_EXPORT Constant TRANSFER_SPAWN_COST( "TRANSFER_SPAWN_COST", 0.1f );
//! When in a group, this is multiplied by all stat amounts to give a slight bonus or reduction in points given.
static Constant		GROUP_STAT_BONUS( "GROUP_STAT_BONUS", 0.1f );
//! When subscribed, the users prestige is boosted by this much (default = 1.1 = 10%)
static Constant		SUBSCRIBER_PRESTIGE_BONUS( "SUBSCRIBER_PRESTIGE_BONUS", 1.1f );
//! For each level in rank how much does it scale the resources lost
static Constant		RESOURCES_LOST_SCALE_PER_RANK( "RESOURCES_LOST_SCALE_PER_RANK", 0.04f );

const int MAX_CARGO_IN_STORAGE = 50;
const int MAX_SHIPS_IN_STORAGE = 8;
const int MAX_SUB_SHIPS_IN_STORAGE = 12;
const int MAX_SPAWN_ATTEMPTS = 10;
const dword LOG_SHIP_STATS_INTERVAL = 300;

//! Enable prestige loss on death
#define ENABLE_PRESTIGE_LOSS_ON_DEATH			0

//---------------------------------------------------------------------------------------------------

static bool GetKeyValue( const char * a_pData, const char * a_pKey, CharString & a_sValue )
{
	const char * pValue = strstr( a_pData, a_pKey );
	if (! pValue )
		return false;
	
	const char * pEqual = strchr( pValue, '=' );
	if (! pEqual )
		return false;
	pEqual += 1;

	while( *pEqual && *pEqual != ';' )
		a_sValue += *pEqual++;

	return true;
}

static int GetKeyIntegerValue( const char * a_pData, const char * a_pKey )
{
	CharString sValue;
	if (! GetKeyValue( a_pData, a_pKey, sValue ) )
		return 0;

	return atoi( sValue );
}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameServer, WorldServer );

GameServer::GameServer() : m_nTopFaction( 0 ), m_nNextShipStatLog( 0 )
{}

//---------------------------------------------------------------------------------------------------

void GameServer::onDisconnect( dword a_nClientId )
{
	ClientContextHash::iterator iContext = m_ClientContext.find( a_nClientId );
	if ( iContext != m_ClientContext.end() )
	{
		ClientContext & context = iContext->second;

		// BUG#977 - abort any jump on disconnect
		NounShip * pShip = WidgetCast<NounShip>( context.pSelf );
		if ( pShip != NULL )
		{
			GadgetJumpDrive * pJD = pShip->jumpDrive();
			if ( pJD != NULL && (pJD->engaged() || pJD->jumping()) )
				Verb::Ref( new VerbJumpAbort( pShip ) );
		}
	}

	WorldServer::onDisconnect( a_nClientId );
}

//---------------------------------------------------------------------------------------------------

dword GameServer::version() const
{
	return GetProgramVersion();
}

bool GameServer::update()
{
	if ( Time::seconds() >= m_nNextShipStatLog )
	{
		Array<Noun::Ref> ships;
		gameContext()->findNouns( ships, CLASS_KEY(NounShip) );

		// log all ships spawned by faction and ship type..
		for(int f=0;f<FACTION_COUNT;++f)
		{
			int nShipsTypes[ NounShip::TYPE_COUNT ];
			int nShipTypesAI[ NounShip::TYPE_COUNT ];
			for(int i=0;i<NounShip::TYPE_COUNT;++i)
			{
				nShipsTypes[i] = 0;
				nShipTypesAI[i] = 0;
			}

			int nSpawnScore = 0;
			int nFactionShipCount = 0;
			for(int j=0;j<ships.size();++j)
			{
				NounShip * pShip = (NounShip *)ships[j].pointer();
				if ( pShip->factionId() == f )
				{
					if ( pShip->rank() <= LAST_RANK )
						nSpawnScore += pShip->rank() + 1;

					++nFactionShipCount;
					++nShipsTypes[ pShip->type() ];
					if ( pShip->userId() == 0 )
						++nShipTypesAI[ pShip->type() ];
				}
			}

			CharString sShipTypes;
			for(int i=0;i<NounShip::TYPE_COUNT;++i)
			{
				if ( nShipsTypes[i] > 0 )
					sShipTypes += CharString().format( ",%s,%d,%d", NounShip::typeText( (NounShip::Type)i ), nShipsTypes[i], nShipTypesAI[i] );
			}

			LOG_STATUS( "GameServer", "[STAT] %s,%d,%d%s", factionText( f ), nFactionShipCount, nSpawnScore, sShipTypes.cstr() );
		}
		m_nNextShipStatLog = Time::seconds() + LOG_SHIP_STATS_INTERVAL;
	}

	return WorldServer::update();
}

bool GameServer::createProfile( ClientContext & context )
{
	context.pProfile = new GameProfile();
	context.pProfile->load( context.MetaProfile );
	return true;
}

bool GameServer::createProxy( ClientContext & context, const WidgetKey & a_nObjectId, int a_nFactionId )
{
	if (! context.bProxy )
		return true;

	GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
	if (! pProfile )
		return false;

	// validate the team for this proxy client..
	if ( context.nFactionId == 0 || context.nFactionId != a_nFactionId )
		setClientTeam( 0, context.nClientId, worldContext()->factionTeam( a_nFactionId ) );

	if ( a_nObjectId != NULL_WIDGET )
	{
		ProxyShip * pShip = new ProxyShip();
		pShip->setKey( a_nObjectId );
		pShip->setName( context.MetaProfile.name );
		pShip->setUserId( context.MetaProfile.userId );
		pShip->setTeamId( context.nTeamId );
		pShip->setRank( pProfile->calculateRank() );		// set the rank so the player can order ships..
		context.pSelf = pShip;

		// attach the proxy object into the world... this is a server-side only object
		// it will never be sent to any clients.
		worldContext()->attachNoun( pShip );
	}
	else
	{
		context.pSelf = NULL;
	}

	return true;
}

bool GameServer::addPlayerProfile( dword nUserId, const char * pField, float fAmount )
{
	bool bCredits = strcmp( pField, "Credits" ) == 0;

	UserClientHash::iterator iClient = m_UserClientHash.find( nUserId );
	if ( iClient == m_UserClientHash.end() )
		return false;
    
	// if the player is local, then send out messages to all other players in this zone when they gain rank, badges, or medals
	ClientContextHash::iterator iContext = m_ClientContext.find( iClient->second );
	if ( iContext == m_ClientContext.end() )
		return false;

	ClientContext & context = iContext->second;
	GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
	if (! pProfile )
		return false;

	int nRank = pProfile->calculateRank();
	if(nRank < 0)
		nRank = 0;

	// no rank gain for players over the maxrank for the server, unless it's "Credits"...
	if ( nRank <= m_Context.maxRank || bCredits )	
	{
		if (! WorldServer::addPlayerProfile( nUserId, pField, fAmount ) )
			return false;

		dword nBadges = pProfile->calculateBadges();
		dword nMedals = pProfile->calculateMedals( nBadges );

		pProfile->addStatistic( pField, fAmount );

		int nNewRank = pProfile->calculateRank();
		if ( nNewRank < 0 )
			nNewRank = 0;
		if ( nNewRank != nRank )
		{
			// player has changed rank, send message to all clients
			sendChat( CharString().format( "<color;ffffff>Comms: '%s' has been %s to '%s'", pProfile->name(), 
				nNewRank > nRank ? "promoted" : "demoted", GameProfile::rankText( nNewRank ) ) );
		}

		dword nNewBadges = pProfile->calculateBadges();
		if ( nNewBadges != nBadges )
		{
			dword nNewMedals = pProfile->calculateMedals(nNewBadges);

			for(int i=0;i<32;i++)
			{
				dword bit = 1 << i;
				if ( (nBadges & bit) == 0 && (nNewBadges & bit) != 0 )
				{
					sendChat( CharString().format( "<color;ffffff>Comms: '%s' has been awarded the '%s' badge", 
						pProfile->name(), GameProfile::badgeText( i ) ) );
				}

				if ( (nMedals & bit) == 0 && (nNewMedals & bit) != 0 )
				{
					sendChat( CharString().format( "<color;ffffff>Comms: '%s' has been awarded the '%s'", 
						pProfile->name(), GameProfile::medalText( i ) ) );
				}
			}
		}
	}

	return true;
}

bool GameServer::publishServer()
{
	if (! checkMetaServer() )
		return false;		// not connected to the metaserver!
	if (! m_Context.bPublish )
		return false;		// this server isn't published

	// connect jump gates to remote servers...
	Array< MetaClient::Server > servers;
	if ( m_MetaClient.getServers( m_Context.name, m_Context.gameId, 0, servers ) < 0 )
	{
		LOG_ERROR( "GameServer", "Failed to get server list!" );
		return false;
	}

	AutoLock lock( &m_Lock );
		
	// update game information
	static const char * CONTEXT_TYPES[] = 
	{
		"Scenario",
		"Combat",
		"Clan",
		"Metaverse",
		"Tutorial"
	};

	// if no name was supplied, use the name of this context
	GameContext * pContext = gameContext();
	if ( pContext == NULL )
		return false;

	// calculate the time and time remaining
	dword tick = pContext->tick();

	CharString sGameTime;
	sGameTime.format("%2.2u:%2.2u:%2.2u", tick / TICKS_PER_HOUR, 
		(tick / TICKS_PER_MINUTE) % 60, (tick / TICKS_PER_SECOND) % 60 );

	if ( pContext->timeLimit() > 0 )
	{
		dword remain = pContext->timeLimit() - tick;
		sGameTime += CharString().format(" - %2.2u:%2.2u:%2.2u", remain / TICKS_PER_HOUR, 
			(remain / TICKS_PER_MINUTE) % 60, (remain / TICKS_PER_SECOND) % 60 );
	}

	// update the meta server
	m_Server.gameId = m_Context.gameId;
	m_Server.type = sm_bHiddenServer ? MetaClient::GAME_SUB_SERVER : MetaClient::GAME_SERVER;
	m_Server.name = m_Context.name; 
	m_Server.port = m_Context.port;
	m_Server.maxClients = m_Context.maxPlayers;
	m_Server.clients = m_nPlayers;

	CharString sGameInfo;
	if ( pContext->type() != GameContext::METAVERSE )
	{
		m_Server.shortDescription = CharString().format("%s : %s", CONTEXT_TYPES[ pContext->type() ], pContext->name() );

		sGameInfo += pContext->description();
		sGameInfo += "\n";

		sGameInfo += CharString().format( 
			"\nTime:       %s"
			"\nBalanced:   %s"
			"\nClan:       %s"
			"\nMax Rank:   %s"
			"\nVersion:    %6.6u", 
			sGameTime.cstr(),
			m_Context.bBalanced ? "Yes" : "No",
			m_Context.bClan ? "Yes" : "No",
			GameProfile::rankText( m_Context.maxRank ),
			version() );
	}
	else
	{
		sGameInfo += CharString().format( 
			"\nTime:       %s"
			"\nVersion:    %6.6u", 
			sGameTime.cstr(),
			version() );

		if( !sm_bHiddenServer )
		{
			m_Server.shortDescription = pContext->name();
			sGameInfo += "\n\nSectors:";

			// check all servers in our cluster, our clients count should population of the top-most server..
			int nServerCount = 0;
			for(int i=0;i<servers.size();++i)
			{
				MetaClient::Server & server = servers[i];
				if( server.name != m_Context.name )
					continue;		// verify the server is in our cluster
				if ( server.type != MetaClient::GAME_SERVER && server.type != MetaClient::GAME_SUB_SERVER )
					continue;		// verify the server type
		
				++nServerCount;

				// if this is this server, then use our real stats for the population level..
				if ( server.address == m_Context.address && server.port == m_Context.port )
				{
					server.maxClients = m_Context.maxPlayers;
					server.clients = m_nPlayers;
					server.shortDescription = pContext->name();		// replace our short description with our map name..
				}
				else
				{
					// for the front-facing server we add up all players from all servers since this is the possible
					// number of clients that might all try to go into one single server.
					m_Server.clients += server.clients;
				}

				sGameInfo += CharString().format( "\n   %s - %s", 
					server.shortDescription.cstr(), MetaClient::populationText( server.clients, server.maxClients ) );
			}

			// for our short description, show the number of servers running this MV..
			if ( nServerCount == 1 )
			{
				sGameInfo.release();
				m_Server.shortDescription = CharString().format( "Metaverse: %s", pContext->name() );
			}
			else
				m_Server.shortDescription = CharString().format( "Metaverse: %d Sectors", nServerCount );
		}
		else
		{
			// use the map name for our short description..
			m_Server.shortDescription = pContext->name();
		}
	}

	if ( sGameInfo.length() > 0 )
		m_Server.description = m_Server.shortDescription + "\n" + sGameInfo + "\n";

	// store information about the worldContext into the data field of the server, so the website can display 
	// in-game information

	CharString sbData;
	sbData = CharString().format("FC=%d", pContext->teamCount() );
	sbData += CharString().format(";KEY=%s", pContext->key().string().cstr() );
	sbData += CharString().format(";POS=%s", CharString( pContext->position() ).cstr() );

	for(int f=0;f<pContext->teamCount();f++)
	{
		const WorldContext::Team & team = pContext->teamN( f );
		sbData += CharString().format(";F%dID=%d", f, team.nTeamId );
		sbData += CharString().format(";F%dFID=%d", f, team.factionId );
		sbData += CharString().format(";F%dCID=%d", f, team.clanId );
		sbData += CharString().format(";F%dNAME=%s", f, team.name.cstr() );
	}

	int planets = 0;		// planets
	int stars = 0;			// stars
	int gates = 0;			// gates

	for(int z=0;z<pContext->zoneCount();z++)
	{
		NodeZone * pZone = pContext->zone( z );
		ASSERT( pZone );

		if ( pZone->childCount() == 0 )
			continue;		// skip empty zones

		for(int j=0;j<pZone->childCount();j++)
		{
			Noun * pNoun = WidgetCast<Noun>( pZone->child(j) );
			if ( pNoun == NULL )
				continue;

			if ( WidgetCast<NounPlanet>( pNoun ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pNoun;
				sbData += CharString().format(";P%dNAME=%s", planets, pPlanet->name() );
				sbData += CharString().format(";P%dKEY=%s", planets, pPlanet->key().string().cstr() );
				sbData += CharString().format(";P%dPOS=%s", planets, CharString( pPlanet->worldPosition() ).cstr() );
				sbData += CharString().format(";P%dFLAG=%d", planets, pPlanet->flags() );
				sbData += CharString().format(";P%dFLEET=%d", planets, pPlanet->teamId() );
				sbData += CharString().format(";P%dPOP=%d", planets, ((int)pPlanet->population()) );
				sbData += CharString().format(";P%dUNIT=%d", planets, pPlanet->unitCount() );
				sbData += CharString().format(";P%dSTRU=%d", planets, pPlanet->structureCount() );
				sbData += CharString().format(";P%dSYS=%s", planets, pPlanet->starSystem() );

				Noun * pOrbiting = pPlanet->orbiting();
				if ( pOrbiting != NULL )
					sbData += CharString().format(";P%dORBIT=%s", planets, pOrbiting->name() );

				// get the clanId for the planet
				dword clanId = 0;
				if ( pContext->isTeamValid( pPlanet->teamId() ) )
					clanId = pContext->team( pPlanet->teamId() ).clanId;
				
				sbData += CharString().format(";P%dCLAN=%d", planets, clanId );

				// increment the planet count
				planets++;
			}
			else if ( WidgetCast<NounStar>( pNoun ) )
			{
				NounStar * pStar = (NounStar *)pNoun;

				sbData += CharString().format(";S%dNAME=%s", stars, pStar->name() );
				sbData += CharString().format(";S%dKEY=%s", stars, pStar->key().string().cstr() );
				sbData += CharString().format(";S%dPOS=%s", stars, CharString( pStar->worldPosition() ).cstr() );
				sbData += CharString().format(";S%dFLEET=%d", stars, pStar->teamId() );

				Noun * pOrbiting = pStar->orbiting();
				if ( pOrbiting != NULL )
					sbData += CharString().format(";S%dORBIT=%s", stars, pOrbiting->name() );

				stars++;
			}
			else if ( WidgetCast<NounJumpGate>( pNoun ) )
			{
				NounJumpGate * pGate = (NounJumpGate *)pNoun;
				if ( pGate->duration() )
					continue;		// ignore inactive/temp gates (i.e. wormholes)

				sbData += CharString().format(";G%dNAME=%s", gates, pGate->name() );
				sbData += CharString().format(";G%dPOS=%s", gates, CharString( pGate->worldPosition() ).cstr() );
				sbData += CharString().format(";G%dFLEET=%d", stars, pGate->teamId() );

				Noun * pOrbiting = pGate->orbiting();
				if ( pOrbiting != NULL )
					sbData += CharString().format(";G%dORBIT=%s", gates, pOrbiting->name() );
				sbData += CharString().format(";G%dDEST=%s", gates, pGate->destination() );

				gates++;
			}
		}

	}

	sbData += CharString().format(";PLANETS=%d", planets );
	sbData += CharString().format(";STARS=%d", stars );
	sbData += CharString().format(";GATES=%d", gates );
	
	m_Server.data = sbData;

	TRACE( CharString().format("Registering server, server.data.length = %u", m_Server.data.length()) );

	// connect our NounJumpGate objects to their remote servers...
	for(int z=0;z<pContext->zoneCount();z++)
	{
		NodeZone * pZone = pContext->zone( z );
		ASSERT( pZone );

		for(int j=0;j<pZone->childCount();j++)
		{
			NounJumpGate * pGate = WidgetCast<NounJumpGate>( pZone->child(j) );
			if (! pGate )
				continue;
			if ( pGate->isLocalDestination() )
			{
				// if we were remote, but we now have a local destionation, clear the remote flag
				if ( pGate->isRemoteServer() )
					pGate->clearRemoteServer();
				continue;		// destination is local, so skip this gate...
			}
			if ( strlen( pGate->destination() ) == 0 )
				continue;		// no named destination, so this is a home gate..

			if ( pGate->isRemoteServer() )
			{
				// validate our current remote server...
				bool bServerFound = false;
				for(int k=0;k<servers.size() && !bServerFound;++k)
				{
					MetaClient::Server & server = servers[ k ];
					if ( server.type != MetaClient::GAME_SERVER && server.type != MetaClient::GAME_SUB_SERVER )
						continue;

					bServerFound = strcmp( server.address, pGate->address() ) == 0 
						&& server.port == pGate->port();
				}

				if (! bServerFound )
				{
					LOG_STATUS( "GameServer", CharString().format("Remote server %s:%d is down... shutting down gate %s!", 
						pGate->address(), pGate->port(), pGate->name()) );
					pGate->clearRemoteServer();
				}
			}
			else //if (! pGate->isRemoteServer() )
			{
				// look for a server with our gate..
				bool bServerFound = false;
				for(int k=0;k<servers.size() && !bServerFound;++k)
				{
					MetaClient::Server & server = servers[k];
					if ( server.type != MetaClient::GAME_SERVER && server.type != MetaClient::GAME_SUB_SERVER )
						continue;

					int nGates = GetKeyIntegerValue( server.data, ";GATES=" );
					for(int i=0;i<nGates && !bServerFound;++i)
					{
						CharString sGateKey;
						sGateKey.format( ";G%dNAME=", i );

						CharString sGateName;
						if (! GetKeyValue( server.data, sGateKey, sGateName ) )
							continue;
						if ( sGateName == pGate->destination() )
						{
							// found our remote gate, hook it up...
							LOG_STATUS( "GameServer", "Connecting gate %s to remote server at %s:%d!", 
								pGate->name(), server.address.cstr(), server.port );
							pGate->setRemoteServer( server.address, server.port );

							bServerFound = true;
						}
					}
				}
			}
		}
	}

	lock.release();

	// register our ProfilerServer
	MetaClient::Server profiler;

	m_ProfilerServer.lock();
	profiler.flags = 0;
	profiler.type = 7;
	profiler.gameId = m_Context.gameId;
	profiler.name = m_Context.name;
	profiler.port = m_ProfilerServer.port();
	profiler.address = m_Context.address;
	profiler.maxClients = 1;
	profiler.clients = m_ProfilerServer.clientCount();
	profiler.shortDescription = CharString().format( "Profiler - Port:%u", profiler.port );
	profiler.description = CharString().format( "Port:%u\nAddress:%s\nClients:%u/%u", 
		profiler.port, profiler.address.cstr(), profiler.clients, profiler.maxClients );
	m_ProfilerServer.unlock();

	if ( m_MetaClient.registerServer( profiler ) < 0 )
		LOG_ERROR( "GameServer", "ERROR: Failed to register profiler server!" );

	// register this server
	if ( m_MetaClient.registerServer( m_Server ) < 0 )
		LOG_ERROR( "GameServer", "ERROR: Failed to register server!" );


	return true;
}

bool GameServer::prepWorld()
{
	GameContext * pGameWorld = WidgetCast<GameContext>( worldContext() );
	if (! pGameWorld )
		return false;

	pGameWorld->setGameUser( this );

	if ( sm_bEnableTeamSpawners )
	{
		// create a spawner for each team that doesn't already have one in the map..
		for(int i=0;i<pGameWorld->teamCount();++i)
		{
			const WorldContext::Team & team = pGameWorld->teamN( i );
			if ( team.factionId == FACTION_UNKNOWN || team.factionId == FACTION_NEUTRAL )
			{
				// detach any invalid spawners from the world..
				Array< Noun::Ref > spawners;
				if ( pGameWorld->findNouns( spawners, CLASS_KEY(NounSpawnShip), -1, team.factionId, true ) )
				{
					for(int j=0;j<spawners.size();++j)
						spawners[j]->detachSelf();
				}
				continue;	
			}
			if (! team.pTemplates.valid() )
				continue;		// no valid collection for this team

			Array< Noun::Ref > spawners;
			if ( pGameWorld->findNouns( spawners, CLASS_KEY(NounSpawnShip), -1, team.factionId, true ) )
			{
				// detach any extra spawners..
				for(int j=1;j<spawners.size();++j)
					spawners[j]->detachSelf();
				continue;		// faction already has a spawner
			}

			Array< Noun::Ref > jumpgates;
			if (! pGameWorld->findNouns( jumpgates, CLASS_KEY(NounJumpGate), -1, team.factionId ) )
				continue;		// MUST have a jump gate owned by our faction!

			// load all the ships up..
			team.pTemplates->load();
			// no spawners found for this teams faction, add one..
			NounSpawnShip::makeTeamSpawner( pGameWorld, team.nTeamId, jumpgates[ rand() % jumpgates.size() ], true );
		}
	}

	return WorldServer::prepWorld();
}

bool GameServer::cleanWorld()
{
	if (! m_pWorldContext.valid() )
		return false;

	Queue< Noun::Ref > remove;
	for(int i=0;i<m_pWorldContext->zoneCount();i++)
	{
		NodeZone * pZone = m_pWorldContext->zone( i );
		for(int j=0;j<pZone->childCount();j++)
		{
			Noun * pNoun = ((Noun *)pZone->child(j));
			if ( WidgetCast<NounShip>( pNoun ) )
			{
				if ( ((NounShip *)pNoun)->userId() != 0 )
					remove.push( pNoun );
			}
			else if ( pNoun->nounType() == TYPE_OBJECT 
				|| pNoun->nounType() == TYPE_EFFECT )
			{
				// local objects and effects are removed such as projectiles and fighters
				remove.push( pNoun );
			}
		}
	}

	// detach the objects
	while( remove.valid() )
	{
		(*remove)->detachSelf();
		remove.next();
	}

	return WorldServer::cleanWorld();
}

bool GameServer::canJoinTeam( ClientContext & client, int nTeamId )
{
	if ( m_Context.bBalanced )
	{
		int nFactionId = gameContext()->teamFaction( nTeamId );
		if ( getFactionScore( nFactionId ) >= getTopFactionScore() )
			return false;		// faction is already the top faction..
	}

	return WorldServer::canJoinTeam( client, nTeamId );
}

void GameServer::onJoinTeam( ClientContext & client, int nTeamId )
{
	updateFactionBalanceMap();

	WorldServer::onJoinTeam( client, nTeamId );
}

bool GameServer::canSpawn( ClientContext & context, Noun * pSpawnPoint, Noun * pSpawn, 
						  Array< Noun::Ref > & Storage )
{
	if (! WorldServer::canSpawn( context, pSpawnPoint, pSpawn, Storage ) )
		return false;
	NounShip * pSpawnShip = WidgetCast<NounShip>( pSpawn );
	if (! pSpawnShip )
		return false;			// we can only spawn ships currently!
	UserStorage * pUserStorage = context.pStorage;
	if (! pUserStorage )
		return false;

	int nTransferCost = 0;
	int nMaxShips = (context.MetaProfile.flags & MetaClient::SUBSCRIBED) != 0 ? MAX_SUB_SHIPS_IN_STORAGE : MAX_SHIPS_IN_STORAGE;
	int nQueuedShips = 0;
	ShipQueueMap::iterator iBuild = m_ShipQueueMap.find( context.nUserId );
	if ( iBuild != m_ShipQueueMap.end() )
		nQueuedShips = iBuild->second;
	int nShipCount = nQueuedShips;

	for(int i=0;i<pUserStorage->storageCount();++i)
	{
		const UserStorage::Storage & storage = pUserStorage->storage( i );
		dword nShipFactionId = storage.nFactionId;
		int nMyFactionId = context.nFactionId;
		
		// get the NounTemplate object for this ship as we don't want to include invalid context'd ships
		Widget::Ref pUncasted = storage.instance.unwrap();
		NounShip::Ref pShip = WidgetCast<NounShip>( pUncasted );
		NounTemplate * pNounTemplate = worldContext()->findNounTemplate( pShip );
		
		if ( storage.nType == CARGO_TYPE_SHIP && nMyFactionId == nShipFactionId && pNounTemplate )
			++nShipCount;
	}

	// use resources if the players ship is not coming from storage
	bool bNewSpawn = false;
	if ( Storage.search( pSpawn ) < 0 )
	{
		bNewSpawn = true;

		if ( nShipCount >= nMaxShips )
		{
			sendChat( context.nClientId, CharString().format( "SERVER: Maximum number of ships docked (%d of %d, %d in build queue)!", 
				nShipCount, nMaxShips, nQueuedShips ) );
			return false;		// no more ships in storage!
		}
	}
	else
	{
		if ( nShipCount > nMaxShips )
		{
			sendChat( context.nClientId, CharString().format( "SERVER: Maximum number of ships docked (%d of %d, %d in build queue)!", 
				nShipCount, nMaxShips, nQueuedShips ) );
			return false;
		}

		UserStorage::Storage * pStorage = pUserStorage->findStorage( pSpawn->key() );
		if ( pStorage != NULL && pStorage->pLocation.valid() && pStorage->pLocation != pSpawnPoint )
			nTransferCost = TRANSFER_SPAWN_COST * NounShip::buildCost( pSpawnShip );
	}

	// get the NounTemplate object for this ship
	NounTemplate * pNounTemplate = worldContext()->findNounTemplate( pSpawn );
	if (! pNounTemplate )
		return false;

	// get the player team information, if player doesn't have a valid team.. then try to select one based upon
	// the faction of the player's selected ship...
	if (! worldContext()->isTeamValid( context.nTeamId ) )
		return false;
	const WorldContext::Team & team = worldContext()->team( context.nTeamId );

	// count cargo items requested...a previous ship may be in this array as well, so ignore it...
	int nCargo = 0;
	for(int i=0;i<Storage.size();++i)
	{
		NounGame * pNounGame = WidgetCast<NounGame>( Storage[ i ] );
		if ( pNounGame && pNounGame->isCargo() )
			++nCargo;
	}

	if ( nCargo > pSpawnShip->availableCargo() )
	{
		sendChat( context.nClientId, "<color;ffffff>SERVER: Not enough cargo space!" );
		return false;
	}

	// skip all remaining checks if the player is an admin or dev..
	if ((context.MetaProfile.flags & sm_nAlwaysSpawnFlags) != 0 )
		return true;
	
	if ( m_Context.bUseShipFlags && ( m_Context.sShipFlags.find( NounShip::typeText( pSpawnShip->type() ), 0, true ) > -1 ) )
		return true;

	// check the player rank and badges
	GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
	if (! pProfile )
		return false;		// wrong profile type..

	int rank =  pProfile->calculateRank();
	if (rank < 0 )
		rank = 0;
			
	bool hasRank = Min<int>( rank, m_Context.maxRank ) >= pNounTemplate->rank();
	if ( !hasRank )
	{
		sendChat( context.nClientId, "<color;ffffff>SERVER: Insufficient rank for ship!" );
		return false;
	}

	bool hasBadges = ( pProfile->calculateBadges() & pNounTemplate->flags()) == pNounTemplate->flags();
	if ( !hasBadges )
	{
		sendChat( context.nClientId, "<color;ffffff>SERVER: Insufficient badges for ship!" );
		return false;
	}

	// check the spawn point
	if ( WidgetCast<NounJumpGate>( pSpawnPoint ) )
	{
		NounJumpGate * pGate = (NounJumpGate *)pSpawnPoint;
		if ( pGate->factionId() != team.factionId 
			&& pGate->factionId() != FACTION_NEUTRAL 
			&& pGate->factionId() != FACTION_UNKNOWN )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Jumpgate has been captured!" );
			return false;		// invalid gate for this player
		}
		if ( ((pGate->factionId() == FACTION_NEUTRAL || pGate->factionId() == FACTION_UNKNOWN)
			&& pGate->destinationNoun() != NULL) || !pGate->active() )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Jumpgate is not valid for spawning!" );
			return false;		// invalid gate for this player
		}
		if ( bNewSpawn && !pSpawnShip->enableGateSpawn() )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Ship must be built at shipyard!" );
			return false;		
		}
	}
	else if ( WidgetCast<NounPlanet>( pSpawnPoint ) )
	{
		NounPlanet * pPlanet = (NounPlanet *)pSpawnPoint;
		if ( pPlanet->factionId() != team.factionId )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Planet has been captured!" );
			return false;
		}
		if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD) == 0 )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Planet shipyard has been destroyed!" );
			return false;
		}
		if ( (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Planet is under blockade!" );
			return false;
		}

		if ( bNewSpawn && pPlanet->technology() < NounShip::buildTechnology( pSpawnShip ) )
		{
			sendChat( context.nClientId, "<color;ffffff>SERVER: Planet technology too low to build this ship!" );
			return false;
		}

		if ( nTransferCost > 0 && pPlanet->resources() < nTransferCost )
		{
			sendChat( context.nClientId, CharString().format("<color;ffffff>SERVER: %s Resources needed to transfer ship to '%s'; %s available.",
				FormatNumber<char,int>( nTransferCost ).cstr(), pPlanet->name(), FormatNumber<char,int>( pPlanet->resources() ).cstr() ) );
			return false;
		}
	}
	else
	{
		return false;		// unknown spawn point type
	}

	return true;
}

bool GameServer::onSpawn( ClientContext & context, Noun * pSpawnPoint, Noun * pSpawn, bool bNewSpawn, Array< Noun::Ref > & Storage )
{
	if (! context.pStorage )
	{
		LOG_ERROR( "GameServer", "ERROR: Client has NULL storage!" );
		return false;
	}
	NounShip * pSpawnShip = WidgetCast<NounShip>( pSpawn );
	if (! pSpawnShip )
		return false;			// we can only spawn ships currently!

	int nTransferCost = 0;
	if (! bNewSpawn )
	{
		UserStorage::Storage * pStorage = findStorage( context.nUserId, pSpawnShip->key() );
		if ( pStorage != NULL && pStorage->pLocation.valid() && pStorage->pLocation != pSpawnPoint )
			nTransferCost = TRANSFER_SPAWN_COST * NounShip::buildCost( pSpawnShip );
	}

	int nBuildCost = bNewSpawn ? NounShip::buildCost( pSpawnShip ) : 0;
	if ((context.MetaProfile.flags & sm_nAlwaysSpawnFlags) != 0 )
		nBuildCost = 0;		// no costs for admins or developers

	// always make a copy the object we are spawning, as to avoid key conflicts with older versions of our self
	pSpawnShip = WidgetCast<NounShip>( pSpawnShip->copy() );
	if (! pSpawnShip )
		return false;		// failed to make a copy, never happens...

	pSpawnShip->setName( context.MetaProfile.name );
	pSpawnShip->setTeamId( context.nTeamId );
	pSpawnShip->setUserId( context.nUserId );

	// team & user need to be set before we call setup()...
	if ( bNewSpawn )
		pSpawnShip->setup();

	// set the tick of the ship to be the same as our spawn point
	NodeZone::setTick( pSpawnShip, pSpawnPoint->tick() );

	pSpawnShip->setPosition( pSpawnPoint->position() );
	pSpawnShip->setOrientation( pSpawnPoint->orientation() );

	NounPlanet * pPlanet = WidgetCast<NounPlanet>( pSpawnPoint );
	if ( pPlanet != NULL )
	{
		if ( pPlanet->resources() < nBuildCost )
		{
			StructureShipyard * pShipYard = StructureShipyard::findYard( pPlanet );
			if (! pShipYard )
				return false;

			// not enough resources currently available, insert our ship into the queue ..
			if (! pShipYard->addBuild( context.nUserId, pSpawnShip, nBuildCost ) )
				return false;

			sendChat( context.nClientId, CharString().format( "<color;ffffff>SERVER: Ship added to build queue on '%s' - %u ships in queue!", 
				pPlanet->name(), pShipYard->buildCount() ) );
			m_ShipQueueMap[ context.nUserId ] += 1;

			return false;
		}
	}

	// check the player rank and badges
	GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
	if (! pProfile )
		return false;		// wrong profile type..

	NodeZone * pZone = pSpawnPoint->zone();
	if (! pZone )
	{
		LOG_ERROR( "GameServer", "ERROR: Spawn point has NULL zone!" );
		return false;
	}

	// get the NounTemplate object for this ship
	NounTemplate * pNounTemplate = worldContext()->findNounTemplate( pSpawnShip );
	if (! pNounTemplate )
	{
		LOG_ERROR( "GameServer", "ERROR: Failed to find NounTemplate for spawning ship!" );
		return false;
	}
	
	// audit the ship against the template, add or remove gadgets as needed...
	NounShip * pShipTemplate = WidgetCast<NounShip>( pNounTemplate->noun() );
	if (! pShipTemplate )
	{
		LOG_ERROR( "GameServer", "ERROR: NounTemplate noun is not a NounShip!" );
		return false;
	}

	if (! auditShip( context.nFactionId, pSpawnShip, pShipTemplate ) )
	{
		LOG_ERROR( "GameServer", "ERROR: auditShip() failed!" );
		return false;
	}

	// make sure all infantry on board are set to the correct team ID, we were seeing a bug 
	// when ships used the transfer gates that the infantry on board were switing sides.
	for(int i=0;i<pSpawnShip->childCount();++i)
	{
		BaseNode * pChild = pSpawnShip->child(i);
		if ( WidgetCast<NounUnit>( pChild ) )
			((NounUnit *)pChild)->setTeamId( pSpawnShip->teamId() );
	}

	// Make sure the ship isn't jumping or anything...
	if ( pSpawnShip->jumpDrive() )
		pSpawnShip->jumpDrive()->clearJump();
	
	// if ship is destroyed, put the ship back to 5% hull...
	if ( pSpawnShip->isDestroyed()  )
		pSpawnShip->setDamage( pSpawnShip->maxDamage() * 0.95f );			// keep ship 95% damaged

	// repair damage based on time left in garage...
	if ( !bNewSpawn && pSpawnShip->damage() > 0 && Storage.size() > 0 )
	{
		// the first item in the array is always the ship, this code rely's on that fact since 
		// we need the original ID of the ship to get the storage time..
		UserStorage::Storage * pStorage = findStorage( context.nUserId, Storage[0]->key() );
		if ( pStorage != NULL && pStorage->nTime != 0 )
		{
			dword nStorageTime = Time::seconds() - pStorage->nTime;
			float fPercent = Clamp<float>( STORAGE_REPAIR_PER_SECOND * nStorageTime, 0.0f, 1.0f );
			pSpawnShip->setDamage( pSpawnShip->damage() - (pSpawnShip->damage() * fPercent) );
		}
		else if ( pStorage == NULL )
			LOG_ERROR( "GameServer", "ERROR: Failed to find ship in storage to get the storage time!" );
	}

	// if we're spawning from a planet, clear the command and order
	if ( pPlanet != NULL )
	{
		pSpawnShip->setOrder( NounShip::NOORDER, NULL, NULL );
		pSpawnShip->setCommand( NounShip::NOCOMMAND, NULL );
	}
	pSpawnShip->setHome( pSpawnPoint );
	pSpawnShip->setRank( pProfile->calculateRank() ); // keep ship real rank
	pSpawnShip->setCredits( (*pProfile)[ CREDITS ] );
	pSpawnShip->clearFlags( NounShip::FLAG_ALL );
	
	if (! pProfile->isSubscriber() )
	{
		pSpawnShip->clearFlags( NounShip::FLAG_SUBSCRIBER );
		pSpawnShip->detachTrait<TraitDisplayName>();		// remove any custom names from the ship
	}
	else
	{
		pSpawnShip->addFlags( NounShip::FLAG_SUBSCRIBER );
	}

	if ( pProfile->isAdmin() )
		pSpawnShip->addFlags( NounShip::FLAG_ADMIN );
	else
		pSpawnShip->clearFlags( NounShip::FLAG_ADMIN );

	int nSpawnAttempts = 0;
	if (! pickSafeSpawnPosition( pSpawnPoint, pSpawnShip ) )
	{
		sendChat( context.nClientId, "SERVER: Failed to find open spawn location!" );
		return false;
	}

	// put ship into orbit if spawning from yard, break orbit if spawning from jump gate
	TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pSpawnShip );
	if ( pOrbit != NULL )
	{
		if ( WidgetCast<NounPlanet>( pSpawnPoint ) )
		{
			Vector3 vDelta( pSpawnShip->position() - pSpawnPoint->position() );
			float fPosition = atan2( vDelta.x, vDelta.z );
			float fDistance = vDelta.magnitude();

			pSpawnShip->setVelocity( 0.0f );
			pOrbit->setOrbit( pSpawnPoint, fDistance, fPosition, 0.0f, pSpawnPoint->tick() );
		}
		else
		{
			pSpawnShip->setVelocity( 5.0f );
			pOrbit->breakOrbit();
		}
	}

	// Spawn any ally ships...
	for( int i=0;i<context.pStorage->storageCount();++i)
	{
		const UserStorage::Storage & storage = context.pStorage->storage( i );
		if ( storage.nType == CARGO_TYPE_ALLY && storage.nFactionId == context.nFactionId )
		{
			Widget::Ref pUncasted = storage.instance.unwrap();
			if (! pUncasted )
			{
				LOG_ERROR( "GameServer", "ERROR: Failed to unwrap ally instance!" );
				continue;
			}

			NounShip::Ref pAlly = WidgetCast<NounShip>( pUncasted->copy() );
			if ( pAlly == NULL )
			{
				LOG_ERROR( "GameServer", "ERROR: Failed to copy ally ship!" );
				continue;
			}
			if (! pickSafeSpawnPosition( pSpawnPoint, pAlly ) )
			{
				sendChat( context.nClientId, "SERVER: Failed to find safe spawn location for ally ship!" );
				continue;
			}

			NodeZone::setTick( pAlly, pSpawnPoint->tick() );

			pAlly->setTeamId( context.nTeamId );
			pAlly->setRank( 0 );							// user might get demoted, so no rank..
			pAlly->clearFlags( NounShip::FLAG_ALL );
			pAlly->addFlags( NounShip::FLAG_AI_ALLY );		// flag this ship as an AI ally
			pAlly->setHome( pSpawnShip );					// our home is the player that spawns us..
			pAlly->setOrder( NounShip::DEFEND, pSpawnShip, pSpawnShip );

			pSpawnPoint->zone()->attachNoun( pAlly );
			pSpawnShip->addAlly( pAlly );

			// Add ally to list of storage items so it will get deleted
			Storage.push( WidgetCast<Noun>( pUncasted ) );
		}
	}

	// Attach all storage items to the spawning object and delete storage items..
	for(int i=0;i<Storage.size();i++)
	{
		NounGame * pNoun = WidgetCast<NounGame>( Storage[i] );
		if ( !pNoun )  
			continue;

		// attach any cargo items as children... note that the spawning ship may be in this
		// list as well, so we must take care..
		if ( pNoun->isCargo() )
			pSpawnShip->attachNode( (Noun *)pNoun->copy() );

		// delete all objects used from storage..
		deleteStorage( 0, context.nUserId, pNoun->key() );
	}


	// call base onSpawn(), which will actually attach the spawned object to the world..
	if (! WorldServer::onSpawn( context, pSpawnPoint, pSpawnShip, bNewSpawn, Storage ) )
		return false;

	// send out spawn message, makes jumpgates flash and resources get used... this has to be sent
	// after the noun is attached, otherwise the verb will not execute correctly.
	Verb::Ref( new VerbSpawn( pSpawnPoint, pSpawnShip, nBuildCost + nTransferCost ) );
	
	// save the users storage after spawning, so they don't lose a ship..
	saveStorage( context.nUserId );
	
	return true;
}

void GameServer::onDeleteClient( ClientContext & client )
{
	if ( client.nLogoffTime == 0 )
		client.nLogoffTime = Time::seconds();

	// update the time played
	addPlayerProfile( client.nUserId, "Time Played", client.nLogoffTime - client.nLoginTime );
	
	// send final score for player...
	GameProfile * pProfile = WidgetCast<GameProfile>( client.pProfile );
	if ( pProfile )
	{
		float fPrestige = pProfile->calculatePrestige();
		if (! m_MetaClient.updateScore( client.nUserId, fPrestige, GameProfile::rankText( pProfile->getRank( fPrestige ) ) ) )
			LOG_ERROR( "GameServer", CharString().format( "Failed to update score for user %u", client.nUserId ) );
	}

	// erase any build queue count..
	m_ShipQueueMap.erase( client.nUserId );
	
	// update our faction balance map
	updateFactionBalanceMap();

	// call the base class so it can update the player queue..
	WorldServer::onDeleteClient( client );
}

bool GameServer::onTransferClient( ClientContext & client, const WorldClient::TransferRequest & a_Req )
{
	NounShip::Ref pSelf = WidgetCast<NounShip>( client.pSelf );
	if ( pSelf.valid() )
	{
		if ( a_Req.m_nSelf != pSelf->key() )
		{
			LOG_ERROR( "GameServer", "ERROR: Transfer self keys are different!" );
			return false;
		}

		storeAllyShips( pSelf );

		Storage storage;
		storage.nType = CARGO_TYPE_SHIP;
		storage.nFactionId = pSelf->factionId();
		storage.nTime = Time::seconds();
		storage.pLocation = pSelf->home(); 
		storage.sServer = a_Req.m_sAddress;
		storage.nPort = a_Req.m_nPort;
		storage.instance.wrap( client.pSelf, 0 );

		addStorage( 0, client.nUserId, a_Req.m_nSelf, storage );
	}
	
	return true;
}

bool GameServer::onStorageLoaded( ClientContext & context, UserStorage * a_pStorage )
{
	bool bSave = false;

	Array< WidgetKey > removeStorage;

	// revalidates the object types stored...
	for( int i=0;i<a_pStorage->storageCount();++i)
	{
		Storage & storage = a_pStorage->getStorage( i );

		Widget::Ref pUncasted = storage.instance.unwrap();
		if ( WidgetCast<NounCargo>( pUncasted ) != NULL )
		{
			if ( storage.nType != CARGO_TYPE_ITEM )
			{
				LOG_ERROR( "GameServer", CharString().format("ERROR: Found NounCargo storage with wrong type of %u!", storage.nType) );
				storage.nType = CARGO_TYPE_ITEM;
				bSave = true;
			}
		}
		else if ( WidgetCast<NounShip>( pUncasted ) != NULL )
		{
			if ( storage.nType != CARGO_TYPE_SHIP 
				&& storage.nType != CARGO_TYPE_ALLY )
			{
				LOG_ERROR( "GameServer", CharString().format("ERROR: Found NounShip storage with wrong type of %u!", storage.nType) );
				storage.nType = CARGO_TYPE_SHIP;
				bSave = true;
			}

			GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
			if (! pProfile )
			{
				LOG_ERROR( "GameServer", "Invalid GameProfile attached in ClientContext." );
				return false;
			}

			// we don't want to modify admin or developer ships
			if ( !pProfile->isDeveloper() && !pProfile->isAdmin() ) 
			{
				// check the player rank and badges, if they can't use this ship anymore, then remove any enhancements..
				NounShip * pShip = (NounShip *)pUncasted.pointer();
				if ( pShip->enhancementCount() > 0 )
				{
					// the NounTemplate may be NULL if the ship was removed, in those case we definitinely want to remove any enhancements.
					NounTemplate * pNounTemplate = worldContext()->findNounTemplate( pShip );

					int rank = pProfile->calculateRank();
					dword badges = pProfile->calculateBadges();

					if ( pNounTemplate == NULL || rank < pNounTemplate->rank() || (badges & pNounTemplate->flags()) != pNounTemplate->flags() )
					{
						LOG_STATUS( "GameServer", "Removing %d enhancements from ship in storage.", pShip->enhancementCount() );

						Array< NounEnhancement::Ref > remove;
						for( int j=0;j<pShip->enhancementCount();++j)
						{
							NounEnhancement * pEnhancement = pShip->enhancement(j);
							remove.push( pEnhancement );

							LOG_STATUS( "GameServer", "Moving enhancement %s to storage.", pEnhancement->getName() );

							CargoEnhancement::Ref pCargo = new CargoEnhancement( pEnhancement->factory()->classKey() );
							pCargo->setDamage( pEnhancement->damage() );	

							Storage storage;
							storage.nType = CARGO_TYPE_ITEM;
							storage.nFactionId = context.nFactionId;
							storage.nTime = Time::seconds();
							storage.instance.wrap( pCargo, 0 );

							a_pStorage->add( pCargo->key(), storage );
						}

						// detach the enhancements from the ship...
						for(int j=0;j<remove.size();++j)
							remove[j]->detachSelf();

						// if the ship has an invalid template, add it to a list to be removed from storags as well..
						if ( pNounTemplate == NULL )
							removeStorage.push( pShip->key() );
						else
							storage.instance.wrap( pShip, 0 );		// re-save the ship instance without the enhancements..

						bSave = true;

					}
				}
			}
		}
		else
		{
			LOG_ERROR( "GameServer", "ERROR: Unknown storage class type!" );
			storage.nType = 0xffffffff;
			bSave = true;

			// add item to be removed
			removeStorage.push( a_pStorage->storageKey(i) );
		}
	}

	if ( removeStorage.size() > 0 )
	{
		for( int i=0;i<removeStorage.size();++i)
		{
			LOG_STATUS( "GameServer", "Removing invalid storage %llu.", removeStorage[i].m_Id );
			a_pStorage->remove( removeStorage[i] );
		}
		bSave = true;
	}

	// save changes back to disk if any were made..
	if ( bSave )
		a_pStorage->save( false );

	return true;
}

void GameServer::onMessage( ClientContext & context, bool bUDP, byte nMessage, const InStream & input )
{
	switch( nMessage )
	{
	case GameClient::STORE_BUY_ENHANCEMENT:
		{
			ClassKey ck;
			input >> ck;

			buyEnhancement( context, ck );
		}
		break;
	default:
		WorldServer::onMessage( context, bUDP, nMessage, input );
		break;
	}
}

void GameServer::onLogin( ClientContext & context )
{
	WorldServer::onLogin( context );

	// we need to send the players initial number of credits down on login.
	int nCredits = 0;
	GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
	if ( pProfile )
		nCredits = (*pProfile)[ CREDITS ];
	send( context.nClientId, GameClient::STORE_UPDATE_CREDITS ) << nCredits;
}

int GameServer::getNounType( Noun * pNoun )
{
	NounShip * pShip = WidgetCast<NounShip>( pNoun );
	if ( pShip != NULL )
		return pShip->type();

	return 0;
}

//---------------------------------------------------------------------------------------------------

void GameServer::onAttachNoun( Noun * pNoun )
{
	// remove ship and cargo from storage
	if ( WidgetCast<NounShip>( pNoun ) )
	{
		NounShip * pSelf = (NounShip *)pNoun;
		ASSERT( pSelf );

		if ( pSelf->userId() != 0 )
			saveStorage( pSelf );
	}

	WorldServer::onAttachNoun( pNoun );
}

void GameServer::onDetachNoun( Noun * pNoun )
{
	// must get the faction ID before let it get detached from the world...
	dword nFactionId = pNoun->factionId();
	// grab a reference to the object so it doesn't delete while in this function..
	Noun::Ref pNounRef = pNoun;

	// NOTE: We call the base class FIRST so the message to detach the noun will be sent before we send 
	// the player their storage object (which has the same ID) that will cause problems with
	// detaching the ship from the GameContext
	WorldServer::onDetachNoun( pNoun );

	// add items to player storage
	if ( WidgetCast<NounShip>( pNoun ) != NULL 
		&& WidgetCast<ProxyShip>( pNoun ) == NULL )
	{
		NounShip * pSelf = (NounShip *)pNoun;
		ASSERT( pSelf );

		if ( pSelf->userId() != 0 )
		{
			dword nUserId = pSelf->userId();

			dword nClientId = getClientId( nUserId );
			ClientContext & client = m_ClientContext[ nClientId ];
			if (! client.bServerTransfer )
			{
				// clear the players ship, so they can select a new ship
				setClientSelf( 0, nClientId, NULL_WIDGET );

#if ENABLE_PRESTIGE_LOSS_ON_DEATH
				// if ship is destroyed, add resources lost..
				if ( pSelf->isDestroyed()  )
				{
					// ship is destroyed, add resources lost
					float fScale = (pSelf->rank() * RESOURCES_LOST_SCALE_PER_RANK) * getFactionBalance( client.nFactionId );
					float fResourcesLost = fScale * NounShip::buildCost( pSelf );

					addProfile( nUserId, RESOURCES_LOST, fResourcesLost );
				}
#endif

				bool bStorageFull = false;

				int nCargoCount = getStorageCargoCount( nUserId );
				for(int i=pSelf->childCount()-1;i>=0;i--)
				{
					BaseNode * pChild = pSelf->child(i);

					if ( WidgetCast<NounCargo>( pChild ) )
					{
						NounCargo * pCargo = (NounCargo *)pChild;
						if ( nCargoCount < MAX_CARGO_IN_STORAGE )
						{
							Storage storage;
							storage.nType = CARGO_TYPE_ITEM;
							storage.nFactionId = nFactionId;
							storage.nTime = Time::seconds();
							storage.instance.wrap( pCargo, 0 );
							// add cargo item to storage
							addStorage( 0, nUserId, pCargo->key(), storage );
							++nCargoCount;

							// detach the cargo from the ship
							pCargo->setSyncronized( false );
							pCargo->detachSelf();
						}
						else
							bStorageFull = true;
					}
					else if ( WidgetCast<NounUnit>( pChild ) )
					{
						NounUnit * pUnit = (NounUnit *)pChild;
						if ( !pSelf->isFriend( pUnit ) )
						{
							// detach the enemy unit from the ship
							pUnit->setSyncronized( false );		// don't send any detach message to the clients
							pUnit->detachSelf();				// immediate detach, not queued..
						}
					}
				}

				if ( bStorageFull )
					sendUserChat( nUserId, "<color;ffffff>Cargo: Storage is full, cargo remains on ship!" );

				// store any ally ships, all ally ships are spawned when the player respawns...
				storeAllyShips( pSelf );

				// lastly, add the ship to storage
				Storage storage;
				storage.nType = CARGO_TYPE_SHIP;
				storage.nFactionId = nFactionId;
				storage.nTime = Time::seconds();
				storage.pLocation = pSelf->home();
				storage.sServer = m_Context.address;
				storage.nPort = m_Context.port;
				storage.instance.wrap( pSelf, 0 );

				addStorage( 0, nUserId, pSelf->key(), storage );

				// release the pointer to the players ship, otherwise we could fail to transfer
				// to another server or duplicate the ship..
				client.pSelf = NULL;
			}
		}
	}
}

void GameServer::onCondition( int nConditionId )
{
	WorldContext * pContext = worldContext();
	if (! pContext )
		return;
	if (! pContext->isConditionValid( nConditionId ) )
		return;

	// get the condition
	const WorldContext::Condition & condition = pContext->condition( nConditionId );

	// generate the end message for all clients
	CharString sResult;
	sResult += condition.message;
	sResult += "\n\n";

	// get the prestige for all players
	Hash< dword, float > playerPrestige;
	for(int i=0;i<clientCount();i++)
	{
		ClientContext & context = m_ClientContext[ client(i) ];
		GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
		if (! pProfile )
			continue;		// client not logged in...

		playerPrestige[ i ] = pProfile->calculatePrestige();
	}

	// sort the players by rank
	Array< int > ranking;
	for(int i=0;i<clientCount();i++)
	{
		// get the players prestige level
		float prestige = playerPrestige[ i ];

		bool inserted = false;
		for(int j=0;j<ranking.size();j++)
			if ( prestige > playerPrestige[ ranking[j] ] )
			{
				ranking.insert( j, i );
				inserted = true;
				break;
			}

		if ( !inserted )
			ranking.push( i );		// add client to the end of the list
	}

	sResult += "<color;ffffffff><X;300>Prestige<X;400>Bonus\n";
	for(int i=0;i<ranking.size();i++)
	{
		int nClient = ranking[i];
		ClientContext & context = m_ClientContext[ client( nClient ) ];
		if (! pContext->isTeamValid( context.nTeamId ) )
			continue;

		GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
		if (! pProfile )
			continue;
		float fPrestige = pProfile->calculatePrestige();
		int nRank = pProfile->calculateRank();
		if(nRank < 0)
			nRank = 0;

		float fBonus = 0;
		for(int j=0;j<condition.objectives.size();j++)
			if ( pContext->checkObjective( condition.objectives[j] ) && condition.objectives[j].factionId == context.nFactionId )
				fBonus += 5.0f;

		// add bonus prestige to player
		addProfile( pProfile->userId(), BONUS_PRESTIGE, fBonus );

		// add player to result text
		sResult += CharString().format("%s %s<X;300>%.1f<X;400>+%.1f\n", 
			GameProfile::rankText( nRank ), pProfile->name(), fPrestige, fBonus );
	}

	// flush out all loaded Widgets, so we will load our map fresh..
	Broker::flushCache();
	// get the pointer to the next context before we call contextEnd() which will release our current context.
	WorldContext::Link pNextContext( condition.next );
	// end the old context
	contextEnd( 0, sResult );

	// if this server is not persistant, then remove any ships between maps..
	if (! m_Context.bPersistant )
	{
		// delete all ships from storage
		for( UserStorageHash::iterator iStorage = m_UserStorage.begin(); iStorage != m_UserStorage.end(); ++iStorage )
		{
			UserStorage * pStorage = iStorage->second;

			// create a brand new storage object for the client.
			std::list< WidgetKey > remove;
			for(int i=0;i<pStorage->storageCount();++i)
			{
				const UserStorage::Storage & item = pStorage->storage( i );
				if ( item.nType == CARGO_TYPE_SHIP )	
					remove.push_back( pStorage->storageKey( i ) );	
			}

			// remove all ships from storage..
			while( remove.size() > 0 )
			{
				deleteStorage( 0, pStorage->userId(), remove.front() );
				remove.pop_front();
			}
		}
	}

	// update our config file with the next context
	if ( pNextContext.valid() && m_Context.sConfigFile.length() > 0 )
	{
		Settings settings( "GameServer", m_Context.sConfigFile );
		settings.put( "context", pNextContext.key().hexString() );
	}

	// attempt to load the next WorldContext object
	if (! pNextContext.valid() || !contextInitialize( 0, WidgetWrap( pNextContext, FF_TRANSMIT, m_pDictionary ) ) )
		m_bStopServer = true;	// no valid context, stop the server!
}

bool GameServer::saveStorage( Noun * a_pSelf )
{
	if ( WidgetCast<NounShip>( a_pSelf ) != NULL && a_pSelf->userId() != 0 )
		return saveStorage( a_pSelf->userId() );

	return false;
}

int GameServer::spawnedScore(int a_nFactionId )
{
	int nSpawnedScore = 0;

	Array< Noun::Ref > ships;
	gameContext()->findNouns( ships, CLASS_KEY(NounShip), -1, a_nFactionId );

	for(int i=0;i<ships.size();++i)
	{
		NounShip * pShip = (NounShip *)ships[i].pointer();
		if ( pShip->rank() <= LAST_RANK )
			nSpawnedScore += pShip->rank() + 1;		// CADET is -1, so let that be zero
	}

	return nSpawnedScore;
}


//---------------------------------------------------------------------------------------------------

void GameServer::onKill( Noun * pWho, Noun *pDead )
{
	addProfile( pWho, KILLS, 1.0f );
}

void GameServer::onDestroyed( NounShip * pWho, Noun * pKiller )
{
	addProfile( pWho, KILLED, 1.0f );

	if ( pWho != NULL && pKiller != NULL )
	{
		LOG_STATUS( "GameServer", CharString().format( "[STAT] %s,(@%u),%s,DESTROYED,%s,(@%u),%s",
			pWho->factory()->className(),
			pWho->userId(),
			factionText( pWho->factionId() ),
			pKiller->factory()->className(),
			pKiller->userId(),
			factionText( pKiller->factionId() )) );
	}
	else if ( pWho != NULL )
	{
		LOG_STATUS( "GameServer", CharString().format( "[STAT] %s,(@%u),%s,DESTROYED,SELF",
			pWho->factory()->className(),
			pWho->userId(),
			factionText( pWho->factionId() ) ) );
	}
}

void GameServer::onDamageShip( Noun * pWho, float damage, Noun *pTarget )
{
	addProfile( pWho, SHIPS_DAMAGED, damage );
}

void GameServer::onCaptureShip( Noun * pWho, NounShip * pCaptured )
{
	float fScale = 1.0f;

	// prevent users from losing a ship with enhancements due to getting capped by saving 
	// their ship back into their storage now, before we go zero out their ship.
	dword nUserId = pCaptured->userId();
	if ( nUserId != 0 )
	{
		// remove the players active ship from their storage..
		deleteStorage( 0, nUserId, pCaptured->key() );

		// make a copy of the ship to save into storage, this gives us a new unique ID, if we don't 
		// do this then the client will receive their storage and get confused about the ship that is 
		// still in the world versus the one in their garage.
		NounShip * pSaveShip = (NounShip *)pCaptured->copy();
		if ( pSaveShip != NULL )
		{
			// lastly, add the ship to storage
			Storage storage;
			storage.nType = CARGO_TYPE_SHIP;
			storage.nFactionId = pCaptured->factionId();		// don't use factionId() from pSaveShip -- it's not valid!
			storage.nTime = Time::seconds();
			storage.pLocation = pCaptured->home();
			storage.sServer = m_Context.address;
			storage.nPort = m_Context.port;
			storage.instance.wrap( pSaveShip, 0 );

			addStorage( 0, nUserId, pSaveShip->key(), storage );

			dword nClientId = getClientId( nUserId );
			if ( nClientId != 0 )
				setClientSelf( 0, nClientId, NULL_WIDGET );
		}

		// save store to disk, players captured ship should be back in their garage!
		saveStorage( nUserId );
		// disassociate the player from the captured ship, so it doesn't get saved into their storage later on..
		pCaptured->setUserId( 0 );
	}

	// get the player credit for capping the ship
	if( WidgetCast<NounShip>( pWho ) != NULL 
		&& WidgetCast<NounShip>( pCaptured ) != NULL )
	{
		// if player captured an AI (can this even happen?)
		if(pWho->userId() != 0 && pCaptured->userId() == 0)
		{
			// players rank
			// ok give players lower than rank 3 full
			// 4-8 50%
			// greater that 8 25%			
			int nRank = ((NounShip *)pWho)->rank();
			if ( nRank < 1 )
				nRank = 1;

			if(nRank <= 3) // full value
				fScale = 1.0f;
			else if(nRank <= 8)
				fScale = 0.5f;
			else
				fScale = 0.25f;
		}
	}
	addProfile( pWho, SHIPS_CAPTURED, 1.0f * fScale );
}

void GameServer::onFriendlyFire( Noun * pWho, float damage )
{
	addProfile( pWho, FRIENDLY_FIRE, damage );
}

void GameServer::onPlanetsDamaged( Noun * pWho, float damage )
{
	addProfile( pWho, PLANETS_DAMAGED, damage );
}

void GameServer::onPlanetsCaptured( Noun * pWho, float amount )
{
	addProfile( pWho, PLANETS_CAPTURED, amount );
}

void GameServer::onRepair( Noun * pWho, float repair )
{
	addProfile( pWho, REPAIR, repair );
}

void GameServer::onConstruct( Noun * pWho, float construct )
{
	addProfile( pWho, CONSTRUCTION, construct );
}

void GameServer::onKamikaze( Noun * pWho, float damage )
{
	addProfile( pWho, KAMIKAZE, damage );
}

void GameServer::onSelfDestruct( NounShip * pWho )
{
	addProfile( pWho, SELF_DESTRUCTS, 1.0f );
}

void GameServer::onJump( Noun * pWho )
{
	addProfile( pWho, JUMPS, 1.0f );
}

void GameServer::onCollidePlanet( Noun * pWho )
{
	addProfile( pWho, PLANET_COLLISIONS, 1.0f );
}

void GameServer::onResourcesLost( Noun * pWho, float amount )
{
	addProfile( pWho, RESOURCES_LOST, amount );
}

void GameServer::onScout( Noun * pWho, float points )
{
	addProfile( pWho, SCOUT, points );
}

void GameServer::onBonusPrestige( Noun * pWho, float bonus )
{
	addProfile( pWho, BONUS_PRESTIGE, bonus );
}

void GameServer::onCredits( Noun * pWho, int credits )
{
	addProfile( pWho, CREDITS, credits );
}

void GameServer::onResources( Noun * pWho, int resources )
{
	addProfile( pWho, RESOURCES, resources );
}

void GameServer::onOrderReceived( NounShip * pWho, NounShip::Order order, Noun * pTarget, Noun * pFrom )
{}

int GameServer::getHighestRank( int a_nFactionId )
{
	int nHigestRank = -1;
	for(int i=0;i<clientCount();++i)
	{
		ClientContext & context = m_ClientContext[ GameServer::client(i) ];
		if ( context.nFactionId != a_nFactionId )
			continue;
		GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
		if (! pProfile )
			continue;
		int nRank = pProfile->calculateRank();
		if( nRank > nHigestRank )
			nHigestRank = nRank;
	}

	return nHigestRank;
}

bool GameServer::onBuildShip( NounPlanet * pPlanet, dword nUserId, NounShip * pShip, int nCost )
{
	dword nClientId = getClientId( nUserId );
	if ( nClientId == 0 )
		return false;		// user logged off..

	// subtract ship from build map..
	ShipQueueMap::iterator iQueue = m_ShipQueueMap.find( nUserId );
	if ( iQueue == m_ShipQueueMap.end() )
		return false;		
	if (  --iQueue->second == 0 )
		m_ShipQueueMap.erase( nUserId );		// last ship in build queue, remove the entry.. 

	ClientContextHash::iterator iContext = m_ClientContext.find( nClientId );
	if ( iContext == m_ClientContext.end() )
		return false;		// no context for the user
	ClientContext & context = iContext->second;

	Storage storage;
	storage.nType = CARGO_TYPE_SHIP;
	storage.nFactionId = context.nFactionId;
	storage.nTime = Time::seconds();
	storage.pLocation = pPlanet;
	storage.sServer = m_Context.address;
	storage.nPort = m_Context.port;
	storage.instance.wrap( pShip, 0 );

	addStorage( 0, nUserId, pShip->key(), storage );

	sendChat( nClientId, CharString().format( "SERVER: Ship build completed on '%s' shipyard!", pPlanet->name() ) );
	return true;
}

bool GameServer::onBuildCancelled( dword nUserId )
{
	ShipQueueMap::iterator iBuild = m_ShipQueueMap.find( nUserId );
	if ( iBuild != m_ShipQueueMap.end() )
	{
		if ( --iBuild->second == 0 )
			m_ShipQueueMap.erase( iBuild );
	}
	return true;
}

//---------------------------------------------------------------------------------------------------

bool GameServer::addProfile( dword nUserId, Statistics nStat, float fValue )
{
	// check for group and subscriber bonuses	
	// don't check stats we don't want to increase first
	if (   nStat != CREDITS
		&& nStat != RESOURCES_LOST
		&& nStat != PLANET_COLLISIONS
		&& nStat != FRIENDLY_FIRE
		&& nStat != KAMIKAZE
		&& nStat != SELF_DESTRUCTS 
		&& nStat != KILLED 
		&& nStat != KILLS
		&& nStat != JUMPS
		&& nStat != BONUS_PRESTIGE
		&& fValue > 0.0f ) // cover negative values for removing things like structures
	{
		// get our group and forward on the stats as bonus prestige to group members
		Group * pGroup = getGroup( nUserId );
		if ( pGroup != NULL )
		{
			// find all connected and valid members, add them to the list..
			std::list< dword > members;
			for(int i=0;i<pGroup->memberCount();++i)
			{
				GroupMember * pMember = pGroup->member( i );
				if ( pMember->isPending() )
					continue;
				if ( getClientId( pMember->userId() ) == 0 )
					continue;
				if ( pMember->userId() == nUserId ) // we don't want to give ourselves an extra 10%
					continue;

				members.push_back( pMember->userId() );
			}

			// get our percentage of the value and apply to all members of the group..
			float fGroupValue = fValue * GROUP_STAT_BONUS;
			fGroupValue *= GameProfile::calculateModifier( nStat );
			
			for( std::list<dword>::iterator iMember = members.begin(); iMember != members.end(); ++iMember )
				addPlayerProfile( *iMember, GameProfile::statisticName( BONUS_PRESTIGE ), fGroupValue );
		}
		
		// before we finish multiply the stat by the subscriber bonus
		UserClientHash::iterator iClient = m_UserClientHash.find( nUserId );
		if ( iClient != m_UserClientHash.end() )
		{
			ClientContextHash::iterator iContext = m_ClientContext.find( iClient->second );
			if ( iContext != m_ClientContext.end() )
			{
				ClientContext & context = iContext->second;
				GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
				if ( pProfile )
					if ( pProfile->isSubscriber() )
						fValue *= SUBSCRIBER_PRESTIGE_BONUS;
			}
		}
	}
	
	return addPlayerProfile( nUserId, GameProfile::statisticName( nStat ), fValue );
}

bool GameServer::addProfile( Noun * pTarget,Statistics nStat, float fValue )
{
	if (! pTarget || !pTarget->userId() )
		return false;
	return addProfile( pTarget->userId(), nStat, fValue );
}

bool GameServer::saveStorage( dword nUserId )
{
	if (! m_Context.bPersistant )
		return false;
	dword nClientId = getClientId( nUserId );
	if ( nClientId == 0 )
		return false;
	ClientContextHash::iterator iContext = m_ClientContext.find( nClientId );
	if ( iContext == m_ClientContext.end() )
		return false;
	ClientContext & context = iContext->second;

	UserStorage::Ref pStorage = context.pStorage;
	if (! pStorage )
		return false;
	if ( context.pSelf.valid() )
	{
		Storage active;
		active.nType = CARGO_TYPE_SHIP;
		active.instance.wrap( context.pSelf, 0 );
		active.nFactionId = context.pSelf->factionId();

		pStorage->add( context.pSelf->key(), active );
	}

	if (! pStorage->save( false ) )
	{
		LOG_ERROR( "GameServer", CharString().format("ERROR: Failed to save user %u storage", nUserId) );
		return false;
	}

	return true;
}

int GameServer::getStorageCargoCount( dword nUserId )
{
	UserStorage * pStorage = getStorage( nUserId );
	if (! pStorage )
		return 0;

	int nCargoCount = 0;
	for(int i=0;i<pStorage->storageCount();++i)
	{
		if ( pStorage->storage( i ).nType == CARGO_TYPE_ITEM )
			++nCargoCount;
	}

	return nCargoCount;
}

bool GameServer::storeAllyShips( NounShip * pShip )
{
	dword nUserId = pShip->userId();
	if ( nUserId == 0 )
		return false;

	if ( pShip->validateAllyShips() > 0 )
	{
		for(int i=0;i<pShip->allyCount();++i)
		{
			NounShip * pAlly = pShip->ally( i );

			Storage storage;
			storage.nType = CARGO_TYPE_ALLY;
			storage.nFactionId = pShip->factionId();
			storage.nTime = Time::seconds();
			storage.pLocation = pShip->home();
			storage.sServer = m_Context.address;
			storage.nPort = m_Context.port;
			storage.instance.wrap( pAlly, 0 );

			addStorage( 0, nUserId, pAlly->key(), storage );

			// detach the ally from the world now..
			pAlly->setDetach();
		}
	}

	return true;
}

static bool CheckNounGameNodes( NounGame * a_pNoun )
{
	bool bNodesRemoved = false;

	for(int i=a_pNoun->childCount()-1;i>0;--i)
	{
		BaseNode * pNode = a_pNoun->child(i);
		if ( WidgetCast<NounGame>( pNode ) == NULL )
		{
			LOG_ERROR( "GameServer", "Found node %s (%s) attached to %s (%s), removing.",
				pNode->name(), pNode->className(), a_pNoun->name(), a_pNoun->className() );
			a_pNoun->detachNode( i );
			bNodesRemoved = true;
		}
		else
			bNodesRemoved |= CheckNounGameNodes( (NounGame *)pNode );
	}

	return bNodesRemoved;
}

bool GameServer::auditShip( int a_nFactionID, NounShip * a_pShip, NounShip * a_pShipTemplate )
{
	if (! a_pShipTemplate || !a_pShip )
		return false;

	if ( CheckNounGameNodes( a_pShipTemplate ) )
		LOG_ERROR( "GameServer", "ShipTemplate %s has bad nodes", a_pShipTemplate->nounContext()->name() );
	if ( CheckNounGameNodes( a_pShip ) )
		LOG_ERROR( "GameServer", "Ship %s has bad nodes", a_pShip->nounContext()->name() );
	
	int j = 0;
	for( int i=0;i < a_pShipTemplate->childCount(); ++i )
	{
		BaseNode * pNodeT = a_pShipTemplate->child(i);

		NounGadget * pGadgetT = WidgetCast<NounGadget>( pNodeT );
		if ( pGadgetT != NULL )
		{
			NounGadget * pGadget = NULL;
			for( ;!pGadget && j < a_pShip->childCount(); ++j )
				pGadget = WidgetCast<NounGadget>( a_pShip->child(j) );

			Array< NounGadget * > upgrades;
			pGadgetT->getUpgrades( a_nFactionID, upgrades );

			bool bIsValid = pGadget != NULL && pGadget->nounContext() == pGadgetT->nounContext();
			if (! bIsValid && pGadget != NULL )
			{
				// check the upgrade paths, it might be one of those gadgets.
				for(int k=0;k<upgrades.size();++k)
				{
					NounGadget * pUpgrade = upgrades[k];
					if ( pUpgrade != NULL && pUpgrade->nounContext() == pGadget->nounContext() )
					{
						// position, orientation, and flags should come from the original device..
						pUpgrade->setPosition( pGadgetT->position() );
						pUpgrade->setOrientation( pGadgetT->orientation() );
						pUpgrade->setFlags( pGadgetT->flags() );

						pGadgetT = pUpgrade;
						bIsValid = true;
						break;
					}
				}
			}

			if ( bIsValid )
			{	
				// correct gadget... copy updates from the template.
				pGadget->setPosition( pGadgetT->position() );
				pGadget->setOrientation( pGadgetT->orientation() );
				pGadget->setName( pGadget->name() );
				pGadget->setNounContext( pGadgetT->nounContext() );
				pGadget->setFlags( pGadgetT->flags() );
			}
			else
			{
				pGadget = (NounGadget *)pGadgetT->copy();
				pGadget->setup();

				// gadgets don't match, insert a copy into the ship being validated..
				LOG_STATUS( "GameServer", CharString().format( "AuditShip: Adding gadget %s", pGadget->name() ) );

				int n = j - 1;
				if ( n < a_pShip->childCount() )
					a_pShip->insertNode( n, pGadget );
				else
					a_pShip->attachNode( pGadget );
			}
		}
	}

	// remove extra child gadgets that were not found in the template...
	while( j < a_pShip->childCount() )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( a_pShip->child(j) );
		if ( pGadget != NULL )
		{
			LOG_STATUS( "GameServer", CharString().format( "AuditShip: Removing gadget %s", pGadget->name() ) );
			a_pShip->detachNode( j );
		}
		else
			j += 1;
	}

	// audit NounTrail objects..
	j = 0;
	for( int i=0;i < a_pShipTemplate->childCount(); ++i )
	{
		BaseNode * pNodeT = a_pShipTemplate->child(i);

		NounTrail * pTrailT = WidgetCast<NounTrail>( pNodeT );
		if ( pTrailT != NULL )
		{
			NounTrail * pTrail = NULL;
			for( ;!pTrail && j < a_pShip->childCount(); ++j )
				pTrail = WidgetCast<NounTrail>( a_pShip->child(j) );

			if ( pTrail != NULL )
			{	
				// correct gadget... copy updates from the template.
				pTrail->setPosition( pTrailT->position() );
				pTrail->setOrientation( pTrailT->orientation() );
				pTrail->setTrailMaterial( pTrailT->trailMaterial() );
				pTrail->setNounContext( pTrailT->nounContext() );
			}
			else
			{
				pTrail = (NounTrail *)pTrailT->copy();
				pTrail->setup();

				// gadgets don't match, insert a copy into the ship being validated..
				LOG_STATUS( "GameServer", CharString().format( "AuditShip: Adding trail %s", pTrail->name() ) );

				int n = j - 1;
				if ( n < a_pShip->childCount() )
					a_pShip->insertNode( n, pTrail );
				else
					a_pShip->attachNode( pTrail );
			}
		}
	}

	// remove extra trails that were not found in the template...
	while( j < a_pShip->childCount() )
	{
		NounTrail * pTrail = WidgetCast<NounTrail>( a_pShip->child(j) );
		if ( pTrail != NULL )
		{
			LOG_STATUS( "GameServer", CharString().format( "AuditShip: Removing trail %s", pTrail->name() ) );
			a_pShip->detachNode( j );
		}
		else
			j += 1;
	}

	return true;
}
	
bool GameServer::pickSafeSpawnPosition( Noun * pSpawnPoint, NounShip * pSpawn )
{
	if (! pSpawnPoint || !pSpawn )
		return false;
	NodeZone * pZone = pSpawnPoint->zone();
	if (! pZone )
		return false;

	float fSpawnDistance = 200.0f;
	if ( WidgetCast<NounPlanet>( pSpawnPoint ) )
		fSpawnDistance = 200.0f;

	int nSpawnAttempts = 0;
	while( ++nSpawnAttempts <= MAX_SPAWN_ATTEMPTS )
	{
		// set the noun position in a random direction outside our spawn point radius
		Vector3 direction( RandomFloat(-1,1), 0.0f, RandomFloat(-1,1) );
		direction.normalize();

		Vector3 position( pSpawnPoint->position() );
		position += direction * ((pSpawnPoint->radius() + pSpawn->radius()) + fSpawnDistance );

		Array< GameContext::NounCollision > collisions;
		if (! pZone->proximityCheck( position, pSpawn->radius(), collisions ) )
		{
			float fHeading = atan2( direction.x, direction.z );
			pSpawn->setPosition( position );
			pSpawn->setOrientation( Vector3( 0.0f, fHeading, 0.0f ) );
			pSpawn->setHeading( fHeading );
			return true;
		}
	}

	return false;
}

void GameServer::buyEnhancement( ClientContext & context, const ClassKey & ck )
{
	if (! m_Context.bPersistant )
		return;		// we only allow buying on persistant servers now..
	if( ( m_MetaClient.profile().flags & MetaClient::SERVER) == 0 )
		return;		// only trusted servers can purchase enhancements

	Widget::Ref pUncasted = Factory::createWidget( ck );

	NounEnhancement::Ref pEnhancement = WidgetCast<NounEnhancement>( pUncasted );
	if ( pEnhancement && pEnhancement->canBuy() )		// is the item valid and is it buyable?
	{
		int nCargoCount = getStorageCargoCount( context.nUserId );
		if ( nCargoCount < MAX_CARGO_IN_STORAGE )
		{
			// get the credits available for this user..
			int nBuyValue = pEnhancement->buyValue();
			int nCredits = 0;
			
			GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
			if ( pProfile )
				nCredits = (*pProfile)[ CREDITS ];

			if ( nCredits >= nBuyValue )					// does the player have enough credits?
			{
				LOG_STATUS( "GameServer", "[STAT] (%s)(%u) bought enhancement (%s) for (%u) credits", 
					context.MetaProfile.name.cstr(), 
					context.nUserId, 
					pEnhancement->factory()->className(), 
					nBuyValue );

				// deduct credits first, if players find a crash exploit at least they will lose credits
				// instead of gaining infinite items with no credit cost..
				addPlayerProfile( context.nUserId, "Credits", -nBuyValue );
				nCredits -= nBuyValue;

				// send updated credits to the client
				send( context.nClientId, GameClient::STORE_UPDATE_CREDITS ) << nCredits;
				// ok, create item and add it to the players storage.. 
				CargoEnhancement::Ref pCargo = new CargoEnhancement( pEnhancement->factory()->classKey() );

				Storage storage;
				storage.nType = CARGO_TYPE_ITEM;
				storage.nFactionId = 0;
				storage.nTime = Time::seconds();
				storage.instance.wrap( pCargo, 0 );

				// add cargo item to storage
				addStorage( 0, context.nUserId, pCargo->key(), storage );
				// save player's storage
				saveStorage( context.nUserId );

				sendChat( context.nClientId, CharString().format( "STORE: '%s' purchased for %s Credits.",
					pEnhancement->getName(), FormatNumber<char,int>( pEnhancement->buyValue() ).cstr() ) );
			}
			else
			{
				// not enough credits to buy.. 
				sendChat( context.nClientId, CharString().format( "STORE: '%s' costs %s Credits and you only have %s Credits.",
					pEnhancement->getName(),
					FormatNumber<char,int>( nBuyValue ).cstr(), 
					FormatNumber<char,int>( nCredits ).cstr() ) );
			}
		}
		else
		{
			sendChat( context.nClientId, "STORE: Sorry your cargo storage is full, please remove some items from storage!" );
		}
	}
	else
	{
		LOG_ERROR( "GameServer", "ERROR: Failed to find class by key %s", ck.string().cstr() );
	}
}

void GameServer::updateFactionBalanceMap()
{
	m_nTopFaction = 0;
	for(int i=0;i<FACTION_COUNT;++i)
		m_FactionScoreMap[i] = 0;

	for(int i=0;i<clientCount();++i)
	{
		ClientContext & context = m_ClientContext[ GameServer::client(i) ];
		if ( context.nFactionId == 0 )
			continue;
		GameProfile * pProfile = WidgetCast<GameProfile>( context.pProfile );
		if (! pProfile )
			continue;
		int nRank = pProfile->calculateRank();
		if( nRank <= 0 )
			continue;

		int & nFactionScore = m_FactionScoreMap[ context.nFactionId ];
		nFactionScore += nRank;
		if ( nFactionScore > m_nTopFaction )
			m_nTopFaction = nFactionScore;
	}
}

//----------------------------------------------------------------------------
// EOF
