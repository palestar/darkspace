/*
	PlanetGas.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef PLANET_GAS_H
#define PLANET_GAS_H

#include "DarkSpace/NounPlanet.h"



//----------------------------------------------------------------------------

class PlanetGas : public NounPlanet
{
public:
	DECLARE_WIDGET_CLASS();

	// NounPlanet interface
	const char * description() const
	{
		return "Gas";
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
