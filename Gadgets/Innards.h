/*
	Innards.h
	(c)2000 PaleStar Development, Richard Lyle
*/

#ifndef INNARDS_H
#define INNARDS_H

#include "DarkSpace/GadgetDrive.h"



//----------------------------------------------------------------------------

class Innards : public GadgetDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int durability() const
	{
		return 0;
	}
	int	maxDamage() const
	{
		return 8000;
	}
	int	addValue() const
	{
		return 0;
	}
	int buildTechnology() const
	{
		return 1000;
	}
	int	buildCost() const
	{
		return 0;
	}
	dword buildFlags() const
	{
		return 0;
	}
	int	buildTime() const
	{
		return 150;
	}

	// GadgetDrive interface
	float thrust() const
	{
		return 5.0f;
	}
	int maxEnergy() const
	{
		return 7000;
	}
	int energy() const
	{
		return 14;
	}
	int energyFalloff() const
	{
		return 14;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
