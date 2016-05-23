/*
	StructurePower.h
	Power plant for planetary defenses
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREPOWER_H
#define STRUCTUREPOWER_H

#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructurePower : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructurePower >	wRef;

	// Construction
	StructurePower()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 0;
	}

	virtual int groupId() const
	{
		return 4;
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
		return 20;
	}

	int	buildCost() const
	{
		return 50;
	}

	int	repairRate() const
	{
		return 200;
	}

	Color color() const
	{
		return BLUE;
	}

	int	workers() const
	{
		return 1;
	}

	int	power()  const
	{
		return 30 * getTechPercentage();
	}
	int technology() const
	{
		return 0;
	}
	bool canUpgrade( NounStructure * pStructure ) const
	{
		if ( WidgetCast<StructurePower>( pStructure ) )
			return true;
		return false;
	}	
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
