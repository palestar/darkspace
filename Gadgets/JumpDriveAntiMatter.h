/*
	JumpDriveAntiMatter.h
	(c)2000 Palestar Development, Richard Lyle
*/

#ifndef JUMP_DRIVE_ANTIMATTER_H
#define JUMP_DRIVE_ANTIMATTER_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveAntiMatter : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 28000 - (level() * 400);
	}
	Type type() const
	{
		return DRIVE_JUMP;
	}
	int addValue() const
	{
		return 15000;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 1500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 250;
	}

	// GadgetJumpDrive interface
	int energyCost() const
	{
		return 3600 + (level() * 15);
	}
	int energyCharge() const
	{
		return 6;
	}
	int	maxFuel() const
	{
		return 1600 + (level() * 400);
	}
	int refuelRate() const
	{
		return 100;
	}
	int fuelCost() const
	{
		return 1;
	}
	float velocity() const
	{
		return 3600.0f;
	}
	float thrust() const
	{
		return 150.0f + (level() * 5.0f);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
