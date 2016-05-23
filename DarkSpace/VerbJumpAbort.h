/*
	VerbJumpAbort.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef VERBJUMPABORT_H
#define VERBJUMPABORT_H

#include "NounShip.h"
#include "World/Verb.h"
#include "GameDll.h"



//-------------------------------------------------------------------------------

class DLL VerbJumpAbort : public Verb
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	VerbJumpAbort();
	VerbJumpAbort( NounShip * pShip );

	// Verb Interface
	Priority				priority() const;	// priority of this verb
	Scope					scope() const;
	bool					client() const;		// is this a client verb, meaning okay to receive from a client
	bool					queue() const;

	bool					canAttach( Noun * pNoun );
	void					onExecute();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
