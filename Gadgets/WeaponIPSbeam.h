/*	
	WeaponIPSbeam.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef WEAPON_IPS_BEAM_H
#define WEAPON_IPS_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponIPSbeam : public GadgetBeamWeapon
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
		return (9000 + (level() * 750));
	}
	int addValue() const
	{
		return 3000;
	}
	int buildTechnology() const
	{
		return 55;
	}
	int	buildCost() const
	{
		return 30;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 75;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return 300;
	}
	int energyCharge() const
	{
		return 1;
	}
	int damage() const
	{
		return 180;
	}
	int damageRandom() const
	{
		return 120;
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
		return 30;
	}
	float length() const
	{
		return 600.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
