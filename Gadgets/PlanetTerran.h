/*
	PlanetTerran.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_TERRAN_H
#define PLANET_TERRAN_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetTerran : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Terran";
	}
	int baseValue() const
	{
		return 50000;
	}
	int	baseHabitat() const
	{
		return 50;
	}
	int	baseFood() const
	{
		return 50;
	}
	int maxResources() const
	{
		return 600000;
	}
	float maxControl() const
	{
		return 15000.0f + ( 1500.0f * allegiance() );
	}
	int maxAllegiance() const
	{
		return 10;
	}		
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
