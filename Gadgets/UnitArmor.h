/*
	UnitArmor.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef UNITARMOR_H
#define UNITARMOR_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitArmor : public NounUnit
{
public:
	DECLARE_WIDGET_CLASS();

	// NounUnit interface
	int maxDamage() const
	{
		return 800;
	}
	int attackRange() const
	{
		return 1;
	}
	int attackDamage() const
	{
		return 200;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC|DAMAGE_ENERGY;
	}
	int healAmount() const
	{
		return 7;
	}
	float sensor() const
	{
		return 150.0f;
	}
	float view() const
	{
		return 10.0f;
	}
	int buildTime() const
	{
		return 60 * 4;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 25;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	bool shipCaptureBonus() const
	{
		return false;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
