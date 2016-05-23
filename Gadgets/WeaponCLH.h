/*
	WeaponCLH.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONCLH_H
#define WEAPONCLH_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCLH : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_CLOSE;
	}
	int maxDamage() const
	{
		return (9000 + (level() * 250));
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 65;
	}
	int	buildCost() const
	{
		return 120;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 405;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return (2880 + (level() * 576));
	}
	int energyCharge() const
	{
		return 36;
	}
	int damage() const
	{
		return 120;
	}
	int damageRandom() const
	{
		return 120;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.75f;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return (80 + (level() * 16));
	}
	float length() const
	{
		return 250.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	int repairRate() const
	{
		return 3;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
