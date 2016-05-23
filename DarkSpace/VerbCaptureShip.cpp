/*
	VerbCaptureShip.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounUnit.h"
#include "resource.h"
#include "VerbCaptureShip.h"
#include "VerbUnload.h"
#include "GameContext.h"
#include "NounEnhancement.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbCaptureShip, Verb );

BEGIN_PROPERTY_LIST( VerbCaptureShip, Verb )
	ADD_TRANSMIT_PROPERTY( m_nFleetId );
	ADD_TRANSMIT_PROPERTY( m_pWho );
END_PROPERTY_LIST();

VerbCaptureShip::VerbCaptureShip()
{}

VerbCaptureShip::VerbCaptureShip( NounShip * pShip, int teamId, Noun * pWho ) 
	: m_nFleetId( teamId ), m_pWho( pWho )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbCaptureShip::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbCaptureShip::scope() const
{
	return LOCAL;
}

bool VerbCaptureShip::client() const
{
	return false;
}

bool VerbCaptureShip::canAttach( Noun * pNoun )
{
	return WidgetCast<NounShip>( pNoun ) != NULL;
}

void VerbCaptureShip::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) )
	{
		Noun * pCapper = m_pWho;
		if ( WidgetCast<NounUnit>( pCapper ) )
			pCapper = ((NounUnit *)pCapper)->home();

		// inform user before we modify the ship
		GameContext * pContext = (GameContext *)context();
		pContext->gameUser()->onCaptureShip( pCapper, pShip );
		
		int nFactionID = pContext->teamFactionId( m_nFleetId );

		// make sure we adjust platform counts if this is a platform
		if ( pShip->type() == NounShip::PLATFORM || pShip->type() == NounShip::PLATFORM_SUPPLY )
		{
			// update platform counts for the factions
			if ( (pContext->factionPlatforms( nFactionID ) + 1) > MAX_PLATFORMS_PER_FACTION ) 
			{
				// detach platform, capping team will be above their limit
				if ( context()->isServer() )
					((GameContext *)context())->detachNoun( pShip );
				return;
			}
			else
			{
				// adjust platform counts so we don't end up with incorrect counts
				((GameContext *)context())->adjustFactionPlatforms( pShip->factionId(), -1 );
				((GameContext *)context())->adjustFactionPlatforms( nFactionID, +1 );
			}
		}
		else
		{
			// set this ship as an ally to the person who capped it
			if ( WidgetCast<NounShip>( pCapper ) )
			{
				NounShip * pCapperShip = WidgetCast<NounShip>( pCapper );
				pShip->setName( CharString().format( "[ALLY] %s", pCapper->name() ) ); 	//informs players that this ship is pCapper's ally
				pShip->addFlags( NounShip::FLAG_AI_ALLY );								// flag this ship as an AI ally
				pShip->setOrder( NounShip::DEFEND, pCapperShip, pCapperShip );			// set this ship to defend the captor
				pShip->setHome( pCapperShip );											// our home is the player that captured us..
				pCapperShip->addAlly( pShip );											// add this ship to the captors allies list
			}
			else
			{
				pShip->setOrder( NounShip::NOORDER, NULL, pShip );	// set the ship to go home
				pShip->setName(CharString("[CAPTURED] ") + CharString(pShip->name())); //informs players that this ship is captured, also helps with logging if the ship gets a kill
			}
		}

		// ship captured set the owner
		pShip->setTeamId( m_nFleetId );
		pShip->setUserId( 0 );								// disown the ship, so it's doesn't get stored

		// if we're the server, take control of this ship
		if ( context()->isServer() )
		{
			// jettison all cargo
			for(int i=pShip->childCount()-1;i>=0;i--)
			{
				NounGame * pChild = WidgetCast<NounGame>( pShip->child( i ) );
				if ( pChild != NULL && pChild->isCargo() 
					&& !WidgetCast<NounEnhancement>( pChild ) 
					&& !WidgetCast<NounUnit>( pChild ) )
				{
					Verb::Ref( new VerbUnload( pShip, pShip, pChild ) );
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
//EOF
