/*
	JumpDriveHMA.h
	(c)2000 Palestar Development, Richard Lyle
*/

#ifndef JUMP_DRIVE_HMA_H
#define JUMP_DRIVE_HMA_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveHMA : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 41500 + (level() * 1275);
	}
	Type type() const
	{
		return DRIVE_JUMP;
	}
	int addValue() const
	{
		return 750000;
	}
	int buildTechnology() const
	{
		return 18;
	}
	int	buildCost() const
	{
		return 3000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 1400;
	}

	// GadgetJumpDrive interface
	int energyCost() const
	{
		return 15000;
	}
	int energyCharge() const
	{
		return 10;
	}
	int	maxFuel() const
	{
		return 10000;
	}
	int refuelRate() const
	{
		return 200;
	}
	int fuelCost() const
	{
		return 1;
	}
	float velocity() const
	{
		return 500.0f;
	}
	float thrust() const
	{
		return 100.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
