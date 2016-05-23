/*	
	ECCM2.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ECCM2_H
#define ECCM2_H

#include "DarkSpace/GadgetECCM.h"



//----------------------------------------------------------------------------

class ECCM2 : public GadgetECCM
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 2250 + (350 * level());
	}
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int	addValue() const
	{
		return 1200;
	}
	int buildTechnology() const
	{
		return 10;
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
		return 140;
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
		return (300.0f * level());
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
