/*
	VerbLoad.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "NounUnit.h"
#include "NounPod.h"
#include "CargoResource.h"
#include "CargoEnhancement.h"
#include "StructurePort.h"
#include "VerbLoad.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant MAX_LOAD_DISTANCE( "MAX_LOAD_DISTANCE", 250.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbLoad, Verb );

BEGIN_PROPERTY_LIST( VerbLoad, Verb )
	ADD_TRANSMIT_PROPERTY( m_pLoad );
END_PROPERTY_LIST();

VerbLoad::VerbLoad()
{}

VerbLoad::VerbLoad( NounShip * pShip, Noun * pLoad ) 
	: m_pLoad( pLoad )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

Verb::Priority VerbLoad::priority() const
{
	return MEDIUM;
}

VerbLoad::Scope VerbLoad::scope() const
{
	return WidgetCast<StructurePort>( m_pLoad ) != NULL ? LOCAL : SERVER;
}

bool VerbLoad::client() const
{
	return true;
}

bool VerbLoad::canAttach( Noun * pNoun )
{
	return canLoad( m_pLoad, WidgetCast<NounShip>( pNoun ) );
}

void VerbLoad::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( m_pLoad ) && validate( pShip ) )
	{
		if ( WidgetCast<StructurePort>( m_pLoad.pointer() ) )
		{
			// use the structure, it will detuct the resource from the planet..
			((StructurePort *)m_pLoad.pointer())->use( tick(), pShip );
		}
		// check one more time that the cargo can be loaded, as the multiple VerbLoad objects may arrive at once
		else if ( context()->isServer() && canLoad( m_pLoad, pShip ) )
		{
			if ( WidgetCast<CargoEnhancement>( m_pLoad ) )
				LOG_STATUS( "VerbUnload", "[ENHANCEMENT]:Load:%u:%s:%s", pShip->userId(), m_pLoad->name(), pShip->name() );

			// send message to ship
			pShip->privateChat( CharString().format( "<color;ffffff>Cargo: Loading %s.", m_pLoad->name()) );

			NounPod * pPod = new NounPod;
			pPod->setName( m_pLoad->name() );
			//pPod->setKey( key() );
			pPod->setPosition( m_pLoad->worldPosition() );
			pPod->setTeamId( pShip->teamId() );
			pPod->setTarget( pShip );
			pPod->setTick( tick() );

			// zero out the cargo's position while on the pod
			m_pLoad->setPosition( Vector3( 0,0,0 ) );
			m_pLoad->setTeamId( pShip->teamId() );
			// detach the cargo from the universe
			m_pLoad->setDetach();
			// attach to the pod, use a copy because if we don't have a different key then the attached cargo may
			// get deleted by the setDetach() above on the client.
			Noun::Ref pCopy = (Noun *)m_pLoad->copy();
			pPod->setCargo( pCopy );
			// attach pod to the universe
			context()->attachNoun( pPod );
		}
	}
}

//----------------------------------------------------------------------------

bool VerbLoad::mayLoad( Noun * pLoad, NounShip * pShip )
{
	if ( !pShip || !WidgetCast<NounGame>( pLoad ) )
		return false;		// invalid pointers
	if ( pShip->isDestroyed() )
		return false;
	if ( pShip->isEnemy( pLoad ) )
		return false;		// cannot load enemy objects, can load friendly and neutral objects
	if ( WidgetCast<StructurePort>( pLoad ) != NULL )
		return ((StructurePort *)pLoad)->usable( pShip );

	if (! ((NounGame *)pLoad)->isCargo() )
		return false;		// not cargo, cannot be loaded
	if ( pLoad->detach() )
		return false;		// cargo is already getting detached from the world!
	if (! pLoad->parent() )
		return false;		// cargo isn't attached to the universe
	if ( WidgetCast<NounShip>( pLoad->parent() ) )
		return false;		// don't allow loading from another ship no matter what
	if ( WidgetCast<NounUnit>( pLoad ) && !pShip->isFriend( pLoad ) )
		return false;		// cannot load neutral units
	// dont allow loading if it is already in our ship
	if(pLoad->parent() == pShip)
		return false;

	return true;
}

bool VerbLoad::canLoad( Noun * pLoad, NounShip * pShip )
{
	if (! mayLoad( pLoad, pShip ) )
		return false;
	if (! NounGame::findCargoHold( pShip, pLoad ) )
		return false;		// no room in cargo hold

	// always allow loading when ship is in orbit
	if ( pShip->orbiting() != pLoad->zoneParent() )
	{
		float distance = (pShip->worldPosition() - pLoad->worldPosition()).magnitude();
		distance -= pShip->radius();

		if ( distance > MAX_LOAD_DISTANCE )
			return false;		// not in range
	}


	return true;
}

//----------------------------------------------------------------------------
//EOF
