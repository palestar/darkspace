/*	
	WeaponCL500.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_CL500_H
#define WEAPON_CL500_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCL500 : public GadgetBeamWeapon
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
		return 3400;
	}
	int addValue() const
	{
		return 6000;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 75;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 270;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return 1400;
	}
	int energyCharge() const
	{
		return 14;
	}
	int damage() const
	{
		return 66;
	}
	int damageRandom() const
	{
		return 22;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 66;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 84;
	}
	float length() const
	{
		return 150.0f;
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
// EOF
