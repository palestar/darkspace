/*
	GadgetCloak.cpp
	(c)2000 Palestar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "NounShip.h"
#include "NounBeacon.h"
#include "GadgetCloak.h"
#include "GadgetDrive.h"
#include "GadgetReactor.h"
#include "GadgetFighterBay.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

const float CLOAK_ENERGY_SCALAR = 6.5f;
	
//----------------------------------------------------------------------------	

IMPLEMENT_ABSTRACT_FACTORY( GadgetCloak, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetCloak, 4137086329828747054LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetCloak, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fCloak );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fSubVisibility );
END_PROPERTY_LIST();

GadgetCloak::GadgetCloak() : m_fCloak( 0.0f ), 
	m_fSubVisibility( 0.0f ),
	m_nEnergyTick( 0 ), 
	m_fCloakTime( 0.0f ), 
	m_fTimeElapsed( 0.0f ),
	m_bCloakTimerState( false )
{}

//----------------------------------------------------------------------------

void GadgetCloak::setup()
{
	NounGadget::setup();

	m_fCloak = 0.0f;
	m_fSubVisibility = 0.0f;
	m_fCloakTime = 0.0f;
	m_fTimeElapsed = 0.0f;
	m_bCloakTimerState = false;
	m_nEnergyTick = 0;
}

void GadgetCloak::initialize()
{
	NounGadget::initialize();

	m_nEnergyTick = 0;
	m_fCloak = 0.0f;
	m_fSubVisibility = 0.0f;
	m_fCloakTime = 0.0f;
	m_fTimeElapsed = 0.0f;
	m_bCloakTimerState = false;

	NounShip * pParent = WidgetCast<NounShip>( parent() );
	if ( pParent != NULL )
	{
        if (! active() )
		{
			m_bCloakTimerState = false;
			pParent->clearFlags( NounShip::FLAG_CLOAKED );
		}
		else
		{
			m_bCloakTimerState = true;
			pParent->addFlags( NounShip::FLAG_CLOAKED );
		}
	}
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetCloak::type() const
{
	return SPECIAL_DEFENSIVE;
}

dword GadgetCloak::hotkey() const
{
	return 'V';
}

CharString GadgetCloak::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	// usage information
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip == NULL )
		return tip;
	float fCloakTime = pShip->cloakTime() * calculateModifier( MT_CLOAK, true );
	float fUncloakTime = pShip->uncloakTime() * calculateModifier( MT_CLOAK, true );
	float fCloakedTime = pShip->maxCloakTime() * calculateModifier( MT_CLOAK, false );
	fCloakTime *= ( 1 + damageRatio() );
	
	tip += CharString().format("\nTime to Cloak:<X;100>%.1fs", fCloakTime );
	
	tip += CharString().format("\nTime to Decloak:<X;100>%.1fs", fUncloakTime );
	
	tip += CharString().format("\nTime cloaked:<X;100>%.1fs", fCloakedTime );	

	float nEnergyCost = 0;
	float fSig = pShip->signature() + m_fCloak;
	if ( fSig > 0 )
		nEnergyCost = ( fSig * ( pShip->maxEnergy() * energyCost() ) ) / pShip->cloakTime();
				
	if ( nEnergyCost > ( pShip->maxEnergy() / pShip->cloakTime() ) )
		nEnergyCost = ( pShip->maxEnergy() / pShip->cloakTime() );
	
	nEnergyCost *= pShip->cloakEnergyModifier();
	
	tip += CharString().format("\nEnergy Cost/s:<X;100>%.1f", nEnergyCost / 1000.0f );
	
	return tip;
}

bool GadgetCloak::usable( Noun * pTarget, bool shift ) const
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip == NULL )
		return false;		// cloaking device only works for ships..
	if ( pShip->energy() < pShip->signature() * ( pShip->maxEnergy() * energyCost() ) )
		return false;		// not enough energy to cloak
	if ( pShip->testFlags( NounShip::FLAG_CONTROL_DISABLED ) )
		return false;		// can't jump, aligning
	if ( active() )
		return true;		// you can always turn it off..
	if ( isClient() && !NounGadget::usable( pTarget, shift ) )
		return false;		// cooldown not reached yet..
	if ( isJumping() )
		return false;		// ship is currently jumping, no cloaking..
	if ( destroyed() )	
		return false;		// device is destroyed
	return true;
}

int	GadgetCloak::usableWhen() const
{
	if ( active() )
	{
		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if (! pShip )
			return 0;

		// if we are cloaking, but not cloaked yet, so show the bar in reverse ..
		if (! pShip->testFlags( NounShip::FLAG_CLOAKED ) && m_fCloakTime > 0.0f )
			return 100 - Clamp<int>( (m_fTimeElapsed * 100) / m_fCloakTime, 0, 100 );

		// return how long we can remained cloaked..
		float fMaxCloakedTime = pShip->maxCloakTime() * calculateModifier( MT_CLOAK, false );
		return Clamp<int>( (m_fCloakedTime * 100) / fMaxCloakedTime, 0, 100 );
	}

	return NounGadget::usableWhen();
}

bool GadgetCloak::useActive() const
{
	// added fix so device can't be removed while we still are uncloaking the ship, leaving the cloaked flag turned on the ship
	return active() || isCloaked();
}

void GadgetCloak::use( dword when, Noun * pTarget, bool shift)
{	
	// create the use effect
	createUseEffect();
	// toggle the active state
	if ( isServer() )
	{
		setFlags( FLAG_ACTIVE, !active() );
		message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), active() ? "Active" : "Inactive" ), true ); 
	}
}

int GadgetCloak::useEnergy( dword nTick, int energy )
{ 
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip == NULL )
		return energy;
	
	if ( active() )
	{
	
		float nEnergyCost = 0;
		float fSig = pShip->signature() + m_fCloak;
		if ( fSig > 0 )
			nEnergyCost = ( ( fSig * ( pShip->maxEnergy() * energyCost() ) ) / pShip->cloakTime() ) / TICKS_PER_SECOND;
					
		if ( nEnergyCost > ( ( pShip->maxEnergy() / pShip->cloakTime() ) / TICKS_PER_SECOND ) )
			nEnergyCost = ( ( pShip->maxEnergy() / pShip->cloakTime() ) / TICKS_PER_SECOND );
			
		
		// if the ship is despawning, cancel the despawn
		if( pShip->despawning() )
			pShip->cancelDespawn();
			
		if ( destroyed() && isLocal() )
			setFlags( FLAG_ACTIVE, false );
					
		if (! m_bCloakTimerState )
		{
			m_fCloakTime = pShip->cloakTime() * calculateModifier( MT_CLOAK, true );
			m_fTimeElapsed = 0.0f;
			m_bCloakTimerState = true;
			m_fCloakedTime = 0.0f;
		}

		// get the ship signature, add our current cloak value to make it our actual signature.. (without cloaking)
		float fSignature = pShip->signature() + m_fCloak;
		
		if ( energy >= pShip->cloakEnergyModifier() * nEnergyCost )
		{
			if ( m_fTimeElapsed < m_fCloakTime )
			{
				// only drain energy whilst we're cloaking
				energy -= ( pShip->cloakEnergyModifier() * nEnergyCost );
				
				// we are in the process of cloaking...
				m_fTimeElapsed += TICK_DURATION_S;
				m_fSubVisibility = Min<float>( m_fTimeElapsed / m_fCloakTime, 1.0f );
				m_fCloak = fSignature * m_fSubVisibility;
			}
			else
			{
				// return all fighters to the ship if we activate cloak
				if ( pShip != NULL && !active() )
				{
					std::vector< GadgetFighterBay * > fighters;
					FindNodeClasses( pShip, fighters );
					for(size_t i = 0; i < fighters.size(); ++i )
						fighters[ i ]->use( nTick, NULL, true );
				}
				// we are fully cloaked now..
				m_fSubVisibility = 1.0f;
				m_fCloak = fSignature;		// cloak all signature..
				// set the cloaking flag
				pShip->addFlags( NounShip::FLAG_CLOAKED );
			}
		}
		
		if ( pShip->testFlags( NounShip::FLAG_CLOAKED ) )
		{
			m_fCloakedTime += TICK_DURATION_S;
		
			// cloak time up, deactivate
			if ( m_fCloakedTime >= ( pShip->maxCloakTime() * calculateModifier( MT_CLOAK, false ) ) )
				setFlags( FLAG_ACTIVE, false );
		}
		
	}
	else
	{
		// cloaking device in-active..
		if ( pShip->testFlags( NounShip::FLAG_CLOAKED) ) 
		{
			if ( m_bCloakTimerState )
			{
				m_fCloakTime = pShip->uncloakTime() * calculateModifier( MT_CLOAK, true );
				m_fTimeElapsed = 0.0f;
				m_bCloakTimerState = false;
			}

			if ( m_fTimeElapsed < m_fCloakTime )
			{
				// in the process of uncloaking..
				m_fTimeElapsed += TICK_DURATION_S;
				m_fSubVisibility = 1.0f - Min<float>( m_fTimeElapsed / m_fCloakTime, 1.0f );
				m_fCloak -= m_fCloak * m_fSubVisibility;
			}
			else
			{
				NounGadget::use( nTick, NULL, false );	// call base class so cooldown tick will get set - we only want to do this if the gadget is being used
				// fully uncloaked..
				pShip->clearFlags( NounShip::FLAG_CLOAKED );
				m_fCloak = 0.0f;
				m_fSubVisibility = 0.0f;
			}
		}
		else
		{
			m_fCloak = 0.0f;
			m_fSubVisibility = 0.0f;
		}
	}

	return energy;
}

bool GadgetCloak::updateLogic()
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if (! pShip )
		return false;
	
	switch( pShip->command() )
	{
	case NounShip::EVADE:
		// turn on cloak
		if (!useActive() )
			pShip->useGadget( this, NULL, false );		
		break;
	default:
		// turn off cloak
		if ( useActive() )
			pShip->useGadget( this, NULL, false );
		break;
	}

	return true;
}

//----------------------------------------------------------------------------
// EOF
