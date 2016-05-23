/*	
	WeaponDisruptorA.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_DISRUPTOR_Assault_H
#define WEAPON_DISRUPTOR_Assault_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponDisruptorA : public GadgetBeamWeapon
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
		return (8700 + (level() * 150));
	}
	int addValue() const
	{
		return 22500;
	}
	int buildTechnology() const
	{
		return 55;
	}
	int	buildCost() const
	{
		return 120;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 360;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return (4000 + (level() * 800));
	}
	int energyCharge() const
	{
		return 40;
	}
	int damage() const
	{
		return 100;
	}
	int damageRandom() const
	{
		return 200;
	}
	bool reverseFalloff() const
	{
		return false;
	}
	float damageFalloff() const
	{
		return 0.9f;
	}
	dword damageType() const
	{
		return DAMAGE_ENERGY;
	}
	int duration() const
	{
		return (100 + (level() * 20));
	}
	float length() const
	{
		return 300.0f;
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
