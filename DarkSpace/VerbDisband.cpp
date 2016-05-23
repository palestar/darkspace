/*
	VerbDisband.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbDisband.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDisband, Verb );

BEGIN_PROPERTY_LIST( VerbDisband, Verb )
	ADD_TRANSMIT_PROPERTY( m_pWho );
END_PROPERTY_LIST();

VerbDisband::VerbDisband()
{}

VerbDisband::VerbDisband( NounUnit * pDisband, Noun * pWho ) : m_pWho( pWho )
{
	attachVerb( pDisband );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDisband::priority() const
{
	return MEDIUM;
}

VerbDisband::Scope VerbDisband::scope() const
{
	return SERVER;
}

bool VerbDisband::client() const
{
	return true;
}

bool VerbDisband::canAttach( Noun * pNoun )
{
	return canDisband( WidgetCast<NounUnit>( pNoun ), m_pWho );
}

void VerbDisband::onExecute()
{
	NounUnit * pUnit = WidgetCast<NounUnit>( target() );
	if ( validate( pUnit ) && validate( m_pWho ) )
		pUnit->disband( m_pWho );
}

//----------------------------------------------------------------------------

bool VerbDisband::canDisband( NounUnit * pDisband, Noun * pWho )
{
	if ( pDisband == NULL || pWho == NULL )
		return false;
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( pDisband->parent() );
	if ( pPlanet == NULL )
		return false;		// unit not attached to planet, may be onboard ship or already disbanned
	if ( pDisband->factionId() != pPlanet->factionId() )
		return false;		// cannot disband enemy units
	if ( pWho == pPlanet ) 
		return true;		// planet can always disband it's own units

	return pPlanet->canGovern( pWho );
}

//----------------------------------------------------------------------------
//EOF
