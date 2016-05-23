/*
	PlanetArid.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_ARID_H
#define PLANET_ARID_H

#include "DarkSpace/NounPlanet.h"

//----------------------------------------------------------------------------

class PlanetArid : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Arid";
	}
	int baseValue() const
	{
		return 30000;
	}
	int baseHabitat() const
	{
		return 40;
	}
	int baseFood() const
	{
		return 10;
	}
	int maxResources() const
	{
		return 400000;
	}
	int maxAllegiance() const
	{
		return 5;
	}		
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
