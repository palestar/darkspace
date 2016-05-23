/*
	GadgetPulseShield.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "GadgetPulseShield.h"
#include "NounProjectile.h"
#include "FieldDamage.h"
#include "GameContext.h"
#include "VerbExplodeProjectiles.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetPulseShield, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetPulseShield, 4346025757259950789LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetPulseShield, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
END_PROPERTY_LIST();

GadgetPulseShield::GadgetPulseShield()
{
	m_Energy = 0;
}

//----------------------------------------------------------------------------

void GadgetPulseShield::setup()
{
	NounGadget::setup();
	m_Energy = energyNeeded();
}

CharString GadgetPulseShield::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	// usage information
	tip += CharString().format("\nRange:<X;100>%.2fgu", range() * calculateModifier( MT_PULSESHIELD ) );
	tip += CharString().format("\nChance to Destroy:<X;100>%d%%", chance() );	
	tip += CharString().format("\nRecharge Time:<X;100>%.1fs", energyNeeded() / ( ( chargeRate() * TICKS_PER_SECOND ) * damageRatioInv() ) );
	tip += CharString().format("\nEnergy Cost:<X;100>%.1f", energyNeeded() / 1000.0f );		
	
	return tip;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetPulseShield::type() const
{
	return SPECIAL_DEFENSIVE;
}

dword GadgetPulseShield::hotkey() const
{
	return 'V';
}

//----------------------------------------------------------------------------

int GadgetPulseShield::usableWhen() const
{
	return Min<int>( (100 - ((m_Energy * 100) / energyNeeded())), NounGadget::usableWhen() );
}

bool GadgetPulseShield::usable( Noun * pTarget, bool shift ) const
{
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( m_Energy < energyNeeded() )
		return false;
	return true;
}

void GadgetPulseShield::use( dword when, Noun * pTarget, bool shift)
{
	NounGadget::use( when, pTarget, shift );

	createUseEffect( false );
	m_Energy = 0;

	if ( isServer() )
	{
		Vector3 origin( worldPosition() );

		Array< NounProjectile::wRef > projs;
		// note, this code will only get collidable
		Array< GameContext::NounCollision > collide;
		if ( context()->proximityCheck( origin, range() * calculateModifier( MT_PULSESHIELD ), collide, CLASS_KEY(NounProjectile) ) )
		{
			for(int i=0;i<collide.size();i++)
			{
				if ( isEnemy( (NounProjectile *)collide[ i ].pNoun ) && (rand() % 100) <= chance() )
					projs.push( (NounProjectile *)collide[i].pNoun );
					//((NounProjectile *)collide[ i ].pNoun)->explode();
			}
		}

		Verb::Ref( new VerbExplodeProjectiles( this, projs ) );
	}
}

int GadgetPulseShield::useEnergy( dword nTick, int energy )
{
	if ( m_Energy < energyNeeded() )
	{
		int use =  damageRatioInv() * chargeRate();
		int charge = Min( Min( energyNeeded() - m_Energy, use ), energy );

		energy -= charge;
		m_Energy += charge;
	}

	return energy;
}

//----------------------------------------------------------------------------
//EOF
