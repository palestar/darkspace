/*
	VerbMoveUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounPlanet.h"
#include "NounUnit.h"
#include "VerbMoveUnit.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbMoveUnit, Verb );

BEGIN_PROPERTY_LIST( VerbMoveUnit, Verb )
	ADD_TRANSMIT_PROPERTY( m_nHex );
END_PROPERTY_LIST();

VerbMoveUnit::VerbMoveUnit()
{}

VerbMoveUnit::VerbMoveUnit( NounUnit * pUnit, u16 nHex ) : m_nHex( nHex )
{
	attachVerb( pUnit );
}

//----------------------------------------------------------------------------

Verb::Priority VerbMoveUnit::priority() const
{
	return MEDIUM;
}

VerbMoveUnit::Scope VerbMoveUnit::scope() const
{
	return LOCAL;
}

bool VerbMoveUnit::client() const
{
	return false;
}

bool VerbMoveUnit::canAttach( Noun * pNoun )
{
	return WidgetCast<NounUnit>( pNoun ) != NULL;
}

void VerbMoveUnit::onExecute()
{
	NounUnit * pUnit = WidgetCast<NounUnit>( target() );
	if ( validate( pUnit ) )
		pUnit->setHex( m_nHex, true );
}

//----------------------------------------------------------------------------
//EOF
