/*	
	MiningBeam.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef MINING_BEAM_H
#define MINING_BEAM_H

#include "DarkSpace/GadgetMiningBeam.h"



//----------------------------------------------------------------------------

class MiningBeam : public GadgetMiningBeam
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int maxDamage() const
	{
		return 4200 + (480 * level());
	}
	Type type() const
	{
		return MINING_BEAM;
	}
	int addValue() const
	{
		return 3100;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 50;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 500;
	}

	// GadgetMiningBeam interface
	float mineScale() const
	{
		return 15.0f;
	}
	int energyCost() const
	{
		return 4;
	}
	float range() const
	{
		return 300.0f + (level() * 20.0f);
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
