/*
	UnitInfantry.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef UNITINFANTRY_H
#define UNITINFANTRY_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitInfantry : public NounUnit
{
public:
	DECLARE_WIDGET_CLASS();

	// NounUnit interface
	int maxDamage() const
	{
		return 400;
	}
	int attackRange() const
	{
		return 0;
	}
	int attackDamage() const
	{
		return 100;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC|DAMAGE_ENERGY;
	}
	int healAmount() const
	{
		return 10;
	}
	float sensor() const
	{
		return 2000.0f;
	}
	float view() const
	{
		return 10.0f;
	}
	int buildTime() const
	{
		return 60 * 2;	
	}
	int buildTechnology() const
	{
		return 0;
	}
	int	buildCost() const
	{
		return 25;
	}
	dword buildFlags() const
	{
		return 0;
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
