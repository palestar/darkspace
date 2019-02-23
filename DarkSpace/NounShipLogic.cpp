/*
	NounShipLogic.cpp

	This file contains all the logic code embedded into a ship for use by the AI
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Debug/Profile.h"
#include "Debug/Log.h"
#include "Math/Helpers.h"
#include "VerbEnterOrbit.h"
#include "VerbOrderShip.h"
#include "VerbCommandShip.h"
#include "VerbUnload.h"
#include "VerbLoad.h"
#include "VerbDistress.h"
#include "VerbBuildStructure.h"
#include "VerbTradeRoute.h"

#include "NounShip.h"
#include "NounUnit.h"
#include "NounBeacon.h"
#include "NounStructure.h"
#include "NounJumpGate.h"
#include "NounJumpTarget.h"
#include "NounNebula.h"
#include "NounStar.h"
#include "NounProjectile.h"
#include "NounDropTarget.h"
#include "GadgetBeamWeapon.h"
#include "GadgetReload.h"
#include "GadgetBeacon.h"
#include "GadgetCloak.h"
#include "GadgetBuild.h"
#include "GadgetECCM.h"
#include "GadgetECM.h"
#include "GadgetELF.h"
#include "StructurePort.h"
#include "StructureHub.h"
#include "StructureMIHub.h"
#include "StructureBarracks.h"
#include "StructurePower.h"
#include "StructurePort.h"
#include "StructureDome.h"
#include "StructureFarm.h"
#include "StructureFactory.h"
#include "StructureMine.h"
#include "StructureLab.h"
#include "StructureDefense.h"
#include "StructureDepot.h"
#include "StructureShipyard.h"
#include "StructureShield.h"
#include "resource.h"
#include "GameContext.h"

//---------------------------------------------------------------------------------------------------

// define to non-zero trace all trade logic
#define ENABLE_TRADE_LOG		0

#if ENABLE_TRADE_LOG
#define TRADE_LOG( msg )		Logging::report( __FUNCTION__, __LINE__, LL_DEBUG_MED, "TRADE", msg )
#else
#define TRADE_LOG( msg )		static_cast<void>(0)
#endif

#define ENABLE_AI_LOG			1

#if ENABLE_AI_LOG
#define AI_LOG( msg )			Logging::report( __FUNCTION__, __LINE__, LL_DEBUG_MED, "AI", msg )
#else
#define AI_LOG( msg )
#endif

//---------------------------------------------------------------------------------------------------

extern int			MAX_RESOURCES;		// defined in CargoResource.cpp

static Constant		AI_CPU_USAGE( "AI_CPU_USAGE", 0.75f );									// what percent of the CPU can be used by the AI
static Constant		SHIP_AI_RATE( "SHIP_AI_RATE", TICKS_PER_SECOND * 5 );					// updateOrder() is called every SHIP_AI_RATE ticks
static Constant		MAX_SHIP_ORBIT_DISTANCE( "MAX_SHIP_ORBIT_DISTANCE", 150.0f );
static Constant		SHIP_ORBIT_VELOCITY( "SHIP_ORBIT_VELOCITY", 0.0f );						// orbit velocity of ship when it enters orbit
static Constant		SHIP_FOLLOW_DISTANCE( "SHIP_FOLLOW_DISTANCE", 50.0f );					// what range to we try to move within when following					
static Constant		SHIP_SHADOW_DISTANCE( "SHIP_SHADOW_DISTANCE", 1000.0f );	
static Constant		SHIP_EVADE_DISTANCE( "SHIP_EVADE_DISTANCE", 200.0f);					// what range do we begin evading our target
static Constant		SHIP_ENGAGE_DISTANCE( "SHIP_ENGAGE_DISTANCE", 75.0f );					// how close to move when engaging a target
static Constant		SHIP_FIND_RANGE( "SHIP_FIND_RANGE", 250000.0f );
static Constant		SHIP_DEFAULT_SPEED( "SHIP_DEFAULT_SPEED", 1.0f );
static Constant		SHIP_ENGAGE_SPEED( "SHIP_ENGAGE_SPEED", 0.6f );

static Constant		FIND_ATTACK_TARGET_DISTANCE( "FIND_ATTACK_TARGET_DISTANCE", 5000000.0f );
static Constant		FIND_ENEMY_DISTANCE_VALUE("FIND_ENEMY_DISTANCE_VALUE", 0.05f );			// findEnemyShip() uses this to determine the value of distance versus other factors
static Constant		MAX_ORDER_TIME( "ORDER_TIME", TICKS_PER_SECOND * 600 );
static Constant		MAX_ALLY_TIME( "MAX_ALLY_TIME", TICKS_PER_SECOND * 3600 );				// how long do ally ships stick around
static Constant		MAX_ORDER_RANGE( "MAX_ORDER_RANGE", 2500000.0f );						// maximum range we will return a positive score for a ship

static Constant		USE_JUMP_DISTANCE( "USE_JUMP_DISTANCE", 2000.0f );					// when moving to a target, at what range do we use the jump drives
static Constant		DEFAULT_SHIP_BRAVERY( "DEFAULT_SHIP_BRAVERY", 1.0f );
static Constant		FIND_ENEMY_SHIP_RANGE( "FIND_ENEMY_SHIP_RANGE", 5000.0f );
static Constant		DEFEND_ENEMY_SHIP_RANGE( "DEFEND_ENEMY_SHIP_RANGE", 750.0f );
static Constant		DEFEND_SHADOW_RANGE( "DEFEND_SHADOW_RANGE", 200.0f );

//! How often will an AI ship send a distress when being attacked...
static Constant		SHIP_AI_ATTACK_DISTRESS( "SHIP_AI_ATTACK_DISTRESS", 300.0f );
//! How often will an AI ship request resupply...
static Constant		SHIP_AI_SUPPLY_DISRESS( "SHIP_AI_SUPPLY_DISRESS", 300.0f );
// How does distance affect the value of a target ... the distance of the target is multiplied
// by this value then subtracted from the target value... the further away an object is, the lower
// it's value.
static Constant		DISTANCE_VALUE_SCALE( "DISTANCE_VALUE_SCALE", 0.5f );
//! Values scales
static Constant		BLOCKADE_VALUE_SCALE( "BLOCKADE_VALUE_SCALE", 1.5f );
static Constant		SHIPYARD_VALUE_SCALE( "SHIPYARD_VALUE_SCALE", 2.0f );
static Constant		CAPTURE_VALUE_SCALE( "CAPTURE_VALUE_SCALE", 10.0f );

//! At what percent does a ship need to refuel jump drives
static Constant		NEED_FUEL_PERCENT( "NEED_FUEL_PERCENT", 0.30f );		// 30%
//! When will needAmmo() return true...
static Constant		NEED_AMMO_PERCENT( "NEED_AMMO_PERCENT", 0.30f );		// 30%
//! What is the minimum distance of a planet to be considered for reconnaissance..
static Constant		MIN_RECON_DISTANCE( "MIN_RECON_DISTANCE", 10000.0f );
//! How close does a ship need to be to complete the recon order
static Constant		RECON_DISTANCE( "RECON_DISTANCE", 500.0f );
//! How close does a ship need to be to complete the move order
static Constant		MOVE_DISTANCE( "MOVE_DISTANCE", 200.0f );
//! used by findUnits() to calculate the value of a planet...
static Constant		UNIT_VALUE( "UNIT_VALUE", 1000.0f );
//! How many units should be left to defend a planet..
static Constant		MIN_PLANET_UNITS( "MIN_PLANET_UNITS", 20.0f );
//! How often to pick a random evade vector in seconds.
static Constant		EVADE_VECTOR_INTERVAL( "EVADE_VECTOR_INTERVAL", 10.0f );
//! What is the chance we will jump to intercept our engage target
static Constant		JUMP_INTERCEPT_CHANCE( "JUMP_INTERCEPT_CHANCE", 20.0f );
//! How many seconds ahead of our target should we jump in seconds..
static Constant		JUMP_INTERCEPT_TIME( "JUMP_INTERCEPT_TIME", 10.0f );
//! How far away should our target be before we consider jumping to intercept
static Constant		MIN_INTERCEPT_DISTANCE( "MIN_INTERCEPT_DISTANCE", 600.0f );
//! How low does the hull need to get before we consider an emergency jump
static Constant		EMERGENCY_JUMP_DAMAGE( "EMERGENCY_JUMP_DAMAGE", 0.05f );
//! Chance that we will actually try to jump out..
static Constant		EMERGENCY_JUMP_CHANCE( "EMERGENCY_JUMP_CHANCE", 25.0f );
//! Min number of resources on a planet, trade order will not buy if resources are below this amount
static Constant		MIN_PLANET_RES( "MIN_PLANET_RES", 15000.0f );
//! How far to look for another planet to trade with..
static Constant		MAX_TRADE_ROUTE_RANGE( "MAX_TRADE_ROUTE_RANGE", 500000.0f );
//! Converts damage into threat, used by the AI to figure out how much of a threat another ship is ..
static Constant		DAMAGE_THREAT( "DAMAGE_THREAT", 15000.0f );
//! much threat damage decays per second..
static Constant		THREAT_DECAY_RATE("THREAT_DECAY_RATE", 0.01f );

//---------------------------------------------------------------------------------------------------

template<typename T>
T * FindChild( BaseNode * a_pNode )
{
	for(int i=0;i<a_pNode->childCount();++i)
	{
		BaseNode * pChild = a_pNode->child(i);
		if ( pChild->factory()->classKey() == T::classKey() )
			return (T *)a_pNode->child(i);
	}
	return NULL;
}

template<typename T>
int CountChildren( BaseNode * a_pNode )
{
	int nCount = 0;
	for(int i=0;i<a_pNode->childCount();++i)
	{
		if ( WidgetCast<T>( a_pNode->child(i) ) != NULL )
			++nCount;
	}
	return nCount;
}

static float RealDistanceFromTarget( NounShip * pSelf, Noun * pTarget )
{
	float fDistance = (pSelf->worldPosition() - pTarget->worldPosition()).magnitude();
	fDistance -= pSelf->radius();
	fDistance -= pTarget->radius();

	return fDistance;
}

//---------------------------------------------------------------------------------------------------

bool NounShip::isOrderValid( Order nOrder, Noun * pTarget, NounShip * pFrom )
{
	switch( nOrder )
	{
	case NOORDER:			// no order
	case HOLD:				// stay at current position
		break;
	case ATTACK:			// attack target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounShip>( pTarget ) == NULL && 
			WidgetCast<NounPlanet>( pTarget ) == NULL )
			return false;
		if ( pFrom->isFriend( pTarget ) )
			return false;
		break;
	case DEFEND:			// defend target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounShip>( pTarget ) == NULL && 
			WidgetCast<NounPlanet>( pTarget ) == NULL )
			return false;
		break;
	case CAPTURE:			// capture target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounPlanet>( pTarget ) == NULL && 
			WidgetCast<NounShip>( pTarget ) == NULL )
			return false;	// only ships/planets can be captured
		if ( pFrom->isFriend( pTarget ) )
			return false;
		break;
	case MOVE:				// move to target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		break;
	case RECON:
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounPlanet>( pTarget ) == NULL )
			return false;	// target is not a planet
		break;
	case RELOAD:			// reload/repair target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounShip>( pTarget ) == NULL )
			return false;		// target is not a ship
		if ( !pFrom->isFriend( pTarget ) )
			return false;
		break;
	case BEACON:			// attach beacon to target
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounStructure>( pTarget ) == NULL && 
			WidgetCast<NounShip>( pTarget ) == NULL )
			return false;	// only ships/planets can be captured
		if ( NounBeacon::findBeacon( pTarget, pFrom ) != NULL )
			return false;
		if ( pFrom->isFriend( pTarget ) )
			return false;
		break;
	case TRADE:				// transfer resources
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounPlanet>( pTarget ) == NULL )
			return false;	// target is not a planet
		if ( (((NounPlanet *)pTarget)->flags() & NounPlanet::FLAG_HAS_PORT) == 0 )
			return false;	// target has no port
		if ( !pFrom->isFriend( pTarget ) )
			return false;
		break;
	case BUILD:
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounPlanet>( pTarget ) == NULL )
			return false;	// target is not a planet
		if ( !pFrom->isFriend( pTarget ) )
			return false;	// planet is enemy/neutral
		break;
	case FALLBACK:
		if (! pFrom->validateTarget( pTarget ) )
			return false;
		if ( !pFrom->isFriend( pTarget ) )
			return false;
		break;		
	}

	return true;
}

//----------------------------------------------------------------------------

// returns a score which can be used to determine how well this ship can accomplish that order, any scores zero or below should be not be assigned
float NounShip::shouldOrder( Order nOrder, Noun * pTarget, NounShip * pFrom )
{
	if ( pFrom == NULL )
		return 0;
	if (! canOrder( nOrder, pTarget,  pFrom ) )
		return 0;

	switch( nOrder )
	{
	case NOORDER:			// no order
	case HOLD:				// stay at current position
		return 0;
	case ATTACK:			// attack target
		{
			// make sure this ship is a combat ship
			if ( type() < SCOUT &&  type() > DREAD )
				return 0;		// this ship is not a combat ship, do not give it attack orders!
			if (! canAttack( pTarget ) )
				return 0;		// this ship doesn't have the weapons to attack this target

			float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
			// first, calculate score based on distance to this target
			float fDistanceScore = 1.0f - Min(fDistance / MAX_ORDER_RANGE, 1.0f);
			// adjust score based on target
			float fTargetScore = scoreTarget( pTarget );

			return fDistanceScore * fTargetScore;
		}
		break;
	case DEFEND:			// defend target
		{
			// make sure this ship is a combat ship
			if ( type() < SCOUT && type() > DREAD )
				return 0;		// this ship is not a combat ship, do not give it defend orders!

			float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
			// first, calculate score based on distance to this target
			float fDistanceScore = 1.0f - Min(fDistance / MAX_ORDER_RANGE, 1.0f);
			// adjust score based on target
			float fTargetScore = scoreTarget( pTarget );

			return fDistanceScore * fTargetScore;
		}
		break;
	case CAPTURE:			// capture target
	case TRADE:				// transfer resources
		{
			float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
			// first, calculate score based on distance to this target
			float fDistanceScore = 1.0f - Min(fDistance / MAX_ORDER_RANGE, 1.0f);
			float fTargetScore = scoreTarget( pTarget );
			float fCargoScore = Clamp( maxCargo() / 20.0f, 0.0f, 1.0f);

			return fDistanceScore * fTargetScore * fCargoScore;
		}
		break;
	case MOVE:				// move to target
	case RELOAD:			// reload/repair target
	case BUILD:
		{
			float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
			// first, calculate score based on distance to this target
			float fDistanceScore = 1.0f - Min(fDistance / MAX_ORDER_RANGE, 1.0f);

			return fDistanceScore;
		}
	case BEACON:			// attach beacon to target
	case RECON:
		{
			if ( type() != SCOUT )
				return 0;

			float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();
			// first, calculate score based on distance to this target
			float fDistanceScore = 1.0f - Min(fDistance / MAX_ORDER_RANGE, 1.0f);
			float fTargetScore = scoreTarget( pTarget );

			return fDistanceScore * fTargetScore;
		}
		break;
	}

	return 0;
}

bool NounShip::canOrder( NounShip * pFrom )
{
	if ( (flags() & FLAG_AI_ALLY) != 0 && m_pHome != pFrom && pFrom != this )
		return false;	// if we are a AI ally, then our orders can only come the same player or ourselves
	if ( userId() == 0 && (flags() & FLAG_AI_ALLY) == 0 && pFrom != this  )
		return false;	// disable ordering AI ships that are not your ally ships..
	if ( pFrom == NULL )
		return false;	// invalid pointers
	if ( this == pFrom && pFrom->userId() != 0  )
		return false;	// don't allow orders to self, unless it's AI
	if ( pFrom->rank() < rank() )
		return false;	// not enough rank
	if (! isFriend( pFrom ) )
		return false;	// not the same fleet

	// check if we have an existing order from a higher/equal ranking player, do not allow the order then...
	// allow the order if it's from the same player, a higher ranking player, or from this ship
	if ( m_Order != NounShip::NOORDER 
		&& validateNoun( m_pOrderFrom )
		&& m_pOrderFrom != pFrom 
		&& pFrom != this 
		&& m_pOrderFrom->rank() >= pFrom->rank() )
	{
		// current order is from a higher ranking player, cannot change this order
		return false;
	}

	return true;
}

bool NounShip::canOrder( Order nOrder, Noun * pTarget, NounShip * pFrom, bool bNewOrder /*= true*/ )
{
	if (! canOrder( pFrom ) )
		return false;
	if ( userId() == 0 && pTarget != NULL && pTarget->context() != context() )
		return false;		// don't allow AI to be order accross server bounds	
	if ( bNewOrder && m_Order == nOrder && pTarget == m_OrderTarget )
		return false;		// this is already our current order
	if (! isOrderValid( nOrder, pTarget, pFrom ) )
		return false;

	// check that the order is valid for this ship
	switch( nOrder )
	{
	case RELOAD:			// reload/repair target
		if ( GadgetReload::getGadgetReload( this ) == NULL )
			return false;
		break;
	case BEACON:			// attach beacon to target
		if ( GadgetBeacon::getGadgetBeacon( this ) == NULL )
			return false;
		break;
	case BUILD:
		if ( GadgetBuild::getGadgetBuild( this ) == NULL )
			return false;	// ship has no build device
		break;
	}

	return true;
}

