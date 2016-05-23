/*
	StructureDefensePD1.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSEPD1_H
#define STRUCTUREDEFENSEPD1_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefensePD1 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefensePD1()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
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
		return 20;
	}

	int buildTime() const
	{
		return 100;
	}

	int buildCost() const
	{
		return 800;
	}

	int repairRate() const
	{
		return 600;
	}

	int workers() const
	{
		return 1;
	}

	int power() const
	{
		return -10;
	}

	int technology() const
	{
		return 20;
	}
	virtual int defense() const
	{
		return 350 * getTechPercentage();
	}
	virtual float protection() const
	{
		return 0.04f * getTechPercentage();		// 4%
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
