/*	
	ECM.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ECM_H
#define ECM_H

#include "DarkSpace/GadgetECM.h"



//----------------------------------------------------------------------------

class ECM : public GadgetECM
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 2250 + (325 * level());
	}
	Type type() const
	{
		return SPECIAL_DEFENSIVE;
	}
	int	addValue() const
	{
		return 1200;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 130;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 160;
	}
	// GadgetECM interface
	int	energyCost() const
	{
		return 5;
	}
	float strength() const
	{
		return 3.0f + (0.2f * level());
	}
	float range() const
	{
		return 500.0f + (50.0f * level());
	}
	int cooldown() const
	{
		return 9 * TICKS_PER_SECOND;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
