/*	
	TacticalECM.h
	(c)2012 Palestar, Richard Lyle
	Meant for Fighters
*/

#ifndef TacticalECM_H
#define TacticalECM_H

#include "DarkSpace/GadgetECM.h"



//----------------------------------------------------------------------------

class TacticalECM : public GadgetECM
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
		return 1;
	}
	float strength() const
	{
		return 1.5f;
	}
	float range() const
	{
		return 450.0f;
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * (level() * 6);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
