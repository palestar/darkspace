/*
	NounDropTarget.cpp

	Target object
	(c)1999 PaleStar Development, Richard Lyle
*/


#include "Math/Helpers.h"
#include "Constants.h"
#include "NounDropTarget.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounDropTarget, NounTarget );
REGISTER_FACTORY_KEY( NounDropTarget, 4247766991935795275LL );

BEGIN_PROPERTY_LIST( NounDropTarget, NounTarget )
	ADD_TRANSMIT_PROPERTY( m_Hex );
	ADD_TRANSMIT_PROPERTY( m_Planet );
END_PROPERTY_LIST();

NounDropTarget::NounDropTarget()
{}

NounDropTarget::NounDropTarget( NounPlanet * pPlanet, int hex ) 
	: NounTarget( pPlanet->hex( hex ).position ), m_Planet( pPlanet ), m_Hex( hex )
{
	// set the parent to the planet, since this object is never really attached to the planet
	setContext( pPlanet->context() );
	setName( CharString().format("%s:%d", m_Planet->name(), m_Hex) );
	setZone( pPlanet->zone() );
}

//----------------------------------------------------------------------------

bool NounDropTarget::read( const InStream & input )
{
	if (! NounTarget::read( input ) )
		return false;

	if ( m_Planet.valid() )
	{
		setName( CharString().format("%s:%d", m_Planet->name(), m_Hex) );
		setContext( m_Planet->context() );
	}

	return true;
}

void NounDropTarget::calculateWorld() const
{
	if ( m_Planet.valid() )
	{
		Vector3 vTranslate( m_Planet->worldFrame() % position() );
		m_WorldPosition = m_Planet->worldPosition() + vTranslate;
		m_vZonePosition = m_Planet->zonePosition() + vTranslate;
	}
}

//----------------------------------------------------------------------------
// EOF

