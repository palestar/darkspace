/*	
	TractorBeam300.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef TRACTOR_BEAM_300_H
#define TRACTOR_BEAM_300_H

#include "DarkSpace/GadgetTractorBeam.h"

//----------------------------------------------------------------------------

class TractorBeam300 : public GadgetTractorBeam
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 6000 + (level() * 300);
	}
	Type type()
	{
		return TRACTOR_BEAM;
	}
	int addValue() const
	{
		return 2500;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 250;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 480;
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * 2;
	}
	// GadgetWeapon interface
	int	energyCost() const
	{
		return 1;
	}
	/*
	int energyFalloff() const
	{
		return 1;
	}
	*/
	float length() const
	{
		return 150.0f + (level() * 20);
	}
};

//----------------------------------------------------------------------------

#endif

