/*
	WeaponCL1000.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONCL1000_H
#define WEAPONCL1000_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCL1000 : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return UNKNOWN;
	}
	int maxDamage() const
	{
		return 18000;
	}
	int addValue() const
	{
		return 6000;
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
		return 3600;
	}
	int energyCharge() const
	{
		return 18;
	}
	int damage() const
	{
		return 132;
	}
	int damageRandom() const
	{
		return 44;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 132;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 100;
	}
	float length() const
	{
		return 200.0f;
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



#endif

//----------------------------------------------------------------------------
//EOF
