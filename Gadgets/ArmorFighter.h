/*	
	ArmorFighter.h
	(c)2004 Palestar, Richard Lyle
*/

#ifndef ARMOR_FIGHTER_H
#define ARMOR_FIGHTER_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class ArmorFighter : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 500;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 5;
	}
	int	buildCost() const
	{
		return 90;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 100;
	}
	// GadgetArmor interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * (2000 + (500 * level())));
	}
	float damageModifier( dword nType ) const
	{
		return 1.0f;
	}
	int	repairRate() const
	{
		return 0;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
