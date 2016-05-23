/*	
	WeaponCLPlanet.h
	(c)2009 Palestar, Richard Lyle
	Created by Robert Kelford, 2009
*/

#ifndef WEAPON_CLPLANET_H
#define WEAPON_CLPLANET_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCLPlanet : public GadgetBeamWeapon
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
		return 4350;
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 0;
	}
	int	buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 0;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return 1100;
	}
	int energyCharge() const
	{
		return 13;
	}
	int damage() const
	{
		return 56;
	}
	int damageRandom() const
	{
		return 27;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 1.0;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 75;
	}
	float length() const
	{
		return 320.0f;
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
