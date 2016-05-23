/*
	VerbUseGadget.cpp

	This verb calls the use() mutator on a gadget object
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Debug/Profile.h"
#include "World/NounTarget.h"
#include "VerbUseGadget.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUseGadget, Verb );

BEGIN_PROPERTY_LIST( VerbUseGadget, Verb )
	ADD_TRANSMIT_PROPERTY( m_Gadgets );
	ADD_TRANSMIT_PROPERTY( m_Shift );
	ADD_TRANSMIT_PROPERTY( m_Target );
END_PROPERTY_LIST();

VerbUseGadget::VerbUseGadget()
{}

VerbUseGadget::VerbUseGadget( Noun * pUser, const Array< NounGadget * > & a_Gadgets, Noun * pTarget, bool shift /*= false*/ ) 
	: m_Gadgets( 0 ), m_Target( pTarget ), m_Shift( shift )
{
	int bit = 0;
	for(int i=0;i<pUser->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pUser->child(i) );
		if ( pGadget == NULL )
			continue;
		if ( a_Gadgets.search( pGadget ) >= 0 )
			m_Gadgets |= (((qword)1) << bit);

		bit++;
	}

	if ( m_Gadgets != 0 )
		attachVerb( pUser );
}

//----------------------------------------------------------------------------

Verb::Priority VerbUseGadget::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbUseGadget::scope() const
{
	return LOCAL;
}

bool VerbUseGadget::client() const
{
	return true;
}

bool VerbUseGadget::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbUseGadget::canAttach( Noun * pNoun )
{
	return pNoun != NULL;
}

void VerbUseGadget::onExecute()
{
	Noun * pUser = target();
	if ( validate( pUser ) )
	{
		// use all usable gadgets on target
		int bit = 0;
		for(int i=0;i<pUser->childCount();i++)
		{
			NounGadget * pGadget = WidgetCast<NounGadget>( pUser->child(i) );
			if ( pGadget == NULL )
				continue;

			if ( m_Gadgets & (((qword)1) << bit) )
			{
				if ( pGadget->usable( m_Target, m_Shift ) )
					pGadget->use( tick(), m_Target, m_Shift );
			}

			bit++;
		}
	}
}

//----------------------------------------------------------------------------
// EOF
