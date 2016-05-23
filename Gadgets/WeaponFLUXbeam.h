/*	
	WeaponFLUXbeam.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_FLUX_BEAM_H
#define WEAPON_FLUX_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponFLUXbeam : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return WEAPON_CLOSE;
	}
	int	maxDamage() const
	{
		return (4200 + (level() * 150));
	}
	int	addValue() const
	{
		return 8000;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 90;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 360;
	}
	// GadgetWeapon interface
	int	energyCost() const
	{
		return (1440 + (level() * 288));
	}
	int	energyCharge() const
	{
		return 18;
	}
	int	damage() const
	{
		return 60;
	}
	int	damageRandom() const
	{
		return 60;
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
		return DAMAGE_ENERGY | DAMAGE_EMP;
	}
	int	duration() const
	{
		return (80 + (level() * 16));
	}
	float length() const
	{
		return 200.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
	int repairRate() const
	{
		return 2;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
