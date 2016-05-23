/*
	GameClient.cpp

	This object is used to connect to an GameServer object over a Socket object
	(c)1999 Palestar, Richard Lyle
*/

#define MEDUSA_TRACE_ON			// force trace messages to stay on
#include "Debug/Assert.h"
#include "Debug/ExceptionHandler.h"

#include "Standard/Time.h"
#include "Standard/MD5.h"
#include "World/NounTarget.h"
#include "NounStar.h"
#include "NounUnit.h"

#include "GameProfile.h"
#include "GameClient.h"

#include <map>
#include <string>

// how often to update our proxy worlds, in seconds
const int UPDATE_PROXY_INTERVAL = 300;
// how long until we remove an inactive proxy connection in seconds
const int PROXY_TIMEOUT = 60;					

//---------------------------------------------------------------------------------------------------

bool	GameClient::sm_bEnableProxyWorlds	= true;

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameClient, WorldClient );

GameClient::GameClient() : 
	m_nCredits( 0 ), 
	m_nUpdateProxy( 0 ),
	m_bShutdown( false ),
	m_pProxyClient( NULL ),
	m_pProxyParent( NULL ),
	m_nProxyTimeout( 0 ),
	m_bServersUpdated( false ),
	m_nLastTeamId( 0 ),
	m_nGlobalRoomId( 0 ),
	m_nFactionRoomId( 0 ),
	m_nFleetRoomId( 0 )
{
	// replace the generic Profile, with a GameProfile ... we need an empty profile until we receive ours from the server.
	m_pProfile = new GameProfile();
	m_pProfile->setName( "John Doe" );
}

GameClient::~GameClient()
{
	m_bShutdown = true;

	if ( m_pProxyClient != NULL )
	{
		delete m_pProxyClient;
		m_pProxyClient = NULL;
	}
}

//---------------------------------------------------------------------------------------------------

void GameClient::UserGameClient::onShipDestroyed( Noun * pKiller )
{}

void GameClient::UserGameClient::onShipCaptured( Noun * pCapper )
{}

void GameClient::UserGameClient::onShipDetached()
{}

void GameClient::UserGameClient::onOrderReceived( NounShip::Order order, Noun * pTarget, Noun * pFrom )
{}

//---------------------------------------------------------------------------------------------------

dword GameClient::version() const
{
	return GetProgramVersion();
}

bool GameClient::prepWorld()
{
	GameContext * pContext = gameContext();
	if (! pContext )
		return false;
	pContext->setGameUser( this );

	// zero out proxy update, so this is called when we receive a new context.
	m_nUpdateProxy = 5;
	m_pProxyTarget = NULL;
	
	if ( m_pProxyClient != NULL )
	{
		delete m_pProxyClient;
		m_pProxyClient = NULL;
	}

	return WorldClient::prepWorld();
}

Noun * GameClient::validateNoun( Noun * pNoun )
{
	if ( WidgetCast<NounTarget>( pNoun ) )
		return pNoun;		// all target noun types are always valid!
	NounGame * pTarget = WidgetCast<NounGame>( WorldClient::validateNoun( pNoun ) );
	if (! pTarget )
		return NULL;
	if (! pTarget->isDetected( factionId() ) )
		return NULL;		// object not detected by the players faction

	return pTarget;
}

void GameClient::onMessage( bool bUDP, u8 nMessage, const InStream & input )
{
	switch( nMessage )
	{
	case STORE_UPDATE_CREDITS:
		input >> m_nCredits;
		break;
	default:
		WorldClient::onMessage( bUDP, nMessage, input );
		break;
	}
}

void GameClient::setFocus( Noun * pFocus, bool bForce /*= false*/ )
{
	//checkProxyClient( pFocus );
	WorldClient::setFocus( pFocus, bForce );
}

void GameClient::setTarget( Noun * pTarget, bool bForce /*= false*/ )
{
	setProxyTarget( pTarget );
	WorldClient::setTarget( pTarget, bForce );
}

bool GameClient::selectSelf( const WidgetKey & a_nSpawn, 
						const WidgetKey & a_nSelf,
						const WidgetKeys & items )
{
	// check out spawn point, if this isn't our current server then request a transfer to the correct server..
	Noun::wRef pSpawn( a_nSpawn );
	if (! pSpawn.valid() )
		return false;

	if ( pSpawn->context() != context() )
	{
		WorldContext * pTargetContext = pSpawn->context();

		TransferRequest req;
		req.m_sAddress = pTargetContext->address();
		req.m_nPort = pTargetContext->port();
		req.m_nSelf = a_nSelf;
		req.m_Items = items;
		req.m_sSpawnPoint = pSpawn->name();

		// initiate a server transfer to the server we want to spawn from..
		return requestTransfer( req );
	}

	return WorldClient::selectSelf( a_nSpawn, a_nSelf, items );
}

