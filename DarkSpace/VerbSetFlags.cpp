/*
	VerbSetFlags.cpp
	(c)2005 Palestar Inc, Richard Lyle
*/

#include "NounGame.h"
#include "VerbSetFlags.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSetFlags, Verb );

BEGIN_PROPERTY_LIST( VerbSetFlags, Verb )
	ADD_TRANSMIT_PROPERTY( m_nFlags );
END_PROPERTY_LIST();

VerbSetFlags::VerbSetFlags()
{}

VerbSetFlags::VerbSetFlags( Noun * pTarget, dword nFlags ) 
	: m_nFlags( nFlags )
{
	attachVerb( pTarget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSetFlags::priority() const
{
	return LOW;
}

VerbSetFlags::Scope VerbSetFlags::scope() const
{
	return GLOBAL;
}

bool VerbSetFlags::client() const
{
	return false;
}

bool VerbSetFlags::canAttach( Noun * pNoun )
{
	return pNoun != NULL;
}

void VerbSetFlags::onExecute()
{
	Noun * pTarget = target();
	if ( validate( pTarget ) )
		pTarget->setFlags( m_nFlags );
}

//----------------------------------------------------------------------------
//EOF
