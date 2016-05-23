/*	
	WeaponIPbeam.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef WEAPON_IP_BEAM_H
#define WEAPON_IP_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponIPbeam : public GadgetBeamWeapon
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
		return 1200;
	}
	int addValue() const
	{
		return 2500;
	}
	int buildTechnology() const
	{
		return 25;
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
		return 60;
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
		return 150;
	}
	int damageRandom() const
	{
		return 300;
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
	bool allowPointDefense() const
	{
		return false;
	}
	float length() const
	{
		return 600.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
