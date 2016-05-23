/*	
	ECM2.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ECM2_H
#define ECM2_H

#include "DarkSpace/GadgetECM.h"



//----------------------------------------------------------------------------

class ECM2 : public GadgetECM
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
		return 120;
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
		return (2 * level());
	}
	float strength() const
	{
		return 3.0f;
	}
	float range() const
	{
		return (150.0f * level());
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * (level() * 2);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
