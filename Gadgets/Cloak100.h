/*	
	Cloak100.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef CLOAK_100_H
#define CLOAK_100_H

#include "DarkSpace/GadgetCloak.h"



//----------------------------------------------------------------------------

class Cloak100 : public GadgetCloak
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 4500 + (level() * 750);
	}
	Type type() const
	{
		return SPECIAL_DEFENSIVE;
	}
	int	addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 20;
	}
	int	buildCost() const
	{
		return 1000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}
	// GadgetCloak interface
	float energyCost() const
	{
		return 0.02f;
	}
	int cooldown() const
	{
		return (level() * 10 * TICKS_PER_SECOND);	
	}

};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
