/**
	@file ShipPlatform.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/26/2008 11:31:57 PM
*/

#include "ShipPlatform.h"
#include "VerbEnterOrbit.h"

//---------------------------------------------------------------------------------------------------

static Constant		PLATFORM_STARTING_RESOURCES( "PLATFORM_STARTING_RESOURCES", 0.1f );
static Constant		PLATFORM_ORBIT_VELOCITY( "PLATFORM_ORBIT_VELOCITY", 0.0f );						// orbit velocity of ship when it enters orbit

//---------------------------------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( ShipPlatform, NounShip ); 

BEGIN_ABSTRACT_PROPERTY_LIST( ShipPlatform, NounShip );
	ADD_REPLICATED_PROPERTY( m_nResources, TICKS_PER_SECOND * 5 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nBuildTime );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

ShipPlatform::ShipPlatform() : m_nResourceTick( 0 ), m_nResources( 0 ), m_nBuildTime( 0 )
{
	// turn off the dynamics for platforms..
	m_bDynamicsEnabled = false;
}

CharString ShipPlatform::status() const
{
	CharString sStatus = NounShip::status();
	if ( isBuilding() )
		sStatus += CharString().format("Building...%d%% complete\n", buildPercent() );
	sStatus += CharString().format( "Resources: %d%%\n", resourcesPercent() );

	return sStatus;
}

void ShipPlatform::setup()
{
	NounShip::setup();

	m_nResources = maxResources() * PLATFORM_STARTING_RESOURCES;
	m_nResourceTick = tick() + ticksPerUpdate();
	m_nBuildTime = 1;
	m_bShipEnabled = false;
}

void ShipPlatform::initialize()
{
	NounShip::initialize();
	m_bShipEnabled = m_nBuildTime >= buildTime();
	m_nResourceTick = tick();
}

void ShipPlatform::simulate( dword nTick )
{
	if ( m_nResourceTick <= nTick )
	{
		m_nResourceTick = nTick + ticksPerUpdate();
		m_nResources -= resourceCost();

		// let the platform reload it'self... 
		reload( this );

		if ( m_nResources <= 0 && isLocal() )
			setDetach();
		
	}

	// decay the Y position to zero
	m_Position.y -= m_Position.y * 0.02f;

	NounShip::simulate( nTick );
}

bool ShipPlatform::updateLogic( dword nTick )
{
	if (! NounShip::updateLogic( nTick ) )
		return false;

	if (! inOrbit() )
	{
		NounPlanet * pPlanet = findPlanet( true );
		if ( VerbEnterOrbit::canEnterOrbit( this, pPlanet ) )
			Verb::Ref( new VerbEnterOrbit( this, pPlanet, PLATFORM_ORBIT_VELOCITY ) );
	}

	return true;
}

//---------------------------------------------------------------------------------------------------

bool ShipPlatform::build()
{
	m_nBuildTime += 1;
	if ( m_nBuildTime >= buildTime() )
	{
		m_bShipEnabled = true;
		return true;
	}

	return false;
}

void ShipPlatform::addResouces( int nQuantity )
{
	m_nResources = Clamp( m_nResources + nQuantity, 0, maxResources() );
}

//---------------------------------------------------------------------------------------------------

Material * ShipPlatform::icon() const
{
	return WidgetCast<Material>( nounContext()->resource( "ICON" ) );
}

//---------------------------------------------------------------------------------------------------
//EOF
