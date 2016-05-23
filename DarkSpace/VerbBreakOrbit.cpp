/*
	VerbBreakOrbit.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounBody.h"
#include "NounShip.h"
#include "VerbBreakOrbit.h"
#include "TraitOrbit.h"
#include "resource.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbBreakOrbit, Verb );

BEGIN_PROPERTY_LIST( VerbBreakOrbit, Verb )
	ADD_TRANSMIT_PROPERTY( m_vPosition );
END_PROPERTY_LIST();


VerbBreakOrbit::VerbBreakOrbit()
{}

VerbBreakOrbit::VerbBreakOrbit( Noun * pTarget ) : m_vPosition( pTarget->position() )
{
	attachVerb( pTarget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbBreakOrbit::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbBreakOrbit::scope() const
{
	return GLOBAL;		// needs to be global so we can update proxy objects as well..
}

bool VerbBreakOrbit::client() const
{
	return true;
}

bool VerbBreakOrbit::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbBreakOrbit::canAttach( Noun * pNoun )
{
	if ( pNoun == NULL )
		return false;			// invalid pointer

	TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pNoun );
	if ( pOrbit == NULL )
		return false;			// object does not orbit
	if ( pOrbit->orbiting() == NULL )
		return false;			// noun not in orbit

	return true;
}

void VerbBreakOrbit::onExecute()
{
	Noun * pTarget = target();
	if ( validate( pTarget ) )
	{
		Noun * pOrbiting = NULL;

		TraitOrbit * pOrbit = FindTrait<TraitOrbit>( pTarget );
		if ( pOrbit != NULL )
		{
			Noun::Ref pOrbiting = pOrbit->orbiting();		// save off our orbit target for the message below
			pOrbit->breakOrbit();

			NounShip * pShip = WidgetCast<NounShip>( pTarget );
			if ( pShip != NULL )
			{
				pShip->setPosition( m_vPosition );
				if ( pOrbiting != NULL )
					pShip->message( CharString().format( "<color;ffffff>Helm: Leaving orbit of %s...", pOrbiting->name() ) );
			}
			else
			{
				// just set the position on all other types of nouns
				pTarget->setPosition( m_vPosition );
			}
		}
	}
}

//----------------------------------------------------------------------------
// EOF
