/*
	StructureMine3.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREMINE3_H
#define STRUCTUREMINE3_H

#include "StructureMine2.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureMine3 : public StructureMine2
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureMine3()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 2;
	}

	virtual int groupId() const
	{
		return 2;
	}

	int maxDamage() const
	{
		return 22680;
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
		return 2750;
	}

	int repairRate() const
	{
		return 300;
	}

	int workers() const
	{
		return 2;
	}

	int power() const
	{
		return -10;
	}

	int mining() const
	{
		return 12 * getTechPercentage();
	}
	int technology() const
	{
		return 70;
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
