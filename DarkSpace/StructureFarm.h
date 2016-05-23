/*
	StructureFarm.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREFARM_H
#define STRUCTUREFARM_H

#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureFarm : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureFarm()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 0;
	}

	virtual int groupId() const
	{
		return 1;
	}

	int maxDamage() const
	{
		return 37800;
	}

	int buildTechnology() const
	{
		return 0;
	}

	int buildTime() const
	{
		return 15;
	}

	int buildCost() const
	{
		return 50;
	}

	int repairRate() const
	{
		return 250;
	}

	int workers() const
	{
		return 1;
	}

	int power() const
	{
		return -1;
	}

	int food() const
	{
		return 10 * getTechPercentage();
	}

	int technology() const
	{
		return 0;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

