/*
	Scanner.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef SCANNER_H
#define SCANNER_H

#include "DarkSpace/GadgetScanner.h"



//----------------------------------------------------------------------------

class Scanner : public GadgetScanner
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
		return SPECIAL_OFFENSIVE;
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
	// GadgetScanner interface
	int energyCost() const
	{
		return 5 + level();
	}
	float sensor() const
	{
		return 2500.0f + (250 * level());
	}
	float signature() const
	{
		return 5.0f + (0.5f * level());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
