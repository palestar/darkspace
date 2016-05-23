/*
	StructureBarracks.h

	This structure is used to train better/more troops
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREBARRACKS_H
#define STRUCTUREBARRACKS_H

#include "NounStructure.h"
#include "NounUnit.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureBarracks : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< StructureBarracks >		wRef;

	// Construction
	StructureBarracks();

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

	int					workers() const;
	int					power() const;
	int					technology() const;

	CharString			status() const;						// status string
	
	// Accessors
	NounUnit *			building() const;					// what kind of unit is being built
	int					eta() const;						// eta until next unit is finished in seconds

	bool				canSetBuild( NounUnit * pUnit ) const;
	bool				canBuild( NounUnit * pUnit ) const;

	// Mutators
	void				setBuild( NounUnit * pUnit );		// set the type of unit to be built

	// Helpers
	void				loadUnits();
	void				getUnits( Array< NounUnit * > & units ) const;

protected:
	// Data
	NounUnit::wRef		m_pBuilding;						// which unit is being built
	int					m_BuildTime;						// how much of the device has been built

	// non-seriliaze
	dword				m_nBuildTick;
	Scene::Ref			m_pBuild;
};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
