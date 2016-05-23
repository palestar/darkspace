/*
	StructureDefense3.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSE3_H
#define STRUCTUREDEFENSE3_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefense3 : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDefense3()
	{}

	// NounStructure interface
	int maxDamage() const
	{
		return 19000;
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
		return 3;
	}

	int power() const
	{
		return -20;
	}

	int technology() const
	{
		return 90;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
