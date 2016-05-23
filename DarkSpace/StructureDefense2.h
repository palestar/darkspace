/*
	StructureDefense2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSE2_H
#define STRUCTUREDEFENSE2_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefense2 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefense2()
	{}

	// NounStructure interface
	int maxDamage() const
	{
		return 17000;
	}

	int buildTechnology() const
	{
		return 60;
	}

	int buildTime() const
	{
		return 180;
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
		return 2;
	}

	int power() const
	{
		return -15;
	}

	int technology() const
	{
		return 60;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
