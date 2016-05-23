/*	
	Armor5000.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef ARMOR_5000_H
#define ARMOR_5000_H

#include "DarkSpace/GadgetArmor.h"



//----------------------------------------------------------------------------

class Armor5000 : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 2000;
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
		return 170;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 60;
	}
	// GadgetArmor interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY | DAMAGE_EMP)) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * 15000);
	}
	float damageModifier( dword nType ) const
	{
		return 1.0f;
	}
	int	repairRate() const
	{
		return 15000 / 300;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
