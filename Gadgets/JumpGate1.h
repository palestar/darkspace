/*
	JumpGate1.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef JUMPGATE1_H
#define JUMPGATE1_H

#include "DarkSpace/GadgetJumpGate.h"



//----------------------------------------------------------------------------

class JumpGate1 : public GadgetJumpGate
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return JUMP_GATE;
	}
	int	maxDamage() const
	{
		return 8750;
	}
	int addValue() const
	{
		return 500000;
	}
	int buildTechnology() const
	{
		return 90;
	}
	int	buildCost() const
	{
		return 10000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 600;
	}
	// GadgetEMP interface
	int	energyNeeded() const
	{
		return 72000;
	}

	int	chargeRate() const
	{
		return 12;
	}

	float range() const
	{
		return (level() * 150000) - 600000.0f;
	}

	float minRange() const
	{
		return 5000.0f;
	}

	dword duration() const
	{
		return TICKS_PER_SECOND * ((level() * 15) - 60);
	}
	float variation() const
	{
		return 2000.0f;
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
