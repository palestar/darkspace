/*
	DriveAME.h
	(c)2004 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_AME_H
#define DRIVE_AME_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DriveAME : public GadgetDrive
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
		return 1.2f;
	}
	int buildTechnology() const
	{
		return 35;
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
		return 500;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 6.85f;
	}
	int maxEnergy() const
	{
		return 12000 + (level() * 6000);
	}
	int energy() const
	{
		return 22;
	}
	int energyFalloff() const
	{
		return 28 - (level() * 3);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
