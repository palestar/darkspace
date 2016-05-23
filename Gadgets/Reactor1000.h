/*
	Reactor1000.h
	(c)2000 PaleStar Development, Richard Lyle
*/

#ifndef REACTOR_1000_H
#define REACTOR_1000_H

#include "DarkSpace/GadgetReactor.h"



//----------------------------------------------------------------------------

class Reactor1000 : public GadgetReactor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 4500 + (level() * 325);
	}
	Type type() const
	{
		return UNKNOWN;
	}
	int	addValue() const
	{
		return 6500;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 750;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 300;
	}
	// GadgetReactor interface
	int maxEnergy() const
	{
		return 9000;
	}
	int energyGenerated() const
	{
		return 10;
	}
	float signature() const
	{
		return 0.75f; // maxEnergy()/10000.0f) * (energy()/12.0f)
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
