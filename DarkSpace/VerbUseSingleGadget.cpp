/*
	VerbUseSingleGadget.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "World/NounTarget.h"
#include "VerbUseSingleGadget.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUseSingleGadget, Verb );

BEGIN_PROPERTY_LIST( VerbUseSingleGadget, Verb )
	ADD_TRANSMIT_PROPERTY( m_bShift );
	ADD_TRANSMIT_PROPERTY( m_pTarget );
END_PROPERTY_LIST();

VerbUseSingleGadget::VerbUseSingleGadget()
{}

VerbUseSingleGadget::VerbUseSingleGadget( NounGadget * pGadget, Noun * pTarget, bool shift /*= false*/ ) 
	: m_pTarget( pTarget ), m_bShift( shift )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbUseSingleGadget::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbUseSingleGadget::scope() const
{
	return LOCAL;
}

bool VerbUseSingleGadget::client() const
{
	return true;
}

bool VerbUseSingleGadget::queue() const
{
	return true;
}

bool VerbUseSingleGadget::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbUseSingleGadget::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
	{
		if ( pGadget->usable( m_pTarget, m_bShift ) )
			pGadget->use( tick(), m_pTarget, m_bShift );
	}
}

//----------------------------------------------------------------------------
//EOF
