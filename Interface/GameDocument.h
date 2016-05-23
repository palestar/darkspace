/*
	GameDocument.h

	This object is used to connect to an UniverseServer object over a Socket object
	(c)1999 Palestar, Richard Lyle
*/

#ifndef GAME_DOCUMENT_H
#define GAME_DOCUMENT_H

#include "Audio/Jukebox.h"
#include "Standard/Event.h"
#include "Network/Client.h"
#include "Gui3d/Document.h"
#include "Gui3d/NodeWindow.h"
#include "World/Noun.h"
#include "DarkSpace/NounPlanet.h"
#include "World/Verb.h"
#include "DarkSpace/GameClient.h"
#include "DarkSpace/GameProfile.h"
#include "Interface/Helpers.h"
#include "Interface/InterfaceDll.h"

//----------------------------------------------------------------------------

class DLL GameDocument : public Document, public GameClient::UserGameClient
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef NounShip::Order				Order;
	typedef GameClient::ServerStatus	ServerStatus;

	enum GameState {
		GS_LOBBY,
		GS_MAIN,					// main menu
		GS_GAME,					// playing the game
		GS_CONNECT_SERVER,
		GS_SELECT_SHIP,				
		GS_SERVER_TRANSFER,			// transferring client to another server
		GS_DEATH,	
		GS_END_GAME,				// scenario has been won, showing end game stats
		GS_DISCONNECTED,
	};

	// Construction
	GameDocument();
	virtual	~GameDocument();

	// Document interface
	void				onActivate();
	void				onLock();
	void				onPreUpdate();
	void				onUpdate( float t );
	void				onUnlock();

	bool				onMessage( const Message & msg );
	
	// GameClient::UserGameClient interface
	virtual void		onConnected();							// connection established with the server
	virtual void		onLogin( bool bSuccess);				// called when client is logged into server
	virtual void		onUpdateQueuePosition();				// called anytime the queue position has changed
	virtual void		onMetaLogin();
	virtual void		onContextReceived();					// called when all game data has been received, textures should now be pre-cached usually
	virtual void		onContextEnd( const char * pMessage );
	virtual void		onDisconnected();						// lost connection to the server

	virtual void		onWorldTransform( const Vector3 & transform );
	virtual void		onServerTransfer( const WorldClient::TransferRequest & a_Req );		// server is redirection this client to a different server
	virtual void		onProfile( Profile * pProfile );		// server sent updated profile info
	virtual void		onServerStatus( ServerStatus & status);	// server sent updated server status info

	virtual void		onChat( const char * pMessage, dword nFromId );		// called when chat is received
	virtual void		onFlushChat();							// called before chat buffer is cleared

	virtual void		onVerb( Verb * pVerb );					// called when verb is received, after it is already pushed into the context

	virtual void		onSetFocus( Noun * pFocus );
	virtual void		onSetFocusArea( float fFocusArea );
	virtual void		onSetTarget( Noun * pTarget );
	virtual void		onSetSelf( Noun * pShip );
	virtual void		onSetTeam( int teamId );

	virtual void		onReceiveStorage( UserStorage * a_pStorage );
	virtual void		onAddStorage( const WidgetKey & nKey );
	virtual void		onDeleteStorage( const WidgetKey & nKey );

	virtual void		onGroupInvite( Noun * a_pWho );			// invoked when invited to a group, groupInvite() should be called to accept. 
	virtual void		onGroupUpdate( Group * a_pGroup );		// invoked when the group changes
	virtual void		onGroupLeave();							// called when the local player leaves their group

	virtual void		onShipDestroyed( Noun * pKiller );
	virtual void		onShipCaptured( Noun * pCapper );
	virtual void		onShipDetached();

	// Accessors
	bool				isConnected() const;				// true if we are connected to a server
	dword				sessionId() const;					// our sessionId
	GameClient *		client() const;						// get the client object
	Noun *				cursorTarget() const;				// get the current cursor target
	Noun *				lastSpawn() const;					// get our last spawn point

	Jukebox *			jukeBox() const;

	GameProfile *		profile() const;					// get our profile
	bool				isMod() const;						// are we a moderator
	bool				isAdmin() const;					// are we an admin
	bool				isDev() const;						// are we a developer
	int					rank() const;						// get player rank, with maxRank() in effect
	dword				badges() const;						// get our badges/medals

	dword				tick() const;
	GameContext *		context() const;					// get the universe context

	int					factionId() const;					// our factionId
	int					teamId() const;						// the players fleet
	Noun *				target() const;
	Noun *				rootTarget() const;					// of the current target, get the root object
	NounGadget *		gadgetTarget() const;				// from the current target, is it a gadget
	NounGadget *		myGadgetTarget() const;				// if the current target is a gadget on the players ship it returns the object
	NounPlanet *		planetTarget() const;				// from the current target, get a planet

	Noun *				focus() const;
	NounShip *			ship() const;

	const char *		leftHint() const;					// these return NULL if no hint is set
	const char *		rightHint() const;

	// Mutators
	void				setGameState( GameState a_nState );
	bool				connect( dword sessionId, const char * address, 
							int port );						// connect to this server

	void				setCursorTarget( Noun * pTarget );	// set the current cursor target
	void				setLastSpawn( Noun * pTarget );
	void				setSpawnTarget( Noun * pSpawn );
	void				setTarget( Noun * pTarget );
	void				setFocus( Noun * pFocus );
	void				setLeftHint( const char * pHint );
	void				setRightHint( const char * pHint );

	void				sendChat( const char * pMessage );
	void				pushChat( const char * pMessage );
	void				flushChat();

	Noun *				validateNoun( Noun * pNoun );		// make sure noun is still attached to the universe

	// Helpers
	bool				isTeamValid();						// have we selected a valid fleet
	bool				isShipValid();						// do we currently have a valid ship in-game

	bool				validateShip( NounShip * pShip );
	bool				validateSpawn( Noun * pSpawn );

	// Static
	static float		s_fCamera;							// static camera information
	static Vector3		s_vCamera;
	static Matrix33		s_mCamera;

