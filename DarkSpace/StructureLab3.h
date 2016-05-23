/*
	StructureLab3.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURELAB3_H
#define STRUCTURELAB3_H

#include "StructureLab2.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureLab3 : public StructureLab2
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureLab3()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 2;
	}

	virtual int groupId() const
	{
		return 3;
	}

	int maxDamage() const
	{
		return 22680;
	}

	int buildTechnology() const
	{
		return 85;
	}

	int buildTime() const
	{
		return 300;
	}

	int buildCost() const
	{
		return 3700;
	}

	int repairRate() const
	{
		return 250;
	}

	int workers() const
	{
		return 3;
	}

	int power() const
	{
		return -12;
	}

	int research() const
	{
		return 20;
	}

	int technology() const
	{
		return 0;
	}
	bool canUpgrade( NounStructure * pStructure ) const
	{
		if ( WidgetCast<StructureLab>( pStructure ) )
			return true;
		return false;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
