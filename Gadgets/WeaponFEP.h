/*	
	WeaponFEP.h
	Focused Energy Pulse
	(c)2008 Palestar, Richard Lyle
*/

#ifndef WEAPON_FEP_H
#define WEAPON_FEP_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponFEP : public GadgetBeamWeapon
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
		return (3250 + (level() * 150));
	}
	int addValue() const
	{
		return 2500;
	}
	int buildTechnology() const
	{
		return 45;
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
		return (1320 + (level() * 264));
	}
	int energyCharge() const
	{
		return 11;
	}
	int damage() const
	{
		return 45;
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
		return (120 + (level() * 24));
	}
	float length() const
	{
		return 245.0f;
	}
	bool allowPointDefense() const
	{
		return true;
	}
	int repairRate() const
	{
		return 4;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
