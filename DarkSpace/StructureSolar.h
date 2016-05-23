/*
	StructureSolar.h
	Solar power plant
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTURESOLAR_H
#define STRUCTURESOLAR_H

#include "StructurePower.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureSolar : public StructurePower
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< StructureSolar >	wRef;

	// Construction
	StructureSolar();

	// Noun interface
	void				simulate( dword nTick );

	// NounStructure interface
	virtual int			sortId() const;
	virtual int			groupId() const;
	int					maxDamage() const;					// how much damage can this structure take
	int					buildTechnology() const;			// technology level required to build
	int					buildTime() const;					// time in seconds to build this structure
	int					buildCost() const;					// cost to build this structure
	int					repairRate() const;					// damaged repaired per second

	Color				color() const;

	int					workers() const;
	int					power() const;
	int					technology() const;

	bool canUpgrade( NounStructure * pStructure ) const
	{
		if ( WidgetCast<StructurePower>( pStructure ) )
			return true;
		return false;
	}

private:
	// Data
	int					m_Power;
	dword				m_nUpdateTick;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
