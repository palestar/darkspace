/*	
	DropPod.h
	(c)2012 Palestar

	Used by pirates to drop infantry onto enemy ships
*/

#ifndef DROPPOD_H
#define DROPPOD_H

#include "DarkSpace/GadgetDropPod.h"


class DropPod : public GadgetDropPod
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 4500 + (level() * 325);
	}
	Type type() const
	{
		return SPECIAL;
	}
	int	addValue() const
	{
		return 8000;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 750;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 300;
	}
	//GadgetDropPod interface
	int energyNeeded() const
	{
		return 2400;
	}
	int chargeRate() const
	{
		return 1;
	}
	float range() const
	{
		return 300.0f;
	}
	bool canDropOnPlanet() const
	{
		return false;
	}
};

#endif
