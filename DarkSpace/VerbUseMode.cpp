/*
	VerbUseMode.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "VerbUseMode.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUseMode, Verb );

BEGIN_PROPERTY_LIST( VerbUseMode, Verb )
	ADD_TRANSMIT_PROPERTY( m_nMode );
END_PROPERTY_LIST();

VerbUseMode::VerbUseMode()
{}

VerbUseMode::VerbUseMode( NounGadget * pGadget, int mode ) : m_nMode( mode )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbUseMode::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbUseMode::scope() const
{
	return LOCAL;
}

bool VerbUseMode::client() const
{
	return true;
}

bool VerbUseMode::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbUseMode::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
		pGadget->useMode( m_nMode );
}

//----------------------------------------------------------------------------
//EOF
