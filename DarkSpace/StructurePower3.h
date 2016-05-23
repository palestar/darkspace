/*
	StructurePower3.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREPOWER3_H
#define STRUCTUREPOWER3_H

#include "StructurePower2.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructurePower3 : public StructurePower2
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructurePower3 >	wRef;

	// Construction
	StructurePower3()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 3;
	}

	virtual int groupId() const
	{
		return 4;
	}

	int maxDamage() const
	{
		return 22680;
	}

	int buildTechnology() const
	{
		return 80;
	}

	int	buildTime() const
	{
		return 60;
	}

	int	buildCost() const
	{
		return 1800;
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
		return 5;
	}

	int	power() const
	{
		return 50 * getTechPercentage();
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
