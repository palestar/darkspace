/*
	VerbSetDetectionFlags.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounPlanet.h"
#include "VerbSetDetectionFlags.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbSetDetectionFlags, Verb );

BEGIN_PROPERTY_LIST( VerbSetDetectionFlags, Verb )
	ADD_TRANSMIT_PROPERTY( m_nFlags );
END_PROPERTY_LIST();

VerbSetDetectionFlags::VerbSetDetectionFlags()
{}

VerbSetDetectionFlags::VerbSetDetectionFlags( Noun * pTarget, dword nFlags ) 
	: m_nFlags( nFlags )
{
	attachVerb( pTarget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbSetDetectionFlags::priority() const
{
	return LOW;
}

VerbSetDetectionFlags::Scope VerbSetDetectionFlags::scope() const
{
	return GLOBAL;
}

bool VerbSetDetectionFlags::client() const
{
	return false;
}

bool VerbSetDetectionFlags::checkNoun() const
{
	return true;
}

bool VerbSetDetectionFlags::canAttach( Noun * pNoun )
{
	return WidgetCast<NounGame>( pNoun ) && ((NounGame *)pNoun)->canBeDetected();
}

void VerbSetDetectionFlags::onExecute()
{
	NounGame * pTarget = WidgetCast<NounGame>( target() );
	if ( validate( pTarget ) )
		pTarget->setDetectionFlags( m_nFlags );
}

//----------------------------------------------------------------------------
//EOF