void NounShip::verbOrder( Order order, Noun * pTarget )
{
	Verb::Ref( new VerbOrderShip( this, order, pTarget, this ) );
}

void NounShip::verbOrder( Order order, Noun * pTarget, NounShip * pFrom )
{
	Verb::Ref( new VerbOrderShip( this, order, pTarget, pFrom ) );
}

void NounShip::setOrder( Order order, const Noun::tRef & pTarget, const NounShip::wRef & pFrom )
{
	m_Order = order;
	m_nOrderTime = 0;
	m_OrderTarget = pTarget;
	m_pOrderFrom = pFrom;
	m_nOrderState = 0;
}

bool NounShip::canSendOrder( Order nOrder, Noun * pTarget )
{
	return isOrderValid( nOrder, pTarget, this );
}

NounShip * NounShip::sendOrder( Order nOrder, Noun * pTarget )
{
	NounShip * pOrderShip = NULL;

	// find the best ship to send this order too
	Queue< NounShip * > fleet;

	// find all ships in the universe in our fleet
	GameContext * pContext = (GameContext *)context();
	ASSERT( pContext );
	for(int i=0;i<pContext->zoneCount();i++)
	{
		NodeZone * pZone = pContext->zone(i);
		for(int j=0;j<pZone->childCount();j++)
		{
			NounShip * pShip = WidgetCast<NounShip>( pZone->child(j) );
			if ( pShip != NULL && isFriend( pShip ) )
				fleet.push( pShip );
		}
	}

	float fBestScore = 0.0f;
	while( fleet.valid() )
	{
		NounShip * pShip = *fleet;
		fleet.next();
		
		if ( pShip == this || pShip == pTarget )
			continue;		// skip self or objective of order
		if ( pShip->order() != NounShip::NOORDER && pShip->orderFrom() == this )
			continue;		// skip ships that already have an order from this ship

		float fScore = pShip->shouldOrder( nOrder, pTarget, this );
		if ( fScore > fBestScore )
		{
			pOrderShip = pShip;
			fBestScore = fScore;
		}
	}

	if ( pOrderShip != NULL )
	{
		message( CharString().format( "<color;ffffff>Tactical: Order Sent, %s %s %s.", 
			pOrderShip->name(), orderText( nOrder ), pTarget ? pTarget->name() : "") );

		Verb::Ref( new VerbOrderShip( pOrderShip, nOrder, pTarget, this ) );
	}
	else
		message( "<color;ffffff>Tactical: No ships available for order." );

	return pOrderShip;
}

NounShip::Order NounShip::bestOrder( Noun * pTarget )
{
	if ( pTarget == NULL )
		return NounShip::NOORDER;

	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();
	
	Noun * pRoot = pTarget->zoneParent();
	if ( WidgetCast<NounPlanet>( pRoot ) )
	{
		if ( isFriend( pTarget ) )
		{
			if ( GadgetBuild::getGadgetBuild( this ) != NULL )
				return NounShip::BUILD;

			return NounShip::DEFEND;
		}
		else
			return NounShip::CAPTURE;
	}
	else if ( WidgetCast<NounShip>( pRoot ) )
	{
		if ( isFriend( pTarget ) )
		{
			if ( GadgetReload::getGadgetReload( this ) != NULL )
				return NounShip::RELOAD;
			return NounShip::DEFEND;
		}
		else
			return NounShip::ATTACK;
	}
	else if ( WidgetCast<NounTarget>( pTarget ) )
		return NounShip::MOVE;

	return NounShip::NOORDER;
}

//----------------------------------------------------------------------------

bool NounShip::canCommand( Command nCommand, Noun * pTarget )
{
	switch( nCommand )
	{
	case ORBIT:
	case REFUEL:
		if (! validateTarget( pTarget ) )
			return false;		// needs a target
		if ( pTarget == this )
			return false;		// cannot orbit ourselves
		if (! VerbEnterOrbit::canOrbit( this, pTarget ) )
			return false;		// cannot orbit this object
		return true;
	case SHADOW:
		if (! validateTarget( pTarget ) )
			return false;
		if ( WidgetCast<NounShip>( pTarget ) == NULL )
			return false;
		if ( pTarget == this )
			return false;
		return true;
	case FOLLOW:
	case EVADE:
		if (! validateTarget( pTarget ) )
			return false;
		if ( pTarget == this )
			return false;
		if(pTarget->parent() == this) // if we are carrying this object
			return false;
		return true;
	case ENGAGE:
		if (! validateTarget( pTarget ) )
			return false;
		if ( isFriend( pTarget ) )
			return false;
		if ( pTarget == this )
			return false;
		return true;
	case STOP:
	case NOCOMMAND:
		return true;
	}

	return false;
}

void NounShip::verbCommand( Command command, Noun * pTarget )
{
	if ( canCommand( command, pTarget ) && !isCommand( command, pTarget ) )
		Verb::Ref( new VerbCommandShip( this, command, pTarget ) );
}

bool NounShip::isCommand( Command nCommand, Noun * pTarget )
{
	return m_Command == nCommand && pTarget == m_CommandTarget;
}

bool NounShip::setCommand( Command nCommand, const Noun::tRef & pTarget )
{
	m_Command = nCommand;
	m_CommandTarget = pTarget;
	return true;
}

NounPlanet * NounShip::isTargetPlanet( Noun * pTarget )
{
	if ( WidgetCast<NounStructure>( pTarget ) )
		pTarget = ((NounStructure *)pTarget)->planet();
	else if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();

	return WidgetCast<NounPlanet>( pTarget );
}

//---------------------------------------------------------------------------------------------------

