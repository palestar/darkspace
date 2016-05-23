/*
	JumpDriveSE.h
	(c)2000 Palestar Development, Richard Lyle
*/

#ifndef JUMP_DRIVE_SE_H
#define JUMP_DRIVE_SE_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveSE : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 27500 - (level() * 300);
	}
	Type type() const
	{
		return DRIVE_JUMP;
	}
	int addValue() const
	{
		return 18000;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 280;
	}

	// GadgetJumpDrive interface
	int energyCost() const
	{
		return 3000 + (level() * 30);
	}
	int energyCharge() const
	{
		return 6;
	}
	int	maxFuel() const
	{
		return 10000;
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
		/* Original .483
		return 4000.0f + (level() * 100.0f);
		*/
		return 4000.0f;
	}
	float thrust() const
	{
		return 250.0f + (level() * 5.0f);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
