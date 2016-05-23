/*
	StructureFarm3.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREFARM3_H
#define STRUCTUREFARM3_H

#include "StructureFarm2.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureFarm3 : public StructureFarm2
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureFarm3()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 2;
	}

	virtual int groupId() const
	{
		return 1;
	}

	int maxDamage() const
	{
		return 22680;
	}

	int buildTechnology() const
	{
		return 40;
	}

	int buildTime() const
	{
		return 60;
	}

	int buildCost() const
	{
		return 1900;
	}

	int repairRate() const
	{
		return 350;
	}

	int workers() const
	{
		return 2;
	}

	int power() const
	{
		return -5;
	}

	int food() const
	{
		return 30 * getTechPercentage();
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
