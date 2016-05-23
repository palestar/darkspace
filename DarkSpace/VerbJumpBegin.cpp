/*
	VerbJumpBegin.cpp
	(c)2005 Palestar Inc, Richard Lyle
*/


#include "VerbJumpBegin.h"
#include "GadgetJumpDrive.h"
#include "World/NounTarget.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbJumpBegin, Verb );

BEGIN_PROPERTY_LIST( VerbJumpBegin, Verb )
	ADD_TRANSMIT_PROPERTY( m_pTarget );
END_PROPERTY_LIST();

VerbJumpBegin::VerbJumpBegin()
{}

VerbJumpBegin::VerbJumpBegin( NounShip * pShip, Noun * pTarget ) : m_pTarget( pTarget )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbJumpBegin::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbJumpBegin::scope() const
{
	return LOCAL;
}

bool VerbJumpBegin::client() const
{
	return true;
}

bool VerbJumpBegin::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbJumpBegin::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbJumpBegin::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) && validate( pShip->jumpDrive() ) && validate( m_pTarget ) )
		pShip->jumpDrive()->beginJump( m_pTarget );
}

//----------------------------------------------------------------------------
//EOF
