/*	
	WeaponCL.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_CL_H
#define WEAPON_CL_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCL : public GadgetBeamWeapon
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
		return (3300 + (level() * 225));
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 45;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 180;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return (720 + (level() * 144));
	}
	int energyCharge() const
	{
		return 9;
	}
	int damage() const
	{
		return 30;
	}
	int damageRandom() const
	{
		return 30;
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
		return 175.0f;
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
