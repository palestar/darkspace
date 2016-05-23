/*
	VerbUseSingleGadget.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBUSESINGLEGADGET_H
#define VERBUSESINGLEGADGET_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL VerbUseSingleGadget : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUseSingleGadget();
	VerbUseSingleGadget( NounGadget * pGadget, Noun * pTarget, bool shift = false );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	Noun::tRef				m_pTarget;			// use on what
	bool					m_bShift;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
