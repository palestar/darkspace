/*
	SupplyDronesLight.h
	(c)2009 Palestar Inc, Richard Lyle
	Created by Robert Kelford, March 2009
*/

#ifndef SUPPLYDRONES_LIGHT_H
#define SUPPLYDRONES_LIGHT_H

#include "DarkSpace/GadgetReload.h"

//----------------------------------------------------------------------------

class SupplyDronesLight : public GadgetReload
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 5000;
	}
	Type type()
	{
		return DRONE_REPAIR;
	}
	int buildTechnology() const
	{
		return 35;
	}
	int	buildCost() const
	{
		return 40;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 80;
	}

	// GadgetReload interface
	int maxReloads() const
	{
		return 1000 + (125 * level());
	}
	int reloadRate() const
	{
		return 50;
	}
	int repairRate() const
	{
		return 150 + (5 * level());
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

