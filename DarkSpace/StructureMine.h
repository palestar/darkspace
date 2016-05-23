/*
	StructureMine.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREMINE_H
#define STRUCTUREMINE_H

#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureMine : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureMine()
	{}
	
	// NounStructure interface
	virtual int	sortId() const
	{
		return 0;
	}

	virtual int groupId() const
	{
		return 2;
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
		return 25;
	}

	int buildCost() const
	{
		return 75;
	}

	int repairRate() const
	{
		return 300;
	}

	int workers() const
	{
		return 1;
	}

	int power() const
	{
		return -1;
	}

	int mining() const
	{
		return 5 * getTechPercentage();
	}

	int technology() const
	{
		return 0;
	}
};



#endif

//----------------------------------------------------------------------------
//EOF
