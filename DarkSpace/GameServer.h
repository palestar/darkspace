/*
	GameServer.h

	This object is used to route verbs for one or more clients
	(c)1999 Palestar Development, Richard Lyle
*/

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "World/WorldServer.h"
#include "GameContext.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL GameServer : public WorldServer, public GameContext::GameUser
{
public:
	DECLARE_WIDGET_CLASS();

	//! Static
	static dword		sm_nAlwaysSpawnFlags;		// what flags are needed to always allow spawning of any ship
	static bool			sm_bEnableTeamSpawners;		// if true, then team spawners are created automatically
	static bool			sm_bHiddenServer;			// if true, then server is registered as sub-game server

	// Construction
	GameServer();

	// Server interface
	virtual void		onDisconnect( dword a_nClientId );

	// WorldServer interface
	virtual dword		version() const;								// get the server version, must match the client version..
	virtual bool		update();								// returns true when running, returns false to shutdown
	virtual bool		createProfile( ClientContext & context);
	virtual bool		createProxy( ClientContext & context, const WidgetKey & a_nObjectId, int a_nFactionId );
	virtual bool		addPlayerProfile( dword nUserId, const char * pField, float fAmount );
	virtual bool		publishServer();								// send updated information back to the MetaServer
	virtual bool		prepWorld();
	virtual bool		cleanWorld();

	virtual bool		canJoinTeam( ClientContext & client, int nTeamId );
	virtual void		onJoinTeam( ClientContext & client, int nTeamId );
	virtual bool		canSpawn( ClientContext & context, Noun * pSpawnPoint, Noun * pSpawn,
							Array< Noun::Ref > & Storage );
	virtual bool		onSpawn( ClientContext & context, Noun * pSpawnPoint, Noun * pSpawn, bool bNewSpawn,
							Array< Noun::Ref > & Storage );
	virtual void		onDeleteClient( ClientContext & client );
	virtual bool		onTransferClient( ClientContext & client, const WorldClient::TransferRequest & a_Req );
	virtual bool		onStorageLoaded( ClientContext & context, UserStorage * a_pStorage );
	virtual void		onMessage( ClientContext & context, bool bUDP, byte nMessage, const InStream & input );
	virtual void		onLogin( ClientContext & client );				// invoked after client has been logged into this server
	virtual int			getNounType( Noun * pNoun );					// use to set the nType in GroupMember, should be overridden to provide a application

	// GameServerCommand.cpp
	virtual bool		processCommand( ClientContext & context, const char * pChat );

	// WorldContext::User interface
	virtual void		onAttachNoun( Noun * pNoun );
	virtual void		onDetachNoun( Noun * pNoun );
	virtual void		onCondition( int conditionId );					// called when a condition is true
	virtual bool		saveStorage( Noun * a_pSelf );
	virtual int			spawnedScore( int a_nFactionId );

	// GameContext::GameUser interface
	virtual void		onKill( Noun * pWho, Noun *pDead );
	virtual void		onDestroyed( NounShip * pWho, Noun * pKiller );
	virtual void		onDamageShip( Noun * pWho, float damage , Noun *pTarget);
	virtual void		onCaptureShip( Noun * pWho, NounShip * pCaptured );
	virtual void		onFriendlyFire( Noun * pWho, float damage );
	virtual void		onPlanetsDamaged( Noun * pWho, float damage );
	virtual void		onPlanetsCaptured( Noun * pWho, float amount );
	virtual void		onRepair( Noun * pWho, float repair );
	virtual void		onConstruct( Noun * pWho, float construct );
	virtual void		onKamikaze( Noun * pWho, float damage );
	virtual void		onSelfDestruct( NounShip * pWho );
	virtual void		onJump( Noun * pWho );
	virtual void		onCollidePlanet( Noun * pWho );
	virtual void		onResourcesLost( Noun * pWho, float amount );
	virtual void		onScout( Noun * pWho, float points );
	virtual void		onBonusPrestige( Noun * pWho, float bonus );
	virtual void		onCredits( Noun * pWho, int credits );
	virtual void		onResources( Noun * pWho, int resources );

	virtual void		onOrderReceived( NounShip * pWho, NounShip::Order order, 
							Noun * pTarget, Noun * pFrom );
	virtual int			getHighestRank( int a_nFactionId );
	virtual bool		onBuildShip( NounPlanet * pPlanet, dword nUserId, 
							NounShip * pShip, int nCost );
	virtual bool		onBuildCancelled( dword nUserId );

	// Accessors
	GameContext *		gameContext() const;

	int					getTopFactionScore() const;							// returns the highest score among all the factions
	int 				getFactionScore( int a_nFactionId ) const;			// returns the score of the given faction
	float				getFactionBalance( int a_nFactionId ) const;		// based on team balance, 1.0f (Most players) - 0.0f (Least Players)
																		
	// Mutators
	bool				addProfile( Noun * pTarget,	Statistics nStat, float fValue );
	bool				addProfile( dword nUserId, Statistics nStat, float fValue );

	bool				saveStorage( dword nUserId );
	int					getStorageCargoCount( dword nUserId );
	bool				storeAllyShips( NounShip * pShip );
	bool				auditShip( int a_nFactionID, NounShip * a_pShip, NounShip * a_pShipTemplate );
	bool				pickSafeSpawnPosition( Noun * pSpawnPoint, NounShip * pSpawn );
	
	void				buyEnhancement( ClientContext & context, const ClassKey & ck );

	void				updateFactionBalanceMap();

protected:
	//! Types
	typedef std::map< dword, int >		ShipQueueMap;
	typedef std::map< int, int >		FactionScoreMap;

	//! Data
	ShipQueueMap		m_ShipQueueMap;
	int					m_nTopFaction;
	FactionScoreMap		m_FactionScoreMap;
	dword				m_nNextShipStatLog;
};

//---------------------------------------------------------------------------------------------------

inline GameContext * GameServer::gameContext() const
{
	return (GameContext *)worldContext();
}

inline int GameServer::getTopFactionScore() const
{
	return m_nTopFaction;
}

inline int GameServer::getFactionScore( int a_nFactionId ) const
{
	FactionScoreMap::const_iterator iScore = m_FactionScoreMap.find( a_nFactionId );
	if ( iScore != m_FactionScoreMap.end() )
		return iScore->second;
	return 0;
}

inline float GameServer::getFactionBalance( int a_nFactionId ) const
{
	if ( m_nTopFaction > 0 )
	{
		int nFactionScore = getFactionScore( a_nFactionId );
		if ( nFactionScore < m_nTopFaction )
		{
			// scale linearly using the top faction score.. so if the score of the top faction is twice that
			// of this faction score, then the scale will be 0.5
			return Clamp<float>( (float)nFactionScore / (float)m_nTopFaction, 0.0f, 1.0f );
		}
	}

	return 1.0f;
}

//-------------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
// EOF
