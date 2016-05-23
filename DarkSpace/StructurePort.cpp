/*
	StructurePort.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "VerbOrderShip.h"
#include "VerbTradeRoute.h"
#include "VerbTransferResource.h"
#include "VerbLoad.h"
#include "NounPlanet.h"
#include "NounJumpGate.h"
#include "NounPod.h"
#include "StructurePort.h"
#include "GameContext.h"

#define ENABLE_AI_TRANSPORTS			1
#define ENABLE_RESOURCE_TRANSFERS		0

//----------------------------------------------------------------------------

static Constant MAX_PORT_TRANSPORTS( "MAX_PORT_TRANSPORTS", 1.0f );
static Constant CREDITS_PER_TRANSPORT( "CREDITS_PER_TRANSPORT", 50000.0f );
static Constant USE_PORT_RANGE( "USE_PORT_RANGE", 250.0f );
static Constant USE_PORT_RESOURCES( "USE_PORT_RESOURCES", 5000.0f );
static Constant RESPAWN_TRANSPORT_TIME( "RESPAWN_TRANSPORT_TIME", 600.0f );

const int PORT_UPDATE_RATE = TICKS_PER_SECOND * 60;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructurePort, NounStructure );
REGISTER_FACTORY_KEY( StructurePort, 4340953170952736210LL );

BEGIN_PROPERTY_LIST( StructurePort, NounStructure )
	ADD_PROPERTY( m_pTransports );
END_PROPERTY_LIST();

StructurePort::StructurePort() : m_nNextRoute( 0 ), m_nPortTick( 0 ), m_nLastSpawn( 0 )
{}

//----------------------------------------------------------------------------

void StructurePort::onDetaching()
{
#if ENABLE_AI_TRANSPORTS
	// detach any of our transports from the world as well...
	for(int i=0;i<m_pTransports.size();++i)
		if ( m_pTransports[i].valid() )
			m_pTransports[i]->setDetach();
#endif
	NounStructure::onDetaching();
}

//---------------------------------------------------------------------------------------------------

void StructurePort::initialize()
{
	NounStructure::initialize();
	m_nPortTick = tick() + seed( PORT_UPDATE_RATE );
}

void StructurePort::simulate( dword nTick )
{
	if ( nTick >= m_nPortTick )
	{
		m_nPortTick += PORT_UPDATE_RATE;
		if ( active() && isServer() )
		{
			NounPlanet * pPlanet = planet();
			ASSERT( pPlanet );			// simulate( dword nTick ) should not have been called if this structure isn't attached to a planet

			// update our cargo items
			updateCargo();

			// transfer resources between ports
#if ENABLE_AI_TRANSPORTS
			updateTransports();
#endif

#if ENABLE_RESOURCE_TRANSFERS
			transferResources();
#endif
		}
	}

	NounStructure::simulate( nTick );
}

//---------------------------------------------------------------------------------------------------

int StructurePort::sortId() const
{
	return 3;
}

int StructurePort::groupId() const
{
	return 0;
}

int StructurePort::maxDamage() const
{
	return 22680;
}

int StructurePort::buildTechnology() const
{
	return 0;
}

int StructurePort::buildTime() const
{
	return 45;
}

int StructurePort::buildCost() const
{
	return 150;
}

int StructurePort::repairRate() const
{
	return 600;
}

Color StructurePort::color() const
{
	return GOLD;
}

bool StructurePort::canBuild( NounPlanet * pPlanet, bool bUpgrade ) const
{
	if (! validateNoun( pPlanet ) )
		return false;
	if ( findPort( pPlanet, false ) != NULL )
		return false;

	return NounStructure::canBuild( pPlanet, bUpgrade );
}

dword StructurePort::planetFlags() const
{
	return NounPlanet::FLAG_HAS_PORT;
}

int	StructurePort::workers() const
{
	return 1;
}

int StructurePort::power() const
{
	return -2;
}

int StructurePort::technology() const
{
	return 0;
}

//---------------------------------------------------------------------------------------------------

bool StructurePort::usable( NounShip * pShip ) const
{
	if ( !active() )
		return false;		// not active
	if ( !pShip || !pShip->parent() )
		return false;		// invalid ship or ship already detached
	NounPlanet * pPlanet = planet();
	if ( !pPlanet )
		return false;		// structure not on planet
	if ( pPlanet->resourceCount() < USE_PORT_RESOURCES )
		return false;
	if ( pShip->orbiting() != pPlanet && (pShip->worldPosition() - worldPosition()).magnitude() > USE_PORT_RANGE )
		return false;		// ship not in orbit or out of range
	if (! pShip->isFriend( pPlanet ) )
		return false;		// enemy ship
	if ( pShip->availableCargo() <= 0 )
		return false;		// no room in cargo hold
	
	return true;
}

CharString StructurePort::useTip( NounShip * pShip ) const
{
	return "LOAD RESOURCES";
}

void StructurePort::use( dword nWhen, NounShip * pShip )
{
	if ( usable( pShip ) )
	{
		NounPlanet * pPlanet = planet();
		if (! pPlanet )
			return;

		int nUsed = pPlanet->useResources( USE_PORT_RESOURCES );
		if ( nUsed > 0 && isServer() )
		{
			// send message to ship
			pShip->privateChat( CharString().format( "<color;ffffff>Cargo: Loading %u Resources.", nUsed) );

			CargoResource * pResource = new CargoResource( nUsed );

			NounPod * pPod = new NounPod;
			pPod->setName( pResource->name() );
			//pPod->setKey( key() );
			pPod->setPosition( worldPosition() );
			pPod->setLastPosition( pPod->position() );
			pPod->setTeamId( pShip->teamId() );
			pPod->setTarget( pShip );
			pPod->setTick( nWhen );

			// zero out the cargo's position while on the pod
			pResource->setTeamId( pShip->teamId() );
			pPod->setCargo( pResource );

			// attach pod to the universe
			context()->attachNoun( pPod );
		}
	}
}

//----------------------------------------------------------------------------

CargoResource * StructurePort::findCargoResource()
{
	for(int i=0;i<childCount();i++)
		if ( WidgetCast<CargoResource>( child(i) ) != NULL )
			return (CargoResource *)child(i);

	return NULL;
}

//----------------------------------------------------------------------------

StructurePort * StructurePort::findPort( NounPlanet * pPlanet, bool bCheckActive /*= true*/ )
{
	if ( pPlanet != NULL )
	{
		for(int j=0;j<pPlanet->childCount();j++)
		{
			StructurePort * pPort = WidgetCast<StructurePort>( pPlanet->child(j) );
			if ( pPort != NULL && (!bCheckActive || pPort->active()) )
				return pPort;
		}
	}

	return NULL;
}