bool NounShip::updateLogic( dword nTick )
{
	PROFILE_FUNCTION();

	if (! isLocal() )
		return false;		// not controlled locally, nothing to do in here..
	if ( nTick < m_nLogicTick )
		return false;		// not time to update yet..
	m_nLogicTick = nTick + SHIP_AI_RATE;

	if ( m_Order != NOORDER )
		m_nOrderTime += SHIP_AI_RATE;

	bool bAlly = (flags() & FLAG_AI_ALLY) != 0;
	if ( bAlly )
	{
		m_nAllyTime += SHIP_AI_RATE;

		if ( m_nAllyTime > MAX_ALLY_TIME )
		{
			addFlags( FLAG_NO_BEST_ORDER );
			clearFlags( FLAG_AI_ALLY );
			setRank( 255 );		
			setHome( NULL );
			verbOrder( NOORDER, NULL );
		}
	}

	if ( m_Order != NOORDER )
	{
		// check the order, set the ship to NOORDER if needed
		if ( !canOrder( m_Order, m_OrderTarget, this, false ) 
			|| ((flags() & FLAG_NO_EXPIRE_ORDER) == 0 && m_nOrderTime > MAX_ORDER_TIME) )
		{
			// end the order
			verbOrder( NOORDER, NULL );
		}
	}

	if ( userId() != 0 )
		return false;		// player so no more logic needs to be done..
	// if CPU is high, skip any additional AI logic for now..
	if ( Profiler::CPUused() >= AI_CPU_USAGE )
		return false;

	// if we have been attacked, consider other options than following our current order..
	if ( m_bAttacked )
	{
		m_bAttacked = false;		// clear the flag

		// check the ship that is attacking us, consider running or attacking it..
		NounShip * pAttacker = WidgetCast<NounShip>( m_pLastAttacker );
		if ( pAttacker != NULL && isEnemy( pAttacker ) )
		{
			// get our current hull damage, 1.0 = none, 0.0 = dead
			float fDamage = damageRatioInv();
			// TODO: We should also consider shielding and armor damage too...
			// > 1.0 then the ship is more powerful than me, less < 1.0 the ship is weaker
			float fComparison = float( pAttacker->value() ) / (value() * fDamage);	
			if ( fComparison > 1.0f )
			{
				// consider falling back... the better the other ship, the more likely we'll retreat...
				int nFallbackChance = static_cast<int>( (fDamage * 100) - (5 * fComparison) );
				if ( (rand() % 100) > nFallbackChance )
				{
					Noun * pTarget = findDepot();
					if ( !pTarget )
						pTarget = findPlanet( true );

					if ( pTarget )
						verbOrder( FALLBACK, pTarget );
				}
			}
		}
	}

	bool bOrderValid = false;

	// update high-level AI
	switch( m_Order )
	{
	case NOORDER:
		if (! findBestOrder() )
			returnHome();
		bOrderValid = true;
		break;
	case ATTACK:
		bOrderValid = updateAttackOrder();
		break;
	case DEFEND:
		bOrderValid = updateDefendOrder();
		break;
	case CAPTURE:
		bOrderValid = updateCaptureOrder();
		break;
	case RECON:
		bOrderValid = updateReconOrder();
		break;
	case MOVE:
		bOrderValid = updateMoveOrder();
		break;
	case RELOAD:
		bOrderValid = updateReloadOrder();
		break;
	case BEACON:
		bOrderValid = updateBeaconOrder();
		break;
	case HOLD:
		bOrderValid = updateHoldOrder();
		break;
	case TRADE:
		bOrderValid = updateTradeOrder();
		break;
	case BUILD:
		bOrderValid = updateBuildOrder();
		break;
	case FALLBACK:
		bOrderValid = updateFallbackOrder();
		break;
	}

	// cancel the order if it's no longer valid..
	if ( !bOrderValid && m_Order != NOORDER )
		verbOrder( NOORDER, NULL );

	// update the AI on all gadgets...
	for(int i=0;i<m_Gadgets.size();i++)
		m_Gadgets[i]->updateLogic();

	return true;
}

bool NounShip::updateAttackOrder()
{
	if ( needAmmo() || needFuel() )
		return false;		// if we need more ammo or fuel, stop the attack
	Noun * pTarget = m_OrderTarget;
	if (! validateTarget( pTarget ) )
		return false;
	if ( WidgetCast<NounPlanet>( pTarget ) )
	{
		NounPlanet * pPlanet = (NounPlanet * )pTarget;
		if ( pPlanet->population() == 0 )
			return false;
		if ( pPlanet->unitCount() == 0 && pPlanet->structureCount() == 0 )
			return false;		// everything is destroyed, stop attacking..
	}
	if ( m_Command != EVADE && m_Command != ENGAGE )
		verbCommand( ENGAGE, pTarget );
	return true;
}

bool NounShip::updateDefendOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;

	float fDistance = (worldPosition() - pTarget->worldPosition()).magnitude();

	NounShip * pEnemy = findEnemyShip( DEFAULT_SHIP_BRAVERY, DEFEND_ENEMY_SHIP_RANGE );
	if ( pEnemy != NULL && fDistance < DEFEND_ENEMY_SHIP_RANGE )
	{
		if ( m_Command != EVADE && m_Command != ENGAGE )
			verbCommand( ENGAGE, pEnemy );
	}
	else if ( orbiting() != pTarget )
	{
		if (! canOrbit( pTarget ) )
		{
			if ( fDistance > DEFEND_SHADOW_RANGE )
				verbCommand( FOLLOW, pTarget );
			else
				verbCommand( SHADOW, pTarget );
		}
		else
			verbCommand( ORBIT, pTarget );
	}

	return true;
}

bool NounShip::updateCaptureOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;
	if ( isFriend( pTarget ) )
		return false;
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( pTarget );
	if( pPlanet == NULL )
		return false;
	if (! pPlanet->isCappable() )
		return false;

	// m_bLoadUnits is set to true when we have no units to drop on our target planet..
	if ( m_bLoadUnits )
	{
		if ( m_Command == ORBIT )
			return true;		// in route to planet to orbit...

		// first, check if we're in orbit around a friendly planet.. 
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( orbiting() );
		if ( validateTarget( pPlanet ) && isFriend( pPlanet ) )
		{
			int nUnits = pPlanet->friendlyUnitCount();
			int nMinUnits = (int)MIN_PLANET_UNITS;

			// find units on the planet we are orbiting and load them...
			for(int i=0;i<pPlanet->childCount() && nUnits > nMinUnits;++i)
			{
				NounUnit * pUnit = WidgetCast<NounUnit>( pPlanet->child(i) );
				if ( canLoad( pUnit ) )
				{
					verbLoad( pUnit );
					nUnits -= 1;
				}
			}

			// unload anything that's not a unit onto the planet..
			for(int i=0;i<childCount();++i)
			{
				NounCargo * pCargo = WidgetCast<NounCargo>( child(i) );
				if (! pCargo )
					continue;
				
				verbUnload( pPlanet, pCargo );
			}
			
			if ( podCount() > 0 )
				return true;

			// no pod in transit, either we are full of units or the planet has none.. 
			if ( unitCount() > 0 && availableCargo() == 0 )
			{
				// we have some units, so proceed to drop phase..
				m_bLoadUnits = false;
				return true;
			}
			// we have no units on board, fall through and search for a planet with units..
		}

		// find a planet with ground units and proceed to load..
		pPlanet = findUnits();
		if ( validateTarget( pPlanet ) )
		{
			verbCommand( ORBIT, pPlanet );
			return true;
		}

		// no friendly planet with units is available..
		if ( unitCount() > 0 )
		{
			// we didn't find a planet, but we have some units so proceed to drop them on some planet..
			m_bLoadUnits = false;
			return true;
		}
		return false;
	}

	int nDropPending = 0;
	for(int i=0;i<childCount();i++)
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( child(i) );
		if (! pUnit )
			continue;

		if ( canUnload( pTarget, pUnit ) )
			verbUnload( pTarget, pUnit );
		else
			nDropPending += 1;
	}

	if ( nDropPending > 0 )
	{
		if ( orbiting() != pTarget )
		{
			// we have units so proceed to our target...
			if ( canOrbit( pTarget ) )
				verbCommand( ORBIT, pTarget );
			else
				verbCommand( FOLLOW, pTarget );
		}
	}
	else
	{
		// no units to drop, proceed to load phase..
		m_bLoadUnits = true;
	}
	
	return true;
}

bool NounShip::updateReconOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;
	if ( WidgetCast<NounStructure>( pTarget ) || WidgetCast<NounUnit>( pTarget ) )
		pTarget = WidgetCast<Noun>( pTarget->parent() );

	float fDistance = RealDistanceFromTarget( this, pTarget ); //(worldPosition() - pTarget->worldPosition()).magnitude();
	if ( fDistance < RECON_DISTANCE )
		return false;		// close enough, order is complete..

	if ( orbiting() != pTarget )
	{
		if ( canOrbit( pTarget ) )
			verbCommand( ORBIT, pTarget );
		else
			verbCommand( FOLLOW, pTarget );
	}

	return true;
}

bool NounShip::updateMoveOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;
	if ( WidgetCast<NounStructure>( pTarget ) || WidgetCast<NounUnit>( pTarget ) )
		pTarget = WidgetCast<Noun>( pTarget->parent() );

	if ( orbiting() != pTarget )
	{
		if ( canOrbit( pTarget ) )
		{
			if ( orbiting() == pTarget )
				return false;

			verbCommand( ORBIT, pTarget );
		}
		else
		{
			float fDistance = RealDistanceFromTarget( this, pTarget ); //(worldPosition() - pTarget->worldPosition()).magnitude();
			if ( fDistance < MOVE_DISTANCE )
				return false;

			verbCommand( FOLLOW, pTarget );
		}
	}

	return true;
}


bool NounShip::updateReloadOrder()
{
	if ( GadgetReload::getGadgetReload( this ) == NULL )
		return false;
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;
	NounShip * pTargetShip = WidgetCast<NounShip>( pTarget );
	if (! pTargetShip )
		return false;
	if (! pTargetShip->needReload( this ) && !pTargetShip->needRepair( this ) )
		return false;

	// gadget will use on the ship automatically once we are in range..
	verbCommand( FOLLOW, pTargetShip );

	return true;
}

bool NounShip::updateBeaconOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;

	if ( orbiting() != pTarget )
	{
		if ( canOrbit( pTarget ) )
			verbCommand( ORBIT, pTarget );
		else
			verbCommand( FOLLOW, pTarget );
	}

	bool bCanBeacon = false;
	for(int i=0;i<m_Gadgets.size();i++)
	{
		NounGadget * pGadget = m_Gadgets[i];
		if ( WidgetCast<GadgetBeacon>( pGadget ) )
		{
			if ( ((GadgetBeacon *)pGadget)->ammo() > 0 )
			{
				bCanBeacon = true;
				if ( !pGadget->useActive()  && pGadget->usable( pTarget, false ) )
					useGadget( pGadget, pTarget, false );			// attempt to beacon the target
			}
		}
	}

	return bCanBeacon;
}

bool NounShip::updateHoldOrder()
{
	verbCommand( STOP, NULL );
	return true;
}

bool NounShip::updateTradeOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;			// no order target
	NounPlanet * pHome = WidgetCast<NounPlanet>( pTarget );
	if (! validateTarget( pHome ) )
		return false;			// invalid order target, cancel this order
	if (! isFriend( pHome ) )
		return false;			// order target has been captured, cancel this order..
	if (! pHome->testFlags( NounPlanet::FLAG_HAS_PORT ) )
		return false;			// port is scrapped or turned off, cancel this order...
	
	if ( m_Command == NOCOMMAND )
	{
		NounPlanet * pOrbiting = WidgetCast<NounPlanet>( orbiting() );
		if (! isMaxFuel() && pOrbiting != NULL && isFriend( pOrbiting ) )
			return true;		// just orbit until fuel is refilled...

		GameContext::BuyMap & buyMap = gameContext()->buyMap();

		switch( m_nOrderState )
		{
		case 0:		// Look for buy/sell pair, if found head to the buy planet to buy the resources
			{
				if ( resourceCount() > 1000 )
				{
					// we already have a cargo hold full of resources, just look for a sell target...
					NounPlanet * pSellTarget = findSellTarget();
					if ( pSellTarget != NULL )
					{
						TRADE_LOG( CharString().format( "%s has cargo, selling to %s", name(), pSellTarget->name() ) );
						verbCommand( ORBIT, pSellTarget );
						m_nOrderState = 2;
						break;
					}
				}

				NounPlanet * pBuyTarget = findBuyTarget();
				NounPlanet * pSellTarget = findSellTarget();
				if ( pBuyTarget != NULL && pSellTarget != NULL && pBuyTarget != pSellTarget )
				{
					TRADE_LOG( CharString().format( "%s found trade pair, buy target is %s and sell target is %s",
						name(), pBuyTarget->name(), pSellTarget->name() ) );

					verbCommand( ORBIT, pBuyTarget );
					m_nOrderState = 1;

					buyMap[ pBuyTarget->key() ].insert( key() );
				}
				else if ( pTarget != pOrbiting )
				{
					TRADE_LOG( CharString().format( "%s found no trade pairs, going home to %s", name(), pTarget->name() ) );
					verbCommand( ORBIT, pTarget );		// go orbit our home planet then..
				}
			}
			break;
		case 1:		// buy resources then head to the sell planet
			{
				// remove this transport from the map..
				if ( pOrbiting != NULL )
					buyMap[ pOrbiting->key() ].erase( key() );

				NounPlanet * pSellTarget = findSellTarget();
				if ( pSellTarget != NULL && pSellTarget != pOrbiting )
				{
					if ( buyCargo() )
					{
						verbCommand( ORBIT, pSellTarget );
						m_nOrderState = 2;

						TRADE_LOG( CharString().format( "%s bought cargo at %s, sell target is %s.", name(),
							pOrbiting != NULL ? pOrbiting->name() : "NULL",
							pSellTarget != NULL ? pSellTarget->name() : "NULL" ) );
					}
				}
				else
				{
					TRADE_LOG( CharString().format( "%s aborting buy, sell target invalid!", name() ) );
					m_nOrderState = 0;
				}
			}
			break;
		case 2:		// sell resources, then back to the first step..
			{
				if ( sellCargo() ) 
				{
					TRADE_LOG( CharString().format( "%s sold cargo at %s", name(), pOrbiting != NULL ? pOrbiting->name() : "NULL" ) );
					m_nOrderState = 0;
				}
			}
			break;
		}
	}

	return true;
}

