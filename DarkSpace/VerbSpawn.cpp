/*
	VerbSpawn.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounJumpGate.h"
#include "VerbSpawn.h"
#include "GameContext.h"

extern Constant PLANET_SPAWN_COSTS;

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSpawn, Verb );

BEGIN_PROPERTY_LIST( VerbSpawn, Verb )
	ADD_TRANSMIT_PROPERTY( m_pSelf );
	ADD_TRANSMIT_PROPERTY( m_nResourceCost );
END_PROPERTY_LIST();

VerbSpawn::VerbSpawn() : m_nResourceCost( 0 )
{}

VerbSpawn::VerbSpawn( Noun * pSpawn, NounShip * pShip, int nResourceCost ) 
	: m_pSelf( pShip ), m_nResourceCost( nResourceCost )
{
	attachVerb( pSpawn );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSpawn::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbSpawn::scope() const
{
	return LOCAL;
}

bool VerbSpawn::client() const
{
	return false;
}

bool VerbSpawn::canAttach( Noun * pNoun )
{
	return WidgetCast<NounJumpGate>( pNoun ) || WidgetCast<NounPlanet>( pNoun );
}

void VerbSpawn::onExecute()
{
	Noun * pSpawn = target();
	if ( validate( pSpawn ) && validate( m_pSelf ) )
	{
		if ( context()->isClient() )
		{
			// create the jump-in effect if the spawn point is a jump gate
			if ( WidgetCast<NounJumpGate>( pSpawn ) )
				((NounJumpGate *)pSpawn)->createUseEffect();
		}

		if ( WidgetCast<NounPlanet>( pSpawn ) )
			((NounPlanet *)pSpawn)->useResources( m_nResourceCost );
	}
}

//----------------------------------------------------------------------------
// EOF
