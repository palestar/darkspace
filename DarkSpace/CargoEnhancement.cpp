
//EOF
/*
	CargoEnhancement.cpp
	(c)2008 Palestar Inc, Jeff Everett
*/


#include "Debug/Assert.h"
#include "Standard/Limits.h"
#include "NounPlanet.h"
#include "VerbLoad.h"
#include "NounShip.h"
#include "CargoEnhancement.h"
#include "StructurePort.h"
#include "ShipPlatform.h"
#include "DarkSpace/NounEnhancement.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

static Constant CARGO_ENHANCEMENT_LIFE( "CARGO_ENHANCEMENT_LIFE", 300.0f );
static Constant CARGO_ENHANCEMENT_SIG( "CARGO_ENHANCEMENT_SIG", 20.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( CargoEnhancement, NounCargo );

BEGIN_PROPERTY_LIST( CargoEnhancement, NounCargo )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_enhancementClassKey );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nDamage );
END_PROPERTY_LIST();

CargoEnhancement::CargoEnhancement() : 
	m_fRemovalTime( 0.0f ),
	m_enhancementClassKey(qword(0)), 
	m_pEnhancement(NULL), 
	m_nDamage( 0 )
{}

CargoEnhancement::CargoEnhancement( qword a_classKey ) : 
	m_fRemovalTime( 0.0f ),
	m_enhancementClassKey( a_classKey ),
	m_pEnhancement(NULL), 
	m_nDamage( 0 )
{
	// set the name
	setQuantity(1);
	createEnhancement();
}

//----------------------------------------------------------------------------

bool CargoEnhancement::read( const InStream & input )
{
	if (! NounCargo::read( input ) )
		return false;
	createEnhancement();
	return true;
}

//----------------------------------------------------------------------------

void CargoEnhancement::onAttached()
{
	NounCargo::onAttached();
	createEnhancement();
}

void CargoEnhancement::onDetaching()
{
	NounCargo::onDetaching();
	m_pEnhancement = NULL; 
}

//----------------------------------------------------------------------------

float CargoEnhancement::baseSignature() const
{
	return CARGO_ENHANCEMENT_SIG;
}

CharString CargoEnhancement::status() const
{
	if ( m_pEnhancement )
		return m_pEnhancement->status();
	return NounGame::status();
}

void CargoEnhancement::initialize()
{
	NounCargo::initialize();
	m_fRemovalTime = 0.0f;
	createEnhancement();
}

void CargoEnhancement::simulate( dword nTick )
{
	// NounCargo::simulate( dword nTick ) will remove the cargo object after timer expired
	float fElapsed = TICK_DURATION_S * (nTick - tick());
	// if we are not on a ship or a structureport
	if ( !WidgetCast<NounShip>( parent() ) )
	{
		m_fRemovalTime += fElapsed;
		if( m_fRemovalTime > CARGO_ENHANCEMENT_LIFE )
			setDetach();
	}
	else
	{
		// reset the removal time when on a ship..
		m_fRemovalTime = 0.0f;
	}

	NounCargo::simulate( nTick );
}

//----------------------------------------------------------------------------

void CargoEnhancement::createEnhancement()
{
	// if its already created...dont
	if(! m_pEnhancement )
	{
		Factory * pFactory = Factory::findFactory( m_enhancementClassKey );
		if ( pFactory != NULL )
		{
			//create the enhancement 
			m_pEnhancement = WidgetCast<NounEnhancement>( pFactory->createWidget() );
			if ( m_pEnhancement )
			{
				m_pEnhancement->loadIcon();
				m_pEnhancement->setDamage( m_nDamage );
				setName(CharString().format("%s",m_pEnhancement->getName()));
			}
		}

		if (! m_pEnhancement )
		{
			LOG_ERROR( "CargoEnhancement", "Failed to find enhancement key %llu, setting class to NULL.", m_enhancementClassKey );
			m_enhancementClassKey = NULL_CLASS;

			// set to detach from the ship in this case as well..
			setDetach();
		}
	}
}

void CargoEnhancement::setDamage( u8 a_nDamage )
{
	m_nDamage = a_nDamage;
	if ( m_pEnhancement )
		m_pEnhancement->setDamage( m_nDamage );
}

//----------------------------------------------------------------------------
//EOF
