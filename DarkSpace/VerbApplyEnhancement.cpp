/*
	VerbApplyEnhancement.cpp

	This verb calls the use() mutator on an enhancement object
	(c)2008 Palestar, Jeff Everett
*/


#include "Debug/Assert.h"
#include "Debug/Profile.h"
#include "World/NounTarget.h"
#include "VerbApplyEnhancement.h"
#include "DarkSpace/CargoEnhancement.h"
#include "Network/LogClient.h"		// LogLevels
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbApplyEnhancement, Verb );

BEGIN_PROPERTY_LIST( VerbApplyEnhancement, Verb )
	ADD_TRANSMIT_PROPERTY( m_pCargo );
END_PROPERTY_LIST();

VerbApplyEnhancement::VerbApplyEnhancement()
{}

VerbApplyEnhancement::VerbApplyEnhancement( Noun * a_pUser, CargoEnhancement * a_pCargo)
	: m_pCargo(a_pCargo)
{
	attachVerb( a_pUser );
}

//----------------------------------------------------------------------------

Verb::Priority VerbApplyEnhancement::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbApplyEnhancement::scope() const
{
	return SERVER;
}

bool VerbApplyEnhancement::client() const
{
	return true;
}

bool VerbApplyEnhancement::queue() const
{
	return false;
}

//----------------------------------------------------------------------------

bool VerbApplyEnhancement::canAttach( Noun * pNoun )
{
	if ( !validate(m_pCargo) || !validate(pNoun) )
		return false;
	return true;
}

void VerbApplyEnhancement::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( context()->isServer() && validate(pShip) && validate(m_pCargo) )
	{
		NounEnhancement::Ref pEnhancement = m_pCargo->enhancement();
		if ( pEnhancement && pEnhancement->parent() == NULL && pEnhancement->canUse( pShip ) )
		{
			// this will attach the contained enhancement to the ship...
			if ( pEnhancement->use( pShip ) )
			{
				// detach the cargo item now..
				pShip->detachNode( m_pCargo );

				pShip->privateChat( CharString().format( "<color;ffffff>Enhancement: Using <color;%6.6x>%s</color> on %s.", 
					pEnhancement->color().RGB(),pEnhancement->getName(), pShip->name() ) );
				LOG_STATUS( "VerbApplyEnhancement", "[ENHANCEMENT]:Apply:%u:%s:%s", 
					pShip->userId(), pEnhancement->getName(),pShip->name() );

				// save off storage with this "active" ship
				context()->user()->saveStorage(pShip);
			}
			else
			{
				pShip->privateChat( CharString().format( "<color;ffffff>Enhancement: Failed to use enhancement <color;%6.6x>%s</color>.",
					pEnhancement->color().RGB(), pEnhancement->getName() ) );
			}
		}
	}
}

bool VerbApplyEnhancement::canApply( CargoEnhancement * a_pCargo, NounShip * a_pUser )
{
	if ( a_pCargo == NULL || a_pUser == NULL )
		return false;		// invaid pointers

	NounEnhancement * pEnhancement = a_pCargo->enhancement();
	if (! pEnhancement )
		return false;
	if (! pEnhancement->canUse( a_pUser ) )
		return false;
	if ( a_pCargo->parent() != a_pUser )
		return false;

	return true;
}

//----------------------------------------------------------------------------
// EOF
