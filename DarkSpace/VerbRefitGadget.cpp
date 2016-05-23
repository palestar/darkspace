/*
	VerbRefitGadget.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "VerbRefitGadget.h"
//#include "VerbSetCredits.h"
#include "GadgetSlot.h"
#include "GadgetJumpDrive.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbRefitGadget, Verb );

BEGIN_PROPERTY_LIST( VerbRefitGadget, Verb )
	ADD_TRANSMIT_PROPERTY( m_pNewGadget );
END_PROPERTY_LIST();

VerbRefitGadget::VerbRefitGadget()
{}

VerbRefitGadget::VerbRefitGadget( NounGadget * pGadget, NounGadget * pNewGadget ) 
	: m_pNewGadget( pNewGadget )
{
	attachVerb( pGadget );
}

//----------------------------------------------------------------------------

Verb::Priority VerbRefitGadget::priority() const
{
	return MEDIUM;
}

Verb::Scope VerbRefitGadget::scope() const
{
	return LOCAL;
}

bool VerbRefitGadget::client() const
{
	return true;
}

bool VerbRefitGadget::canAttach( Noun * pNoun )
{
	NounGadget * pGadget = WidgetCast<NounGadget>( pNoun );
	if ( pGadget == NULL )
		return false;

	// we need to get the upgrades first, as to make sure the Scene gets loaded and referenced
	Array< NounGadget * > upgrades;
	if (! pGadget->getUpgrades( pNoun->factionId(), upgrades ) )
		return false;		// no upgrades available

	if ( pNoun->isServer() && !canRefit( pGadget, m_pNewGadget ) )
		return false;
	return true;
}

void VerbRefitGadget::onExecute()
{
	NounGadget * pGadget = WidgetCast<NounGadget>( target() );
	if (! validate( pGadget ) || !m_pNewGadget.valid() )
		return;
	NounShip * pShip = WidgetCast<NounShip>( pGadget->parent() );
	if (! validate( pShip ) )
		return;

	NounPlanet * pPlanet = WidgetCast<NounPlanet>( pShip->orbiting() );
	if ( ( pShip->flags() & NounShip::FLAG_ADMIN ) == 0 )
	{
		if (! validate( pPlanet ) )
			return;
	}
	else


	// use up the planet resources ( as an admin we want to use resources if we are oribitting so check again here)
	if ( validate( pPlanet ) )
		pPlanet->useResources( m_pNewGadget->buildCost() );

	if ( context()->isServer() )
	{
		NounGadget::Ref pInstall = (NounGadget *)m_pNewGadget->copy();
		if ( pInstall == NULL )
			return;

		pInstall->setPosition( pGadget->position() );
		pInstall->setFrame( pGadget->frame() );
		pInstall->clearFlags( NounGadget::FLAG_MOUNT_FULL );
		pInstall->addFlags( pGadget->mountFlags() );
		pInstall->setGroup( pGadget->group() );
	
		// send a message to the client
		pShip->privateChat( CharString().format( "<color;ffffff>Engineering: Installing %s.", pInstall->name()) );
		// remove the old gadget to be removed on the next update
		pGadget->setDetach();
		// attach the new gadget to the ship
		pShip->attachNode( pInstall );
		pShip->initializeGadgets();

		// save off storage with this "active" ship
		context()->user()->saveStorage(pShip);
	}
}

//----------------------------------------------------------------------------

bool VerbRefitGadget::canRefit( NounGadget * pGadget, NounGadget * pNewGadget )
{
	if ( pGadget == NULL || pNewGadget == NULL )
		return false;		// invalid pointers
	if ( pGadget->useActive() )
		return false;		// cannot refit if the gadget is active
	if ( !pGadget->cooledDown() )
		return false;		// device still on cooldown
	NounShip * pShip = WidgetCast<NounShip>( pGadget->parent() );
	if ( pShip == NULL )
		return false;		// current gadget not attached to a ship
		
	// skip these checks if we're an admin
	if ( ( pShip->flags() & NounShip::FLAG_ADMIN ) == 0 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( pShip->orbiting() );
		if ( pPlanet == NULL || !pShip->isFriend( pPlanet ) )
			return false;		// not orbiting a friendly planet
		if ( pPlanet->resources() < pNewGadget->buildCost() )
			return false;		// not enough resources on the planet
		if ( pPlanet->technology() < pNewGadget->buildTechnology() )
			return false;		// technology level too low
		if ( (pPlanet->flags() & NounPlanet::FLAG_BLOCKADE) != 0 )
			return false;		// planet under blockade
		if ( (pPlanet->flags() & pNewGadget->buildFlags()) != pNewGadget->buildFlags() )
			return false;		// missing build requirement flags
	}

	Array< NounGadget * > upgrades;
	if (! pGadget->getUpgrades( pShip->factionId(), upgrades ) )
		return false;		// no upgrades available
	if ( upgrades.search( pNewGadget ) < 0 )
		return false;		// this item not available as an upgrade

	return true;
}

//----------------------------------------------------------------------------
//EOF
