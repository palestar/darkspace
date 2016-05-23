/*	
	WeaponAmplifiedCoilBeam.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef WEAPON_AMPLIFIED_COIL_BEAM_H
#define WEAPON_AMPLIFIED_COIL_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class WeaponAmplifiedCoilBeam : public GadgetBeamWeapon
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
		return (1800 + (level() * 75));
	}
	int addValue() const
	{
		return 4000;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 13;
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
		return 300;
	}
	int energyCharge() const
	{
		return 1;
	}
	int damage() const
	{
		return 90;
	}
	int damageRandom() const
	{
		return 540;
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
