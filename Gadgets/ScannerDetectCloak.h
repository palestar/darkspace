/*
	ScannerDetectCloak.h
	(c)2005 Palestar Inc, Richard Lyle
*/

#ifndef SCANNER_DETECTCLOAK_H
#define SCANNER_DETECTCLOAK_H

#include "DarkSpace/GadgetScanner.h"



//----------------------------------------------------------------------------

class ScannerDetectCloak : public GadgetScanner
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 2700 + (level() * 150);
	}
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int	addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 75;
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
		return 320;
	}
	// GadgetDetectCloak interface
	int energyCost() const
	{
		return 10 + level();
	}
	float sensor() const
	{
		return 1000.0f + (250 * level());
	}
	float signature() const
	{
		return 8.0f + (0.5f * level());
	}
};
//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
