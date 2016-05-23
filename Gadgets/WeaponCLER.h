/*	
	WeaponCLER.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_CLER_H
#define WEAPON_CLER_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"

//----------------------------------------------------------------------------

class WeaponCLER : public GadgetBeamWeapon
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
		return (2500 + (level() * 150));
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 30;
	}
	int	buildCost() const
	{
		return 100;
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
		return 600;
	}
	int energyCharge() const
	{
		return 2;
	}
	int damage() const
	{
		return 10;
	}
	int damageRandom() const
	{
		return 0;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.2f;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 80;
	}
	float length() const
	{
		return 500.0f;
	}
	bool allowPointDefense() const
	{
		return true;
	}
	int repairRate() const
	{
		return 1;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

