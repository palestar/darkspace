/*
	StructureShipyard.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "resource.h"
#include "GameContext.h"
#include "StructureShipyard.h"
#include "TraitOrbit.h"

//----------------------------------------------------------------------------

static Constant		USE_SHIPYARD_RANGE( "USE_SHIPYARD_RANGE", 250.0f );
//! Min number of resources on a planet, trade order will not buy if resources are below this amount
static Constant		MIN_PLANET_RES( "MIN_PLANET_RES", 10000.0f );
//! Min number of resources that need to be available before we spend it on building ships.
static Constant		MIN_SPEND_RES( "MIN_SPEND_RES", 100.0f );
//! Maximum number of ships we can build on at once..
static Constant		MAX_BUILD_SHIPS( "MAX_BUILD_SHIPS", 10.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureShipyard, NounStructure );
REGISTER_FACTORY_KEY( StructureShipyard, 4435834781079288815LL );

StructureShipyard::StructureShipyard() : m_nBuildTick( 0 )
{}

//---------------------------------------------------------------------------------------------------

void StructureShipyard::simulate( dword nTick )
{
	NounStructure::simulate( nTick );

	if ( isServer() && nTick >= m_nBuildTick )
	{
		m_nBuildTick = nTick + (TICKS_PER_SECOND * 5);

		if (! m_BuildQueue.empty() )
		{
			NounPlanet * pPlanet = planet();
			if (! pPlanet )
				return;

			int nAvailable = pPlanet->resources() - MIN_PLANET_RES;
			if ( nAvailable > MIN_SPEND_RES )
			{
				int nShipCount = m_BuildQueue.size();

				// limit the number of ships we apply resources so it doesn't take too long ...
				if ( nShipCount > MAX_BUILD_SHIPS )
					nShipCount = MAX_BUILD_SHIPS;

				// split the number of available resources by the number of ships..
				int nResPerShip = nAvailable / nShipCount;
				int nUsed = 0;
				int nShip = 0;

				for( BuildQueue::iterator iBuild = m_BuildQueue.begin(); iBuild != m_BuildQueue.end() && nShip < nShipCount; ++nShip )
				{
					Build & build = *iBuild;

					int nUse = build.m_nCost - build.m_nResources;
					if ( nUse > nResPerShip )
						nUse = nResPerShip;

					build.m_nResources += nUse;
					nUsed += nUse;

					if ( build.m_nResources >= build.m_nCost )
					{
						if (! ((GameContext *)context())->gameUser()->onBuildShip( pPlanet, build.m_nUserId, build.m_pShip, build.m_nCost ) )
						{
							// failed to build the ship, user logged off more than likely, return the resources to the planet then..
							nUsed -= build.m_nResources;
						}
						iBuild = m_BuildQueue.erase( iBuild );
					}
					else
					{
						// next build...
						++iBuild;
					}
				}

				// if any resources used/added, update the planet...
				if ( nUsed != 0 )
					pPlanet->setResources( pPlanet->resources() - nUsed );
			}
		}

		if ( !m_pSpawner.valid() || !m_pSpawner->isFriend( this ) )
		{
			// detach any old spawner
			detachAllNodes();
			// create a new spawner, attach it to this ship yard.. 
			if ( factionId() != FACTION_UNKNOWN && factionId() != FACTION_NEUTRAL )
				m_pSpawner = NounSpawnShip::makeTeamSpawner( gameContext(), teamId(), this, false );
		}
	}
}

//----------------------------------------------------------------------------

int StructureShipyard::sortId() const
{
	return 5;
}

int StructureShipyard::groupId() const
{
	return 0;
}

int StructureShipyard::maxDamage() const
{
	return 45360;
}

int StructureShipyard::buildTechnology() const
{
	return 65;
}

int	StructureShipyard::buildTime() const
{
	return 270;		
}

int	StructureShipyard::buildCost() const
{
	return 20000;
}

int	StructureShipyard::repairRate() const
{
	return 500;
}

Color StructureShipyard::color() const
{
	return BLUE;
}

dword StructureShipyard::planetFlags() const
{
	return NounPlanet::FLAG_HAS_SHIPYARD;
}

int	StructureShipyard::workers() const
{
	return 40;
}

int	StructureShipyard::power() const
{
	return -75;
}

int StructureShipyard::technology() const
{
	return 65;
}

bool StructureShipyard::usable( NounShip * pShip ) const
{
	if ( !active() )
		return false;		// shipyard is not active
	if ( pShip == NULL || pShip->parent() == NULL )
		return false;		// invalid ship or ship already detached
	NounPlanet * pPlanet = planet();
	if ( pPlanet == NULL )
		return false;		// structure not on planet
	if ( (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
		return false;		// can't, planet is under blockade..
	if ( pShip->orbiting() != pPlanet && (pShip->worldPosition() - worldPosition()).magnitude() > USE_SHIPYARD_RANGE )
		return false;		// ship not in orbit or out of range
	if (! pShip->isFriend( pPlanet ) )
		return false;		// enemy ship
	
	return true;
}

CharString StructureShipyard::useTip( NounShip * pShip ) const
{
	return "USE SHIPYARD";
}

bool StructureShipyard::useConfirm( NounShip * pShip ) const
{
	return true;	
}

CharString StructureShipyard::useConfirmMessage( NounShip * pShip ) const
{
	return "<color;ffffff>Dock Master: Confirm Ship Change";
}

void StructureShipyard::use( dword nWhen, NounShip * pShip )
{
	// just detach the players ship from the universe
	if ( usable( pShip ) )
	{
		// move ship out of orbit, so it doesn't snap back to this planet when they respawn
		TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pShip );
		if ( pOrbit != NULL )
			pOrbit->breakOrbit();

		// set the ship home to this planet, so they must respawn at this position
		pShip->setHome( planet() );
		// detach the ship from the universe
		pShip->despawn( pShip->despawnTime() );
	}
}

//---------------------------------------------------------------------------------------------------

bool StructureShipyard::addBuild( dword nUserId, NounShip::Ref pShip, int nCost )
{
	if (! isServer() )
		return false;

	// remove any previous build by this user first, do not allow multiple queue items from the same user
	cancelBuild( nUserId );
	// just push the item into the queue, the simulate() function will take care of building the ship when
	// resources are available
	m_BuildQueue.push_back( Build( nUserId, pShip, nCost ) );

	return true;
}

bool StructureShipyard::cancelBuild( dword nUserId )
{
	if (! isServer() )
		return false;

	for( BuildQueue::iterator iBuild = m_BuildQueue.begin(); iBuild != m_BuildQueue.end(); ++iBuild )
	{
		Build & build = *iBuild;
		if ( build.m_nUserId == nUserId )
		{
			if ( ((GameContext *)context())->gameUser()->onBuildCancelled( build.m_nUserId ) )
			{
				Build & build = *iBuild;

				// add any spent resources back into the planet..
				if ( build.m_nResources > 0 )
				{
					NounPlanet * pPlanet = planet();
					if ( pPlanet != NULL )
						pPlanet->setResources( pPlanet->resources() + build.m_nResources );
				}

				m_BuildQueue.erase( iBuild );
			}
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------------------------------

StructureShipyard * StructureShipyard::findYard( NounPlanet * pPlanet )
{
	if (! pPlanet )
		return NULL;
	for(int i=0;i<pPlanet->childCount();++i)
		if ( WidgetCast<StructureShipyard>( pPlanet->child(i) ) != NULL )
			return (StructureShipyard *)pPlanet->child(i);
	return NULL;
}

//----------------------------------------------------------------------------
//EOF