CargoResource * StructurePort::findCargoResource( NounPlanet * pPlanet )
{
	if ( pPlanet != NULL )
	{
		StructurePort * pPort = findPort( pPlanet );
		if ( pPort != NULL )
			return pPort->findCargoResource();
	}

	return NULL;
}

//----------------------------------------------------------------------------

void StructurePort::updateCargo()
{
	NounPlanet * pPlanet = planet();
	ASSERT( pPlanet );
	ASSERT( isServer() );

	// create a CargoResource child object if needed...
	if ( findCargoResource() == NULL )
		attachNode( new CargoResource( 0, false ) );

	// TODO: Remove items from the port after some period of time. This will help prevent build-up in the universe
	// of goods in all the ports. Ideally, we want to keep scarce items hard to find, and should try to reach
	// some type of balance in the universe for each type of device and resource.
}

void StructurePort::updateTransports()
{
#if ENABLE_AI_TRANSPORTS
	NounPlanet * pPlanet = planet();
	ASSERT( pPlanet );
	ASSERT( isServer() );

	// check current list of transport, remove any destroyed or detached transports
	for(int i=m_pTransports.size()-1;i>=0;i--)
	{
		if ( !m_pTransports[i].valid() 
			|| m_pTransports[i]->isDestroyed() 
			|| m_pTransports[i]->zone() == NULL 
			|| m_pTransports[i]->teamId() != pPlanet->teamId() )
		{
			m_pTransports.remove(i);
			m_nLastSpawn = tick();		// update the last spawn tick to the current time... so it takes a while before we replace this transport
		}
	}

	dword nRespawnTicks = TICKS_PER_SECOND * RESPAWN_TRANSPORT_TIME;

	// spawn transports to carry goods for our planet
	while ( m_pTransports.size() < MAX_PORT_TRANSPORTS )
	{
		if ( pPlanet->flags() & NounPlanet::FLAG_BLOCKADE )
			break;
		if ( m_nLastSpawn != 0 && tick() < (m_nLastSpawn + nRespawnTicks) )
			break;		// not enough time has elapsed...

		int nTeamID = pPlanet->teamId();
		NounTemplate * pBuild = ((GameContext *)context())->pickShipTemplate( nTeamID, NounShip::TRANSPORT );
		if (! pBuild )
			break;
		NounShip * pSpawn = WidgetCast<NounShip>( pBuild->spawn() );
		if (! pSpawn )
			break;

		pSpawn->setName( CharString().format( "%s Transport", pPlanet->name() ) );
		pSpawn->setTeamId( nTeamID );
		pSpawn->setVelocity( 5.0f );
		pSpawn->addFlags( NounShip::FLAG_NO_BEST_ORDER );		// don't look for new orders
		pSpawn->addFlags( NounShip::FLAG_NO_EXPIRE_ORDER );		// order never expires due to time limit
		pSpawn->setOrder( NounShip::TRADE, pPlanet, pSpawn );		// set the order to perform trade actions for this planet
		pSpawn->setRank( 255 );									// important, make it so this ship will not get orders/missions from players
		pSpawn->setup();
		
		Matrix33 mFrame( pPlanet->worldFrame() );
		Vector3 vPosition( pPlanet->worldPosition() );
		vPosition += mFrame.k * (pPlanet->radius() + pSpawn->radius() + 15.0f);

		pSpawn->setPosition( vPosition );
		pSpawn->setHeading( atan2( mFrame.k.x, mFrame.k.z ) );
		pSpawn->setFrame( Matrix33( 0, pSpawn->heading(), 0 ) );

		context()->attachNoun( pSpawn );

		// track the objects that we spawn
		m_pTransports.push( pSpawn );
		// turn on point defense
		//pSpawn->togglePointDefense();

		m_nLastSpawn = tick();
	}

	// check transport order, set their order if not set already ..
	for(int i=0;i<m_pTransports.size();i++)
	{
		NounShip * pTransport = m_pTransports[ i ];
		if ( validateNoun( pTransport ) )
		{
			if ( pTransport->order() != NounShip::TRADE || pTransport->orderTarget() != pPlanet )
				pTransport->verbOrder( NounShip::TRADE, pPlanet, pTransport );
		}
	}
#endif
}

