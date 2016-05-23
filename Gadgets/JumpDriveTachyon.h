/*
	JumpDriveTachyon.h
	(c)2000 Palestar Development, Richard Lyle
*/

#ifndef JUMP_DRIVE_TACHYON_H
#define JUMP_DRIVE_TACHYON_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveTachyon : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 37000 + (level() * 875);
	}
	Type type() const
	{
		return DRIVE_JUMP;
	}
	int addValue() const
	{
		return 12000;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 250;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}

	// GadgetJumpDrive interface
	int energyCost() const
	{
		return 2400 + (level() * 10);
	}
	int energyCharge() const
	{
		return 4;
	}
	int	maxFuel() const
	{
		return 2400 + (level() * 600);
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
		return 2400.0f;
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
