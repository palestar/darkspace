#ifndef REDUCE_WEAPON_RANGE_H 
#define REDUCE_WEAPON_RANGE_H

#include "DarkSpace/GadgetModifierEW.h"

//----------------------------------------------------------------------------

class ReduceWeaponRange : public GadgetModifierEW 
{
public:
	DECLARE_WIDGET_CLASS();
	Type type() const
	{
		return SPECIAL_OFFENSIVE;
	}
	int	addValue() const
	{
		return 1200;
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 300;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 140;
	}
	int	energyCost() const
	{
		return 10;
	}
	float strength() const
	{
		return 10.0f;
	}
	float range() const
	{
		return 500.0f;
	}
	int cooldown() const
	{
		return TICKS_PER_SECOND * 30;
	}
	ModifierType modifierType() const
	{
		return MT_WEAPON_RANGE;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
