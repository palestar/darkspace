/*
	DriveHVPFE.h
	(c)2008 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_HVPFE_H
#define DRIVE_HVPFE_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DriveHVPFE : public GadgetDrive
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
		return 92100;
	}
	float addMaxVelocity() const
	{
		return 0.9f;
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
		return 2700;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 5.5f;
	}
	int maxEnergy() const
	{
		return 30000 + (level() * 15000);
	}
	int energy() const
	{
		return 30;
	}
	int energyFalloff() const
	{
		switch (level()) 
		{
			case 10:
				return 0;
			default:
				return 36 - (level() * 3);
		}
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
