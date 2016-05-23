/*
	PlanetIce.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_ICE_H
#define PLANET_ICE_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetIce : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Ice";
	}
	int baseValue() const
	{
		return 30000;
	}
	int baseHabitat() const
	{
		return 20;
	}
	int baseFood() const
	{
		return 30;
	}
	int maxResources() const
	{
		return 200000;
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
