/*
	DriveAFE.h
	(c)2004 PaleStar Development, Richard Lyle
*/

#ifndef DRIVE_AFE_H
#define DRIVE_AFE_H

#include "DarkSpace/GadgetDrive.h"

//----------------------------------------------------------------------------

class DriveAFE : public GadgetDrive
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
		return 16000;
	}
	float addMaxVelocity() const
	{
		return 1.1f;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 4386;
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
		return 6.75f;
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
