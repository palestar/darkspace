/**
	@file SupplyDronesStation.h
	@brief TODO

	(c)2011 Palestar Inc
	@author Jack Wallace @date 7/05/2011 23:34:51 PM
*/

#ifndef SUPPLYDRONESSTATION_H
#define SUPPLYDRONESSTATION_H

#include "DarkSpace/GadgetAreaReload.h"

//----------------------------------------------------------------------------

class SupplyDronesStation : public GadgetAreaReload
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
		return 3500;
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
		return 1000;
	}
	float reloadRange() const
	{
		return 500.0f;
	}
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