void GameClient::onSetSelf( Noun * pSelf )
{
	NounShip * pShip = WidgetCast<NounShip>( pSelf );
	if (! pSelf )
		return;

	// find all ally ships already attached into the world and add them to our ship..
	WorldContext * pContext = context();
	for(int z=0;z<pContext->zoneCount();++z)
	{
		NodeZone * pZone = pContext->zone( z );
		for(int i=0;i<pZone->childCount();++i)
		{
			NounShip * pChild = WidgetCast<NounShip>( pZone->child(i) );
			if (! pChild )
				continue;
			if ( (pChild->flags() & NounShip::FLAG_AI_ALLY) == 0 )
				continue;
			if ( pChild->home() != pShip )
				continue;		// not our AI ally

			pShip->addAlly( pChild );
		}
	}
}

bool GameClient::pushChat( const char * pChat, dword nFromId /*= 0*/ )
{
	if ( m_pProxyParent )
		return m_pProxyParent->pushChat( pChat, nFromId );

	return WorldClient::pushChat( pChat, nFromId );
}

bool GameClient::canGroupInvite( Noun * pTarget )
{
	if ( WidgetCast<NounUnit>( pTarget ) != NULL )
		return false;

	return WorldClient::canGroupInvite( pTarget );
}

void GameClient::onUpdate()
{
	if (! m_bProxy )
	{
		updateProxies();
		updateProxyClient();
	}
}

//---------------------------------------------------------------------------------------------------

void GameClient::onChat( Noun * pNoun, const char * pMessage )
{
	bool bPushChat = true;

	// ignore chat messages from planets and structures not in our faction...
	if ( (WidgetCast<NounPlanet>( pNoun ) || WidgetCast<NounStructure>( pNoun )) )
		bPushChat = pNoun->factionId() == factionId();

	if ( bPushChat )
		WorldClient::onChat( pNoun, pMessage );
}

void GameClient::onAttachNoun( Noun * pNoun )
{
	// ally ships are not replicated down with the ship, instead we have to catch an AI ships getting added to our
	// client who has the correct flag and home is set to our ship.
	if ( WidgetCast<NounShip>( pNoun ) != NULL )
	{
		NounShip * pSelf = WidgetCast<NounShip>( self() );
		if ( pSelf != NULL )
		{
			NounShip * pNewShip = (NounShip *)pNoun;
			if ( (pNewShip->flags() & NounShip::FLAG_AI_ALLY) != 0 && pNewShip->home() == pSelf )
				pSelf->addAlly( pNewShip );
		}
	}
}

void GameClient::onDetachNoun( Noun * pNoun )
{}

//---------------------------------------------------------------------------------------------------

void GameClient::onKill( Noun * pWho, Noun *pDead )
{}

void GameClient::onDestroyed( NounShip * pWho, Noun * pKiller )
{
	pushChat( CharString().format( "<color;ffffff>Comms: %s has been destroyed by %s!", 
		pWho->name(), pKiller != NULL ? pKiller->name() : "?" ) );

	// check if the players ship was destroyed
	if ( pWho != NULL && pWho == m_pSelf )
	{
		// notify the user
		((UserGameClient *)user())->onShipDestroyed( pKiller );
	}
}

void GameClient::onDamageShip( Noun * pWho, float damage, Noun *pTarget )
{}

void GameClient::onCaptureShip( Noun * pWho, NounShip * pCaptured )
{
	if ( pCaptured != NULL && pWho != NULL )
		pushChat( CharString().format( "<color;ffffff>Comms: %s has been captured by the %s (%s)", 
			pCaptured->name(), pWho->teamName(), pWho->name() ) );

	// this is the captured ship ?
	if ( pCaptured != NULL && pCaptured == m_pSelf )	
	{
		// inform user
		((UserGameClient *)user())->onShipCaptured( pWho );
	}
}

void GameClient::onFriendlyFire( Noun * pWho, float damage )
{}

void GameClient::onPlanetsDamaged( Noun * pWho, float damage )
{}

void GameClient::onPlanetsCaptured( Noun * pWho, float amount )
{}

void GameClient::onRepair( Noun * pWho, float repair )
{}

void GameClient::onConstruct( Noun * pWho, float construct )
{}

void GameClient::onKamikaze( Noun * pWho, float damage )
{}

