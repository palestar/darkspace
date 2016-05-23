/**
	@file GameContext.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:43:04 PM
*/

#include "GameContext.h"
#include "NounStar.h"
#include "NounNebula.h"
#include "NounJumpGate.h"
#include "NounSpawnShip.h"
#include "NounEnhancement.h"
#include "VerbSetDetectionFlags.h"
#include "VerbDistress.h"
#include "VerbOrderShip.h"
#include "DarkSpace/ShipPlatform.h"
#include "GameZone.h"
#include "DefaultLootTables.h"
#include "ProxyShip.h"

#undef PROFILE_OFF
#include "Debug/Profile.h"
#include "Debug/Assert.h"

#include <map>

//! define to non-zero to enable all detection of object on the client side..
#define ENABLE_CLIENT_DETECTION		0

//---------------------------------------------------------------------------------------------------

BEGIN_PROPERTY_LIST( GameContext, WorldContext )
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

bool		GameContext::sm_bJumpEffect = false;

//---------------------------------------------------------------------------------------------------

Constant	STAR_FIELD_PARTICLES( "STAR_FIELD_PARTICLES", 150 );
Constant	STAR_FIELD_FRONT("STAR_FIELD_FRONT", 1.0f );
Constant	STAR_FIELD_BACK("STAR_FIELD_BACK", 2500.0f );
Constant	STAR_FIELD_PARTICLE_SIZE("STAR_FIELD_PARTICLE_SIZE", 0.001f );
Constant	DISTRESS_DURATION( "DISTRESS_DURATION", 300.0f );		// how many seconds to remember the last distress from someone..

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( GameContext, WorldContext );

GameContext::GameContext() : 
	m_pGameUser( NULL ), 
	m_nStatDamagePushed( 0 ),
	m_nStatDamageAdded( 0 )
{
	// initialize the star field
	m_StarField.initialize( STAR_FIELD_PARTICLES, STAR_FIELD_FRONT, STAR_FIELD_BACK );
	m_StarField.setActive( false );		// turn it off by default..
	// set some static members
	NodeStarField::s_StarSize = STAR_FIELD_PARTICLE_SIZE;
	
	memset(&m_nPlatformCount,0,FACTION_COUNT*sizeof(int));
}

//---------------------------------------------------------------------------------------------------

