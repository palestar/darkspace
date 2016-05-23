/*
	ShieldPlanetary.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef SHIELD_PLANETARY_H
#define SHIELD_PLANETARY_H

#include "DarkSpace/GadgetShield.h"



//----------------------------------------------------------------------------

class ShieldPlanetary : public GadgetShield
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 90000;
	}
	Type type() const
	{
		return SHIELD;
	}
	float addSignature() const
	{
		return active() ? 1.0f : 0.0f;
	}
	int	maxDamage() const
	{
		return 12000;
	}
	int buildTechnology() const
	{
		return 50;
	}
	int	buildCost() const
	{
		return 3000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 1020;
	}
	// GadgetShield interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY | DAMAGE_ELF | DAMAGE_EMP | DAMAGE_PLANET)) != 0;
	}
	int	energyCost() const
	{
		return 24;
	}
	int chargeEnergyCost() const
	{
		return 0;
	}
	int maxCharge() const
	{
		return 62500;
	}
	int chargeRate() const
	{
		return 3; // + (level() * 2);
	}
	float damageModifier( dword nType ) const
	{
		return 1.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
