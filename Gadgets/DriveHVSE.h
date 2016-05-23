/*
	DriveHVSE.h
	(c)2008 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_HVSE_H
#define DRIVE_HVSE_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DriveHVSE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 150000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 102400;
	}
	float addMaxVelocity() const
	{
		return 1.3f;
	}
	int buildTechnology() const
	{
		return 105;
	}
	int	buildCost() const
	{
		return 32000;
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
		return 7.0f;
	}
	int maxEnergy() const
	{
		return 36000 + (level() * 18000);
	}
	int energy() const
	{
		return 36;
	}
	int energyFalloff() const
	{
		switch (level()) 
		{
			case 10:
				return 0;
			default:
				return 42 - (level() * 3);
		}
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
