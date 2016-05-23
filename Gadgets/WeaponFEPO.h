/*
	WeaponFEPO.h
	Focused Energy Pulse Overload
	(c)2008 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONFEPO_H
#define WEAPONFEPO_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponFEPO : public GadgetBeamWeapon
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
		return (9000 + (level() * 325));
	}
	int addValue() const
	{
		return 17000;
	}
	int buildTechnology() const
	{
		return 45;
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
		return (5280 + (level() * 1056));
	}
	int energyCharge() const
	{
		return 44;
	}
	int damage() const
	{
		return 180;
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
		return 1;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return (120 + (level() * 24));
	}
	float length() const
	{
		return 350.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	int repairRate() const
	{
		return 4;
	}
};



#endif

//----------------------------------------------------------------------------
//EOF
