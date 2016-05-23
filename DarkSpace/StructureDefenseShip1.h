/*
	StructureDefenseShip1.h
	(c)2009 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSESHIP1_H
#define STRUCTUREDEFENSESHIP1_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefenseShip1 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefenseShip1()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 3;
	}

	virtual int groupId() const
	{
		return 6;
	}

	int maxDamage() const
	{
		return 22680;
	}

	int buildTechnology() const
	{
		return 35;
	}

	int buildTime() const
	{
		return 200;
	}

	int buildCost() const
	{
		return 2000;
	}

	int repairRate() const
	{
		return 600;
	}

	int workers() const
	{
		return 3;
	}

	int power() const
	{
		return -20;
	}

	int technology() const
	{
		return 35;
	}
	virtual int defense() const
	{
		return 1050 * getTechPercentage();
	}
	virtual float protection() const
	{
		return 0.01f * getTechPercentage();	
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
