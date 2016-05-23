/*
	VerbJumpAbort.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "VerbJumpAbort.h"
#include "GadgetJumpDrive.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbJumpAbort, Verb );

BEGIN_PROPERTY_LIST( VerbJumpAbort, Verb )
END_PROPERTY_LIST();

VerbJumpAbort::VerbJumpAbort()
{}

VerbJumpAbort::VerbJumpAbort( NounShip * pShip )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbJumpAbort::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbJumpAbort::scope() const
{
	return LOCAL;
}

bool VerbJumpAbort::client() const
{
	return true;
}

bool VerbJumpAbort::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbJumpAbort::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbJumpAbort::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) && validate( pShip->jumpDrive() ) )
		pShip->jumpDrive()->abortJump( tick() );
}

//----------------------------------------------------------------------------
//EOF
