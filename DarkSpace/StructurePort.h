/*
	StructurePort.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREPORT_H
#define STRUCTUREPORT_H

#include "NounStructure.h"
#include "NounShip.h"
#include "CargoResource.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructurePort : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< StructurePort >	wRef;

	// Construction
	StructurePort();

	// BaseNode interface
	void				onDetaching();
	// Noun interface
	void				initialize();
	void				simulate( dword nTick );

	// NounStructure interface
	virtual int			sortId() const;
	virtual int			groupId() const;
	int					maxDamage() const;					// how much damage can this structure take
	int					buildTechnology() const;			// technology level required to build
	int					buildTime() const;					// time in seconds to build this structure
	int					buildCost() const;					// cost to build this structure
	int					repairRate() const;					// damaged repaired per second
	int					technology() const;

	Color				color() const;

	bool				canBuild( NounPlanet * pPlanet, bool bUpgrade ) const;
	dword				planetFlags() const;
	int					workers() const;
	int					power() const;

	bool				usable( NounShip * pShip ) const;
	CharString			useTip( NounShip * pShip ) const;
	void				use( dword nWhen, NounShip * pShip );

	// Helpers
	CargoResource *		findCargoResource();

	// Static
	static StructurePort *		
						findPort( NounPlanet * pPlanet, bool bCheckActive = true );
	static CargoResource *
						findCargoResource( NounPlanet * pPlanet );


protected:
	// Data
	Array< NounShip::wRef >
						m_pTransports;
	// non-serialized
	int					m_nNextRoute;
	dword				m_nPortTick;
	dword				m_nLastSpawn;

	// Mutators
	void				updateCargo();
	void				updateTransports();
	void				transferResources();
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