bool NounShip::updateBuildOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;
	GadgetBuild * pBuildGadget = GadgetBuild::getGadgetBuild( this );
	if (! pBuildGadget )
		return false;
	if ( pBuildGadget->useActive() )
		return true;	// currently building something..
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( pTarget );
	if (! validateTarget( pPlanet ) )
		return false;
	if (! pPlanet->isBuildTarget() )
		return false;
	if ( pPlanet->lockRank() > 0 )
		return false;	// we don't want to build locked planets

	// first, get into orbit of the planet we need to build on...
	if ( orbiting() != pPlanet )
	{
		verbCommand( ORBIT, pPlanet );
		return true;
	}

	// build the next needed structure...
	BaseNode * pBuildRoot = pBuildGadget->build()->node();
	if (! pBuildRoot )
		return false;

	NounStructure * pBuild = NULL;

	int nPower = 0;
	int nHabitat = 0;
	int nFood = 0;
	int nWorkers = 0;
	int nResearch = 0;

	for(int i=0;i<pPlanet->childCount();++i)
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( pPlanet->child(i) );
		if ( ! pStructure )
			continue;

		if ( pBuildGadget->usable( pStructure, false ) )
		{
			// found a incomplete structure... help build.
			useGadget( pBuildGadget, pStructure, false );
			return true;
		}

		nPower += pStructure->power();
		nHabitat += pStructure->habitat();
		nFood += pStructure->food();
		nWorkers += pStructure->workers();
		nResearch += pStructure->research();
	}

	if ( CountChildren<StructureHub>( pPlanet ) < 1 && CountChildren<StructureMIHub>( pPlanet ) < 1 )
		pBuild = FindChild<StructureHub>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureBarracks>( pPlanet ) < 1 )
		pBuild = FindChild<StructureBarracks>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureMine>( pPlanet ) < 2 )
		pBuild = FindChild<StructureMine>( pBuildRoot );
	if ( pBuild == NULL && (CountChildren<StructurePower>( pPlanet ) < 1 || nPower <= 0) )
		pBuild = FindChild<StructurePower>( pBuildRoot );
	if ( pBuild == NULL && (CountChildren<StructureDome>( pPlanet ) < 4 || nWorkers > nHabitat) )
		pBuild = FindChild<StructureDome>( pBuildRoot );
	if ( pBuild == NULL && (CountChildren<StructureFarm>( pPlanet ) < 4 || nWorkers > nFood) )
		pBuild = FindChild<StructureFarm>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureLab>( pPlanet ) < 2 )
		pBuild = FindChild<StructureLab>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructurePort>( pPlanet ) < 1 )
		pBuild = FindChild<StructurePort>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureFactory>( pPlanet ) < 2 )
		pBuild = FindChild<StructureFactory>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureMine>( pPlanet ) < 4 )
		pBuild = FindChild<StructureMine>( pBuildRoot );
	if ( pBuild == NULL && CountChildren<StructureDepot>( pPlanet ) < 1 )
		pBuild = FindChild<StructureDepot>( pBuildRoot );
	// build shipyard if the planet is Terran...
	if ( pBuild == NULL && pPlanet->baseHabitat() >= 50 && CountChildren<StructureShipyard>( pPlanet ) < 1 )
	{
		if ( nResearch < 65 )
			pBuild = FindChild<StructureLab>( pBuildRoot );
		else
			pBuild = FindChild<StructureShipyard>( pBuildRoot );
	}
	// build shield if available..
	if ( pBuild == NULL && CountChildren<StructureShield>( pPlanet ) < 1 )
		pBuild = FindChild<StructureShield>( pBuildRoot );
	// lastly, build defense bases until full..
	// defense bases are tricky, manual logic required:

	if (pBuild == NULL)
	{
		Array<StructureDefense * > m_pDefenseBases;
		for (int i = 0; i < pBuildRoot->childCount(); i++)
		{
			if (WidgetCast<StructureDefense>(pBuildRoot->child(i)))
			{
				NounStructure * pTemp = WidgetCast<StructureDefense>(pBuildRoot->child(i));
				CharString sName = pTemp->name();
				if (pTemp->buildTechnology() <= nResearch && sName.find("Defense") != std::string::npos)
					pBuild = pTemp;
			}
		}


	}

	// TODO: Upgrade structures...

	if ( ! pBuild )
		return false;		// nothing left to build..
	
	// for the sake of getting it done, just pick a random position on the planet and find a hex..
	Vector3 vOffset( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) );
	vOffset.normalize();
	vOffset *= pPlanet->radius();

	int nHex = pPlanet->findHex( vOffset, true );
	if ( nHex < 0 )
		return false;

	NounDropTarget::Ref pDropTarget = new NounDropTarget( pPlanet, nHex );

	// we may need to wait for resources to build up before we can build...
	if ( VerbBuildStructure::canBuild( this, pDropTarget, pBuild ) )
		Verb::Ref( new VerbBuildStructure( this, pDropTarget, pBuild ) );

	return true;
}

static Constant FALLBACK_DEFEND_DISTANCE("FALLBACK_DEFEND_DISTANCE", 1000.0f );

bool NounShip::updateFallbackOrder()
{
	Noun * pTarget = m_OrderTarget;
	if (! pTarget )
		return false;

	bool bCornered = false;

	float fTargetDistance = RealDistanceFromTarget( this, pTarget ); //(worldPosition() - pTarget->worldPosition()).magnitude();
	if( fTargetDistance < FALLBACK_DEFEND_DISTANCE )
	{
		NounShip * pEnemy = findEnemyShip( DEFAULT_SHIP_BRAVERY, FALLBACK_DEFEND_DISTANCE );
		if ( pEnemy != NULL && fTargetDistance < FALLBACK_DEFEND_DISTANCE )
		{
			bCornered = true;		// ok, we have fallen back, but we still have enemies... so FIGHT to the death!
			if ( m_Command != EVADE && m_Command != ENGAGE )
				verbCommand( ENGAGE, pEnemy );
		}
	}

	if (! bCornered && orbiting() != pTarget )
	{
		if ( canOrbit( pTarget ) )
			verbCommand( ORBIT, pTarget );
		else
			verbCommand( FOLLOW, pTarget );
	}

	// this returns false once refueled, repaired, and reloaded..
	return needFuel() || needAmmo() || damageRatio() > 0.01f;
}

//---------------------------------------------------------------------------------------------------

bool NounShip::findBestOrder()
{
	if ( (flags() & FLAG_NO_BEST_ORDER) != 0 )
		return false;		// ally ships need to leave the game once they are done..

	if ( (flags() & FLAG_AI_ALLY) != 0 )
	{
		verbOrder( DEFEND, m_pHome );
		return true;
	}

	switch( type() )
	{
	case SCOUT:
		if ( acquireReconTarget() )
			return true;
	case FIGHTER:
	case FRIGATE:
	case DESTROYER:
	case CRUISER:
	case DREAD:
		if ( acquireCombatTarget() )
			return true;
		return false;
	case STATION:
		if ( acquireCombatTarget() )
			return true;
		if ( acquireReloadTarget() )
			return true;
		return false;
	case ENGINEER:
		if ( acquireBuildTarget() )
			return true;
		return false;
	case SUPPLY:
		if ( acquireReloadTarget() )
			return true;
		return false;
	case TRANSPORT:
		if ( acquireCaptureTarget() )
			return true;
		if ( acquireTradeTarget() )
			return true;
		return false;
	case PLATFORM:
		verbOrder( DEFEND, this, this );
		return true;	
	case PLATFORM_SUPPLY:
		if ( acquirePlatformReloadTarget() )
			return true;
		return false;
	}

	return false;
}

bool NounShip::returnHome()
{
	Noun * pHome = findHomeJumpGate();
	if (! validateTarget( pHome ) )
	{
		// look for the closest planet/depot to refuel and reload ...
		pHome = findDepot();
		if (! validateTarget( pHome ) )
			pHome = findPlanet( true );
	}

	if ( ! validateTarget( pHome ) )
		return false;
	
	if ( orbiting() != pHome )
	{
		if ( canOrbit( pHome ) )
			verbCommand( ORBIT, pHome );
		else
			verbCommand( FOLLOW, pHome ); 
	}
	// we don't want to issue an order, it can't be interrupted if an order is found..
	//verbOrder( MOVE, m_pHome, this );

	return true;
}

bool NounShip::updateCommand()
{
	PROFILE_FUNCTION();

	if (! isLocal() )
		return false;
	if ( (flags() & FLAG_COMMAND_DISABLED) != 0 )
		return false;
	//if ( userId() == 0 && Profiler::CPUused() >= AI_CPU_USAGE )
	//	return false;		// note we check userId(), we don't want player's ship to run into planets just because their CPU usage is high!

	switch( m_Command )
	{
	case ORBIT:
		updateOrbitCommand();
		break;
	case FOLLOW:
		updateFollowCommand();
		break;
	case SHADOW:
		updateShadowCOmmand();
		break;
	case EVADE:
		updateEvadeCommand();
		break;
	case ENGAGE:
		updateEngageCommand();
		break;
	case STOP:
		updateStopCommand();
		break;
	case REFUEL:
		updateRefuelCommand();
		break;
	}

	return m_Command != NOCOMMAND;
}

void NounShip::updateOrbitCommand()
{
	if ( validateTarget( m_CommandTarget ) )
	{
		if ( VerbEnterOrbit::canOrbit( this, m_CommandTarget ) )
		{
			if ( moveToTarget( m_CommandTarget, radius() + m_CommandTarget->radius(), 
				MAX_SHIP_ORBIT_DISTANCE, SHIP_DEFAULT_SPEED, true, false ) == MOVE_DONE )
			{
				if ( VerbEnterOrbit::canEnterOrbit( this, m_CommandTarget ) )
				{
					// enter orbit
					Verb::Ref( new VerbEnterOrbit( this, m_CommandTarget, SHIP_ORBIT_VELOCITY ) );
					// command complete
					verbCommand( NOCOMMAND, NULL );
				}
				else
				{
					// stop the ship
					controlVelocity( 0.0f );
				}
			}
		}
		else
			verbCommand( NOCOMMAND, NULL );
	}
}

void NounShip::updateShadowCOmmand()
{
	// shadow & follow use the same logic, just different within distances..
	updateFollowCommand();
}

void NounShip::updateFollowCommand()
{
	if ( validateTarget( m_CommandTarget ) )
	{
		Noun * pTarget = m_CommandTarget;
		if ( WidgetCast<NounJumpGate>( pTarget ) )
		{
			// move inside the target
			if ( moveToTarget( pTarget, 0.0f, 0.0f, 1.0f, false, false ) == MOVE_DONE )
				controlVelocity( 0 );
		}
		else
		{
			// move within follow distance, then match velocity and heading
			float fWithin = m_Command == FOLLOW ? SHIP_FOLLOW_DISTANCE : SHIP_SHADOW_DISTANCE;
			if ( moveToTarget( pTarget, radius() + pTarget->radius(), fWithin, 1.0f, true, false ) == MOVE_DONE )
			{
				Vector3 vWorldVelocity( pTarget->worldVelocity() );
				float fVelocity = vWorldVelocity.magnitude();
				float fYaw = m_fSetHeading;

				if ( fVelocity > 0.0f )
				{
					vWorldVelocity *= 1.0f / fVelocity;		// normalize the vector..
					fYaw = atan2( vWorldVelocity.x, vWorldVelocity.z );
				}

				controlHeadingVelocity( fYaw, fVelocity );
			}
		}
	}
	else
		verbCommand( NOCOMMAND, NULL );
}

