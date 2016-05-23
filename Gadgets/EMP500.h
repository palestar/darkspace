/*
	EMP500.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef EMP500_H
#define EMP500_H

#include "DarkSpace/GadgetEMP.h"



//----------------------------------------------------------------------------

class EMP500 : public GadgetEMP
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 1800;
	}
	Type type() const
	{
		return WEAPON_CLOSE;
	}
	int addValue() const
	{
		return 5000;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 300;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 60;
	}
	// GadgetEMP interface
	int	energyNeeded() const
	{
		return 8000;
	}

	int	chargeRate() const
	{
		return 5;
	}

	float range() const
	{
		return 250.0f + (level() * 25);
	}

	int	damageCaused() const
	{
		return 10000 + (level() * 1000);
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
