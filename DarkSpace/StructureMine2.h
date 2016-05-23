/*
	StructureMine2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREMINE2_H
#define STRUCTUREMINE2_H

#include "StructureMine.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureMine2 : public StructureMine
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureMine2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
	}

	virtual int groupId() const
	{
		return 2;
	}

	int maxDamage() const
	{
		return 30240;
	}

	int buildTechnology() const
	{
		return 30;
	}

	int buildTime() const
	{
		return 50;
	}

	int buildCost() const
	{
		return 700;
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
		return -5;
	}

	int mining() const
	{
		return 9 * getTechPercentage();
	}

	int technology() const
	{
		return 30;
	}
	bool canUpgrade( NounStructure * pStructure ) const
	{
		if ( WidgetCast<StructureMine>( pStructure ) )
			return true;
		return false;
	}
};



#endif

//----------------------------------------------------------------------------
//EOF
