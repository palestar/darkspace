/*
	VerbScrap.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbScrap.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbScrap, Verb );

BEGIN_PROPERTY_LIST( VerbScrap, Verb )
	ADD_TRANSMIT_PROPERTY( m_pWho );
END_PROPERTY_LIST();

VerbScrap::VerbScrap()
{}

VerbScrap::VerbScrap( NounStructure * pScrap, NounShip * pWho ) : m_pWho( pWho )
{
	attachVerb( pScrap );
}

//----------------------------------------------------------------------------

Verb::Priority VerbScrap::priority() const
{
	return MEDIUM;
}

VerbScrap::Scope VerbScrap::scope() const
{
	return LOCAL;
}

bool VerbScrap::client() const
{
	return true;
}

bool VerbScrap::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
		return canScrap( WidgetCast<NounStructure>( pNoun ), m_pWho );

	return WidgetCast<NounStructure>( pNoun ) != NULL;
}

extern Constant BUILD_POINTS_PER_SECOND;

void VerbScrap::onExecute()
{
	NounStructure * pScrap = WidgetCast<NounStructure>( target() );
	if ( validate( pScrap ) )
	{
		if ( validate( m_pWho ) )
		{
			NounPlanet * pPlanet = pScrap->planet();
			ASSERT( pPlanet );

			if ( pPlanet->isServer() )
				pPlanet->factionChat( CharString().format( "<color;ffffff>Comms: %s %s scrapped by %s.", 
					pPlanet->name(), pScrap->name(), m_pWho->name()) );

			float fPoints = BUILD_POINTS_PER_SECOND * pScrap->buildTime();
			float fDiv = pPlanet->control() / pPlanet->maxControl();
			fPoints *= 2 - fDiv; // lose more points if we've lost control
			((GameContext *)context())->gameUser()->onConstruct( m_pWho, -fPoints );
		}

		pScrap->scrap();
	}
}

//----------------------------------------------------------------------------

bool VerbScrap::canScrap( NounStructure * pScrap, NounShip * pWho )
{
	if ( pScrap == NULL || pWho == NULL )
		return false;
	NounPlanet * pPlanet = pScrap->planet();
	if ( pPlanet == NULL )
		return false;		// structure not attached to planet, already scrapped!
	if ( pWho->type() != NounShip::ENGINEER && pWho->orbiting() != pPlanet )
		return false;		// non-engineer ships need to be in orbit to scrap
	if ( pWho->type() == NounShip::ENGINEER && pWho->starSystem() != pPlanet->starSystem() )
		return false;		// engineers have to be in the same system to scrap
	
	return pPlanet->canGovern( pWho );
}

//----------------------------------------------------------------------------
//EOF
