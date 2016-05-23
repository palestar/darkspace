/*
	VerbUseMode.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBUSEMODE_H
#define VERBUSEMODE_H

#include "NounGadget.h"
#include "World/Verb.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL VerbUseMode : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbUseMode();
	VerbUseMode( NounGadget * pGadget, int mode );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client

	bool					canAttach( Noun * pNoun );
	void					onExecute();

	// Data
	int						m_nMode;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF

