/*
	UnitRaidingParty.h
*/

#ifndef UNITRAIDINGPARTY_H
#define UNITRAIDINGPARTY_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitRaidingParty : public NounUnit
{
public:
	DECLARE_WIDGET_CLASS();

	// Noun interface
	float baseSignature() const
	{
		return 0.25f;
	}

	// NounUnit interface
	int maxDamage() const
	{
		return 300;
	}
	int attackRange() const
	{
		return 1;
	}
	int attackDamage() const
	{
		return 125;
	}
	dword damageType() const
	{
		return DAMAGE_KINETIC|DAMAGE_ENERGY;
	}
	int healAmount() const
	{
		return 20;
	}
	float sensor() const
	{
		return 250.0f;
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
		return true;
	}
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
