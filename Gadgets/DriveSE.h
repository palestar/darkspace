/*
	DriveSE.h
	(c)2005 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_SE_H
#define DRIVE_SE_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class DriveSE : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 75000;
	}
	Type type() const
	{
		return DRIVE;
	}
	int	addValue() const
	{
		return 20480;
	}
	float addMaxVelocity() const
	{
		return 1.3f;
	}
	int buildTechnology() const
	{
		return 45;
	}
	int	buildCost() const
	{
		return 6400;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 400;
	}
	// GadgetDrive interface
	float thrust() const
	{
		return 7;
	}
	int maxEnergy() const
	{
		return 24000 + (level() * 12000);
	}
	int energy() const
	{
		return 36;
	}
	int energyFalloff() const
	{
		return 42 - (level() * 3);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
