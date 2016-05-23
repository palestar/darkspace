/*
	VerbSetStructureActive.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

//----------------------------------------------------------------------------


#include "Debug/Assert.h"
#include "NounPlanet.h"
#include "VerbSetStructureActive.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSetStructureActive, Verb );

BEGIN_PROPERTY_LIST( VerbSetStructureActive, Verb )
	ADD_TRANSMIT_PROPERTY( m_pWho );
	ADD_TRANSMIT_PROPERTY( m_bActive );
END_PROPERTY_LIST();

VerbSetStructureActive::VerbSetStructureActive()
{}

VerbSetStructureActive::VerbSetStructureActive( NounStructure * pStructure, NounShip * pWho, bool active) 
	: m_pWho( pWho ), m_bActive( active )
{
	attachVerb( pStructure );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSetStructureActive::priority() const
{
	return MEDIUM;
}

VerbSetStructureActive::Scope VerbSetStructureActive::scope() const
{
	return LOCAL;
}

bool VerbSetStructureActive::client() const
{
	return true;
}

bool VerbSetStructureActive::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() )
		return m_bActive ? canContinueStructure( WidgetCast<NounStructure>( pNoun ), m_pWho ) : 
			canPauseStructure( WidgetCast<NounStructure>( pNoun ), m_pWho );

	return true;
}

void VerbSetStructureActive::onExecute()
{
	NounStructure * pStructure = WidgetCast<NounStructure>( target() );
	if ( validate( pStructure ) )
	{
		pStructure->setActive( m_bActive );

		if ( pStructure->isServer() && validate( m_pWho ) && validate( pStructure->planet() ) )
		{
			NounPlanet * pPlanet = pStructure->planet();
			ASSERT( pPlanet );

			pStructure->factionChat( m_bActive ? 
				CharString().format( "<color;ffffff>Comms: %s %s activated by %s.", pPlanet->name(), pStructure->name(), m_pWho->name()) :
				CharString().format( "<color;ffffff>Comms: %s %s deactivated by %s.", pPlanet->name(), pStructure->name(), m_pWho->name()) );
		}
	}
}

//----------------------------------------------------------------------------

bool VerbSetStructureActive::canControl( NounStructure * pStructure, NounShip * pWho )
{
	if ( pStructure == NULL || pWho == NULL )
		return false;		// invalid pointers
	NounPlanet * pPlanet = pStructure->planet();
	if ( pPlanet == NULL )
		return false;

	return pPlanet->canGovern( pWho );
}

bool VerbSetStructureActive::canPauseStructure( NounStructure * pStructure, NounShip * pWho )
{
	if ( ! canControl( pStructure, pWho ) )
		return false;
	if ( pStructure->isPaused() )
		return false;		// structure already paused
	NounPlanet * pPlanet = pStructure->planet();
	if (! pPlanet )
		return false;		// not on a planet!
	if (! pPlanet->canPauseStructure(pStructure) )
		return false;		// planet won't allow structure to be paused..

	return true;
}

bool VerbSetStructureActive::canContinueStructure( NounStructure * pStructure, NounShip * pWho )
{
	if ( ! canControl( pStructure, pWho ) )
		return false;
	if ( ! pStructure->isPaused() )
		return false;		// structure is not paused

	return true;
}

//----------------------------------------------------------------------------
//EOF
