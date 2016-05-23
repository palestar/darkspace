/*
	WeaponELF.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONELF_H
#define WEAPONELF_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponELF : public GadgetBeamWeapon
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
		return (3900 + (level() * 150));
	}
	int	addValue() const
	{
		return 8500;
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
		return 300;
	}
	// GadgetWeapon interface
	int	energyCost() const
	{
		return (1000 + (level() * 200));
	}
	int	energyCharge() const
	{
		return 5;
	}
	int	damage() const
	{
		return 50;
	}
	int	damageRandom() const
	{
		return 100;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.9f;
	}
	dword damageType() const
	{
		return DAMAGE_ELF;
	}
	int	duration() const
	{
		return (100 + (level() * 20));
	}
	float length() const
	{
		return 240.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	int repairRate() const
	{
		return 2;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
