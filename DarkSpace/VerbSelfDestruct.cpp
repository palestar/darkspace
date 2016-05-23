/*
	VerbSelfDestruct.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbSelfDestruct.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSelfDestruct, Verb );

BEGIN_PROPERTY_LIST( VerbSelfDestruct, Verb )
END_PROPERTY_LIST();

VerbSelfDestruct::VerbSelfDestruct()
{}

VerbSelfDestruct::VerbSelfDestruct( NounShip * pShip ) 
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSelfDestruct::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbSelfDestruct::scope() const
{
	return LOCAL; 
}

bool VerbSelfDestruct::client() const
{
	return true;		// this verb is only valid from a client when they are destroying themselves
}

bool VerbSelfDestruct::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbSelfDestruct::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) )
		pShip->destroyShip( pShip, false );
}

//----------------------------------------------------------------------------
// EOF
