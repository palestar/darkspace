/*
	StructureDefenseMix1.h
	(c)2009 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSEMIX1_H
#define STRUCTUREDEFENSEMIX1_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefenseMix1 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefenseMix1()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 2;
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
		return 25;
	}

	int buildTime() const
	{
		return 120;
	}

	int buildCost() const
	{
		return 1000;
	}

	int repairRate() const
	{
		return 600;
	}

	int workers() const
	{
		return 2;
	}

	int power() const
	{
		return -10;
	}

	int technology() const
	{
		return 25;
	}
	virtual int defense() const
	{
		return 800 * getTechPercentage();
	}
	virtual float protection() const
	{
		return 0.02f * getTechPercentage();		
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