void GameClient::onSelfDestruct( NounShip * pWho )
{
	pushChat( CharString().format( "<color;ffffff>Comms: %s has self-destructed!", pWho->name() ) );

	if ( pWho == m_pSelf )
	{
		// notify user
		((UserGameClient *)user())->onShipDestroyed( pWho );
	}
}

void GameClient::onJump( Noun * pWho )
{}

void GameClient::onCollidePlanet( Noun * pWho )
{}

void GameClient::onResourcesLost( Noun * pWho, float amount )
{}

void GameClient::onScout( Noun * pWho, float points )
{}

void GameClient::onBonusPrestige( Noun * pWho, float bonus )
{}

void GameClient::onCredits( Noun * pWho, int credits )
{}

void GameClient::onResources( Noun * pWho, int resources )
{}

void GameClient::onOrderReceived( NounShip * pWho, NounShip::Order order, Noun * pTarget, Noun * pFrom )
{
	if ( pWho == self() )
		((UserGameClient *)user())->onOrderReceived( order, pTarget, pFrom );
}

//---------------------------------------------------------------------------------------------------

bool GameClient::buyEnhancement( const ClassKey & a_nEnhancement )
{
	if (! m_LoggedIn )
		return false;

	send( STORE_BUY_ENHANCEMENT ) << a_nEnhancement;
	return true;
}

//---------------------------------------------------------------------------------------------------

typedef std::map< CharString, CharString >		DataMap;

// this functions splits the incoming server data into key/value pairs...
static void HashServerData( CharString & data, DataMap & map )
{
	CharString token;
	while( data.tokenize( token, ";" ) )
	{
		int nSplit = token.find( '=' );
		if ( nSplit < 0 )
			continue;		// no equal sign..
		
		token[ nSplit ] = 0;
		CharString sKey( token.cstr() );
		CharString sValue( token.cstr() + nSplit + 1 );
		map[ sKey ] = sValue;
	}
}

//---------------------------------------------------------------------------------------------------

void GameClient::updateMeta()
{
	WorldClient::updateMeta();

	if ( sm_bEnableProxyWorlds && --m_nUpdateProxy <= 0 )
	{
		if ( m_pMetaClient->connected() && m_pMetaClient->loggedIn() )
		{
			m_nUpdateProxy = UPDATE_PROXY_INTERVAL;

			Array< MetaClient::Server > servers, subservers;
			if ( m_pMetaClient->getServers( m_ServerStatus.name, m_ServerStatus.gameId, MetaClient::GAME_SERVER, servers ) >= 0 
				&& m_pMetaClient->getServers( m_ServerStatus.name, m_ServerStatus.gameId, MetaClient::GAME_SUB_SERVER, subservers) >= 0 )
			{
				lock();

				m_Servers.release();
				for(int i=0;i<servers.size();++i)
					m_Servers.push( servers[i] );
				for(int i=0;i<subservers.size();++i)
					m_Servers.push( subservers[i] );

				m_bServersUpdated = true;
				unlock();
			}
		}
	}

	lock();
	bool bUpdateRooms = m_nLastTeamId != m_nTeamId && context()->isTeamValid( m_nTeamId );
	if ( bUpdateRooms && m_pMetaClient->connected() && m_pMetaClient->loggedIn() )
	{
		m_nLastTeamId = m_nTeamId;

		const Team & t = context()->team( m_nTeamId );
		CharString sGlobalRoom;
		sGlobalRoom.format( "GlobalChat;%s", m_ServerStatus.name.cstr() );
		CharString sFleetRoom;
		sFleetRoom.format( "FleetChat;%s;%s;%u", m_ServerStatus.name.cstr(), factionText( t.factionId ), t.clanId );
		CharString sFactionRoom;
		sFactionRoom.format("FactionChat;%s;%s", m_ServerStatus.name.cstr(), factionText( t.factionId ) );
		
		unlock();

		// leave all previous rooms
		m_pMetaClient->leaveRoom();
		// now join 3 different rooms based on the chat 
		m_nGlobalRoomId = m_pMetaClient->createRoom( sGlobalRoom, MD5( sGlobalRoom + "8yehd3he7dh" ).checksum().cstr(), false, false, true );
		m_nFleetRoomId = m_pMetaClient->createRoom( sFleetRoom, MD5( sFleetRoom + "2hd9wqqhd73e" ).checksum().cstr(), false, false, true );
		m_nFactionRoomId = m_pMetaClient->createRoom( sFactionRoom, MD5( sFactionRoom + "jsdhi3heyfgsd" ).checksum().cstr(), false, false, true );

		// set the default room
		m_nDefaultRoomId = m_nGlobalRoomId;	
	}
	else
		unlock();
}