void StructurePort::transferResources()
{
#if ENABLE_RESOURCE_TRANSFERS
	// remove any active transports
	while( m_pTransports.size() > 0 )
	{
		NounShip * pShip = m_pTransports[ 0 ];
		if ( pShip != NULL && pShip->zone() != NULL )
			pShip->setDetach();

		m_pTransports.remove( 0 );
	}

	NounPlanet * pPlanet = planet();
	ASSERT( pPlanet );

	if ( pPlanet->tradeRoutes() > 0 )
	{
		if ( m_nNextRoute >= pPlanet->tradeRoutes() )
			m_nNextRoute = 0;

		NounPlanet * pTarget = pPlanet->tradeRoute( m_nNextRoute );
		m_nNextRoute++;

		if ( VerbTransferResource::canTransferResource( pPlanet, pTarget, 1) )
		{
			float fTargetDemand = pTarget->resourceDemand();
			float fTotalDemand = fTargetDemand - pPlanet->resourceDemand();
			if ( fTotalDemand > 0)
			{
				float fDemand = (fTargetDemand / fTotalDemand) - 0.5f;
				int nTransfer = fDemand * pPlanet->resources();

				if ( nTransfer > 0 )
					Verb::Ref( new VerbTransferResource( pPlanet, pTarget, nTransfer ) );
			}
		}
	}
#endif
}

//----------------------------------------------------------------------------
//EOF
