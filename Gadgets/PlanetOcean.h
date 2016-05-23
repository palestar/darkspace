/*
	PlanetOcean.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_OCEAN_H
#define PLANET_OCEAN_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetOcean : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Ocean";
	}
	int	baseValue() const
	{
		return 40000;
	}
	int baseHabitat() const
	{
		return 20;
	}
	int baseFood() const
	{
		return 60;
	}
	int maxResources() const
	{
		return 300000;
	}
	float maxControl() const
	{
		return 12500.0f + ( 1250.0f * allegiance() );
	}
	int maxAllegiance() const
	{
		return 8;
	}		
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
