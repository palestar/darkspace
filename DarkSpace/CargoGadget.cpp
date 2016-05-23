/*
	CargoGadget.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "NounShip.h"
#include "NounPlanet.h"
#include "StructurePort.h"
#include "GameContext.h"
#include "CargoGadget.h"

//----------------------------------------------------------------------------

const int CARGO_GADGET_LIFE = TICKS_PER_SECOND * 600;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( CargoGadget, NounCargo );
REGISTER_FACTORY_KEY( CargoGadget, 4335719361658260400LL );

BEGIN_PROPERTY_LIST( CargoGadget, NounCargo )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nLife );
	ADD_TRANSMIT_PROPERTY( m_pGadget );
END_PROPERTY_LIST();

CargoGadget::CargoGadget() : m_nLife( CARGO_GADGET_LIFE )
{
	setName( "Rubbish" );
}

CargoGadget::CargoGadget( NounGadget * pGadget ) : m_nLife( CARGO_GADGET_LIFE )
{
	setGadget( pGadget );
}

//----------------------------------------------------------------------------

const int VERSION_062802 = 62802;
const int VERSION_20050128 = 20050128;
const int VERSION_20050506 = 20050506;
const int VERSION_20050621 = 20050621;

bool CargoGadget::read( const InStream & input )
{
	if (! NounCargo::read( input ) )
		return false;
	// update this object name
	setName( CharString( nounContext()->name() ) );
	return true;
}

//----------------------------------------------------------------------------

void CargoGadget::simulate( dword nTick )
{
	BaseNode * pParent = parent();
	
	// unless gadget is on a ship then convert it back into resources
	if ( WidgetCast<NounShip>( pParent ) == NULL )
	{
		if ( isServer() )
		{
			// convert into resources and detach from the port
			if ( m_pGadget.valid() )
				context()->attachNoun( new CargoResource( m_pGadget->buildCost()), (Noun *)pParent );
			setDetach();
		}
	}

	NounCargo::simulate( nTick );
}

//----------------------------------------------------------------------------

int CargoGadget::value() const
{
	return (m_pGadget->addValue() * (m_pGadget->level() + 1)) * quantity();
}

int CargoGadget::maxQuantity() const
{
	return 1;
}

//----------------------------------------------------------------------------

NounGadget * CargoGadget::gadget() const
{
	return m_pGadget;
}

CargoGadget::Type CargoGadget::type() const
{
	return m_pGadget.valid() ? m_pGadget->type() : NounGadget::UNKNOWN;
}

int CargoGadget::damage() const
{
	return m_pGadget.valid() ? m_pGadget->damage() : 0;
}

int	CargoGadget::level() const
{
	return m_pGadget.valid() ? m_pGadget->level() : 0;
}

//----------------------------------------------------------------------------

void CargoGadget::setGadget( NounGadget * pGadget )
{
	if ( pGadget != NULL )
	{
		setNounContext( pGadget->nounContext() );
		setName( CharString( nounContext()->name() ) );
		
		// put a copy of the provided gadget into this cargo object
		m_pGadget = WidgetCast<NounGadget>( pGadget->copy() );
		// set the life of this cargo item
		m_nLife = CARGO_GADGET_LIFE;
	}
	else
	{
		setNounContext( NULL_WIDGET );
		setName( "Rubbish" );
	}
}

//----------------------------------------------------------------------------

Material * CargoGadget::icon() const
{
	return WidgetCast<Material>( nounContext()->resource( "ICON" ) );
}

//----------------------------------------------------------------------------
//EOF
