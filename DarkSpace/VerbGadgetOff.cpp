/**
	@file VerbGadgetOff.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 5/26/2008 12:32:03 AM
*/

//---------------------------------------------------------------------------------------------------

#include "Debug/Assert.h"
#include "World/NounTarget.h"
#include "VerbGadgetOff.h"
#include "GameContext.h"

IMPLEMENT_FACTORY( VerbGadgetOff, Verb );

VerbGadgetOff::VerbGadgetOff()
{}

VerbGadgetOff::VerbGadgetOff( NounGadget * pGadget ) 
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbGadgetOff::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbGadgetOff::scope() const
{
	return LOCAL;
}

bool VerbGadgetOff::client() const
{
	return true;
}

bool VerbGadgetOff::queue() const
{
	return false;
}

bool VerbGadgetOff::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbGadgetOff::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
	{
		if ( pGadget->usable( NULL, false ) && pGadget->useActive() )
			pGadget->use( tick(), NULL, false );
	}
}

//---------------------------------------------------------------------------------------------------
//EOF
