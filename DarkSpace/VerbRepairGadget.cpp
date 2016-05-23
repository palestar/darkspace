/*
	VerbRepairGadget.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "VerbRepairGadget.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbRepairGadget, Verb );

BEGIN_PROPERTY_LIST( VerbRepairGadget, Verb )
	ADD_TRANSMIT_PROPERTY( m_When );
END_PROPERTY_LIST();

VerbRepairGadget::VerbRepairGadget()
{}

VerbRepairGadget::VerbRepairGadget( NounGadget * pGadget, When when ) 
	: m_When( when )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbRepairGadget::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbRepairGadget::scope() const
{
	return LOCAL;
}

bool VerbRepairGadget::client() const
{
	return true;
}

bool VerbRepairGadget::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbRepairGadget::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
	{
		NounShip * pShip = WidgetCast<NounShip>( pGadget->parentBody() );
		if ( validate( pShip ) )
			pShip->repairGadget( pGadget, m_When );
	}
}

//----------------------------------------------------------------------------
// EOF
