/*
	StructureDome.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDOME_H
#define STRUCTUREDOME_H

#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDome : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructureDome >	wRef;

	// Construction
	StructureDome()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
	}

	virtual int groupId() const
	{
		return 0;
	}

	int maxDamage() const
	{
		return 37800;
	}

	int buildTechnology() const
	{
		return 0;
	}

	int	buildTime() const
	{
		return 25;
	}

	int	buildCost() const
	{
		return 100;
	}

	int	repairRate() const
	{
		return 400;
	}

	int	workers() const
	{
		return 0;
	}

	int	power() const
	{
		return -1;
	}

	int habitat() const
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
