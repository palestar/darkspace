/*
	GadgetAreaWeapon.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "GadgetAreaWeapon.h"
#include "GameContext.h"

static Constant AI_USE_AREA_WEAPON( "AI_USE_AREA_WEAPON", 0.75f );

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetAreaWeapon, NounGadget );
REGISTER_FACTORY_KEY( GadgetAreaWeapon, 4373539570291853529LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetAreaWeapon, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
END_PROPERTY_LIST();

GadgetAreaWeapon::GadgetAreaWeapon() : m_Energy( 0 ), m_nEnergyTick( 0 ), m_fChargeRate( 0.0f )
{}

//----------------------------------------------------------------------------

void GadgetAreaWeapon::setup()
{
	NounGadget::setup();
	// start out with full energy
	m_Energy = energyNeeded();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetAreaWeapon::type() const
{
	return WEAPON;
}

NounGadget::EnergyClass GadgetAreaWeapon::energyClass() const
{
	return ENERGY_CLASS_WEAPONS;
}

dword GadgetAreaWeapon::hotkey() const
{
	return 'V';
}

CharString GadgetAreaWeapon::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	// usage information	
	tip += CharString().format("\nRange:<X;100>%.0fgu", range() * calculateModifier( MT_WEAPON_RANGE ) );
	tip += CharString().format("\nDamage:<X;100>%.0f-%.0f", ( damageCaused() * calculateModifier( MT_WEAPON_DAMAGE ) ) * damageRolls(), ( ( damageCaused() + damageRandom() ) * calculateModifier( MT_WEAPON_DAMAGE ) ) * damageRolls() );	
	tip += CharString().format("\nRecharge Time:<X;100>%.1fs", energyNeeded() / ( ( ( chargeRate() * TICKS_PER_SECOND ) * damageRatioInv() ) * calculateModifier( MT_WEAPON_ENERGY ) ) );
	tip += CharString().format("\nEnergy Cost:<X;100>%.1f", energyNeeded() / 1000.0f );		
	
	return tip;
}

//----------------------------------------------------------------------------

int GadgetAreaWeapon::usableWhen() const
{
	return (100 - ((m_Energy * 100) / energyNeeded()));
}

bool GadgetAreaWeapon::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		return false;		
	if ( m_Energy < energyNeeded() )
		return false;
	return true;
}

void GadgetAreaWeapon::use( dword when, Noun * pTarget, bool shift)
{
	NounGadget::use( when, pTarget, shift );

	createUseEffect( false );
	m_Energy = 0;
	m_nEnergyTick = when;

	Vector3 origin( worldPosition() );

	float fRange = range() * calculateModifier( MT_WEAPON_RANGE );
	Array< GameContext::NounCollision > collide;
	if ( context()->proximityCheck( origin, fRange, collide ) )
	{
		for(int i=0;i<collide.size();i++)
		{
			NounGame * pCollide = WidgetCast<NounGame>( collide[ i ].pNoun );
			if ( pCollide != NULL && pCollide != parent() && isEnemy( pCollide )
				&& pCollide->canDamage( damageType() ) )
			{
				float fDistance = collide[i].fDistance - pCollide->radius();
				if ( fDistance < range() )
				{
					float damageRatio = 1.0f - (fDistance / range());

					for(int k=0;k<damageRolls();++k)
					{
						int damage = damageRatio * damageCaused();
						if ( damageRandom() > 0 )
							damage += rand() % damageRandom();
						
						if ( damage > 0 )
						{
							// send the damage verb
							damage *= calculateModifier( MT_WEAPON_DAMAGE );

							pCollide->inflictDamage( tick() + k, parentBody(), damage, damageType(), 
								pCollide->worldFrame() * (origin - pCollide->worldPosition()) );
						}
					}
				}
			}
		}
	}
}

int GadgetAreaWeapon::useEnergy( dword nTick, int energy )
{
	int nElapsed = nTick - m_nEnergyTick;
	m_nEnergyTick = nTick;

	if ( m_Energy < energyNeeded() )
	{
		float fEnergyMod = calculateModifier( MT_WEAPON_ENERGY );
		float fChargeScale = damageRatioInv() * fEnergyMod;

		m_fChargeRate += chargeRate() * fChargeScale * nElapsed;
		int chargeRate = floor( m_fChargeRate );
		m_fChargeRate -= chargeRate;		// leave fractional amount in the float for next update..
		int charge = Min( Min( energyNeeded() - m_Energy, chargeRate ), energy );

		energy -= charge;
		m_Energy += charge;
	}

	return energy;
}

bool GadgetAreaWeapon::updateLogic()
{
	if ( useActive() || usableWhen() > 0 )
		return true;

	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		
		bool bFireWeapon = false;
		for(int i=0;i<pShip->contactCount() && !bFireWeapon;++i)
		{
			Noun * pContact = pShip->contact( i );
			if (! pContact || !isEnemy( pContact ) ) 
				continue;

			float fDistance = (pContact->worldPosition() - pShip->worldPosition()).magnitude();
			if ( fDistance < (range() * AI_USE_AREA_WEAPON) )
				bFireWeapon = true;
		}

		if ( bFireWeapon )
			pShip->useGadget( this, NULL, false );
	}

	return false;
}

//----------------------------------------------------------------------------
//EOF
