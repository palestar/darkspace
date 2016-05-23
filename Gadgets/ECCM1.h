/*	
	ECCM1.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ECCM1_H
#define ECCM1_H

#include "DarkSpace/GadgetECCM.h"

//----------------------------------------------------------------------------

class ECCM1 : public GadgetECCM
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 2500;
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
		return 300;
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
		return 10;
	}
	float strength() const
	{
		return 3.0f;
	}
	float range() const
	{
		return 1500.0f;
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * 30;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
