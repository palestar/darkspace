/*
	VerbDestroyShip.cpp
	(c)1999 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbDestroyShip.h"
#include "resource.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbDestroyShip, Verb );

BEGIN_PROPERTY_LIST( VerbDestroyShip, Verb )
	ADD_TRANSMIT_PROPERTY( m_pKiller );
	ADD_TRANSMIT_PROPERTY( m_bZeroVelocity );
END_PROPERTY_LIST();

VerbDestroyShip::VerbDestroyShip()
{}

VerbDestroyShip::VerbDestroyShip( NounShip * pShip, Noun * pKiller, bool bZeroVelocity ) 
	: m_pKiller( pKiller ), m_bZeroVelocity( bZeroVelocity )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbDestroyShip::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbDestroyShip::scope() const
{
	return LOCAL; 
}

bool VerbDestroyShip::client() const
{
	return true;		// this verb is only valid from a client when they are destroying themselves
}

bool VerbDestroyShip::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbDestroyShip::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) )
		pShip->destroyShip( m_pKiller, m_bZeroVelocity );
}

//----------------------------------------------------------------------------
// EOF
