/*
	WeaponRapidCycleDisruptor.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONRAPIDCYCLEDISRUPTOR_H
#define WEAPONRAPIDCYCLEDISRUPTOR_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponRapidCycleDisruptor : public GadgetBeamWeapon
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
		return (3000 + (level() * 150));
	}
	int addValue() const
	{
		return 3000;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 50;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 300;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return 480;
	}
	int	 energyCharge() const
	{
		return 12;
	}
	int	damage() const
	{
		return 20;
	}
	int	damageRandom() const
	{
		return 1;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 1.00f;
	}
	int repairRate() const
	{
		return 2;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int	duration() const
	{
		return 40;
	}
	float length() const
	{
		return 100.0f;
	}
	bool allowPointDefense() const
	{
		return true;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

