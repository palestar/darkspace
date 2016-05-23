/*
	StructureInterdictor.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREINTERDICTOR_H
#define STRUCTUREINTERDICTOR_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureInterdictor : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureInterdictor()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 4;
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
		return 70;
	}

	int buildTime() const
	{
		return 100;
	}

	int buildCost() const
	{
		return 800;
	}

	dword buildFlags() const
	{
		return 0;
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
		return -20;
	}

	int technology() const
	{
		return 70;
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
