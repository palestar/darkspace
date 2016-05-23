/*	
	DropPodMI.h
	(c)2012 Palestar

	Used by MI to drop infantry onto enemy ships or planets
*/

#ifndef DROPPODMI_H
#define DROPPODMI_H

#include "DarkSpace/GadgetDropPod.h"


class DropPodMI : public GadgetDropPod
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
		return 800;
	}
	int chargeRate() const
	{
		return 1;
	}
	float range() const
	{
		return 350.0f;
	}
	bool canDropOnPlanet() const
	{
		return true;
	}
};

#endif
