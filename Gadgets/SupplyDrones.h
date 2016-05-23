/*
	SupplyDrones.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef SUPPLYDRONES_H
#define SUPPLYDRONES_H

#include "DarkSpace/GadgetReload.h"

//----------------------------------------------------------------------------

class SupplyDrones : public GadgetReload
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 10000;
	}
	Type type()
	{
		return DRONE_REPAIR;
	}
	int buildTechnology() const
	{
		return 75;
	}
	int	buildCost() const
	{
		return 70;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}

	// GadgetReload interface
	int maxReloads() const
	{
		return 1000 + (250 * level());
	}
	int reloadRate() const
	{
		return 50;
	}
	int repairRate() const
	{
		return 300 + (10 * level());
	}
	float reloadRange() const
	{
		return 250.0f + (5.0f * level());
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF

