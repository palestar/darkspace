/*
	PlanetBarren.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_BARREN_H
#define PLANET_BARREN_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetBarren : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Barren";
	}
	int baseValue() const
	{
		return 10000;
	} 
	int baseHabitat() const
	{
		return 0;
	}
	int baseFood() const
	{
		return 0;
	}
	int maxResources() const
	{
		return 100000;
	}
	int maxAllegiance() const
	{
		return 3;
	}		
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
