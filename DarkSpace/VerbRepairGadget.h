/*
	VerbRepairGadget.h
	(c)1999 Palestar, Richard Lyle
*/

#ifndef VERB_REPAIR_GADGET_H
#define VERB_REPAIR_GADGET_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbRepairGadget : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef NounShip::When		When;

	// Construction
	VerbRepairGadget();
	VerbRepairGadget( NounGadget * pGadget, When when );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	When					m_When;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