bool GameClient::updateProxies()
{
	GameContext * pContext = gameContext();
	if (! pContext )
		return false;
	if (! m_bServersUpdated )
		return false;
	m_bServersUpdated = false;

	bool bFlushGates = false;

	std::set< WidgetKey > contextSet;
	for(int i=0;i<m_Servers.size();++i)
	{
		const MetaClient::Server & server = m_Servers[i];

		// skip if the server is not the correct type..
		if ( server.type != MetaClient::GAME_SERVER &&
			server.type != MetaClient::GAME_SUB_SERVER )
		{
			continue;
		}
		// skip if this is our current server..
		if ( server.address == m_ServerStatus.address 
			&& server.port == m_ServerStatus.port )
		{
			continue;
		}

		int nKeyPosition = server.data.find( "KEY=" );
		if ( nKeyPosition < 0 )
		{
			TRACE( "Failed to get context key from server data!" );
			continue;
		}

		WidgetKey nWorld( server.data.buffer() + nKeyPosition + 4 );
		if ( nWorld == pContext->key() )	
			continue;		// don't add a proxy for our current map..

		// insert into our set, so we know which world's to remove..
		contextSet.insert( nWorld );

		GameContext::Ref pWorld = WidgetCast<GameContext>( pContext->findWorld( nWorld ) );
		if (! pWorld.valid() )
		{
			// world isn't loaded yet, load the resource from disk..
			ResourceLink< GameContext > pWorldLink( nWorld );
			if (! pWorldLink.valid( true, false ) )
			{
				// set the flag back to true, try again next frame after it's had some time to load..
				m_bServersUpdated = true;
				return false;
			}

			// add the loaded context to our current context as another world..
			pWorld = pWorldLink;
			pWorld->setUser( NULL );		// clear any user, this could be set if the player transfered from one of their proxy worlds
			pWorld->clearWorlds();
			pWorld->detachAllShips();
			pWorld->onWorldTransform( -pWorld->zoneTransform() );		// move everything back to 0,0,0
			pWorld->onWorldTransform( pContext->zoneTransform() + (pWorld->position() - pContext->position()) );
			pWorld->calculateWorldExtents();
			pWorld->setAddress( server.address, server.port );
			pWorld->start();
		}

		// don't care if this fails, just want to make sure it's added as a proxy world..
		pContext->addWorld( pWorld );		
		bFlushGates = true;

		// split the server data into key/value pairs for quick access..
		DataMap map;
		CharString dataCopy( server.data );
		HashServerData( dataCopy, map );

		// since we don't have all the teams since this is a proxy, we need to enumerate the fleets from the data
		// and add any missing teams to our proxy context.
		int nFleetCount = atoi( map[ "FC" ] );
		for(int f=0;f<nFleetCount;++f)
		{
			int nFleetId = atoi( map[ CharString().format( "F%dID", f ) ] );
			if (! pWorld->isTeamValid( nFleetId ) )
			{
				WorldContext::Team fleet;
				fleet.nTeamId = nFleetId;
				fleet.clanId = atoi( map[ CharString().format( "F%dCID", f ) ] );
				fleet.factionId = atoi( map[ CharString().format( "F%dFID", f ) ] );
				fleet.flags = 0;
				fleet.name = map[ CharString().format( "F%dNAME", f ) ];

				int nTemplateId = pWorld->factionTeam( fleet.factionId );
				if ( nTemplateId != 0 )
				{
					const WorldContext::Team & temp = pWorld->team( nTemplateId );
					fleet.about = temp.about;
					fleet.longName = temp.longName;
					fleet.pTemplates = temp.pTemplates;
				}

				pWorld->addTeam( fleet );
			}
		}

		// update the proxy world to the current server data...
		int nPlanetCount = atoi( map[ "PLANETS" ] );
		for(int p=0;p<nPlanetCount;++p)
		{
			WidgetKey nPlanetKey( map[ CharString().format("P%dKEY", p) ] );
			//NounPlanet::wRef pPlanet = WidgetCast<NounPlanet>( pWorld->findNounByKey( nPlanetKey ) );
			NounPlanet::wRef pPlanet( nPlanetKey );
			if ( pPlanet.valid() )
			{
				pPlanet->setFlags( atoi( map[ CharString().format( "P%dFLAG", p ) ] ) );
				pPlanet->setTeamId( atoi( map[ CharString().format( "P%dFLEET", p ) ] ) );
			}
			else
			{
				LOG_ERROR( "GameClient", "Failed to find planet '%s'!", nPlanetKey.string().cstr() );
			}
		}

		int nStarCount = atoi( map[ "STARS" ] );
		for(int s=0;s<nStarCount;++s)
		{
			WidgetKey nStarKey( map[ CharString().format( "S%dKEY", s ) ] );
			//NounStar::wRef pStar = WidgetCast<NounStar>( pWorld->findNounByKey( nStarKey ) );
			NounStar::wRef pStar( nStarKey );
			if ( pStar.valid() )
			{
				pStar->setTeamId( atoi( map[ CharString().format( "S%dFLEET", s ) ] ) );
			}
			else
			{
				LOG_ERROR( "GameClient", "Failed to find star '%s'!", nStarKey.string().cstr() );
			}
		}

		// update all invalid jump gates so they can find their destination..
		pContext->updateInvalidJumpgates();
	}

	// remove any worlds that no longer have a server...
	std::list< WorldContext * > remove;
	for(int i=0;i<context()->worldCount();++i)
	{
		WorldContext * pWorld = context()->world(i);
		if ( contextSet.find( pWorld->key() ) == contextSet.end() )
			remove.push_back( pWorld );
	}

	while( remove.size() > 0 )
	{
		context()->deleteWorld( remove.front() );
		bFlushGates = true;
		
		remove.pop_front();
	}

	// invalidate the gate path-finding if any worlds are added or removed..
	if ( bFlushGates )
		pContext->flushGatePaths();

	return true;
}

