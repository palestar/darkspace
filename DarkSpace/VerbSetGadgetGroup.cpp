/*
	VerbSetGadgetGroup.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

//----------------------------------------------------------------------------


#include "Debug/Assert.h"
#include "CargoGadget.h"
#include "GadgetSlot.h"
#include "NounShip.h"
#include "VerbSetGadgetGroup.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSetGadgetGroup, Verb );

BEGIN_PROPERTY_LIST( VerbSetGadgetGroup, Verb )
	ADD_TRANSMIT_PROPERTY( m_Group );
END_PROPERTY_LIST();

VerbSetGadgetGroup::VerbSetGadgetGroup()
{}

VerbSetGadgetGroup::VerbSetGadgetGroup( NounGadget * pGadget, dword group ) 
	: m_Group( group )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSetGadgetGroup::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbSetGadgetGroup::scope() const
{
	return PRIVATE;
}

bool VerbSetGadgetGroup::client() const
{
	return true;
}

bool VerbSetGadgetGroup::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbSetGadgetGroup::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
		pGadget->setGroup( m_Group );
}

//----------------------------------------------------------------------------
//EOF
