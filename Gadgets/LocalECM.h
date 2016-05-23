/*	
	LocalECM.h
	(c)2005 Palestar, Richard Lyle
*/

#ifndef LocalECM_H
#define LocalECM_H

#include "DarkSpace/GadgetECM.h"



//----------------------------------------------------------------------------

class LocalECM : public GadgetECM
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
		return SPECIAL_DEFENSIVE;
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
		return (level() * 2);
	}
	float strength() const
	{
		return 3.0f;
	}
	float range() const
	{
		return (level() * 5.0f);
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * level();
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
