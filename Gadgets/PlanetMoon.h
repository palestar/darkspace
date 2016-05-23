/*
	PlanetMoon.h
	(c)2008 Palestar, Richard Lyle
	Created by Drafell in 2008
*/

#ifndef PLANET_MOON_H
#define PLANET_MOON_H

#include "DarkSpace/NounPlanet.h"

//----------------------------------------------------------------------------

class PlanetMoon : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Moon";
	}
	int maxStructures() const
	{
		return 0;
	}
	int maxUnits() const
	{
		return 0;
	}
	int maxResources() const
	{
		return 0;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
