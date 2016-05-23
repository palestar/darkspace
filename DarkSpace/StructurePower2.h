/*
	StructurePower2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREPOWER2_H
#define STRUCTUREPOWER2_H

#include "StructurePower.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructurePower2 : public StructurePower
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructurePower2 >	wRef;

	// Construction
	StructurePower2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 2;
	}

	virtual int groupId() const
	{
		return 4;
	}

	int maxDamage() const
	{
		return 30240;
	}

	int buildTechnology() const
	{
		return 40;
	}

	int	buildTime() const
	{
		return 40;
	}

	int	buildCost() const
	{
		return 450;
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
		return 3;
	}

	int	power() const
	{
		return 40 * getTechPercentage();
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
