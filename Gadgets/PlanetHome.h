/*
	PlanetHome.h
	(c)2008 Palestar, Richard Lyle
*/

#ifndef PLANET_HOME_H
#define PLANET_HOME_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetHome : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Home";
	}
	int baseValue() const
	{
		return 75000;
	}
	int	baseHabitat() const
	{
		return 75;
	}
	int	baseFood() const
	{
		return 75;
	}
	int maxStructures() const
	{
		return 40;
	}
	int maxResources() const
	{
		return 1000000;
	}
	int maxUnits() const
	{
		return 40;
	}
	float maxControl() const
	{
		return 25000.0f + ( 2500.0f * allegiance() );
	}
	int maxAllegiance() const
	{
		return 11;
	}	
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
