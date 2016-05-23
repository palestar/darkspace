/*	
	WeaponDSbeam.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef WEAPON_DS_BEAM_H
#define WEAPON_DS_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponDSbeam : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return UNKNOWN;
	}
	int maxDamage() const
	{
		return 5000;
	}
	int addValue() const
	{
		return 7000;
	}
	int buildTechnology() const
	{
		return 35;
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
		return 105;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return 5400;
	}
	int energyCharge() const
	{
		return 18;
	}
	int damage() const
	{
		return 60;
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
		return 1;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 100;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	float length() const
	{
		return 250.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
