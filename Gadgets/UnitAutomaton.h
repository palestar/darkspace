/*
	UnitAutomaton.h
	(c)2012 Palestar Inc, Eledore Massis.
*/

#ifndef UNITAUTOMATON_H
#define UNITAUTOMATON_H

#include "DarkSpace/NounUnit.h"



//----------------------------------------------------------------------------

class UnitAutomaton : public NounUnit
{
public:
	DECLARE_WIDGET_CLASS();

	// NounUnit interface
	int maxDamage() const
	{
		return 500;
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
		return DAMAGE_KINETIC|DAMAGE_BIO;
	}
	int healAmount() const
	{
		return 35;
	}
	float sensor() const
	{
		return 1000.0f;
	}
	float view() const
	{
		return 25.0f;
	}
	int buildTime() const
	{
		return 60 * 5;
	}
	int buildTechnology() const
	{
		return 30;
	}
	int	buildCost() const
	{
		return 200;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	bool shipCaptureBonus() const
	{
		return false;
	}
	bool cannotBoardShip() const
	{
		return false;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
