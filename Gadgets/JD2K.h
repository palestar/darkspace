/*
	JD2K.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef JD2K_H
#define JD2K_H

#include "DarkSpace/GadgetJD.h"



//----------------------------------------------------------------------------

class JD2K : public GadgetJD
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
		/* Original .483
		return 25 - level();
		*/
		return 0; // this is just a temp device not ment be used on a ship
	}
	float range() const
	{
		return 2000.0f;
	}
};




#endif

//----------------------------------------------------------------------------
//EOF
