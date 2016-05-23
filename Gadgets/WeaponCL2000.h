/*
	WeaponCL2000.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONCL2000_H
#define WEAPONCL2000_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCL2000 : public GadgetBeamWeapon
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
		return 7400;
	}
	int addValue() const
	{
		return 22000;
	}
	int buildTechnology() const
	{
		return 85;
	}
	int	buildCost() const
	{
		return 240;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 480;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return 7500;
	}
	int energyCharge() const
	{
		return 25;
	}
	int damage() const
	{
		return 198;
	}
	int damageRandom() const
	{
		return 66;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 198;
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




#endif

//----------------------------------------------------------------------------
//EOF
