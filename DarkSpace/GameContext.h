/**
	@file GameContext.h
	@brief DarkSpace specific implementation of the WorldContext object.

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:41:34 PM
*/

#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "Standard/Graph.h"
#include "World/WorldContext.h"
#include "Render3D/NodeStarField.h"

#include "Constants.h"
#include "NounShip.h"
#include "NounJumpGate.h"
#include "DarkSpace/Constants.h"

#include <list>
#include <vector>
#include <map>

#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class VerbDistress;
class VerbOrderShip;
class NounJumpGate;

class DLL GameContext : public WorldContext
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< GameContext >		Ref;

	enum ContextType
	{
		SCENERIO,			
		COMBAT,
		CLAN,
		METAVERSE,
		TUTORIAL,
	};
	enum ObjectiveType
	{
		CONTROL,			// true if target is controlled by the team
		DESTROY,			// true if target is destroyed
		PROTECT,			// true if target is alive
		ORBIT,				// true if team member is orbiting the target
		DOCKED,				// true if team member is docked with the target
		PATROL,				// true if team member is near the target
	};

	class DLL GameUser 
	{
	public:
		virtual void	onKill( Noun * pWho, Noun *pDead );
		virtual void	onDestroyed( NounShip * pWho, Noun * pKiller );
		virtual void	onDamageShip( Noun * pWho, float damage, Noun *pTarget );
		virtual void	onCaptureShip( Noun * pWho, NounShip * pCaptured );
		virtual void	onFriendlyFire( Noun * pWho, float damage );
		virtual void	onPlanetsDamaged( Noun * pWho, float damage );
		virtual void	onPlanetsCaptured( Noun * pWho, float amount );
		virtual void	onRepair( Noun * pWho, float repair );
		virtual void	onConstruct( Noun * pWho, float construct );
		virtual void	onKamikaze( Noun * pWho, float damage );
		virtual void	onSelfDestruct( NounShip * pWho );
		virtual void	onJump( Noun * pWho );
		virtual void	onCollidePlanet( Noun * pWho );
		virtual void	onResourcesLost( Noun * pWho, float amount );
		virtual void	onScout( Noun * pWho, float points );
		virtual void	onBonusPrestige( Noun * pWho, float bonus );
		virtual void	onCredits( Noun * pWho, int credits );
		virtual void	onResources( Noun * pWho, int resources );

		virtual void	onOrderReceived( NounShip * pWho, NounShip::Order order, 
							Noun * pTarget, Noun * pFrom );
		virtual void	onOrderCompleted( NounShip * pWho );
		virtual void	onOrderFailed( NounShip * pWho );

		virtual int		getHighestRank( int nFactionId );

		virtual bool	onBuildShip( NounPlanet * pPlanet, 
							dword nUserId, NounShip * pShip, int nCost );
		virtual bool	onBuildCancelled( dword nUserId );
	};

	typedef std::list< NounShip::Ref >			ShipList;

	// this record is used to track distress signals sent within a given faction...
	struct Distress : public Referenced
	{
		Distress() : 
			m_nTick( 0 ),
			m_nType( 0 )
		{}

		typedef Reference< Distress >		Ref;

		dword				m_nTick;			// tick when distress was recorded
		Noun::wRef			m_pWho;				// who sent the distress
		int					m_nType;			// type of distress
	};
	typedef std::list< Distress::Ref >		DistressList;
	typedef std::map< int, DistressList >	DistressMap;		

	struct Order : public Referenced
	{
		Order() :
			m_nTick( 0 ),
			m_nScore( 0 )
		{}

		typedef Reference< Order >			Ref;

		dword				m_nTick;
		NounShip::Order		m_nOrder;
		Noun::Ref			m_pOrderTarget;
		int					m_nScore;			
		ShipList			m_ShipList;			
	};
	typedef std::list< Order::Ref >				OrderList;
	typedef std::map< WidgetKey, OrderList >	OrderListMap;
	typedef std::map< int, OrderListMap >		FactionOrderMap;
	typedef std::map< WidgetKey, Order::Ref >	ShipOrderMap;
	typedef std::set< WidgetKey >				TransportSet;
	typedef std::map< WidgetKey, TransportSet >	BuyMap;

	//! Configuration
	static bool					sm_bJumpEffect;

	// Construction
	GameContext();

	// Widget interface
	bool						read( const InStream & input );

	// WorldContext interface
	virtual void				onAttachNoun( Noun * pNoun );
	virtual void				onDetachNoun( Noun * pNoun );
	virtual bool				onMergeUpdate( Noun * a_pUpdate, Noun * a_pTemplate );
	virtual bool				onMergeRemove( Noun * a_pDetach );

	virtual bool				start();										// start this context, should be called before update()
	virtual bool				stop();											// stop/pause this context
	virtual bool				update();										// update all locked zones
	virtual void				updateSecond();									// called by update() every second

	virtual void				render( RenderContext & context,				// render context
									const Matrix33 & frame,						// frame of the camera within the current zone
									const Vector3 & position,					// position of the camera within the current zone
									bool bProxy = false );						// if proxy, then this is rendered from another context

	virtual NodeZone *			createZone();
	virtual bool				checkObjective( const Objective & objective );	// returns true if the specific object is completed

	// Accessors
	GameUser *					gameUser() const;
	int							factionPlatforms( int factionId ) const;		// number of platforms owned by this faction
	void						adjustFactionPlatforms( int factionId,
									int amount );							// adjust platform numbers
	
	// Mutators
	void						setGameUser( GameUser * pUser );
	void						setStarFieldActive( bool a_bActive );			// turn on or off the star field
		
	NodeStarField &				getStarField();
	DistressMap &				getDistressMap();								// get a mutable map of all generated distress signals
	DistressList &				getDistressList( int a_nFactionId );
	void						updateDistressMap();							// removes old and invalid distress signals from the map
	bool						onVerbDistress( VerbDistress * a_pDistress );

	FactionOrderMap &			getFactionOrderMap();
	OrderListMap &				getOrderListMap( int a_nFactionId );
	Order *						findOrder( int a_nFactionId, NounShip::Order a_nOrder, Noun * a_pOrderTarget );
	Order *						getOrder( int a_nFactionId, NounShip::Order a_nOrder, Noun * a_pOrderTarget );
	bool						trackOrder( NounShip * a_pShip, NounShip::Order a_nOrder, Noun * a_pOrderTarget );
	bool						clearOrder( NounShip * a_pShip );

	//! This map is used to track tranports enroute to a planet, the NounShip::findBuyTarget() uses this map
	//! to prevent multiple transports trying to get to the same planet at the same time.
	BuyMap &					buyMap();

	bool						onVerbOrderShip( VerbOrderShip * a_pOrder );

	//! Called to update any invalid jumpgates
	void						updateInvalidJumpgates();
	//! Call to remove all ships from this context
	void						detachAllShips();
	//! This finds a jump gate to use to get to noun to the destination, if a direct route is best it will return NULL. 
	NounJumpGate *				findGatePath( Noun * a_pWho, Noun * a_pDestination );
	void						flushGatePaths();		

	//! Helpers
	NounTemplate *				pickShipTemplate( int nTeamId, NounShip::Type eType = NounShip::UNKNOWN );

	//! Static
	static const char *			objectiveText( ObjectiveType type );

	// GameContextScript.cpp
	static int					isStar( lua_State * pScript );
	static int					isAsteroid( lua_State * pScript );
	static int					isNebula( lua_State * pScript );
	static int					isBody( lua_State * pScript );
	static int					isUnit( lua_State * pScript );
	static int					isPlanet( lua_State * pScript );
	static int					isOrbiting( lua_State * pScript );
	static int					enterOrbit( lua_State * pScript );
	static int					breakOrbit( lua_State * pScript );
	static int					isBeaconed( lua_State * pScript );
	static int					canDetect( lua_State * pScript );
	static int					getAllNouns( lua_State * pScript );	// getAllNouns( nounId, range ); returns a table 1..n
	static int					getAllNounsRelative( lua_State * pScript );	// getAllNounsRelative( shipId, range ); returns a table 1..n
	// PLANET FUNCTIONS
	static int					planetCount( lua_State * pScript );		// planetCount();
	static int					getPlanet( lua_State * pScript );			// getPlanet( n );
	static int					getPlanetMorale( lua_State * pScript );
	static int					getPlanetPopulation( lua_State * pScript );
	static int					setPlanetPopulation( lua_State * pScript ); // setPlanetPopulation( planet, population );
	static int					getPlanetResources( lua_State * pScript );
	static int					setPlanetResources( lua_State * pScript ); 	// setPlanetResources( planet, resources );
	static int					getPlanetTech( lua_State * pScript );
	static int					setPlanetTech( lua_State * pScript ); 		// setPlanetTech( planet, tech );	
	static int					getPlanetValue( lua_State * pScript );
	static int					getPlanetFlags( lua_State * pScript );
	static int					planetEvent( lua_State * pScript );
	static int					revoltPlanet( lua_State * pScript );
	static int					hexCount( lua_State * pScript );
	static int					getHex( lua_State * pScript );
	static int					getHexPosition( lua_State * pScript );
	static int					structureCount( lua_State * pScript );
	static int					getStructure( lua_State * pScript );
	static int					getStructureHex( lua_State * pScript );
	static int					getStructureHull( lua_State * pScript );
	static int					setStructureHull( lua_State * pScript );
	static int					isStructureActive( lua_State * pScript );
	static int					isStructurePaused( lua_State * pScript );
	static int					pauseStructure( lua_State * pScript );
	static int					isStructureFinished( lua_State * pScript );
	static int					unitCount( lua_State * pScript );
	static int					getUnit( lua_State * pScript );
	static int					getUnitHex( lua_State * pScript );
	static int					getRandomPlanet( lua_State * pScript );
	static int					getFriendlyPlanet( lua_State * pScript );
	static int					getEnemyPlanet( lua_State * pScript );
	static int					setAllegiance( lua_State * pScript );
	static int					setAllegianceXP( lua_State * pScript );
	// SHIP FUNCTIONS
	static int					isShip( lua_State * pScript );
	static int					isShipDestroyed( lua_State * pScript );
	static int					shipCount( lua_State * pScript );		// shipCount();
	static int					getShip( lua_State * pScript );			// getShip( n );
	static int					getHull( lua_State * pScript );
	static int					setHull( lua_State * pScript );
	static int					getSignature( lua_State * pScript );
	static int					useGadget( lua_State * pScript );			// useGadget( ship, key, target, shift );
	static int					playersShip( lua_State * pScript );			// playersShip();
	static int					getShipType( lua_State * pScript );
	static int					spawnShip( lua_State * pScript );		// spawnShip( resource, where, name, team );
	static int					orderShip( lua_State * pScript );		// orderShip( name, target, order );
	static int					getShipOrder( lua_State * pScript );		// getShipOrder( name );
	static int					isJumping( lua_State * pScript );
	static int					isAIControlled( lua_State * pScript );
	static int					getEnergy( lua_State * pScript );
	static int					getFacingForNoun( lua_State * pScript );
	static int					enableShipAI( lua_State * pScript );
	static int					controlShipAbsolute( lua_State * pScript ); // controlShipAbsolute( shipId, heading, velocity );
	static int					controlShipRelative( lua_State * pScript ); // controlShipRelative( shipId, yaw, velocity );
	static int					getShipVelocity( lua_State * pScript ); // getShipVelocity( shipId );
	static int					getShipHeading( lua_State * pScript ); // getShipHeading( shipId );
	static int					getShipYaw( lua_State * pScript ); // getShipYaw( shipId );
	// GADGET FUNCTIONS
	static int					isGadget( lua_State * pScript );
	static int					gadgetCount( lua_State * pScript );
	static int					getGadget( lua_State * pScript );
	static int					getGadgetType( lua_State * pScript );
	static int					getGadgetMount( lua_State * pScript );
	static int					setGadgetMount( lua_State * pScript );
	static int					getGadgetAmmo( lua_State * pScript );
	static int					setGadgetAmmo( lua_State * pScript );
	static int					hasGadgetAmmo( lua_State * pScript );
	static int					useGadget2( lua_State * pScript );
	static int					isGadgetActive( lua_State * pScript );
	static int					isGadgetUsable( lua_State * pScript );
	static int					beamPointDefenceEnabled( lua_State * pScript );
	static int					setBeamPointDefence( lua_State * pScript );
	static int					getJumpTarget( lua_State * pScript );
	static int					getShielding( lua_State * pScript );
	static int					setShielding( lua_State * pScript );
	// UNIT FUNCTIONS
	static int					getUnitExperience( lua_State * pScript );
	static int					setUnitExperience( lua_State * pScript );
	static int					getUnitOrder( lua_State * pScript );
	static int					setUnitOrder( lua_State * pScript );
	static int					jumpgateCount( lua_State * pScript );
	static int					getJumpgate( lua_State * pScript );
	static int					getRandomJumpgate( lua_State * pScript );
	static int					getFriendlyJumpgate( lua_State * pScript );
	static int					setSafeZones( lua_State * pScript );

	// WorldContext interface
	virtual void				registerFunctions( lua_State * pScript );