void NounShip::updateEvadeCommand()
{
	if ( validateTarget( m_CommandTarget ) )
	{
		Vector3 delta( m_CommandTarget->worldPosition() - worldPosition() );
		
		// check distance to target
		float distance = delta.magnitude();
		//delta.normalize();

		distance -= (m_CommandTarget->radius() + radius());
		if ( distance < SHIP_EVADE_DISTANCE )
		{
			static dword EVADE_VECTOR_TICKS = TICKS_PER_SECOND * EVADE_VECTOR_INTERVAL;
			if ( (m_nLastEvadeVector + EVADE_VECTOR_TICKS) < tick() )
			{
				delta.normalize();
				float fEvadeAngle = atan2( delta.x, delta.z ) + RandomFloat( -PI4, PI4 );

				m_nLastEvadeVector = tick();
				m_vEvadeVector.x = sinf( fEvadeAngle );
				m_vEvadeVector.y = 0.0f;
				m_vEvadeVector.z = cosf( fEvadeAngle );
			}

			NounZoneTarget target( worldPosition() - (m_vEvadeVector * (SHIP_EVADE_DISTANCE * 2)), context() );
			moveToTarget( &target, 0, 0, 1.0f, 0, false, false );		
		}
		else
			verbCommand( NOCOMMAND, NULL );
	}
	else
		verbCommand( NOCOMMAND, NULL );
}

void NounShip::updateEngageCommand()
{
	if ( validateTarget( m_CommandTarget ) )
	{
		bool bForceJump = false;

		// calculate an intercept position for our target based on our velocity and theirs..
		Vector3 vIntercept = m_CommandTarget->worldPosition();
		if ( WidgetCast<NounShip>( m_CommandTarget ) )
		{
			NounShip * pTarget = (NounShip *)m_CommandTarget.pointer();
			if ( pTarget->maxVelocity() > maxVelocity() )		// ship must be faster then ours 
			{
				// if the distance is outside of weapons range, then consider jumping ahead of our target...
				Vector3 vDelta( vIntercept - worldPosition() );
				float fInterceptDistance = vDelta.magnitude();
				if ( fInterceptDistance > MIN_INTERCEPT_DISTANCE )
				{
					int nChance = JUMP_INTERCEPT_CHANCE + (m_fCunning * JUMP_INTERCEPT_CHANCE);
					if ( (rand() % 100) < nChance )
					{
						vIntercept += pTarget->worldVelocity() * JUMP_INTERCEPT_TIME;
						bForceJump = true;
					}
				}
			}
		}

		NounZoneTarget::Ref pTarget = new NounZoneTarget( vIntercept, context() );
		if ( moveToTarget( pTarget, radius() + m_CommandTarget->radius(), 
			SHIP_ENGAGE_DISTANCE, SHIP_ENGAGE_SPEED, false, bForceJump ) == MOVE_DONE )
		{
			if ( maxVelocity() > 5.0f )
			{
				// begin evading, after evading the command will be set back to NOCOMMAND
				m_Command = EVADE;
			}
		}
	}
	else
		verbCommand( NOCOMMAND, NULL );
}

void NounShip::updateStopCommand()
{
	controlVelocity( 0.0f );		// stop ship
}

void NounShip::updateRefuelCommand()
{
	if ( validateTarget( m_CommandTarget ) )
	{
		if ( orbiting() != m_CommandTarget )
		{
			if ( VerbEnterOrbit::canOrbit( this, m_CommandTarget ) )
			{
				if ( moveToTarget( m_CommandTarget, radius() + m_CommandTarget->radius(), 
					MAX_SHIP_ORBIT_DISTANCE, 1.0f , true, false ) == MOVE_DONE )
				{
					if ( VerbEnterOrbit::canEnterOrbit( this, m_CommandTarget ) )
					{
						// enter orbit
						Verb::Ref( new VerbEnterOrbit( this, m_CommandTarget, SHIP_ORBIT_VELOCITY ) );
					}
					else
					{
						// stop the ship
						controlVelocity( 0.0f );
					}
				}
			}
			else
				verbCommand( NOCOMMAND, NULL );
		}
		else
		{
			// we are in orbit around our command target, wait for drive to be fully fueled!
			GadgetJumpDrive * pDrive = jumpDrive();
			if ( !validateNoun( pDrive ) || pDrive->fuel() >= pDrive->maxFuel() )
				verbCommand( NOCOMMAND, NULL );		// no drive or drive fueled, command completed!
		}
	}
	else
		verbCommand( NOCOMMAND, NULL );
}

void NounShip::updatePointDefense()
{
	PROFILE_FUNCTION();

	// handle point defense
	if ( isLocal() )
	{
		for(int j=0;j<contactCount();++j)
		{
			NounGame * pContact = WidgetCast<NounGame>( contact( j ) );
			if ( pContact != NULL && pContact->enablePD() )
			{
				if ( isFriend( pContact ) )
					continue;

				// any other weapons on this ship have this projectile as their target
				GadgetBeamWeapon * pBeam = NULL;
				bool bTargeted = false;

				for(int i=0;i<m_Gadgets.size() && !bTargeted;i++)
				{
					GadgetBeamWeapon * pCheck = WidgetCast<GadgetBeamWeapon>( m_Gadgets[i] );
					if ( pCheck != NULL && pCheck->pointDefense() )
					{
						if ( pCheck->target() == pContact )
							bTargeted = true;
						else if ( pCheck->usable( pContact, false ) )
							pBeam = pCheck;
					}
				}

				// if projectile not targeted, check for usablitly
				if (! bTargeted && pBeam != NULL )
				{
					// fire weapon at target, set the target first so no other beam weapons attempt to fire
					// at the same projectile this loop
					pBeam->setTarget( pContact );
					// issue the verb to use this beam weapon on the target
					useGadget( pBeam, pContact, false );
					useGadgets();
				}
			}
		}
	}
}

//----------------------------------------------------------------------------

struct Avoid {
	NounGame::Ref		m_pObject;
	float				m_fDistance;
	float				m_fPriority;
};
typedef std::list< Avoid >		AvoidList;

static void InsertAvoid( AvoidList & a_List, const Avoid & a_Avoid )
{
	bool bInserted = false;
	for( AvoidList::iterator iAvoid = a_List.begin(); 
		iAvoid != a_List.end(); ++iAvoid )
	{
		if ( a_Avoid.m_fPriority > (*iAvoid).m_fPriority )
		{
			a_List.insert( iAvoid, a_Avoid );
			bInserted = true;
			break;
		}
	}

	if (! bInserted )
		a_List.push_back( a_Avoid );
}

