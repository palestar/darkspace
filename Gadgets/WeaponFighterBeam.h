/*	
	WeaponFighterBeam.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_FIGHTER_BEAM_H
#define WEAPON_FIGHTER_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponFighterBeam : public GadgetBeamWeapon
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
		return 1000;
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
		return 400;
	}
	int energyCharge() const
	{
		return 1;
	}
	int damage() const
	{
		return 2;
	}
	int damageRandom() const
	{
		return 2;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 1.0f;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return 20;
	}
	float length() const
	{
		return 350.0f;
	}
	bool allowPointDefense() const
	{
		return true;
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
