/*
	Scanner1.h
	(c)2005 Palestar Inc, Richard Lyle
*/

#ifndef SCANNER1_H
#define SCANNER1_H

#include "DarkSpace/GadgetScanner.h"



//----------------------------------------------------------------------------

class Scanner1 : public GadgetScanner
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 1200;
	}
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int	addValue() const
	{
		return 900;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 60;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 80;
	}
	// GadgetScanner interface
	int energyCost() const
	{
		return 1;
	}
	float sensor() const
	{
		return 100.0f;
	}
	float signature() const
	{
		return 2.5f;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
