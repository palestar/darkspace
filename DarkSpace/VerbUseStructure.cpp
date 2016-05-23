/*
	VerbUseStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbUseStructure.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUseStructure, Verb );

BEGIN_PROPERTY_LIST( VerbUseStructure, Verb )
	ADD_TRANSMIT_PROPERTY( m_pUser );
END_PROPERTY_LIST();

VerbUseStructure::VerbUseStructure()
{}

VerbUseStructure::VerbUseStructure( NounStructure * pStructure, NounShip * pUser ) : m_pUser( pUser )
{
	attachVerb( pStructure );
}

//----------------------------------------------------------------------------

Verb::Priority VerbUseStructure::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbUseStructure::scope() const
{
	return LOCAL;
}

bool VerbUseStructure::client() const
{
	return true;
}

bool VerbUseStructure::canAttach( Noun * pNoun )
{
	return canUse( WidgetCast<NounStructure>( pNoun ), m_pUser );
}

void VerbUseStructure::onExecute()
{
	NounStructure * pStructure = WidgetCast<NounStructure>( target() );
	if ( validate( pStructure ) && validate( m_pUser ) )
		pStructure->use( tick(), m_pUser );
}

//----------------------------------------------------------------------------

bool VerbUseStructure::canUse( NounStructure * pStructure, NounShip * pUser )
{
	if ( pStructure == NULL || pUser == NULL )
		return false;		// invaid pointers
	if ( !pStructure->active() )
		return false;		// structure not active

	return pStructure->usable( pUser );
}

//----------------------------------------------------------------------------
//EOF
