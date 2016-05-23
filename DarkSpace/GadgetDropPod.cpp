/*
	GadgetDropPod.h

	Gadget that creates an infantry unit in a pod for boarding
*/

#include "Debug/Assert.h"
#include "GadgetDropPod.h"
#include "NounPod.h"
#include "NounShip.h"
#include "NounUnit.h"
#include "World/VerbChat.h"
#include "GameContext.h"


IMPLEMENT_ABSTRACT_FACTORY( GadgetDropPod, NounGadget ); 

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetDropPod, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
END_PROPERTY_LIST();


GadgetDropPod::GadgetDropPod()
{}


void GadgetDropPod::setup()
{
	NounGadget::setup();
	m_Energy = energyNeeded();
}

void GadgetDropPod::initialize()
{
	NounGadget::initialize();
	loadUnit();
}

dword GadgetDropPod::hotkey() const
{
	return 'V';
}

//---------------------------------------------------------
// NounGadget interface

NounGadget::Type GadgetDropPod::type() const
{
	return SPECIAL;
}

NounGadget::EnergyClass GadgetDropPod::energyClass() const
{
	return ENERGY_CLASS_SPECIAL;
}

int GadgetDropPod::usableWhen()
{
	float percentleft = (energyNeeded() - m_Energy) / energyNeeded();
	return 100 - (int)(percentleft * 100);
}

bool GadgetDropPod::usable( Noun * pTarget, bool shift ) const
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	NounShip * pShipTarget = WidgetCast<NounShip>( pTarget );
	NounPlanet * pPlanetTarget = WidgetCast<NounPlanet>( pTarget );

	int teamId = -1;
	if( pShipTarget != NULL)
	{
		teamId = pShipTarget->teamId();
	}
	else if( pPlanetTarget != NULL)
	{
		teamId = pPlanetTarget->teamId();
	}
	else
	{
		return false; //if both possible target types are null then this device can't be used
	}

	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )									//destroyed gadget	
		return false;
	if ( m_Energy < energyNeeded() )					//recharging
		return false;
	if ( pShip->teamId() == teamId  )					//same team
		return false;
	if ( pShip == NULL )								//parent is not a ship
		return false;	
	if( !canDropOnPlanet() && pShipTarget == NULL )		//not targetting a ship and can't drop on planet
		return false;
	if ( pShipTarget == pShip || shift )				//targeting self
		return false;
	if ( pShip->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		return false;
	if ( pShipTarget != NULL && pShip->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		return false;
	
	
	
	

	float distance = (worldPosition() - pTarget->worldPosition()).magnitude();
	if(distance > range())
		return false;

	return true;
}


void GadgetDropPod::use( dword when, Noun * pTarget, bool shift)
{
	if( shift ) return; 

	if(WidgetCast<NounPlanet>( pTarget ) && !canDropOnPlanet() ) return;
	// Can't drop on planets if you're not allowed

	NounGadget::use( when, pTarget, shift );

	createUseEffect( false );
	m_Energy = 0;
	m_nEnergyTick = when;

	//spawn the drop pod at target
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	NounGame * pDropTarget = WidgetCast<NounGame>( pTarget );
	
	if( isServer() && pShip != NULL && pDropTarget != NULL )
	{
		//Set up the infantry
		NounUnit * pSpawn = (NounUnit *)m_pUnit->copy();
		ASSERT ( pSpawn );
		pSpawn->setTeamId( pShip->teamId() );
		pSpawn->setParent( pShip );
		pSpawn->setHex( 0xffff );
		pSpawn->setExperience( 0.40f );
		pSpawn->setup();
		pSpawn->setTick( tick() );
		
		Noun * pHold = NounGame::findCargoHold( pDropTarget, pSpawn );
		if ( pHold != NULL )
		{
				//Shove the infantry into the ship or planet
				Vector3 vLocalPosition( pHold->worldFrame() * (pDropTarget->worldPosition() - pHold->worldPosition()) );
				pSpawn->setPosition( vLocalPosition );		
				pHold->attachNode( pSpawn );
				pDropTarget->context()->user()->saveStorage(pDropTarget);
				pShip->message( CharString( "<color;0000ff>Security: Infantry have been launched!" ) );
		}
	}
}

int GadgetDropPod::useEnergy( dword nTick, int energy )
{
	int nElapsed = nTick - m_nEnergyTick;
	m_nEnergyTick = nTick;

	if ( m_Energy < energyNeeded() )
	{
		m_fChargeRate += chargeRate() * nElapsed;
		int chargeRate = floor( m_fChargeRate );
		m_fChargeRate -= chargeRate;		// leave fractional amount in the float for next update..
		int charge = Min( Min( energyNeeded() - m_Energy, chargeRate ), energy );

		energy -= charge;
		m_Energy += charge;
	}

	return energy;
}

void GadgetDropPod::loadUnit()
{
	m_pBuild = WidgetCast<Scene>( nounContext()->resource( "UNIT" ) );

	if ( m_pBuild.valid() )
	{
		BaseNode * pRoot = m_pBuild->node();
		ASSERT( pRoot );

		for(int i=0;i<pRoot->childCount();i++)
		{
			NounUnit * pUnit = WidgetCast<NounUnit>( pRoot->child(i) );
			if ( pUnit != NULL )
			{
				m_pUnit = pUnit;
				return;
			}
		}
	}
}

bool GadgetDropPod::updateLogic()
{
	NounShip * pShip = WidgetCast<NounShip>(parent());
	if( pShip == NULL ) //our parent isn't a ship? don't use this
		return true;
	if(! usable( pShip->commandTarget(), false ) ) //Device isn't ready? Don't use it yet
		return true;
	pShip->useGadget(this, pShip->commandTarget() , false );
	return false;
}

