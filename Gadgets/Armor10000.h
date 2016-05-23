/*	
	Armor10000.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef ARMOR_10000_H
#define ARMOR_10000_H

#include "DarkSpace/GadgetArmor.h"

//----------------------------------------------------------------------------

class Armor10000 : public GadgetArmor
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int addValue() const
	{
		return 4000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 350;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 105;
	}
	// GadgetArmor interface
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY | DAMAGE_EMP)) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * 30000);
	}
	float damageModifier( dword nType ) const
	{
		return 1.0f;
	}
	int	repairRate() const
	{
		return 30000 / 300;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
