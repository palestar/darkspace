/*
	StructureFarm2.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREFARM2_H
#define STRUCTUREFARM2_H

#include "StructureFarm.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureFarm2 : public StructureFarm
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureFarm2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
	}

	virtual int groupId() const
	{
		return 1;
	}

	int maxDamage() const
	{
		return 30240;
	}

	int buildTechnology() const
	{
		return 20;
	}

	int buildTime() const
	{
		return 30;
	}

	int buildCost() const
	{
		return 350;
	}

	int repairRate() const
	{
		return 350;
	}

	int workers() const
	{
		return 0;
	}

	int power() const
	{
		return -2;
	}

	int food() const
	{
		return 20 * getTechPercentage();
	}

	int technology() const
	{
		return 0;
	}
	bool canUpgrade( NounStructure * pStructure ) const
	{
		if ( WidgetCast<StructureFarm>( pStructure ) )
			return true;
		return false;
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
