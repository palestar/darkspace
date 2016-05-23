/*
	VerbDestroyUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbDestroyUnit.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDestroyUnit, Verb );

BEGIN_PROPERTY_LIST( VerbDestroyUnit, Verb )
	ADD_TRANSMIT_PROPERTY( m_pKiller );
END_PROPERTY_LIST();

VerbDestroyUnit::VerbDestroyUnit()
{}

VerbDestroyUnit::VerbDestroyUnit( NounUnit * pUnit, Noun * pKiller ) : m_pKiller( pKiller )
{
	attachVerb( pUnit );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDestroyUnit::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbDestroyUnit::scope() const
{
	return LOCAL;
}

bool VerbDestroyUnit::client() const
{
	return false;	
}

bool VerbDestroyUnit::canAttach( Noun * pNoun )
{
	return WidgetCast<NounUnit>( pNoun ) != NULL;
}

void VerbDestroyUnit::onExecute()
{
	NounUnit * pUnit = WidgetCast<NounUnit>( target() );
	if ( validate( pUnit ) )
		pUnit->destroy( m_pKiller );
}

//----------------------------------------------------------------------------
//EOF
