/*	
	WeaponCLICC.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_CLICC_H
#define WEAPON_CLICC_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponCLICC : public GadgetBeamWeapon
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
		return (480 + (level() * 96));
	}
	int energyCharge() const
	{
		return 8;
	}
	int damage() const
	{
		return 30;
	}
	int damageRandom() const
	{
		return 20;
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
		return 140.0f;
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
