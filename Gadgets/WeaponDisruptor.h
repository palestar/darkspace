/*	
	WeaponDisruptor.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_DISRUPTOR_H
#define WEAPON_DISRUPTOR_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponDisruptor : public GadgetBeamWeapon
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
		return 2500;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 40;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 160;
	}
	// GadgetWeapon interface
	int energyCost() const
	{
		return (1000 + (level() * 200));
	}
	int energyCharge() const
	{
		return 10;
	}
	int damage() const
	{	
		return 25;
	}
	int damageRandom() const
	{
		return 50;
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
		return 210.0f;
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
