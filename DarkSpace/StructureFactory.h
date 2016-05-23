/*
	StructureFactory.h

	This structure increases industrial output
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREFACTORY_H
#define STRUCTUREFACTORY_H

#include "NounStructure.h"
#include "NounGadget.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureFactory : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef WidgetReference< StructureFactory >		wRef;

	// Construction
	StructureFactory();

	// NounStructure interface
	virtual int			sortId() const;
	virtual int			groupId() const;
	int					maxDamage() const;					// how much damage can this structure take
	int					buildTechnology() const;			// technology level required to build
	int					buildTime() const;					// time in seconds to build this structure
	int					buildCost() const;					// cost to build this structure
	int					repairRate() const;					// damaged repaired

	int					workers() const;					// workers needed
	int					power() const;						// power required/generated
	int					production() const;				// production added by this structure
	int					technology() const;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
