/*
	UnitHeavyInfantry.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef UNITHEAVYINFANTRY_H
#define UNITHEAVYINFANTRY_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitHeavyInfantry : public NounUnit
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
		return 0;
	}
	int attackDamage() const
	{
		return 150;
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
		return 60 * 4;	
	}
	int buildTechnology() const
	{
		return 10;
	}
	int	buildCost() const
	{
		return 150;
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
