/*
	StructureDepot.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEPOT_H
#define STRUCTUREDEPOT_H

#include "NounPlanet.h"
#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDepot : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureDepot()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 0;
	}

	virtual int groupId() const
	{
		return 5;
	}

	int maxDamage() const
	{
		return 15120;
	}

	int buildTechnology() const
	{
		return 10;
	}

	int buildTime() const
	{
		return 60;
	}

	int buildCost() const
	{
		return 200;
	}

	int repairRate() const
	{
		return 300;
	}

	Color color() const
	{
		return SKYBLUE;
	}

	dword planetFlags() const
	{
		return NounPlanet::FLAG_HAS_DEPOT;
	}

	int workers() const
	{
		return 1;
	}

	int power() const
	{
		return -5;
	}

	int technology() const
	{
		return 0;
	}
	virtual int defense() const
	{
		return 0;
	}
	virtual float protection() const
	{
		return 0.0f;		
	}
	bool canBuild( NounPlanet * pPlanet, bool bUpgrade ) const
	{
		if (! validateNoun( pPlanet ) )
			return false;
		for(int i=0;i<pPlanet->childCount();i++)
			if ( WidgetCast<StructureDepot>( pPlanet->child(i) ) != NULL )
				return false;

		return NounStructure::canBuild( pPlanet, bUpgrade );
	}	
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
