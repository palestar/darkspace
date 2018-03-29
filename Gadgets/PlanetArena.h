/*
	PlanetArena.h
	This is a ridiculous planet for the usage of Arena/Battle Royale
	type maps. It is not intended for usage outside of these scenarios.
	
	There is code in NounPlanet.cpp to auto set this to NOT_CAPTURABLE
	via the description().
*/

#ifndef PLANET_ARENA_H
#define PLANET_ARENA_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetArena : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Arena";
	}
	int baseValue() const
	{
		return 75000;
	}
	int	baseHabitat() const
	{
		return 1000;
	}
	int	baseFood() const
	{
		return 1000;
	}
	int maxStructures() const
	{
		return 60;
	}
	int maxResources() const
	{
		return 100000000;
	}
	int maxUnits() const
	{
		return 100;
	}
	float maxControl() const
	{
		return 1000000.0f;
	}
	int maxAllegiance() const
	{
		return 20;
	}	
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
