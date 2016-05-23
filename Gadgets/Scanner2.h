/*
	Scanner2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef SCANNER2_H
#define SCANNER2_H

#include "DarkSpace/GadgetScanner.h"



//----------------------------------------------------------------------------

class Scanner2 : public GadgetScanner
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
		return 5;
	}
	float sensor() const
	{
		return 500.0f;
	}
	float signature() const
	{
		return 5.0f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
