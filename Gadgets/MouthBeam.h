/*
	MouthBeam.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef MOUTH_BEAM_H
#define MOUTH_BEAM_H

#include "DarkSpace/Constants.h"
#include "DarkSpace/GadgetBeamWeapon.h"



//----------------------------------------------------------------------------

class MouthBeam : public GadgetBeamWeapon
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int maxDamage() const
	{
		return 3400;
	}
	int	durability() const
	{
		return 0;
	}
	int	addValue() const
	{
		return 0;
	}
	int buildTechnology() const
	{
		return 1000;
	}
	int	buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return 0;
	}
	int	buildTime() const
	{
		return 150;
	}
	// GadgetWeapon interface
	int	energyCost() const
	{
		return 5400;
	}
	int	energyCharge() const
	{
		return 18;
	}
	int	damage() const
	{
		return 200;
	}
	int damageRandom() const
	{
		return 0;
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
		return DAMAGE_KINETIC | DAMAGE_ENERGY;
	}
	int	duration() const
	{
		return 100;
	}
	float length() const
	{
		return 250.0f;
	}
	bool allowPointDefense() const
	{
		return false;
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
