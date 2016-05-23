/*
	UnitSpy.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef UNITSPY_H
#define UNITSPY_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitSpy : public NounUnit
{
public:
	DECLARE_WIDGET_CLASS();

	// Noun interface
	float baseSignature() const
	{
		return 0.1f;
	}

	// NounUnit interface
	int maxDamage() const
	{
		return 50;
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
		return 1;
	}
	float sensor() const
	{
		return 250.0f;
	}
	float view() const
	{
		return 100.0f;
	}
	int buildTime() const
	{
		return 60 * 5;	
	}
	int buildTechnology() const
	{
		return 65;
	}
	int	buildCost() const
	{
		return 250;
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
