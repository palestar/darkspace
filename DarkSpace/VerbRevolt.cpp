/*
	VerbRevolt.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounBody.h"
#include "VerbRevolt.h"
#include "GameContext.h"
#include "resource.h"
#include "NounUnit.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbRevolt, Verb );

BEGIN_PROPERTY_LIST( VerbRevolt, Verb )
	ADD_TRANSMIT_PROPERTY( m_nFleetId );
END_PROPERTY_LIST();

VerbRevolt::VerbRevolt()
{}

VerbRevolt::VerbRevolt( NounPlanet * pPlanet, int nFleetId ) 
	: m_nFleetId( nFleetId )
{
	attachVerb( pPlanet );
}

//----------------------------------------------------------------------------

Verb::Priority VerbRevolt::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbRevolt::scope() const
{
	return GLOBAL;
}

bool VerbRevolt::client() const
{
	return false;
}

bool VerbRevolt::canAttach( Noun * pNoun )
{
	return WidgetCast<NounPlanet>( pNoun ) != NULL;
}

void VerbRevolt::onExecute()
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( target() );
	if ( validate( pPlanet ) )
	{
		// notify the planet's current owners the planet has revolted...
		CharString sMessage;
		if ( pPlanet->context()->isTeamValid( m_nFleetId ) )
		{
			sMessage.format( "<color;ffffff>NEWS: Revolt on %s, planet now controlled by %s.", 
				pPlanet->name(), pPlanet->context()->team( m_nFleetId ).name.cstr() );
		}
		else
		{
			sMessage.format( "<color;ffffff>NEWS: Revolt on %s, planet now controlled by rebels.", pPlanet->name() );
		}
		pPlanet->clientChat( sMessage );

		// planet captured set the owner
		pPlanet->setTeamId( m_nFleetId );
		pPlanet->setMoral( 1.0f );
		pPlanet->setControl( pPlanet->maxControl() );

		// change the ownership on all units on the planet
		for(int i=0;i<pPlanet->childCount();i++)
		{
			NounUnit * pUnit = WidgetCast<NounUnit>( pPlanet->child(i) );
			if ( pUnit != NULL )
			{
				pUnit->setTeamId( m_nFleetId );
				pUnit->setUserId( 0 );	// clear any user ID, otherwise a player could lose prestige if this unit causes damage to their own 
			}
		}

		// inform planets new owners... if any..
		pPlanet->clientChat( sMessage );
	}
}

//----------------------------------------------------------------------------
//EOF
