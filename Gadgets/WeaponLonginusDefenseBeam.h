/*
	WeaponLonginusDefenseBeam.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef WEAPONLONGINUSDEFENSEBEAM_H
#define WEAPONLONGINUSDEFENSEBEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponLonginusDefenseBeam : public GadgetBeamWeapon
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
		return 360;
	}
	int	 energyCharge() const
	{
		return 12;
	}
	int	damage() const
	{
		return 10;
	}
	int	damageRandom() const
	{
		return 0;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.5f;
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
		return 30;
	}
	float length() const
	{
		return 125.0f;
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
