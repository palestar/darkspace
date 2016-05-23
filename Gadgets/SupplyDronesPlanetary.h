/**
	@file SupplyDronesPlanetary.h
	@brief TODO

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/22/2011 10:56:09 PM
*/

#ifndef SUPPLYDRONESPLANETARY_H
#define SUPPLYDRONESPLANETARY_H

#include "DarkSpace/GadgetAreaReload.h"

//----------------------------------------------------------------------------

class SupplyDronesPlanetary : public GadgetAreaReload
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
		return 1000;
	}
	int reloadRate() const
	{
		return 50;
	}
	int gadgetRepairRate() const
	{
		return 500;
	}
	int hullRepairRate() const
	{
		return 2000;
	}
	float reloadRange() const
	{
		return 500.0f;
	}
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
