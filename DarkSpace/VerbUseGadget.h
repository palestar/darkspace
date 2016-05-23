/*
	VerbUseGadget.h

	This verb calls the use() mutator on a gadget object
	(c)1999 Palestar, Richard Lyle
*/

#ifndef VERB_USE_GADGET_H
#define VERB_USE_GADGET_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbUseGadget : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUseGadget();
	VerbUseGadget( Noun * pUser, const Array< NounGadget *> & a_Gadgets, 
		Noun * pTarget, bool shift = false );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	qword					m_Gadgets;			// bit array of active gadgets
	Noun::tRef				m_Target;			// use on what
	bool					m_Shift;			// secondary use flag
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
