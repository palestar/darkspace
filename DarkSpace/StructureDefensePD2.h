/*
	StructureDefensePD2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSEPD2_H
#define STRUCTUREDEFENSEPD2_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefensePD2 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefensePD2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 4;
	}

	virtual int groupId() const
	{
		return 6;
	}

	int maxDamage() const
	{
		return 30240;
	}

	int buildTechnology() const
	{
		return 70;
	}

	int buildTime() const
	{
		return 240;
	}

	int buildCost() const
	{
		return 2500;
	}

	int repairRate() const
	{
		return 600;
	}

	int workers() const
	{
		return 5;
	}

	int power() const
	{
		return -20;
	}

	int technology() const
	{
		return 60;
	}
	virtual int defense() const
	{
		return 350 * getTechPercentage();
	}
	virtual float protection()  const
	{
		return 0.06f * getTechPercentage();		// 6%
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
