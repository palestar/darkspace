/*
	WeaponIonizedBeamCannon.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPON_IONIZED_BEAM_CANNON_H
#define WEAPON_IONIZED_BEAM_CANNON_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponIonizedBeamCannon : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON;
	}
	int maxDamage() const
	{
		return (2400 + (level() * 75));
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
		return 20;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 20;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return 100;
	}
	int	 energyCharge() const
	{
		return 5;
	}
	int	damage() const
	{
		return 60;
	}
	int	damageRandom() const
	{
		return (20 + level());
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 1;
	}
	int repairRate() const
	{
		return 1;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int	duration() const
	{
		return 10;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	float length() const
	{
		return (80.0f + level());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
