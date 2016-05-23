/*
	WeaponFluxWave.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONFLUXWAVE_H
#define WEAPONFLUXWAVE_H

#include "DarkSpace/GadgetAreaWeapon.h"

//----------------------------------------------------------------------------

class WeaponFluxWave : public GadgetAreaWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_CLOSE;
	}
	int	maxDamage() const
	{
		return (5100 + (level() * 150));
	}
	int addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 150;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 260;
	}
	// GadgetAreaWeapon interface
	int	energyNeeded() const
	{
		return 18000;
	}
	int	chargeRate() const
	{
		return 15;
	}
	float range() const
	{
		return 300.0f;
	}
	int	damageCaused() const
	{
		return 3000;
	}
	int 	damageRandom() const
	{
		return 6000;
	}
	dword 	damageType() const
	{
		return DAMAGE_EMP | DAMAGE_ENERGY;
	}
	int damageRolls() const
	{
		return level() + 5;		// damage up to 4 internal systems
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
