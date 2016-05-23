/*
	JumpGate2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef JUMPGATE2_H
#define JUMPGATE2_H

#include "DarkSpace/GadgetJumpGate.h"



//----------------------------------------------------------------------------

class JumpGate2 : public GadgetJumpGate
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
		return 12250 + (level() * 1225);
	}
	int addValue() const
	{
		return 700000;
	}
	int buildTechnology() const
	{
		return 100;
	}
	int	buildCost() const
	{
		return 20000;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 1200;
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
		return 450000.0f;
	}

	float minRange() const
	{
		return 5000.0f;
	}

	dword duration() const
	{
		return TICKS_PER_SECOND * 60;
	}
	float variation() const
	{
		return 2000.0f;
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
