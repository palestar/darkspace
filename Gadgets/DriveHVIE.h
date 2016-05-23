/*
	DriveHVIE.h
	(c)2008 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_HVIE_H
#define DRIVE_HVIE_H

#include "DarkSpace/GadgetDrive.h"

//----------------------------------------------------------------------------

class DriveHVIE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 90000;
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
		return 1.0f;
	}
	int buildTechnology() const
	{
		return 95;
	}
	int	buildCost() const
	{
		return 25000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 2400;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 6.0f;
	}
	int maxEnergy() const
	{
		return 24000 + (level() * 12000);
	}
	int energy() const
	{
		return 26;
	}
	int energyFalloff() const
	{
		switch (level()) 
		{
			case 10:
				return 0;
			default:
				return 32 - (level() * 3);
		}
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
