/*
	VerbDestroyGadget.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbDestroyGadget.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDestroyGadget, Verb );

BEGIN_PROPERTY_LIST( VerbDestroyGadget, Verb )
	ADD_TRANSMIT_PROPERTY( m_pFrom );
END_PROPERTY_LIST();

VerbDestroyGadget::VerbDestroyGadget()
{}

VerbDestroyGadget::VerbDestroyGadget( NounGadget * pGadget, Noun * pFrom ) : m_pFrom( pFrom )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDestroyGadget::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbDestroyGadget::scope() const
{
	return LOCAL;
}

bool VerbDestroyGadget::client() const
{
	return false;
}

bool VerbDestroyGadget::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGadget>( pNoun ) != NULL;
}

void VerbDestroyGadget::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if ( validate( pGadget ) )
		pGadget->inflictDamage( tick(), m_pFrom, pGadget->maxDamage(), DAMAGE_EMP, Vector3( 0.0f ) );
}

//----------------------------------------------------------------------------
//EOF