bool GameClient::setProxyTarget( Noun * pTarget )
{
	// resolved to the root object..
	pTarget = pTarget->zoneParent();

	if ( !m_bProxy && (pTarget == NULL || pTarget->context() != context()) )
	{
		if ( m_pProxyTarget != pTarget )
		{
			m_pProxyTarget = pTarget;
			return true;
		}
	}

	return false;
}

bool GameClient::updateProxyClient()
{
	if ( m_pProxyTarget.valid() )
	{
		if ( m_pProxyClient == NULL )
		{
			m_pProxyClient = new GameClient();
			m_pProxyClient->setEnableUDP( false );
			m_pProxyClient->m_pProxyParent = this;
			// need to share the same lock object, so the clients don't access the same objects at the same time
			m_pProxyClient->m_Lock = m_Lock;
		}

		WidgetKey nSelf( NULL_WIDGET );
		if ( self() != NULL )
			nSelf = self()->key();
		int nFactionId = factionId();

		WorldContext::wRef pTargetContext = m_pProxyTarget->context();
		if ( pTargetContext.valid() && !isServerTransfer()
			&& (m_ServerStatus.address != pTargetContext->address() || m_ServerStatus.port != pTargetContext->port())
			&& (pTargetContext != m_pProxyClient->context() || !m_pProxyClient->loggedIn()) )
		{
			// get the address/port of our new server...
			CharString		sAddress( pTargetContext->address() );
			u16				nPort( pTargetContext->port() );
			dword			nSessionId( sessionId() );

			m_pProxyClient->setContext( pTargetContext );

			LOG_DEBUG_HIGH( "GameClient", "Proxy client connecting to %s:%d", sAddress.cstr(), nPort );
			if (! m_pProxyClient->connect( sAddress, nPort ) )
			{
				LOG_ERROR( "GameClient", "Failed to connect proxy client to %s:%d", sAddress.cstr(), nPort );
				lock();
				return false;
			}

			if (! m_pProxyClient->loginAsProxy( nSessionId, nSelf, nFactionId ) )
			{
				LOG_ERROR( "GameClient", "Failed to login with session %u", nSessionId );
				lock();
				return false;
			}

			// we only want to receive our target object with a proxy connection!
			m_pProxyClient->setFocus( m_pProxyTarget, true );
		}

		if ( m_pProxyTarget.valid() && m_pProxyClient->focus() != m_pProxyTarget )
			m_pProxyClient->setFocus( m_pProxyTarget );
		if ( m_pProxyClient->proxyId() != nSelf || m_pProxyClient->proxyFactionId() != nFactionId )
			m_pProxyClient->selectProxy( nSelf, nFactionId );

		m_nProxyTimeout = Time::seconds() + PROXY_TIMEOUT;
	}
	else if ( m_pProxyClient != NULL && m_pProxyClient->connected() )
	{
		if ( Time::seconds() > m_nProxyTimeout )		// 600 * 100ms = 5 minutes
		{
			m_pProxyClient->close();
		}
	}

	if ( m_pProxyClient != NULL )
		m_pProxyClient->update();

	return true;
}

//---------------------------------------------------------------------------------------------------
// EOF
