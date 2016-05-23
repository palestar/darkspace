/*
	VerbRemoveEnhancement.cpp

	This verb calls the use() mutator on an enhancement object
	(c)2008 Palestar, Jeff Everett
*/


#include "Debug/Assert.h"
#include "Debug/Profile.h"
#include "World/NounTarget.h"
#include "VerbRemoveEnhancement.h"
#include "DarkSpace/CargoEnhancement.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbRemoveEnhancement, Verb );

BEGIN_PROPERTY_LIST( VerbRemoveEnhancement, Verb )
	ADD_TRANSMIT_PROPERTY( m_pEnhancement );
	ADD_TRANSMIT_PROPERTY( m_bScrap );
END_PROPERTY_LIST();

VerbRemoveEnhancement::VerbRemoveEnhancement()
{}

VerbRemoveEnhancement::VerbRemoveEnhancement( Noun * a_pUser, NounEnhancement * a_pEnhancement, bool a_bScrap )
	: m_pEnhancement(a_pEnhancement), m_bScrap( a_bScrap )
{
	attachVerb( a_pUser );
}

//----------------------------------------------------------------------------

Verb::Priority VerbRemoveEnhancement::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbRemoveEnhancement::scope() const
{
	return SERVER;
}

bool VerbRemoveEnhancement::client() const
{
	return true;
}

bool VerbRemoveEnhancement::queue() const
{
	return false;
}

//----------------------------------------------------------------------------

bool VerbRemoveEnhancement::canAttach( Noun * pNoun )
{
	if (! validate(m_pEnhancement) || !validate(pNoun) )
		return false;
	return true;
}

void VerbRemoveEnhancement::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( context()->isServer() && validate( m_pEnhancement ) && validate( pShip ) )
	{
		NounEnhancement::Ref pRemove = m_pEnhancement.pointer();
		if (! m_bScrap )
		{
			int nRemovalCost = pRemove->calculateRemoveCost();
			if ( pShip->credits() >= nRemovalCost )
			{
				CargoEnhancement::Ref pCargo = new CargoEnhancement( pRemove->factory()->classKey() );
				pCargo->setDamage( pRemove->damage() );				
						
				Noun * pHold = NounGame::findCargoHold( pShip, pCargo );
				if(	pShip->canAttachCargo( pCargo ) && pHold != NULL )
				{				
					// update the credits of the ship...
					pShip->setCredits( pShip->credits() - nRemovalCost );
					// notify the game context, this will push the actual update of the credits on the users profile.
					((GameContext *)context())->gameUser()->onCredits( pShip, -nRemovalCost );

					// add cargo item to ship
					pCargo->setTick( tick() );
					pHold->attachNode( pCargo );	

					// remove old enhancement before we save out ship
					pRemove->detachSelf();	
					
					// save off storage with this "active" ship
					context()->user()->saveStorage(pShip);
					LOG_STATUS( "VerbRemoveEnhancement", "[ENHANCEMENT]:Remove:%u:%s:%s",
						pShip->userId(), pRemove->getName(),pShip->name() );
				}
				else
				{
					pShip->privateChat( CharString().format("<color;ffffff>Cargo: Storage is full, cannot remove enhancement.") );
				}
			}
			else
			{
				pShip->privateChat( CharString().format( "<color;ffffff>Enhancement: '%s' costs %s credits to remove.",
					pRemove->getName(), FormatNumber<char,int>( nRemovalCost ).cstr() ) );		
			}
		}
		else
		{
			// remove old enhancement before we save out ship
			pRemove->detachSelf();	

			pShip->privateChat( CharString().format( 
				"<color;ffffff>Enhancement: Scrapping <color;%6.6x>%s</color> from %s.",
				pRemove->color().RGB(), pRemove->getName(), pShip->name() ) );
			// save off storage with this "active" ship
			context()->user()->saveStorage(pShip);
			
			LOG_STATUS( "VerbRemoveEnhancement", "[ENHANCEMENT]:Scrap:%u:%s:%s",	
				pShip->userId(), pRemove->getName(),pShip->name() );
		}
	}
}

//----------------------------------------------------------------------------
// EOF
