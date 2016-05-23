/*
	VerbOrderShip.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounDropTarget.h"
#include "VerbOrderShip.h"
#include "resource.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbOrderShip, Verb );

BEGIN_PROPERTY_LIST( VerbOrderShip, Verb )
	ADD_TRANSMIT_PROPERTY( m_nOrder );
	ADD_TRANSMIT_PROPERTY( m_pTarget );
	ADD_TRANSMIT_PROPERTY( m_pFrom );
END_PROPERTY_LIST();

VerbOrderShip::VerbOrderShip()
{}

VerbOrderShip::VerbOrderShip( NounShip * pShip, Order nOrder, Noun * pTarget, NounShip * pFrom ) 
	: m_nOrder( nOrder ), m_pTarget( pTarget ), m_pFrom( pFrom )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbOrderShip::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbOrderShip::scope() const
{
	return FACTION;
}

bool VerbOrderShip::client() const
{
	return true;
}

bool VerbOrderShip::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
	//return canOrderShip( WidgetCast<NounShip>( pNoun ), m_nOrder, m_pTarget, m_pFrom );
}

void VerbOrderShip::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) )
	{
		// set the order
		pShip->setOrder( m_nOrder, m_pTarget, m_pFrom );
		
		// post a message
		if ( m_nOrder != NounShip::NOORDER )
		{
			pShip->message( CharString().format( "<color;ffffff>Comms: Received Order, %s %s", 
				NounShip::orderText( m_nOrder ), m_pTarget.valid() ? m_pTarget->name() : "" ) );

			if ( validate( m_pFrom ) )
			{
				m_pFrom->message( CharString().format( "<color;ffffff>Comms: Sent Order to %s, %s %s",
					pShip->name(), NounShip::orderText( m_nOrder ), m_pTarget.valid() ? m_pTarget->name() : "" ) );
			}
		}

		// notify the AI...
		GameContext * pGameContext = WidgetCast<GameContext>( context() );
		if ( pGameContext != NULL )
			pGameContext->onVerbOrderShip( this );
	}
}

//----------------------------------------------------------------------------

bool VerbOrderShip::canOrder( Noun * pWho, NounShip * pFrom )
{
	NounShip * pShip = WidgetCast<NounShip>( pWho );
	if ( pShip == NULL || pFrom == NULL )
		return false;
	return pShip->canOrder( pFrom );
}

bool VerbOrderShip::canOrderShip( NounShip * pShip, Order nOrder, Noun * pTarget, NounShip * pFrom )
{
	if ( pShip == NULL || pFrom == NULL )
		return false;
	return pShip->canOrder( nOrder, pTarget, pFrom );
}

//----------------------------------------------------------------------------
// EOF