private:
	//! Types
	typedef std::vector< NounJumpGate::Ref >		GateVector;
	typedef Graph< Noun * >							GateGraph;				// graph of all gates and their connections
	typedef std::map< Noun *, int >					SystemNodeMap;			// Noun * to node index inside the GateGraph..
	typedef std::map< u64, Array< int > >			GatePathMap;			// map of found gate paths

	// Data
	GameUser *					m_pGameUser;
	DistressMap					m_DistressMap;				// map of distress signals by FactionID
	FactionOrderMap				m_FactionOrderMap;
	ShipOrderMap				m_ShipOrderMap;
	BuyMap						m_BuyResourceMap;

	NodeStarField				m_StarField;				// star field object

	dword						m_nStatDamagePushed;
	dword						m_nStatDamageAdded;

	int							m_nPlatformCount[FACTION_COUNT];			// total number of platforms in this context by faction
	
	GateVector					m_GateVector;
	GateGraph					m_GateGraph;
	SystemNodeMap				m_SystemNodeMap;
	GatePathMap					m_GatePathMap;

	//! Mutators
	void						updateIntelligence();
};

IMPLEMENT_RAW_STREAMING( GameContext::ContextType );
IMPLEMENT_RAW_STREAMING( GameContext::ObjectiveType );

//---------------------------------------------------------------------------------------------------

inline GameContext::GameUser * GameContext::gameUser() const
{
	return m_pGameUser;
}

inline int GameContext::factionPlatforms( int factionId ) const
{
	return m_nPlatformCount[factionId];
}

//---------------------------------------------------------------------------------------------------

inline NodeStarField & GameContext::getStarField()
{
	return m_StarField;
}

inline GameContext::DistressMap & GameContext::getDistressMap()
{
	return m_DistressMap;
}

inline GameContext::DistressList & GameContext::getDistressList( int a_nFactionId )
{
	return m_DistressMap[ a_nFactionId ];
}

inline GameContext::FactionOrderMap & GameContext::getFactionOrderMap()
{
	return m_FactionOrderMap;
}

inline GameContext::OrderListMap & GameContext::getOrderListMap( int a_nFactionId )
{
	return m_FactionOrderMap[ a_nFactionId ];
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
