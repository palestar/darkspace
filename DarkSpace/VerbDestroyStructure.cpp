/*
	VerbDestroyStructure.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "SceneryEffect.h"
#include "VerbDestroyStructure.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDestroyStructure, Verb );

BEGIN_PROPERTY_LIST( VerbDestroyStructure, Verb )
	ADD_TRANSMIT_PROPERTY( m_pKiller );
END_PROPERTY_LIST();

VerbDestroyStructure::VerbDestroyStructure()
{}

VerbDestroyStructure::VerbDestroyStructure( NounStructure * pStructure, Noun * pKiller ) : m_pKiller( pKiller )
{
	attachVerb( pStructure );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDestroyStructure::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbDestroyStructure::scope() const
{
	return LOCAL;
}

bool VerbDestroyStructure::client() const
{
	return false;	
}

bool VerbDestroyStructure::canAttach( Noun * pNoun )
{
	return WidgetCast<NounStructure>( pNoun ) != NULL;
}

void VerbDestroyStructure::onExecute()
{
	NounStructure * pStructure = WidgetCast<NounStructure>( target() );
	if ( validate( pStructure ) )
		pStructure->destroy( m_pKiller );
}


//----------------------------------------------------------------------------
//EOF
