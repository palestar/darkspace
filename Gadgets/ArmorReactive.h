#ifndef ARMORREACTIVE_R
#define ARMORREACTIVE_R

#include "DarkSpace/GadgetArmorReactive.h"

//----------------------------------------------------------------------------

class ArmorReactive : public GadgetArmorReactive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	addValue() const
	{
		return 6000;
	}
	Type type() const
	{
		return ARMOR;
	}
	int buildTechnology() const
	{
		return 80;
	}
	int	buildCost() const
	{
		return 920;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 660;
	}
	// GadgetArmorReactive interface
	float addMass() const
	{
		return  armor() / 200000.0f;	
	}
	bool canDeflect( dword nType ) const
	{
		return (nType & (DAMAGE_KINETIC | DAMAGE_ENERGY )) != 0;
	}
	int	maxArmor() const
	{
		return (modifier() * ( 45000 + ( 15000 * level())));
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
