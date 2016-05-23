/*
	DrivePIE.h
	(c)2009 PaleStar Development, Richard Lyle
	Created by Robert Kelford, June 2009
*/

#ifndef DRIVE_PIE_H
#define DRIVE_PIE_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DrivePIE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 45000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 16000;
	}
	float addMaxVelocity() const
	{
		return 1.0f;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 5000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 480;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 6.0f;
	}
	int maxEnergy() const
	{
		return 16000 + (level() * 8000);
	}
	int energy() const
	{
		return 26;
	}
	int energyFalloff() const
	{
		return 32 - (level() * 3);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
