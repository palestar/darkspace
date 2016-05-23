/*
	StructureShield.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURESHIELD_H
#define STRUCTURESHIELD_H

#include "StructureDefense.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureShield : public StructureDefense
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	StructureShield()
	{}

	// NounStructure interface
	virtual int	sortId() const
	{
		return 3;
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
		return 20;
	}

	int buildTime() const
	{
		return 120;
	}

	int buildCost() const
	{
		return 1500;
	}

	int repairRate() const
	{
		return 200;
	}

	Color color() const
	{
		return CYAN;
	}

	int workers() const
	{
		return 1;
	}

	int power() const
	{
		return -20;
	}

	int technology() const
	{
		return 20;
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
			if ( WidgetCast<StructureShield>( pPlanet->child(i) ) != NULL )
				return false;

		return NounStructure::canBuild( pPlanet, bUpgrade );
	}
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
