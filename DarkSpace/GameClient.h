/*
	GameClient.h

	This object is used to connect to an GameServer object over a Socket object
	(c)1999 Palestar, Richard Lyle
*/

#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "World/WorldClient.h"
#include "GameContext.h"
#include "NounShip.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GameClient : public WorldClient, public GameContext::GameUser
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< GameClient >		Ref;

	class DLL UserGameClient : public WorldClient::UserWorldClient
	{
	public:
		virtual void	onShipDestroyed( Noun * pKiller );
		virtual void	onShipCaptured( Noun * pCapper );
		virtual void	onShipDetached();						// called when players ship is detached
		virtual void	onOrderReceived( NounShip::Order order, Noun * pTarget, Noun * pFrom );
	};
	enum GameMessages {
		STORE_BUY_ENHANCEMENT		= 0x23,			// sent by the client to buy an ehancement
		STORE_UPDATE_CREDITS		= 0x24,			// send by the server to set the number of credits available
	};

	static bool			sm_bEnableProxyWorlds;		// if true, then other servers get proxy worlds created

	// Construction
	GameClient();
	virtual ~GameClient();

	//! WorldClient Interface
	virtual dword		version() const;
	virtual bool		prepWorld();
	virtual Noun *		validateNoun( Noun * pNoun );
	virtual void		onMessage( bool bUDP, u8 nMessage, const InStream & input );

	virtual void		setFocus( Noun * pFocus, bool bForce = false );			// set focus object
	virtual void		setTarget( Noun * pTarget, bool bForce = false );		// set target object
	virtual bool		selectSelf( const WidgetKey & a_nSpawn, 
							const WidgetKey & a_nSelf,
							const WidgetKeys & items );
	virtual void		onSetSelf( Noun * pSelf );
	virtual bool		pushChat( const char * pChat, dword nFromId = 0 );		// push message into the local chat buffer
	virtual bool		canGroupInvite( Noun * pTarget );

	virtual void		onUpdate();

	// GameClientCommand.cpp
	virtual bool		pushMetaChat( const MetaClient::Chat & a_Chat );
	virtual bool		processCommand( const char * pChat, dword nFromId = 0 );

	// WorldContext::User interface
	void				onChat( Noun * pNoun, const char * pMessage );
	void				onAttachNoun( Noun * pNoun );
	void				onDetachNoun( Noun * pNoun );

	// GameContext::GameUser interface
	void				onKill( Noun * pWho, Noun *pDead );
	void				onDestroyed( NounShip * pWho, Noun * pKiller );
	void				onDamageShip( Noun * pWho, float damage, Noun *pTarget );
	void				onCaptureShip( Noun * pWho, NounShip * pCaptured );
	void				onFriendlyFire( Noun * pWho, float damage );
	void				onPlanetsDamaged( Noun * pWho, float damage );
	void				onPlanetsCaptured( Noun * pWho, float amount );
	void				onRepair( Noun * pWho, float repair );
	void				onConstruct( Noun * pWho, float construct );
	void				onKamikaze( Noun * pWho, float damage );
	void				onSelfDestruct( NounShip * pWho );
	void				onJump( Noun * pWho );
	void				onCollidePlanet( Noun * pWho );
	void				onResourcesLost( Noun * pWho, float amount );
	void				onScout( Noun * pWho, float points );
	void				onBonusPrestige( Noun * pWho, float bonus );
	void				onCredits( Noun * pWho, int credits );
	void				onResources( Noun * pWho, int resources );

	void				onOrderReceived( NounShip * pWho, NounShip::Order order, 
							Noun * pTarget, Noun * pFrom );

	//! Accessors
	GameContext *		gameContext() const;
	GameClient *		proxyClient() const;
	int					credits() const;

	//! Mutators
	bool				buyEnhancement( const ClassKey & a_nEnhancement );

protected:
	//! Data
	int					m_nCredits;					// how many credits do we have currently
	int					m_nUpdateProxy;				// how long until we update our proxy data
	bool				m_bShutdown;				// set to true when shutting down
	Noun::wRef			m_pProxyTarget;				// what is our current proxy target object.
	GameClient *		m_pProxyClient;				// proxy client
	GameClient *		m_pProxyParent;				// if this is a proxy client, this points to our parent client
	dword				m_nProxyTimeout;

	bool				m_bServersUpdated;
	Array< MetaClient::Server >
						m_Servers;
	
	int					m_nLastTeamId;				// team ID when we joined our chat rooms, if the team ID changes then we will re-join our rooms
	dword				m_nGlobalRoomId;
	dword				m_nFactionRoomId;
	dword				m_nFleetRoomId;

	//! WorldClient interface
	virtual void		updateMeta();

	bool				updateProxies();
	bool				setProxyTarget( Noun * pTarget );
	bool				updateProxyClient();
};

//---------------------------------------------------------------------------------------------------

inline GameContext * GameClient::gameContext() const
{
	return WidgetCast<GameContext>( context() );
}

inline GameClient * GameClient::proxyClient() const
{
	return m_pProxyClient;
}

inline int GameClient::credits() const
{
	return m_nCredits;
}

#endif

//----------------------------------------------------------------------------
// EOF
