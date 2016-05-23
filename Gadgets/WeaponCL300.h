/*	
	WeaponCL300.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_CL300_H
#define WEAPON_CL300_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"

//----------------------------------------------------------------------------

class WeaponCL300 : public GadgetBeamWeapon
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
		return 2200;
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 6;
	}
	int	buildCost() const
	{
		return 44;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 160;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return 600;
	}
	int energyCharge() const
	{
		return 12;
	}
	int damage() const
	{
		return 33;
	}
	int damageRandom() const
	{
		return 16;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 32;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 60;
	}
	float length() const
	{
		return 120.0f;
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
