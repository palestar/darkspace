/*	
	FocusedTECCM.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef FocusedTECCM_H
#define FocusedTECCM_H

#include "DarkSpace/GadgetTECCM.h"



//----------------------------------------------------------------------------

class FocusedTECCM : public GadgetTECCM
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
	// GadgetTECCM interface
	int	energyCost() const
	{
		return (2 * level());
	}
	float strength() const
	{
		return 2.25f;
	}
	float range() const
	{
		return (450.0f * level());
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
