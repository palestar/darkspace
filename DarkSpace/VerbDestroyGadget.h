/*
	VerbDestroyGadget.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBDESTROYGADGET_H
#define VERBDESTROYGADGET_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//-------------------------------------------------------------------------------

class DLL VerbDestroyGadget : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbDestroyGadget();
	VerbDestroyGadget( NounGadget * pGadget, Noun * pFrom );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	Noun::wRef				m_pFrom;
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