//! This function returns the shortest distance between the two given line segments.
//! http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
static float SegmentDistance( const Vector3 & a_s1_p0, const Vector3 & a_s1_p1, 
							  const Vector3 & a_s2_p0, const Vector3 & a_s2_p1 )
{
	const float SMALL_NUM = 0.00000001f;

	Vector3 u( a_s1_p1 - a_s1_p0);
	Vector3 v( a_s2_p1 - a_s2_p0 );
	Vector3 w( a_s1_p0 - a_s2_p0 );
	float a = u | u;			// always >= 0
	float b = u | v;
	float c = v | v;			// always >= 0
	float d = u | w;
	float e = v | w;
	float D = a*c - b*b;		// always >= 0
	float sc,sN,sD = D;			// sc = sN / sD, default sD = D >= 0
	float tc,tN,tD = D;			// tc = tN / tD, default tD = D >= 0

   // compute the line parameters of the two closest points
    if (D < SMALL_NUM) 
	{ 
		// the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else 
	{
		// get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) 
		{
			// sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) 
		{  
			// sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) 
	{
		// tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else 
		{
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) 
	{
		// tc > 1 => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (fabs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (fabs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
    Vector3 dP = w + (u * sc) - (v * tc);  // = S1(sc) - S2(tc)

    return dP.magnitude();	// return the closest distance
}

const float AVOID_BASE_TIME = 5.0f;

bool NounShip::avoidCollision()
{
	float fAvoidTime = AVOID_BASE_TIME + (PI4 / maxYaw());
	float r2 = radius() * 2.0f;

	// TODO: Adjust avoid time based on the manuverability of this ship..
	Vector3 vWorldPosition( worldPosition() );
	Vector3 vWorldEndPosition( vWorldPosition + (worldVelocity() * fAvoidTime) );

	AvoidList avoid;

	// generate a list of collisions to avoid sorted in order of highest to lowest priority, first add all incoming
	// projetiles to the list..
	for(int i=0;i<m_Incoming.size();++i)
	{
		Avoid a;
		a.m_pObject = m_Incoming[i];
		if (! a.m_pObject.valid() )
			continue;

		a.m_fDistance = (vWorldPosition - a.m_pObject->worldPosition()).magnitude2();
		a.m_fPriority = a.m_fDistance;

		InsertAvoid( avoid, a );
	}

#if 0
	// now add contacts that we must avoid, these are much higher in pririty..
	for(int i=0;i<m_Contacts.size();++i)
	{
		Avoid a;
		a.m_pObject = WidgetCast<NounGame>( m_Contacts[i] );
		if (! a.m_pObject.valid() )
			continue;
		if (! a.m_pObject->canShipCollide() )
			continue;

		a.m_fDistance = (vWorldPosition - a.m_pObject->worldPosition()).magnitude2();
		a.m_fPriority = a.m_fDistance;	

		InsertAvoid( avoid, a );
	}
#endif

	// Avoid targets are sorted in order from highest to lowest priority, start with 
	// the highest, if we detect an intercept then plot a new heading to avoid the collision
	// and stop.. otherwise continue checking until we determine we won't be colliding with
	// any of possible avoid targets..
	for( AvoidList::iterator iAvoid = avoid.begin();
		iAvoid != avoid.end(); ++iAvoid )
	{
		Avoid & a = *iAvoid;

		NounGame * pContact = a.m_pObject;
		if (! pContact )
			continue;

		Vector3 vContactPosition( pContact->worldPosition() );
		Vector3 vContactEndPosition( vContactPosition + (pContact->worldVelocity() * fAvoidTime) );

		float fDistance = SegmentDistance( vContactPosition, vContactEndPosition, 
							  vWorldPosition, vWorldEndPosition );
		if ( fDistance < (r2 + pContact->radius()) )
		{
			AI_LOG( CharString().format("%s Avoiding collision with %s, distance = %.1f", name(), pContact->name(), fDistance) );

			Vector3 vAvoid;
			calculateAvoidVector( pContact, vAvoid );

			Vector3 delta( vAvoid - vWorldPosition);
			delta.normalize();

			// turn towards escape route, use last velocity passed to verbControl()
			controlHeading( atan2( delta.x, delta.z ));
			return true;
		}
	}

	return false;
}

bool NounShip::checkForCollision( Noun * a_pTarget,
								 NounShip::CollisionInfo * a_pInfo /*= NULL*/, 
								 bool a_bIgnoreShips /*= true*/ )
{
	if (! a_pTarget )
		return false;

	Vector3 vTarget( a_pTarget->worldPosition() );
	Vector3 vPosition( worldPosition() );
	Vector3 vDelta( vTarget - vPosition );
	Vector3 vMidPoint( vPosition + (vDelta * 0.5f) );

	float fDistance = vDelta.magnitude();
	float r2 = radius() * 2.0f;

	// get a list of all objects that we might encounter
	Array< GameContext::NounCollision > nouns;
	context()->proximityCheck( vMidPoint, fDistance * 0.5f, nouns );

	if ( a_pInfo != NULL )
	{
		a_pInfo->m_fDistance = fDistance;
		a_pInfo->m_pCollide = NULL;
	}

	// check for the first collision
	for(int i=0;i<nouns.size();i++)
	{
		Noun * pCheck = nouns[i].pNoun;
		if ( WidgetCast<NounBody>( pCheck ) == NULL || 
			WidgetCast<NounNebula>( pCheck ) != NULL )							// can't collide with nebulas
		{
			// ignore object
			continue;
		}
		if ( a_bIgnoreShips && WidgetCast<NounShip>( pCheck ) != NULL )
			continue;
		// ignore collision with our target or self
		if ( pCheck == a_pTarget || pCheck == this )
			continue;

		Vector3 intersect;
		if ( pCheck->intersect( r2, vPosition, vDelta, intersect ) )
		{
			float fRange = (intersect - vPosition ).magnitude();
			if ( fRange < fDistance )
			{
				if ( a_pInfo == NULL )
					return true;		// return on first collision if the info on the closest collision is not required..

				a_pInfo->m_pCollide = pCheck;
				a_pInfo->m_vIntersect = intersect;
				a_pInfo->m_fDistance = fDistance = fRange;
			}
		}
	}

	return a_pInfo != NULL && a_pInfo->m_pCollide != NULL;
}

void NounShip::calculateAvoidVector( Noun * a_pTarget,
					Vector3 & a_vAvoid )
{
	ASSERT( a_pTarget != NULL );
	float fRadius = (radius() + a_pTarget->radius()) * 3.0f;

	Vector3 vMyPosition( worldPosition() );
	Vector3 vTargetPosition( a_pTarget->worldPosition() );
	Vector3 vDelta( vTargetPosition - vMyPosition );
	vDelta.y = 0.0f;		// make sure it's happening on the X/Z plane only..
	float fDistance = vDelta.magnitude();

	Vector3 vTargetVelocity( a_pTarget->worldVelocity() /*- worldVelocity()*/ );
	float fTargetVelocity = vTargetVelocity.magnitude();
	if ( fTargetVelocity > 0.0f )
	{
		// adjust the target position to account for it's current velocity ..
		float t = fDistance / fTargetVelocity;			// t = the approximate travel time until the collision
		vTargetPosition += vTargetVelocity * t;
		vDelta = vTargetPosition - vMyPosition;
		vDelta.y = 0.0f;
		fDistance = vDelta.magnitude();
	}

	vDelta *= 1.0f / fDistance;			
	Matrix33 mCollide( vDelta );

	Vector3 vDirection( sinf( m_fHeading ), 0.0f, cosf( m_fHeading ) );
	float fDot = vDirection | mCollide.i;
	Vector3 Z( fDot > 0.0f ? mCollide.i : -mCollide.i );
	Z -= vDelta;			// bring vector towards this ship
	Z.normalize();			// normalize now..

	a_vAvoid = a_pTarget->worldPosition() + (Z * fRadius);
}

//----------------------------------------------------------------------------

float NounShip::scoreTarget( Noun * pTarget )
{
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();
	pTarget = pTarget->zoneParent();

	float fValue = value();
	float fTargetValue = 1.0f;

	if ( WidgetCast<NounShip>( pTarget ) )
		fTargetValue = ((NounShip *)pTarget)->value();
	else if ( WidgetCast<NounPlanet>( pTarget ) )
		fTargetValue = ((NounPlanet *)pTarget)->value();

	return fValue / Max( (fValue + fTargetValue), 1.0f );
}

bool NounShip::canAttack( Noun * pTarget )
{
	if ( WidgetCast<NounShip>( pTarget ) )
		return true;

	if ( isTargetPlanet( pTarget ) )
	{
		for(int i=0;i<m_Gadgets.size();i++)
			if ( m_Gadgets[i]->type() == NounGadget::WEAPON_BOMB )
				return true;
	}

	return false;
}

bool NounShip::isMaxFuel()
{
	GadgetJumpDrive * pDrive = jumpDrive();
	if ( pDrive != NULL )
		return pDrive->fuel() >= pDrive->maxFuel();

	return true;
}

bool NounShip::needFuel()
{
	GadgetJumpDrive * pDrive = jumpDrive();
	if (! pDrive )
		return false;

	float f = ((float)pDrive->fuel()) / ((float)pDrive->maxFuel());
	return f <= NEED_FUEL_PERCENT;
}

bool NounShip::needAmmo()
{
	// check all weapons, if any weapon ammo level is below a certain level then this will return true...
	for(int i=0;i<childCount();++i)
	{
		GadgetWeapon * pWeapon = WidgetCast< GadgetWeapon >( child(i) );
		if (! pWeapon )
			continue;
		if ( pWeapon->ammoMax() <= 0 )
			continue;		// no ammo needed..
		float fAmmoLevel = ((float)pWeapon->ammo()) / ((float)pWeapon->ammoMax());
		if ( fAmmoLevel <= NEED_AMMO_PERCENT )
			return true;
	}

	return false;
}

bool NounShip::sellCargo()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( orbiting() );
	if (! validateTarget( pPlanet ) )
		return true;

	StructurePort * pPort = StructurePort::findPort( pPlanet );
	if ( ! validateTarget( pPort ) || ! pPort->active() )
		return true;

	float fResources = pPlanet->resources();
	for(int i=0;i<childCount();i++)
	{
		CargoResource * pCargo = WidgetCast<CargoResource>( child(i) );
		if ( pCargo != NULL )
		{
			if ( (fResources + pCargo->quantity()) < pPlanet->maxResources() )
			{
				fResources += pCargo->quantity();
				Verb::Ref( new VerbUnload( this, pPort, pCargo ) );
			}
		}
	}

	// no pods in inroute, we are done..
	return true;		
}

bool NounShip::buyCargo()
{
	if ( podCount() > 0 )
		return false;		// keep trying to buy cargo until all pods arrive...
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( orbiting() );
	if (! validateTarget( pPlanet ) )
		return true;		// not in orbit
	int nBuyCount = availableCargo();
	if ( nBuyCount <= 0 )
		return true;		// no cargo space available
	if (! isFriend( pPlanet ) )
		return true;		// not a friendly planet
	if ( pPlanet->resources() < MIN_PLANET_RES )
		return true;		// planet has less than min, don't suck it dry..
	StructurePort * pPort = StructurePort::findPort( pPlanet );
	if (! validateTarget( pPort ) || !pPort->active() )
		return true;		// no port found

	// don't take more than 50% of a planets res in a single load..
	float fMaxLoad = (pPlanet->resources() - MIN_PLANET_RES) / 2.0f;
	float fLoaded = 0.0f;

	// everything ok, buy n cargo items and fill our cargo hold with resources..
	for(int i=0;i<nBuyCount;i++)
	{
		Verb::Ref( new VerbLoad( this, pPort ) );

		fLoaded += MAX_RESOURCES;
		if ( fLoaded > fMaxLoad )
			break;
		if ( pPlanet->resources() <= MIN_PLANET_RES )
			break;			// don't suck the planet dry..
	}

	return false;
}

NounShip * NounShip::findEnemyShip( float bravery, float within ) const
{
	ASSERT( bravery >= 0.0f && bravery <= 1.0f );

	float			myValue = value();
	Vector3			myPosition( worldPosition() );

	NounShip *		bestShip = NULL;
	float			bestShipValue = 32768.0f;

	for(int i=0;i<contactCount();i++)
	{
		NounShip * pShip = WidgetCast<NounShip>( contact( i ) );
		if ( pShip && pShip != this && !isFriend( pShip ) )
		{
			float distance = (myPosition - pShip->worldPosition()).magnitude();
			if ( distance > within )
				continue;

			// > 1.0 then the ship is more powerful than me, less < 1.0 the ship is weaker
			float value = float( pShip->value() ) / myValue;	
			// factor in the bravery, a lower bravery make me look for weaker ships than myself
			value *= (1.0f - bravery);
			// factor in the distance to the target
			value += distance * FIND_ENEMY_DISTANCE_VALUE;

			if ( value < bestShipValue )
			{
				bestShip = pShip;
				bestShipValue= value;
			}
		}
	}

	return( bestShip );
}

//---------------------------------------------------------------------------------------------------

bool NounShip::acquireReloadTarget()
{
	if ( needFuel() )
		return false;

	int nMyFactionId = factionId();
	// first off, check for any distress signals...
	GameContext::DistressList & list = gameContext()->getDistressList( nMyFactionId );
	for( GameContext::DistressList::iterator iDistress = list.begin(); iDistress != list.end(); ++iDistress )
	{
		GameContext::Distress * pDistress = *iDistress;
		if ( pDistress->m_nType != VerbDistress::SUPPLY )
			continue;			// ignore non-supply distress

		if ( WidgetCast<NounShip>( pDistress->m_pWho ) )
		{
			NounShip * pShip = (NounShip *)pDistress->m_pWho.pointer();
			if ( pShip == this )
				continue;		// don't respond to our own distress signal..
			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), RELOAD, pShip );
			if ( pOrder )
				continue;		// another ship is already responding to this distress..

			verbOrder( RELOAD, pDistress->m_pWho, this );
			return true;
		}
	}

	NounShip * pBestReload = NULL;
	float fBestDistance = 0.0f;

	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounShip * pShip = WidgetCast<NounShip>( pZone->child(i) );
			if (! pShip )
				continue;
			if (! isFriend( pShip ) )
				continue;
			if ( !pShip->needRepair( (Noun *)this ) 
				&& !pShip->needReload(  (Noun *)this ) )
				continue;
			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), RELOAD, pShip );
			if ( pOrder )
				continue;		// another ship is already responding to this distress..

			float fDistance = (worldPosition() - pShip->worldPosition()).magnitude();
			if ( fDistance < fBestDistance || !pBestReload )
			{
				pBestReload = pShip;
				fBestDistance = fDistance;
			}
		}
	}

	if (! pBestReload )
		return false;

	verbOrder( RELOAD, pBestReload, this );
	return true;
}

bool NounShip::acquirePlatformReloadTarget()
{
    NounShip * pBestReload = NULL;
    float fBestDistance = 0.0f;

    Array< GameContext::NounCollision > detect;
    if ( context()->proximityCheck( worldPosition(), 250, detect ) )
    {
        for(int i=0;i<detect.size();i++)
        {
            Noun * pNoun = detect[i].pNoun;
            if (! isFriend( pNoun ) )
				continue;
			if ( WidgetCast<NounShip>( pNoun ) == NULL )
				continue;
            NounShip * pShipContact = (NounShip *)pNoun;               

            if ( pShipContact->needRepair( (Noun *)this ) 
				|| pShipContact->needReload( (Noun *)this ) )
            {
                float fDistance = (worldPosition() - pShipContact->worldPosition()).magnitude();
                if ( fDistance < fBestDistance || !pBestReload )
                {
                    pBestReload = pShipContact;
                    fBestDistance = fDistance;
                }
            }
        }
    }

    // no repair target found, does the platform need to repair itself?
    if (! pBestReload && (needRepair( this) || needReload( this ) ) )
        pBestReload = this;

    if (! pBestReload )
        return false;

    verbOrder( RELOAD, pBestReload, this );
    return true;
}

bool NounShip::acquireReconTarget() 
{
	if ( needFuel() )
		return false;

	std::vector< NounPlanet * > targets;

	Vector3 vMyPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;

			float fDistance = (vMyPosition - pPlanet->worldPosition()).magnitude();
			if ( fDistance < MIN_RECON_DISTANCE )
				continue;		// planet is too close, next ...
			
			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), RECON, pPlanet );
			if ( pOrder )
				continue;		// another ship is already scouting this planet.. 

			targets.push_back( pPlanet );
		}
	}

	if ( targets.size() <= 0 )
		return false;

	int nPick = rand() % targets.size();
	verbOrder( RECON, targets[ nPick ], this );

	return true;
}

