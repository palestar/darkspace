/*
	StructureLab.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURELAB_H
#define STRUCTURELAB_H

#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureLab : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureLab()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 0;
	}

	virtual int groupId() const
	{
		return 3;
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
		return 75;
	}

	int buildCost() const
	{
		return 700;
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
		return -3;
	}

	int research() const
	{
		return 10;
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
