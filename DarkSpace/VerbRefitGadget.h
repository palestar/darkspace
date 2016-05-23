/*
	VerbRefitGadget.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBREFITGADGET_H
#define VERBREFITGADGET_H

#include "NounGadget.h"
#include "CargoGadget.h"
#include "World/Verb.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbRefitGadget : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbRefitGadget();
	VerbRefitGadget( NounGadget * pGadget, NounGadget * pNewGadget );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Helpers
	static bool				canRefit( NounGadget * pGadget, NounGadget * pNewGadget );

	// Data
	NounGadget::wRef		m_pNewGadget;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

