/*
	GadgetDrive.cpp
	(c)1999 PaleStar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounShip.h"
#include "GadgetDrive.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetDrive,  NounGadget  ); 
REGISTER_FACTORY_KEY( GadgetDrive, 4099437687301286850LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetDrive, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Flags, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetDrive::GadgetDrive()
{
	m_Flags |= FLAG_ACTIVE;
}

//----------------------------------------------------------------------------

void GadgetDrive::setup()
{
	NounGadget::setup();
	m_Flags |= FLAG_ACTIVE;
}

void GadgetDrive::initialize()
{
	NounGadget::initialize();
	m_Flags |= FLAG_ACTIVE;
}

void GadgetDrive::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip != NULL )
	{
		// apply the damage to this drive
		NounGadget::inflictDamage( nWhen, pFrom, damage, type, direction );
		// update the ship characteristics and unlock the dynamics
		pShip->updateCharacter();
	}
	else
	{
		// drive not attached to a ship, just call the base class
		NounGadget::inflictDamage( nWhen, pFrom, damage, type, direction );
	}
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetDrive::type() const
{
	return( DRIVE );
}

NounGadget::EnergyClass GadgetDrive::energyClass() const
{
	return ENERGY_CLASS_DRIVES;
}

float GadgetDrive::addThrust() const
{
	return( active() ? thrust() * damageRatioInv() : 0.0f );
}

int GadgetDrive::addMaxEnergy() const
{
	return( maxEnergy() );
}

float GadgetDrive::addSignature() const
{
	NounShip * pParent = WidgetCast<NounShip>( parent() );
	if ( pParent != NULL )
		return( active() ? pParent->velocity() * 0.1f : 0.0f );
	return 0.0f;
}

CharString GadgetDrive::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	
	// usage information
	int generate = 0;
	if ( active() )
	{
		generate = damageRatioInv() * this->energy();
		// reduce energy based on velocity
		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if ( pShip != NULL )
		{
			if ( energyFalloff() > 0 && pShip->maxVelocity() > 0.0f && pShip->velocity() > pShip->baseVelocity() )
			{
				float scale = ( pShip->velocity() - pShip->baseVelocity() ) / pShip->velocityRange();
				generate -= ( energyFalloff() * scale) ;
			}
		}
	}
	
	tip += CharString().format("\nEnergy p/s:<X;100>%.1f", ( generate * TICKS_PER_SECOND ) / 1000.0f );
	tip += CharString().format("\nEnergy Stored:<X;100>%.0f", maxEnergy() / 1000.0f );	
	
	return tip;
}

bool GadgetDrive::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	return true;
}

bool GadgetDrive::useActive() const
{
	return active();
}

void GadgetDrive::use( dword nWhen, Noun * pTarget, bool shift)
{
	if (! active() )
		NounGadget::use( nWhen, pTarget, shift );

	if ( isServer() )
		setFlags( FLAG_ACTIVE, !active() );
}

int	GadgetDrive::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		int generate = damageRatioInv() * this->energy();
		// reduce energy based on velocity
		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if ( pShip != NULL )
		{
			if ( energyFalloff() > 0 && pShip->maxVelocity() > 0.0f && pShip->velocity() > pShip->baseVelocity() )
			{
				float scale = ( pShip->velocity() - pShip->baseVelocity() ) / pShip->velocityRange();
				generate -= (energyFalloff() * scale);
			}
		}

		energy += generate;
	}

	return energy;
}

//----------------------------------------------------------------------------
// EOF


