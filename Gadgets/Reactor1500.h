/*
	Reactor1500.h
	(c)2000 PaleStar Development, Richard Lyle
*/

#ifndef REACTOR_1500_H
#define REACTOR_1500_H

#include "DarkSpace/GadgetReactor.h"



//----------------------------------------------------------------------------

class Reactor1500 : public GadgetReactor
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
		return REACTOR;
	}
	int	addValue() const
	{
		return 10000;
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
		return 7500;
	}
	int energyGenerated() const
	{
		return 15;
	}
	float signature() const
	{
		return 0.75f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
