/*
	DriveHVAFE.h
	(c)2008 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_HVAFE_H
#define DRIVE_HVAFE_H

#include "DarkSpace/GadgetDrive.h"

//----------------------------------------------------------------------------

class DriveHVAFE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 120000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 80000;
	}
	float addMaxVelocity() const
	{
		return 1.1f;
	}
	int buildTechnology() const
	{
		return 90;
	}
	int	buildCost() const
	{
		return 21930;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 2000;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 6.75f;
	}
	int maxEnergy() const
	{
		return 18000 + (level() * 9000);
	}
	int energy() const
	{
		return 22;
	}
	int energyFalloff() const
	{
		switch (level()) 
		{
			case 10:
				return 0;
			default:
				return 28 - (level() * 3);
		}
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
