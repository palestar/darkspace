/*
	StructureDefenseMix2.h
	(c)2009 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSEMIX2_H
#define STRUCTUREDEFENSEMIX2_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefenseMix2 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefenseMix2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 5;
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
		return 80;
	}

	int buildTime() const
	{
		return 210;
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
		return 4;
	}

	int power() const
	{
		return -15;
	}

	int technology() const
	{
		return 70;
	}
	virtual int defense() const
	{
		return 950 * getTechPercentage();
	}
	virtual float protection() const
	{
		return 0.04f * getTechPercentage();		
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
