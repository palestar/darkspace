/*
	VerbUnload.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "World/VerbChat.h"
#include "NounShip.h"
#include "NounPod.h"
#include "NounUnit.h"
#include "CargoResource.h"
#include "CargoEnhancement.h"
#include "VerbUnload.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant MAX_UNLOAD_DISTANCE( "MAX_UNLOAD_DISTANCE", 250.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUnload, Verb );

BEGIN_PROPERTY_LIST( VerbUnload, Verb )
	ADD_TRANSMIT_PROPERTY( m_pTarget );
	ADD_TRANSMIT_PROPERTY( m_pUnload );
END_PROPERTY_LIST();

VerbUnload::VerbUnload()
{}

VerbUnload::VerbUnload(  NounShip * pShip, Noun * pTarget, Noun * pUnload ) : m_pTarget( pTarget ), m_pUnload( pUnload )
{
	attachVerb( pShip );
}

//----------------------------------------------------------------------------

bool VerbUnload::queue() const
{
	return true;
}

Verb::Priority VerbUnload::priority() const
{
	return MEDIUM;
}

VerbUnload::Scope VerbUnload::scope() const
{
	return SERVER;
}

bool VerbUnload::client() const
{
	return true;
}

bool VerbUnload::canAttach( Noun * pNoun )
{
	return canUnload( m_pTarget, m_pUnload, WidgetCast<NounShip>( pNoun ) );
}

void VerbUnload::onExecute()
{
	NounShip * pShip = WidgetCast<NounShip>( target() );
	if ( validate( pShip ) && validate( m_pTarget ) && validate( m_pUnload ))
	{
		if ( context()->isServer() )
		{
			if ( WidgetCast<CargoEnhancement>( m_pUnload ) )
				LOG_STATUS( "VerbUnload", "[ENHANCEMENT]:Unload:%u:%s:%s", pShip->userId(), m_pUnload->name(), pShip->name() );

			if ( m_pTarget != pShip )
			{
				Noun::Ref pUnload = m_pUnload;
				pShip->privateChat( CharString().format( "<color;ffffff>Cargo: Unloading %s onto %s.", pUnload->name(), m_pTarget->name() ) );

				// create NounPod object and attach the cargo to this object destined for the target
				NounPod::Ref pPod = new NounPod;
				pPod->setName( pUnload->name() );
				//pPod->setKey( key() );		// use the key of this verb for the pod, so it will be the same on all clients
				pPod->setPosition( pShip->worldPosition() );
				pPod->setTeamId( pShip->teamId() );
				pPod->setFrom ( pShip );
				pPod->setTarget( m_pTarget );
				pPod->setTick( tick() );

				// detach the cargo from the ship
				pUnload->setDetach();
				// attach to the cargo pod
				pPod->setCargo( pUnload );
				// attach pod to the universe
				context()->attachNoun( pPod );
			}
			else
			{
				Noun::Ref pUnload = m_pUnload;
				pShip->privateChat( CharString().format( "<color;ffffff>Cargo: %s jettisoned.", pUnload->name() ) );

				// detach the noun from its current parent
				pUnload->detachSelf();
				// set position of jettisoned item to be behind the ship, with a random offset
				Vector3 position( pShip->worldPosition() - (pShip->frame().k * (pShip->radius() + 2.0f)) );
				position += RandomVector( -0.5f, 0.5f );
				position.y = 0.0f;
				pUnload->setPosition( position );
				// re-attach to the universe now
				context()->attachNoun( pUnload );
			}
		}
	}
}

//----------------------------------------------------------------------------

bool VerbUnload::mayUnload( Noun * pTarget, Noun * pUnload, NounShip * pShip )
{
	if ( !pShip || !pTarget || !WidgetCast<NounGame>( pUnload  ) )
		return false;		// invalid pointers
	if (! ((NounGame *)pUnload)->isCargo() )
		return false;		// this type of object cannot be unloaded
	if ( pUnload->detach() )
		return false;		// cargo is already getting detached from the world!
	if (! pUnload->isParent( pShip ) )
		return false;		// cargo is not on the person unloading the cargo
	if ( pTarget->isParent( pShip ) )
		return false;		// target is onboard this ship

	return true;
}

bool VerbUnload::canUnload( Noun * pTarget, Noun * pUnload, NounShip * pShip )
{
	if (! mayUnload( pTarget, pUnload, pShip ) )
		return false;
	if ( pShip->isEnemy( pUnload ) )
		return false;		// cannot unload enemy objects! 
	if ( pShip->testFlags( NounShip::FLAG_IN_SAFE_ZONE|NounShip::FLAG_CLOAKED ) )
		return false;		// unloading disable when in safe zone or cloaked..

	// if target is ourselves, then we are ejecting the cargo into space
	if ( pTarget != pShip )
	{
		// unloading to a ship ?
		NounShip * pTargetShip = WidgetCast<NounShip>( pTarget );
		if( pTargetShip && pTargetShip->isMonster() )
			return false;	// cannot unload to space monsters
		if (pTargetShip && ((NounUnit *)pUnload)->cannotBoardShip())
			return false;	// armor cannot board enemy ships
		// unloading to an enemy ?
		if( pShip->isEnemy( pTarget ) )
			if( ! WidgetCast<NounUnit>( pUnload ) )
				return false;	// can only unload units to enemy objects
		
		// make sure the target can hold the cargo
		if (! NounGame::findCargoHold( pTarget, pUnload ) )
			return false;

		if ( pShip->orbiting() != pTarget->zoneParent() )
		{
			// check range to target
			float distance = (pShip->worldPosition() - pTarget->worldPosition()).magnitude();
			distance -= pTarget->radius();
			if ( distance > MAX_UNLOAD_DISTANCE )
				return false;		// out of range
		}
	}

	return true;
}

//----------------------------------------------------------------------------
//EOF
