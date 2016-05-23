/*
	DriveHVAME.h
	(c)2008 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_HVAME_H
#define DRIVE_HVAME_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DriveHVAME : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 60000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 91200;
	}
	float addMaxVelocity() const
	{
		return 1.2f;
	}
	int buildTechnology() const
	{
		return 100;
	}
	int	buildCost() const
	{
		return 28500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 2500;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 6.85f;
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
