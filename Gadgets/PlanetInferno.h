/*
	PlanetInferno.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_INFERNO_H
#define PLANET_INFERNO_H

#include "DarkSpace/NounPlanet.h"

//----------------------------------------------------------------------------

class PlanetInferno : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Inferno";
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
