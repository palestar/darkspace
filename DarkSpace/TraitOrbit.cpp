/*
	TraitOrbit.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "TraitOrbit.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( TraitOrbit, Trait );
REGISTER_FACTORY_KEY( TraitOrbit, 4780777174566942733LL );

BEGIN_PROPERTY_LIST( TraitOrbit, Trait )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_pOrbiting );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fOrbitDistance );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fOrbitPosition );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fOrbitVelocity );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fRotVelocity );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nBaseTick );
END_PROPERTY_LIST();

TraitOrbit::TraitOrbit()
	: m_fOrbitDistance( 0 ), m_fOrbitPosition( 0 ), m_fOrbitVelocity( 0 ), m_fRotVelocity( 0.0f ), m_nBaseTick( 0 )
{}

//---------------------------------------------------------------------------------------------------

bool TraitOrbit::read( const InStream & input )
{
	if ( (input.filter() && FF_TRANSMIT) != 0 )
	{
		bool bOrbiting = false;
		input >> bOrbiting;

		if ( bOrbiting )
		{
			if (! Trait::read( input ) )
				return false;
		}

		return true;
	}

	return Trait::read( input );
}

bool TraitOrbit::write( const OutStream & output )
{
	if ( (output.filter() && FF_TRANSMIT) != 0 )
	{
		if ( m_pOrbiting.valid() )
		{
			output << true;
			if (! Trait::write( output ) )
				return false;
		}
		else
		{
			output << false;
		}

		return true;
	}

	return Trait::write( output );
}

//----------------------------------------------------------------------------

void TraitOrbit::initialize()
{
	Trait::initialize();
	m_pMovement = FindTrait<TraitMovement>( parent() );
}

void TraitOrbit::simulate( dword nTick )
{
	dynamics( nTick );
	Trait::simulate( nTick );
}

void TraitOrbit::setOrbit( Noun * pOrbit, float fDistance, float fPosition, float fVelocity, dword nTick )
{
	m_pOrbiting = pOrbit;
	m_fOrbitDistance = fDistance;
	m_fOrbitPosition = fPosition;
	m_fOrbitVelocity = fVelocity;
	m_vLastPosition = parent()->position();
	m_nBaseTick = nTick;

	// set the world position for our parent now.. call dynamics() even if nTicks is 0, because it
	// sets other variables such as the base velocity... etc
	dynamics( nTick );
}

void TraitOrbit::setRotationalVelocity( float a_fRotVelocity )
{
	m_fRotVelocity = a_fRotVelocity;
}

void TraitOrbit::breakOrbit()
{
	m_pOrbiting = NULL;
}

void TraitOrbit::dynamics( dword nTick )
{
	// update the position if in orbit
	if ( m_pOrbiting.valid() )
	{
		NodeZone * pZone = parent()->zone();
		if (! pZone )
			pZone = m_pOrbiting->zone();

		float fElapsed = TICK_DURATION_S * (nTick - m_nBaseTick);
		// update the orbit position
		float fPosition = m_fOrbitPosition + (m_fOrbitVelocity * fElapsed);
		// calculate our new zone position
		Vector3 vPosition = m_pOrbiting->zonePosition( pZone ) + 
			(Vector3( sinf( fPosition ), 0, cosf( fPosition ) ) * m_fOrbitDistance );

		// update the position in TraitMovement, so it will not attempt send movement packets more than normal. 
		// since orbits are circular and movement uses a linear prediction, it will trigger a movement packet 
		// more often than normal unless we do this.
		if ( m_pMovement )
			m_pMovement->setPosition( vPosition );
		// rotate the object
		if ( m_fRotVelocity != 0.0f )
			parent()->setOrientation( Euler( 0.0f, m_fRotVelocity * fElapsed, 0.0f ) );

		// set our world position
		parent()->setPosition( vPosition );
		parent()->invalidateWorld();

		parent()->setVelocity( (vPosition - m_vLastPosition) / TICK_DURATION_S );
		m_vLastPosition = vPosition;
	}
}

//----------------------------------------------------------------------------
//EOF
