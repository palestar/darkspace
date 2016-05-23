/*	
	WeaponPSIBeam.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_PSI_BEAM_H
#define WEAPON_PSI_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponPSIBeam : public GadgetBeamWeapon
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
		return (1000 + (level() * 50));
	}
	int addValue() const
	{
		return 5000;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 15;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int buildTime() const
	{
		return 20;
	}

	// GadgetWeapon interface
	int energyCost() const
	{
		return 50;
	}
	int energyCharge() const
	{
		return 2;
	}
	int damage() const
	{
		return 90;
	}
	int damageRandom() const
	{
		return 30;
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
		return DAMAGE_ENERGY | DAMAGE_PSI;
	}
	int duration() const
	{
		return 10 + floor (level() * 0.5);
	}
	float length() const
	{
		return (78.0f + (level() * 1.0f));
	}
	bool allowPointDefense() const
	{
		return true;
	}
	int repairRate() const
	{
		return 1;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
