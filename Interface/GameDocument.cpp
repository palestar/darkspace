/*
	GameDocument.cpp

	This object is used to connect to an UniverseServer object over a Socket object
	(c)1999 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Debug/Profile.h"
#include "Standard/Settings.h"
#include "Standard/CommandLine.h"
#include "Standard/Time.h"
#include "Standard/RegExpM.h"
#include "File/FileDisk.h"
#include "System/Keyboard.h"
#include "Gui3d/WindowText.h"
#include "Gui3d/InterfaceContext.h"
#include "World/VerbChat.h"
#include "World/VerbCallScript.h"
#include "DarkSpace/Constants.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/VerbOrderShip.h"
#include "Interface/WindowObserve.h"
#include "Interface/WindowNavigation.h"
#include "Interface/ViewGame.h"
#include "Interface/ViewTactical.h"
#include "Interface/GameDocument.h"
#include "System/Platform.h"

//----------------------------------------------------------------------------

static Constant CHECK_MUSIC_STATE_TIME( "CHECK_MUSIC_STATE_TIME", 10.0f );

float			GameDocument::s_fCamera = 0.0f;
Vector3			GameDocument::s_vCamera = Vector3( 0, 0, 0);
Matrix33		GameDocument::s_mCamera = Matrix33( true );

//---------------------------------------------------------------------------------------------------

class GroupInviteDialog : public MessageBoxDialog
{
public:
	GroupInviteDialog( Noun * pFrom ) : m_pFrom( pFrom ),
	  MessageBoxDialog( MBT_YESNO, CharString().format( "'%s' has invited you into their group - accept invite?", pFrom->name() ) )
	{}

	virtual void onOK()
	{
		m_pDoc->client()->groupInvite( m_pFrom );
	}
	virtual void onCancel()
	{
		m_pDoc->client()->groupDecline( m_pFrom );
	}

	Noun::wRef			m_pFrom;
};

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameDocument, Document );
REGISTER_FACTORY_KEY( GameDocument, 4041296134731847970 );

#pragma warning(disable:4800) // forcing value to bool 'true' or 'false' (performance warning)

GameDocument::GameDocument() : m_GameState( GS_GAME ), 
	m_SessionId( 0 ), 
	m_pClient( NULL ), 
	m_pJukeBox( NULL ),
	m_EndGame( false ), 
	m_CheckMusicState( 0 ), 
	m_bShipLost( false ), 
	m_bIsConnected( false )
{
	m_pClient = new GameClient();
	m_pClient->setUser( this );
}

GameDocument::~GameDocument()
{
	// delete the client
	delete m_pClient;
	// stop the music
	if ( m_pJukeBox != NULL )
		delete m_pJukeBox;
}

//----------------------------------------------------------------------------

// interface is initialized, this is called by InterfaceContext
void GameDocument::onActivate()
{
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif

	// begin playing music
	if ( m_pJukeBox == NULL )
	{
		int musicVolume = settings.get( "musicVolume", 50 );
		if ( musicVolume > 0 )
		{
			CharString musicDirectory = settings.get( "music", ".\\Music\\" );

			// create the jukebox object and initialize
			if ( ( m_pJukeBox = Jukebox::create() ) != NULL )
				m_pJukeBox->initialize( musicDirectory, musicVolume );
		}
	}

	// enable keyboard mapping
	Keyboard::enableKeyMap();
	// load the players keyboard mapping
	Keyboard::loadKeyMap( settings.get( "keyMap", ".\\keyboard.map" ) );
	// begin at the lobby screen
	setGameState( GS_LOBBY );

	// attempt to connect to server at localhost on port 9010
	connect( 0, "localhost", 9010 );
}

void GameDocument::onLock()
{
	m_pClient->lock();
}

void GameDocument::onPreUpdate()
{
	// display number of lines and triangles rendered
	Profiler::message( 4, CharString().format("Triangles Rendered: %u, Lines Rendered: %u", 
		DisplayDevice::sm_nTrianglesRendered, DisplayDevice::sm_nLinesRendered) );

	// reset the triangle / line counters
	DisplayDevice::sm_nTrianglesRendered = DisplayDevice::sm_nLinesRendered = 0;
}

void GameDocument::onUpdate( float t )
{
	// update the client
	m_pClient->update();
	// validate the cursor target
	setCursorTarget( validateNoun( m_pCursorTarget ) );

	// if we are playing music, check the game state
	if ( m_pJukeBox != NULL )
	{
		m_CheckMusicState += t;
		if ( m_CheckMusicState > CHECK_MUSIC_STATE_TIME )
		{
			m_CheckMusicState = 0.0f;

			bool isCombat = false;

			NounShip * pShip = ship();
			if ( pShip != NULL )
			{
				// check contacts for the ship, if any enemy contacts found, switch to combat music
				for(int i=0;i<pShip->contactCount() && !isCombat;i++)
					if ( pShip->isEnemy( pShip->contact( i ) ) )
						isCombat = true;
			}

			if ( isCombat )
				m_pJukeBox->setPlayList( "COMBAT" );
			else
				m_pJukeBox->setPlayList( "AMBIENT" );
		}
	}
}

void GameDocument::onUnlock()
{
	m_pClient->unlock();
}

bool GameDocument::onMessage( const Message & msg )
{
	return( Document::onMessage( msg ) );
}

//----------------------------------------------------------------------------

void GameDocument::onConnected()
{
	TRACE( "GameDocument::onConnected" );
	pushChat( "Connected to server..." );
	m_bIsConnected = true;

	if (! m_pClient->isServerTransfer() )
		setGameState( GS_CONNECT_SERVER );
}

void GameDocument::onLogin( bool bLogin )
{
	TRACE( "GameDocument::onLogin" );
	pushChat( bLogin ? "Logged into server..." : "Failed to login..." );

	if (! bLogin )
	{
		setGameState( GS_DISCONNECTED );
		m_bIsConnected = false;
	}
}

void GameDocument::onUpdateQueuePosition()
{
	//if ( client()->queuePosition() == 0 )
	//	pushChat( "<color;ffffff>SERVER: Ready to engage drives!" );
}

void GameDocument::onMetaLogin()
{}

void GameDocument::onContextReceived()
{
	TRACE( "GameDocument::onContextReceived" );

	if ( context() != NULL )
	{
		if ( !client()->isServerTransfer() && !m_EndGame )
		{
			// update the client before changing the scene
			m_pClient->update();
			// pre-cache all the textures
			setGameState( GS_CONNECT_SERVER );
		}
	}
	else
	{
		// failed to receive the context, this is bad...
		pushChat( "Failed to receive the world, disconnecting..." );
		setGameState( GS_DISCONNECTED );
		m_bIsConnected = false;
	}

	// clear the end game flag
	m_EndGame = false;
}

void GameDocument::onContextEnd( const char * pMessage )
{
	TRACE( "GameDocument::onContextEnd" );

	// display the end-game screen!
	setGameState( GS_END_GAME );
	// set the end game boolean to true
	m_EndGame = true;
	// inform our script
	context()->scriptCall( "onEndGame()" );

	// clear some pointers
	m_pCursorTarget = NULL;
	m_pLastSpawn = NULL;
	// store the end message into the message window of the EndGame scene
	WindowText * pWindow = WidgetCast<WindowText>( InterfaceContext::findNode( "Message" ) );
	if ( pWindow != NULL )
		pWindow->setText( pMessage );
}

void GameDocument::onDisconnected()
{
	TRACE( "GameDocument::onDisconnected" );
	setGameState( GS_DISCONNECTED );
	m_bIsConnected = false;
}

void GameDocument::onWorldTransform( const Vector3 & transform )
{
	TRACE( CharString().format( "GameDocument::onWorldTransform (%s)", (CharString)transform) );

	// update all static camera positions by the shift in the zone positions
	s_vCamera += transform;
	WindowNavigation::sm_vCameraTarget += transform;
	ViewTactical::sm_AdjustPosition += transform;
	ViewTactical::sm_CameraPosition += transform;
}

void GameDocument::onServerTransfer( const WorldClient::TransferRequest & a_Req )
{
#ifdef _DEBUG
	Settings settings( "ClientD" );
#else
	Settings settings( "Client" );
#endif
	settings.put( "transferAddress", CharString().format( "%s:%d", a_Req.m_sAddress, a_Req.m_nPort) );

	setGameState( GS_SERVER_TRANSFER );
}

void GameDocument::onProfile( Profile * pProfile )
{
	TRACE( "GameDocument::onProfile" );
}

void GameDocument::onServerStatus( ServerStatus & status)
{
	TRACE( "GameDocument::onServerStatus" );
}

void GameDocument::onChat( const char * pMessage, dword nFromId )
{}

void GameDocument::onFlushChat()
{
	TRACE( "GameDocument::onFlushChat" );
}

void GameDocument::onVerb( Verb * pVerb )
{}

void GameDocument::onSetFocus( Noun * pFocus )
{}

void GameDocument::onSetFocusArea( float fFocusArea )
{}

void GameDocument::onSetTarget( Noun * pTarget )
{}

void GameDocument::onSetSelf( Noun * pShip )
{
	if ( pShip )
	{
		TRACE( CharString().format("GameDocument::onShipSelected, pShip = %p", pShip) );
		m_bShipLost = false;

		context()->scriptCall( CharString().format("onShipAttached( %s )", pShip->key().string()) );

		if ( m_GameState == GS_SERVER_TRANSFER )
			setGameState( GS_GAME );
	}
	else
	{
		onShipDetached();
	}
}

void GameDocument::onSetTeam( int teamId )
{
	TRACE( CharString().format( "GameDocument::onSetFleet, teamId = %d", teamId ) );
	// call into our scripts
	context()->scriptCall( CharString().format("onSetFleet( %u )", teamId) );
}

void GameDocument::onReceiveStorage( UserStorage * a_pStorage )
{}

void GameDocument::onAddStorage( const WidgetKey & nKey )
{}

void GameDocument::onDeleteStorage( const WidgetKey & nKey )
{}

void GameDocument::onGroupInvite( Noun * a_pWho )
{
	if ( ViewGame::sm_pViewGame != NULL )
		ViewGame::sm_pViewGame->pushMessageBox( new GroupInviteDialog( a_pWho ) );
}

void GameDocument::onGroupUpdate( Group * a_pGroup )
{}

void GameDocument::onGroupLeave()
{}

void GameDocument::onShipDestroyed( Noun * pKiller )
{
	TRACE( "GameDocument::onShipDestroyed" );

	setGameState( GS_DEATH );
	m_bShipLost = true;

	// inform our script
	CharString sSelf = ship()->key().string();
	CharString sKiller = "";
	if ( pKiller != NULL )
		sKiller = pKiller->key().string();
	context()->scriptCall( CharString().format("onDeath( '%s','%s' )", sSelf, sKiller) );

	// reset the camera time
	s_fCamera = 0.0f;

	WindowObserve * pWindow = WidgetCast<WindowObserve>( InterfaceContext::findNode( "ObserveWindow" ) );
	if ( pWindow != NULL )
	{
		pWindow->setCameraMode( WindowObserve::CHASE );
		pWindow->setTargetMode( WindowObserve::LOCKED );
		pWindow->setTarget( ship() );
	}

	WindowText * pMessage = WidgetCast<WindowText>( InterfaceContext::findNode( "TextMessage" ) );
	if ( pMessage != NULL )
	{
		if ( pKiller && pKiller != ship() )
			pMessage->setText( CharString().format("Ship destroyed by %s...", pKiller->name()) );
		else if ( pKiller == ship() )
			pMessage->setText( "You have self destructed..." );
		else
			pMessage->setText( "Ship destroyed..." );
	}
}

void GameDocument::onShipCaptured( Noun * pCapper )
{
	TRACE( "GameDocument::onShipCaptured" );

	setGameState( GS_DEATH );
	m_bShipLost = true;

	// inform our script
	CharString sSelf = ship()->key().string();
	CharString sCapper = pCapper != NULL ? pCapper->key().string() : "";
	context()->scriptCall( CharString().format("onCaptured( '%s''%s' )", sSelf, sCapper ) );
	
	// reset the camera time
	s_fCamera = 0.0f;

	WindowObserve * pWindow = WidgetCast<WindowObserve>( InterfaceContext::findNode( "ObserveWindow" ) );
	if ( pWindow != NULL )
	{
		pWindow->setCameraMode( WindowObserve::CHASE );
		pWindow->setTargetMode( WindowObserve::LOCKED );
		pWindow->setTarget( ship() );
	}

	WindowText * pMessage = WidgetCast<WindowText>( InterfaceContext::findNode( "TextMessage" ) );
	if ( pMessage != NULL )
	{
		if ( pCapper != NULL )
			pMessage->setText( CharString().format("Ship captured by %s...", pCapper->name()) );
		else
			pMessage->setText( "Ship captured..." );
	}
}

void GameDocument::onShipDetached()
{
	TRACE( "GameDocument::onShipDetached" );
	// player left game through a jumpgate or exited the zone
	if ( !client()->isServerTransfer() && !m_bShipLost )
		setGameState( GS_SELECT_SHIP );
	// inform our script
	context()->scriptCall( "onShipDetached()" );
}

//----------------------------------------------------------------------------

Noun * GameDocument::rootTarget() const
{
	Noun * pTarget = target();
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();

	if ( pTarget != NULL )
		pTarget = pTarget->zoneParent();

	return pTarget;
}

NounGadget * GameDocument::gadgetTarget() const
{
	return WidgetCast<NounGadget>( target() );
}

NounGadget * GameDocument::myGadgetTarget() const
{
	NounGadget * pGadget = gadgetTarget();
	if ( pGadget != NULL && pGadget->parentNoun() == ship() )
		return pGadget;

	return NULL;
}

NounPlanet * GameDocument::planetTarget() const
{
	Noun * pTarget = target();
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();
	
	while( pTarget != NULL )
	{
		if ( WidgetCast<NounPlanet>( pTarget ) )
			return (NounPlanet *)pTarget;
		pTarget = pTarget->parentNoun();
	}

	return NULL;
}

//----------------------------------------------------------------------------

void GameDocument::setGameState( GameState a_nState )
{
	m_GameState = a_nState;
	switch( m_GameState )
	{
	case GS_LOBBY:
		setScene( "Lobby" );
		break;
	case GS_MAIN:
		setScene( "Main" );
		break;
	case GS_GAME:
		setScene( "Game" );
		break;
	case GS_CONNECT_SERVER:
		setScene( "ConnectServer" );
		break;
	case GS_SELECT_SHIP:
		setScene( "SelectShip" );
		break;
	case GS_SERVER_TRANSFER:
		//setScene( "Redirect" );
		break;
	case GS_DEATH:
		setScene( "Death" );
		break;
	case GS_END_GAME:
		setScene( "EndGame" );
		break;
	case GS_DISCONNECTED:
		setScene( "ServerDisconnect" );
		break;
	}
}

bool GameDocument::connect( dword sessionId, const char * pAddress, int port )
{
	m_SessionId = sessionId;
	// connect to the server
	if (! m_pClient->connect( pAddress, port ) )
		return false;

	// connected
	return true;
}

//----------------------------------------------------------------------------

void GameDocument::sendChat( const char * pMessage )
{
	client()->sendChat( pMessage );
}

void GameDocument::pushChat( const char * pMessage )
{
	client()->pushChat( pMessage );
}

void GameDocument::flushChat()
{
	client()->flushChat();
}

//----------------------------------------------------------------------------

bool GameDocument::isTeamValid()
{
	return context()->isTeamValid( client()->teamId() );
}

bool GameDocument::isShipValid()
{
	return validateShip( ship() );
}

bool GameDocument::validateShip( NounShip * pShip )
{
	if (! isTeamValid() )
		return false;			// player has no valid fleet selected!
	if ( pShip == NULL )
		return false;			// no ship selected
	if ( pShip->zone() == NULL )
		return false;			// ship isn't attached to the universe
	if ( pShip->isDestroyed() )
		return false;			// ship is destroyed
	if ( pShip->teamId() != teamId() )
		return false;			// ship is captured

	return true;
}

bool GameDocument::validateSpawn( Noun * pSpawn )
{
	if (! isTeamValid() )
		return false;			// player has no valid fleet selected!

	if ( pSpawn == NULL )
		return false;			// invalid pointer
	if ( pSpawn->zone() == NULL )
		return false;			// not attached to the universe

	if ( WidgetCast<NounPlanet>( pSpawn ) )	
	{
		// TODO: later on, fleets will be able to use diplomacy to determine which other fleets can spawn at their shipyards
		return ((NounPlanet *)pSpawn)->factionId() == factionId() && 
			(((NounPlanet *)pSpawn)->flags() & NounPlanet::FLAG_HAS_SHIPYARD) != 0;
	}
	else if ( WidgetCast<NounJumpGate>( pSpawn ) )
	{
		NounJumpGate * pGate = (NounJumpGate *)pSpawn;

		if (! pGate->active() )
			return false;

		// allow spawning from unconnected neutral jump gates... 
		if ( (pGate->factionId() == FACTION_UNKNOWN || pGate->factionId() == FACTION_NEUTRAL) 
			&& pGate->destinationNoun() == NULL )
			return true;

		// otherwise, the gate must aligned with our faction..
		return pGate->factionId() == factionId();
	}

	return false;
}

//----------------------------------------------------------------------------
// EOF
