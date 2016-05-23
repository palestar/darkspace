/*
	Reactor500.h
	(c)2000 PaleStar Development, Richard Lyle
*/

#ifndef REACTOR_500_H
#define REACTOR_500_H

#include "DarkSpace/GadgetReactor.h"

//----------------------------------------------------------------------------

class Reactor500 : public GadgetReactor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 1975 + (level() * 145);
	}
	Type type() const
	{
		return UNKNOWN;
	}
	int	addValue() const
	{
		return 4000;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 325;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 125;
	}
	// GadgetReactor interface
	int maxEnergy() const
	{
		return 5625;
	}
	int energyGenerated() const
	{
		return 7;
	}
	float signature() const
	{
		return 0.33f; // (maxEnergy()/10000.0f) * (energy()/12.0f)
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
