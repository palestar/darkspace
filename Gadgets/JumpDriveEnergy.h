/*
	JumpDriveEnergy.h
	(c)2008 Palestar Development, Richard Lyle
	Created by Robert Kelford, Oct 2008.
*/

#ifndef JUMP_DRIVE_ENERGY_H
#define JUMP_DRIVE_ENERGY_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveEnergy : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 45000 + (level() * 2000);
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
		return 25;
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
		return 3500 + (level() * 50);
	}
	int energyCharge() const
	{
		return 4;
	}
	int	maxFuel() const
	{
		return 0;
	}
	int refuelRate() const
	{
		return 0;
	}
	int fuelCost() const
	{
		return 0;
	}
	float velocity() const
	{	
		return 2200.0f;
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