bool NounShip::acquireCombatTarget()
{
	if ( needFuel() || needAmmo() )
		return false;

	int nMyFactionId = factionId();

	// dreads & stations lead attacks, so this logic is ignored for those ship types..
	if ( type() == NounShip::FRIGATE 
		|| type() == NounShip::DESTROYER 
		|| type() == NounShip::CRUISER )
	{
		// first off, check for any distress signals...
		GameContext::DistressList & list = gameContext()->getDistressList( nMyFactionId );
		for( GameContext::DistressList::iterator iDistress = list.begin(); iDistress != list.end(); ++iDistress )
		{
			GameContext::Distress * pDistress = *iDistress;
			if ( pDistress->m_nType == VerbDistress::SUPPLY )
				continue;			// ignore supply distress

			if ( WidgetCast<NounShip>( pDistress->m_pWho ) )
			{
				NounShip * pShip = (NounShip *)pDistress->m_pWho.pointer();
				if ( pShip == this )
					continue;		// don't respond to our own distress signal..
					
				if ( pShip->isOutOfCombat() )
					continue;			// ignore ships that are out of combat

				float fValue = pShip->value();
				GameContext::Order * pOrder = gameContext()->findOrder( factionId(), DEFEND, pShip );
				if ( pOrder != NULL )
					fValue -= pOrder->m_nScore;
				if ( fValue < 0.0f )
					continue;

				verbOrder( DEFEND, pShip, this );
				return true;
			}
			else if ( WidgetCast<NounPlanet>( pDistress->m_pWho ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pDistress->m_pWho.pointer();

				float fValue = pPlanet->value();

				GameContext::Order * pOrder = gameContext()->findOrder( factionId(), DEFEND, pPlanet );
				if ( pOrder != NULL )
					fValue -= pOrder->m_nScore;
				if ( fValue < 0.0f )
					continue;

				verbOrder( DEFEND, pPlanet, this );
				return true;
			}
		}

		// check for any transports on a capture run, provide escort if needed..
		GameContext::OrderListMap & orderListMap = gameContext()->getFactionOrderMap()[ factionId() ];
		for( GameContext::OrderListMap::iterator iOrderList = orderListMap.begin(); iOrderList != orderListMap.end(); ++iOrderList )
		{
			GameContext::OrderList & orders = iOrderList->second;
			for( GameContext::OrderList::iterator iOrder = orders.begin(); iOrder != orders.end(); ++iOrder )
			{
				GameContext::Order * pOrder = *iOrder;
				if ( pOrder->m_nOrder == NounShip::CAPTURE )
				{
					// check for any combat ships in the ship list..
					bool bEscortFound = false;
					std::vector< NounShip * > transports;

					for( GameContext::ShipList::iterator iShip = pOrder->m_ShipList.begin(); 
						iShip != pOrder->m_ShipList.end() && !bEscortFound; ++iShip )
					{
						NounShip * pShip = *iShip;
						if ( pShip->type() == NounShip::FRIGATE 
							|| pShip->type() == NounShip::DESTROYER
							|| pShip->type() == NounShip::CRUISER )
						{
							bEscortFound = true;
						}
						else if ( pShip->type() == NounShip::TRANSPORT )
							transports.push_back( pShip );
					}

					if (! bEscortFound && transports.size() > 0 )
					{
						// defend a random transport with this capture order..
						verbOrder( DEFEND, transports[ rand() % transports.size() ], this );
						return true;
					}
				}
			}
		}
	}

	Noun *			pBestTarget = NULL;
	float			fBestValue = 0.0f;
	float			fMyValue = value();
	Vector3			vPosition( worldPosition() );

	// now scan the world for a attack target...
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounGame * pNoun = WidgetCast<NounGame>( pZone->child(i) );
			if (! pNoun )
				continue;
			if (! pNoun->isDetected( nMyFactionId ) )
				continue;
			if ( isFriend( pNoun ) )
				continue;
			if (! canAttack( pNoun ) )
				continue;

			if ( WidgetCast<NounShip>( pNoun ) )
			{
				NounShip * pShip = (NounShip *)pNoun;

				// adjust value of ship based on distance ...
				float fDistance = (vPosition - pShip->worldPosition()).magnitude();
				float fValue = pShip->value() - (fDistance * DISTANCE_VALUE_SCALE);

				GameContext::Order * pOrder = gameContext()->findOrder( factionId(), ATTACK, pShip );
				if ( pOrder != NULL )
					fValue -= pOrder->m_nScore;

				if ( !pBestTarget || fValue > fBestValue )
				{
					pBestTarget = pNoun;
					fBestValue = fValue;
				}
			}
			else if ( WidgetCast<NounPlanet>( pNoun ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pNoun;

				if ( pPlanet->maxUnits() == 0 )
					continue;		// don't attack gas giants
				if ( pPlanet->structureCount() == 0 || pPlanet->unitCount() == 0 )
					continue;		// don't attack planets with no structures or units

				float fDistance = (vPosition - pPlanet->worldPosition()).magnitude();
				float fValue = pPlanet->value() - (fDistance * DISTANCE_VALUE_SCALE);

				GameContext::Order * pOrder = gameContext()->findOrder( factionId(), ATTACK, pPlanet );
				if ( pOrder != NULL )
					fValue -= pOrder->m_nScore;
				if ( fValue < 0.0f )
					continue;		// if negative, don't check the rest..
				pOrder = gameContext()->findOrder( factionId(), CAPTURE, pPlanet );
				if ( pOrder != NULL )
					fValue *= CAPTURE_VALUE_SCALE;			// planets that our faction is trying to capture gain a bonus
				if ( (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
					fValue *= BLOCKADE_VALUE_SCALE;			// blockaded planets get bonus value
				// if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD) != 0 )
					// fValue *= SHIPYARD_VALUE_SCALE;			// shipyard planets get bonus value

				if ( !pBestTarget || fValue > fBestValue )
				{
					pBestTarget = pNoun;
					fBestValue = fValue;
				}
			}

		}
	}

	if (! pBestTarget )
		return false;

	verbOrder( ATTACK, pBestTarget, this );
	return true;
}

bool NounShip::acquireBuildTarget()
{
	if ( needFuel() )
		return false;

	NounPlanet *	pBestTarget = NULL;
	float			fBestValue = 0.0f;

	Vector3 vMyPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;
			if (! isFriend( pPlanet ) )
				continue;		// planet must be friendly..
			if (! pPlanet->isBuildTarget() )
				continue;		// planet is fully built and all structures are repaired..
			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), BUILD, pPlanet );
			if ( pOrder != NULL )
				continue;		// another ship has an order to build this planet..
			if ( pPlanet->lockRank() > 0 )	// ignore planets that are locked
				continue;

			float fDistance = (vMyPosition - pPlanet->worldPosition()).magnitude();
			float fValue = pPlanet->value() - (fDistance * DISTANCE_VALUE_SCALE);

			if ( !pBestTarget || fValue > fBestValue )
			{
				pBestTarget = pPlanet;
				fBestValue = fValue;
			}
		}
	}

	if (! pBestTarget )
		return false;

	verbOrder( BUILD, pBestTarget, this );
	return true;
}

bool NounShip::acquireCaptureTarget() 
{
	if ( needFuel() )
		return false;

	int nMinUnits = MIN_PLANET_UNITS;
	nMinUnits += availableCargo();

	bool bUnitsOnPlanets = false;
	bool bUnitsOnShip = friendlyUnitCount() > 0;

	NounPlanet *	pBestTarget = NULL;
	float			fBestValue = 0.0f;

	Vector3 vMyPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;
			if (! pPlanet->isCappable() )
				continue;
			if ( isFriend( pPlanet ) )
			{
				bUnitsOnPlanets |= pPlanet->friendlyUnitCount() >= nMinUnits;
				continue;		// planet must not be friendly..
			}
			if ( pPlanet->maxUnits() <= 0 )
				continue;		// planet must be able to support life... no gas giants

			float fDistance = (vMyPosition - pPlanet->worldPosition()).magnitude();
			float fValue = pPlanet->value() - (fDistance * DISTANCE_VALUE_SCALE);
			
			GameContext * pContext = gameContext();
			for(int k=0;k<pContext->conditionCount();++k)
			{
				const WorldContext::Condition & condition = pContext->conditionN( k );
				for(int n=0;n<condition.objectives.size();++n)
				{
					const WorldContext::Objective & objective = condition.objectives[n];
					if ( objective.factionId != factionId() )
						continue;		// not an objective for our faction
					if ( objective.type != GameContext::CONTROL )
						continue;		// not a capture objective

					// If planet is an objective, then double it's value
					if ( objective.noun == pPlanet->key() )
						fValue *= 2.0f;
				}
			}

			// for each AI ship trying to capture this planet, INCREASE value by 50%... this will make AI ships all try
			// to capture the same planets, thus improving the chance of success!
			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), CAPTURE, pPlanet );
			if ( pOrder != NULL )
				fValue += fValue * (pOrder->m_ShipList.size() * 0.5f);

			// scale value by the number of troops on the planet.. fewer troops means higher value, thus
			// the AI will try to capture weakly defended planets.
			fValue *= 1.0f - (((float)pPlanet->friendlyUnitCount()) / ((float)pPlanet->maxUnits()));

			if ( !pBestTarget || fValue > fBestValue )
			{
				pBestTarget = pPlanet;
				fBestValue = fValue;
			}
		}
	}

	if (! bUnitsOnShip && !bUnitsOnPlanets )
		return false;		// no units to capture anything!
	if (! pBestTarget )
		return false;

	// send this ship the order..
	verbOrder( CAPTURE, pBestTarget, this );
	return true;
}

bool NounShip::acquireTradeTarget()
{
	if ( needFuel() )
		return false;

	NounPlanet *	pBestTarget = NULL;
	float			fBestValue = 0.0f;

	Vector3 vMyPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;
			if (! isFriend( pPlanet ) )
				continue;		// planet must be friendly..
			if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_PORT) == 0 )
				continue;		// planet must have a port..
			if ( (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
				continue;		// planet is currently blockaded..

			float fDistance = (vMyPosition - pPlanet->worldPosition()).magnitude();
			float fValue = pPlanet->value() - (fDistance * DISTANCE_VALUE_SCALE);

			GameContext::Order * pOrder = gameContext()->findOrder( factionId(), TRADE, pPlanet );
			if ( pOrder != NULL )
				fValue -= pOrder->m_nScore;

			if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_SHIPYARD) != 0 )
				fValue *= SHIPYARD_VALUE_SCALE;			// blockaded planets get bonus value

			if ( !pBestTarget || fValue > fBestValue )
			{
				pBestTarget = pPlanet;
				fBestValue = fValue;
			}
		}
	}

	if (! pBestTarget )
		return false;

	verbOrder( TRADE, pBestTarget, this );
	return true;
}

//---------------------------------------------------------------------------------------------------

Noun * NounShip::findDepot() const
{
	Noun *			pBestDepot = NULL;
	float			fBestDistance = 0.0f;

	Vector3	myPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			Noun * pNoun = WidgetCast<Noun>( pZone->child(i) );
			if (! pNoun )
				continue;
			if ( WidgetCast<NounPlanet>(pNoun ) )
			{
				NounPlanet * pPlanet = (NounPlanet *)pNoun;
				if ( !isFriend( pPlanet ) )
					continue;

				float fDistance = (pPlanet->worldPosition() - myPosition).magnitude2();
				if ( (pPlanet->flags() & NounPlanet::FLAG_HAS_DEPOT) == 0 )
					fDistance *= 1.5f;		// consider planets without depots further

				if ( !pBestDepot || fDistance < fBestDistance )
				{
					pBestDepot = pPlanet;
					fBestDistance = fDistance;
				}
			}
			//else if ( WidgetCast<NounShip>( pNoun ) )
			//{
			//	NounShip * pShip = (NounShip *)pNoun;
			//	if ( !isFriend( pShip ) )
			//		continue;
			//	if ( pShip == this )
			//		continue;
			//	if ( type() == NounShip::SUPPLY ||
			//		type() == NounShip::STATION ||
			//		type() == NounShip::PLATFORM_SUPPLY )
			//		continue;		// supply ships must orbit a friendly planet..
			//	if ( pShip->type() != NounShip::SUPPLY &&
			//		pShip->type() != NounShip::STATION &&
			//		pShip->type() != NounShip::PLATFORM_SUPPLY )
			//		continue;

			//	float fDistance = (pShip->worldPosition() - myPosition).magnitude2();
			//	if ( !pBestDepot || fDistance < fBestDistance )
			//	{
			//		pBestDepot = pShip;
			//		fBestDistance = fDistance;
			//	}
			//}
		}
	}

	return pBestDepot;
}


