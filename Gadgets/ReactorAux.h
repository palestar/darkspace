/*
	ReactorAux.h
	(c)2004 PaleStar Development, Richard Lyle
*/

#ifndef REACTOR_AUX_H
#define REACTOR_Aux_H

#include "DarkSpace/GadgetReactor.h"



//----------------------------------------------------------------------------

class ReactorAux : public GadgetReactor
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
		return 8000;
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
