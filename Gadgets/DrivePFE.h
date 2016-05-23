/*
	DrivePFE.h
	(c)2004 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_PFE_H
#define DRIVE_PFE_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DrivePFE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 30000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 18240;
	}
	float addMaxVelocity() const
	{
		return 0.9f;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 5700;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 540;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 5.5f;
	}
	int maxEnergy() const
	{
		return 20000 + (level() * 10000);
	}
	int energy() const
	{
		return 30;
	}
	int energyFalloff() const
	{
		return 36 - (level() * 3);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
