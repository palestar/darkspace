/*
	VerbJump.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "VerbJump.h"
#include "GadgetJumpDrive.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbJump, Verb );

BEGIN_PROPERTY_LIST( VerbJump, Verb )
	ADD_TRANSMIT_PROPERTY( m_nJumpTime );
	ADD_TRANSMIT_PROPERTY( m_vJumpVelocity );
END_PROPERTY_LIST();

VerbJump::VerbJump()
{}

VerbJump::VerbJump( NounShip * pShip, dword jumpTime, const Vector3 & jump ) 
	: m_nJumpTime( jumpTime ), m_vJumpVelocity( jump )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbJump::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbJump::scope() const
{
	return LOCAL;
}

bool VerbJump::client() const
{
	return true;
}

bool VerbJump::queue() const
{
	return true;
}

//----------------------------------------------------------------------------

bool VerbJump::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbJump::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) && validate( pShip->jumpDrive() ) )
		pShip->jumpDrive()->doJump( tick(), m_nJumpTime, m_vJumpVelocity );
}

//----------------------------------------------------------------------------
//EOF
