/*
	StructureLab2.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURELAB2_H
#define STRUCTURELAB2_H

#include "StructureLab.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureLab2 : public StructureLab
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureLab2()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 1;
	}

	virtual int groupId() const
	{
		return 3;
	}

	int maxDamage() const
	{
		return 30240;
	}

	int buildTechnology() const
	{
		return 55;
	}

	int buildTime() const
	{
		return 150;
	}

	int buildCost() const
	{
		return 1500;
	}

	int repairRate() const
	{
		return 250;
	}

	int workers() const
	{
		return 2;
	}

	int power() const
	{
		return -6;
	}

	int research() const
	{
		return 15;
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