private:
	// Data
	GameState			m_GameState;						// our current game state
	bool				m_bIsConnected;						// true if we are connected
	dword				m_SessionId;						// our sessionID
	GameClient *		m_pClient;							// Gameclient object

	Noun::wRef			m_pCursorTarget;					// current cursor target
	Noun::wRef			m_pLastSpawn;

	Jukebox *			m_pJukeBox;			// music
	float				m_CheckMusicState;

	bool				m_EndGame;
	bool				m_bShipLost;

	CharString			m_sLeftHint;
	CharString			m_sRightHint;
};

//----------------------------------------------------------------------------

inline bool GameDocument::isConnected() const
{
	return m_bIsConnected;
}

inline dword GameDocument::sessionId() const
{
	return m_SessionId;
}

inline GameClient * GameDocument::client() const
{
	return m_pClient;
}

inline Noun * GameDocument::cursorTarget() const
{
	return m_pCursorTarget;
}

inline Noun * GameDocument::lastSpawn() const
{
	return m_pLastSpawn;
}

inline Jukebox * GameDocument::jukeBox() const
{
	return m_pJukeBox;
}

inline GameProfile * GameDocument::profile() const
{
	return WidgetCast<GameProfile>( m_pClient->profile() );
}

inline bool GameDocument::isMod() const
{
	return (profile()->flags() & MetaClient::MODERATOR) != 0;
}

inline bool GameDocument::isAdmin() const
{
	return (profile()->flags() & MetaClient::ADMINISTRATOR) != 0;
}

inline bool GameDocument::isDev() const
{
	return (profile()->flags() & MetaClient::DEVELOPER) != 0;
}

inline int GameDocument::rank() const
{
	int nMaxRank = LAST_RANK;
	
	if ( m_pClient != NULL )
	{
		nMaxRank = m_pClient->maxRank();

		// check if our current target is on another server, we will need to use the rank from that server
		// in that case..
		Noun * pTarget = m_pClient->target();
		if ( pTarget != NULL && pTarget->context() != m_pClient->context() )
		{
			GameClient * pProxyClient = m_pClient->proxyClient();
			if ( pProxyClient != NULL && pProxyClient->context() == pTarget->context() )
				nMaxRank = pProxyClient->maxRank();
		}
	}

	return Min<int>( ((GameProfile *)profile())->calculateRank(), nMaxRank );
}

inline dword GameDocument::badges() const
{
	return ((GameProfile *)profile())->calculateBadges();
}

inline dword GameDocument::tick() const
{
	return m_pClient->tick();
}

inline GameContext * GameDocument::context() const
{
	return (GameContext *)m_pClient->context();
}

inline int GameDocument::factionId() const
{
	return m_pClient->factionId();
}

inline int GameDocument::teamId() const
{
	return m_pClient->teamId();
}

inline Noun * GameDocument::target() const
{
	return m_pClient->target();
}

inline Noun * GameDocument::focus() const
{
	return m_pClient->focus();
}

inline NounShip * GameDocument::ship() const
{
	return WidgetCast<NounShip>( m_pClient->self() );
}

inline const char * GameDocument::leftHint() const
{
	if ( m_sLeftHint[0] != 0 )
		return m_sLeftHint;
	return NULL;
}

inline const char * GameDocument::rightHint() const
{
	if ( m_sRightHint[0] != 0 )
		return m_sRightHint;
	return NULL;
}

//----------------------------------------------------------------------------

inline void GameDocument::setCursorTarget( Noun * pTarget )
{
	m_pCursorTarget = pTarget;
}

inline void GameDocument::setLastSpawn( Noun * pTarget )
{
	m_pLastSpawn = pTarget;
}

inline void	GameDocument::setTarget( Noun * pTarget )
{
	m_pClient->setTarget( pTarget );
}

inline void GameDocument::setFocus( Noun * pFocus )
{
	m_pClient->setFocus( pFocus );
}

inline void GameDocument::setLeftHint( const char * pHint )
{
	if ( pHint != NULL )
		m_sLeftHint = pHint;
	else
		m_sLeftHint.empty();
}

inline void GameDocument::setRightHint( const char * pHint )
{
	if ( pHint != NULL )
		m_sRightHint = pHint;
	else
		m_sRightHint.empty();
}

//----------------------------------------------------------------------------

inline Noun * GameDocument::validateNoun( Noun * pNoun )
{
	return m_pClient->validateNoun( pNoun );
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
