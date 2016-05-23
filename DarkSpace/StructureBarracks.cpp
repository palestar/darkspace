/*
	StructureBarracks.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounPlanet.h"
#include "StructureBarracks.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant MAX_BARRACKS_UNIT_TRAIN( "MAX_BARRACKS_UNIT_TRAIN", 0.31f );

const int UNIT_BUILD_RATE = 10;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureBarracks, NounStructure );
REGISTER_FACTORY_KEY( StructureBarracks, 4229572335011401381LL );

BEGIN_PROPERTY_LIST( StructureBarracks, NounStructure )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_pBuilding );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_BuildTime );
END_PROPERTY_LIST();

StructureBarracks::StructureBarracks() : m_BuildTime( 0 ), m_nBuildTick( 0 )
{}

//----------------------------------------------------------------------------

void StructureBarracks::initialize()
{
	NounStructure::initialize();
	// load the units we can create
	loadUnits();
	// initialize our build tick
	m_nBuildTick = tick() + seed( TICKS_PER_SECOND * UNIT_BUILD_RATE );
}

void StructureBarracks::simulate( dword nTick )
{
	if ( nTick >= m_nBuildTick )
	{
		m_nBuildTick += (TICKS_PER_SECOND * UNIT_BUILD_RATE);
		if ( active() )
		{
			// train all infantry on planet slowly
			NounPlanet * pSurface = planet();
			if ( pSurface != NULL )
			{
				// increase experience of all units on this planet
				for(int i=0;i<pSurface->childCount();i++)
				{
					NounUnit * pUnit = WidgetCast<NounUnit>( pSurface->child(i) );
					if ( pUnit != NULL && pSurface->isFriend( pUnit ) && pUnit->experience() < MAX_BARRACKS_UNIT_TRAIN )
						pUnit->setExperience( pUnit->experience() + 0.01 );		// takes about an 2 hours to reach full strength
				}
			}

			// make sure we are building some type of unit
			if (! m_pBuilding.valid() )
			{
				// build the first type of unit available, usually infantry
				Array< NounUnit *> units;
				getUnits( units );

				if ( units.size() > 0 )
					m_pBuilding = units[ 0 ];
			}

			if ( m_pBuilding.valid() )
			{
				if ( m_BuildTime >= m_pBuilding->buildTime() )
				{
					// check for resources
					if ( canBuild( m_pBuilding ) )
					{
						NounPlanet * pPlanet = (NounPlanet *)parent();
						ASSERT( pPlanet );
					
						// unit build complete, spawn the unit onto the surface
						if ( isServer() )
						{
							NounUnit * pSpawn = (NounUnit *)m_pBuilding->copy();
							pSpawn->setTeamId( pPlanet->teamId() );
							pSpawn->setHex( pPlanet->findHex( position(), false ) );
							pSpawn->setup();

							pPlanet->attachNode( pSpawn );
						}

						// remove resources for the unit
						pPlanet->useResources( m_pBuilding->buildCost() );
						// reduce the build time, leaving excess for next item in queue
						m_BuildTime -= m_pBuilding->buildTime();
					}
				}
				else
					m_BuildTime += UNIT_BUILD_RATE;
			}
		}
	}

	NounStructure::simulate( nTick );
}

int StructureBarracks::sortId() const
{
	return 2;
}

int StructureBarracks::groupId() const
{
	return 0;
}

int StructureBarracks::maxDamage() const
{
	return 22680;
}

int StructureBarracks::buildTechnology() const
{
	return 0;
}

int StructureBarracks::buildTime() const
{
	return 30;
}

int StructureBarracks::buildCost() const
{
	return 100;
}

int StructureBarracks::repairRate() const
{
	return 80;
}

int StructureBarracks::workers() const
{
	return 1;
}

int	StructureBarracks::power() const
{
	return -2;
}

int StructureBarracks::technology() const
{
	return 0;
}

CharString StructureBarracks::status() const
{
	CharString info( NounStructure::status() );

	if ( active() )
	{
		if ( m_pBuilding.valid() )
		{
			int time = eta();
			int seconds = time % 60;
			int min = time / 60;

			info += CharString().format( "Building %s ... %d:%2.2d\n", 
				m_pBuilding->nounContext()->name(), min, seconds );
		}
	}

	return info;
}

//----------------------------------------------------------------------------

NounUnit * StructureBarracks::building() const
{
	return m_pBuilding;
}

int StructureBarracks::eta() const
{
	if ( m_pBuilding.valid() )
		return Max( m_pBuilding->buildTime() - m_BuildTime, 0 );

	return 0;
}

bool StructureBarracks::canSetBuild( NounUnit * pUnit ) const
{
	if ( pUnit == NULL )
		return false;			// invalid pointer

	Array< NounUnit * > units;
	getUnits( units );

	if ( units.search( pUnit ) < 0 )
		return false;			// unknown type of unit

	return true;
}

bool StructureBarracks::canBuild( NounUnit * pUnit ) const
{
	if (! canSetBuild( pUnit ) )
		return false;

	NounPlanet * pPlanet = (NounPlanet *)parent();
	if ( pPlanet == NULL )
		return false;			// structure not on planet
	if ( pPlanet->friendlyUnitCount() >= pPlanet->maxUnits() )
		return false;			// max units
	if ( pPlanet->technology() < pUnit->buildTechnology() )
		return false;			// insufficient technology level
	if ( pPlanet->resources() < pUnit->buildCost() )
		return false;		// not enough resources to build this unit

	return true;
}

//----------------------------------------------------------------------------

void StructureBarracks::setBuild( NounUnit * pUnit )
{
	m_pBuilding = pUnit;
}

//----------------------------------------------------------------------------

void StructureBarracks::loadUnits()
{
	m_pBuild = WidgetCast<Scene>( nounContext()->resource( "BUILD" ) );
}

void StructureBarracks::getUnits( Array< NounUnit * > & units ) const
{
	if ( m_pBuild.valid() )
	{
		BaseNode * pRoot = m_pBuild->node();
		ASSERT( pRoot );

		for(int i=0;i<pRoot->childCount();i++)
		{
			NounUnit * pUnit = WidgetCast<NounUnit>( pRoot->child(i) );
			if ( pUnit != NULL )
				units.push( pUnit );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
