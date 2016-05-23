/*	
	ECCM.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ECCM_H
#define ECCM_H

#include "DarkSpace/GadgetECCM.h"



//----------------------------------------------------------------------------

class ECCM : public GadgetECCM
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
		return 5;
	}
	float strength() const
	{
		return 3.0f + (0.2f * level());
	}
	float range() const
	{
		return 1000.0f + (150.0f * level());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
