/*
	VerbTransferCredits.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Network/LogClient.h"
#include "resource.h"
#include "VerbTransferCredits.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

Constant MAX_TRANSFER_CREDITS_RANGE( "MAX_TRANSFER_CREDITS_RANGE", 250.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbTransferCredits, Verb );

BEGIN_PROPERTY_LIST( VerbTransferCredits, Verb )
	ADD_TRANSMIT_PROPERTY( m_pOrigin );
	ADD_TRANSMIT_PROPERTY( m_pDestination );
	ADD_TRANSMIT_PROPERTY( m_Credits );
END_PROPERTY_LIST();

VerbTransferCredits::VerbTransferCredits() : m_Credits( 0 )
{}

VerbTransferCredits::VerbTransferCredits( NounShip * pSrc, Noun * pDst, int credits ) 
	: m_pOrigin( pSrc ), m_pDestination( pDst ), m_Credits( credits )
{
	attachVerb( pSrc );
}

//----------------------------------------------------------------------------

Verb::Priority VerbTransferCredits::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbTransferCredits::scope() const
{
	return LOCAL;
}

bool VerbTransferCredits::client() const
{
	return true;
}

bool VerbTransferCredits::canAttach( Noun * pNoun )
{
	if ( pNoun->isServer() && pNoun == m_pOrigin )
		return canTransferCredits( m_pOrigin, m_pDestination, m_Credits );
	return true;
}

void VerbTransferCredits::onExecute()
{
	if ( validate( m_pOrigin ) && validate( m_pDestination ) )
	{
		if ( target() == m_pOrigin )
		{
			if ( m_Credits > 0 )
				m_pOrigin->message( CharString().format( "<color;ffffff>SERVER: Sent %d credits to %s...", m_Credits, m_pDestination->name()) );
			else if ( m_Credits < 0 )
				m_pOrigin->message( CharString().format( "<Color;ffffff>SERVER: Received %d credits from %s...", -m_Credits, m_pOrigin->name()) );

			// update the credits in the senders ship..
			m_pOrigin->setCredits( m_pOrigin->credits() - m_Credits );
			// notify the game context, this will push the actual update of the credits on the users profile.
			((GameContext *)context())->gameUser()->onCredits( m_pOrigin, -m_Credits );

			if ( context()->isServer() )
			{
				// log this transfer so it can be tracked..
				LOG_STATUS( "VerbTransferCredits", CharString().format( "[STAT] %u credits transfered from user %s (%u) to user %s  (%u)", 
					m_Credits, m_pOrigin->name(), m_pOrigin->userId(), m_pDestination->name(), m_pDestination->userId() ) );

				// attach this verb onto the destination, so it will be sent down the client who is receiving the credits
				attachVerb( m_pDestination );
			}
		}
		else if ( target() == m_pDestination )
		{
			if ( WidgetCast<NounShip>( target() ) )
			{
				NounShip * pShip = (NounShip *)target();
				// update the credits on the players ship..
				pShip->setCredits( pShip->credits() + m_Credits );
				// notify the game context, this will push the actual update of the credits on the users profile.
				((GameContext *)context())->gameUser()->onCredits( pShip, m_Credits );

				pShip->message( CharString().format( "<Color;ffffff>SERVER: Received %d credits from %s...", m_Credits, m_pOrigin->name()) );
			}
		}
	}
}

//----------------------------------------------------------------------------

bool VerbTransferCredits::canTransferCredits( NounShip * pSrc, Noun * pDst, int credits )
{
	if ( pSrc == NULL || pDst == NULL )
		return false;
	if ( pSrc == pDst )
		return false;			// check for this, otherwise STACK OVERFLOW!
	if ( pDst->userId() == 0 )
		return false;			// don't allow transfers to AI
	if ( WidgetCast<NounShip>( pDst ) )
	{
		if ( credits < 0 )
			return false;		// don't allow someone to take someone elses credits without their permission
		if ( pSrc->credits() < credits )
			return false;		// not enough credits on the source ship
		//if ( pSrc->factionId() != pDst->factionId() )
		//	return false;		// can't transfer credits to enemy unit

		return true;
	}

	// unknown type of destination
	return false;
}


//----------------------------------------------------------------------------
//EOF
