/*
	RF250.h
	(c)2011 Palestar Inc, Jack Wallace
*/

#ifndef RF250_H
#define RF250_H

#include "DarkSpace/GadgetRF.h"



//----------------------------------------------------------------------------

class RF250 : public GadgetRF
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return REPAIR_FIELD;
	}
	int	maxDamage() const
	{
		return 4500 + (level() * 300);
	}
	int addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 75;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}
	// GadgetRF interface
	int energyCost() const
	{
		return 30;
	}
	float range() const
	{
		return 250.0f;
	}
	int repairRate() const
	{
		return 300 + (10 * level());
	}	
};




#endif

//----------------------------------------------------------------------------
//EOF
