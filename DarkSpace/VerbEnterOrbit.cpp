/*
	VerbEnterOrbit.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "VerbEnterOrbit.h"
#include "TraitOrbit.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

static Constant MAX_SHIP_ORBIT_DISTANCE( "MAX_SHIP_ORBIT_DISTANCE", 150.0f );
static Constant MAX_SHIP_ORBIT_VELOCITY( "MAX_SHIP_ORBIT_VELOCITY", 4.0f );

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbEnterOrbit, Verb );

BEGIN_PROPERTY_LIST( VerbEnterOrbit, Verb )
	ADD_TRANSMIT_PROPERTY( m_pOrbit );
	ADD_TRANSMIT_PROPERTY( m_fPosition );
	ADD_TRANSMIT_PROPERTY( m_fDistance );
	ADD_TRANSMIT_PROPERTY( m_fVelocity );
END_PROPERTY_LIST();

VerbEnterOrbit::VerbEnterOrbit()
{}

VerbEnterOrbit::VerbEnterOrbit( Noun * pTarget, Noun * pOrbit, float fVelocity ) 
	: m_pOrbit( pOrbit ), m_fVelocity( 0.0f )
{
	Vector3 vDelta( pTarget->worldPosition() - pOrbit->worldPosition() );
	m_fPosition = atan2( vDelta.x, vDelta.z );
	m_fDistance = vDelta.magnitude();
	m_fVelocity = (fVelocity / m_fDistance) * PI;

	attachVerb( pTarget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbEnterOrbit::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbEnterOrbit::scope() const
{
	return GLOBAL;
}

bool VerbEnterOrbit::client() const
{
	return true;
}

bool VerbEnterOrbit::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbEnterOrbit::canAttach( Noun * pNoun )
{
	//if ( pNoun->server() )
	//	return canEnterOrbit( pNoun, m_pOrbit );
	return true;
}

void VerbEnterOrbit::onExecute()
{
	Noun * pTarget = target();
	if ( validate( pTarget ) && validate( m_pOrbit ) )
	{
		// set the orbit trait properties
		TraitOrbit * pOrbitDynamics = FindTrait<TraitOrbit>( pTarget );
		if ( pOrbitDynamics != NULL )
			pOrbitDynamics->setOrbit( m_pOrbit, m_fDistance, m_fPosition, m_fVelocity, tick() );

		NounShip * pShip = WidgetCast<NounShip>( pTarget );
		if ( pShip != NULL )
		{
			pShip->setVelocity( 0.0f );
			pShip->message( CharString().format( "<color;ffffff>Helm: Orbiting %s.", m_pOrbit->name() ) );
		}
	}
}

//----------------------------------------------------------------------------

bool VerbEnterOrbit::canEnterOrbit( Noun * pTarget, Noun * pOrbit )
{
	if (! canOrbit( pTarget, pOrbit ) )
		return false;		// cannot orbit
	
	if ( WidgetCast<NounShip>( pTarget ) )
	{
		NounShip * pShip = (NounShip *)pTarget;

		float fDistance = (pShip->worldPosition() - pOrbit->worldPosition()).magnitude();
		fDistance -= (pOrbit->radius() + pShip->radius());

		if ( fDistance > MAX_SHIP_ORBIT_DISTANCE )
			return false;		// out of range
		if ( pShip->velocity() > MAX_SHIP_ORBIT_VELOCITY )
			return false;		// moving too fast
	}
	else
	{
		// TODO: use mass of both objects to determine if there is enough gravity to keep them in orbit
	}

	return true;
}

bool VerbEnterOrbit::canOrbit( Noun * pTarget, Noun * pOrbit )
{
	if ( !pTarget || !WidgetCast<NounGame>( pOrbit ) )
		return false;		// invalid objects
	if (! ((NounGame *)pOrbit)->canOrbit() )
		return false;		// object cannot be orbited

	TraitOrbit * pTrait = (TraitOrbit *)pTarget->findTrait( CLASS_KEY(TraitOrbit) );
	if (! pTrait )
		return false;		// this object cannot orbit
	if ( pTrait->orbiting() == pOrbit )
		return false;		// already in orbit

	return true;
}

//----------------------------------------------------------------------------
// EOF