NounPlanet * NounShip::findPlanet( bool bFriendly ) const
{
	NounPlanet *	pBestPlanet = NULL;
	float			fBestDistance = 0.0f;

	Vector3	myPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;
			if ( bFriendly && !isFriend( pPlanet ) )
				continue;

			float fDistance = (pPlanet->worldPosition() - myPosition).magnitude();
			if ( bFriendly && (pPlanet->flags() & NounPlanet::FLAG_HAS_DEPOT) != 0 )
				fDistance *= 0.5f;		// consider planets with depots closer..

			if ( !pBestPlanet || fDistance < fBestDistance )
			{
				pBestPlanet = pPlanet;
				fBestDistance = fDistance;
			}
		}
	}

	return pBestPlanet;
}

NounPlanet * NounShip::findUnits() const
{
	NounPlanet *	pBestPlanet = NULL;
	float			fBestValue = 0.0f;

	Vector3	myPosition( worldPosition() );
	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounPlanet * pPlanet = WidgetCast<NounPlanet>( pZone->child(i) );
			if (! pPlanet )
				continue;
			if ( !isFriend( pPlanet ) )
				continue;
			int nUnitCount = pPlanet->friendlyUnitCount();
			if ( nUnitCount <= (int)MIN_PLANET_UNITS )
				continue;

			float fDistance = (pPlanet->worldPosition() - myPosition).magnitude();
			float fValue = (nUnitCount * UNIT_VALUE) - (fDistance * DISTANCE_VALUE_SCALE);

			if ( !pBestPlanet || fValue > fBestValue )
			{
				pBestPlanet = pPlanet;
				fBestValue = fValue;
			}
		}
	}

	return pBestPlanet;

}

NounJumpGate * NounShip::findHomeJumpGate() const
{
	Vector3	myPosition( worldPosition() );

	NounJumpGate *	pBestJumpGate = NULL;
	float			fBestRange = SHIP_FIND_RANGE;

	for(int j=0;j<context()->zoneCount();j++)
	{
		NodeZone * pZone = context()->zone( j );
		for(int i=0;i<pZone->childCount();i++)
		{
			NounJumpGate * pJumpGate = WidgetCast<NounJumpGate>( pZone->child(i) );
			if ( validateTarget( pJumpGate ) && pJumpGate->active() && pJumpGate->destinationNoun() == NULL )
			{
				float fRange = (myPosition - pJumpGate->worldPosition()).magnitude();
				if ( !pBestJumpGate || fRange < fBestRange )
				{
					pBestJumpGate = pJumpGate;
					fBestRange= fRange;
				}
			}
		}
	}

	return pBestJumpGate;
}

NounJumpGate * NounShip::findJumpGate( Noun * pDestination )
{
	GameContext * pContext = gameContext();
	if (! pContext )
		return NULL;
	if (! pDestination )
		return NULL;

	NounJumpGate * pGate = pContext->findGatePath( this, pDestination );
	//if ( pGate != NULL )
	//	AI_LOG( CharString().format( "Found gate (%s) best path to (%s) for ship (%s)", pGate->name(), pDestination->name(), name() ) );

	return pGate;
}

NounPlanet * NounShip::findSellTarget()
{
	// find the best planet to sell cargo too..
	NounPlanet * pBestTarget = NULL;
	float fBestDemand = 0.0f;

	// update the trade routes for our planet
	Array< GameContext::NounCollision > nouns;
	if ( context()->proximityCheck( worldPosition(), MAX_TRADE_ROUTE_RANGE, nouns, CLASS_KEY(NounPlanet) ) )
	{
		for(int i=0;i<nouns.size();i++)
		{
			NounPlanet * pTarget = WidgetCast<NounPlanet>( nouns[i].pNoun );
			if (! pTarget )
				continue;
			if ( pTarget->maxResources() == 0 )
				continue;	// gas/inferno planet.. 
			if ( pTarget->resources() >= pTarget->maxResources() )
				continue;	// planet already at max res.. 
			if (! pTarget->testFlags( NounPlanet::FLAG_HAS_PORT ) )
				continue;	// no port on the target planet
			if ( pTarget->testFlags( NounPlanet::FLAG_BLOCKADE) )
				continue;	// planet is under blockade..
			if ( pTarget->factionId() != factionId() )
				continue;	// cannot trade with planets of a different faction
			if ( !pTarget->importStatus() )
				continue;	// check if planet allows imports

			// the greater the trade imbalance, the greater the score..
			float fDemand = pTarget->resourceDemand();
			if ( pBestTarget == NULL || fDemand > fBestDemand )
			{
				pBestTarget = pTarget;
				fBestDemand = fDemand;
			}
		}
	}

	return pBestTarget;
}

NounPlanet * NounShip::findBuyTarget()
{
	GameContext::BuyMap & buyMap = gameContext()->buyMap();

	// find the best planet to buy cargo from
	NounPlanet * pBestTarget = NULL;
	float fBestDemand = 0.0f;

	// update the trade routes for our planet
	Array< GameContext::NounCollision > nouns;
	if ( context()->proximityCheck( worldPosition(), MAX_TRADE_ROUTE_RANGE, nouns, CLASS_KEY(NounPlanet) ) )
	{
		for(int i=0;i<nouns.size();i++)
		{
			NounPlanet * pTarget = WidgetCast<NounPlanet>( nouns[i].pNoun );
			if (! pTarget )
				continue;
			if ( pTarget->maxResources() == 0 )
				continue;	// gas/inferno planet.. 
			if ( pTarget->resources() < MIN_PLANET_RES )
				continue;	// planet already at min res.. 
			if (! pTarget->testFlags( NounPlanet::FLAG_HAS_PORT ) )
				continue;	// no port on the target planet
			if ( pTarget->testFlags( NounPlanet::FLAG_BLOCKADE) )
				continue;	// planet is under blockade..
			if ( pTarget->factionId() != factionId() )
				continue;	// cannot trade with planets of a different faction
			if ( buyMap[ pTarget->key() ].size() > 0 )
				continue;	// transport already enroute to this planet
			if ( !pTarget->exportStatus() )
				continue;	// check if planet allows exports

			float fDemand = pTarget->resourceDemand();
			if ( pBestTarget == NULL || fDemand < fBestDemand )
			{
				pBestTarget = pTarget;
				fBestDemand = fDemand;
			}
		}
	}

	return pBestTarget;
}

//----------------------------------------------------------------------------

void NounShip::onAttacked( Noun * pFrom, int nDamage, dword damageType )
{
	if ( userId() == 0 && isLocal() )
	{
		m_bAttacked = true;
		m_pLastAttacker = pFrom;

		if ( isEnemy( pFrom ) && WidgetCast<NounShip>( pFrom ) != NULL )
		{
			// always try to return fire ... 
			if( command() != ENGAGE && command() != EVADE )	
				verbCommand( ENGAGE, pFrom );	
		}

		// if damage is very bad, consider a emergency jump...
		if ( damageRatioInv() < EMERGENCY_JUMP_DAMAGE && jumpDrive() != NULL )
		{
			if ( (rand() % 100) < EMERGENCY_JUMP_CHANCE )
				useGadget( jumpDrive(), NULL, true );
		}

		static dword ATTACK_DISTRESS_TICKS = (dword)(TICKS_PER_SECOND * SHIP_AI_ATTACK_DISTRESS);
		if ( (m_nLastDistress + ATTACK_DISTRESS_TICKS) < tick() && isEnemy( pFrom ) )
		{
			m_nLastDistress = tick();
			Verb::Ref( new VerbDistress( this, VerbDistress::ATTACKED ) );
		}
	}

	// track damage from whom only server-side... this is used to award enhancements
	// to players who destroy this ship and allow AI to track threat levels.
	if( isServer() )
		addThreat( pFrom, nDamage );
}

bool NounShip::addThreat( Noun * a_pNoun, int a_nDamage )
{
	ThreatList::iterator iThreat = m_ThreatList.begin();
	for( ; iThreat != m_ThreatList.end(); ++iThreat )
		if ( (*iThreat).m_pThreat.pointer() == a_pNoun )
			break;

	if ( iThreat != m_ThreatList.end() )
	{
		Threat & t = *iThreat;
		t.m_nDamage += a_nDamage;
		t.m_fThreat = (t.m_nDamage / DAMAGE_THREAT) + t.m_fStrength;
		return true;
	}

	// add to map and initialize the threat structure...
	Threat t;
	t.m_pThreat = a_pNoun;
	t.m_nDamage = a_nDamage;
	t.m_fStrength = calculateStrength( a_pNoun );
	t.m_fOptimialRange = calculateWeaponRange( a_pNoun );
	t.m_fThreat = (t.m_nDamage / DAMAGE_THREAT) + t.m_fStrength;

	//AI_LOG( CharString().format( "%p: Adding new threat %p, Strength: %.2f, Range: %.2f, Threat: %.2f ", 
	//	this, t.m_pThreat.pointer(), t.m_fStrength, t.m_fOptimialRange, t.m_fThreat ) );
	m_ThreatList.push_back( t );

	return true;
}

float NounShip::calculateStrength(Noun * a_pThreat )
{
	if ( WidgetCast<NounShip>( a_pThreat ) )
	{
		float fMyValue = value();
		float fTheirValue = ((NounShip *)a_pThreat)->value();
		if ( fMyValue > 0.0f )
			return fTheirValue / fMyValue;
	}

	return 1.0f;
}

float NounShip::calculateWeaponRange(Noun * a_pThreat)
{
	// calculate the optimal weapon range for this ship against the given target..
	// the default behavior is to stay at maximum range for any weapon that is ready to fire at the target
	float fBestRange = 0.0f;
	for(int i=0;i<m_Gadgets.size();++i)
	{
		NounGadget * pGadget = m_Gadgets[i];
		if ( WidgetCast<GadgetWeapon>( pGadget ) != NULL )
		{
			GadgetWeapon * pWeapon = (GadgetWeapon *)pGadget;

			// TODO: Adjust the fDodgeTime based on the ability of the target to actually dodge which would be a factor
			// of it's current velocity, radius and angle to this ship.
			float fMaxRange = pWeapon->maxRange();
			float fMinRange = pWeapon->minRange();
			float fPercent = 0.5f;		// TODO: adjust based on projectile speed, radius of target, damage fallout
			float fRange =  fMinRange + ((fMaxRange - fMinRange) * fPercent);
			fBestRange = Max<float>( fRange, fBestRange );
		}
		else if ( WidgetCast<GadgetBeamWeapon>( pGadget ) )
		{
			GadgetBeamWeapon * pBeam = (GadgetBeamWeapon *)pGadget;

			float fPercent = 0.5f;		// TODO: adjust based on damage falloff
			float fRange = pBeam->maxRange() * fPercent;
			fBestRange = Max<float>( fRange, fBestRange );
		}
	}

	if ( fBestRange == 0.0f )
	{
		AI_LOG( CharString().format( "%p: No weapons found on ship for calculateWeaponRange()", this ) );
		return 5000.0f;		// no weapons, keep quite a distance from the threat.
	}

	return fBestRange;
}

void NounShip::updateThreats()
{
	PROFILE_FUNCTION();

	if(isServer())
	{
		// drain off damage over time and remove threats once enough time has elapsed and we can no longer find the contact..
		for(ThreatList::iterator iThreat = m_ThreatList.begin(); iThreat != m_ThreatList.end();)
		{
			bool bRemove = false;

			Threat & t = *iThreat;
			if ( t.m_nDamage > 0 )
			{
				t.m_nDamage -= Max<int>( t.m_nDamage * THREAT_DECAY_RATE, Min<int>( 100, t.m_nDamage) );
				t.m_fThreat = (t.m_nDamage / DAMAGE_THREAT) + t.m_fStrength;
			}

			// remove any ship that has had its total damage decayed to 0 and is no longer a contact
			if ( t.m_nDamage == 0 && m_Contacts.search( t.m_pThreat.pointer() ) < 0 )
				bRemove = true;

			if( bRemove )
			{
				//AI_LOG( CharString().format( "%p: Removing threat %p", this, t.m_pThreat.pointer()) );
				iThreat = m_ThreatList.erase(iThreat);
			}
			else
				++iThreat;
		}

		// add threats to our map...
		for( int i=0;i<m_Contacts.size();++i)
		{
			Noun * pContact = m_Contacts[i];
			if (! pContact )
				continue;
			if (! isEnemy( pContact ) )
				continue;

			addThreat( pContact, 0 );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
