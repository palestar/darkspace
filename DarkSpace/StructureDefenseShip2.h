/*
	StructureDefenseShip2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSESHIP2_H
#define STRUCTUREDEFENSESHIP2_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefenseShip2 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefenseShip2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 6;
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
		return 90;
	}

	int buildTime() const
	{
		return 360;
	}

	int buildCost() const
	{
		return 8000;
	}

	int repairRate() const
	{
		return 600;
	}

	int workers() const
	{
		return 6;
	}

	int power() const
	{
		return -25;
	}

	int technology() const
	{
		return 80;
	}
	virtual int defense() const
	{
		return 1250 * getTechPercentage();
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
