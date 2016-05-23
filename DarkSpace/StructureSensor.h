/*
	StructureSensor.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURESENSOR_H
#define STRUCTURESENSOR_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureSensor : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureSensor()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
	}

	virtual int groupId() const
	{
		return 5;
	}

	int maxDamage() const
	{
		return 15120;
	}

	int buildTechnology() const
	{
		return 10;
	}

	int buildTime() const
	{
		return 30;
	}

	int buildCost() const
	{
		return 250;
	}

	int repairRate() const
	{
		return 200;
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
		return 0;
	}
	virtual int defense() const
	{
		return 0;
	}
	virtual float protection() const
	{
		return 0.0f;		
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
