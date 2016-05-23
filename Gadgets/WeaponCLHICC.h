/*
	WeaponCLHICC.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONCLHICC_H
#define WEAPONCLHICC_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCLHICC : public GadgetBeamWeapon
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
		return (1920 + (level() * 384));
	}
	int energyCharge() const
	{
		return 32;
	}
	int damage() const
	{
		return 120;
	}
	int damageRandom() const
	{
		return 80;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.6f;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return (60 + (level() * 12));
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

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
