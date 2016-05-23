/*
	PulseShield.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef PULSESHIELD_H
#define PULSESHIELD_H

#include "DarkSpace/GadgetPulseShield.h"



//----------------------------------------------------------------------------

class PulseShield : public GadgetPulseShield
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return SPECIAL_DEFENSIVE;
	}
	int	maxDamage() const
	{
		return 5400 + (level() * 75);
	}
	int addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 100;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 240;
	}
	// GadgetPulseShield interface
	int energyNeeded() const
	{
		return 7200;
	}
	int chargeRate() const
	{
		return 4;
	}
	int areaDamage() const
	{
		return 600;
	}
	int areaDamageRandom() const
	{
		return 300;
	}
	float range() const
	{
		return 300.0f;
	}
	int chance() const
	{
		return 50;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
