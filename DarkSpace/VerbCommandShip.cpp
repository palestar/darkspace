/*
	VerbCommandShip.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

//----------------------------------------------------------------------------


#include "Debug/Assert.h"
#include "World/NounTarget.h"
#include "VerbCommandShip.h"
#include "VerbEnterOrbit.h"
#include "resource.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbCommandShip, Verb );

BEGIN_PROPERTY_LIST( VerbCommandShip, Verb )
	ADD_TRANSMIT_PROPERTY( m_nCommand );
	ADD_TRANSMIT_PROPERTY( m_pTarget );
END_PROPERTY_LIST();

VerbCommandShip::VerbCommandShip()
{}

VerbCommandShip::VerbCommandShip( NounShip * pShip, Command nCommand, Noun * pTarget) 
	: m_nCommand( nCommand ), m_pTarget( pTarget )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbCommandShip::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbCommandShip::scope() const
{
	return PRIVATE;
}

bool VerbCommandShip::client() const
{
	return true;
}

bool VerbCommandShip::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbCommandShip::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) )
		pShip->setCommand( m_nCommand, m_pTarget );
}

//----------------------------------------------------------------------------

bool VerbCommandShip::canCommand( NounShip * pShip, Command nCommand, Noun * pTarget )
{
	if ( pShip != NULL )
		return pShip->canCommand( nCommand, pTarget );
	return false;
}

//----------------------------------------------------------------------------
// EOF
