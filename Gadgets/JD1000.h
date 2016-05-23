/*
	JD1000.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef JD1000_H
#define JD1000_H

#include "DarkSpace/GadgetJD.h"



//----------------------------------------------------------------------------

class JD1000 : public GadgetJD
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	Type type() const
	{
		return INTERDICTOR;
	}
	int	maxDamage() const
	{
		return 4500 + (level() * 300);
	}
	int addValue() const
	{
		return 10000;
	}
	int buildTechnology() const
	{
		return 75;
	}
	int	buildCost() const
	{
		return 500;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}
	// GadgetJD interface
	int energyCost() const
	{
		return 55;
	}
	float range() const
	{
		return 1000.0f;
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
