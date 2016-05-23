/*
	ELF750.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef ELF750_H
#define ELF750_H

#include "DarkSpace/GadgetELF.h"

//----------------------------------------------------------------------------

class ELF750 : public GadgetELF 
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 3000;
	}
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int addValue() const
	{
		return 2000;
	}
	int buildTechnology() const
	{
		return 15;
	}
	int	buildCost() const
	{
		return 70;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}
	// GadgetELF interface
	float range() const
	{
		return 750.0f + (level() * 25);
	}

};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