bool GameContext::read( const InStream & input )
{
	memset(&m_nPlatformCount,0,FACTION_COUNT*sizeof(int));

	if (! WorldContext::read( input ) )
		return false;

	for(int i=0;i<m_Conditions.size();i++)
		m_NextConditionId = Max( m_NextConditionId, m_Conditions[i].conditionId + 1 );

	// count platforms after starting context
	for(int i=0;i<m_ZoneArray.size();i++)
	{
		NodeZone * pZone = m_ZoneArray[i];
		ASSERT( pZone );
		for(int c=0;c<pZone->childCount();c++)
		{
			ShipPlatform *pPlatform = WidgetCast<ShipPlatform>(pZone->child(c));
			if(pPlatform)
			{
				int myFleetId = pPlatform->teamId();
				if ( isTeamValid( myFleetId ) )
					++m_nPlatformCount[team( myFleetId ).factionId];

			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------

void GameContext::onAttachNoun( Noun * pNoun )
{
	WorldContext::onAttachNoun( pNoun );
	// add up platforms
	if(WidgetCast<ShipPlatform>(pNoun))
	{
		int myFleetId = pNoun->teamId();
		if ( isTeamValid( myFleetId ) )
			++m_nPlatformCount[team( myFleetId ).factionId];
	}

}

void GameContext::onDetachNoun( Noun * pNoun )
{
	if ( WidgetCast<NounShip>( pNoun ) )
		clearOrder( (NounShip *)pNoun );

	WorldContext::onDetachNoun( pNoun );

	if(WidgetCast<ShipPlatform>(pNoun))
	{
		int myFleetId = pNoun->teamId();
		if ( isTeamValid( myFleetId ) )
			--m_nPlatformCount[team( myFleetId ).factionId];
	}
}

bool GameContext::onMergeUpdate( Noun * a_pUpdate, Noun * a_pTemplate )
{
	if (! WorldContext::onMergeUpdate( a_pUpdate, a_pTemplate ) )
		return false;

	// handle merging jump gate changes so we can stop cutting and pasting jumpgates in resourcer..
	if ( WidgetCast<NounJumpGate>( a_pTemplate ) != NULL )
	{
		NounJumpGate * pTemplateJumpGate = (NounJumpGate *)a_pTemplate;
		if ( WidgetCast<NounJumpGate>( a_pUpdate ) == NULL )
		{
			LOG_ERROR( "GameContext", "Failed to merge nouns, class mismatch on NounJumpGate!" );
			return false;
		}
		NounJumpGate * pJumpGate = (NounJumpGate *)a_pUpdate;

		pJumpGate->setTeamId( pTemplateJumpGate->teamId() );
		pJumpGate->setActive( pTemplateJumpGate->active() );
		pJumpGate->setAutoActivate( pTemplateJumpGate->autoActivate() );
		pJumpGate->setDestination( pTemplateJumpGate->destination() );
		pJumpGate->setSafeZone( pTemplateJumpGate->safeZone(), pTemplateJumpGate->safeZoneDistance() );
		pJumpGate->setDuration( pTemplateJumpGate->duration(), pTemplateJumpGate->durationTicks() );
		pJumpGate->setInterval( pTemplateJumpGate->interval(), pTemplateJumpGate->intervalTicks() );
	}
	else if ( WidgetCast<NounSpawnShip>( a_pTemplate ) != NULL )
	{
		NounSpawnShip * pTemplateSpawner = (NounSpawnShip *)a_pTemplate;
		if ( WidgetCast<NounSpawnShip>( a_pUpdate ) == NULL )
		{
			LOG_ERROR( "GameContext", "Failed to merge nouns, class mismatch on NounSpawnShip!" );
			return false;
		}
		
		NounSpawnShip * pSpawner = (NounSpawnShip *)a_pUpdate;
		pSpawner->setSpawnType( pTemplateSpawner->spawnType() );
		pSpawner->setSpawnDelay( pTemplateSpawner->spawnDelay() );
		pSpawner->setSpawnArea( pTemplateSpawner->spawnArea() );
		pSpawner->setMinSpawn( pTemplateSpawner->minSpawn() );
		pSpawner->setMaxSpawn( pTemplateSpawner->maxSpawn() );
		pSpawner->setMinPlanets( pTemplateSpawner->minPlanets() );

		pSpawner->releaseSpawners();
		for(int i=0;i<pTemplateSpawner->spawnerCount();++i)
			pSpawner->addSpawner( pTemplateSpawner->spawner(i) );
	}

	flushGatePaths();

	return true;
}

bool GameContext::onMergeRemove( Noun * a_pDetach )
{
	// leave all player ships in the world alone... don't let the merge remove them. (except proxy ships)
	if ( (WidgetCast<NounShip>( a_pDetach ) != NULL && ((NounShip *)a_pDetach)->userId() != 0)
		&& WidgetCast<ProxyShip>( a_pDetach ) == NULL  )
		return true;

	// leave platforms in-game
	if ( WidgetCast<ShipPlatform>( a_pDetach ) != NULL )
		return true;

	flushGatePaths();
	return WorldContext::onMergeRemove( a_pDetach );
}

bool GameContext::start()
{
	if (! WorldContext::start() )
		return false;

	// initialize static data for all enhancements
	NounEnhancement::staticInitialize();
	// initialize our default loot tables
	DefaultLootTables::initialize();

	return true;
}

bool GameContext::stop()
{
	flushGatePaths();
	m_BuyResourceMap.clear();

	return WorldContext::stop();
}

bool GameContext::update()
{
	//PROFILE_LMESSAGE( 1, CharString().format( "Damaged Pushed: %u, Added: %u", m_nStatDamagePushed, m_nStatDamageAdded ) );
	return WorldContext::update();
}

void GameContext::updateSecond()
{
	WorldContext::updateSecond();

	if (! isSleeping() )
	{
		// update intelligence
		updateIntelligence();
		updateDistressMap();
	}
}

void GameContext::render( RenderContext & context,				// render context
						const Matrix33 & frame,					// frame of the camera within the current zone
						const Vector3 & position,
						bool bProxy /*= false*/ )	
{
	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	// set our ambient lighting...
	pDisplay->setAmbient( UNIVERSE_AMBIENT );

	// turn on ship alphas
	bool bShipSelection = NounShip::sm_bShipSelection;
	NounShip::sm_bShipSelection = false;
	// don't render the gadget meshes or structures
	bool bRenderStructures = NounStructure::sm_bRender;
	NounStructure::sm_bRender = false;

	WorldContext::render( context, frame, position, bProxy );

	// render the star field if this isn't the proxy
	if (! bProxy )
		m_StarField.render( context, frame, position );

	NounStructure::sm_bRender = bRenderStructures;
	NounShip::sm_bShipSelection = bShipSelection;
}

//---------------------------------------------------------------------------------------------------

void GameContext::setGameUser( GameUser * pUser )
{
	m_pGameUser = pUser;
}

void GameContext::setStarFieldActive( bool a_bActive )
{
	m_StarField.setActive( a_bActive );
}

NodeZone * GameContext::createZone()
{
	return new GameZone();
}

const float PATROL_DISTANCE = 1000.0f;

bool GameContext::checkObjective( const Objective & objective )
{
	NounBody * pBody = NounBody::wRef( objective.noun );
	if ( pBody == NULL || pBody->context() != this )
		return false;			// noun is missing!

	switch( objective.type )
	{
	case CONTROL:
		if ( pBody->factionId() == objective.factionId )
			return true;
		break;
	case DESTROY:
		if ( WidgetCast<NounShip>( pBody ) && ((NounShip *)pBody)->isDestroyed() )
			return true;
		break;
	case PROTECT:
		if ( WidgetCast<NounShip>( pBody ) && !((NounShip *)pBody)->isDestroyed() )
			return true;
		break;
	case ORBIT:
		{
			BaseNode * pRoot = pBody->parent();
			for(int i=0;i<pRoot->childCount();i++)
				if ( WidgetCast<NounShip>( pRoot->child(i) ) )
				{
					NounShip * pSelf = (NounShip *)pRoot->child(i);
					if ( (pSelf->factionId() == objective.factionId) && pSelf->orbiting() == pBody )
						return true;
				}
		}
		break;
	case DOCKED:
		break;
	case PATROL:
		{
			BaseNode * pRoot = pBody->parent();
			for(int i=0;i<pRoot->childCount();i++)
				if ( WidgetCast<NounShip>( pRoot->child(i) ) )
				{
					NounShip * pSelf = (NounShip *)pRoot->child(i);
					if ( (pSelf->factionId() == objective.factionId) && 
						(pSelf->worldPosition() - pBody->worldPosition()).magnitude() < PATROL_DISTANCE )
						return true;
				}
		}
		break;
	}

	return false;
}

void GameContext::updateDistressMap()
{
	const dword DISTRESS_DURATION_TICKS = (dword)(DISTRESS_DURATION * TICKS_PER_SECOND);
	for( DistressMap::iterator iList = m_DistressMap.begin(); iList != m_DistressMap.end(); ++iList )
	{
		DistressList & list = iList->second;
		// update our distress list, remove old distress signals...
		for( DistressList::iterator iDistress = list.begin(); iDistress != list.end(); )
		{
			Distress * pDistress = *iDistress;	
			if ( (pDistress->m_nTick + DISTRESS_DURATION_TICKS) > tick() )
				break;

			iDistress = list.erase( iDistress );		// distress has expired...
		}
	}
}

bool GameContext::onVerbDistress( VerbDistress * a_pDistress )
{
	Noun * pWho = a_pDistress->target();
	if (! pWho )
		return false;
	
	int nFactionId = pWho->factionId();
	DistressList & list = m_DistressMap[ nFactionId ];

	// search current distress signals before we push another record..
	for( DistressList::iterator iDistress = list.begin(); iDistress != list.end(); ++iDistress )
	{
		Distress::Ref pDistress = *iDistress;
		if ( pWho == pDistress->m_pWho && a_pDistress->m_Type == (VerbDistress::Type)pDistress->m_nType )
			return true;		// we already have this distress signal.. so just return
	}

	Distress::Ref pDistress = new Distress();
	pDistress->m_nTick = tick();
	pDistress->m_pWho = pWho;
	pDistress->m_nType = a_pDistress->m_Type;

	list.push_back( pDistress );
	return true;
}

//---------------------------------------------------------------------------------------------------

GameContext::Order * GameContext::findOrder( int a_nFactionId, NounShip::Order a_nOrder, Noun * a_pOrderTarget )
{
	if (! a_pOrderTarget )
		return NULL;
	FactionOrderMap::iterator iFactionOrderMap = m_FactionOrderMap.find( a_nFactionId );
	if ( iFactionOrderMap == m_FactionOrderMap.end() )
		return NULL;
	OrderListMap & orderMap = iFactionOrderMap->second;
	OrderListMap::iterator iOrderList = orderMap.find( a_pOrderTarget->key() );
	if ( iOrderList == orderMap.end() )
		return NULL;
	OrderList & list = iOrderList->second;
	for( OrderList::iterator iOrder = list.begin(); iOrder != list.end(); ++iOrder )
	{
		Order * pOrder = *iOrder;
		if ( pOrder->m_nOrder == a_nOrder )
			return pOrder;
	}

	return NULL;
}

GameContext::Order * GameContext::getOrder( int a_nFactionId, NounShip::Order a_nOrder, Noun * a_pOrderTarget )
{
	Order::Ref pOrder = findOrder( a_nFactionId, a_nOrder, a_pOrderTarget );
	if ( pOrder )
		return pOrder;

	pOrder = new GameContext::Order();
	pOrder->m_nTick = tick();
	pOrder->m_nOrder = a_nOrder;
	pOrder->m_pOrderTarget = a_pOrderTarget;

	// push the order into the linked list of orders..
	m_FactionOrderMap[ a_nFactionId ][ a_pOrderTarget->key() ].push_back( pOrder );

	return pOrder;
}

bool GameContext::trackOrder( NounShip * a_pShip, NounShip::Order a_nOrder, Noun * a_pOrderTarget )
{
	NounShip::Ref pShip = a_pShip;
	if (! pShip.valid() )
		return false;

	// grab a reference to our target, just in case it goes away..
	Noun::Ref pTarget( a_pOrderTarget );
	// remove any previous order tracking for this ship ...
	clearOrder( pShip );

	// find the Order object for tracking all orders for the given target..
	Order * pOrder = getOrder( pShip->factionId(), a_nOrder, pTarget );
	if (! pOrder )
		return false;

	pOrder->m_nScore += pShip->value();
	pOrder->m_ShipList.push_back( pShip );
	m_ShipOrderMap[ pShip->key() ] = pOrder;

	return true;
}

bool GameContext::clearOrder( NounShip * a_pShip )
{
	int nFactionId = a_pShip->factionId();

	ShipOrderMap::iterator iShipOrder = m_ShipOrderMap.find( a_pShip->key() );
	if ( iShipOrder == m_ShipOrderMap.end() )
		return false;

	Order::Ref pOrder = iShipOrder->second;
	pOrder->m_nScore -= a_pShip->value();
	if ( pOrder->m_nScore < 0 )
		pOrder->m_nScore = 0;
	pOrder->m_ShipList.remove( a_pShip );

	m_ShipOrderMap.erase( iShipOrder );

	if ( pOrder->m_ShipList.size() == 0 )
	{
		OrderList & list = m_FactionOrderMap[ nFactionId ][ pOrder->m_pOrderTarget->key() ];
		for( OrderList::iterator iOrder = list.begin(); iOrder != list.end(); ++iOrder )
		{
			Order * pOrder = *iOrder;
			if ( pOrder->m_nOrder == pOrder->m_nOrder )
			{
				list.erase( iOrder );
				break;
			}
		}

		if ( list.size() == 0 && pOrder->m_pOrderTarget.valid() )
			m_FactionOrderMap[ nFactionId ].erase( pOrder->m_pOrderTarget->key() );
	}

	return true;
}

GameContext::BuyMap & GameContext::buyMap()
{
	return m_BuyResourceMap;
}

bool GameContext::onVerbOrderShip( VerbOrderShip * a_pOrder )
{
	NounShip::Ref pShip = WidgetCast<NounShip>( a_pOrder->target() );
	if (! pShip.valid() )
		return false;

	Noun::Ref pTarget = a_pOrder->m_pTarget.pointer();
	if (! pTarget.valid() )
		return false;

	trackOrder( pShip, a_pOrder->m_nOrder, pTarget );
	return true;
}

void GameContext::updateInvalidJumpgates()
{
	std::list< WorldContext * > worlds;
	worlds.push_back( this );

	while( worlds.size() > 0 )
	{
		WorldContext * pUpdate = worlds.front();
		worlds.pop_front();

		for(int i=0;i<pUpdate->worldCount();++i)
			worlds.push_back( pUpdate->world(i) );

		for(int i=0;i<pUpdate->zoneCount();++i)
		{
			NodeZone * pZone = pUpdate->zone(i);
			for(int j=0;j<pZone->childCount();++j)
			{
				NounJumpGate * pGate = WidgetCast<NounJumpGate>( pZone->child(j) );
				if ( pGate != NULL && pGate->destinationInvalid() )
					pGate->updateDestination( true );
			}
		}
	}
}

void GameContext::detachAllShips()
{
	std::list< Noun::Ref > remove;
	for(int i=0;i<zoneCount();++i)
	{
		NodeZone * pZone = zone(i);
		for(int j=0;j<pZone->childCount();++j)
		{
			NounShip * pShip = WidgetCast<NounShip>( pZone->child(j) );
			if ( pShip != NULL )
				remove.push_back( pShip );
		}
	}

	while( remove.size() > 0 )
	{
		remove.front()->detachSelf();
		remove.pop_front();
	}
}

NounJumpGate * GameContext::findGatePath( Noun * a_pWho, Noun * a_pDestination )
{
	float fJumpDistance = (a_pWho->worldPosition() - a_pDestination->worldPosition()).magnitude2();

	// initialize our maps if not initialized...
	if ( m_GateVector.size() == 0 )
	{
		for(int i=0;i<=m_Worlds.size();++i)
		{
			WorldContext * pContext = i == 0 ? this : m_Worlds[i-1];

			// find all gates, adding nodes for all of them..
			for(int z=0;z<pContext->zoneCount();++z)
			{
				NodeZone * pZone = pContext->zone( z );
				for(int k=0;k<pZone->childCount();++k)
				{
					NounJumpGate * pGate = WidgetCast<NounJumpGate>( pZone->child(k) );
					if (! pGate )
						continue;
					m_GateVector.push_back( pGate );

					Noun * pSystem = pGate->orbitRoot();
					if (! pSystem )
						continue;
					if ( m_SystemNodeMap.find( pSystem ) != m_SystemNodeMap.end() )
						continue;		// already added system to the graph..

					m_SystemNodeMap[ pSystem ] = m_GateGraph.addNode( pSystem );
				}
			}
		}

		// now link the systems together using the gates to establish links..
		for( u32 i=0;i<m_GateVector.size();++i)
		{
			NounJumpGate * pGate = m_GateVector[i];
			
			Noun * pStartSystem = pGate->orbitRoot();
			if (! pStartSystem )
				continue;
			SystemNodeMap::iterator iStart = m_SystemNodeMap.find( pStartSystem );
			if ( iStart == m_SystemNodeMap.end() )
				continue;		// failed to find the starting node..

			NounGame * pGateDest = WidgetCast<NounGame>( pGate->destinationNoun() );
			if (! pGateDest )
				continue;

			Noun * pEndSystem = pGateDest->orbitRoot();
			if (! pEndSystem || pEndSystem == pStartSystem )
				continue;		// don't link gates within the same systems..

			SystemNodeMap::iterator iEnd = m_SystemNodeMap.find( pEndSystem );
			if ( iEnd == m_SystemNodeMap.end() )
				continue;		// failed to find the ending node

			// link the systems together.. 
			m_GateGraph.addLink( iStart->second, iEnd->second );
			TRACE( CharString().format( "GP: Linking system %s to system %s", pStartSystem->name(), pEndSystem->name() ) );
		}
	}

	Noun *			pDestSystem = NULL;
	NounJumpGate *	pDestGate = NULL;
	float			fDestGate = 0.0f;
	int				nDestNode = -1;

	// firstly, find gate closest to our destination...
	for(u32 i=0;i<m_GateVector.size();++i)
	{
		NounJumpGate * pGate = m_GateVector[i];

		float fDistance = (pGate->worldPosition() - a_pDestination->worldPosition()).magnitude2();
		if ( fDistance > fJumpDistance )
			continue;		// distance from this gate to our destination is further than the distance if we jumped directly..

		if ( fDistance < fDestGate || pDestGate == NULL )
		{
			fDestGate = fDistance;
			pDestGate = pGate;
			pDestSystem = pDestGate->orbitRoot();
		}
	}

	if ( pDestGate == NULL || pDestSystem == NULL )
		return NULL;		// no gate found..
	SystemNodeMap::iterator iDestSystem = m_SystemNodeMap.find( pDestSystem );
	if ( iDestSystem == m_SystemNodeMap.end() )
		return NULL;		// didn't find the system in the map
	nDestNode = iDestSystem->second;

	// firstly, find closest gate.. this is only so we can get the current star system... 
	NounJumpGate *	pClosestGate = NULL;
	float			fClosestGateDistance = 0.0f;

	for(u32 i=0;i<m_GateVector.size();++i)
	{
		NounJumpGate * pGate = m_GateVector[i];

		float fDistance = (pGate->worldPosition() - a_pWho->worldPosition()).magnitude2();
		if ( fDistance < fClosestGateDistance || pClosestGate == NULL )
		{
			pClosestGate = pGate;
			fClosestGateDistance = fDistance;
		}
	}

	if (! pClosestGate )
		return NULL;		// found no gate.. this shouldn't happen..

	// now, find a gate close to our ship that will get us to our desintation the fastest...
	Noun *			pStartSystem = pClosestGate->orbitRoot();
	NounJumpGate *	pStartGate = NULL;
	int				nStartNode = -1;

	SystemNodeMap::iterator iStart = m_SystemNodeMap.find( pStartSystem );
	if ( iStart == m_SystemNodeMap.end() )
		return NULL;
	nStartNode = iStart->second;

	// look to see if we already have a path calculated.. no need to do the A* more than once.
	u64 nPathHash = ((u64)nDestNode << 32) | (u64)nStartNode;
	GatePathMap::iterator iPath = m_GatePathMap.find( nPathHash );
	if ( iPath == m_GatePathMap.end() )
	{
		// make the entry in the map, so we will avoid future A* checks..
		Array< int > & path = m_GatePathMap[ nPathHash ];
		iPath = m_GatePathMap.find( nPathHash );

		// no path found, so do an A* to find the way.. this is the expensive part.
		m_GateGraph.findPath( nStartNode, nDestNode, path );
	}

	Array< int > & path = iPath->second;
	if ( path.size() < 1 )
		return NULL;	// found no path from start to end.. 

	// get the next system in our path to our destination...
	Noun *			pNextSystem = m_GateGraph.node( path[0] ).Object;
	NounJumpGate *	pNextGate = NULL;
	float			fNextGateDistance = 0.0f;

	for(u32 i=0;i<m_GateVector.size();++i)
	{
		NounJumpGate * pGate = m_GateVector[i];

		NounGame * pDestination = WidgetCast<NounGame>( pGate->destinationNoun() );
		if (! pDestination )
			continue;
		if ( pDestination->orbitRoot() != pNextSystem )
			continue;		// gate doesn't lead to the system we want..
		float fDistance = (pGate->worldPosition() - a_pWho->worldPosition()).magnitude2();
		if ( fDistance > fJumpDistance )
			continue;		// distance to gate is further than jumping directly..
		float fTotalDistance = fDistance + fDestGate;
		if ( fTotalDistance > fJumpDistance )
			continue;		// combined distance is greater, so skip this gate..

		if ( fDistance < fNextGateDistance || pNextGate == NULL )
		{
			pNextGate = pGate;
			fNextGateDistance = fDistance;
		}
	}
	
	if ( pNextGate == pDestGate )
		return NULL;		// if we get the same gate, then we're just better off jumping directly to our destination..

	return pNextGate;
}

void GameContext::flushGatePaths()
{
	m_GateVector.clear();
	m_GateGraph.release();
	m_SystemNodeMap.clear();
	m_GatePathMap.clear();
}

//---------------------------------------------------------------------------------------------------

NounTemplate * GameContext::pickShipTemplate( int nTeamId, NounShip::Type eType /*= NounShip::UNKNOWN*/ )
{
	if (! isTeamValid( nTeamId ) )
		return NULL;
	const WorldContext::Team & t = team( nTeamId );
	if (! t.pTemplates )
		return NULL;
	// find all ships on our team of the given type, then pick one at random...
	std::vector< NounTemplate * > templates;
	for(int i=0;i<t.pTemplates->resourceCount();++i)
	{
		NounTemplate::Link pTemplate = t.pTemplates->resource( i );
		if (! pTemplate.valid() )
			continue;
		NounShip * pShipTemplate = WidgetCast<NounShip>( pTemplate->noun() );
		if (! pShipTemplate )
			continue;
		if ( eType != NounShip::UNKNOWN && pShipTemplate->type() != eType )
			continue;

		templates.push_back( pTemplate );
	}
	if ( templates.size() == 0 )
		return NULL;		// no templates to choose from..

	return templates[ rand() % templates.size() ];
}

//---------------------------------------------------------------------------------------------------

const char * GameContext::objectiveText( ObjectiveType type )
{
	static const char * OBJECTIVE_TEXT[] = 
	{
		"CONTROL",
		"DESTROY",
		"PROTECT",
		"ORBIT",
		"DOCKED",
		"PATROL",
	};

	if ( type >= 0 && type < sizeof(OBJECTIVE_TEXT)/sizeof(OBJECTIVE_TEXT[0]) )
		return OBJECTIVE_TEXT[ type ];
	return "?";
}

//---------------------------------------------------------------------------------------------------

void GameContext::updateIntelligence()
{
	PROFILE_START( "GameContext::updateIntelligence()" );

	typedef std::map< NounGame::Ref, dword > DetectionHash;

	DetectionHash detectHash;	
	for(int k=0;k<lockedZoneCount();k++)
	{
		NodeZone * pZone = lockedZone( k );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounGame * pNoun = WidgetCast<NounGame>( pZone->child(i) );
			if (! pNoun )
				continue;

			if ( WidgetCast<NounShip>( pNoun ) )
			{
				NounShip * pSelf = (NounShip *)pNoun;

				int nFactionId = pSelf->factionId();
				dword nFactionBit = 1 << nFactionId;
				// all ships of the same faction can detect each other
				detectHash[ pSelf ] |= nFactionBit;
				// add ship objects
				for(int j=0;j<pSelf->childCount();j++)
				{
					NounGame * pChild = WidgetCast<NounGame>( pSelf->child(j) );
					if ( pChild != NULL && pChild->canBeDetected() )
						detectHash[ pChild ] |= nFactionBit;
				}

				// add all ship contacts to team intelligence
				for(int j=0;j<pSelf->contactCount();j++)
				{
					NounGame * pContact = WidgetCast<NounGame>( pSelf->contact(j) );
					if ( pContact != NULL )
						detectHash[ pContact ] |= nFactionBit;
				}
			}
			else if ( WidgetCast<NounPlanet>( pNoun ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pNoun;
				int nFactionId = pPlanet->factionId();
				dword nFactionBit = 1 << nFactionId;
				// always detectable, add to all faction intelligence
				detectHash[ pPlanet ] = 0xffffffff;

				// any units on the surface, make all items on the surface visible to the owning team
				for(int j=0;j<pPlanet->childCount();j++)
				{
					NounGame * pChild = WidgetCast<NounGame>( pPlanet->child(j) );
					if ( pChild != NULL )
						detectHash[ pChild ] |= nFactionBit;
				}

				// add planet intelligence to faction intel
				for(int j=0;j<pPlanet->contactCount();j++)
				{
					NounGame * pContact = WidgetCast<NounGame>( pPlanet->contact(j) );
					if ( pContact != NULL )
						detectHash[ pContact ] |= nFactionBit;
				}
			}
			else if ( WidgetCast<NounStar>( pNoun ) 
				|| WidgetCast<NounNebula>( pNoun ) )
			{
				// always detectable, add to all ship intelligence
				detectHash[ pNoun ] = 0xffffffff;
			}
			else if ( WidgetCast<NounJumpGate>( pNoun ) )
			{
				NounJumpGate * pJumpGate = (NounJumpGate *)pNoun;
				if (! pJumpGate->active() )
				{
					detectHash[ pNoun ] = 0;
					continue;										// gate is only visible when it's active
				}

				int factionId = pJumpGate->factionId();
				if ( factionId != 0 )
					detectHash[ pJumpGate ] |= (1 << factionId);	// jumpgate is always visible to it's own faction
				else	
					detectHash[ pJumpGate ] = 0xffffffff;			// neutral jumpgates are visible to everyone
			}
			else if ( pNoun->canBeDetected() )
			{
				// make sure all detectable objects are in the hash
				if ( detectHash.find( pNoun ) == detectHash.end() )
					detectHash[ pNoun ] = 0;
			}
		}
	}

	// Iterate all items in the tree, and update the detectionFlags() when needed
	for(DetectionHash::iterator i = detectHash.begin(); i != detectHash.end(); ++i )
	{
		NounGame * pNoun = i->first;
		if ( !pNoun )
			continue;

		dword nFlags = i->second;
		if ( pNoun->detectionFlags() != nFlags )
		{
#if ENABLE_CLIENT_DETECTION
			// all detection is done on each client and server, this cuts down
			// on traffic from the server telling us what objects are detected or not..
			pNoun->setDetectionFlags( nFlags );
#else
			if ( pNoun->nounType() >= TYPE_OBJECT )
			{
				// noun is either TYPE_OBJECT or TYPE_LANDMARK, only the server can
				// set the detection flags for these types of objects
				if ( isServer() )
				{
					Verb::Ref( new VerbSetDetectionFlags( pNoun, nFlags ) );
					pNoun->setDetectionFlags( nFlags );
				}
			}
			else
			{
				// noun is TYPE_EFFECT, TYPE_SERVER_OBJECT, or TYPE_LOCAL_OBJECT set the flags locally always
				pNoun->setDetectionFlags( nFlags );
			}
#endif
		}
	}

	PROFILE_END();
}

//---------------------------------------------------------------------------------------------------

void GameContext::GameUser::onKill( Noun * pWho, Noun *pDead )
{}

void GameContext::GameUser::onDestroyed( NounShip * pWho, Noun * pKiller )
{}

void GameContext::GameUser::onDamageShip( Noun * pWho, float damage, Noun *pTarget )
{}

void GameContext::GameUser::onCaptureShip( Noun * pWho, NounShip * pCaptured )
{}

void GameContext::GameUser::onFriendlyFire( Noun * pWho, float damage )
{}

void GameContext::GameUser::onPlanetsDamaged( Noun * pWho, float damage )
{}

void GameContext::GameUser::onPlanetsCaptured( Noun * pWho, float amount )
{}

void GameContext::GameUser::onRepair( Noun * pWho, float repair )
{}

void GameContext::GameUser::onConstruct( Noun * pWho, float construct )
{}

void GameContext::GameUser::onKamikaze( Noun * pWho, float damage )
{}

void GameContext::GameUser::onSelfDestruct( NounShip * pWho )
{}

void GameContext::GameUser::onJump( Noun * pWho )
{}

void GameContext::GameUser::onCollidePlanet( Noun * pWho )
{}

void GameContext::GameUser::onResourcesLost( Noun * pWho, float amount )
{}

void GameContext::GameUser::onScout( Noun * pWho, float points )
{}

void GameContext::GameUser::onBonusPrestige( Noun * pWho, float bonus )
{}

void GameContext::GameUser::onCredits( Noun * pWho, int credits )
{}

void GameContext::GameUser::onResources( Noun * pWho, int resources )
{}

void GameContext::GameUser::onOrderReceived( NounShip * pWho, NounShip::Order order, Noun * pTarget, Noun * pFrom )
{}

void GameContext::GameUser::onOrderCompleted( NounShip * pWho )
{}

void GameContext::GameUser::onOrderFailed( NounShip * pWho )
{}

int GameContext::GameUser::getHighestRank( int nFactionId )
{
	return LAST_RANK;
}

bool GameContext::GameUser::onBuildShip( NounPlanet * pPlanet, dword nUserId, NounShip * pShip, int nCost )
{
	return false;
}

bool GameContext::GameUser::onBuildCancelled( dword nUserId )
{
	return false;
}

void GameContext::adjustFactionPlatforms( int factionId, int amount )
{
	m_nPlatformCount[factionId] += amount;
}


//---------------------------------------------------------------------------------------------------
//EOF
