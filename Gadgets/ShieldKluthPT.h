/**
	@file ShieldKluthPT.h
	@brief K'Luth shield for event.
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 4/23/2010 10:39:54 PM
*/

#ifndef SHIELDKLUTHPT_H
#define SHIELDKLUTHPT_H

#include "DarkSpace/GadgetShield.h"

//----------------------------------------------------------------------------

class ShieldKluthPT : public GadgetShield
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 6500;
	}
	Type type() const
	{
		return SHIELD;
	}
	float addSignature() const
	{
		return active() ? 0.5f : 0.0f;
	}
	int	maxDamage() const
	{
		return 5700 + (300 * level());
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 460;
	}
	// GadgetShield interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY | DAMAGE_ELF | DAMAGE_EMP)) != 0;
	}
	int	energyCost() const
	{
		// return ((3 * level()) / 5);
		return 4;
	}
	int chargeEnergyCost() const
	{
		// return ((2 * level()) / 4);
		return 1;
	}
	int maxCharge() const
	{
		return (177500 + (9800 * level())) * modifier();
	}
	int chargeRate() const
	{
		// return (maxCharge() / 9100);
		return 24;
	}
	float damageModifier( dword nType ) const
	{
		if ( (nType & (DAMAGE_ENERGY)) != 0 )
			if( (nType & (DAMAGE_KINETIC|DAMAGE_PSI)) != 0 )
				return 0.925f;
			else
				return 0.85f;
				
		return 1.0f;
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
